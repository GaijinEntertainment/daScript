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
        fakeLineInfo,
        anyArgument,
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
        tBitfield,
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

    enum class AllowSubstitute   {
        no,
        yes
    };

    struct StructInfo;
    struct TypeAnnotation;
    struct EnumInfo;

    struct BasicAnnotation : ptr_ref_count {
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
        const char * getSource() const { return source; }
        string                name;
        const char *          source = nullptr;
        uint32_t              sourceLength = 0;
        int32_t               tabSize = 4;
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

    class FileAccess : public ptr_ref_count {
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
    typedef smart_ptr<FileAccess> FileAccessPtr;

    struct SimFunction;
    class Context;

    class ModuleFileAccess : public FileAccess {
    public:
        ModuleFileAccess();
        ModuleFileAccess ( const string & pak, const FileAccessPtr & access );
        virtual ~ModuleFileAccess();
        bool failed() const { return !context || !modGet; }
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const override;
        virtual string getIncludeFileName ( const string & fileName, const string & incFileName ) const override;
    protected:
        Context *           context = nullptr;
        SimFunction *       modGet = nullptr;
        SimFunction *       includeGet = nullptr;
    };

    struct LineInfo {
        LineInfo() = default;
        LineInfo(FileInfo * fi, int c, int l, int lc, int ll)
            : fileInfo(fi)
            , column(uint32_t(c)), line(uint32_t(l))
            , last_column(uint32_t(lc)), last_line(uint32_t(ll)) {}
        bool operator < ( const LineInfo & info ) const;
        bool operator == ( const LineInfo & info ) const;
        bool operator != ( const LineInfo & info ) const;
        bool inside ( const LineInfo & info ) const;
        string describe(bool fully = false) const;
        FileInfo *  fileInfo = nullptr;
        uint32_t    column = 0, line = 0;
        uint32_t    last_column = 0, last_line = 0;
    };

    struct LineInfoArg : LineInfo {};

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
            flag_hasInitValue = 1<<9,
            flag_isSmartPtr = 1<<10,
        };
        Type                type;
        StructInfo *        structType;
        EnumInfo *          enumType;
        mutable TypeAnnotation * annotation_or_name;    // WARNING: unresolved. use 'getAnnotation'
        TypeInfo *          firstType;      // map  from, or array
        TypeInfo *          secondType;     // map  to
        TypeInfo **         argTypes;
        char **             argNames;
        uint32_t            argCount;
        uint32_t            dimSize;
        uint32_t *          dim;
        uint32_t            flags;
        uint32_t            hash;
        __forceinline bool isRef() const { return flags & flag_ref; }
        __forceinline bool isRefType() const { return flags & flag_refType; }
        __forceinline bool isRefValue() const { return flags & flag_refValue; }
        __forceinline bool canCopy() const { return flags & flag_canCopy; }
        __forceinline bool isPod() const { return flags & flag_isPod; }
        __forceinline bool isRawPod() const { return flags & flag_isRawPod; }
        __forceinline bool isConst() const { return flags & flag_isConst; }
        __forceinline bool isTemp() const { return flags & flag_isTemp; }
        __forceinline bool isImplicit() const { return flags & flag_isImplicit; }
        __forceinline bool isSmartPtr() const { return flags & flag_isSmartPtr; }
        TypeAnnotation * getAnnotation() const;
    };

    struct VarInfo : TypeInfo {
        char *                      name;
        uint32_t                    offset;
        void *                      annotation_arguments = nullptr;
        vec4f                       value = v_zero();
        char *                      sValue = nullptr;
        VarInfo() = default;
        VarInfo(Type _type, StructInfo * _structType, EnumInfo * _enumType, TypeAnnotation * _annotation_or_name,
                TypeInfo * _firstType, TypeInfo * _secondType, TypeInfo ** _argTypes, char ** _argNames, uint32_t _argCount,
                uint32_t _dimSize, uint32_t * _dim, uint32_t _flags,
                uint32_t _hash, const char * _name, uint32_t _offset ) {
                type               = _type;
                structType         = _structType;
                enumType           = _enumType;
                annotation_or_name = _annotation_or_name;
                firstType          = _firstType;
                secondType         = _secondType;
                argTypes           = _argTypes;
                argNames           = _argNames;
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

    struct LocalVariableInfo : TypeInfo {
        char *      name;
        uint32_t    stackTop;
        LineInfo    visibility;
        union {
            struct {
                bool    cmres : 1;
            };
            uint32_t    localFlags;
        };
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
        LocalVariableInfo **  locals;
        uint32_t    localCount;
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
    ,   humanReadable =         (1<<4)

    ,   string_builder  =   PrintFlags::none
    ,   debugger        =   PrintFlags::escapeString | PrintFlags::namesAndDimensions
            | PrintFlags::humanReadable | PrintFlags::typeQualifiers | PrintFlags::refAddresses
    ,   stackwalker     =   PrintFlags::escapeString | PrintFlags::namesAndDimensions
            | PrintFlags::typeQualifiers | PrintFlags::humanReadable
    };

    string debug_type ( TypeInfo * info );
}

