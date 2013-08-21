# Ling编译步骤
## 安装VC++ 2008 express
## 安装SDK:SDK for Windows 7 and .NET Framework 3.5 Service Pack 1
  Windows SDK包括了一组工具， 示例代码，文档，编译器，头文件和库， 这
  使得开发人员可以使用SDK开发运行在Windows上的应该程序。可以使用
  Win32/Com开发本地运行的代码，也可以开发基于.Net Framework的托管代码。
  完了后，把这个目录添加到

> include： C:\MicrosoftSDKs\Windows\v7.0\Include

## 安装WDK:WDK 7600.16385.1
  WDK是微软提供的用来开始驱动程序的一个工具包，基中包括了ATL库， WTL依
  赖于这个库。完了后，把这个目录添加到

> include: C:\WinDDK\7600.16385.1\inc\atl71 
> lib:     C:\WinDDK\7600.16385.1\lib\ATL\i386

## Boost
  Ling目前只使用了boost.filesystem和boost.unittest， 所以只需要编译这两个库即可。
  另外需要的boost.statechart是不需要编译就可使用的。
> bjam --with-thread --with-date_time --with-filesystem --build-type=complete
> 
> include: D:\bzhu\boost_1_46_1
> 
> lib:

## WTL
> include: C:\WTL81_9127\Include
## 打开 Ling.sln就可以编译，运行了。
