#pragma once

#include "daScript/misc/uric.h"
namespace das {
    class Context;
    void delete_uri ( Uri & uri );
    void clone_uri ( Uri & uri, const Uri & uriS );
    Uri add_base_uri ( const Uri & base, const Uri & relative );
    const char * uri_to_string ( const Uri & uri, Context * context );
    char * uri_to_unix_file_name ( char * uristr, Context * context );
    char * uri_to_windows_file_name ( char * uristr, Context * context );
    char * unix_file_name_to_uri ( char * uristr, Context * context );
    char * windows_file_name_to_uri ( char * uristr, Context * context );
    char * escape_uri ( char * uristr, bool spaceToPlus, bool normalizeBreaks, Context * context );
    char * unescape_uri ( char * uristr,Context * context );
    char * normalize_uri ( char * uristr,Context * context );
    char * makeNewGuid( Context * context );
}
