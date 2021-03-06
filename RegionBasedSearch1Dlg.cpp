
// RegionBasedSearch1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegionBasedSearch1.h"
#include "RegionBasedSearch1Dlg.h"
#include "afxdialogex.h"
#include"CvvImage.h"
#include"WatershedAlgorithm.h"
#include"RAG.h"
#include"RMG.h"
#include"RegionTree.h"
#include"PopDia.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRegionBasedSearch1Dlg dialog



CRegionBasedSearch1Dlg::CRegionBasedSearch1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGIONBASEDSEARCH1_DIALOG, pParent)

	, regionCount(0)
	, str_mergeDebug(_T(""))
	, R_value(0)
	, G_value(0)
	, B_value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegionBasedSearch1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, regionCount);
	DDX_Text(pDX, IDC_EDIT2, str_mergeDebug);
	DDX_Text(pDX, IDC_EDIT3, R_value);
	DDX_Text(pDX, IDC_EDIT4, G_value);
	DDX_Text(pDX, IDC_EDIT5, B_value);
	DDX_Control(pDX, IDC_SLIDER1, layerScroll);
	DDX_Control(pDX, IDC_SLIDER_H, colorBar);
	DDX_Control(pDX, IDC_SLIDER_H2, adjColorBar);
}

BEGIN_MESSAGE_MAP(CRegionBasedSearch1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_rcImg, &CRegionBasedSearch1Dlg::OnBnClickedButtonrcimg)
	ON_BN_CLICKED(IDC_BUTTON_segment, &CRegionBasedSearch1Dlg::OnBnClickedButtonsegment)
	ON_EN_CHANGE(IDC_EDIT4, &CRegionBasedSearch1Dlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON_Search, &CRegionBasedSearch1Dlg::OnBnClickedButtonSearch)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CRegionBasedSearch1Dlg::OnNMCustomdrawSlider1)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CRegionBasedSearch1Dlg::OnReleasedcaptureSlider1)


//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_H, &CRegionBasedSearch1Dlg::OnNMCustomdrawSliderH)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_H, &CRegionBasedSearch1Dlg::OnReleasedcaptureSliderH)

ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_H2, &CRegionBasedSearch1Dlg::OnNMReleasedcaptureSliderH2)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_H2, &CRegionBasedSearch1Dlg::OnNMCustomdrawSliderH2)
END_MESSAGE_MAP()


// CRegionBasedSearch1Dlg message handlers

BOOL CRegionBasedSearch1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	theImage = cvCreateImage(cvSize(500, 500), IPL_DEPTH_8U, 3);
	imgName = "satelliteImage";
	colorBar.SetRange(0, 360);
	colorBar.SetPos(0);
	SetDlgItemInt(IDC_EDIT_ColorValue, 0);
	adjColorBar.SetRange(0, 360);
	adjColorBar.SetPos(0);
	SetDlgItemInt(IDC_EDIT_ColorValue2, 0);

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_HUE);
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T("HueInHSV.bmp"),  
		IMAGE_BITMAP,     
		0,          
		0,           
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	 pWnd = (CStatic*)GetDlgItem(IDC_STATIC_HUE2);
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T("HueInHSV.bmp"),
		IMAGE_BITMAP,
		0,
		0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegionBasedSearch1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegionBasedSearch1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegionBasedSearch1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRegionBasedSearch1Dlg::OnBnClickedButtonrcimg()
{
	// TODO: Add your control notification handler code here
	 ipl = cvLoadImage("satelliteImage.jpg");
	if (NULL == ipl) { 
		INT_PTR nRes;
		PopDia popDialog;
		nRes = popDialog.DoModal();
		if (IDCANCEL != nRes)
		return;
	}


	if (theImage)
		cvZero(theImage);
	ResizeImage(ipl);
	ShowImage(theImage, IDC_STATIC);
	//cvReleaseImage(&ipl);
}


