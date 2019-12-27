#pragma once
#include <afxcmn.h>

/// 具備提示功能的 ListCtrl
class CToolTipListCtrl : public CListCtrl {
protected:
			CMapStringToString		m_ToolTipMap;		//< Map's SubItems to related tooltip text	
			WORD					m_wHitMask;			//< A bit mask of LVHT_* flags the control will show tooltips for
public:
			CDialogEx			*	m_CERNSDlg;
									
									CToolTipListCtrl();	//< Construction
	virtual							~CToolTipListCtrl();

	virtual int						OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	virtual BOOL					DeleteAllItems();
	virtual BOOL					DeleteItem(int nItem);
	virtual BOOL					SetItemText(int nItem, int nSubItem, LPTSTR lpszText);


	//** Additional Functions **
	
			WORD					SetToolTipHitMask(WORD wHitMask);										//< Set the TVHT_* flags that will trigger the display of a tooltip	
	virtual void					DeleteAllToolTips();													//< Clear all tooltips

	virtual BOOL					SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText);	//< Set the tooltip text for a specific SubItem
	virtual CString					GetItemToolTipText(int nItem, int nSubItem);							//< Retrieves the tooltip text for a specific SubItem 

protected:
	// Generated message map functions
	//Respondes to the TTN_NEEDTEXT* messages, provides the text of a tooltip
	virtual afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	virtual afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

};