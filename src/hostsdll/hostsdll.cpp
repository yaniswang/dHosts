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
#include <windows.h>
#include <atlstr.h>
#include "detours.h"
#include "hostsdll.h"
#include "windns.h"

#pragma comment(lib, "detours.lib")

#pragma data_seg("Shared")
	HWND hHostWnd = NULL;
	TCHAR tcHostUsername[256] = {0};
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS") 

HHOOK g_hHook;

LRESULT CALLBACK CBTProc( int nCode,WPARAM wParam,LPARAM lParam)
{
	return CallNextHookEx(g_hHook,nCode,wParam,lParam);
}

//初始化HOOK
HOOK_API int initHook(HWND hWnd,TCHAR* username){
	hHostWnd = hWnd;
	_tcsncpy(tcHostUsername,username,256);
	g_hHook = SetWindowsHookEx(WH_CBT,CBTProc,GetModuleHandle(_T("hostsdll.dll")),0);
	return 0;
};

DWORD getHostIp(PCWSTR pszName)
{
	CString str= pszName; 
	COPYDATASTRUCT myCopyDATA; 
	myCopyDATA.cbData=str.GetLength()*2; 
	myCopyDATA.lpData=str.GetBuffer();
	return SendMessage(hHostWnd,WM_COPYDATA,NULL,(LPARAM)&myCopyDATA);
}

DNS_STATUS (WINAPI * SysDnsQuery_A)(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved) = NULL;
DNS_STATUS (WINAPI * SysDnsQuery_UTF8)(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved) = NULL;
DNS_STATUS (WINAPI * SysDnsQuery_W)(PCWSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved) = NULL;

DNS_STATUS WINAPI HookDnsQuery_A(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved)
{	
	//MessageBoxA(NULL,pszName,"HookDnsQuery_A",MB_OK);
 	return SysDnsQuery_A(pszName, wType, Options, pExtra, ppQueryResults, pReserved);
}
DNS_STATUS WINAPI HookDnsQuery_UTF8(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved)
{	
	//MessageBoxA(NULL,pszName,"HookDnsQuery_UTF8",MB_OK);
 	return SysDnsQuery_UTF8(pszName, wType, Options, pExtra, ppQueryResults, pReserved);
}
DNS_STATUS WINAPI HookDnsQuery_W(PCWSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved)
{	
	DWORD ip = getHostIp(pszName);
	if(DNS_TYPE_A == wType && 0 != ip)
	{
		SysDnsQuery_W(_T("localhost"), wType, Options, pExtra, ppQueryResults, pReserved);

		PDNS_RECORD pResult=*ppQueryResults;

		if(pResult != NULL){
			//pResult->pName = NULL;
			//wcscpy(pResult->pName,pszName);
			pResult->wType = DNS_TYPE_A;
			pResult->dwTtl = 0;
			pResult->Flags.DW = 8201;
			pResult->dwReserved = 1;
			pResult->wDataLength = 4;
			pResult->Data.A.IpAddress = ip;
			pResult->pNext = NULL;
		}

		return 0;
	}
	else
	{
		return SysDnsQuery_W(pszName, wType, Options, pExtra, ppQueryResults, pReserved);
	}
}

INT (WSAAPI * SysGetAddrInfoW)(PCWSTR pNodeName,PCWSTR pServiceName,const ADDRINFOW * pHints,PADDRINFOW * ppResult) = NULL;

