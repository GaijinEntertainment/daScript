#pragma once

namespace das {
    struct FStat;
    class Context;
    struct Block;
    struct SimNode_CallBase;

    const FILE * builtin_fopen  ( const char * name, const char * mode );
    void builtin_fclose ( const FILE * f, Context * context );
    void builtin_fprint ( const FILE * f, const char * text, Context * context );
    char * builtin_fread ( const FILE * _f, Context * context );
    char* builtin_fgets(const FILE* _f, Context* context);
    void builtin_fwrite(const FILE * _f, char * str, Context * context);
    bool builtin_feof(const FILE* _f);
    vec4f builtin_read ( Context &, SimNode_CallBase * call, vec4f * args );
    vec4f builtin_write ( Context &, SimNode_CallBase * call, vec4f * args );
    vec4f builtin_load ( Context & context, SimNode_CallBase *, vec4f * args );
    void builtin_map_file(const FILE* _f, const TBlock<void, TTemporary<const char*>>& blk, Context* context);
    char * builtin_dirname ( const char * name, Context * context );
    char * builtin_basename ( const char * name, Context * context );
    bool builtin_fstat ( const FILE * f, FStat & fs, Context * context, LineInfoArg * at );
    bool builtin_stat ( const char * filename, FStat & fs );
    void builtin_dir ( const char * path, const Block & fblk, Context * context );
    bool builtin_mkdir ( const char * path );
    const FILE * builtin_stdin();
    const FILE * builtin_stdout();
    const FILE * builtin_stderr();
}
