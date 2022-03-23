
// PVZcheaterDlg.h: 头文件
//

#pragma once


// CPVZcheaterDlg 对话框
class CPVZcheaterDlg : public CDialogEx
{
// 构造
public:
	CPVZcheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVZCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	friend DWORD MonitorThreadFunc(LPVOID);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
	// 秒杀僵尸
	CButton KillZombie;
	afx_msg void OnBnClickedCheck2();
	// 无限阳光
	CButton Sun;
	// 创建线程监控
	HANDLE MonitorThread;
};
