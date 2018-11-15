//
//  debug_info.hpp
//  yzg
//
//  Created by Boris Batkin on 11/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef debug_info_hpp
#define debug_info_hpp

#include <string>
#include <sstream>
#include <xmmintrin.h>

namespace yzg
{
    using namespace std;
    
    enum Type : uint8_t
    {
        none,
        tNull,
        tVoid,
        tBool,
        tInt,
        tInt2,
        tInt3,
        tInt4,
        tUInt,
        tUInt2,
        tUInt3,
        tUInt4,
        tFloat,
        tFloat2,
        tFloat3,
        tFloat4,
        tString,
        tStructure,
        tPointer
    };
    
    struct StructInfo;
    
    struct TypeInfo
    {
        Type            type;
        TypeInfo *      ptrType;
        StructInfo *    structType;
        uint32_t        dimSize;
        uint32_t *      dim;
        bool            ref;
    };
    
    struct VarInfo : TypeInfo
    {
        char *      name;
    };
    
    struct StructInfo
    {
        char *      name;
        VarInfo **  fields;
        uint32_t    fieldsSize;
    };
    
    struct FuncInfo
    {
        VarInfo **  args;
        uint32_t    argsSize;
    };
    
    string to_string ( Type t );
    Type nameToBasicType(const string & name);
    
    int getTypeBaseSize ( Type type );
    int getTypeBaseSize ( TypeInfo * info );
    int getDimSize ( TypeInfo * info );
    int getTypeSize ( TypeInfo * info );
    
    string debug_value ( void * pX, TypeInfo * info );
    string debug_value ( __m128 value, TypeInfo * info );
    string debug_type ( TypeInfo * info );
}

#endif /* debug_info_hpp */
