#pragma once

#include <daScript/simulate/aot_builtin_time.h>

#if !DAS_NO_FILEIO

#include <sys/stat.h>

#if defined(_MSC_VER)

#include <io.h>
#include <direct.h>

#else
#include <libgen.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
#endif
#endif

namespace das {
    struct FStat;
    class Context;
    struct Block;
    struct SimNode_CallBase;
    struct LineInfoArg;

#if !DAS_NO_FILEIO

    struct FStat {
        struct stat stats;
        bool        is_valid;
        uint64_t size() const   { return stats.st_size; }
        Time     atime() const  { return { stats.st_atime }; }
        Time     ctime() const  { return { stats.st_ctime }; }
        Time     mtime() const  { return { stats.st_mtime }; }
#if defined(_MSC_VER)
        bool     is_reg() const { return stats.st_mode & _S_IFREG; }
        bool     is_dir() const { return stats.st_mode & _S_IFDIR; }
#else
        bool     is_reg() const { return S_ISREG(stats.st_mode); }
        bool     is_dir() const { return S_ISDIR(stats.st_mode); }
#endif
    };
#endif

    DAS_API const FILE * builtin_fopen  ( const char * name, const char * mode );
    DAS_API void builtin_fclose ( const FILE * f, Context * context, LineInfoArg * at );
    DAS_API void builtin_fflush ( const FILE * f, Context * context, LineInfoArg * at );
    DAS_API void builtin_fprint ( const FILE * f, const char * text, Context * context, LineInfoArg * at );
    DAS_API char * builtin_fread ( const FILE * _f, Context * context, LineInfoArg * at );
    DAS_API char* builtin_fgets(const FILE* _f, Context* context, LineInfoArg * at );
    DAS_API void builtin_fwrite(const FILE * _f, char * str, Context * context, LineInfoArg * at );
    DAS_API bool builtin_feof(const FILE* _f);
    DAS_API int64_t builtin_ftell ( const FILE * f, Context * context, LineInfoArg * at );
    DAS_API int64_t builtin_fseek ( const FILE * f, int64_t offset, int32_t mode, Context * context, LineInfoArg * at );
    DAS_API vec4f builtin_read ( Context &, SimNode_CallBase * call, vec4f * args );
    DAS_API vec4f builtin_write ( Context &, SimNode_CallBase * call, vec4f * args );
    DAS_API vec4f builtin_load ( Context & context, SimNode_CallBase *, vec4f * args );
    DAS_API void builtin_map_file ( const FILE* _f, const TBlock<void, TTemporary<TArray<uint8_t>>>& blk, Context*, LineInfoArg * at );
    DAS_API char * builtin_dirname ( const char * name, Context * context, LineInfoArg * at );
    DAS_API char * builtin_basename ( const char * name, Context * context, LineInfoArg * at );
    DAS_API bool builtin_fstat ( const FILE * f, FStat & fs, Context * context, LineInfoArg * at );
    DAS_API bool builtin_stat ( const char * filename, FStat & fs );
    DAS_API void builtin_dir ( const char * path, const Block & fblk, Context * context, LineInfoArg * at );
    DAS_API bool builtin_mkdir ( const char * path );
    DAS_API bool builtin_chdir ( const char * path );
    DAS_API char * builtin_getcwd ( Context * context, LineInfoArg * at );
    DAS_API const FILE * builtin_stdin();
    DAS_API const FILE * builtin_stdout();
    DAS_API const FILE * builtin_stderr();
    DAS_API int builtin_popen ( const char * cmd, const TBlock<void,const FILE *> & blk, Context * context, LineInfoArg * at );
    DAS_API int builtin_popen_binary ( const char * cmd, const TBlock<void,const FILE *> & blk, Context * context, LineInfoArg * at );
    DAS_API char * get_full_file_name ( const char * path, Context * context, LineInfoArg * );
    DAS_API bool builtin_remove_file ( const char * path );
    DAS_API bool builtin_rename_file ( const char * old_path, const char * new_path );
    DAS_API bool has_env_variable ( const char * var, Context * context, LineInfoArg * at );
    DAS_API char * get_env_variable ( const char * var, Context * context, LineInfoArg * at );
    DAS_API char * sanitize_command_line ( const char * cmd, Context * context, LineInfoArg * at );
}
