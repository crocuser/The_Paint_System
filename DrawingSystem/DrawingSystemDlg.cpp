
// DrawingSystemDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DrawingSystem.h"
#include "DrawingSystemDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDrawingSystemDlg 对话框



CDrawingSystemDlg::CDrawingSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWINGSYSTEM_DIALOG, pParent)
	, LineWidth(0)
	, m_LineType(0)
	, m_Text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawingSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, LineWidth);
	DDV_MinMaxInt(pDX, LineWidth, 0, 20);
	DDX_Radio(pDX, IDC_RADIO1, m_LineType);
	DDX_Control(pDX, IDC_COMBO1, m_filled);
	DDX_Control(pDX, IDC_COMBO3, m_DrawMode);
	DDX_Text(pDX, IDC_EDIT2, m_Text);
}

BEGIN_MESSAGE_MAP(CDrawingSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawingSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDrawingSystemDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CDrawingSystemDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO1, &CDrawingSystemDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDrawingSystemDlg::OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDrawingSystemDlg::OnCbnSelchangeCombo1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDrawingSystemDlg::OnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON3, &CDrawingSystemDlg::OnBnClickedButton3)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CDrawingSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDrawingSystemDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDrawingSystemDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDrawingSystemDlg 消息处理程序

BOOL CDrawingSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	LineColor = RGB(0, 0, 255);//初始化线条颜色
	FillColor = RGB(255, 0, 0);//初始化填充颜色

	LineWidth = 5;//初始化线条粗细
	UpdateData(FALSE);//初始后不更新该值

	LineType = 0;//默认实线

	//下拉框添加选项
	m_filled.AddString(TEXT("有填充"));
	m_filled.AddString(TEXT("无填充"));
	m_filled.SetCurSel(0);//设置默认选项

	IsFilled = 1;//默认有填充

	DrawMode = FreeDrawing;
	m_DrawMode.AddString(TEXT("点"));
	m_DrawMode.AddString(TEXT("直线"));
	m_DrawMode.AddString(TEXT("椭圆"));
	m_DrawMode.AddString(TEXT("矩形"));
	m_DrawMode.AddString(TEXT("自由绘制"));
	m_DrawMode.SetCurSel(4);//设置默认选项

	StartDrawing = false;//自由绘制没有开始

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDrawingSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrawingSystemDlg::OnPaint()
{
	if (IsIconic())//检查窗口是否处于最小化状态
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CPaintDC dc(this);//用于绘制的设备上下文
		//dc.SetPixel(50, 50, RGB(255, 0, 0));

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDrawingSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//设置线条颜色按钮关联事件
void CDrawingSystemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CColorDialog colorDlg(LineColor);//构造对话框，传入初始颜色值
	if (IDOK == colorDlg.DoModal())//显示对话框并判断是否点击"确定"
	{
		LineColor = colorDlg.GetColor();//获取颜色对话框中选择的颜色值

	}
}

//设置填充颜色按钮关联事件
void CDrawingSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg(FillColor);//构造对话框，传入初始颜色值
	if (IDOK == colorDlg.DoModal())//显示对话框并判断是否点击"确定"
	{
		FillColor = colorDlg.GetColor();//获取颜色对话框中选择的颜色值

	}
}


//当编辑框中内内容改变时，触发事件
void CDrawingSystemDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);//编辑框中的值更新到绑定的变量值中
	//CString str;
	//str.Format(TEXT("%d"), LineWidth);//整型转字符串
	//MessageBox(str);//弹出消息提示框
}

//实线选项
void CDrawingSystemDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = 0;
}

//虚线选项
void CDrawingSystemDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = 1;
}

//下拉框选项发生改变，触发事件
void CDrawingSystemDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_filled.GetCurSel();//获取当前所选项下标
	if (index == 0) IsFilled = 1;
	else IsFilled = 0;

}

//鼠标左键按下时，触发事件
void CDrawingSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CClientDC dc(this);//用于绘制的设备上下文
	DownPoint = point;

	if (DrawMode == PointMode)
	{
		dc.SetPixel(point.x, point.y, LineColor);//绘制点
	}
	else if (DrawMode == FreeDrawing)
	{
		StartDrawing = true;
		LastPoint = point;
	}
	else if (DrawMode == InsertText)
	{
		dc.TextOut(point.x, point.y, m_Text);//文本输出
	}
	else if (DrawMode == InsertImage)
	{
		CRect rect;
		GetClientRect(&rect);

		CImage image;
		image.Load(m_ImagePath);  // 加载图像
		image.BitBlt(dc,point.x,point.y,rect.Width(),rect.Height(),0,0);//绘制
		image.Destroy();//销毁图片，防止内存泄漏
	}
	OnSelchangeCombo3();//切换回原来的模式
	CDialogEx::OnLButtonDown(nFlags, point);
}

//鼠标左键抬起时，触发事件
void CDrawingSystemDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (DrawMode)
	{
	case LineMode:
	{
		DrawLine(point);
		break;
	}
	case EllipseMode:
	{
		DrawEllipse(point);
		break;
	}
	case RectangleMode:
	{
		DrawRectangle(point);
		break;
	}
	case FreeDrawing:
	{
		StartDrawing = false;
		break;
	}
	default:
		break;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

//绘画模式选择
void CDrawingSystemDlg::OnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	DrawMode = m_DrawMode.GetCurSel();
	
}

