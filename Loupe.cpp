//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Loupe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Tool"
#pragma resource "*.dfm"

/////////////////////////////////////////////////////////////////////////////
__fastcall TLoupeForm::TLoupeForm(TComponent* Owner)
: TToolForm(Owner, "loupe"),
  m_pLoupe(NULL),
//  m_RefreshRate(250),
  m_pToolbarImageList(NULL)
{
  const int WindowSize = 200;
  ClientWidth = WindowSize;
  ClientHeight = WindowSize;

  m_pLoupe = new TLoupePanel(this);
  m_pLoupe->Parent = this;
  m_pLoupe->Align = alClient;

  Screen->Cursors[crDragHand] = LoadCursor((void*)HInstance, "DRAGHAND");

  LoadGraphics();

  SetSelfMagnify(m_bMagnifySelf);
}

//---------------------------------------------------------------------------
__fastcall TLoupeForm::~TLoupeForm()
{
  delete m_pLoupe;
  delete m_pToolbarImageList;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormCreate(TObject *Sender)
{
  LoadSettings();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormDestroy(TObject *Sender)
{
  SaveSettings();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoadGraphics()
{
  // For some reason CBuilder (and Delphi for that matter) stores
  // images in the .dfm with the same color-depth as that of
  // the machine the dfm was last saved on.
  // To work around this we load all our bitmaps dynamically from resource.
  m_pToolbarImageList = new TImageList(this);
  Graphics::TBitmap* IconsBmp = new Graphics::TBitmap();
  IconsBmp->LoadFromResourceName((int)HInstance, "LOUPEICONS");
  m_pToolbarImageList->AddMasked(IconsBmp, clFuchsia);
  delete IconsBmp;

  LoupeToolBar->Images = m_pToolbarImageList;
  LoupeMenu->Images = m_pToolbarImageList;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::WndProc(Messages::TMessage &Message)
{
  TToolForm::WndProc(Message);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SetSelfMagnify(bool magnify)
{
  // On Windows 9X systems, the only way to get rid of self-magnification
  // is to black-out the loupe-view when the mouse is above it.
  // Windows 2000 and up support layered (so called 'transparent') windows
  // and we can (ab)use that feature as a means to "hide from ourselves",
  // just by sitting on a layer with 0% transparancy.

  m_bMagnifySelf = magnify;

  if (m_bMagnifySelf)
  {
    // Tell the loupeview to magnify no matter where the mouse is
    m_pLoupe->MagnifySelf = true;
    // On Windows 2000 we also remove the layered attribute
    SetTransparency(false);
  }
  else
  {
    if (TransparencyIsSupported())
    {
      // On Windows 2000 and up we make the window layered
      SetTransparency(true);
      // So we want the loupeview to not block magnification
      m_pLoupe->MagnifySelf = true;
    }
    else
    {
      // On Windows 9X systems we tell the loupeview to
      // black out the diplay when the mouse is above it
      m_pLoupe->MagnifySelf = false;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::Freeze()
{
//  m_pLoupe->ToggleFrozen();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ToggleLock()
{
  m_pLoupe->ToggleLocked();

  if (m_pLoupe->Locked)
    m_pLoupe->Cursor = (TCursor) crDragHand;
  else
    m_pLoupe->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ToggleMagnifySelf()
{
  SetSelfMagnify(!m_bMagnifySelf);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UnLock()
{
  if (m_pLoupe->Locked)
  {
    bnLockPos->Down = false;
    m_pLoupe->ToggleLocked();
    m_pLoupe->Cursor = crDefault;
  }
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UpdateUI()
{
  // Update the state of all UI elements
  Caption = "Loupe " + String(m_pLoupe->Zoom) + "x";

  miCrosshair->Checked = m_pLoupe->CrosshairVisible;
  bnCrosshair->Down = m_pLoupe->CrosshairVisible;
  miGrid->Checked = m_pLoupe->GridVisible;
  bnGrid->Down = m_pLoupe->GridVisible;
  miCenterbox->Checked = m_pLoupe->CenterboxVisible;
  bnCenterbox->Down = m_pLoupe->CenterboxVisible;
  miLockPos->Checked = m_pLoupe->Locked;
  bnLockPos->Down = m_pLoupe->Locked;
  miMagnifySelf->Checked = m_bMagnifySelf;

  // we need both 'lock position" and "freeze view"
//  miFreezeView->Caption = m_pLoupe->Frozen ? "Unfreeze View" : "Freeze View";

  bnZoomOut->Enabled = (m_pLoupe->Zoom > 1);
  miZoomOut->Enabled = (m_pLoupe->Zoom > 1);
  bnZoomIn->Enabled = (m_pLoupe->Zoom < 20);
  miZoomIn->Enabled = (m_pLoupe->Zoom < 20);

  // We don't want a grid at low magnification
//  bnGrid->Enabled = (m_pLoupe->Zoom > 2);
//  miGrid->Enabled = (m_pLoupe->Zoom > 2);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoupeMenuPopup(TObject *Sender)
{
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miZoomInClick(TObject *Sender)
{
  m_pLoupe->ZoomIn();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miZoomOutClick(TObject *Sender)
{
  m_pLoupe->ZoomOut();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miHideClick(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miCrosshairClick(TObject *Sender)
{
  m_pLoupe->ToggleCrosshair();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miGridClick(TObject *Sender)
{
  m_pLoupe->ToggleGrid();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miCenterboxClick(TObject *Sender)
{
  m_pLoupe->ToggleCenterbox();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miFreezeViewClick(TObject *Sender)
{
  Freeze();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miLockPosClick(TObject *Sender)
{
  ToggleLock();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miMagnifySelfClick(TObject *Sender)
{
  ToggleMagnifySelf();
  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::miExitClick(TObject *Sender)
{
  Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  ZoomOut();
  Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  ZoomIn();
  Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoadSettings()
{
//  TToolOptions options("loupe");
//
//  m_pLoupe->Zoom = options.Get("zoom", 4);
//  m_pLoupe->CrosshairVisible = options.Get("crosshair", false);
//  m_pLoupe->CenterboxVisible = options.Get("centerbox", false);
//  m_pLoupe->GridVisible = options.Get("grid", false);
//  m_bMagnifySelf = options.Get("selfmagnify", false);
//  m_pLoupe->RefreshRate = options.Get("refresh", 250);
//  Width = options.Get("width", 200);
//  Height = options.Get("height", 200);
//
//  UpdateUI();

  TLoupeOptions options;

  m_pLoupe->Zoom = options.Zoom;
  m_pLoupe->CrosshairVisible = options.CrosshairVisible;
  m_pLoupe->CenterboxVisible = options.CenterboxVisible;
  m_pLoupe->GridVisible = options.GridVisible;
  m_bMagnifySelf = options.MagnifySelf;
  m_pLoupe->RefreshRate = options.RefreshRate;
  Width = options.Width;
  Height = options.Height;

  UpdateUI();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SaveSettings()
{
//  TToolOptions options("loupe");
//
//  options.Set("centerbox", false);
//  options.Set("crosshair", false);
//  options.Set("grid", false);
//  options.Set("height", 200);
//  options.Set("refresh", 250);
//  options.Set("selfmagnify", false);
//  options.Set("width", 200);
//  options.Set("zoom", 4);
////  options.Set("info", "prefix", false);
////  options.Set("info", "quotes", false);
////  options.Set("capture", "autosave", false);
////  options.Set("capture", "showloupe", false);
//
//  options.Save();

  TLoupeOptions options;

  options.Zoom = m_pLoupe->Zoom;
  options.CrosshairVisible = m_pLoupe->CrosshairVisible;
  options.CenterboxVisible = m_pLoupe->CenterboxVisible;
  options.GridVisible = m_pLoupe->GridVisible;
  options.RefreshRate = m_pLoupe->RefreshRate;
  options.MagnifySelf = m_bMagnifySelf;
  options.Width = Width;
  options.Height = Height;

  options.Save();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::UpdateSettings()
{
  TLoupeOptions options;
  m_pLoupe->RefreshRate = options.RefreshRate;
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  // The arrow-keys can be used to move the loupe focus,
  // if the loupe is locked we move the focuspoint,
  // else we move the mouse-cursor
  POINT ptMouse;
  GetCursorPos(&ptMouse);

  // Default change increment is 1
  int delta = 1;
  // Increment is 10 when shift is pressed
  if (Shift.Contains(ssShift))
    delta = 10;

  switch (Key)
  {
    case VK_LEFT:
      if (m_pLoupe->Locked)
        m_pLoupe->MoveLockPos(-delta, 0);
      else
        SetCursorPos(ptMouse.x - delta, ptMouse.y);
      break;
    case VK_RIGHT:
      if (m_pLoupe->Locked)
        m_pLoupe->MoveLockPos(delta, 0);
      else
        SetCursorPos(ptMouse.x + delta, ptMouse.y);
      break;
    case VK_UP:
      if (m_pLoupe->Locked)
        m_pLoupe->MoveLockPos(0, -delta);
      else
        SetCursorPos(ptMouse.x, ptMouse.y - delta);
      break;
    case VK_DOWN:
      if (m_pLoupe->Locked)
        m_pLoupe->MoveLockPos(0, delta);
      else
        SetCursorPos(ptMouse.x, ptMouse.y + delta);
      break;
  }
//  Refresh();
}


