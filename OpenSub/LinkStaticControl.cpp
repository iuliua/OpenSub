#include "stdafx.h"
#include "LinkStaticControl.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CLink::CLink():CStatic(),m_PointerOnWnd(FALSE)
{
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CLink::OnMouseMove(UINT nFlags, CPoint point)
{
   SetCapture();             
   CRect wndRect;
   GetWindowRect(&wndRect);
   ScreenToClient(&wndRect);
   if (wndRect.PtInRect(point))	
   {  
      if (m_PointerOnWnd!=TRUE)	
      {
         UnderlineText(1); 
         m_PointerOnWnd=TRUE;
      }
   } 
   else
   {
      UnderlineText(0); 
      ReleaseCapture();
      m_PointerOnWnd=FALSE;
   }
   CWnd::OnMouseMove(nFlags, point);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CLink::UnderlineText(BOOL underline)
{
   LOGFONT lfLogFont;
   GetFont()->GetLogFont(&lfLogFont);
   lfLogFont.lfUnderline = underline;
   m_font.DeleteObject();
   m_font.CreateFontIndirect(&lfLogFont);
   SetFont(&m_font);
   this->RedrawWindow();
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
HBRUSH CLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    m_bck_brush.DeleteObject();
    m_bck_brush.CreateSysColorBrush(COLOR_BTNFACE);
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0,0,234));
    return m_bck_brush;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
afx_msg BOOL CLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
    return TRUE;
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CLink::DoDataExchange(CDataExchange* pDX)
{
    CStatic::DoDataExchange(pDX);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
BEGIN_MESSAGE_MAP(CLink, CStatic)
   ON_WM_MOUSEMOVE()
   ON_WM_CTLCOLOR_REFLECT()
   ON_WM_SETCURSOR()
END_MESSAGE_MAP()
//+------------------------------------------------------------------+

