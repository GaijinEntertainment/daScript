#pragma once

namespace das {
    FILE * builtin_fopen  ( const char * name, const char * mode );
    void builtin_fclose ( const FILE * f );
    void builtin_fprint ( const FILE * f, const char * text );
}
