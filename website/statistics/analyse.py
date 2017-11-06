#!/usr/bin/env python

import os
import sys
import os.path
import fnmatch
import re
import time
import datetime

#------------------------------------------------------------------------------
def isHtml(file):
    return fnmatch.fnmatch(file,'*.html')

#------------------------------------------------------------------------------
def isZip(file):
    return fnmatch.fnmatch(file,'*.zip')

#------------------------------------------------------------------------------
def hasExtension(file, ext):
    return fnmatch.fnmatch(file,'*.' + ext)

#------------------------------------------------------------------------------
def getPreSections(html):
    """
    Extract all <pre> sections and return their content as a list
    """
    pre = []

    end = 0
    while 1:
        start = html.find('<pre>', end)
        if start < 0:
            break
        end = html.find('</pre>', start)
        pre.append(html[start+6:end])

    return pre

#------------------------------------------------------------------------------
def processDir(arg, path, files):
    for file in filter(isHtml, files):
        processFile(arg, path, file)

#------------------------------------------------------------------------------
def processFile(arg, path, file):
    """
    Fill traffic tables
    """
    fullname = os.path.join(path, file)
    html = open(fullname).read()
    pre_elements = getPreSections(html)

    # first <pre> element contains list of downloads in month
    lines = str(pre_elements[0]).splitlines()
    month = dateFromFilename(file)

    month_downloads[month] = getMonthDownloads(lines, [])

    updateBinaryDownloads(lines)

    # third <pre> element contains downloads per day (in Kb)
    lines = str(pre_elements[2]).splitlines()

    day_traffic.update(getDayTraffic(lines))

#------------------------------------------------------------------------------
def getMonthDownloads(lines, filenames):
    """
    Count number of downloads for some hardcoded
    filenames in month this file is covering
    """
    hits243 = 0
    hits300 = 0
    hits400 = 0
    hits401 = 0
    hits410 = 0

## Setup_TopTools30.exe
## Setup_TopTools40.exe
## Setup_TopTools_401.exe
## Setup_TopTools_410.exe
## TopTools4_00_52.zip
## TopTools4_00_59.zip
## TopTools4_01_71.zip
## TopTools4_01_74.zip
## TopTools4_01_77.zip
## TopTools4_10_26.zip
## TopTools4_10_27.zip
## toptools243.exe

    for line in lines:
        if 'toptools243.exe' in line:
            parts = line.strip().split()
            hits243 = int(parts[3])

        if 'Setup_TopTools30.exe' in line:
            parts = line.strip().split()
            hits300 = int(parts[3])

        if 'TopTools4_00_52.zip' in line:
            parts = line.strip().split()
            hits400 += int(parts[3])

        if 'TopTools4_00_59.zip' in line:
            parts = line.strip().split()
            hits400 += int(parts[3])

        if 'Setup_TopTools40.exe' in line:
            parts = line.strip().split()
            hits400 += int(parts[3])

        if 'TopTools4_01_71.zip' in line:
            parts = line.strip().split()
            hits401 += int(parts[3])

        if 'TopTools4_01_74.zip' in line:
            parts = line.strip().split()
            hits401 += int(parts[3])

        if 'TopTools4_01_77.zip' in line:
            parts = line.strip().split()
            hits401 += int(parts[3])

        if 'Setup_TopTools_401.exe' in line:
            parts = line.strip().split()
            hits401 += int(parts[3])

        if 'TopTools4_10_26.zip' in line:
            parts = line.strip().split()
            hits410 += int(parts[3])

        if 'TopTools4_10_27.zip' in line:
            parts = line.strip().split()
            hits410 += int(parts[3])

        if 'Setup_TopTools_410.exe' in line:
            parts = line.strip().split()
            hits410 += int(parts[3])

    return [hits243, hits300, hits400, hits401, hits410]

#------------------------------------------------------------------------------
def updateBinaryDownloads(lines):
    """
    Retrieve all downloads.
    Maintain count in a dict indexed by filename.
    """
    for line in lines:
        if 'download' in line:
            filename, count = extractFilename(line)
            if bin_downloads.has_key(filename):
                bin_downloads[filename] += int(count)
            else:
                bin_downloads[filename] = 1

#------------------------------------------------------------------------------
def extractFilename(line):
    """
    Extract filename out of a line like this:
    24. <A HREF="http://www.xs4all.nl/~wij/toptools/download/AsciiTab_0.11.zip">http://www.xs4all.nl/~wij/toptools/download/AsciiTab_0.11.zip</A>     2 (186Kb)
    """
    parts = line.strip().split()
    pieces = parts[2].split('download')
    piece = pieces[2][1:-4]

    count = parts[3]
    #print count
    return piece, int(count)

#------------------------------------------------------------------------------
def getDayTraffic(lines):
    """
    Count number of downloads per day
    in month this file is covering
    """
    daytraffic = {}

    for line in lines:
        # format of line:  18 Nov 2009       278  (5423Kb)
        parts = line.strip().split()
        if len(parts) > 3:
            date = convertDate(parts[0] + ' ' + parts[1] + ' ' + parts[2])
            number_of_hits = int(parts[3])
            kbstr = parts[4]
            # convert "(5423Kb)" to "5.423"
            traffic_in_mb = float(kbstr[1:-3]) / 1000
            daytraffic[date] = [number_of_hits, traffic_in_mb]

    return daytraffic