INT WSAAPI HookGetAddrInfoW(PCWSTR pNodeName,PCWSTR pServiceName,const ADDRINFOW * pHints,PADDRINFOW * ppResult)
{
	DWORD ip = getHostIp(pNodeName);
	if(0 != ip && pHints != NULL && pHints->ai_flags != 4){

		SysGetAddrInfoW(_T("localhost"), pServiceName, pHints, ppResult);

		PADDRINFOW pResult=*ppResult;

		//CString str;
		//str.Format(_T("ai_family : %i, ai_protocol : %i, ai_socktype : %i, ai_flags : %i, ai_addrlen : %i, ai_canonname : %s"),pResult->ai_family,pResult->ai_protocol,pResult->ai_socktype,pResult->ai_flags,pResult->ai_addrlen, pResult->ai_canonname);
		//MessageBox(NULL,str,_T("HookGetAddrInfoW"),MB_OK);

		pResult->ai_canonname = NULL;
		pResult->ai_family = AF_INET;//IPV4
		pResult->ai_protocol = IPPROTO_IP;//IP协议
		pResult->ai_socktype = 0;
		pResult->ai_flags = 0;
		pResult->ai_addrlen = 16;

		sockaddr_in *addr = (sockaddr_in*)(pResult->ai_addr);
		pResult->ai_addr = (sockaddr*)addr;
		addr->sin_family = AF_INET;
		addr->sin_addr.S_un.S_addr = ip;

		pResult->ai_next = NULL;

		return 0;
	}
	else{
		return SysGetAddrInfoW(pNodeName, pServiceName, pHints, ppResult);
	}
}



//int (WINAPI *SysMessageBox)(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
//                                  = MessageBox;
//
//int WINAPI HookMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
//{	
// 	return SysMessageBox(hWnd, _T("HookMsg调用中..."), lpCaption, uType);
//}


//是否成功HOOK API
BOOL bHook = FALSE;

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	if(hHostWnd != NULL)
	{
		if (DLL_PROCESS_ATTACH == fdwReason)
		{
			//读取当前用户名
			const int nBufSize = 256;
			TCHAR tcUsername[nBufSize];
			ZeroMemory(tcUsername,nBufSize);
			DWORD dwRet = nBufSize;
			GetUserName(tcUsername,&dwRet);

			if(_tcscmp(tcHostUsername,tcUsername) != 0)//不是同一个用户名下，取消DLL加载
			{
				return false;
			}

			SysDnsQuery_A
			= ((DNS_STATUS (WINAPI *)(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved))
			   DetourFindFunction("dnsapi.dll", "DnsQuery_A"));
			SysDnsQuery_UTF8
			= ((DNS_STATUS (WINAPI *)(PCSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved))
			   DetourFindFunction("dnsapi.dll", "DnsQuery_UTF8"));
			SysDnsQuery_W
			= ((DNS_STATUS (WINAPI *)(PCWSTR pszName,WORD wType,DWORD Options,PVOID pExtra,PDNS_RECORD * ppQueryResults,PVOID * pReserved))
			   DetourFindFunction("dnsapi.dll", "DnsQuery_W"));

			SysGetAddrInfoW
			= ((INT (WSAAPI *)(PCWSTR pNodeName,PCWSTR pServiceName,const ADDRINFOW * pHints,PADDRINFOW * ppResult))
			   DetourFindFunction("Ws2_32.dll", "GetAddrInfoW"));

			if (SysDnsQuery_W == NULL) {  //没找到,取消HOOK
				return FALSE;
			}
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)SysDnsQuery_A, HookDnsQuery_A);
			DetourAttach(&(PVOID&)SysDnsQuery_UTF8, HookDnsQuery_UTF8);
			DetourAttach(&(PVOID&)SysDnsQuery_W, HookDnsQuery_W);
			DetourAttach(&(PVOID&)SysGetAddrInfoW, HookGetAddrInfoW);
			/*DetourAttach(&(PVOID&)SysMessageBox, HookMessageBox);*/
			DetourTransactionCommit();
			bHook = TRUE;
		}
		else if (DLL_PROCESS_DETACH == fdwReason)
		{
			if(bHook == TRUE){
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourDetach(&(PVOID&)SysDnsQuery_A, HookDnsQuery_A);
				DetourDetach(&(PVOID&)SysDnsQuery_UTF8, HookDnsQuery_UTF8);
				DetourDetach(&(PVOID&)SysDnsQuery_W, HookDnsQuery_W);
				DetourDetach(&(PVOID&)SysGetAddrInfoW, HookGetAddrInfoW);
				/*DetourDetach(&(PVOID&)SysMessageBox, HookMessageBox);*/
				DetourTransactionCommit();
			}
		}
	}
	return TRUE;
}