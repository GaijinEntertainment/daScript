#pragma once

namespace das
{
    using namespace std;

    enum Type : uint8_t {
        none,
        autoinfer,
        alias,
        fakeContext,
        anyArgument,
        tVoid,
        tBool,
        tInt64,
        tUInt64,
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
        tRange,
        tURange,
        tString,
        tStructure,
        tHandle,
        tPointer,
        tIterator,
        tArray,
        tTable,
        tBlock
    };

    struct StructInfo;
    struct TypeAnnotation;

    struct LineInfo {
        LineInfo() = default;
        LineInfo(int c, int l) : column(uint32_t(c)), line(uint32_t(l)) {}
        __forceinline bool operator < ( const LineInfo & info ) const { return (line==info.line) ? column<info.column : line<info.line; }
        __forceinline bool operator == ( const LineInfo & info ) const { return line==info.line && column==info.column; }
        __forceinline bool operator != ( const LineInfo & info ) const { return line!=info.line || column!=info.column; }
        string describe() const;
        uint32_t    column = 0, line = 0;
    };

    struct TypeInfo {
        Type                type;
        StructInfo *        structType;
        TypeAnnotation *    annotation;
        TypeInfo *          firstType;      // map  from, or array
        TypeInfo *          secondType;     // map  to
        uint32_t            dimSize;
        uint32_t *          dim;
        bool                ref;
        bool                canCopy;
        bool                isPod;
    };

    struct VarInfo : TypeInfo {
        char *      name;
        uint32_t    offset;
    };

    struct StructInfo {
        char *      name;
        VarInfo **  fields;
        uint32_t    fieldsSize;
        uint32_t    size;
    };

    struct FuncInfo {
        char *      name;
        VarInfo **  args;
        uint32_t    argsSize;
        uint32_t    stackSize;
    };

    string to_string ( Type t );
    Type nameToBasicType(const string & name);

    int getTypeBaseSize ( Type type );
    int getTypeBaseAlign ( Type type );
    int getTypeBaseSize ( TypeInfo * info );
    int getDimSize ( TypeInfo * info );
    int getTypeSize ( TypeInfo * info );
    int getTypeAlign ( TypeInfo * info );

    enum class PrintFlags : uint32_t {
        none         = 0
    ,   escapeString =  (1<<0)

    ,   string_builder  =   PrintFlags::none
    ,   debugger        =   PrintFlags::escapeString
    ,   stackwalker     =   PrintFlags::escapeString
    };

    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags );
    string debug_value ( vec4f value, TypeInfo * info, PrintFlags flags );
    string debug_type ( TypeInfo * info );
}

