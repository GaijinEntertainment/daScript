set PATH=D:\devtools\vc2012\include;D:\devtools\vc2012\bin;D:\devtools\vc2012\lib;D:\devtools\win.sdk.100\lib\10.0.10240.0\um\x86;
set VSINC=D:\devtools\vc2012\include
set WININC=D:\devtools\win.sdk.100\include\10.0.10586.0\um
set WINSINC=D:\devtools\win.sdk.100\include\10.0.10586.0\shared
@SET LIB=D:\devtools\win.sdk.100\lib\10.0.10240.0\um\x86;D:\devtools\vc2012\lib;
@SET INCLUDE=D:\devtools\vc2012\include;D:\devtools\vc2012\bin;D:\devtools\vc2012\lib;D:\devtools\win.sdk.100\lib\10.0.10240.0\um\x86;
@SET LIBPATH==D:\devtools\win.sdk.100\lib\10.0.10240.0\um\x86;D:\devtools\vc2012\lib;
echo %INCLUDE%
cl /Feasjit.exe /DUSE_JIT -I %WINSINC% -I %WININC% -I %VSINC% -I jit jit\as_jit.cpp jit\virtual_asm_windows.cpp jit\virtual_asm_x86.cpp main.cpp -I sdk\angelscript\include -I sdk\add_on\ sdk\angelscript\source\*.cpp /EHsc /DAS_USE_STLNAMES=1 sdk\add_on\scriptstdstring\*.cpp sdk\add_on\scriptarray\*.cpp /Ox /Oy /DNDEBUG /DAS_NO_THREADS winmm.lib 
cl /Feas.exe main.cpp -I %WINSINC% -I %WININC% -I %VSINC% -I sdk\angelscript\include -I sdk\add_on\ sdk\angelscript\source\*.cpp /EHsc /DAS_USE_STLNAMES=1 sdk\add_on\scriptstdstring\*.cpp sdk\add_on\scriptarray\*.cpp /Ox /Oy /DNDEBUG /DAS_NO_THREADS winmm.lib 

del *.obj
