
// hostsDlg.h : 头文件
//

#pragma once
#define WM_NOTIFYICON (WM_USER+1001)

// CdhostsDlg 对话框
class CdhostsDlg : public CDialogEx
{
// 构造
public:
	CdhostsDlg(CWnd* pParent = NULL);	// 标准构造函数

	void showDialog();
	void endDialog();
	void updateIpMap();

// 对话框数据
	enum { IDD = IDD_DHOSTS_DIALOG };
	BOOL DestroyWindow();

protected:
	BOOL bVisible;
	CMap<CString, LPCTSTR, DWORD, DWORD> ipMap;
	BOOL bHostsInterval;//定时刷新HOSTS

	CString sHostpath;//桌面hosts文件路径，在OnInitDialog中初始化
	FILETIME   fHostLastWriteTime;//上次修改时间

	void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	HICON m_hIcon;

	NOTIFYICONDATA m_NotifyIconData;//托盘图标

	HRESULT OnNotifyIcon(WPARAM wParam,LPARAM lParam);//托盘图标回调函数

	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	void OnClose();
	BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
