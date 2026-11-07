// The only TU allowed to touch the environment directly. Host configs (dagor)
// poison getenv for everyone else, so this must be defined before platform.h.
#define DAS_ALLOW_GETENV 1

#include "daScript/misc/platform.h"

#include "daScript/misc/env_cfg.h"

#include <stdlib.h>

#if !DAS_TARGET_PS && !defined(_WIN32)
extern "C" char ** environ;
#endif

#if defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
#endif

namespace das {

#if defined(_WIN32)
    // the environment is UTF-16; the narrow CRT view renders it in the ANSI codepage
    static std::wstring env_wide ( const char * utf8 ) {
        std::wstring w;
        if ( !utf8 ) return w;
        int n = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
        if ( n <= 0 ) return w;
        w.resize(n);
        if ( MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &w[0], n) != n ) return std::wstring();
        w.resize(n - 1);
        return w;
    }

    static string env_utf8 ( const wchar_t * wide ) {
        string s;
        int n = WideCharToMultiByte(CP_UTF8, 0, wide, -1, nullptr, 0, nullptr, nullptr);
        if ( n <= 0 ) return s;
        s.resize(n);
        if ( WideCharToMultiByte(CP_UTF8, 0, wide, -1, &s[0], n, nullptr, nullptr) != n ) return string();
        s.resize(n - 1);
        return s;
    }
#endif

    const char * das_getenv ( const char * name ) {
#if DAS_TARGET_PS
        (void)name;
        return nullptr;
#elif defined(_WIN32)
        thread_local string value;
        std::wstring wname = env_wide(name);
        DWORD n = GetEnvironmentVariableW(wname.c_str(), nullptr, 0);
        if ( n == 0 ) return nullptr;
        std::wstring wvalue(n, L'\0');
        DWORD got = GetEnvironmentVariableW(wname.c_str(), &wvalue[0], n);
        wvalue.resize(got);
        value = env_utf8(wvalue.c_str());
        return value.c_str();
#else
        return getenv(name);
#endif
    }

    void das_setenv ( const char * name, const char * value ) {
#if DAS_TARGET_PS
        (void)name; (void)value;
#elif defined(_WIN32)
        _wputenv_s(env_wide(name).c_str(), env_wide(value ? value : "").c_str());
#else
        setenv(name, value ? value : "", 1);
#endif
    }

    char * const * das_environ () {
#if DAS_TARGET_PS
        return nullptr;
#elif defined(_WIN32)
        return _environ;
#else
        return environ;
#endif
    }

    const char * get_dasenv_gc_stage_report ()         { return das_getenv("DAS_GC_STAGE_REPORT"); }
    const char * get_dasenv_gc_break_on_id ()          { return das_getenv("DAS_GC_BREAK_ON_ID"); }
    const char * get_dasenv_jobque_threads ()          { return das_getenv("DAS_JOBQUE_THREADS"); }
    const char * get_dasenv_jobque_affinity ()         { return das_getenv("DAS_JOBQUE_AFFINITY"); }
    const char * get_dasenv_jobque_limit_order ()      { return das_getenv("DAS_JOBQUE_LIMIT_ORDER"); }
    const char * get_dasenv_jobque_team_rank_gate ()   { return das_getenv("DAS_JOBQUE_TEAM_RANK_GATE"); }
    const char * get_dasenv_jobque_team_eager_exit ()  { return das_getenv("DAS_JOBQUE_TEAM_EAGER_EXIT"); }
    const char * get_dasenv_team_prof ()               { return das_getenv("DAS_TEAM_PROF"); }
    const char * get_dasenv_trace_module_load ()       { return das_getenv("DAS_TRACE_MODULE_LOAD"); }
    const char * get_dasenv_module_cache_limit ()      { return das_getenv("DAS_MODULE_CACHE_LIMIT"); }
    const char * get_dasenv_log_level ()               { return das_getenv("DAS_LOG_LEVEL"); }
    const char * get_dasenv_depfile ()                 { return das_getenv("DAS_DEPFILE"); }
    const char * get_dasenv_depfile_target ()          { return das_getenv("DAS_DEPFILE_TARGET"); }
    const char * get_dasenv_depfile_exclude ()         { return das_getenv("DAS_DEPFILE_EXCLUDE"); }

    const char * get_columns ()                     { return das_getenv("COLUMNS"); }
}