void CRegionBasedSearch1Dlg::OnBnClickedButtonsegment()
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	WatershedAlgorithm myWatershed;
	myWatershed.run(ipl,imgName);
	ShowImage(myWatershed.pWS, IDC_STATIC);


	//合并
	RAG* pRAG = NULL;
	RAG* preRAG = NULL;
	RMG* pRMG = NULL;
	RMG* preRMG = NULL;
	IplImage* labelmat;
	labelmat = myWatershed.labelMat;



	int threshold = 5;
	
	 pRAG = new RAG(labelmat, ipl, imgName);
	tree.addBottomLayer(&pRAG->regionMap);
	pRMG = new RMG(&pRAG->regionMap, labelmat, threshold, myWatershed.curlab);
	map<int, vector<Region>> mergeMap = pRMG->binaryMergeMap;
	preRAG = pRAG;
	preRMG = pRMG;
	while (pRAG->regionCount > regionCount) {


		while (pRMG->binaryMergeMap.size() > 10) {

			pRAG = new RAG(pRMG->newlabelMat,ipl, imgName);
			pRMG = new RMG(&pRAG->regionMap, labelmat, threshold, pRMG->maxLabel);
			tree.addLayer(&pRAG->regionMap, preRMG->binaryMergeMap);
			ShowImage(pRAG->RegionImg, IDC_STATIC);

			delete preRAG;
			delete preRMG;
			preRAG = pRAG;
			preRMG = pRMG;
			
		}
		threshold += 5;
		pRAG = new RAG(pRMG->newlabelMat, ipl, imgName);
		pRMG = new RMG(&pRAG->regionMap, labelmat, threshold, pRMG->maxLabel);
		ShowImage(pRAG->RegionImg, IDC_STATIC);
		tree.addLayer(&pRAG->regionMap, preRMG->binaryMergeMap);
		int maxLayer = pRAG->layer;
		layerScroll.SetRange(0, maxLayer-1);
		layerScroll.SetPos(maxLayer-1);
		SetDlgItemInt(IDC_EDIT2, maxLayer);

		delete preRAG;
		delete preRMG;
		preRAG = pRAG;
		preRMG = pRMG;
	
	}


	
}


void CRegionBasedSearch1Dlg::ResizeImage(IplImage* img) {
	int w = img->width;
	int h = img->height;

	int max = (w > h) ? w : h;

	//按比例因子缩放
	float scale = (float)((float)max / 500.0f);

	int nw = (int)(w / scale);
	int nh = (int)(h / scale);

	//将缩放后的图存入theImage的正中部位，计算左上角的坐标值
	int tlx = (nw > nh) ? 0 : (int)(500 - nw) / 2;
	int tly = (nw > nh) ? (int)(500 - nh) / 2 : 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI(theImage, cvRect(tlx, tly, nw, nh));

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, theImage);
	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(theImage);
}

void CRegionBasedSearch1Dlg::ShowImage(IplImage* img, unsigned int ID) {
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	int iw = img->width;                        // 读取图片的宽和高
	int ih = img->height;
	int tx = (int)(rw - iw) / 2;                    // 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);


	CvvImage cimg;

	cimg.CopyOf(img);                            // 复制图片
	cimg.DrawToHDC(hDC, &rect);                // 将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}



void CRegionBasedSearch1Dlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CRegionBasedSearch1Dlg::OnBnClickedButtonSearch()
{
	UpdateData(TRUE);
	CvScalar searchValue(R_value, G_value, B_value);
	list<Region*> targetList= tree.BreadthFirstSearch(searchValue);
	showTargetRegion(ipl, targetList);
	
}





void CRegionBasedSearch1Dlg::OnReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	scrollPos = layerScroll.GetPos();
	SetDlgItemInt(IDC_EDIT2, scrollPos);
	string layerImgName = imgName + "_layer" + to_string(scrollPos) + ".jpg";
	char*char_name = (char*)layerImgName.data();
	IplImage* layerImg = cvLoadImage(char_name);
	ShowImage(layerImg, IDC_STATIC);
	*pResult = 0;
}




void CRegionBasedSearch1Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}





void CRegionBasedSearch1Dlg::OnReleasedcaptureSliderH(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	colorBarValue = colorBar.GetPos();
	SetDlgItemInt(IDC_EDIT_ColorValue, colorBarValue);

	list<Region*> targetList = tree.BreadthFirstSearch(colorBarValue);
	showTargetRegion(ipl, targetList);
	*pResult = 0;
}

void CRegionBasedSearch1Dlg::OnNMReleasedcaptureSliderH2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	adjColorBarValue = adjColorBar.GetPos();
	SetDlgItemInt(IDC_EDIT_ColorValue2, adjColorBarValue);

	list<Region*> targetList = tree.BreadthFirstSearch(colorBarValue,adjColorBarValue);
	showTargetRegion(ipl, targetList);
	*pResult = 0;
}


void CRegionBasedSearch1Dlg::showTargetRegion(IplImage* img,list<Region*> targets) {
	IplImage* targetImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	cvZero(targetImage);
	char* pTarget = targetImage->imageData;
	int step = targetImage->widthStep;
	if (targets.size() != 0) {
		for (list<Region*>::iterator iter = targets.begin(), end = targets.end();iter != end;iter++) {
			Region* r = *iter;
			for (list<Point>::iterator pixelIter = r->pixels.begin(), end = r->pixels.end();pixelIter != end;pixelIter++) {
				int x = pixelIter->x;
				int y = pixelIter->y;
				pTarget[x * 3 + y * step] = ipl->imageData[x * 3 + y * ipl->widthStep];
				pTarget[x * 3 + y * step + 1] = ipl->imageData[x * 3 + y * ipl->widthStep + 1];
				pTarget[x * 3 + y * step + 2] = ipl->imageData[x * 3 + y * ipl->widthStep + 2];

			}
		}
	}
	ShowImage(targetImage, IDC_TARGETS);
}




//void CRegionBasedSearch1Dlg::OnNMCustomdrawSliderH2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}
