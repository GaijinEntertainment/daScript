// Foreign Function Binding -- Calling Native DLL Functions
//
// The `[extern]` annotation lets you call C functions from shared
// libraries (DLLs on Windows, .so on Linux, .dylib on macOS)
// directly from daslang, without writing any C++ glue code.
//
// Annotation syntax:
//   [extern(convention, name="CName", library="LibName")]
//   def DasName(args) : ReturnType { return default_value; }
//
// Parameters:
//   convention -- calling convention: stdcall (aka WINAPI), cdecl, opengl
//   name       -- the exported symbol name in the library
//   library    -- DLL / shared library to load
//
// The function body is a stub (never executed) -- the annotation
// replaces it with a direct call to the native function at link time.
//
// Platform-specific libraries can be specified:
//   [extern(cdecl, name="foo",
//           windows_library="foo.dll",
//           macos_library="libfoo.dylib",
//           linux_library="libfoo.so")]
//
// Add `late` to defer symbol resolution until first call:
//   [extern(cdecl, late, name="foo", library="plugin.dll")]
//
// Limitations:
//   - Vector types (float2/float3/float4) are not supported as arguments
//   - Maximum ~13 arguments per function (platform-dependent)
//   - Requires `require dasbind` module
//
// This example calls Win32 API functions from Kernel32.dll.
// Run (Windows only): daslang.exe examples/dasbind/dasbind_example.das

options gen2
require dasbind

// --- Win32 API bindings ---
// Each function declares the calling convention (stdcall for Win32),
// the exported C name, and the DLL that contains it.
// The function body is a stub -- it is never executed.

// Returns the processor number the current thread is running on.
[extern(stdcall, name="GetCurrentProcessorNumber", library="Kernel32")]
def GetCurrentProcessorNumber() : int { return -1; }

// Sets the title of the console window.
[extern(stdcall, name="SetConsoleTitleA", library="Kernel32")]
def SetConsoleTitle(lpConsoleTitle : string) : bool { return false; }

// Returns the current process ID.
[extern(stdcall, name="GetCurrentProcessId", library="Kernel32")]
def GetCurrentProcessId() : uint { return 0u; }

// Retrieves the number of milliseconds since the system was started.
[extern(stdcall, name="GetTickCount", library="Kernel32")]
def GetTickCount() : uint { return 0u; }

// Terminates the current process immediately.
[extern(stdcall, name="ExitProcess", library="Kernel32")]
def ExitProcess(uExitCode : uint) {}

// --- Main ---

[export]
def main() {
    // Query system information
    let proc_num = GetCurrentProcessorNumber()
    let pid = int(GetCurrentProcessId())
    let uptime_ms = int(GetTickCount())
    print("processor number : {proc_num}\n")
    print("process ID       : {pid}\n")
    print("system uptime    : {uptime_ms} ms\n")

    // Modify the console window title
    SetConsoleTitle("daslang -- dasbind example")
    print("(console title changed)\n")

    // ExitProcess terminates immediately -- nothing after it runs.
    // Uncomment to see: the process exits with code 42, skipping
    // any cleanup or finally blocks.
    // ExitProcess(42u)

    print("done\n")
}

