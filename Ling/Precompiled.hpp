// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
//for a vc2010 issue http://connect.microsoft.com/VisualStudio/feedback/details/621653/including-stdint-after-intsafe-generates-warnings#
//#ifdef _MSC_VER // This check can be removed if you only build for Windows
//#pragma warning (push)
//#pragma warning (disable : 4005)
//#include <intsafe.h>
//#include <stdint.h>
//#pragma warning (pop)
//#pragma warning (default : 4005)
//#endif

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlgdi.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>
//#include <atlstdthunk.h>


//GDI plus
#include <gdiplus.h>
using namespace Gdiplus;

#include <Windows.h>

#include <shobjidl.h>
#include <shlobj.h>
#include <knownfolders.h>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/result.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/mpl/list.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <math.h>
#include <map>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <cstdlib>// getenv
#include <cstddef>// NULL
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"

#ifndef JSON_SPIRIT_VALUE_ENABLED
#error Please define JSON_SPIRIT_VALUE_ENABLED for the Value type to be enabled 
#endif

#include "log.hpp"
#pragma comment(lib, "atlthunk.lib") 

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif