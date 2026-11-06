#pragma once

#include <stdio.h>

namespace das {

    #define DAS_MAX_HW_BREAKPOINTS  16

    enum HwBpType {
        HwBp_Execute =      0,
        HwBp_Write =        1,
        HwBp_ReadWrite =    3,
    };

    enum HwBpSize {
        HwBp_1 =    0,
        HwBp_2 =    1,
        HwBp_4 =    3,  // yes 3
        HwBp_8 =    2,
    };

    DAS_API string getExecutableFileName ( void );
    DAS_API size_t getExecutablePathName ( char * pathName, size_t pathNameCapacity );
    DAS_API string getExecutablePathUtf8 ( void );
    //! the file the code at `address` was loaded from - the executable or a shared library; "" when the platform cannot say
    DAS_API string getModuleFileNameOf ( const void * address );
    DAS_API string getDasRoot ( void );
    //! runs `entry` with the process arguments as UTF-8: on Windows they are taken again from the
    //! wide command line (the narrow argv main receives is the ANSI codepage's rendering)
    DAS_API int runMainWithUtf8Arguments ( int (*entry)(int, char **), int argc, char * argv[] );
    DAS_API void setDasRoot ( const string & dr );

    DAS_API string normalizeFileName ( const char * fileName );
    DAS_API FILE * das_fopen_utf8 ( const char * fileName, const char * mode );
    //! Returns an owned FILE for regular-file reading without waiting on a FIFO; size comes from the opened object.
    DAS_API FILE * das_fopen_regular_read_utf8 ( const char * fileName, uint64_t & size );
    //! size, modification time and kind of `fileName`; false when there is no such file
    DAS_API bool das_file_stat_utf8 ( const char * fileName, uint64_t & size, int64_t & mtime, bool & isDirectory );
    DAS_API bool das_remove_utf8 ( const char * fileName );
    //! renames over an existing `to`
    DAS_API bool das_rename_utf8 ( const char * from, const char * to );
    //! `fn` for every entry of `dir` but `.` and `..`, names in UTF-8
    DAS_API void das_each_directory_entry ( const char * dir, void (*fn)(const char * name, bool isDirectory, void * user), void * user );
#if defined(_WIN32)
    //! the path forms the runtime holds (UTF-8) and the Windows API takes (UTF-16); an invalid UTF-8 path converts to ""
    DAS_API std::wstring das_utf8_to_wide ( const char * utf8 );
    DAS_API string das_wide_to_utf8 ( const wchar_t * wide );
#endif

    DAS_API string get_prefix ( const string & req );   // blah.... \ foo.bar - returns blah....
    DAS_API string get_suffix ( const string & req );   // blah.... \ foo.bar - returns foo.bar

    void * loadDynamicLibrary ( const char * fileName );
    void * getFunctionAddress ( void * module, const char * func );
    void * getLibraryHandle ( const char * moduleName );
    bool closeLibrary ( void * module );
    string getDynamicLibraryError ( void );


    void hwSetBreakpointHandler ( void (* handler ) ( int, void * ) );
    int hwBreakpointSet ( void * address, int len, int when );
    bool hwBreakpointClear ( int bp_index );
}
