#pragma once

#include "daScript/misc/platform.h"
#include "daScript/simulate/debug_info.h"

namespace das {

    struct AstSerializer;
    struct AnnotationArgumentList;

    //      [annotation (value,value,...,value)]
    //  or  [annotation (key=value,key,value,...,key=value)]
    struct DAS_API AnnotationArgument {
        Type    type;       // tInt, tInt64, tUInt64, tFloat, tBool, tString
        string  name;
        string  sValue;
        union {
            bool    bValue;
            int     iValue;
            int64_t lValue;
            uint64_t ulValue;
            float   fValue;
            AnnotationArgumentList * aList; // only used during parsing
        };
        LineInfo    at;
        AnnotationArgument () : type(Type::tVoid), ulValue(0) {}
        //explicit copy is required to avoid copying union as float and cause FPE
        AnnotationArgument ( const AnnotationArgument & a )
            : type(a.type), name(a.name), sValue(a.sValue), ulValue(0), at(a.at) { memcpy(&ulValue, &a.ulValue, sizeof(ulValue)); }
        AnnotationArgument & operator = ( const AnnotationArgument & a ) {
            type=a.type; name=a.name; sValue=a.sValue; memmove(&ulValue, &a.ulValue, sizeof(ulValue)); at=a.at; return *this;
        }
        AnnotationArgument ( const string & n, const string & s, const LineInfo & loc = LineInfo() )
            : type(Type::tString), name(n), sValue(s), ulValue(0), at(loc) {}
        AnnotationArgument ( const string & n, bool b, const LineInfo & loc = LineInfo() )
            : type(Type::tBool), name(n), ulValue(0), at(loc) { bValue = b; }
        AnnotationArgument ( const string & n, int i, const LineInfo & loc = LineInfo() )
            : type(Type::tInt), name(n), ulValue(0), at(loc) { iValue = i; }
        AnnotationArgument ( const string & n, int64_t i, const LineInfo & loc = LineInfo() )
            : type(Type::tInt64), name(n), lValue(i), at(loc) {}
        AnnotationArgument ( const string & n, uint64_t i, const LineInfo & loc = LineInfo() )
            : type(Type::tUInt64), name(n), ulValue(i), at(loc) {}
        AnnotationArgument ( const string & n, float f, const LineInfo & loc = LineInfo() )
            : type(Type::tFloat), name(n), ulValue(0), at(loc) { fValue = f; }
        AnnotationArgument ( const string & n, AnnotationArgumentList * al, const LineInfo & loc = LineInfo() )
            : type(Type::none), name(n), ulValue(0), at(loc) { aList = al; }
        void serialize ( AstSerializer & ser );
    };

    typedef vector<AnnotationArgument> AnnotationArguments;

    struct DAS_API AnnotationArgumentList : AnnotationArguments {
        const AnnotationArgument * find ( const string & name, Type type ) const;
        bool getBoolOption(const string & name, bool def = false) const;
        int32_t getIntOption(const string & name, int32_t def = 0) const;
        uint64_t getUInt64Option(const string & name, uint64_t def = 0) const;
        uint64_t getUInt64OptionEx (const string & name, const string & name2, uint64_t def = 0) const;
        void serialize ( AstSerializer & ser );
    };
}
