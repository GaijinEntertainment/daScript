#pragma once

namespace das
{
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
        tEnumeration,
        tPointer,
        tIterator,
        tArray,
        tTable,
        tBlock
    };

    struct StructInfo;
    struct TypeAnnotation;
    struct EnumInfo;

    struct BasicAnnotation {
        BasicAnnotation ( const string & n ) : name(n) {}
        string      name;
    };

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
        EnumInfo *          enumType;
        TypeAnnotation *    annotation;
        TypeInfo *          firstType;      // map  from, or array
        TypeInfo *          secondType;     // map  to
        uint32_t            dimSize;
        uint32_t *          dim;
        union {
            struct {
                bool        ref : 1;
                bool        refType : 1;
                bool        canCopy : 1;
                bool        isPod : 1;
            };
            uint32_t flags = 0;
        };
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

    struct EnumValueInfo {
        char *      name;
        int32_t     value;
    };

    struct EnumInfo {
        char *              name;
        EnumValueInfo **    values;
        uint32_t            totalValues;
    };

    struct FuncInfo {
        char *      name;
        VarInfo **  args;
        uint32_t    argsSize;
        uint32_t    stackSize;
    };

    string das_to_string ( Type t );
    Type nameToBasicType(const string & name);

    int getTypeBaseSize ( Type type );
    int getTypeBaseAlign ( Type type );
    int getTypeBaseSize ( TypeInfo * info );
    int getDimSize ( TypeInfo * info );
    int getTypeSize ( TypeInfo * info );
    int getTypeAlign ( TypeInfo * info );

    enum class PrintFlags : uint32_t {
        none =                  0
    ,   escapeString =          (1<<0)
    ,   namesAndDimensions =    (1<<1)
    ,   typeQualifiers =        (1<<2)

    ,   string_builder  =   PrintFlags::none
    ,   debugger        =   PrintFlags::escapeString | PrintFlags::namesAndDimensions | PrintFlags::typeQualifiers
    ,   stackwalker     =   PrintFlags::escapeString | PrintFlags::namesAndDimensions | PrintFlags::typeQualifiers
    };

    string debug_type ( TypeInfo * info );
}

