//---------------------------------------------------------------------------

#ifndef OptionDlgH
#define OptionDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <PNGImage.hpp>

#include "HotkeyInfo.h"
#include "HotkeyPanel.h"
#include <Graphics.hpp>

extern String g_sBuildDate;

/////////////////////////////////////////////////////////////////////////////
class TToolOptionsDialog : public TForm
{
__published:  // IDE-managed Components
    TButton *bnCancel;
    TButton *bnOk;
    TCheckBox *ckBinary;
    TCheckBox *ckOnTop;
    TCheckBox *ckRememberSettings;
    TCheckBox *ckAutoStart;
    TEdit *edLength;
    TEdit *edRefresh;
    TGroupBox *gbBaseConv;
    TGroupBox *gbGeneral;
    TGroupBox *gbGrab;
    TGroupBox *gbInfo;
    TGroupBox *gbLoupe;
    TGroupBox *gbRef;
    TGroupBox *gbRuler;
    TLabel *lbRefreshrate;
    TLabel *lbMilliseconds;
    TLabel *lbLength;
    TListView *lvOptionSelector;
    TPanel *plBaseconv;
    TPanel *plGeneral;
    TPanel *plGrabber;
    TPanel *plInfo;
    TPanel *plLoupe;
    TPanel *plRef;
    TPanel *plRuler;
    TPanel *plAbout;
    TUpDown *udLength;
    TUpDown *udRefresh;
    THotkeyPanel *hkpZoomIn;
    THotkeyPanel *hkpZoomOut;
    THotkeyPanel *hkpColorCopy;
    THotkeyPanel *hkpGrabScreen;
    TLabel *lbPixels;
    TCheckBox *ckNudgeRuler;
    TGroupBox *gbAutoSave;
    TButton *bnAutosaveOptions;
    TPanel *plMarker;
    TCheckBox *ckTransparent;
    TLabel *lbTransparency;
    TEdit *edTransparency;
    TUpDown *udTransparency;
    TLabel *lbPercent;
    TCheckBox *ckTrayApp;
    TPanel *plTrayicon;
    TGroupBox *gbTrayicon;
    TGroupBox *gbDoubleClick;
    TCheckBox *ckOpenRuler;
    TCheckBox *ckOpenLoupe;
    TCheckBox *ckOpenToolbar;
    TCheckBox *ckGrabScreen;
    THotkeyPanel *hkpDoubleClick;
    TCheckBox *ckSingleton;
    TRadioButton *rbInifile;
    TRadioButton *rbRegistry;
    TCheckBox *ckSingleClick;
    TGroupBox *gbPreset;
    TButton *bnManagePresets;
    TRadioButton *rbStandard;
    TRadioButton *rbCustomCopy;
    TCheckBox *ckQuotes;
    TCheckBox *ckPrefix;
    TEdit *edTemplate;
    TButton *bnEditTemplate;
    TGroupBox *gbAbout;
    TLabel *lbVersion;
    TLabel *lbCopy;
    TLabel *lbUrl;
    TImage *Logo;
    TLabel *lbHomepage;
    void __fastcall lvOptionSelectorChange(TObject *Sender, TListItem *Item, TItemChange Change);
    void __fastcall bnOkClick(TObject *Sender);
    void __fastcall bnAutosaveOptionsClick(TObject *Sender);
    void __fastcall ckRememberSettingsClick(TObject *Sender);
    void __fastcall ckTransparentClick(TObject *Sender);
    void __fastcall bnManagePresetsClick(TObject *Sender);
    void __fastcall rbCustomCopyClick(TObject *Sender);
    void __fastcall bnEditTemplateClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall lbUrlClick(TObject *Sender);
    void __fastcall lbHomepageClick(TObject *Sender);

private:  // User declarations
    String m_sActivePage;
    String m_sHomePageUrl;
    String m_sMailtoUrl;
    
    void HideAll();
    void ActivatePage(const String sActive);
    void ActivatePage(int PageIndex);

    void InitOptions();
    void InitListView();
    void InitHotkeyPanels();
    void SaveOptions();
    void SaveHotkeyPanels();

    THotkeyInfo LoadHotkeyInfo(const String& sHotKeyName);
    void SaveHotkeyInfo(const String& sHotKeyName, const THotkeyInfo& HotkeyInfo);
    String __fastcall GetVersionString(void);

public:   // User declarations
    __fastcall TToolOptionsDialog(TComponent* Owner, const String& sPageName);
    __fastcall ~TToolOptionsDialog();
};
//---------------------------------------------------------------------------
//extern PACKAGE TToolOptionsDialog *ToolOptionsDialog;
//---------------------------------------------------------------------------
#endif