#------------------------------------------------------------------------------
def approxDownloads(kbstr, size=1):
    kbytes = kbstr[1:-3]
    return int(kbytes) / size

#------------------------------------------------------------------------------
def convertDate(rawdate):
    """
    Convert date format; ie. 27 Dec 2009 becomes 2009-12-27
    """
    ts = time.strptime(rawdate, "%d %b %Y")
    return time.strftime("%Y-%m-%d", ts)

#------------------------------------------------------------------------------
def dateFromFilename(filename):
    """
    Convert filename; ie. Dec2009.html becomes 2009-12
    """
    ts = time.strptime(filename, "%b%Y.html")
    return time.strftime("%Y-%m", ts)

#------------------------------------------------------------------------------
def pivotMonths(monthstats):

    t243 = []
    t300 = []
    t400 = []
    t401 = []
    t410 = []

    months = monthstats.keys()
    months.sort()
    for month in months:
        t243.append(monthstats[month][0])
        t300.append(monthstats[month][1])
        t400.append(monthstats[month][2])
        t401.append(monthstats[month][3])
        t410.append(monthstats[month][4])

    generateBluffScript(t243, t300, t400, t401, t410, months)

#------------------------------------------------------------------------------
def pivotDays(daystats):

    traffic = []

    days = daystats.keys()
    days.sort()
    for day in days:
        traffic.append(daystats[day][1])

    print formatBluffData('traffic', traffic)
    print formatBluffGraphLabels(days)

#------------------------------------------------------------------------------
def formatBluffData(label, datalist):
    """
    Reorder data so it can be used by a Bluff graphing script
    ex. g.data('Apples', [1, 2, 3, 4, 4, 3]);
        g.data('Oranges', [4, 8, 7, 9, 8, 9]);
    """
    printstr = "g.data('%s', [%s" % (label, datalist[0])
    for data in datalist[1:]:
        printstr += " ,%s" % (data)
    printstr += "]);"

    return printstr

#------------------------------------------------------------------------------
def formatBluffDataLabels(labellist):
    """
    Arrange labels so they can be used by a
    Bluff graphing script to display tooltips
    ex. g.data_labels = {0: '2003-1', 1: '2003-2', 2: '2003-3'};
    """
    i = 1
    printstr = "g.data_labels = {0: '%s'" % (labellist[0])
    for label in labellist[1:]:
        printstr += ", %d: '%s'" % (i, label)
        i = i + 1
    printstr += "};"

    return printstr

#------------------------------------------------------------------------------
def formatBluffGraphLabels(labellist):
    """
    Arrange labels so they can be used by a
    Bluff graphing script without overlapping
    ex. g.labels = {0: '2003', 2: '2004', 4: '2005'};
    """
    # Define labels centered around july (month 7)
    i = 1
    printstr = "g.labels = {0: '%s'" % (labellist[0][0:4])
    for label in labellist[1:]:
        if label[5:] == '07':
            printstr += ", %d: '%s'" % (i, label[0:4])
        i = i + 1
    printstr += "};"

    return printstr

#------------------------------------------------------------------------------
def generateBluffScript(t243, t300, t400, t401, t410, months):

    template_prefix = """
    function showDownloads()
    {
        var lastupdate = document.getElementById('lastupdate');
        lastupdate.firstChild.nodeValue = 'Last update: %s';

        var g = new Bluff.StackedBar('downloads', '600x400');
        g.title = 'Downloads';
        g.tooltips = true;
        g.set_theme({
                    colors: ['#6886B4', '#72AE6E', '#FDD84E', '#D1695E',
                             '#FF66CC', '#999999', '#3a5b87', 'black'],
                    marker_color: '#aea9a9',
                    font_color: 'white',
                    background_colors: ['#333333', '#333333']
                    });

        g.sort = false;
        g.marker_font_size = 16;
        g.marker_count = 10;
    """ % (datetime.datetime.now().strftime("%a %d %b %Y %H:%M"))

    template_data = formatBluffData('v243', t243) + '\n' + \
                    formatBluffData('v300', t300) + '\n' + \
                    formatBluffData('v400', t400) + '\n' + \
                    formatBluffData('v401', t401) + '\n' + \
                    formatBluffData('v410', t410) + '\n' + \
                    formatBluffDataLabels(months) + '\n' + \
                    formatBluffGraphLabels(months) + '\n'

    template_postfix = """
        g.draw();
    }
    """
    dumpfile = open('stats.js', 'w')
    dumpfile.write(template_prefix + template_data + template_postfix)
    dumpfile.close()


#------------------------------------------------------------------------------
month_downloads = {}
day_traffic = {}
bin_downloads = {}

#------------------------------------------------------------------------------
def print_dict_sorted(dict):
    keys = dict.keys()
    keys.sort()
    for key in keys:
        print key,
        print dict[key]

#------------------------------------------------------------------------------
if __name__ == '__main__':
    folder = os.curdir  #+ '/test'
    os.path.walk(folder, processDir, None)

    pivotMonths(month_downloads)

##     print_dict_sorted(bin_downloads)

##     pivotDays(day_traffic)

##     print_dict_sorted(month_downloads)
##     print_dict_sorted(day_traffic)
