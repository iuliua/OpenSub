#pragma once

class CLink : public CStatic
{
public:
   CLink();
private:
   BOOL    m_PointerOnWnd;
   CFont   m_font;
   CBrush  m_bck_brush;
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   void    UnderlineText(BOOL underline);
   // Implementation
protected:
   DECLARE_MESSAGE_MAP()

};