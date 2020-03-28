#pragma once

namespace das
{

#ifndef DAS_ENABLE_PROFILER
#define DAS_ENABLE_PROFILER 0
#endif

    enum Type : int32_t {
        none,
        autoinfer,
        alias,
        fakeContext,
        anyArgument,
        declType,
        tVoid,
        tBool,
        tInt8,
        tUInt8,
        tInt16,
        tUInt16,
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
        tEnumeration8,
        tEnumeration16,
        tPointer,
        tFunction,
        tLambda,
        tIterator,
        tArray,
        tTable,
        tBlock,
        tTuple,
        tVariant
    };

    enum class RefMatters {
        no
    ,   yes
    };

    enum class ConstMatters {
        no
    ,   yes
    };

    enum class TemporaryMatters {
        no
    ,   yes
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
        void reserveProfileData();
        string                name;
        const char *          source = nullptr;
        uint32_t              sourceLength = 0;
#if DAS_ENABLE_PROFILER
        vector<uint64_t>      profileData;
#endif
    };
    typedef unique_ptr<FileInfo> FileInfoPtr;

    struct ModuleInfo {
        string  moduleName;
        string  fileName;
        string  importName;
    };

    class FileAccess {
    public:
        virtual ~FileAccess() {}
        FileInfo * setFileInfo ( const string & fileName, FileInfoPtr && info );
        FileInfo * getFileInfo ( const string & fileName );
        virtual string getIncludeFileName ( const string & fileName, const string & incFileName ) const;
        void freeSourceData();
        FileInfoPtr letGoOfFileInfo ( const string & fileName );
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const;
    protected:
        virtual FileInfo * getNewFileInfo ( const string & ) { return nullptr; }
    protected:
        das_map<string, FileInfoPtr>    files;
    };
    typedef shared_ptr<FileAccess> FileAccessPtr;

    struct SimFunction;
    class Context;

    class ModuleFileAccess : public FileAccess {
    public:
        ModuleFileAccess();
        ModuleFileAccess ( const string & pak, const FileAccessPtr & access );
        virtual ~ModuleFileAccess();
        bool failed() const { return !context || !modGet; }
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const;
    protected:
        Context *           context = nullptr;
        SimFunction *       modGet = nullptr;
    };

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
            flag_isTemp = 1<<6,
            flag_isImplicit = 1<<7,
            flag_refValue = 1<<8,
        };
        Type                type;
        StructInfo *        structType;
        EnumInfo *          enumType;
        TypeAnnotation *    annotation_or_name;
        TypeInfo *          firstType;      // map  from, or array
        TypeInfo *          secondType;     // map  to
        TypeInfo **         argTypes;
        uint32_t            argCount;
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
        __forceinline bool isTemp() const { return flags & flag_isTemp; }
        __forceinline bool isImplicit() const { return flags & flag_isImplicit; }
    };

    TypeAnnotation * resolveAnnotation ( TypeInfo * info );

    struct VarInfo : TypeInfo {
        char *                      name;
        uint32_t                    offset;
        void *                      annotation_arguments = nullptr;
        union {
            vec4f       value = v_zero();
            bool        bValue;
            int         iValue;
            float       fValue;
        };
        char *          sValue = nullptr;
        VarInfo() = default;
        VarInfo(Type _type, StructInfo * _structType, EnumInfo * _enumType, TypeAnnotation * _annotation_or_name, 
                TypeInfo * _firstType, TypeInfo * _secondType, TypeInfo ** _argTypes, uint32_t _argCount,
                uint32_t _dimSize, uint32_t * _dim, uint32_t _flags,
                uint32_t _hash, const char * _name, uint32_t _offset ) {
                type               = _type;
                structType         = _structType;
                enumType           = _enumType;
                annotation_or_name = _annotation_or_name;
                firstType          = _firstType;
                secondType         = _secondType;
                argTypes           = _argTypes;
                argCount           = _argCount;
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
        void *      annotation_list;
        uint32_t    hash;
    };

    struct EnumValueInfo {
        char *      name;
        int64_t     value;
    };

    struct EnumInfo {
        char *              name;
        EnumValueInfo **    fields;
        uint32_t            count;
        uint32_t            hash;
    };

    struct FuncInfo {
        enum {
            flag_init = (1<<0)
        ,   flag_builtin = (1<<1)
        };
        char *      name;
        char *      cppName;
        VarInfo **  fields;
        uint32_t    count;
        uint32_t    stackSize;
        TypeInfo *  result;
        uint32_t    hash;
        uint32_t    flags;
    };

    string das_to_string ( Type t );
    Type nameToBasicType(const string & name);

    int getTypeBaseSize ( Type type );
    int getTypeBaseAlign ( Type type );
    int getTypeBaseSize ( TypeInfo * info );
    int getDimSize ( TypeInfo * info );
    int getTypeSize ( TypeInfo * info );
    int getTypeAlign ( TypeInfo * info );

    bool isSameType ( const TypeInfo * THIS, const TypeInfo * decl, RefMatters refMatters, ConstMatters constMatters, TemporaryMatters temporaryMatters, bool topLevel );
    bool isCompatibleCast ( const StructInfo * THIS, const StructInfo * castS );
    bool isValidArgumentType ( TypeInfo * argType, TypeInfo * passType );

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

