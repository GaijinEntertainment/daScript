#include "daScript/misc/platform.h"

#include "daScript/misc/sysos.h"

#if defined(_MSC_VER) && !defined(_GAMING_XBOX) && !defined(_DURANGO)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    namespace das {
        __forceinline void setBits ( uint64_t & dw, uint64_t lowBit, uint64_t bits, uint64_t newValue ) {
            uint64_t mask = (uint64_t(1) << bits) - 1;
            dw = (dw & ~(mask << lowBit)) | (newValue << lowBit);
        }

        bool g_isVHSet = false;
        void ( * g_HwBpHandler ) ( int, void * ) = nullptr;

        long VEH_handler ( struct _EXCEPTION_POINTERS* ExceptionInfo ) {
            if ( ExceptionInfo->ContextRecord->Dr6 & 0x0f ) {
                if ( g_HwBpHandler ) {
                    if ( ExceptionInfo->ContextRecord->Dr6 & 1 ) g_HwBpHandler(0, (void *) ExceptionInfo->ContextRecord->Dr0 );
                    if ( ExceptionInfo->ContextRecord->Dr6 & 2 ) g_HwBpHandler(1, (void *) ExceptionInfo->ContextRecord->Dr1 );
                    if ( ExceptionInfo->ContextRecord->Dr6 & 4 ) g_HwBpHandler(2, (void *) ExceptionInfo->ContextRecord->Dr2 );
                    if ( ExceptionInfo->ContextRecord->Dr6 & 8 ) g_HwBpHandler(3, (void *) ExceptionInfo->ContextRecord->Dr3 );
                }
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            return EXCEPTION_CONTINUE_SEARCH;
        }

        void hwSetBreakpointHandler ( void (* handler ) ( int, void * ) ) {
            g_HwBpHandler = handler;
        }

        int hwBreakpointSet ( void * address, uint64_t len, uint64_t when ) {
            if ( !g_isVHSet ) {
                g_isVHSet = true;
                AddVectoredExceptionHandler(1, VEH_handler);
            }
            CONTEXT cxt;
	        HANDLE thisThread = GetCurrentThread();
        	cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;
            if ( !GetThreadContext(thisThread, &cxt) ) return -1;
            int bp_index = 0;
            for ( bp_index=0; bp_index!=4; ++bp_index ) {
                uint64_t mask = uint64_t(1) << (bp_index*2);
        		if ( (cxt.Dr7 & mask) == 0ul ) {
                    break;
                }
            }
            switch ( bp_index ) {
            case 0:     cxt.Dr0 = intptr_t(address); break;
            case 1:     cxt.Dr1 = intptr_t(address); break;
            case 2:     cxt.Dr2 = intptr_t(address); break;
            case 3:     cxt.Dr3 = intptr_t(address); break;
            default:    return -1;
            }
	        setBits(cxt.Dr7, 16 + (bp_index*4), 2, when);
	        setBits(cxt.Dr7, 18 + (bp_index*4), 2, len);
	        setBits(cxt.Dr7, bp_index*2,        1, 1);
        	if ( !SetThreadContext(thisThread, &cxt) ) return -1;
            return bp_index;
        }

        bool hwBreakpointClear ( int bp_index ) {
            if ( bp_index==-1 ) return false;
            CONTEXT cxt;
            HANDLE thisThread = GetCurrentThread();
            cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;
            if (!GetThreadContext(thisThread, &cxt)) return false;
            setBits(cxt.Dr7, bp_index*2, 1, 0);
            if (!SetThreadContext(thisThread, &cxt)) return false;
            return true;
        }

        size_t getExecutablePathName(char* pathName, size_t pathNameCapacity) {
            return GetModuleFileNameA(NULL, pathName, (DWORD)pathNameCapacity);
        }
        void * loadDynamicLibrary ( const char * fileName ) {
            return LoadLibraryA(fileName);
        }
        void * getFunctionAddress ( void * module, const char * func ) {
            return GetProcAddress(HMODULE(module),func);
        }
        void * getLibraryHandle ( const char * moduleName ) {
            return GetModuleHandleA(moduleName);
        }
    }
