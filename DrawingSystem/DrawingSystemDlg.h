
// DrawingSystemDlg.h: 头文件
//

#pragma once

//枚举会话模式
enum DrawingMode
{
	PointMode = 0,
	LineMode,
	EllipseMode,
	RectangleMode,
	FreeDrawing,
	InsertText,
	InsertImage
};

// CDrawingSystemDlg 对话框
class CDrawingSystemDlg : public CDialogEx
{
// 构造
public:
	CDrawingSystemDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWINGSYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();//初始化对话框：初始化操作尽量都放在这个函数里
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	COLORREF LineColor;//线条颜色
	COLORREF FillColor;//填充颜色
	int LineWidth;//线条粗细
	int LineType;//线条类型：0实线，1虚线
	int IsFilled;//有无填充：0无填充
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
	afx_msg void OnEnChangeEdit1();
	int m_LineType;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CComboBox m_filled;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CPoint DownPoint;//记录鼠标左键按下的坐标
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//绘制模式
	int DrawMode;
	CComboBox m_DrawMode;
	afx_msg void OnSelchangeCombo3();

	void DrawLine(CPoint point);
	void DrawEllipse(CPoint point);
	void DrawRectangle(CPoint point);

	bool StartDrawing;//自由绘图开始的标记
	CPoint LastPoint;//上一时刻的点的位置，即起点
	afx_msg void OnBnClickedButton3();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CString m_Text;//插入文本的内容
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	CString m_ImagePath;//插入图片的位置

	afx_msg void OnBnClickedButton6();
};
