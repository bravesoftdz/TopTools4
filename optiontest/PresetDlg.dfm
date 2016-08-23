object PresetDialog: TPresetDialog
  Left = 1377
  Top = 131
  Width = 346
  Height = 239
  Caption = 'Grabber Presets'
  Color = clBtnFace
  Constraints.MaxWidth = 346
  Constraints.MinHeight = 120
  Constraints.MinWidth = 346
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 176
    Width = 338
    Height = 36
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object LoadBtn: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Load'
      TabOrder = 0
      OnClick = LoadBtnClick
    end
    object OKBtn: TButton
      Left = 135
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = OKBtnClick
    end
    object CancelBtn: TButton
      Left = 255
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
      OnClick = CancelBtnClick
    end
  end
  object Grid: TStringGrid
    Left = 0
    Top = 0
    Width = 338
    Height = 176
    Align = alClient
    DefaultColWidth = 48
    DefaultRowHeight = 17
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goEditing, goTabs, goAlwaysShowEditor, goThumbTracking]
    TabOrder = 0
    OnGetEditMask = GridGetEditMask
    OnGetEditText = GridGetEditText
    OnSelectCell = GridSelectCell
    OnSetEditText = GridSetEditText
    ColWidths = (
      118
      48
      48
      48
      48)
  end
end