void CDrawingSystemDlg::DrawLine(CPoint point)
{
	CClientDC dc(this);//用于绘制的设备上下文
	// 创建红色虚线画笔
	CPen pen(LineType, LineWidth, LineColor);

	// 选择画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&pen);//传一个新的，返回一个老的

	// 设置画笔透明度
	dc.SetBkMode(TRANSPARENT);

	dc.MoveTo(DownPoint.x, DownPoint.y);//先move到起点（按下）
	dc.LineTo(point.x, point.y);//再line到终点（抬起）

	// 恢复原来的画笔
	dc.SelectObject(pOldPen);
}

void CDrawingSystemDlg::DrawEllipse(CPoint point)
{
	CClientDC dc(this);//用于绘制的设备上下文
	//绘制椭圆，边缘有颜色粗细，内部有填充
	CPen outlinePen(LineType, LineWidth, LineColor);//画笔
	CPen* pOldPen = dc.SelectObject(&outlinePen);

	CBrush fillBrush(FillColor);//画刷
	if (!IsFilled)
	{
		fillBrush.CreateStockObject(NULL_BRUSH);//无填充，则画刷设置为透明
	}
	CBrush* pOldBrush = dc.SelectObject(&fillBrush);

	dc.Ellipse(DownPoint.x, DownPoint.y, point.x, point.y);//绘制椭圆，其外接矩形的左上角和后下角

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void CDrawingSystemDlg::DrawRectangle(CPoint point)
{
	CClientDC dc(this);//用于绘制的设备上下文
	//绘制矩形，边缘有颜色粗细，内部有填充
	CPen outlinePen(LineType, LineWidth, LineColor);//画笔
	CPen* pOldPen = dc.SelectObject(&outlinePen);

	CBrush fillBrush(FillColor);//画刷
	if (!IsFilled)
	{
		fillBrush.CreateStockObject(NULL_BRUSH);//无填充，则画刷设置为透明
	}
	CBrush* pOldBrush = dc.SelectObject(&fillBrush);

	dc.Rectangle(DownPoint.x, DownPoint.y, point.x, point.y);//绘制矩形，其外接矩形的左上角和后下角

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

//清空画布
void CDrawingSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	
	Invalidate();//清空绘图内容并触发重绘
}

//鼠标移动时(自由绘制)，触发事件--化曲为直
void CDrawingSystemDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (DrawMode == FreeDrawing && StartDrawing)//是自由绘画模式且按下了鼠标左键
	{
		CClientDC dc(this);//用于绘制的设备上下文
		// 创建红色虚线画笔
		CPen pen(LineType, LineWidth, LineColor);

		// 选择画笔到设备上下文
		CPen* pOldPen = dc.SelectObject(&pen);//传一个新的，返回一个老的

		// 设置画笔透明度
		dc.SetBkMode(TRANSPARENT);

		dc.MoveTo(LastPoint.x, LastPoint.y);//起点
		dc.LineTo(point.x, point.y);//终点

		// 恢复原来的画笔
		dc.SelectObject(pOldPen);

		LastPoint = point;//更新当前起点
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

//插入文本按钮
void CDrawingSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	DrawMode = InsertText;//改变模式
	UpdateData(true);//让文本框中的值更新绑定的值
	MessageBox(TEXT("使用鼠标左键确定插入 文本 位置"), TEXT("小提示~"));

}

//插入图片按钮
void CDrawingSystemDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	//设置过滤器
	TCHAR szFilter[] = _T("图片(*.png)|*.png|所有文件(*.*)|*.*||");
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, szFilter, this);
	
	if (IDOK == fileDlg.DoModal())//若点击了“打开”按钮，则获得了图片路径
	{
		m_ImagePath = fileDlg.GetPathName();
		DrawMode = InsertImage;//模式改为插入图片
		MessageBox(TEXT("使用鼠标左键确定插入 图片 位置"), TEXT("小提示~~"));
	}
}

//保存图片按钮
void CDrawingSystemDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CClientDC dc(this);
	CRect rect;
	//GetClientRect(&rect);  //获取画布大小
	rect.left = 0;
	rect.right = 930;//boxlength为宏定义
	rect.top = 0;
	rect.bottom = 740;
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	//创建位图
	HDC hdc = CreateCompatibleDC(dc);      //创建DC，以便将图像保存为不同的格式
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//将屏幕DC的图像复制到内存DC中
	CImage image;
	image.Attach(hbitmap); //将位图转化为一般图像
	CString imageName = _T("crocuser-")+ CTime::GetCurrentTime().Format(_T("%Y%m%d-%H%M%S"));
	CString saveFilePath;
	{
		CString  strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件 |*.gif| PNG图像文件 |*.png|其他格式(*.*)| *.* ||");
		CFileDialog dlg(FALSE, _T("bmp"), imageName, NULL, strFilter);
		if (dlg.DoModal() != IDOK)
			return;
		CString strFileName; //文件扩展名
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;
		if (dlg.m_ofn.nFileExtension = 0)
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			case 3:
				strExtension = "gif"; break;
			case 4:
				strExtension = "png"; break;
			default:
				break;
			}
			strFileName = strFileName + "." + strExtension;
		}
		saveFilePath = strFileName;
	}
	HRESULT hResult = image.Save(saveFilePath);     //保存图像
	image.Detach();
	SelectObject(hdc, hOldMap);

}
