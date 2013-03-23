#pragma once
//+------------------------------------------------------------------+
//| custom control for internet links                                |
//+------------------------------------------------------------------+
class CLink : public CStatic
{

public:
   CLink();

private:
   BOOL    m_PointerOnWnd;
   CFont   m_font;
   CBrush  m_bck_brush;

private:
   void    UnderlineText(BOOL underline);

protected:
   virtual void DoDataExchange(CDataExchange* pDX);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

protected:
   DECLARE_MESSAGE_MAP()
};
//+------------------------------------------------------------------+