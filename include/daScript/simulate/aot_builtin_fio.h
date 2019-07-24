#pragma once

namespace das {
    struct FStat;
    class Context;
    struct Block;
    struct SimNode_CallBase;

    const FILE * builtin_fopen  ( const char * name, const char * mode );
    void builtin_fclose ( const FILE * f );
    void builtin_fprint ( const FILE * f, const char * text );
    char * builtin_fread ( const FILE * _f, Context * context );
    vec4f builtin_read ( Context &, SimNode_CallBase * call, vec4f * args );
    vec4f builtin_write ( Context &, SimNode_CallBase * call, vec4f * args );
    vec4f builtin_load ( Context & context, SimNode_CallBase *, vec4f * args );
    char * builtin_dirname ( const char * name, Context * context );
    char * builtin_basename ( const char * name, Context * context );
    bool builtin_fstat ( const FILE * f, FStat & fs );
    bool builtin_stat ( const char * filename, FStat & fs );
    void builtin_dir ( const char * path, const Block & fblk, Context * context );
    bool builtin_mkdir ( const char * path );
    const FILE * builtin_stdin();
    const FILE * builtin_stdout();
    const FILE * builtin_stderr();
}
