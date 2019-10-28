set VSPATH=D:\devtools\vc2015.3
set WINSDK=D:\devtools\win.sdk.100
set PATH=D:\devtools\LLVM-8.0.0\bin;%VSPATH%\include;%WINSDK%\10.0.10586.0\ucrt;%VSPATH%\bin\x86_amd64;%VSPATH%\lib\amd64;%WINSDK%\lib\10.0.10586.0\um\x64;%WINSDK%\lib\10.0.10586.0\ucrt\x64;
set VSINC=%VSPATH%\include;%WINSDK%\10.0.10586.0\ucrt;%WINSDK%\include\10.0.10586.0\um
set WININC=%WINSDK%\include\10.0.10586.0\um;%WINSDK%\include\10.0.10586.0\ucrt
set WINSINC=%WINSDK%\include\10.0.10586.0\shared;
@SET LIB=%WINSDK%\lib\10.0.10586.0\um\x64;%VSPATH%\lib\amd64
@SET INCLUDE=%VSPATH%\include;%WINSDK%\include\10.0.10586.0\ucrt;%WINSDK%\include\10.0.10586.0\um;%WINSDK%\include\10.0.10586.0\shared
@SET LIBPATH=%WINSDK%\lib\10.0.10586.0\ucrt\x64;%WINSDK%\lib\10.0.10586.0\um\x64;%VSPATH%\lib\amd64
echo %INCLUDE%
ml64.exe  -c -nologo sdk\angelscript\source\as_callfunc_x64_msvc_asm.asm
clang-cl /arch:AVX /Feasjit.exe /DUSE_JIT -I %WINSINC% -I %WININC% -I %VSINC% -I jit jit\as_jit.cpp jit\virtual_asm_windows.cpp jit\virtual_asm_x64.cpp main.cpp -I sdk\angelscript\include -I sdk\add_on\ sdk\angelscript\source\*.cpp /EHsc /DAS_USE_STLNAMES=1 sdk\add_on\scriptstdstring\*.cpp sdk\add_on\scriptmath\*.cpp sdk\add_on\scriptarray\*.cpp /Ox /Oy /DNDEBUG /DAS_NO_THREADS /DAS_NO_EXCEPTIONS winmm.lib %WINSDK%\lib\10.0.10586.0\ucrt\x64\libucrt.lib as_callfunc_x64_msvc_asm.obj 
clang-cl /arch:AVX /Feas.exe main.cpp -I %WINSINC% -I %WININC% -I %VSINC% -I sdk\angelscript\include -I sdk\add_on\ sdk\angelscript\source\*.cpp /EHsc /DAS_USE_STLNAMES=1 sdk\add_on\scriptstdstring\*.cpp sdk\add_on\scriptmath\*.cpp sdk\add_on\scriptarray\*.cpp /Ox /Oy /DNDEBUG /DAS_NO_THREADS /DAS_NO_EXCEPTIONS winmm.lib  %WINSDK%\lib\10.0.10586.0\ucrt\x64\libucrt.lib as_callfunc_x64_msvc_asm.obj 

del *.obj
