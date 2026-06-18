@echo off
setlocal EnableExtensions
rem ---------------------------------------------------------------------------
rem daslang MCP server launcher for Windows + MSVC.
rem
rem Loads the x64 Visual Studio developer environment (INCLUDE / LIB / PATH)
rem BEFORE starting the MCP server, so the cpp_compile_check tool can spawn
rem cl.exe and have it find the system headers. The compile DB
rem (compile_commands.json) omits system include paths on MSVC by design — the
rem compiler reads them from the environment — so without this the syntax check
rem fails on <vcruntime.h> etc.
rem
rem Point .mcp.json at this script instead of the bare daslang.exe:
rem   "daslang": { "command": "cmd", "args": ["/c", "utils\\mcp\\daslang-mcp-msvc.cmd"] }
rem
rem Stdio MCP discipline (this whole script's stdin/stdout IS the JSON-RPC pipe):
rem   * NO `for /f` here. A `for /f` that shells out runs a nested cmd that
rem     shares — and drains — this script's stdin (the JSON-RPC request bytes)
rem     before daslang reads them. vswhere's output is captured via a temp file
rem     + `set /p` instead, and vswhere is given `<nul` so it can't touch the pipe.
rem   * vcvars64.bat reads stdin too -> the `<nul` on its `call` line (top-level,
rem     NOT inside a parenthesized block, where cmd mis-parses the redirect and a
rem     stray al.exe fires "ALINK error A1017") keeps it off the pipe.
rem   * vcvars64.bat's banner is silenced (>nul 2>&1) — stray stdout corrupts frames.
rem
rem Linux / macOS do NOT need this: clang/gcc find their system headers
rem automatically, so there point .mcp.json straight at the daslang binary.
rem ---------------------------------------------------------------------------

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto run
set "VSINSTALL="
set "VSTMP=%TEMP%\das_mcp_vsinstall_%RANDOM%.txt"
"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath <nul > "%VSTMP%" 2>nul
set /p VSINSTALL=<"%VSTMP%"
del "%VSTMP%" >nul 2>&1
if not defined VSINSTALL goto run
if not exist "%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat" goto run
call "%VSINSTALL%\VC\Auxiliary\Build\vcvars64.bat" <nul >nul 2>&1

:run
rem Prefer the single-config (Ninja) binary, fall back to the MSVC Release layout.
set "DASLANG=%~dp0..\..\bin\daslang.exe"
if not exist "%DASLANG%" set "DASLANG=%~dp0..\..\bin\Release\daslang.exe"

"%DASLANG%" "%~dp0main.das" %*
exit /b %ERRORLEVEL%
