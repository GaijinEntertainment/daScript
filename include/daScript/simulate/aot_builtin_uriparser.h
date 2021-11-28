#pragma once

namespace das {
    class Context;
    char * uri_to_unix_file_name ( char * uristr, Context * context );
    char * uri_to_windows_file_name ( char * uristr, Context * context );
    char * unix_file_name_to_uri ( char * uristr, Context * context );
    char * windows_file_name_to_uri ( char * uristr, Context * context );
    char * escape_uri ( char * uristr, bool spaceToPlus, bool normalizeBreaks, Context * context );
    char * unescape_uri ( char * uristr,Context * context );
    char * normalize_uri ( char * uristr,Context * context );
    char * makeNewGuid( Context * context );
}
