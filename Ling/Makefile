CC = cl.exe
CLFLAGS = /nologo /Od /D "WIN32" /D "_WINDOWS" /D "STRICT" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" \
	  /Gm /EHsc /RTC1 /MTd /Yc"Precompiled.hpp" /Fp"Release\Precompiled.pch" \
	  /Fd"Release\\" \
	  /D _CRT_SECURE_NO_WARNINGS \
          /W3 /c /ZI /TP \
          /I"C:\WinDDK\7600.16385.1\inc\atl71" \
          /I"C:\WTL81_9127\Include" \
          /I"D:\3rd_lib\boost_1_46_1\include\boost-1_46_1" \
          /I"D:\3rd_lib\libzplay-2.02-sdk\C++"


LKFLAGS = /SUBSYSTEM:WINDOWS /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /nologo \
	  /INCREMENTAL \
          /LIBPATH:"C:\WinDDK\7600.16385.1\lib\ATL\i386" \
	  /LIBPATH:"D:\3rd_lib\libzplay-2.02-sdk\C++"

LIBS = kernel32.lib user32.lib gdi32.lib

all:Release\Ling.exe


Release\Ling.exe:Release\Ling.res Release\Bar.obj Release\CoolButton.obj Release\Ui.obj Release\Player.obj \
	Release\Player_impl.obj Release\Fsm.obj Release\PlayerState.obj Release\MainWindow.obj Release\Ling.obj
	link.exe  /out:$@ /DEBUG /PDB:Release\ling.pdb $(LKFLAGS) $(LIBS) $^ 

Release\Ling.res:Ling.rc resource.h
	rc.exe /d "_DEBUG" /d "_UNICODE" /d "UNICODE" /l 0x409 /i"C:\WTL81_9127\Include" /fo"Release/Ling.res" $<  

Release\Bar.obj:Bar.cpp Bar.hpp MainWindow.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\CoolButton.obj:CoolButton.cpp CoolButton.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\Ui.obj:Ui.cpp UiInterface.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\Player.obj:Player.cpp Player.hpp Player_impl.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\Player_impl.obj:Player_impl.cpp Player_impl.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\Fsm.obj:Fsm.cpp Fsm.hpp UiInterface.hpp PlayerState.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\PlayerState.obj:PlayerState.cpp PlayerState.hpp Fsm.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\MainWindow.obj:MainWindow.cpp MainWindow.hpp Bar.hpp CoolButton.hpp UiInterface.hpp Fsm.hpp Ui.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@

Release\Ling.obj:Ling.cpp MainWindow.hpp
	$(CC) $(CLFLAGS) /c $< /Fo$@


clean:
	del Release\*.obj Release\ling.exe Release\*.res Release\*.pdb Release\*.ilk Release\*.pch

