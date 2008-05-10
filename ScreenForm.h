//---------------------------------------------------------------------------
#ifndef ScreenFormH
#define ScreenFormH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Tool.h"
#include <ComCtrls.hpp>

#ifdef _DEBUG
#define TRACE(s) OutputDebugString(s)
#else
#define TRACE(s)
#endif

#include "tool.h"
//---------------------------------------------------------------------------
class TScreenForm : public TToolForm
{
public:
  __fastcall TScreenForm(TComponent* Owner);
  __fastcall TScreenForm::~TScreenForm();

private:
  int m_MouseOldX;
  int m_MouseOldY;
  bool m_bSticky;
  
protected:
  // This class overrides these two TForm methods
  DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
  DYNAMIC void __fastcall MouseDown(TMouseButton Button,
                                    Classes::TShiftState Shift, int X, int Y);

  BEGIN_MESSAGE_MAP
    VCL_MESSAGE_HANDLER(WM_NCHITTEST, TWMNCHitTest, OnNCHitTest)
  END_MESSAGE_MAP(TForm)

  void __fastcall OnNCHitTest(TWMNCHitTest &Message);


__published:  // IDE-managed Components
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);

__published:
  __property TMouseEvent OnRightButtonClick = { read = FOnRightButtonClick, write = FOnRightButtonClick };

protected:
  TMouseEvent FOnRightButtonClick;

  // Override to show up near mouse pointer
  virtual void __fastcall OnToolShow(TObject *Sender) { FormShow(Sender); }
};

//---------------------------------------------------------------------------
extern PACKAGE TScreenForm *ScreenForm;

//---------------------------------------------------------------------------
#endif
