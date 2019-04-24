#pragma once

namespace das
{
    enum Type : int32_t {
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
        tDouble,
        tRange,
        tURange,
        tString,
        tStructure,
        tHandle,
        tEnumeration,
        tPointer,
        tFunction,
        tLambda,
        tIterator,
        tArray,
        tTable,
        tBlock
    };

    struct StructInfo;
    struct TypeAnnotation;
    struct EnumInfo;

    struct BasicAnnotation {
        BasicAnnotation ( const string & n, const string & cpn = "" ) : name(n), cppName(cpn) {}
        string      name;
        string      cppName;
    };

    struct FileInfo {
        FileInfo() = default;
        FileInfo(const char * s, uint32_t l) : source(s), sourceLength(l) {}
        virtual void freeSourceData() { }
        virtual ~FileInfo() { freeSourceData(); }
        string                name;
        const char *          source = nullptr;
        uint32_t              sourceLength = 0;
    };
    typedef unique_ptr<FileInfo> FileInfoPtr;

    class FileAccess {
    public:
        virtual ~FileAccess() {}
        FileInfo * setFileInfo ( const string & fileName, FileInfoPtr && info );
        FileInfo * getFileInfo ( const string & fileName );
        virtual string getIncludeFileName ( const string & fileName, const string & incFileName ) const;
        void freeSourceData();
        FileInfoPtr letGoOfFileInfo ( const string & fileName );
    protected:
        virtual FileInfo * getNewFileInfo ( const string & ) { return nullptr; }
    protected:
        map<string, FileInfoPtr>    files;
    };
    typedef shared_ptr<FileAccess> FileAccessPtr;

    struct LineInfo {
        LineInfo() = default;
        LineInfo(FileInfo * fi, int c, int l)
            : fileInfo(fi), column(uint32_t(c)), line(uint32_t(l)) {}
        bool operator < ( const LineInfo & info ) const;
        bool operator == ( const LineInfo & info ) const;
        bool operator != ( const LineInfo & info ) const;
        string describe() const;
        FileInfo *  fileInfo = nullptr;
        uint32_t    column = 0, line = 0;
    };

    struct TypeInfo {
        enum {
            flag_ref = 1<<0,
            flag_refType = 1<<1,
            flag_canCopy = 1<<2,
            flag_isPod = 1<<3,
            flag_isRawPod = 1<<4,
            flag_isConst = 1<<5,
        };
        Type                type;
        StructInfo *        structType;
        EnumInfo *          enumType;
        TypeAnnotation *    annotation_or_name;
        TypeInfo *          firstType;      // map  from, or array
        TypeInfo *          secondType;     // map  to
        uint32_t            dimSize;
        uint32_t *          dim;
        uint32_t            flags;
        uint32_t            hash;
        __forceinline bool isRef() const { return flags & flag_ref; }
        __forceinline bool isRefType() const { return flags & flag_refType; }
        __forceinline bool canCopy() const { return flags & flag_canCopy; }
        __forceinline bool isPod() const { return flags & flag_isPod; }
        __forceinline bool isRawPod() const { return flags & flag_isRawPod; }
        __forceinline bool isConst() const { return flags & flag_isConst; }
    };

    TypeAnnotation * resolveAnnotation ( TypeInfo * info );

    struct VarInfo : TypeInfo {
        char *      name;
        uint32_t    offset;
        VarInfo() = default;
        VarInfo(Type _type, StructInfo * _structType, EnumInfo * _enumType, TypeAnnotation * _annotation_or_name, 
                TypeInfo * _firstType, TypeInfo * _secondType, uint32_t _dimSize, uint32_t * _dim, uint32_t _flags,
                uint32_t _hash, const char * _name, uint32_t _offset ) {
                type               = _type;
                structType         = _structType;
                enumType           = _enumType;
                annotation_or_name = _annotation_or_name;
                firstType          = _firstType;
                secondType         = _secondType;
                dimSize            = _dimSize;
                dim                = _dim;
                flags              = _flags;
                hash               = _hash;
                name               = (char *) _name;
                offset             = _offset;
        }
    };

    struct StructInfo {
        char *      name;
        VarInfo **  fields;
        uint32_t    count;
        uint32_t    size;
        int32_t     initializer;
        uint32_t    hash;
    };

    struct EnumValueInfo {
        char *      name;
        int32_t     value;
    };

    struct EnumInfo {
        char *              name;
        EnumValueInfo **    fields;
        uint32_t            count;
        uint32_t            hash;
    };

    struct FuncInfo {
        char *      name;
        VarInfo **  args;
        uint32_t    argsSize;
        uint32_t    stackSize;
        TypeInfo *  result;
        uint32_t    hash;
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
    ,   refAddresses =          (1<<3)

    ,   string_builder  =   PrintFlags::none
    ,   debugger        =   PrintFlags::escapeString | PrintFlags::namesAndDimensions
        | PrintFlags::typeQualifiers | PrintFlags::refAddresses
    ,   stackwalker     =   PrintFlags::escapeString | PrintFlags::namesAndDimensions | PrintFlags::typeQualifiers
    };

    string debug_type ( TypeInfo * info );
}

