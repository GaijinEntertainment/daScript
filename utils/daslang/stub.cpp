// src/misc/ARCHITECTURE_DAS_ROOT.md#root-sidecar
#include "daScript/misc/das_root.h"
#include "asan_options.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#if defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
    #include <shellapi.h>
    #include <filesystem>
#else
    #include <dlfcn.h>
    #include <limits.h>
    #include <stdlib.h>
    #include <unistd.h>
    #if defined(__APPLE__)
        #include <mach-o/dyld.h>
    #endif
#endif

using namespace std;
using namespace das;

#if defined(_WIN32)
// bytes that are not UTF-8 (a sidecar saved in the ANSI codepage) convert to an empty path
static wstring to_wide ( const string & utf8 ) {
    std::filesystem::path p;
    return dasWidePath(utf8.c_str(), p) ? p.wstring() : wstring();
}
static string to_utf8 ( const wstring & wide ) { return std::filesystem::path(wide).u8string(); }

static wstring final_path ( const wstring & path ) {
    HANDLE h = CreateFileW(path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if ( h==INVALID_HANDLE_VALUE ) return path;
    wchar_t buf[4096];
    DWORD len = GetFinalPathNameByHandleW(h, buf, DWORD(sizeof(buf) / sizeof(buf[0])), FILE_NAME_NORMALIZED);
    CloseHandle(h);
    if ( !len || len >= sizeof(buf) / sizeof(buf[0]) ) return path;
    wstring real(buf, len);
    if ( real.compare(0, 4, L"\\\\?\\")==0 ) real.erase(0, 4);
    return real;
}
#endif

static string executable_path ( const char * argv0 ) {
#if defined(_WIN32)
    wchar_t buf[4096];
    DWORD len = GetModuleFileNameW(NULL, buf, DWORD(sizeof(buf) / sizeof(buf[0])));
    if ( len && len < sizeof(buf) / sizeof(buf[0]) ) return to_utf8(final_path(wstring(buf, len)));
#elif defined(__APPLE__)
    char buf[4096];
    uint32_t size = uint32_t(sizeof(buf));
    if ( _NSGetExecutablePath(buf, &size)==0 ) {
        char real[PATH_MAX];
        return realpath(buf, real) ? string(real) : string(buf);
    }
#elif defined(__linux__)
    char buf[4096];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if ( len > 0 ) return string(buf, size_t(len));
#endif
#if !defined(_WIN32)
    char real[PATH_MAX];
    if ( realpath(argv0, real) ) return string(real);
#endif
    return argv0;
}

struct HostLibrary {
    string  file;
    string  entry;
};

static HostLibrary host_library_of ( const string & exe ) {
    string name = dasBinaryStem(exe)=="daslang-live" ? "daslang_live_main" : "daslang_main";
#if defined(_WIN32)
    return { name + ".dll", name };
#elif defined(__APPLE__)
    return { "lib" + name + ".dylib", name };
#else
    return { "lib" + name + ".so", name };
#endif
}

static string dasroot_argument ( int argc, char * argv[] ) {
    string root;
    for ( int i=1; i + 1 < argc && strcmp(argv[i],"--")!=0; ++i ) {
        if ( strcmp(argv[i],"-dasroot")==0 ) root = argv[i + 1];
    }
    return root;
}

// src/misc/ARCHITECTURE_DAS_ROOT.md#root-sidecar
static vector<string> library_dirs ( const string & exeDir, int argc, char * argv[] ) {
    vector<string> dirs;
    auto add_root = [&](const string & root) {
        if ( root.empty() ) return;
        dirs.push_back(root + "/lib");
        dirs.push_back(root + "/bin");
    };
    string sidecar = readDasRootSidecar(exeDir);
    if ( !sidecar.empty() ) {
        add_root(sidecar);
        return dirs;
    }
    auto layout = dasBinLayoutOf(exeDir);
    if ( layout.underBin ) {
        dirs.push_back(layout.root + "/lib");
        if ( !layout.config.empty() ) dirs.push_back(layout.root + "/lib/" + layout.config);
    }
    dirs.push_back(exeDir);
    if ( layout.underBin ) dirs.push_back(layout.bin);
    add_root(dasroot_argument(argc, argv));
    return dirs;
}

typedef int (*host_main_t) ( int argc, char * argv[] );

static void * load_library ( const string & path, string & error ) {
#if defined(_WIN32)
    wstring wide = to_wide(path);
    wchar_t full[MAX_PATH];
    DWORD got = GetFullPathNameW(wide.c_str(), MAX_PATH, full, nullptr);
    const wchar_t * loadPath = (got > 0 && got < MAX_PATH) ? full : wide.c_str();
    SetDllDirectoryW(to_wide(dasPathPrefix(path)).c_str());
    void * lib = LoadLibraryExW(loadPath, NULL, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if ( !lib ) error = "error code " + to_string(GetLastError());
    return lib;
#else
    void * lib = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if ( !lib ) {
        const char * msg = dlerror();
        error = msg ? msg : "dlopen failed";
    }
    return lib;
#endif
}

static host_main_t entry_of ( void * lib, const string & name ) {
#if defined(_WIN32)
    return (host_main_t) GetProcAddress(HMODULE(lib), name.c_str());
#else
    return (host_main_t) dlsym(lib, name.c_str());
#endif
}

#if DAS_TRACK_ALLOC && !defined(_WIN32)
void das_stub_bind_alloc_tracker ( void * lib );
#endif

//! loads the host library and returns its entry; every string this allocates is freed before the entry runs
static host_main_t resolve_entry ( int argc, char * argv[], void * & lib ) {
    string exe = executable_path(argv[0]);
    string exeDir = exe.find_last_of("\\/")==string::npos ? string(".") : dasPathPrefix(exe);
    HostLibrary host = host_library_of(exe);
    vector<string> tried;
    for ( const auto & dir : library_dirs(exeDir, argc, argv) ) {
        string path = dir + "/" + host.file;
        tried.push_back(path);
        if ( !dasIsFile(path.c_str()) ) continue;
        string error;
        lib = load_library(path, error);
        if ( !lib ) {
            fprintf(stderr, "%s: cannot load %s: %s\n", argv[0], path.c_str(), error.c_str());
            return nullptr;
        }
        host_main_t entry = entry_of(lib, host.entry);
        if ( !entry ) fprintf(stderr, "%s: %s exports no %s\n", argv[0], path.c_str(), host.entry.c_str());
        return entry;
    }
    fprintf(stderr, "%s: no %s found - looked for", argv[0], host.file.c_str());
    for ( const auto & path : tried ) fprintf(stderr, " %s", path.c_str());
    fprintf(stderr, "; a relocated daslang names its das root in %s/%s or with -dasroot\n", exeDir.c_str(), dasRootSidecarName());
    return nullptr;
}

static int run_host ( int argc, char * argv[] ) {
    void * lib = nullptr;
    host_main_t entry = resolve_entry(argc, argv, lib);
    if ( !entry ) return 1;
#if DAS_TRACK_ALLOC && !defined(_WIN32)
    das_stub_bind_alloc_tracker(lib);
#endif
    return entry(argc, argv);
}

#if defined(_WIN32)
// the host reads argv as UTF-8; the narrow argv main receives is the ANSI codepage's rendering,
// so the arguments are taken again from the wide command line (wmain needs a MinGW link flag)
int main ( int, char ** ) {
    int argc = 0;
    wchar_t ** wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if ( !wargv ) return 1;
    vector<string> utf8(argc);
    vector<char *> argv(argc + 1, nullptr);
    for ( int i=0; i < argc; ++i ) {
        int n = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, nullptr, 0, nullptr, nullptr);
        if ( n > 0 ) {
            utf8[i].resize(n);
            if ( WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, &utf8[i][0], n, nullptr, nullptr) != n ) {
                utf8[i].clear();
            }
        }
        argv[i] = const_cast<char *>(utf8[i].c_str());
    }
    LocalFree(wargv);
    return run_host(argc, argv.data());
}
#else
int main ( int argc, char * argv[] ) {
    return run_host(argc, argv);
}
#endif
