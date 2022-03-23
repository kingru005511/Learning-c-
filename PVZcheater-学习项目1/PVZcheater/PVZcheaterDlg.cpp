
// PVZcheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PVZcheater.h"
#include "PVZcheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static CPVZcheaterDlg* g_dlg;
static HANDLE g_processHandle;

void WriteMemory(void* value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1) {
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}


void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

DWORD MonitorThreadFunc(LPVOID lpThreadParameter) {
	while (1) {
		// 获取游戏的句柄
		HWND gameHANDLE = FindWindow(CString("MainWindow"), CString("Plants vs. Zombies GOTY "));
		

		if (gameHANDLE == NULL) {
			g_dlg->KillZombie.SetCheck(FALSE);
			g_dlg->Sun.SetCheck(FALSE);
			g_dlg->KillZombie.EnableWindow(FALSE);
			g_dlg->Sun.EnableWindow(FALSE);
		}
		else {
			g_dlg->KillZombie.EnableWindow(TRUE);
			g_dlg->Sun.EnableWindow(TRUE);

			// 获取游戏的进程ID
			DWORD ProcessPid;
			GetWindowThreadProcessId(gameHANDLE, &ProcessPid);
			// 获取游戏的进程句柄
			g_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessPid);
		}
		if (g_dlg->Sun.GetCheck()) {
			/*
			CString str;
			str.Format(CString("启动无限阳光"));
			AfxMessageBox(str);
			*/
			DWORD value = 9999;
			WriteMemory(&value, sizeof(value), 0x0077959C, 0x00000868, 0x000005578, -1);
		}
		Sleep(1000);
	}
	return NULL;
};
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


// CPVZcheaterDlg 对话框



CPVZcheaterDlg::CPVZcheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPVZcheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, KillZombie);
	DDX_Control(pDX, IDC_CHECK2, Sun);
}

BEGIN_MESSAGE_MAP(CPVZcheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPVZcheaterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CPVZcheaterDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CPVZcheaterDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CPVZcheaterDlg 消息处理程序

BOOL CPVZcheaterDlg::OnInitDialog()
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
	// 监控游戏线程句柄
	MonitorThread = CreateThread(NULL,NULL, MonitorThreadFunc,NULL,NULL,NULL);
	//保存对话框
	g_dlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPVZcheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPVZcheaterDlg::OnPaint()
{
	if (IsIconic())
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPVZcheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPVZcheaterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(
		NULL,
		CString("open"),
		CString("https://www.baidu.com"),
		NULL,
		NULL,
		SW_SHOWNORMAL
	);
}


void CPVZcheaterDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (KillZombie.GetCheck()) {
		/*
		CString str;
		str.Format(CString("启动秒杀僵尸"));
		AfxMessageBox(str);
		*/
		BYTE data[] = {0xF6,0x90,0x90};
		WriteMemory(data,sizeof(data),0x00566D07);
	}
	else {
		/*
		CString str;
		str.Format(CString("关闭秒杀僵尸"));
		AfxMessageBox(str);
		*/
		BYTE data[] = { 0x74,0x24,0x20 };
		WriteMemory(data, sizeof(data), 0x00566D07);
	}
}


void CPVZcheaterDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
}
