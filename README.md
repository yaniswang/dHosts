dHosts
================

dHosts的全称为desktopHosts，意为桌面多用户hosts。

是一款基于VC和Detours开发的互联网开发辅助软件，以实现Windows Server远程多用户之间相互完全隔离的hosts绑定。

主要功能：
================

1. 实现远程桌面登录的多用户之间完全隔离的hosts文件，可部署为大量用户并发使用的公用多浏览器测试服务器。
2. 可将域名或者主机名重定向到另一个域名或主机名，可解决某些公司动态IP的指向问题。

应用场景：
================

1. 作为互联网公司前端开发时公用的多浏览器测试环境，实现用户之间完全隔离的hosts环境，从此再也不需要开发者安装虚拟机才能测试。
2. 部署为测试部门UI自动化的执行机，可在一台机器上同时并发运行无数个不同hosts绑定的应用环境。

实现原理：
================

dHosts利用API HOOK将系统的DNS请求API拦截，之后在用户桌面上的hosts.txt文件过滤一遍，如果本地有做相关域名的绑定，则直接返回，否则重新调用系统API。

实际上就是在系统层面为每个用户实现了各自独立的hosts，开启dHosts之后，请求一个域名的DNS就变成了如下流程：

1. 在dHost中查找是否有记录，如果有直接返回，没有继续下一步
2. 在系统hosts中查找是否有记录，如果有直接返回，没有继续下一步
3. 在电信DNS中查找是否有记录，如果有直接返回，如果没有则DNS解析失败

使用方法：
================

1. 安装Server 2003或者Server 2008，并配置好多用户远程桌面环境
2. 为每个开发者建账号
3. 在全部用户的自动运行中添加dhosts.exe的快捷方式
4. 用远程桌面软件连接至各自的账号中，修改桌面上的hosts.txt文件，此文件的语法同系统hosts完全一样。并在系统hosts语法基础上，增加了绑定到主机名或域名的功能。

		例如：
		pc1	www.google.com	# 绑定到PC1
		www.alibaba.com	www.baidu.com	#绑定到www.alibaba.com

		
License
================

dHosts is released under the MIT license:

> The MIT License
> 
> Copyright (c) 2012 Yanis Wang \<yanis.wang@gmail.com\>
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
> 
> The above copyright notice and this permission notice shall be included in
> all copies or substantial portions of the Software.
> 
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
> THE SOFTWARE.

Thanks
================

* Detours: [http://research.microsoft.com/en-us/projects/detours/](http://research.microsoft.com/en-us/projects/detours/)
* GitHub: [https://github.com/](https://github.com/)
* Amuro: [非常感谢Amuro的Api Hook技术支持](http://szamuro.blogbus.com/)