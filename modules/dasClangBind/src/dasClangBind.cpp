#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"

#include "dasClangBind.h"

MAKE_TYPE_FACTORY(CXTranslationUnitImpl,CXTranslationUnitImpl);
MAKE_TYPE_FACTORY(CXCursor,CXCursor);
MAKE_TYPE_FACTORY(CXString,CXString);
MAKE_TYPE_FACTORY(CXUnsavedFile,CXUnsavedFile);
MAKE_TYPE_FACTORY(CXSourceLocation,CXSourceLocation);
MAKE_TYPE_FACTORY(CXType,CXType);

DAS_BIND_ENUM_CAST(CXChildVisitResult);
DAS_BASE_BIND_ENUM(CXChildVisitResult, CXChildVisitResult, CXChildVisit_Break, CXChildVisit_Continue, CXChildVisit_Recurse);

DAS_BIND_ENUM_CAST(CXCursorKind);
DAS_BASE_BIND_ENUM_FACTORY(CXCursorKind,"CXCursorKind");

DAS_BIND_ENUM_CAST(CXTypeKind);
DAS_BASE_BIND_ENUM_FACTORY(CXTypeKind,"CXTypeKind");

#include "dasClangBind_enums.inc"

namespace das {

struct CXType_Annotation : ManagedStructureAnnotation <CXType> {
    CXType_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXType", ml) {
        addField<DAS_BIND_MANAGED_FIELD(kind)>("kind");
    }
};
template <> struct cast_arg<CXType> {
    static __forceinline CXType to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        return * cast<CXType *>::to(res);
    }
};

struct CXSourceLocation_Annotation : ManagedStructureAnnotation <CXSourceLocation> {
    CXSourceLocation_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceLocation", ml) {
    }
};
template <> struct cast_arg<CXSourceLocation> {
    static __forceinline CXSourceLocation to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        return * cast<CXSourceLocation *>::to(res);
    }
};


struct CXIndex_Annotation : ManagedStructureAnnotation <CXIndex> {
    CXIndex_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIndex", ml) {
    }
};

struct CXCursor_Annotation : ManagedStructureAnnotation <CXCursor> {
    CXCursor_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursor", ml) {
        addField<DAS_BIND_MANAGED_FIELD(kind)>("kind");
    }
};
template <> struct cast_arg<CXCursor> {
    static __forceinline CXCursor to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        return * cast<CXCursor *>::to(res);
    }
};

struct CXString_Annotation : ManagedStructureAnnotation <CXString> {
    CXString_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXString", ml) {
    }
};

struct CXUnsavedFile_Annotation : ManagedStructureAnnotation <CXUnsavedFile> {
    CXUnsavedFile_Annotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXUnsavedFile", ml) {
    }
};

char * from_CXString_to_string ( const CXString & cxs, Context * context ) {
    auto cst = clang_getCString(cxs);
    auto res = context->stringHeap->allocateString(cst);
    clang_disposeString(cxs);
    return res;
}

void peek_CXString ( const CXString & cxs, const TBlock<void,TTemporary<const char *>> & blk, Context * context, LineInfoArg * arg ) {
    auto cst = clang_getCString(cxs);
    das_invoke<void>::invoke<const char *>(context,arg,blk,cst);
    clang_disposeString(cxs);
}

struct vchGoo {
    Block           blk;
    Context *       ctx;
    LineInfoArg *   at;
};

void das_clangVisitChildren ( CXCursor cursor, const TBlock<CXChildVisitResult,CXCursor,CXCursor> & block, Context * ctx, LineInfoArg * at ) {
    vchGoo goo {block, ctx, at};
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            vchGoo * goo = (vchGoo *) client_data;
            return das_invoke<CXChildVisitResult>::invoke<const CXCursor &,const CXCursor &>(goo->ctx,goo->at,goo->blk,c,parent);
        },
        &goo
    );
};

