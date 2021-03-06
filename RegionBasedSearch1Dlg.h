
// RegionBasedSearch1Dlg.h : header file
//

#pragma once
#include"RegionTree.h"

// CRegionBasedSearch1Dlg dialog
class CRegionBasedSearch1Dlg : public CDialogEx
{
// Construction
public:
	CRegionBasedSearch1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGIONBASEDSEARCH1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonrcimg();
	afx_msg void OnBnClickedButtonsegment();

	IplImage* theImage;
	IplImage* ipl;
	void ShowImage(IplImage* img, unsigned int ID);
	void ResizeImage(IplImage* img);

	string imgName;
	int regionCount;
	RegionTree tree;

	CString str_mergeDebug;
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedButtonSearch();
	
	float R_value;
	float G_value;
	float B_value;
	int scrollPos;
	CSliderCtrl layerScroll;
//	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedButton1();
	CSliderCtrl colorBar;
//	afx_msg void OnNMCustomdrawSliderH(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderH(NMHDR *pNMHDR, LRESULT *pResult);
	int colorBarValue;
	afx_msg void OnBnClickedButtonColorsearch();
	void showTargetRegion(IplImage* img, list<Region*> targets);
	afx_msg void OnNMReleasedcaptureSliderH2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl adjColorBar;
	int adjColorBarValue;
//	afx_msg void OnNMCustomdrawSliderH2(NMHDR *pNMHDR, LRESULT *pResult);
};
