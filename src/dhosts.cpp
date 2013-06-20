/*
dHosts is released under the MIT license:

Copyright (c) 2012 Yanis Wang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
// hosts.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "dhosts.h"
#include "dhostsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdhostsApp

BEGIN_MESSAGE_MAP(CdhostsApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CdhostsApp 构造

CdhostsApp::CdhostsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CdhostsApp 对象

CdhostsApp theApp;


// CdhostsApp 初始化

BOOL CdhostsApp::InitInstance()
{
	CreateMutex(NULL,true,m_pszAppName); 
	if(GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		//AfxMessageBox(_T("应用程序不可以重复启动"),MB_OK | MB_APPLMODAL | MB_ICONSTOP); 
		return(false); 
	}
	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	CdhostsDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

