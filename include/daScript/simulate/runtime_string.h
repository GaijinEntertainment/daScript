#pragma once

#include "daScript/simulate/cast.h"
#include "daScript/ast/compilation_errors.h"

namespace das
{
    class Context;

    string unescapeString ( const string & input );
    string escapeString ( const string & input );
    string to_string_ex ( double dnum );
    string reportError ( const struct LineInfo & li, const string & message, CompilationError erc = CompilationError::unspecified );
    string reportError ( const char * st, const char * fileName, int row, int col, const string & message, CompilationError erc = CompilationError::unspecified );
}

