#pragma once

#include "daScript/simulate/cast.h"
#include "daScript/ast/compilation_errors.h"

namespace das
{
    class Context;

    string unescapeString ( const string & input, bool * error );
    string escapeString ( const string & input );
    string to_string_ex ( double dnum );
    string to_string_ex ( float dnum );
    string reportError ( const struct LineInfo & li, const string & message, const string & extra, 
        const string & fixme, CompilationError erc = CompilationError::unspecified );
    string reportError ( const char * st, const char * fileName, int row, int col, const string & message, 
        const string & extra, const string & fixme, CompilationError erc = CompilationError::unspecified );
    string reportErrorJson(const struct LineInfo & at, const string & message, const string & extra, 
        const string & fixme, CompilationError erc = CompilationError::unspecified );
    string reportErrorJson ( const char * st, const char * fileName, int row, int col, const string & message, 
        const string & extra, const string & fixme, CompilationError erc = CompilationError::unspecified );
}