class Module_ClangBind : public Module {
public:
    void addVoidAlias ( const string & aliasName ) {
        auto CXIndexType = make_smart<TypeDecl>(Type::tPointer);
        CXIndexType->alias = aliasName;
        addAlias(CXIndexType);
    }
    Module_ClangBind() : Module("cbind") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        // enums
        addEnumeration(make_smart<EnumerationCXChildVisitResult>());
        addEnumeration(make_smart<EnumerationCXCursorKind>());
        addEnumeration(make_smart<EnumerationCXTypeKind>());
        // aliases for the handles
        addVoidAlias("CXIndex");
        addVoidAlias("CXClientData");
        addVoidAlias("CXFile");
        // annotations
        addAnnotation(make_smart<CXString_Annotation>(lib));
        addAnnotation(make_smart<CXIndex_Annotation>(lib));
        addAnnotation(make_smart<DummyTypeAnnotation>("CXTranslationUnitImpl","CXTranslationUnitImpl",1,1));
        addAnnotation(make_smart<CXCursor_Annotation>(lib));
        addAnnotation(make_smart<CXUnsavedFile_Annotation>(lib));
        addAnnotation(make_smart<CXSourceLocation_Annotation>(lib));
        addAnnotation(make_smart<CXType_Annotation>(lib));
        // string
        addExtern<DAS_BIND_FUN(from_CXString_to_string)>(*this, lib,  "string",
            SideEffects::worstDefault, "from_CXString_to_string")
                ->args({"CXString","context"});
        addExtern<DAS_BIND_FUN(peek_CXString)>(*this, lib,  "peek",
            SideEffects::worstDefault, "peek_CXString")
                ->args({"CXString","blk","context","line"});
        // index
        addExtern<DAS_BIND_FUN(clang_createIndex)>(*this, lib,  "clang_createIndex",
            SideEffects::worstDefault, "clang_createIndex")
                ->args({"excludeDeclarationsFromPCH","displayDiagnostics"});
        addExtern<DAS_BIND_FUN(clang_disposeIndex)>(*this, lib,  "clang_disposeIndex",
            SideEffects::worstDefault, "clang_disposeIndex")
                ->args({"index"});
        // translation unit
        addExtern<DAS_BIND_FUN(clang_parseTranslationUnit)>(*this, lib,  "clang_parseTranslationUnit",
            SideEffects::worstDefault, "clang_parseTranslationUnit")
                ->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options"});
        addExtern<DAS_BIND_FUN(clang_disposeTranslationUnit)>(*this, lib,  "clang_disposeTranslationUnit",
            SideEffects::worstDefault, "clang_disposeTranslationUnit")
                ->args({"unit"});
        addExtern<DAS_BIND_FUN(clang_getTranslationUnitCursor),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getTranslationUnitCursor",
            SideEffects::worstDefault, "clang_getTranslationUnitCursor")
                ->args({"unit"});
        // cursor
        addExtern<DAS_BIND_FUN(das_clangVisitChildren)>(*this, lib,  "clang_visitChildren",
            SideEffects::worstDefault, "das_clangVisitChildren")
                ->args({"cursor","block","context","line"});
        addExtern<DAS_BIND_FUN(clang_getCursorSpelling),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCursorSpelling",
            SideEffects::worstDefault, "clang_getCursorSpelling")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getCursorKind)>(*this, lib,  "clang_getCursorKind",
            SideEffects::worstDefault, "clang_getCursorKind")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getCursorKindSpelling),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCursorKindSpelling",
            SideEffects::worstDefault, "clang_getCursorKindSpelling")
                ->args({"kind"});
        addExtern<DAS_BIND_FUN(clang_getCursorLocation),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCursorLocation",
            SideEffects::worstDefault, "clang_getCursorLocation")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getSpellingLocation)>(*this, lib,  "clang_getSpellingLocation",
            SideEffects::worstDefault, "clang_getSpellingLocation")
                ->args({"location","file","line","column","offset"});
        addExtern<DAS_BIND_FUN(clang_getFileName),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getFileName",
            SideEffects::worstDefault, "clang_getFileName")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getCursorDisplayName),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCursorDisplayName",
            SideEffects::worstDefault, "clang_getCursorDisplayName")
                ->args({"cursor"});
        // type
        addExtern<DAS_BIND_FUN(clang_getCursorType),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCursorType",
            SideEffects::worstDefault, "clang_getCursorType")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getTypeSpelling),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getTypeSpelling",
            SideEffects::worstDefault, "clang_getTypeSpelling")
                ->args({"type"});
        addExtern<DAS_BIND_FUN(clang_getCanonicalType),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getCanonicalType",
            SideEffects::worstDefault, "clang_getCanonicalType")
                ->args({"type"});
        addExtern<DAS_BIND_FUN(clang_getResultType),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getResultType",
            SideEffects::worstDefault, "clang_getResultType")
                ->args({"type"});
        addExtern<DAS_BIND_FUN(clang_getPointeeType),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getPointeeType",
            SideEffects::worstDefault, "clang_getPointeeType")
                ->args({"type"});
        addExtern<DAS_BIND_FUN(clang_isConstQualifiedType)>(*this, lib,  "clang_isConstQualifiedType",
            SideEffects::worstDefault, "clang_isConstQualifiedType")
                ->args({"type"});
        // cursor arguments
        addExtern<DAS_BIND_FUN(clang_Cursor_getNumArguments)>(*this, lib,  "clang_Cursor_getNumArguments",
            SideEffects::worstDefault, "clang_Cursor_getNumArguments")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_Cursor_getArgument),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_Cursor_getArgument",
            SideEffects::worstDefault, "clang_Cursor_getArgument")
                ->args({"cursor","index"});
        // enum
        addExtern<DAS_BIND_FUN(clang_getEnumDeclIntegerType),SimNode_ExtFuncCallAndCopyOrMove>(*this, lib,  "clang_getEnumDeclIntegerType",
            SideEffects::worstDefault, "clang_getEnumDeclIntegerType")
                ->args({"type"});
        addExtern<DAS_BIND_FUN(clang_getEnumConstantDeclValue)>(*this, lib,  "clang_getEnumConstantDeclValue",
            SideEffects::worstDefault, "clang_getEnumConstantDeclValue")
                ->args({"cursor"});
        addExtern<DAS_BIND_FUN(clang_getEnumConstantDeclUnsignedValue)>(*this, lib,  "clang_getEnumConstantDeclUnsignedValue",
            SideEffects::worstDefault, "clang_getEnumConstantDeclUnsignedValue")
                ->args({"cursor"});
    }
};

}

REGISTER_MODULE_IN_NAMESPACE(Module_ClangBind,das);