#elif defined(__linux__)
    #include <unistd.h>
    namespace das {
        void hwSetBreakpointHandler ( void (*) ( int, void * ) ) { }
        int hwBreakpointSet ( void *, uint64_t, uint64_t ) {
            return -1;
        }
        bool hwBreakpointClear ( int ) {
            return false;
        }
        size_t getExecutablePathName(char* pathName, size_t pathNameCapacity) {
            size_t pathNameSize = readlink("/proc/self/exe", pathName, pathNameCapacity - 1);
            pathName[pathNameSize] = '\0';
            return pathNameSize;
        }
        void * loadDynamicLibrary ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
        void * getFunctionAddress ( void *, const char * ) {
            // TODO: implement
            return nullptr;
        }
        void * getLibraryHandle ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
    }
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <limits.h>
    namespace das {
        void hwSetBreakpointHandler ( void (*) ( int, void * ) ) { }
        int hwBreakpointSet ( void *, uint64_t, uint64_t ) {
            return -1;
        }
        bool hwBreakpointClear ( int ) {
            return false;
        }
        size_t getExecutablePathName(char* pathName, size_t pathNameCapacity) {
            uint32_t pathNameSize = 0;
            _NSGetExecutablePath(NULL, &pathNameSize);
            if (pathNameSize > pathNameCapacity)
                pathNameSize = pathNameCapacity;
            if (!_NSGetExecutablePath(pathName, &pathNameSize)) {
                char real[PATH_MAX];
                if (realpath(pathName, real) != NULL) {
                    pathNameSize = strlen(real);
                    memcpy(pathName, real, pathNameSize+1);
                }
                return pathNameSize;
            }
            return 0;
        }
        void * loadDynamicLibrary ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
        void * getFunctionAddress ( void *, const char * ) {
            // TODO: implement
            return nullptr;
        }
        void * getLibraryHandle ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
    }
#else
    namespace das {
        void hwSetBreakpointHandler ( void (*) ( int, void * ) ) { }
        int hwBreakpointSet ( void *, uint64_t, uint64_t ) {
            return -1;
        }
        bool hwBreakpointClear ( int ) {
            return false;
        }
        size_t getExecutablePathName(char*, size_t) {
            DAS_FATAL_LOG("platforms without getExecutablePathName should not use default getDasRoot");
            DAS_FATAL_ERROR;
            return 0;
        }
        void * loadDynamicLibrary ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
        void * getFunctionAddress ( void *, const char * ) {
            // TODO: implement
            return nullptr;
        }
        void * getLibraryHandle ( const char *  ) {
            // TODO: implement
            return nullptr;
        }
    }
#endif

namespace das {
    string getExecutableFileName ( void ) {
        char buffer[1024];
        return getExecutablePathName(buffer,1024) ? buffer : "";
    }

    string get_prefix ( const string & req ) {
        auto np = req.find_last_of("\\/");
        if ( np != string::npos ) {
            return req.substr(0,np);
        } else {
            return req;
        }
    }

    string get_suffix ( const string & req ) {
        auto np = req.find_last_of("\\/");
        if ( np != string::npos ) {
            return req.substr(np+1);
        } else {
            return req;
        }
    }

    string g_dasRoot;

    void setDasRoot ( const string & dr ) {
        g_dasRoot = dr;
    }

    string getDasRoot ( void ) {
        if ( g_dasRoot.empty() ) {
            string efp = getExecutableFileName();   // ?/bin/debug/binary.exe
            auto np = efp.find_last_of("\\/");
            if ( np != string::npos ) {
                auto ep = get_prefix(efp);          // remove file name
                auto suffix = get_suffix(ep);
                if ( suffix != "bin" ) {
                    ep = get_prefix(ep);            // remove debug
                }
                if ( get_suffix(ep)!="bin" ) {
                    g_dasRoot = ".";
                } else {
                    g_dasRoot = get_prefix(ep);     // remove bin
                }
            } else {
                g_dasRoot = ".";
            }
        }
        return g_dasRoot;
    }
}
