//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <inifiles.hpp>

#include "PresetDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPresetDialog *PresetDialog;

//---------------------------------------------------------------------------
__fastcall TPresetDialog::TPresetDialog(TComponent* Owner)
    : TForm(Owner)
{
    m_presetList = new TStringList();
    m_currentRowIndex = 0;
}

//---------------------------------------------------------------------------
__fastcall TPresetDialog::~TPresetDialog()
{
    delete m_presetList;
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::FormCreate(TObject *Sender)
{
    Grid->RowCount = 2;
    Grid->Rows[0]->CommaText = "Description,  X, Y, W, H";
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::LoadBtnClick(TObject *Sender)
{
    LoadFromIniFile("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::OKBtnClick(TObject *Sender)
{
    SaveToIniFile("..\\presets.ini");
}

//---------------------------------------------------------------------------
void __fastcall TPresetDialog::CancelBtnClick(TObject *Sender)
{
    Close();
}

//-------------------------------------------------------------------------
bool __fastcall TPresetDialog::LoadFromIniFile(String const& path)
{
    TIniFile *inifile = new TIniFile(path);
    if (!inifile)
        return false;

    TStringList *presetList = new TStringList;

    String sectionName = "grabber.presets";
    inifile->ReadSection(sectionName, presetList);
    Grid->RowCount = presetList->Count + 1;
    for (int i = 0; i < presetList->Count; i++)
    {
        String optionName = IntToStr(i+1);
        String optionValue = inifile->ReadString(sectionName, optionName, "");
        Grid->Rows[i+1]->CommaText = optionValue;
    }

    delete presetList;

    delete inifile;
    return true;
}

//-------------------------------------------------------------------------
bool __fastcall TPresetDialog::SaveToIniFile(String const& path)
{
    TIniFile *inifile = new TIniFile(path);
    if (!inifile)
        return false;

    String sectionName = "grabber.presets";
    for (int i = 1; i < Grid->RowCount; i++)
    {
        String commaText = Grid->Rows[i]->CommaText;
        // Only add rows with a non-empty description
        if (!Grid->Cells[0][i].IsEmpty())
        {
            String optionName = IntToStr(i);
            inifile->WriteString(sectionName, optionName, commaText);
        }
    }

    delete inifile;
    return true;
}

//---------------------------------------------------------------------------

