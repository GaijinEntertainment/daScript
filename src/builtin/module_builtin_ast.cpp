#include "daScript/misc/platform.h"

#include "module_builtin_rtti.h"

#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/ast/ast_generate.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/simulate/aot_builtin_string.h"
#include "daScript/misc/performance_time.h"

#include "module_builtin_ast.h"

using namespace das;

namespace das {
    class VisitorAdapter;
};

IMPLEMENT_EXTERNAL_TYPE_FACTORY(TypeDecl,TypeDecl)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(FieldDeclaration, Structure::FieldDeclaration)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Structure,Structure)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(EnumEntry,Enumeration::EnumEntry)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Enumeration,Enumeration)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Expression,Expression)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Function,Function)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(InferHistory, InferHistory)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(Variable,Variable)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(VisitorAdapter,VisitorAdapter)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(FunctionAnnotation,FunctionAnnotation)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(StructureAnnotation,StructureAnnotation)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(EnumerationAnnotation,EnumerationAnnotation)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(PassMacro,PassMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(VariantMacro,VariantMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ReaderMacro,ReaderMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(CallMacro,CallMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ModuleGroup,ModuleGroup)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ModuleLibrary,ModuleLibrary)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(AstContext,AstContext)

IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprBlock,ExprBlock)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprLet,ExprLet)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprStringBuilder,ExprStringBuilder)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprNew,ExprNew)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprNamedCall,ExprNamedCall)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(MakeFieldDecl,MakeFieldDecl)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(MakeStruct,MakeStruct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprCall,ExprCall)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprCallFunc,ExprCallFunc)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprLooksLikeCall,ExprLooksLikeCall)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprNullCoalescing,ExprNullCoalescing)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAt,ExprAt)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprSafeAt,ExprSafeAt)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprPtr2Ref,ExprPtr2Ref)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprIs,ExprIs)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprOp,ExprOp)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprOp2,ExprOp2)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprOp3,ExprOp3)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprCopy,ExprCopy)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMove,ExprMove)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprClone,ExprClone)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprWith,ExprWith)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAssume,ExprAssume)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprWhile,ExprWhile)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprTryCatch,ExprTryCatch)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprIfThenElse,ExprIfThenElse)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprFor,ExprFor)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeLocal,ExprMakeLocal)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeVariant,ExprMakeVariant)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeStruct,ExprMakeStruct)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeArray,ExprMakeArray)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeTuple,ExprMakeTuple)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprArrayComprehension,ExprArrayComprehension)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(TypeInfoMacro,TypeInfoMacro);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprTypeInfo,ExprTypeInfo)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprTypeDecl,ExprTypeDecl)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprLabel,ExprLabel);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprGoto,ExprGoto);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprRef2Value,ExprRef2Value);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprRef2Ptr,ExprRef2Ptr);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAddr,ExprAddr);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAssert,ExprAssert);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprStaticAssert,ExprStaticAssert);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprQuote,ExprQuote);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprDebug,ExprDebug);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprInvoke,ExprInvoke);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprErase,ExprErase);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprFind,ExprFind);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprKeyExists,ExprKeyExists);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAscend,ExprAscend);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprCast,ExprCast);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprDelete,ExprDelete);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprVar,ExprVar);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprSwizzle,ExprSwizzle);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprField,ExprField);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprSafeField,ExprSafeField);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprIsVariant,ExprIsVariant);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprAsVariant,ExprAsVariant);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprSafeAsVariant,ExprSafeAsVariant);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprOp1,ExprOp1);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprReturn,ExprReturn);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprYield,ExprYield);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprBreak,ExprBreak);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprContinue,ExprContinue);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConst,ExprConst);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprFakeContext,ExprFakeContext);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprFakeLineInfo,ExprFakeLineInfo);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstPtr,ExprConstPtr);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstEnumeration,ExprConstEnumeration);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstBitfield,ExprConstBitfield);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt8,ExprConstInt8);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt16,ExprConstInt16);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt64,ExprConstInt64);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt,ExprConstInt);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt2,ExprConstInt2);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt3,ExprConstInt3);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstInt4,ExprConstInt4);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt8,ExprConstUInt8);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt16,ExprConstUInt16);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt64,ExprConstUInt64);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt,ExprConstUInt);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt2,ExprConstUInt2);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt3,ExprConstUInt3);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstUInt4,ExprConstUInt4);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstRange,ExprConstRange);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstURange,ExprConstURange);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstBool,ExprConstBool);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstFloat,ExprConstFloat);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstFloat2,ExprConstFloat2);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstFloat3,ExprConstFloat3);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstFloat4,ExprConstFloat4);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstString,ExprConstString);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprConstDouble,ExprConstDouble);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(CaptureEntry,CaptureEntry);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeBlock,ExprMakeBlock);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMakeGenerator,ExprMakeGenerator);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprMemZero,ExprMemZero);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprReader,ExprReader);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprCallMacro,ExprCallMacro);
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ExprUnsafe,ExprUnsafe);

DAS_BASE_BIND_ENUM(das::SideEffects, SideEffects,
    none, unsafe, userScenario, modifyExternal, accessExternal, modifyArgument,
    modifyArgumentAndExternal, worstDefault, accessGlobal, invoke, inferredSideEffects)

DAS_BASE_BIND_ENUM(das::CaptureMode, CaptureMode,
    capture_any, capture_by_copy, capture_by_reference, capture_by_clone, capture_by_move)

namespace das {
    TypeDeclPtr makeExprGenFlagsFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprGenFlags";
        ft->argNames = { "alwaysSafe", "generated", "userSaidItsSafe" };
        return ft;
    }

    TypeDeclPtr makeExprFlagsFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprFlags";
        ft->argNames = { "constexpression", "noSideEffects", "noNativeSideEffects" };
        return ft;
    }

    TypeDeclPtr makeExprPrintFlagsFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprPrintFlags";
        ft->argNames = { "topLevel", "argLevel", "bottomLevel" };
        return ft;
    }

    void init_expr ( BasicStructureAnnotation & ann ) {
        ann.addFieldEx("at", "at", offsetof(Expression, at), makeType<LineInfo>(*ann.mlib));
        ann.addFieldEx("_type", "type", offsetof(Expression, type), makeType<TypeDeclPtr>(*ann.mlib));
        ann.addFieldEx("__rtti", "__rtti", offsetof(Expression, __rtti), makeType<const char *>(*ann.mlib));
        ann.addFieldEx("genFlags", "genFlags", offsetof(Expression, genFlags), makeExprGenFlagsFlags());
        ann.addFieldEx("flags", "flags", offsetof(Expression, flags), makeExprFlagsFlags());
        ann.addFieldEx("printFlags", "printFlags", offsetof(Expression, printFlags), makeExprPrintFlagsFlags());
    }

    bool canSubstituteExpr ( const TypeAnnotation* thisAnn, TypeAnnotation* ann ) {
        if (ann == thisAnn) return true;
        if (thisAnn->module != ann->module) return false;
        if (memcmp(ann->name.c_str(), "Expr", 4) != 0) return false;
        auto* AEA = static_cast<BasicStructureAnnotation*>(ann);
        for (auto p : AEA->parents) {
            if (p == thisAnn) return true;
        }
        return false;
    }

    template <typename EXPR>
    struct AstExprAnnotation : ManagedStructureAnnotation <EXPR> {
        const char * parentExpression = nullptr;
        AstExprAnnotation(const string & en, ModuleLibrary & ml)
            : ManagedStructureAnnotation<EXPR> (en, ml) {
        }
        __forceinline void init() {
            init_expr(*this);
        }
        virtual bool canSubstitute ( TypeAnnotation * ann ) const override {
            return canSubstituteExpr(this, ann);
        }
    };

    template <typename EXPR>
    struct AstExpressionAnnotation : AstExprAnnotation<EXPR> {
        AstExpressionAnnotation(const string & en, ModuleLibrary & ml)
            :  AstExprAnnotation<EXPR> (en, ml) {
            this->init();
        }
    };

    TypeDeclPtr makeExprBlockFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprBlockFlags";
        ft->argNames = { "isClosure", "hasReturn", "copyOnReturn", "moveOnReturn",
            "inTheLoop", "finallyBeforeBody", "finallyDisabled","aotSkipMakeBlock",
            "aotDoNotSkipAnnotationData", "isCollapseable", "needCollapse" };
        return ft;
    }

    struct AstExprBlockAnnotation : AstExpressionAnnotation<ExprBlock> {
        AstExprBlockAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprBlock> ("ExprBlock", ml) {
            addField<DAS_BIND_MANAGED_FIELD(list)>("list");
            addField<DAS_BIND_MANAGED_FIELD(finalList)>("finalList");
            addField<DAS_BIND_MANAGED_FIELD(returnType)>("returnType");
            addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(stackVarTop)>("stackVarTop");
            addField<DAS_BIND_MANAGED_FIELD(stackVarBottom)>("stackVarBottom");
            addField<DAS_BIND_MANAGED_FIELD(stackCleanVars)>("stackCleanVars");
            addField<DAS_BIND_MANAGED_FIELD(maxLabelIndex)>("maxLabelIndex");
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addField<DAS_BIND_MANAGED_FIELD(annotationData)>("annotationData");
            addField<DAS_BIND_MANAGED_FIELD(annotationDataSid)>("annotationDataSid");
            addFieldEx ( "blockFlags", "blockFlags", offsetof(ExprBlock, blockFlags), makeExprBlockFlags() );
        }
    };

    struct AstExprLetAnnotation : AstExpressionAnnotation<ExprLet> {
        AstExprLetAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprLet> ("ExprLet", ml) {
            addField<DAS_BIND_MANAGED_FIELD(variables)>("variables");
            addField<DAS_BIND_MANAGED_FIELD(atInit)>("atInit");
        }
    };

    struct AstExprStringBuilderAnnotation : AstExpressionAnnotation<ExprStringBuilder> {
        AstExprStringBuilderAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprStringBuilder> ("ExprStringBuilder", ml) {
            addField<DAS_BIND_MANAGED_FIELD(elements)>("elements");
        }
    };

    TypeDeclPtr makeMakeFieldDeclFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "MakeFieldDeclFlags";
        ft->argNames = { "moveSemantics", "cloneSemantics" };
        return ft;
    }

    struct AstMakeFieldDeclAnnotation : ManagedStructureAnnotation<MakeFieldDecl> {
        AstMakeFieldDeclAnnotation(ModuleLibrary & ml)
            :  ManagedStructureAnnotation<MakeFieldDecl> ("MakeFieldDecl", ml) {
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(value)>("value");
            addFieldEx ( "flags", "flags", offsetof(MakeFieldDecl, flags), makeMakeFieldDeclFlags() );
        }
    };

    struct AstMakeStructAnnotation : ManagedVectorAnnotation<MakeStruct> {
        AstMakeStructAnnotation(ModuleLibrary & ml)
            :  ManagedVectorAnnotation<MakeStruct> ("MakeStruct", ml) {
        };
        virtual bool isSmart() const override { return true; }
        virtual bool canNew() const override { return true; }
        virtual bool canDeletePtr() const override { return true; }
        virtual string getSmartAnnotationCloneFunction () const override { return "smart_ptr_clone"; }
        virtual SimNode * simulateGetNew ( Context & context, const LineInfo & at ) const override {
            return context.code->makeNode<SimNode_NewHandle<MakeStruct,true>>(at);
        }
        virtual SimNode * simulateDeletePtr ( Context & context, const LineInfo & at, SimNode * sube, uint32_t count ) const override {
            return context.code->makeNode<SimNode_DeleteHandlePtr<MakeStruct,true>>(at,sube,count);
        }
    };

    void init_expr_looks_like_call ( BasicStructureAnnotation & ann ) {
        ann.addFieldEx("name", "name", offsetof(ExprLooksLikeCall, name), makeType<string>(*ann.mlib));
        ann.addFieldEx("arguments", "arguments", offsetof(ExprLooksLikeCall, arguments), makeType<vector<ExpressionPtr>>(*ann.mlib));
        ann.addFieldEx("argumentsFailedToInfer", "argumentsFailedToInfer", offsetof(ExprLooksLikeCall, argumentsFailedToInfer), makeType<bool>(*ann.mlib));
        ann.addFieldEx("atEnclosure", "atEnclosure", offsetof(ExprLooksLikeCall, atEnclosure), makeType<LineInfo>(*ann.mlib));
    }

    template <typename EXPR>
    struct AstExprLooksLikeCallAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprLooksLikeCallAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            init_expr_looks_like_call(*this);
        }
    };

    template <typename EXPR>
    struct AstExprCallFuncAnnotation : AstExprLooksLikeCallAnnotation<EXPR> {
        AstExprCallFuncAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExprLooksLikeCallAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(func)>("func");
            this->template addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
        }
    };

    struct AstExprNamedCallAnnotation : AstExpressionAnnotation<ExprNamedCall> {
        AstExprNamedCallAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprNamedCall> ("ExprNamedCall", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            addField<DAS_BIND_MANAGED_FIELD(argumentsFailedToInfer)>("argumentsFailedToInfer");
        }
    };

    struct AstExprNewAnnotation : AstExprCallFuncAnnotation<ExprNew> {
        AstExprNewAnnotation(ModuleLibrary & ml)
            :  AstExprCallFuncAnnotation<ExprNew> ("ExprNew", ml) {
            addField<DAS_BIND_MANAGED_FIELD(typeexpr)>("typeexpr");
            addField<DAS_BIND_MANAGED_FIELD(initializer)>("initializer");
        }
    };

    struct AstExprCallAnnotation : AstExprCallFuncAnnotation<ExprCall> {
        AstExprCallAnnotation(ModuleLibrary & ml)
            :  AstExprCallFuncAnnotation<ExprCall> ("ExprCall", ml) {
            addField<DAS_BIND_MANAGED_FIELD(doesNotNeedSp)>("doesNotNeedSp");
        }
    };

    template <typename EXPR>
    struct AstExprPtr2RefAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprPtr2RefAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    struct AstExprNullCoalescingAnnotation : AstExprPtr2RefAnnotation<ExprNullCoalescing> {
        AstExprNullCoalescingAnnotation(ModuleLibrary & ml)
            :  AstExprPtr2RefAnnotation<ExprNullCoalescing> ("ExprNullCoalescing", ml) {
            addField<DAS_BIND_MANAGED_FIELD(defaultValue)>("defaultValue");
        }
    };

    TypeDeclPtr makeExprAtFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprAtFlags";
        ft->argNames = { "r2v", "r2cr", "write", "no_promotion" };
        return ft;
    }

    template <typename EXPR>
    struct AstExprAtAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprAtAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            this->template addField<DAS_BIND_MANAGED_FIELD(index)>("index");
            this->addFieldEx ( "atFlags", "atFlags", offsetof(ExprAt, atFlags), makeExprAtFlags() );
        }
    };

    struct AstExprIsAnnotation : AstExpressionAnnotation<ExprIs> {
        AstExprIsAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprIs> ("ExprIs", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(typeexpr)>("typeexpr");
        }
    };

    template <typename EXPR>
    struct AstExprOpAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprOpAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(op)>("op");
        }
    };

    template <typename EXPR>
    struct AstExprOp2Annotation : AstExprOpAnnotation<EXPR> {
        AstExprOp2Annotation(const string & na, ModuleLibrary & ml)
            :  AstExprOpAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(left)>("left");
            this->template addField<DAS_BIND_MANAGED_FIELD(right)>("right");
        }
    };

    struct AstExprOp3Annotation : AstExprOpAnnotation<ExprOp3> {
        AstExprOp3Annotation(ModuleLibrary & ml)
            :  AstExprOpAnnotation<ExprOp3> ("ExprOp3", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(left)>("left");
            addField<DAS_BIND_MANAGED_FIELD(right)>("right");
        }
    };

    struct AstExprCopyAnnotation : AstExprOp2Annotation<ExprCopy> {
        AstExprCopyAnnotation(ModuleLibrary & ml)
            :  AstExprOp2Annotation<ExprCopy> ("ExprCopy", ml) {
            addField<DAS_BIND_MANAGED_FIELD(takeOverRightStack)>("takeOverRightStack");
        }
    };

    struct AstExprWithAnnotation : AstExpressionAnnotation<ExprWith> {
        AstExprWithAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprWith> ("ExprWith", ml) {
            addField<DAS_BIND_MANAGED_FIELD(with)>("_with", "with");
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
        }
    };

    struct AstExprAssumeAnnotation : AstExpressionAnnotation<ExprAssume> {
        AstExprAssumeAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprAssume> ("ExprAssume", ml) {
            addField<DAS_BIND_MANAGED_FIELD(alias)>("alias", "alias");
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    struct AstExprWhileAnnotation : AstExpressionAnnotation<ExprWhile> {
        AstExprWhileAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprWhile> ("ExprWhile", ml) {
            addField<DAS_BIND_MANAGED_FIELD(cond)>("cond");
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
        }
    };

    struct AstExprTryCatchAnnotation : AstExpressionAnnotation<ExprTryCatch> {
        AstExprTryCatchAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprTryCatch> ("ExprTryCatch", ml) {
            addField<DAS_BIND_MANAGED_FIELD(try_block)>("try_block");
            addField<DAS_BIND_MANAGED_FIELD(catch_block)>("catch_block");
        }
    };

    struct AstExprForAnnotation : AstExpressionAnnotation<ExprFor> {
        AstExprForAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprFor> ("ExprFor", ml) {
            addField<DAS_BIND_MANAGED_FIELD(iterators)>("iterators");
            addField<DAS_BIND_MANAGED_FIELD(iteratorsAka)>("iteratorsAka");
            addField<DAS_BIND_MANAGED_FIELD(iteratorsAt)>("iteratorsAt");
            addField<DAS_BIND_MANAGED_FIELD(iteratorVariables)>("iteratorVariables");
            addField<DAS_BIND_MANAGED_FIELD(sources)>("sources");
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
            addField<DAS_BIND_MANAGED_FIELD(visibility)>("visibility");
            addField<DAS_BIND_MANAGED_FIELD(allowIteratorOptimization)>("allowIteratorOptimization");
        }
    };

    struct AstExprIfThenElseAnnotation : AstExpressionAnnotation<ExprIfThenElse> {
        AstExprIfThenElseAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprIfThenElse> ("ExprIfThenElse", ml) {
            addField<DAS_BIND_MANAGED_FIELD(cond)>("cond");
            addField<DAS_BIND_MANAGED_FIELD(if_true)>("if_true");
            addField<DAS_BIND_MANAGED_FIELD(if_false)>("if_false");
            addField<DAS_BIND_MANAGED_FIELD(isStatic)>("isStatic");
        }
    };

    TypeDeclPtr makeExprMakeLocalFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprMakeLocalFlags";
        ft->argNames = { "useStackRef", "useCMRES", "doesNotNeedSp",
            "doesNotNeedInit", "initAllFields" };
        return ft;
    }

    template <typename EXPR>
    struct AstExprMakeLocalAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprMakeLocalAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(makeType)>("makeType");
            this->template addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            this->template addField<DAS_BIND_MANAGED_FIELD(extraOffset)>("extraOffset");
            this->addFieldEx ( "makeFlags", "makeFlags", offsetof(ExprMakeLocal, makeFlags), makeExprMakeLocalFlags() );
        }
    };

    TypeDeclPtr makeExprMakeStructFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprMakeStructFlags";
        ft->argNames = { "useInitializer", "isNewHandle" };
        return ft;
    }

    struct AstExprMakeStructAnnotation : AstExprMakeLocalAnnotation<ExprMakeStruct> {
        AstExprMakeStructAnnotation(ModuleLibrary & ml)
            :  AstExprMakeLocalAnnotation<ExprMakeStruct> ("ExprMakeStruct", ml) {
            addField<DAS_BIND_MANAGED_FIELD(structs)>("structs");
            addField<DAS_BIND_MANAGED_FIELD(block)>("_block","block");
            this->addFieldEx ( "makeStructFlags", "makeStructFlags", offsetof(ExprMakeStruct, makeStructFlags), makeExprMakeStructFlags() );
        }
    };

    struct AstExprMakeVariantAnnotation : AstExprMakeLocalAnnotation<ExprMakeVariant> {
        AstExprMakeVariantAnnotation(ModuleLibrary & ml)
            :  AstExprMakeLocalAnnotation<ExprMakeVariant> ("ExprMakeVariant", ml) {
            addField<DAS_BIND_MANAGED_FIELD(variants)>("variants");
        }
    };

    template <typename EXPR>
    struct AstExprMakeArrayAnnotation : AstExprMakeLocalAnnotation<EXPR> {
        AstExprMakeArrayAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExprMakeLocalAnnotation<EXPR> (na, ml) {
                using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(recordType)>("recordType");
            this->template addField<DAS_BIND_MANAGED_FIELD(values)>("values");
        }
    };

    struct AstExprMakeTupleAnnotation : AstExprMakeArrayAnnotation<ExprMakeTuple> {
        AstExprMakeTupleAnnotation(ModuleLibrary & ml)
            :  AstExprMakeArrayAnnotation<ExprMakeTuple> ("ExprMakeTuple", ml) {
            addField<DAS_BIND_MANAGED_FIELD(isKeyValue)>("isKeyValue");
        }
    };

    struct AstExprArrayComprehensionAnnotation : AstExpressionAnnotation<ExprArrayComprehension> {
        AstExprArrayComprehensionAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprArrayComprehension> ("ExprArrayComprehension", ml) {
            addField<DAS_BIND_MANAGED_FIELD(exprFor)>("exprFor");
            addField<DAS_BIND_MANAGED_FIELD(exprWhere)>("exprWhere");
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(generatorSyntax)>("generatorSyntax");
        }
    };

    struct AstTypeInfoMacroAnnotation : ManagedStructureAnnotation<TypeInfoMacro,false,true> {
        AstTypeInfoMacroAnnotation(ModuleLibrary & ml)
            :  ManagedStructureAnnotation<TypeInfoMacro,false,true> ("TypeInfoMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };

    struct AstExprTypeInfoAnnotation : AstExpressionAnnotation<ExprTypeInfo> {
        AstExprTypeInfoAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprTypeInfo> ("ExprTypeInfo", ml) {
            addField<DAS_BIND_MANAGED_FIELD(trait)>("trait");
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(typeexpr)>("typeexpr");
            addField<DAS_BIND_MANAGED_FIELD(subtrait)>("subtrait");
            addField<DAS_BIND_MANAGED_FIELD(extratrait)>("extratrait");
            addField<DAS_BIND_MANAGED_FIELD(macro)>("macro");
        }
    };

    struct AstExprTypeDeclAnnotation : AstExpressionAnnotation<ExprTypeDecl> {
        AstExprTypeDeclAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprTypeDecl> ("ExprTypeDecl", ml) {
            addField<DAS_BIND_MANAGED_FIELD(typeexpr)>("typeexpr");
        }
    };

    struct AstExprLabelAnnotation : AstExpressionAnnotation<ExprLabel> {
        AstExprLabelAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprLabel> ("ExprLabel", ml) {
            addField<DAS_BIND_MANAGED_FIELD(label)>("labelName","label");
            addField<DAS_BIND_MANAGED_FIELD(comment)>("comment");
        }
    };

    struct AstExprGotoAnnotation : AstExpressionAnnotation<ExprGoto> {
        AstExprGotoAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprGoto> ("ExprGoto", ml) {
            addField<DAS_BIND_MANAGED_FIELD(label)>("labelName","label");
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    struct AstExprRef2ValueAnnotation : AstExpressionAnnotation<ExprRef2Value> {
        AstExprRef2ValueAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprRef2Value> ("ExprRef2Value", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    struct AstExprRef2PtrAnnotation : AstExpressionAnnotation<ExprRef2Ptr> {
        AstExprRef2PtrAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprRef2Ptr> ("ExprRef2Ptr", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    struct AstExprAddrAnnotation : AstExpressionAnnotation<ExprAddr> {
        AstExprAddrAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprAddr> ("ExprAddr", ml) {
            addField<DAS_BIND_MANAGED_FIELD(target)>("target");
            addField<DAS_BIND_MANAGED_FIELD(funcType)>("funcType");
            addField<DAS_BIND_MANAGED_FIELD(func)>("func");
        }
    };

    template <typename EXPR>
    struct AstExprLikeCallAnnotation : AstExprLooksLikeCallAnnotation<EXPR> {
        AstExprLikeCallAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExprLooksLikeCallAnnotation<EXPR> (na, ml) {
        }
    };

    struct AstExprQuoteAnnotation : AstExprLikeCallAnnotation<ExprQuote> {
        AstExprQuoteAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprQuote> ("ExprQuote", ml) {
        }
    };

    struct AstExprAssertAnnotation : AstExprLikeCallAnnotation<ExprAssert> {
        AstExprAssertAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprAssert> ("ExprAssert", ml) {
            addField<DAS_BIND_MANAGED_FIELD(isVerify)>("isVerify");
        }
    };

    struct AstExprStaticAssertAnnotation : AstExprLikeCallAnnotation<ExprStaticAssert> {
        AstExprStaticAssertAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprStaticAssert> ("ExprStaticAssert", ml) {
        }
    };

    struct AstExprDebugAnnotation : AstExprLikeCallAnnotation<ExprDebug> {
        AstExprDebugAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprDebug> ("ExprDebug", ml) {
        }
    };

    struct AstExprInvokeAnnotation : AstExprLikeCallAnnotation<ExprInvoke> {
        AstExprInvokeAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprInvoke> ("ExprInvoke", ml) {
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(doesNotNeedSp)>("doesNotNeedSp");
            addField<DAS_BIND_MANAGED_FIELD(isInvokeMethod)>("isInvokeMethod");
        }
    };

    struct AstExprEraseAnnotation : AstExprLikeCallAnnotation<ExprErase> {
        AstExprEraseAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprErase> ("ExprErase", ml) {
        }
    };

    struct AstExprFindAnnotation : AstExprLikeCallAnnotation<ExprFind> {
        AstExprFindAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprFind> ("ExprFind", ml) {
        }
    };

    struct AstExprKeyExistsAnnotation : AstExprLikeCallAnnotation<ExprKeyExists> {
        AstExprKeyExistsAnnotation(ModuleLibrary & ml)
            :  AstExprLikeCallAnnotation<ExprKeyExists> ("ExprKeyExists", ml) {
        }
    };

    TypeDeclPtr makeExprAscendFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprAscendFlags";
        ft->argNames = { "useStackRef", "needTypeInfo" };
        return ft;
    }

    struct AstExprAscendAnnotation : AstExpressionAnnotation<ExprAscend> {
        AstExprAscendAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprAscend> ("ExprAscend", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(ascType)>("ascType");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addFieldEx ( "ascendFlags", "ascendFlags", offsetof(ExprAscend, ascendFlags), makeExprAscendFlags() );
        }
    };

    TypeDeclPtr makeExprCastFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprCastFlags";
        ft->argNames = { "upcastCast", "reinterpretCast" };
        return ft;
    }

    struct AstExprCastAnnotation : AstExpressionAnnotation<ExprCast> {
        AstExprCastAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprCast> ("ExprCast", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(castType)>("castType");
            addFieldEx ( "castFlags", "castFlags", offsetof(ExprCast, castFlags), makeExprCastFlags() );
        }
    };

    struct AstExprDeleteAnnotation : AstExpressionAnnotation<ExprDelete> {
        AstExprDeleteAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprDelete> ("ExprDelete", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(native)>("native");
        }
    };

    TypeDeclPtr makeExprVarFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprVarFlags";
        ft->argNames = { "local", "argument", "_block",
            "thisBlock", "r2v", "r2cr", "write" };
        return ft;
    }

    struct AstExprVarAnnotation : AstExpressionAnnotation<ExprVar> {
        AstExprVarAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprVar> ("ExprVar", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(variable)>("variable");
            addField<DAS_BIND_MANAGED_FIELD(pBlock)>("pBlock");
            addField<DAS_BIND_MANAGED_FIELD(argumentIndex)>("argumentIndex");
            addFieldEx ( "varFlags", "varFlags", offsetof(ExprVar, varFlags), makeExprVarFlags() );
        }
    };

    TypeDeclPtr makeExprFieldDerefFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprFieldDerefFlags";
        ft->argNames = { "unsafeDeref", "ignoreCaptureConst" };
        return ft;
    }

    TypeDeclPtr makeExprFieldFieldFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprFieldFieldFlags";
        ft->argNames = { "r2v", "r2cr", "write", "no_promotion" };
        return ft;
    }

    template <typename EXPR>
    struct AstExprFieldAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprFieldAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(value)>("value");
            this->template addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            this->template addField<DAS_BIND_MANAGED_FIELD(atField)>("atField");
            this->template addField<DAS_BIND_MANAGED_FIELD(field)>("field");
            this->template addField<DAS_BIND_MANAGED_FIELD(fieldIndex)>("fieldIndex");
            this->template addField<DAS_BIND_MANAGED_FIELD(annotation)>("annotation");
            this->addFieldEx ( "derefFlags", "derefFlags", offsetof(ExprField, derefFlags), makeExprFieldDerefFlags() );
            this->addFieldEx ( "fieldFlags", "fieldFlags", offsetof(ExprField, fieldFlags), makeExprFieldFieldFlags() );
        }
    };

    struct AstExprSafeFieldAnnotation : AstExprFieldAnnotation<ExprSafeField> {
        AstExprSafeFieldAnnotation(ModuleLibrary & ml)
            :  AstExprFieldAnnotation<ExprSafeField> ("ExprSafeField", ml) {
            addField<DAS_BIND_MANAGED_FIELD(skipQQ)>("skipQQ");
        }
    };

    TypeDeclPtr makeExprSwizzleFieldFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprSwizzleFieldFlags";
        ft->argNames = { "r2v", "r2cr", "write" };
        return ft;
    }

    struct AstExprSwizzleAnnotation : AstExpressionAnnotation<ExprSwizzle> {
        AstExprSwizzleAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprSwizzle> ("ExprSwizzle", ml) {
            addField<DAS_BIND_MANAGED_FIELD(value)>("value");
            addField<DAS_BIND_MANAGED_FIELD(mask)>("mask");
            addField<DAS_BIND_MANAGED_FIELD(fields)>("fields");
            addFieldEx ( "fieldFlags", "fieldFlags", offsetof(ExprSwizzle, fieldFlags), makeExprSwizzleFieldFlags() );
        }
    };

    struct AstExprSafeAsVariantAnnotation : AstExprFieldAnnotation<ExprSafeField> {
        AstExprSafeAsVariantAnnotation(ModuleLibrary & ml)
            :  AstExprFieldAnnotation<ExprSafeField> ("ExprSafeAsVariant", ml) {
            addField<DAS_BIND_MANAGED_FIELD(skipQQ)>("skipQQ");
        }
    };

    struct AstExprOp1Annotation : AstExprOpAnnotation<ExprOp1> {
        AstExprOp1Annotation(ModuleLibrary & ml)
            :  AstExprOpAnnotation<ExprOp1> ("ExprOp1", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
        }
    };

    TypeDeclPtr makeExprYieldFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprYieldFlags";
        ft->argNames = { "moveSemantics" };
        return ft;
    }

    struct AstExprYieldAnnotation : AstExpressionAnnotation<ExprYield> {
        AstExprYieldAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprYield> ("ExprYield", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addFieldEx ( "returnFlags", "returnFlags", offsetof(ExprYield, returnFlags), makeExprYieldFlags() );
        }
    };

    TypeDeclPtr makeExprReturnFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprReturnFlags";
        ft->argNames = { "moveSemantics", "returnReference", "returnInBlock",
            "takeOverRightStack", "returnCallCMRES", "returnCMRES", "fromYield" };
        return ft;
    }

    struct AstExprReturnAnnotation : AstExpressionAnnotation<ExprReturn> {
        AstExprReturnAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprReturn> ("ExprReturn", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(refStackTop)>("refStackTop");
            addField<DAS_BIND_MANAGED_FIELD(block)>("block");
            addFieldEx ( "returnFlags", "returnFlags", offsetof(ExprReturn, returnFlags), makeExprReturnFlags() );
        }
    };

    struct AstCaptureEntryAnnotation : ManagedStructureAnnotation<CaptureEntry> {
        AstCaptureEntryAnnotation(ModuleLibrary & ml)
            :  ManagedStructureAnnotation<CaptureEntry> ("CaptureEntry", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(mode)>("mode");
        }
    };

    TypeDeclPtr makeExprMakeBlockFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprMakeBlockFlags";
        ft->argNames = { "isLambda", "isLocalFunction" };
        return ft;
    }

    struct AstExprMakeBlockAnnotation : AstExpressionAnnotation<ExprMakeBlock> {
        AstExprMakeBlockAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprMakeBlock> ("ExprMakeBlock", ml) {
            addField<DAS_BIND_MANAGED_FIELD(block)>("_block","block");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(capture)>("capture");
            addFieldEx ( "mmFlags", "mmFlags", offsetof(ExprMakeBlock, mmFlags), makeExprMakeBlockFlags() );
        }
    };

    struct AstExprMakeGeneratorAnnotation : AstExprLooksLikeCallAnnotation<ExprMakeGenerator> {
        AstExprMakeGeneratorAnnotation(ModuleLibrary & ml)
            :  AstExprLooksLikeCallAnnotation<ExprMakeGenerator> ("ExprMakeGenerator", ml) {
            addField<DAS_BIND_MANAGED_FIELD(iterType)>("iterType");
            addField<DAS_BIND_MANAGED_FIELD(capture)>("capture");
        }
    };

    template <typename EXPR>
    struct AstExprConstAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprConstAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(baseType)>("baseType");
        }
        template <typename TT>
        void init( ModuleLibrary & ml ) {
            auto cpptype = makeType<TT>(ml);
            string cppname = "cvalue<" + describeCppType(cpptype) + ">()";
            this->addFieldEx ( "value", cppname, offsetof(ExprConst, value), cpptype );
        }
    };

    template <typename EXPR, typename TT>
    struct AstExprConstTAnnotation : AstExprConstAnnotation<EXPR> {
        AstExprConstTAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExprConstAnnotation<EXPR> (na, ml) {
            this->template init<TT>(ml);
        }
    };

    struct AstExprConstEnumerationAnnotation : AstExprConstAnnotation<ExprConstEnumeration> {
        AstExprConstEnumerationAnnotation(ModuleLibrary & ml)
            :  AstExprConstAnnotation<ExprConstEnumeration> ("ExprConstEnumeration", ml) {
            addField<DAS_BIND_MANAGED_FIELD(enumType)>("enumType");
            addField<DAS_BIND_MANAGED_FIELD(text)>("value","text");
        }
    };

    struct AstExprConstBitfieldAnnotation : AstExprConstTAnnotation<ExprConstBitfield,Bitfield> {
        AstExprConstBitfieldAnnotation(ModuleLibrary & ml)
            :  AstExprConstTAnnotation<ExprConstBitfield,Bitfield> ("ExprConstBitfield", ml) {
            addField<DAS_BIND_MANAGED_FIELD(bitfieldType)>("bitfieldType");

        }
    };

    struct AstExprConstStringAnnotation : AstExprConstAnnotation<ExprConstString> {
        AstExprConstStringAnnotation(ModuleLibrary & ml)
            :  AstExprConstAnnotation<ExprConstString> ("ExprConstString", ml) {
            addField<DAS_BIND_MANAGED_FIELD(text)>("value","text");
        }
    };

    struct AstExprReaderAnnotation : AstExpressionAnnotation<ExprReader> {
        AstExprReaderAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprReader> ("ExprReader", ml) {
            addField<DAS_BIND_MANAGED_FIELD(macro)>("macro");
            addField<DAS_BIND_MANAGED_FIELD(sequence)>("sequence");
        }
    };

    struct AstExprCallMacroAnnotation : AstExprLooksLikeCallAnnotation<ExprCallMacro> {
        AstExprCallMacroAnnotation(ModuleLibrary & ml)
            :  AstExprLooksLikeCallAnnotation<ExprCallMacro> ("ExprCallMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(macro)>("macro");
        }
    };

    struct AstExprUnsafeAnnotation : AstExpressionAnnotation<ExprUnsafe> {
        AstExprUnsafeAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprUnsafe> ("ExprUnsafe", ml) {
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
        }
    };

    // CONTEXT
    struct AstContextAnnotation : ManagedStructureAnnotation<AstContext,false> {
        AstContextAnnotation(ModuleLibrary & ml)
            :  ManagedStructureAnnotation ("AstContext", ml) {
            addField<DAS_BIND_MANAGED_FIELD(func)>("func");
            addField<DAS_BIND_MANAGED_FIELD(loop)>("_loop","loop");
            addField<DAS_BIND_MANAGED_FIELD(blocks)>("blocks");
            addField<DAS_BIND_MANAGED_FIELD(scopes)>("scopes");
            addField<DAS_BIND_MANAGED_FIELD(with)>("_with","with");
        }
    };

    void getAstContext ( smart_ptr_raw<Program> prog, smart_ptr_raw<Expression> expr, const TBlock<void,bool,AstContext> & block, Context * context, LineInfoArg * at ) {
        AstContext astc = generateAstContext(prog,expr.get());
        vec4f args[2];
        args[0] = cast<bool>::from ( astc.valid );
        args[1] = astc.valid ? cast<AstContext&>::from(astc) : v_zero();
        context->invoke(block, args, nullptr, at);
    }

    // TYPE STUFF

    struct AstModuleLibraryAnnotation : ManagedStructureAnnotation<ModuleLibrary,false> {
        AstModuleLibraryAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ModuleLibrary", ml) {
        }
    };

    struct AstEnumEntryAnnotation : ManagedStructureAnnotation <Enumeration::EnumEntry> {
        AstEnumEntryAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("EnumEntry", ml, "Enumeration::EnumEntry") {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(value)>("value");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
        }
    };

    struct AstEnumerationAnnotation : ManagedStructureAnnotation <Enumeration> {
        AstEnumerationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("Enumeration", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(cppName)>("cppName");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(list)>("list");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module","module");
            addField<DAS_BIND_MANAGED_FIELD(external)>("external");
            addField<DAS_BIND_MANAGED_FIELD(baseType)>("baseType");
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addField<DAS_BIND_MANAGED_FIELD(isPrivate)>("isPrivate");
        }
    };

    TypeDeclPtr makeTypeDeclFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "TypeDeclFlags";
        ft->argNames = { "ref", "constant", "temporary", "_implicit",
            "removeRef", "removeConstant", "removeDim",
            "removeTemporary", "explicitConst", "aotAlias", "smartPtr",
            "smartPtrNative", "isExplicit", "isNativeDim" };
        return ft;
    }

    struct AstTypeDeclAnnnotation : ManagedStructureAnnotation <TypeDecl> {
        AstTypeDeclAnnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("TypeDecl", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(baseType)>("baseType");
            addField<DAS_BIND_MANAGED_FIELD(structType)>("structType");
            addField<DAS_BIND_MANAGED_FIELD(enumType)>("enumType");
            addField<DAS_BIND_MANAGED_FIELD(annotation)>("annotation");
            addField<DAS_BIND_MANAGED_FIELD(firstType)>("firstType");
            addField<DAS_BIND_MANAGED_FIELD(secondType)>("secondType");
            addField<DAS_BIND_MANAGED_FIELD(argTypes)>("argTypes");
            addField<DAS_BIND_MANAGED_FIELD(argNames)>("argNames");
            addField<DAS_BIND_MANAGED_FIELD(dim)>("dim");
            addField<DAS_BIND_MANAGED_FIELD(dimExpr)>("dimExpr");
            addFieldEx ( "flags", "flags", offsetof(TypeDecl, flags), makeTypeDeclFlags() );
            addField<DAS_BIND_MANAGED_FIELD(alias)>("alias");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
            // properties
            addProperty<bool (TypeDecl::*)() const, &TypeDecl::canAot>("canAot","canAot");
            addProperty<DAS_BIND_MANAGED_PROP(isExprType)>("isExprType","isExprType");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::isSimpleType>("isSimpleType","isSimpleType");
            addProperty<DAS_BIND_MANAGED_PROP(isArray)>("isArray","isArray");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodArrayType)>("isGoodArrayType","isGoodArrayType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodTableType)>("isGoodTableType","isGoodTableType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodBlockType)>("isGoodBlockType","isGoodBlockType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodFunctionType)>("isGoodFunctionType","isGoodFunctionType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodLambdaType)>("isGoodLambdaType","isGoodLambdaType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodTupleType)>("isGoodTupleType","isGoodTupleType");
            addProperty<DAS_BIND_MANAGED_PROP(isGoodVariantType)>("isGoodVariantType","isGoodVariantType");
            addProperty<DAS_BIND_MANAGED_PROP(isVoid)>("isVoid","isVoid");
            addProperty<DAS_BIND_MANAGED_PROP(isRef)>("isRef","isRef");
            addProperty<DAS_BIND_MANAGED_PROP(isRefType)>("isRefType","isRefType");
            addProperty<DAS_BIND_MANAGED_PROP(canWrite)>("canWrite","canWrite");
            addProperty<DAS_BIND_MANAGED_PROP(isAotAlias)>("isAotAlias","isAotAlias");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::isShareable>("isShareable", "isShareable");
            addProperty<DAS_BIND_MANAGED_PROP(isIndex)>("isIndex","isIndex");
            addProperty<DAS_BIND_MANAGED_PROP(isInteger)>("isInteger","isInteger");
            addProperty<DAS_BIND_MANAGED_PROP(isNumeric)>("isNumeric","isNumeric");
            addProperty<DAS_BIND_MANAGED_PROP(isNumericComparable)>("isNumericComparable","isNumericComparable");
            addProperty<DAS_BIND_MANAGED_PROP(isPointer)>("isPointer","isPointer");
            addProperty<DAS_BIND_MANAGED_PROP(isVoidPointer)>("isVoidPointer","isVoidPointer");
            addProperty<DAS_BIND_MANAGED_PROP(isIterator)>("isIterator","isIterator");
            addProperty<DAS_BIND_MANAGED_PROP(isEnum)>("isEnum","isEnum");
            addProperty<DAS_BIND_MANAGED_PROP(isEnumT)>("isEnumT","isEnumT");
            addProperty<DAS_BIND_MANAGED_PROP(isHandle)>("isHandle","isHandle");
            addProperty<DAS_BIND_MANAGED_PROP(isStructure)>("isStructure","isStructure");
            addProperty<DAS_BIND_MANAGED_PROP(isClass)>("isClass","isClass");
            addProperty<DAS_BIND_MANAGED_PROP(isFunction)>("isFunction","isFunction");
            addProperty<DAS_BIND_MANAGED_PROP(isTuple)>("isTuple","isTuple");
            addProperty<DAS_BIND_MANAGED_PROP(isVariant)>("isVariant","isVariant");
            addProperty<DAS_BIND_MANAGED_PROP(getSizeOf)>("sizeOf","getSizeOf");
            addProperty<DAS_BIND_MANAGED_PROP(getCountOf)>("countOf","getCountOf");
            addProperty<DAS_BIND_MANAGED_PROP(getAlignOf)>("alignOf","getAlignOf");
            addProperty<DAS_BIND_MANAGED_PROP(getBaseSizeOf)>("baseSizeOf","getBaseSizeOf");
            addProperty<DAS_BIND_MANAGED_PROP(getStride)>("stride","getStride");
            addProperty<DAS_BIND_MANAGED_PROP(getTupleSize)>("tupleSize","getTupleSize");
            addProperty<DAS_BIND_MANAGED_PROP(getTupleAlign)>("tupleAlign","getTupleAlign");
            addProperty<DAS_BIND_MANAGED_PROP(getVariantSize)>("variantSize","getVariantSize");
            addProperty<DAS_BIND_MANAGED_PROP(getVariantAlign)>("variantAlign","getVariantAlign");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::canCopy>("canCopy","canCopy");
            addProperty<DAS_BIND_MANAGED_PROP(canMove)>("canMove","canMove");
            addProperty<DAS_BIND_MANAGED_PROP(canClone)>("canClone","canClone");
            addProperty<DAS_BIND_MANAGED_PROP(canNew)>("canNew","canNew");
            addProperty<DAS_BIND_MANAGED_PROP(canDeletePtr)>("canDeletePtr","canDeletePtr");
            addProperty<DAS_BIND_MANAGED_PROP(canDelete)>("canDelete","canDelete");
            addProperty<DAS_BIND_MANAGED_PROP(needDelete)>("needDelete","needDelete");
            addProperty<DAS_BIND_MANAGED_PROP(isPod)>("isPod","isPod");
            addProperty<DAS_BIND_MANAGED_PROP(isRawPod)>("isRawPod","isRawPod");
            addProperty<DAS_BIND_MANAGED_PROP(isNoHeapType)>("isNoHeapType","isNoHeapType");
            addProperty<DAS_BIND_MANAGED_PROP(isWorkhorseType)>("isWorkhorseType","isWorkhorseType");
            addProperty<DAS_BIND_MANAGED_PROP(isPolicyType)>("isPolicyType","isPolicyType");
            addProperty<DAS_BIND_MANAGED_PROP(isVecPolicyType)>("isVecPolicyType","isVecPolicyType");
            addProperty<DAS_BIND_MANAGED_PROP(isReturnType)>("isReturnType","isReturnType");
            addProperty<DAS_BIND_MANAGED_PROP(isCtorType)>("isCtorType","isCtorType");
            addProperty<DAS_BIND_MANAGED_PROP(isRange)>("isRange","isRange");
            addProperty<DAS_BIND_MANAGED_PROP(isString)>("isString","isString");
            addProperty<DAS_BIND_MANAGED_PROP(isConst)>("isConst","isConst");
            addProperty<DAS_BIND_MANAGED_PROP(isFoldable)>("isFoldable","isFoldable");
            addProperty<DAS_BIND_MANAGED_PROP(isAlias)>("isAlias","isAlias");
            addProperty<DAS_BIND_MANAGED_PROP(isAutoArrayResolved)>("isAutoArrayResolved","isAutoArrayResolved");
            addProperty<DAS_BIND_MANAGED_PROP(isAuto)>("isAuto","isAuto");
            addProperty<DAS_BIND_MANAGED_PROP(isAutoOrAlias)>("isAutoOrAlias","isAutoOrAlias");
            addProperty<DAS_BIND_MANAGED_PROP(isVectorType)>("isVectorType","isVectorType");
            addProperty<DAS_BIND_MANAGED_PROP(isBitfield)>("isBitfield","isBitfield");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::isLocal>("isLocal", "isLocal");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::hasClasses>("hasClasses", "hasClasses");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::hasNonTrivialCtor>("hasNonTrivialCtor", "hasNonTrivialCtor");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::hasNonTrivialDtor>("hasNonTrivialDtor", "hasNonTrivialDtor");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::hasNonTrivialCopy>("hasNonTrivialCopy", "hasNonTrivialCopy");
            addProperty<bool (TypeDecl::*)() const, &ManagedType::canBePlacedInContainer>("canBePlacedInContainer", "canBePlacedInContainer");
            addProperty<DAS_BIND_MANAGED_PROP(getVectorBaseType)>("vectorBaseType","getVectorBaseType");
            addProperty<DAS_BIND_MANAGED_PROP(getVectorDim)>("vectorDim","getVectorDim");
            addProperty<DAS_BIND_MANAGED_PROP(canInitWithZero)>("canInitWithZero","canInitWithZero");
            addProperty<DAS_BIND_MANAGED_PROP(getRangeBaseType)>("rangeBaseType","getRangeBaseType");
        }
    };

    TypeDeclPtr makeFieldDeclarationFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FieldDeclarationFlags";
        ft->argNames = { "moveSemantics", "parentType", "capturedConstant",
            "generated", "capturedRef", "doNotDelete", "privateField", "sealed" };
        return ft;
    }

    struct AstFieldDeclarationAnnotation : ManagedStructureAnnotation<Structure::FieldDeclaration> {
        AstFieldDeclarationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FieldDeclaration", ml, "Structure::FieldDeclaration") {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(type)>("_type","type");
            addField<DAS_BIND_MANAGED_FIELD(init)>("init");
            addField<DAS_BIND_MANAGED_FIELD(annotation)>("annotation");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(offset)>("offset");
            addFieldEx ( "flags", "flags", offsetof(Structure::FieldDeclaration, flags), makeFieldDeclarationFlags() );
        }
    };

    TypeDeclPtr makeStructureFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "StructureFlags";
        ft->argNames = { "isClass", "genCtor", "cppLayout", "cppLayoutNotPod",
            "generated", "persistent", "isLambda", "privateStructure",
            "macroInterface", "sealed" };
        return ft;
    }

    struct AstStructureAnnotation : ManagedStructureAnnotation<Structure> {
        AstStructureAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("Structure", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(fields)>("fields");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module","module");
            addField<DAS_BIND_MANAGED_FIELD(parent)>("parent");
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addFieldEx ( "flags", "flags", offsetof(Structure, flags), makeStructureFlags() );
        }
    };

    TypeDeclPtr makeFunctionFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FunctionFlags";
        ft->argNames = {
            "builtIn", "policyBased", "callBased", "interopFn", "hasReturn", "copyOnReturn", "moveOnReturn", "exports",
            "init", "addr", "used", "fastCall", "knownSideEffects", "hasToRunAtCompileTime", "unsafeOperation", "unsafeDeref",
            "hasMakeBlock", "aotNeedPrologue", "noAot", "aotHybrid", "aotTemplate", "generated", "privateFunction", "_generator",
            "_lambda", "firstArgReturnType", "noPointerCast", "isClassMethod", "isTypeConstructor", "shutdown", "anyTemplate", "macroInit"
        };
        return ft;
    }

    TypeDeclPtr makeMoreFunctionFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "MoreFunctionFlags";
        ft->argNames = {
            "macroFunction", "needStringCast"
        };
        return ft;
    }

    TypeDeclPtr makeFunctionSideEffectFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FunctionSideEffectFlags";
        ft->argNames = { "unsafe", "userScenario","modifyExternal",
            "modifyArgument","accessGlobal","invoke"
        };
        return ft;
    }

    struct AstInferHistoryAnnotation : ManagedStructureAnnotation<InferHistory> {
        AstInferHistoryAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("InferHistory", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(func)>("func");
        }
    };

    struct AstFunctionAnnotation : ManagedStructureAnnotation<Function> {
        AstFunctionAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("Function", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            addField<DAS_BIND_MANAGED_FIELD(result)>("result");
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
            addField<DAS_BIND_MANAGED_FIELD(index)>("index");
            addField<DAS_BIND_MANAGED_FIELD(totalStackSize)>("totalStackSize");
            addField<DAS_BIND_MANAGED_FIELD(totalGenLabel)>("totalGenLabel");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(atDecl)>("atDecl");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
            // addField<DAS_BIND_MANAGED_FIELD(useFunctions)>("useFunctions");
            // addField<DAS_BIND_MANAGED_FIELD(useGlobalVariables)>("useGlobalVariables");
            addField<DAS_BIND_MANAGED_FIELD(module)>("classParent", "classParent");
            // use global v
            addFieldEx ( "flags", "flags",
                offsetof(Function, flags), makeFunctionFlags() );
            addFieldEx ( "moreFlags", "moreFlags",
                offsetof(Function, flags), makeMoreFunctionFlags() );
            addFieldEx ( "sideEffectFlags", "sideEffectFlags",
                offsetof(Function, sideEffectFlags), makeFunctionSideEffectFlags() );
            addField<DAS_BIND_MANAGED_FIELD(inferStack)>("inferStack");
            addField<DAS_BIND_MANAGED_FIELD(fromGeneric)>("fromGeneric");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            addField<DAS_BIND_MANAGED_FIELD(aotHash)>("aotHash");
            // properties
            addProperty<DAS_BIND_MANAGED_PROP(getOrigin)>("origin","getOrigin");
        }
    };

    TypeDeclPtr makeVariableFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "VariableFlags";
        ft->argNames = { "init_via_move", "init_via_clone", "used", "aliasCMRES",
            "marked_used", "global_shared", "do_not_delete", "generated",
            "capture_as_ref", "can_shadow", "private_variable" };
        return ft;
    }

    TypeDeclPtr makeVariableAccessFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "VariableAccessFlags";
        ft->argNames = { "access_extern", "access_get", "access_ref",
            "access_init", "access_pass" };
        return ft;
    }

    struct AstVariableAnnotation : ManagedStructureAnnotation<Variable> {
        AstVariableAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("Variable", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(aka)>("_aka");
            addField<DAS_BIND_MANAGED_FIELD(type)>("_type","type");
            addField<DAS_BIND_MANAGED_FIELD(init)>("init");
            addField<DAS_BIND_MANAGED_FIELD(source)>("source");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(index)>("index");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
            // addField<DAS_BIND_MANAGED_FIELD(useFunctions)>("useFunctions");
            // addField<DAS_BIND_MANAGED_FIELD(useFunctions)>("useGlobalVariables");
            addField<DAS_BIND_MANAGED_FIELD(initStackSize)>("initStackSize");
            addFieldEx ( "flags", "flags", offsetof(Variable, flags), makeVariableFlags() );
            addFieldEx ( "access_flags", "access_flags", offsetof(Variable, access_flags), makeVariableAccessFlags() );
            addField<DAS_BIND_MANAGED_FIELD(annotation)>("annotation");
            // properties
            addProperty<DAS_BIND_MANAGED_PROP(isAccessUnused)>("isAccessUnused","isAccessUnused");
        }
    };

    struct SimNode_AstGetTypeDecl : SimNode_CallBase {
        DAS_PTR_NODE;
        SimNode_AstGetTypeDecl ( const LineInfo & at, const TypeDeclPtr & d, char * dE )
            : SimNode_CallBase(at) {
            typeExpr = d.get();
            descr = dE;
        }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override {
            auto that = (SimNode_AstGetTypeDecl *) SimNode::copyNode(context, code);
            that->descr = code->allocateName(descr);
            return that;
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(AstGetTypeDecl);
            V_ARG(descr);
            V_END();
        }
        __forceinline char * compute(Context &) {
            DAS_PROFILE_NODE
            return (char *) typeExpr;
        }
        TypeDecl *  typeExpr;   // requires RTTI
        char *      descr;
    };

    struct SimNode_AstGetFunction : SimNode_CallBase {
        DAS_PTR_NODE;
        SimNode_AstGetFunction ( const LineInfo & at, Function * f, char * dE )
            : SimNode_CallBase(at) {
            func = f;
            descr = dE;
        }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override {
            auto that = (SimNode_AstGetFunction *) SimNode::copyNode(context, code);
            that->descr = code->allocateName(descr);
            return that;
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(AstGetTypeDecl);
            V_ARG(descr);
            V_END();
        }
        __forceinline char * compute(Context &) {
            DAS_PROFILE_NODE
            return (char *) func;
        }
        Function *  func;   // requires RTTI
        char *      descr;
    };

    struct AstTypeDeclMacro : TypeInfoMacro {
        AstTypeDeclMacro() : TypeInfoMacro("ast_typedecl") {}
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr &, string & ) override {
            return typeFactory<smart_ptr<TypeDecl>>::make(lib);
        }
        virtual SimNode * simluate ( Context * context, const ExpressionPtr & expr, string & ) override {
            auto exprTypeInfo = static_pointer_cast<ExprTypeInfo>(expr);
            char * descr = context->code->allocateName(exprTypeInfo->typeexpr->getMangledName());
            return context->code->makeNode<SimNode_AstGetTypeDecl>(expr->at, exprTypeInfo->typeexpr, descr);
        }
        virtual bool noAot ( const ExpressionPtr & ) const override {
            return true;
        }
    };

    struct AstFunctionMacro : TypeInfoMacro {
        AstFunctionMacro() : TypeInfoMacro("ast_function") {}
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr &, string & ) override {
            return typeFactory<smart_ptr<Function>>::make(lib);
        }
        virtual SimNode * simluate ( Context * context, const ExpressionPtr & expr, string & errors ) override {
            auto exprTypeInfo = static_pointer_cast<ExprTypeInfo>(expr);
            if ( exprTypeInfo->subexpr && exprTypeInfo->subexpr->rtti_isAddr() ) {
                auto exprAddr = static_pointer_cast<ExprAddr>(exprTypeInfo->subexpr);
                char * descr = context->code->allocateName(exprAddr->func->getMangledName());
                return context->code->makeNode<SimNode_AstGetFunction>(expr->at, exprAddr->func, descr);
            } else {
                errors = "ast_expression requires @@func expression";
                return nullptr;
            }
        }
        virtual bool noAot ( const ExpressionPtr & ) const override {
            return true;
        }
    };

    int adapt_field_offset ( const char * fName, const StructInfo * info ) {
        for ( uint32_t i=0; i!=info->count; ++i ) {
            if ( strcmp(info->fields[i]->name,fName)==0 ) {
                return info->fields[i]->offset;
            }
        }
        DAS_VERIFYF(0,"mapping %s not found. not fully implemented dervied class %s", fName, info->name);
        return 0;
    }

    char * adapt_field ( const char * fName, char * pClass, const StructInfo * info ) {
        return pClass + adapt_field_offset(fName,info);
    }

    Func adapt ( const char * funcName, char * pClass, const StructInfo * info ) {
        char * field = adapt_field(funcName, pClass, info);
        return field ? *(Func*)field : Func(0);
    }

#define FN_PREVISIT(WHAT)  fnPreVisit##WHAT
#define FN_VISIT(WHAT)      fnVisit##WHAT

#define IMPL_PREVISIT1(WHAT,WHATTYPE) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>> \
            (context,nullptr,FN_PREVISIT(WHAT),classPtr,expr); \
    }

#define IMPL_PREVISIT2(WHAT,WHATTYPE,ARG1T,ARG1) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T> \
            (context,nullptr,FN_PREVISIT(WHAT),classPtr,expr,ARG1); \
    }

#define IMPL_PREVISIT3(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T> \
            (context,nullptr,FN_PREVISIT(WHAT),classPtr,expr,ARG1,ARG2); \
    }

#define IMPL_PREVISIT4(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,nullptr,FN_PREVISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3); \
    }

#define IMPL_PREVISIT(WHAT) IMPL_PREVISIT1(WHAT,WHAT)

#define IMPL_VISIT_VOID1(WHAT,WHATTYPE) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr); \
    }

#define IMPL_VISIT_VOID2(WHAT,WHATTYPE,ARG1T,ARG1) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1); \
    }

#define IMPL_VISIT_VOID3(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2); \
    }

#define IMPL_VISIT_VOID4(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3); \
    }

#define IMPL_VISIT1(WHAT,WHATTYPE,RETTYPE,RETVALUE) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr_raw<WHATTYPE>> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr).marshal(RETVALUE); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT2(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1).marshal(RETVALUE); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT3(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2).marshal(RETVALUE); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT4(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr_raw<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,nullptr,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3).marshal(RETVALUE); \
    } else { \
        return RETVALUE; \
    }


#define IMPL_VISIT(WHAT) IMPL_VISIT1(WHAT,WHAT,WHAT,expr)

#define IMPL_VISIT_VOID(WHAT) IMPL_VISIT_VOID1(WHAT,WHAT)

#define IMPL_ADAPT(WHAT) \
    FN_PREVISIT(WHAT) = adapt("preVisit" #WHAT,pClass,info); \
    FN_VISIT(WHAT) = adapt("visit" #WHAT,pClass,info);

#define IMPL_BIND_EXPR(WHAT) \
    void VisitorAdapter::preVisit ( WHAT * expr ) \
        { IMPL_PREVISIT(WHAT); } \
    ExpressionPtr VisitorAdapter::visit ( WHAT * expr ) \
        { IMPL_VISIT(WHAT); }

    VisitorAdapter::VisitorAdapter ( char * pClass, const StructInfo * info, Context * ctx ) {
        context = ctx;
        classPtr = pClass;
        // adapt
        IMPL_ADAPT(Program);
        FN_PREVISIT(ProgramBody) = adapt("preVisitProgramBody",pClass,info);
        IMPL_ADAPT(TypeDecl);
        IMPL_ADAPT(Expression);
        IMPL_ADAPT(Alias);
        fnCanVisitEnumeration = adapt("canVisitEnumeration",pClass,info);
        IMPL_ADAPT(Enumeration);
        IMPL_ADAPT(EnumerationValue);
        fnCanVisitStructure = adapt("canVisitStructure",pClass,info);
        IMPL_ADAPT(Structure);
        IMPL_ADAPT(StructureField);
        fnCanVisitFunction = adapt("canVisitFunction",pClass,info);
        fnCanVisitArgumentInit = adapt("canVisitFunctionArgumentInit",pClass,info);
        IMPL_ADAPT(Function);
        IMPL_ADAPT(FunctionArgument);
        IMPL_ADAPT(FunctionArgumentInit);
        IMPL_ADAPT(FunctionBody);
        IMPL_ADAPT(ExprBlock);
        IMPL_ADAPT(ExprBlockArgument);
        IMPL_ADAPT(ExprBlockArgumentInit);
        IMPL_ADAPT(ExprBlockExpression);
        IMPL_ADAPT(ExprBlockFinal);
        IMPL_ADAPT(ExprBlockFinalExpression);
        IMPL_ADAPT(ExprLet);
        IMPL_ADAPT(ExprLetVariable);
        IMPL_ADAPT(ExprLetVariableInit);
        fnCanVisitGlobalVariable = adapt("canVisitGlobalVariable",pClass,info);
        IMPL_ADAPT(GlobalLet);
        IMPL_ADAPT(GlobalLetVariable);
        IMPL_ADAPT(GlobalLetVariableInit);
        IMPL_ADAPT(ExprStringBuilder);
        IMPL_ADAPT(ExprStringBuilderElement);
        IMPL_ADAPT(ExprNew);
        IMPL_ADAPT(ExprNewArgument);
        IMPL_ADAPT(ExprNamedCall);
        IMPL_ADAPT(ExprNamedCallArgument);
        IMPL_ADAPT(ExprCall);
        IMPL_ADAPT(ExprCallArgument);
        IMPL_ADAPT(ExprLooksLikeCall);
        IMPL_ADAPT(ExprLooksLikeCallArgument);
        IMPL_ADAPT(ExprNullCoalescing);
        FN_PREVISIT(ExprNullCoalescingDefault) = adapt("preVisitExprNullCoalescingDefault",pClass,info);
        IMPL_ADAPT(ExprAt);
        FN_PREVISIT(ExprAtIndex) = adapt("preVisitExprAtIndex",pClass,info);
        IMPL_ADAPT(ExprSafeAt);
        FN_PREVISIT(ExprSafeAtIndex) = adapt("preVisitExprSafeAtIndex",pClass,info);
        IMPL_ADAPT(ExprIs);
        FN_PREVISIT(ExprIsType) = adapt("preVisitExprIsType",pClass,info);
        IMPL_ADAPT(ExprOp2);
        FN_PREVISIT(ExprOp2Right) = adapt("preVisitExprOp2Right",pClass,info);
        IMPL_ADAPT(ExprOp3);
        FN_PREVISIT(ExprOp3Left) = adapt("preVisitExprOp3Left",pClass,info);
        FN_PREVISIT(ExprOp3Right) = adapt("preVisitExprOp3Right",pClass,info);
        IMPL_ADAPT(ExprCopy);
        FN_PREVISIT(ExprCopyRight) = adapt("preVisitExprCopyRight",pClass,info);
        IMPL_ADAPT(ExprMove);
        FN_PREVISIT(ExprMoveRight) = adapt("preVisitExprMoveRight",pClass,info);
        IMPL_ADAPT(ExprClone);
        FN_PREVISIT(ExprCloneRight) = adapt("preVisitExprCloneRight",pClass,info);
        IMPL_ADAPT(ExprAssume);
        IMPL_ADAPT(ExprWith);
        FN_PREVISIT(ExprWithBody) = adapt("preVisitExprWithBody",pClass,info);
        IMPL_ADAPT(ExprWhile);
        FN_PREVISIT(ExprWhileBody) = adapt("preVisitExprWhileBody",pClass,info);
        IMPL_ADAPT(ExprTryCatch);
        FN_PREVISIT(ExprTryCatchCatch) = adapt("preVisitExprTryCatchCatch",pClass,info);
        IMPL_ADAPT(ExprIfThenElse);
        FN_PREVISIT(ExprIfThenElseIfBlock) = adapt("preVisitExprIfThenElseIfBlock",pClass,info);
        FN_PREVISIT(ExprIfThenElseElseBlock) = adapt("preVisitExprIfThenElseElseBlock",pClass,info);
        IMPL_ADAPT(ExprFor);
        IMPL_ADAPT(ExprForVariable);
        IMPL_ADAPT(ExprForSource);
        FN_PREVISIT(ExprForStack) = adapt("preVisitExprForStack",pClass,info);
        FN_PREVISIT(ExprForBody) = adapt("preVisitExprForBody",pClass,info);
        IMPL_ADAPT(ExprMakeVariant);
        IMPL_ADAPT(ExprMakeVariantField);
        fnCanVisitMakeStructBody = adapt("canVisitMakeStructBody",pClass,info);
        fnCanVisitMakeStructBlock = adapt("canVisitMakeStructBlock",pClass,info);
        IMPL_ADAPT(ExprMakeStruct);
        IMPL_ADAPT(ExprMakeStructIndex);
        IMPL_ADAPT(ExprMakeStructField);
        IMPL_ADAPT(ExprMakeArray);
        IMPL_ADAPT(ExprMakeArrayIndex);
        IMPL_ADAPT(ExprMakeTuple);
        IMPL_ADAPT(ExprMakeTupleIndex);
        IMPL_ADAPT(ExprArrayComprehension);
        FN_PREVISIT(ExprArrayComprehensionSubexpr) = adapt("preVisitExprArrayComprehensionSubexpr",pClass,info);
        FN_PREVISIT(ExprArrayComprehensionWhere) = adapt("preVisitExprArrayComprehensionWhere",pClass,info);
        IMPL_ADAPT(ExprTypeInfo);
        IMPL_ADAPT(ExprLabel);
        IMPL_ADAPT(ExprGoto);
        IMPL_ADAPT(ExprRef2Value);
        IMPL_ADAPT(ExprRef2Ptr);
        IMPL_ADAPT(ExprPtr2Ref);
        IMPL_ADAPT(ExprAddr);
        IMPL_ADAPT(ExprAssert);
        IMPL_ADAPT(ExprStaticAssert);
        IMPL_ADAPT(ExprQuote);
        IMPL_ADAPT(ExprDebug);
        IMPL_ADAPT(ExprInvoke);
        IMPL_ADAPT(ExprErase);
        IMPL_ADAPT(ExprFind);
        IMPL_ADAPT(ExprKeyExists);
        IMPL_ADAPT(ExprAscend);
        IMPL_ADAPT(ExprCast);
        IMPL_ADAPT(ExprDelete);
        IMPL_ADAPT(ExprVar);
        IMPL_ADAPT(ExprSwizzle);
        IMPL_ADAPT(ExprField);
        IMPL_ADAPT(ExprSafeField);
        IMPL_ADAPT(ExprIsVariant);
        IMPL_ADAPT(ExprAsVariant);
        IMPL_ADAPT(ExprSafeAsVariant);
        IMPL_ADAPT(ExprOp1);
        IMPL_ADAPT(ExprReturn);
        IMPL_ADAPT(ExprYield);
        IMPL_ADAPT(ExprBreak);
        IMPL_ADAPT(ExprContinue);
        IMPL_ADAPT(ExprConst);
        IMPL_ADAPT(ExprFakeContext);
        IMPL_ADAPT(ExprFakeLineInfo);
        IMPL_ADAPT(ExprConstPtr);
        IMPL_ADAPT(ExprConstEnumeration);
        IMPL_ADAPT(ExprConstBitfield);
        IMPL_ADAPT(ExprConstInt8);
        IMPL_ADAPT(ExprConstInt16);
        IMPL_ADAPT(ExprConstInt64);
        IMPL_ADAPT(ExprConstInt);
        IMPL_ADAPT(ExprConstInt2);
        IMPL_ADAPT(ExprConstInt3);
        IMPL_ADAPT(ExprConstInt4);
        IMPL_ADAPT(ExprConstUInt8);
        IMPL_ADAPT(ExprConstUInt16);
        IMPL_ADAPT(ExprConstUInt64);
        IMPL_ADAPT(ExprConstUInt);
        IMPL_ADAPT(ExprConstUInt2);
        IMPL_ADAPT(ExprConstUInt3);
        IMPL_ADAPT(ExprConstUInt4);
        IMPL_ADAPT(ExprConstRange);
        IMPL_ADAPT(ExprConstURange);
        IMPL_ADAPT(ExprConstBool);
        IMPL_ADAPT(ExprConstFloat);
        IMPL_ADAPT(ExprConstFloat2);
        IMPL_ADAPT(ExprConstFloat3);
        IMPL_ADAPT(ExprConstFloat4);
        IMPL_ADAPT(ExprConstString);
        IMPL_ADAPT(ExprConstDouble);
        IMPL_ADAPT(ExprMakeBlock);
        IMPL_ADAPT(ExprMakeGenerator);
        IMPL_ADAPT(ExprMemZero);
        IMPL_ADAPT(ExprReader);
        IMPL_ADAPT(ExprUnsafe);
        IMPL_ADAPT(ExprCallMacro);
    }
// whole program
    void VisitorAdapter::preVisitProgram ( Program * expr )
        { IMPL_PREVISIT(Program); }
    void VisitorAdapter::visitProgram ( Program * expr )
        { IMPL_VISIT_VOID(Program); }
    void VisitorAdapter::preVisitProgramBody ( Program * expr, Module * mod )
        { IMPL_PREVISIT2(ProgramBody,Program,Module *,mod); }
// type
    void VisitorAdapter::preVisit ( TypeDecl * expr )
        { IMPL_PREVISIT(TypeDecl); }
    TypeDeclPtr VisitorAdapter::visit ( TypeDecl * expr )
        { IMPL_VISIT(TypeDecl); }
// alias
    void VisitorAdapter::preVisitAlias ( TypeDecl * expr, const string & name )
        { IMPL_PREVISIT2(Alias,TypeDecl,const string &,name); }
    TypeDeclPtr VisitorAdapter::visitAlias ( TypeDecl * expr, const string & name )
        { IMPL_VISIT2(Alias,TypeDecl,TypeDecl,expr,const string &,name); }
// enumeration
    bool VisitorAdapter::canVisitEnumeration ( Enumeration * enu ) {
        if ( fnCanVisitEnumeration ) {
            return das_invoke_function<bool>::invoke<void *,Enumeration *>
                (context,nullptr,fnCanVisitEnumeration,classPtr,enu);
        } else {
            return true;
        }
    }

    void VisitorAdapter::preVisit ( Enumeration * expr )
        { IMPL_PREVISIT(Enumeration); }
    EnumerationPtr VisitorAdapter::visit ( Enumeration * expr )
        { IMPL_VISIT(Enumeration); }
    void VisitorAdapter::preVisitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last )
        { IMPL_PREVISIT4(EnumerationValue,Enumeration,const string &,name,ExpressionPtr,value,bool,last); }
    ExpressionPtr VisitorAdapter::visitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last )
        { IMPL_VISIT4(EnumerationValue,Enumeration,Expression,value,const string &,name,ExpressionPtr,value,bool,last); }
// structure
    bool VisitorAdapter::canVisitStructure ( Structure * var ) {
        if ( fnCanVisitStructure ) {
            return das_invoke_function<bool>::invoke<void *,Structure *>
                (context,nullptr,fnCanVisitStructure,classPtr,var);
        } else {
            return true;
        }
    }
    void VisitorAdapter::preVisit ( Structure * expr )
        { IMPL_PREVISIT(Structure); }
    void VisitorAdapter::preVisitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last )
        { IMPL_PREVISIT3(StructureField,Structure,Structure::FieldDeclaration &,decl,bool,last); }
    void VisitorAdapter::visitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last )  {
        if ( FN_VISIT(StructureField) ) {
            das_invoke_function<void>::invoke<void *,StructurePtr,Structure::FieldDeclaration&,bool>
                (context,nullptr,FN_VISIT(StructureField),classPtr,expr,decl,last);
        }
    }
    StructurePtr VisitorAdapter::visit ( Structure * expr )
        { IMPL_VISIT(Structure); }
// function
    bool VisitorAdapter::canVisitFunction ( Function * fun ) {
        if ( fnCanVisitFunction ) {
            return das_invoke_function<bool>::invoke<void *,Function *>
                (context,nullptr,fnCanVisitFunction,classPtr,fun);
        } else {
            return true;
        }
    }
    bool VisitorAdapter::canVisitArgumentInit ( Function * fun, const VariablePtr & var, Expression * init ) {
        if ( fnCanVisitArgumentInit ) {
            return das_invoke_function<bool>::invoke<void *,Function *,VariablePtr,ExpressionPtr>
                (context,nullptr,fnCanVisitArgumentInit,classPtr,fun,var,init);
        } else {
            return true;
        }
    }
    void VisitorAdapter::preVisit ( Function * expr )
        { IMPL_PREVISIT(Function); }
    FunctionPtr VisitorAdapter::visit ( Function * expr )
        { IMPL_VISIT(Function); }
    void VisitorAdapter::preVisitArgument ( Function * expr, const VariablePtr & var, bool lastArg )
        { IMPL_PREVISIT3(FunctionArgument,Function,VariablePtr,var,bool,lastArg); }
    VariablePtr VisitorAdapter::visitArgument ( Function * expr, const VariablePtr & var, bool lastArg )
        { IMPL_VISIT3(FunctionArgument,Function,Variable,var,VariablePtr,var,bool,lastArg); }
    void VisitorAdapter::preVisitArgumentInit ( Function * expr, const VariablePtr & var, Expression * init )
        { IMPL_PREVISIT3(FunctionArgument,Function,VariablePtr,var,ExpressionPtr,init); }
    ExpressionPtr VisitorAdapter::visitArgumentInit ( Function * expr, const VariablePtr & var, Expression * init )
        { IMPL_VISIT3(FunctionArgumentInit,Function,Expression,init,VariablePtr,var,ExpressionPtr,init); }
    void VisitorAdapter::preVisitFunctionBody ( Function * expr, Expression * that )
        { IMPL_PREVISIT2(FunctionBody,Function,ExpressionPtr,that); }
    ExpressionPtr VisitorAdapter::visitFunctionBody ( Function * expr, Expression * that )
        { IMPL_VISIT2(FunctionBody,Function,Expression,that,ExpressionPtr,that); }
// expression
    void VisitorAdapter::preVisitExpression ( Expression * expr )
        { IMPL_PREVISIT(Expression); }
    ExpressionPtr VisitorAdapter::visitExpression ( Expression * expr )
        { IMPL_VISIT(Expression); }
// block
    IMPL_BIND_EXPR(ExprBlock);
    void VisitorAdapter::preVisitBlockArgument ( ExprBlock * expr, const VariablePtr & var, bool lastArg )
        { IMPL_PREVISIT3(ExprBlockArgument,ExprBlock,VariablePtr,var,bool,lastArg); }
    VariablePtr VisitorAdapter::visitBlockArgument ( ExprBlock * expr, const VariablePtr & var, bool lastArg )
        { IMPL_VISIT3(ExprBlockArgument,ExprBlock,Variable,var,VariablePtr,var,bool,lastArg); }
    void VisitorAdapter::preVisitBlockArgumentInit ( ExprBlock * expr, const VariablePtr & var, Expression * init )
        { IMPL_PREVISIT3(ExprBlockArgumentInit,ExprBlock,VariablePtr,var,ExpressionPtr,init); }
    ExpressionPtr VisitorAdapter::visitBlockArgumentInit ( ExprBlock * expr, const VariablePtr & var, Expression * init )
        { IMPL_VISIT3(ExprBlockArgumentInit,ExprBlock,Expression,init,VariablePtr,var,ExpressionPtr,init); }
    void VisitorAdapter::preVisitBlockExpression ( ExprBlock * expr, Expression * bexpr )
        { IMPL_PREVISIT2(ExprBlockExpression,ExprBlock,ExpressionPtr,bexpr); }
    ExpressionPtr VisitorAdapter::visitBlockExpression (  ExprBlock * expr, Expression * bexpr )
        { IMPL_VISIT2(ExprBlockExpression,ExprBlock,Expression,bexpr,ExpressionPtr,bexpr); }
    void VisitorAdapter::preVisitBlockFinal ( ExprBlock * expr )
        { IMPL_PREVISIT1(ExprBlockFinal,ExprBlock); }
    void VisitorAdapter::visitBlockFinal ( ExprBlock * expr )  {
        if ( FN_VISIT(ExprBlockFinal) ) {
            das_invoke_function<void>::invoke<void *,smart_ptr_raw<ExprBlock>>
                (context,nullptr,FN_VISIT(ExprBlockFinal),classPtr,expr);
        }
    }
    void VisitorAdapter::preVisitBlockFinalExpression ( ExprBlock * expr, Expression * bexpr )
        { IMPL_PREVISIT2(ExprBlockFinalExpression,ExprBlock,ExpressionPtr,bexpr); }
    ExpressionPtr VisitorAdapter::visitBlockFinalExpression (  ExprBlock * expr, Expression * bexpr )
        { IMPL_VISIT2(ExprBlockFinalExpression,ExprBlock,Expression,bexpr,ExpressionPtr,bexpr); }
// let
    IMPL_BIND_EXPR(ExprLet);
    void VisitorAdapter::preVisitLet ( ExprLet * expr, const VariablePtr & var, bool last )
        { IMPL_PREVISIT3(ExprLetVariable,ExprLet,VariablePtr,var,bool,last); }
    VariablePtr VisitorAdapter::visitLet ( ExprLet * expr, const VariablePtr & var, bool last )
        { IMPL_VISIT3(ExprLetVariable,ExprLet,Variable,var,VariablePtr,var,bool,last); }
    void VisitorAdapter::preVisitLetInit ( ExprLet * expr, const VariablePtr & var, Expression * init )
        { IMPL_PREVISIT3(ExprLetVariableInit,ExprLet,VariablePtr,var,ExpressionPtr,init); }
    ExpressionPtr VisitorAdapter::visitLetInit ( ExprLet * expr, const VariablePtr & var, Expression * init )
        { IMPL_VISIT3(ExprLetVariableInit,ExprLet,Expression,init,VariablePtr,var,ExpressionPtr,init); }
// global let
    bool VisitorAdapter::canVisitGlobalVariable ( Variable * var ) {
        if ( fnCanVisitGlobalVariable ) {
            return das_invoke_function<bool>::invoke<void *,Variable *>
                (context,nullptr,fnCanVisitGlobalVariable,classPtr,var);
        } else {
            return true;
        }
    }
    void VisitorAdapter::preVisitGlobalLetBody ( Program * expr )
        { IMPL_PREVISIT1(GlobalLet,Program); }
    void VisitorAdapter::visitGlobalLetBody ( Program * expr )
        { IMPL_VISIT_VOID1(GlobalLet,Program); }
    void VisitorAdapter::preVisitGlobalLet ( const VariablePtr & expr )
        { IMPL_PREVISIT1(GlobalLetVariable,Variable); }
    VariablePtr VisitorAdapter::visitGlobalLet ( const VariablePtr & expr )
        { IMPL_VISIT1(GlobalLetVariable,Variable,Variable,expr); }
    void VisitorAdapter::preVisitGlobalLetInit ( const VariablePtr & expr, Expression * init )
        { IMPL_PREVISIT2(GlobalLetVariableInit,Variable,ExpressionPtr,init); }
    ExpressionPtr VisitorAdapter::visitGlobalLetInit ( const VariablePtr & expr, Expression * init )
        { IMPL_VISIT2(GlobalLetVariableInit,Variable,Expression,init,ExpressionPtr,init); }
// string builder
    IMPL_BIND_EXPR(ExprStringBuilder);
    void VisitorAdapter::preVisitStringBuilderElement ( ExprStringBuilder * expr, Expression * element, bool last )
        { IMPL_PREVISIT3(ExprStringBuilderElement,ExprStringBuilder,ExpressionPtr,element,bool,last); }
    ExpressionPtr VisitorAdapter::visitStringBuilderElement ( ExprStringBuilder * expr, Expression * element, bool last )
        { IMPL_VISIT3(ExprStringBuilderElement,ExprStringBuilder,Expression,element,ExpressionPtr,element,bool,last); }
// new
    IMPL_BIND_EXPR(ExprNew);
    void VisitorAdapter::preVisitNewArg ( ExprNew * expr, Expression * arg, bool last )
        { IMPL_PREVISIT3(ExprNewArgument,ExprNew,ExpressionPtr,arg,bool,last); }
    ExpressionPtr VisitorAdapter::visitNewArg ( ExprNew * expr, Expression * arg , bool last )
        { IMPL_VISIT3(ExprNewArgument,ExprNew,Expression,arg,ExpressionPtr,arg,bool,last); }
// named call
    IMPL_BIND_EXPR(ExprNamedCall);
    void VisitorAdapter::preVisitNamedCallArg ( ExprNamedCall * expr, MakeFieldDecl * arg, bool last )
        { IMPL_PREVISIT3(ExprNamedCallArgument,ExprNamedCall,MakeFieldDeclPtr,arg,bool,last); }
    MakeFieldDeclPtr VisitorAdapter::visitNamedCallArg ( ExprNamedCall * expr, MakeFieldDecl * arg , bool last )
        { IMPL_VISIT3(ExprNamedCallArgument,ExprNamedCall,MakeFieldDecl,arg,MakeFieldDeclPtr,arg,bool,last); }
// call
    IMPL_BIND_EXPR(ExprCall);
    void VisitorAdapter::preVisitCallArg ( ExprCall * expr, Expression * arg, bool last )
        { IMPL_PREVISIT3(ExprCallArgument,ExprCall,ExpressionPtr,arg,bool,last); }
    ExpressionPtr VisitorAdapter::visitCallArg ( ExprCall * expr, Expression * arg , bool last )
        { IMPL_VISIT3(ExprCallArgument,ExprCall,Expression,arg,ExpressionPtr,arg,bool,last); }
// looks like call
    IMPL_BIND_EXPR(ExprLooksLikeCall);
    void VisitorAdapter::preVisitLooksLikeCallArg ( ExprLooksLikeCall * expr, Expression * arg, bool last )
        { IMPL_PREVISIT3(ExprLooksLikeCallArgument,ExprLooksLikeCall,ExpressionPtr,arg,bool,last); }
    ExpressionPtr VisitorAdapter::visitLooksLikeCallArg ( ExprLooksLikeCall * expr, Expression * arg , bool last )
        { IMPL_VISIT3(ExprLooksLikeCallArgument,ExprLooksLikeCall,Expression,arg,ExpressionPtr,arg,bool,last); }
// null coaelescing
    IMPL_BIND_EXPR(ExprNullCoalescing);
    void VisitorAdapter::preVisitNullCoaelescingDefault ( ExprNullCoalescing * expr, Expression * defval )
        { IMPL_PREVISIT2(ExprNullCoalescingDefault,ExprNullCoalescing,ExpressionPtr, defval); }
// at
    IMPL_BIND_EXPR(ExprAt);
    void VisitorAdapter::preVisitAtIndex ( ExprAt * expr, Expression * index )
        { IMPL_PREVISIT2(ExprAtIndex,ExprAt,ExpressionPtr,index); }
// safe at
    IMPL_BIND_EXPR(ExprSafeAt);
    void VisitorAdapter::preVisitSafeAtIndex ( ExprSafeAt * expr, Expression * index )
        { IMPL_PREVISIT2(ExprSafeAtIndex,ExprSafeAt,ExpressionPtr,index); }
// is
    IMPL_BIND_EXPR(ExprIs);
    void VisitorAdapter::preVisitType ( ExprIs * expr, TypeDecl * typeDecl )
        { IMPL_PREVISIT2(ExprIsType,ExprIs,TypeDeclPtr,typeDecl); }
// op2
    IMPL_BIND_EXPR(ExprOp2);
    void VisitorAdapter::preVisitRight ( ExprOp2 * expr, Expression * right )
        { IMPL_PREVISIT2(ExprOp2Right,ExprOp2,ExpressionPtr,right); }
// op3
    IMPL_BIND_EXPR(ExprOp3);
    void VisitorAdapter::preVisitLeft  ( ExprOp3 * expr, Expression * left )
        { IMPL_PREVISIT2(ExprOp3Left,ExprOp3,ExpressionPtr,left); }
    void VisitorAdapter::preVisitRight ( ExprOp3 * expr, Expression * right )
        { IMPL_PREVISIT2(ExprOp3Right,ExprOp3,ExpressionPtr,right); }
// copy
    IMPL_BIND_EXPR(ExprCopy);
    void VisitorAdapter::preVisitRight ( ExprCopy * expr, Expression * right )
        { IMPL_PREVISIT2(ExprCopyRight,ExprCopy,ExpressionPtr,right); }
// move
    IMPL_BIND_EXPR(ExprMove);
    void VisitorAdapter::preVisitRight ( ExprMove * expr, Expression * right )
        { IMPL_PREVISIT2(ExprMoveRight,ExprMove,ExpressionPtr,right); }
// clone
    IMPL_BIND_EXPR(ExprClone);
    void VisitorAdapter::preVisitRight ( ExprClone * expr, Expression * right )
        { IMPL_PREVISIT2(ExprCloneRight,ExprClone,ExpressionPtr,right); }
// assume
    IMPL_BIND_EXPR(ExprAssume);
// with
    IMPL_BIND_EXPR(ExprWith);
    void VisitorAdapter::preVisitWithBody ( ExprWith * expr, Expression * body )
        { IMPL_PREVISIT2(ExprWithBody,ExprWith,ExpressionPtr,body); }
// while
    IMPL_BIND_EXPR(ExprWhile);
    void VisitorAdapter::preVisitWhileBody ( ExprWhile * expr, Expression * body )
        { IMPL_PREVISIT2(ExprWhileBody,ExprWhile,ExpressionPtr,body); }
// try-catch
    IMPL_BIND_EXPR(ExprTryCatch);
    void VisitorAdapter::preVisitCatch ( ExprTryCatch * expr, Expression * body )
        { IMPL_PREVISIT2(ExprTryCatchCatch,ExprTryCatch,ExpressionPtr,body); }
// if-then-else
    IMPL_BIND_EXPR(ExprIfThenElse);
    void VisitorAdapter::preVisitIfBlock ( ExprIfThenElse * expr, Expression * ifBlock )
        { IMPL_PREVISIT2(ExprIfThenElseIfBlock,ExprIfThenElse,ExpressionPtr,ifBlock); }
    void VisitorAdapter::preVisitElseBlock ( ExprIfThenElse * expr, Expression * elseBlock )
        { IMPL_PREVISIT2(ExprIfThenElseElseBlock,ExprIfThenElse,ExpressionPtr,elseBlock); }
// for
    IMPL_BIND_EXPR(ExprFor);
    void VisitorAdapter::preVisitFor ( ExprFor * expr, const VariablePtr & var, bool last )
        { IMPL_PREVISIT3(ExprForVariable,ExprFor,VariablePtr,var,bool,last); }
    VariablePtr VisitorAdapter::visitFor ( ExprFor * expr, const VariablePtr & var, bool last )
        { IMPL_VISIT3(ExprForVariable,ExprFor,Variable,var,VariablePtr,var,bool,last); }
    void VisitorAdapter::preVisitForStack ( ExprFor * expr )
        { IMPL_PREVISIT1(ExprForStack,ExprFor); }
    void VisitorAdapter::preVisitForBody ( ExprFor * expr, Expression * body )
        { IMPL_PREVISIT2(ExprForBody,ExprFor,ExpressionPtr,body); }
    void VisitorAdapter::preVisitForSource ( ExprFor * expr, Expression * source, bool last )
        { IMPL_PREVISIT3(ExprForSource,ExprFor,ExpressionPtr,source,bool,last); }
    ExpressionPtr VisitorAdapter::VisitorAdapter::visitForSource ( ExprFor * expr, Expression * source , bool last )
        { IMPL_VISIT3(ExprForSource,ExprFor,Expression,source,ExpressionPtr,source,bool,last); }
// make variant
    IMPL_BIND_EXPR(ExprMakeVariant);
    void VisitorAdapter::preVisitMakeVariantField ( ExprMakeVariant * expr, int index, MakeFieldDecl * decl, bool last )
        { IMPL_PREVISIT4(ExprMakeVariantField,ExprMakeVariant,int,index,MakeFieldDeclPtr,decl,bool,last); }
    MakeFieldDeclPtr VisitorAdapter::visitMakeVariantField(ExprMakeVariant * expr, int index, MakeFieldDecl * decl, bool last)
        { IMPL_VISIT4(ExprMakeVariantField,ExprMakeVariant,MakeFieldDecl,decl,int,index,MakeFieldDeclPtr,decl,bool,last); }
// make structure
    bool VisitorAdapter::canVisitMakeStructureBlock ( ExprMakeStruct * expr, Expression * blk ) {
        if ( fnCanVisitMakeStructBlock ) {
            return das_invoke_function<bool>::invoke<void *,smart_ptr<ExprMakeStruct>,ExpressionPtr>
                (context,nullptr,fnCanVisitMakeStructBlock,classPtr,expr,blk);
        } else {
            return true;
        }
    }
    bool VisitorAdapter::canVisitMakeStructureBody ( ExprMakeStruct * expr ) {
        if ( fnCanVisitMakeStructBody ) {
            return das_invoke_function<bool>::invoke<void *,smart_ptr<ExprMakeStruct>>
                (context,nullptr,fnCanVisitMakeStructBody,classPtr,expr);
        } else {
            return true;
        }
    }
    IMPL_BIND_EXPR(ExprMakeStruct);
    void VisitorAdapter::preVisitMakeStructureIndex ( ExprMakeStruct * expr, int index, bool last )
        { IMPL_PREVISIT3(ExprMakeStructIndex,ExprMakeStruct,int,index,bool,last); }
    void VisitorAdapter::visitMakeStructureIndex ( ExprMakeStruct * expr, int index, bool last )
        { IMPL_VISIT_VOID3(ExprMakeStructIndex,ExprMakeStruct,int,index,bool,last); }
    void VisitorAdapter::preVisitMakeStructureField ( ExprMakeStruct * expr, int index, MakeFieldDecl * decl, bool last )
            { IMPL_PREVISIT4(ExprMakeStructField,ExprMakeStruct,int,index,MakeFieldDeclPtr,decl,bool,last); }
    MakeFieldDeclPtr VisitorAdapter::visitMakeStructureField ( ExprMakeStruct * expr, int index, MakeFieldDecl * decl, bool last )
        { IMPL_VISIT4(ExprMakeStructField,ExprMakeStruct,MakeFieldDecl,decl,int,index,MakeFieldDeclPtr,decl,bool,last); }
// make array
    IMPL_BIND_EXPR(ExprMakeArray);
    void VisitorAdapter::preVisitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool last )
        { IMPL_PREVISIT4(ExprMakeArrayIndex,ExprMakeArray,int,index,ExpressionPtr,init,bool,last); }
    ExpressionPtr VisitorAdapter::visitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool last )
        { IMPL_VISIT4(ExprMakeArrayIndex,ExprMakeArray,Expression,init,int,index,ExpressionPtr,init,bool,last); }
// make tuple
    IMPL_BIND_EXPR(ExprMakeTuple);
    void VisitorAdapter::preVisitMakeTupleIndex ( ExprMakeTuple * expr, int index, Expression * init, bool last )
        { IMPL_PREVISIT4(ExprMakeTupleIndex,ExprMakeTuple,int,index,ExpressionPtr,init,bool,last); }
    ExpressionPtr VisitorAdapter::visitMakeTupleIndex ( ExprMakeTuple * expr, int index, Expression * init, bool last )
        { IMPL_VISIT4(ExprMakeTupleIndex,ExprMakeTuple,Expression,init,int,index,ExpressionPtr,init,bool,last); }
// array comprehension
    IMPL_BIND_EXPR(ExprArrayComprehension);
    void VisitorAdapter::preVisitArrayComprehensionSubexpr ( ExprArrayComprehension * expr, Expression * subexpr )
        { IMPL_PREVISIT2(ExprArrayComprehensionSubexpr,ExprArrayComprehension,ExpressionPtr,subexpr); }
    void VisitorAdapter::preVisitArrayComprehensionWhere ( ExprArrayComprehension * expr, Expression * where )
        { IMPL_PREVISIT2(ExprArrayComprehensionWhere,ExprArrayComprehension,ExpressionPtr,where); }
// type info
    IMPL_BIND_EXPR(ExprTypeInfo);
// all other expressions
    IMPL_BIND_EXPR(ExprLabel);
    IMPL_BIND_EXPR(ExprGoto);
    IMPL_BIND_EXPR(ExprRef2Value);
    IMPL_BIND_EXPR(ExprRef2Ptr);
    IMPL_BIND_EXPR(ExprPtr2Ref);
    IMPL_BIND_EXPR(ExprAddr);
    IMPL_BIND_EXPR(ExprAssert);
    IMPL_BIND_EXPR(ExprStaticAssert);
    IMPL_BIND_EXPR(ExprQuote);
    IMPL_BIND_EXPR(ExprDebug);
    IMPL_BIND_EXPR(ExprInvoke);
    IMPL_BIND_EXPR(ExprErase);
    IMPL_BIND_EXPR(ExprFind);
    IMPL_BIND_EXPR(ExprKeyExists);
    IMPL_BIND_EXPR(ExprAscend);
    IMPL_BIND_EXPR(ExprCast);
    IMPL_BIND_EXPR(ExprDelete);
    IMPL_BIND_EXPR(ExprVar);
    IMPL_BIND_EXPR(ExprSwizzle);
    IMPL_BIND_EXPR(ExprField);
    IMPL_BIND_EXPR(ExprSafeField);
    IMPL_BIND_EXPR(ExprIsVariant);
    IMPL_BIND_EXPR(ExprAsVariant);
    IMPL_BIND_EXPR(ExprSafeAsVariant);
    IMPL_BIND_EXPR(ExprOp1);
    IMPL_BIND_EXPR(ExprReturn);
    IMPL_BIND_EXPR(ExprYield);
    IMPL_BIND_EXPR(ExprBreak);
    IMPL_BIND_EXPR(ExprContinue);
    IMPL_BIND_EXPR(ExprConst);
    IMPL_BIND_EXPR(ExprFakeContext);
    IMPL_BIND_EXPR(ExprFakeLineInfo);
    IMPL_BIND_EXPR(ExprConstPtr);
    IMPL_BIND_EXPR(ExprConstEnumeration);
    IMPL_BIND_EXPR(ExprConstBitfield);
    IMPL_BIND_EXPR(ExprConstInt8);
    IMPL_BIND_EXPR(ExprConstInt16);
    IMPL_BIND_EXPR(ExprConstInt64);
    IMPL_BIND_EXPR(ExprConstInt);
    IMPL_BIND_EXPR(ExprConstInt2);
    IMPL_BIND_EXPR(ExprConstInt3);
    IMPL_BIND_EXPR(ExprConstInt4);
    IMPL_BIND_EXPR(ExprConstUInt8);
    IMPL_BIND_EXPR(ExprConstUInt16);
    IMPL_BIND_EXPR(ExprConstUInt64);
    IMPL_BIND_EXPR(ExprConstUInt);
    IMPL_BIND_EXPR(ExprConstUInt2);
    IMPL_BIND_EXPR(ExprConstUInt3);
    IMPL_BIND_EXPR(ExprConstUInt4);
    IMPL_BIND_EXPR(ExprConstRange);
    IMPL_BIND_EXPR(ExprConstURange);
    IMPL_BIND_EXPR(ExprConstBool);
    IMPL_BIND_EXPR(ExprConstFloat);
    IMPL_BIND_EXPR(ExprConstFloat2);
    IMPL_BIND_EXPR(ExprConstFloat3);
    IMPL_BIND_EXPR(ExprConstFloat4);
    IMPL_BIND_EXPR(ExprConstString);
    IMPL_BIND_EXPR(ExprConstDouble);
    IMPL_BIND_EXPR(ExprMakeBlock);
    IMPL_BIND_EXPR(ExprMakeGenerator);
    IMPL_BIND_EXPR(ExprMemZero);
    IMPL_BIND_EXPR(ExprReader);
    IMPL_BIND_EXPR(ExprCallMacro);
    IMPL_BIND_EXPR(ExprUnsafe);
    IMPL_BIND_EXPR(ExprTypeDecl);

#include "ast_gen.inc"

    struct AstVisitorAdapterAnnotation : ManagedStructureAnnotation<VisitorAdapter,false,true> {
        AstVisitorAdapterAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VisitorAdapter", ml) {
        }
    };

    class BlockAnnotationAdapter : public FunctionAnnotation, AstBlockAnnotation_Adapter {
    public:
        BlockAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
        : FunctionAnnotation(n), AstBlockAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( const FunctionPtr &, ModuleGroup &,
                            const AnnotationArgumentList &, string & err ) override {
            err = "not a function annotation";
            return false;
        }
        virtual bool finalize ( const FunctionPtr &, ModuleGroup &,
                               const AnnotationArgumentList &,
                               const AnnotationArgumentList &, string & err ) override {
            err = "not a function annotation";
            return false;
        }
        virtual bool apply ( ExprBlock * blk, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                return invoke_apply(context,fnApply,classPtr,blk,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool finalize ( ExprBlock * blk, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                return invoke_finish(context,fnFinish,classPtr,blk,group,args,progArgs,errors);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    class FunctionAnnotationAdapter : public FunctionAnnotation, AstFunctionAnnotation_Adapter {
    public:
        FunctionAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
        : FunctionAnnotation(n), AstFunctionAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( const FunctionPtr & func, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                return invoke_apply(context,fnApply,classPtr,func,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool finalize ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                return invoke_finish(context,fnFinish,classPtr,func,group,args,progArgs,errors);
            } else {
                return true;
            }
        }
        virtual bool lint ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnLint = get_lint(classPtr) ) {
                return invoke_lint(context,fnLint,classPtr,func,group,args,progArgs,errors);
            } else {
                return true;
            }
        }
        virtual bool patch ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors, bool & astChanged ) override {
            if ( auto fnPatch = get_patch(classPtr) ) {
                return invoke_patch(context,fnPatch,classPtr,func,group,args,progArgs,errors,astChanged);
            } else {
                return true;
            }
        }
        virtual bool fixup ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) override {
            if ( auto fnFixup = get_fixup(classPtr) ) {
                return invoke_fixup(context,fnFixup,classPtr,func,group,args,progArgs,errors);
            } else {
                return true;
            }
        }
        virtual bool apply ( ExprBlock *, ModuleGroup &,
                            const AnnotationArgumentList &, string & err ) override {
            err = "not a block annotation";
            return false;
        }
        virtual bool finalize ( ExprBlock *, ModuleGroup &,
                               const AnnotationArgumentList &,
                               const AnnotationArgumentList &, string & err ) override {
            err = "not a block annotation";
            return false;
        }
        virtual ExpressionPtr transformCall ( ExprCallFunc * call, string & err ) override {
            if ( auto fnTransform = get_transform(classPtr) ) {
                return invoke_transform(context,fnTransform,classPtr,call,err);
            } else {
                return nullptr;
            }
        }
        virtual bool verifyCall ( ExprCallFunc * call, const AnnotationArgumentList & args,
                const AnnotationArgumentList & progArgs, string & err ) override {
            if ( auto fnTransform = get_verifyCall(classPtr) ) {
                return invoke_verifyCall(context,fnTransform,classPtr,call,args,progArgs,err);
            } else {
                return true;
            }
        }
        virtual bool isSpecialized () const override {
            if ( auto fnIsSpecialized = get_isSpecialized(classPtr) ) {
                return invoke_isSpecialized(context,fnIsSpecialized,classPtr);
            } else {
                return false;
            }
        }
        virtual bool isCompatible ( const FunctionPtr & fn, const vector<TypeDeclPtr> & types,
            const AnnotationDeclaration & decl, string & err  ) const override {
            if ( auto fnIsCompatible = get_isCompatible(classPtr) ) {
                return invoke_isCompatible(context,fnIsCompatible,classPtr,fn,const_cast<vector<TypeDeclPtr> &>(types),decl,err);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstFunctionAnnotationAnnotation : ManagedStructureAnnotation<FunctionAnnotation,false,true> {
        AstFunctionAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FunctionAnnotation", ml) {
        }
    };

    struct StructureAnnotationAdapter : StructureAnnotation, AstStructureAnnotation_Adapter {
        StructureAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : StructureAnnotation(n), AstStructureAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool touch ( const StructurePtr & st, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                return invoke_apply(context,fnApply,classPtr,st,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool look (const StructurePtr & st, ModuleGroup & group,
            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnFinish = get_finish(classPtr) ) {
                return invoke_finish(context,fnFinish,classPtr,st,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool patch (const StructurePtr & st, ModuleGroup & group,
            const AnnotationArgumentList & args, string & errors, bool & astChanged ) override {
            if ( auto fnPatch = get_patch(classPtr) ) {
                return invoke_patch(context,fnPatch,classPtr,st,group,args,errors,astChanged);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstStructureAnnotationAnnotation : ManagedStructureAnnotation<StructureAnnotation,false,true> {
        AstStructureAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("StructureAnnotation", ml) {
        }
    };

    struct EnumerationAnnotationAdapter : EnumerationAnnotation, AstEnumerationAnnotation_Adapter {
        EnumerationAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : EnumerationAnnotation(n), AstEnumerationAnnotation_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool touch ( const EnumerationPtr & st, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                return invoke_apply(context,fnApply,classPtr,st,group,args,errors);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstEnumerationAnnotationAnnotation : ManagedStructureAnnotation<EnumerationAnnotation,false,true> {
        AstEnumerationAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("EnumerationAnnotation", ml) {
        }
    };

    struct PassMacroAdapter : PassMacro, AstPassMacro_Adapter {
        PassMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : PassMacro(n), AstPassMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool apply ( Program * prog, Module * mod ) override {
            if ( auto fnApply = get_apply(classPtr) ) {
                return invoke_apply(context,fnApply,classPtr,prog,mod);
            } else {
                return false;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstPassMacroAnnotation : ManagedStructureAnnotation<PassMacro,false,true> {
        AstPassMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("PassMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct VariantMacroAdapter : VariantMacro, AstVarianMacro_Adapter {
        VariantMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : VariantMacro(n), AstVarianMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr visitIs ( Program * prog, Module * mod, ExprIsVariant * expr ) override {
            if ( auto fnVisitIs = get_visitExprIsVariant(classPtr) ) {
                return invoke_visitExprIsVariant(context,fnVisitIs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitAs ( Program * prog, Module * mod, ExprAsVariant * expr ) override {
            if ( auto fnVisitAs = get_visitExprAsVariant(classPtr) ) {
                return invoke_visitExprAsVariant(context,fnVisitAs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitSafeAs ( Program * prog, Module * mod, ExprSafeAsVariant * expr ) override {
            if ( auto fnVisitSafeAs = get_visitExprSafeAsVariant(classPtr) ) {
                return invoke_visitExprSafeAsVariant(context,fnVisitSafeAs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstVariantMacroAnnotation : ManagedStructureAnnotation<VariantMacro,false,true> {
        AstVariantMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VariantMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct ReaderMacroAdapter : ReaderMacro, AstReaderMacro_Adapter {
        ReaderMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : ReaderMacro(n), AstReaderMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual bool accept ( Program * prog, Module * mod, ExprReader * expr, int Ch, const LineInfo & info ) override {
            if ( auto fnAccept = get_accept(classPtr) ) {
                return invoke_accept(context,fnAccept,classPtr,prog,mod,expr,Ch,info);
            } else {
                return false;
            }
        }
        virtual ExpressionPtr visit (  Program * prog, Module * mod, ExprReader * expr ) override {
            if ( auto fnVisit = get_visit(classPtr) ) {
                return invoke_visit(context,fnVisit,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstReaderMacroAnnotation : ManagedStructureAnnotation<ReaderMacro,false,true> {
        AstReaderMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ReaderMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };

    struct CallMacroAdapter : CallMacro, AstCallMacro_Adapter {
        CallMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : CallMacro(n), AstCallMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual void preVisit (  Program * prog, Module * mod, ExprCallMacro * expr ) override {
            if ( auto fnPreVisit = get_preVisit(classPtr) ) {
                invoke_preVisit(context,fnPreVisit,classPtr,prog,mod,expr);
            }
        }
        virtual ExpressionPtr visit (  Program * prog, Module * mod, ExprCallMacro * expr ) override {
            if ( auto fnVisit = get_visit(classPtr) ) {
                return invoke_visit(context,fnVisit,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    struct AstCallMacroAnnotation : ManagedStructureAnnotation<CallMacro,false,true> {
        AstCallMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("CallMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };
    struct TypeInfoMacroAdapter : TypeInfoMacro, AstTypeInfoMacro_Adapter {
        TypeInfoMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : TypeInfoMacro(n), AstTypeInfoMacro_Adapter(info), classPtr(pClass), context(ctx) {
        }
        virtual ExpressionPtr getAstChange ( const ExpressionPtr & expr, string & err ) override {
            if ( auto fnGetAstChange = get_getAstChange(classPtr) ) {
                auto tinfo = static_pointer_cast<ExprTypeInfo>(expr);
                return invoke_getAstChange(context,fnGetAstChange,classPtr,tinfo,err);
            } else {
                return nullptr;
            }
        }
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr & expr, string & err ) override {
            if ( auto fnGetAstType = get_getAstType(classPtr) ) {
                auto tinfo = static_pointer_cast<ExprTypeInfo>(expr);
                return invoke_getAstType(context,fnGetAstType,classPtr,lib,tinfo,err);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    };

    #include "ast.das.inc"

    ReaderMacroPtr makeReaderMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<ReaderMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleReaderMacro ( Module * module, ReaderMacroPtr & _newM, Context * context ) {
        ReaderMacroPtr newM = move(_newM);
        if ( !module->addReaderMacro(newM, true) ) {
            context->throw_error_ex("can't add reader macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    CallMacroPtr makeCallMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<CallMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleCallMacro ( Module * module, CallMacroPtr & _newM, Context * context ) {
        CallMacroPtr newM = move(_newM);
        if ( ! module->addCallMacro(newM->name, [=](const LineInfo & at) -> ExprLooksLikeCall * {
            auto ecm = new ExprCallMacro(at, newM->name);
            ecm->macro = newM.get();
            return ecm;
        }) ) {
            context->throw_error_ex("can't add call macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    TypeInfoMacroPtr makeTypeInfoMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<TypeInfoMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleTypeInfoMacro ( Module * module, TypeInfoMacroPtr & _newM, Context * context ) {
        TypeInfoMacroPtr newM = move(_newM);
        if ( ! module->addTypeInfoMacro(newM,true) ) {
            context->throw_error_ex("can't add type info macro %s to module %s", newM->name.c_str(), module->name.c_str());
        }
    }

    smart_ptr<VisitorAdapter> makeVisitor ( const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<VisitorAdapter>((char *)pClass,info,context);
    }

    PassMacroPtr makePassMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<PassMacroAdapter>(name,(char *)pClass,info,context);
    }

    VariantMacroPtr makeVariantMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<VariantMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleVariantMacro ( Module * module, VariantMacroPtr & _newM, Context * ) {
        VariantMacroPtr newM = move(_newM);
        module->variantMacros.push_back(newM);
    }

    void addModuleInferMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = move(_newM);
        module->macros.push_back(newM);
    }

    void addModuleInferDirtyMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = move(_newM);
        module->inferMacros.push_back(newM);
    }

    void addModuleLintMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = move(_newM);
        module->lintMacros.push_back(newM);
    }

    void addModuleGlobalLintMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = move(_newM);
        module->globalLintMacros.push_back(newM);
    }

    void addModuleOptimizationMacro ( Module * module, PassMacroPtr & _newM, Context * ) {
        PassMacroPtr newM = move(_newM);
        module->optimizationMacros.push_back(newM);
    }

    EnumerationAnnotationPtr makeEnumerationAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<EnumerationAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleEnumerationAnnotation ( Module * module, EnumerationAnnotationPtr & _ann, Context * context ) {
        EnumerationAnnotationPtr ann = move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_ex("can't add enumeration annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    int addEnumerationEntry ( smart_ptr<Enumeration> enu, const char* name ) {
        if ( !name ) return -1;
        for ( auto & ee : enu->list ) {
            if ( ee.name==name ) {
                return -1;
            }
        }
        enu->list.push_back(Enumeration::EnumEntry());
        int index = (int)enu->list.size() - 1;
        enu->list[index].name = name;
        return index;
    }

    StructureAnnotationPtr makeStructureAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<StructureAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleStructureAnnotation ( Module * module, StructureAnnotationPtr & _ann, Context * context ) {
        StructureAnnotationPtr ann = move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_ex("can't add structure annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addStructureStructureAnnotation ( smart_ptr_raw<Structure> st, StructureAnnotationPtr & _ann, Context * context ) {
        StructureAnnotationPtr ann = move(_ann);
        string err;
        ModuleGroup dummy;
        if ( !ann->touch(st, dummy, AnnotationArgumentList(), err) ) {
            context->throw_error_ex("annotation %s failed to apply to structure %s",
                ann->name.c_str(), st->name.c_str());
        }
        auto annDecl = make_smart<AnnotationDeclaration>();
        annDecl->annotation = ann;
        st->annotations.push_back(annDecl);
    }

    void forEachFunction ( Module * module, const char * name, const TBlock<void,FunctionPtr> & block, Context * context, LineInfoArg * lineInfo ) {
        if ( !module ) context->throw_error_at(*lineInfo, "expecting module, not null");
        vec4f args[1];
        if ( builtin_empty(name) ) {
            auto & fnbn = module->functions;
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                fnbn.foreach([&](auto fnv){
                    args[0] = cast<FunctionPtr>::from(fnv);
                    code->eval(*context);
                });
            },lineInfo);
        } else {
            auto & fnbn = module->functionsByName[hash64z(name)];
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                for ( auto & nv : fnbn ) {
                    args[0] = cast<FunctionPtr>::from(nv);
                    code->eval(*context);
                }
            },lineInfo);
        }
    }

    void forEachGenericFunction ( Module * module, const char * name, const TBlock<void,FunctionPtr> & block, Context * context, LineInfoArg * lineInfo ) {
        if ( !module ) context->throw_error_at(*lineInfo, "expecting module, not null");
        vec4f args[1];
        if ( builtin_empty(name) ) {
            auto & fnbn = module->generics;
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                fnbn.foreach([&](auto nvfn){
                    args[0] = cast<FunctionPtr>::from(nvfn);
                    code->eval(*context);
                });
            },lineInfo);
        } else {
            auto & fnbn = module->genericsByName[hash64z(name)];
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                for ( auto & nv : fnbn ) {
                    args[0] = cast<FunctionPtr>::from(nv);
                    code->eval(*context);
                }
            },lineInfo);
        }
    }

    FunctionAnnotationPtr makeBlockAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<BlockAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    FunctionAnnotationPtr makeFunctionAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<FunctionAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    bool addModuleFunction ( Module * module, FunctionPtr & _func, Context * ) {
        FunctionPtr func = move(_func);
        return module->addFunction(func, true);
    }

    bool addModuleGeneric ( Module * module, FunctionPtr & _func, Context * ) {
        FunctionPtr func = move(_func);
        return module->addGeneric(func, true);
    }

    bool addModuleVariable ( Module * module, VariablePtr & _var, Context * ) {
        VariablePtr var = move(_var);
        return module->addVariable(move(var), true);
    }

    VariablePtr findModuleVariable ( Module * module, const char * name ) {
        return name ? module->findVariable(name) : nullptr;
    }

    bool addModuleStructure ( Module * module, StructurePtr & _struct, Context * ) {
        StructurePtr stru = move(_struct);
        return module->addStructure(stru, true);
    }

    void addModuleFunctionAnnotation ( Module * module, FunctionAnnotationPtr & _ann, Context * context ) {
        FunctionAnnotationPtr ann = move(_ann);
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_ex("can't add function annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addFunctionFunctionAnnotation ( smart_ptr_raw<Function> func, FunctionAnnotationPtr & _ann, Context * context ) {
        FunctionAnnotationPtr ann = move(_ann);
        string err;
        ModuleGroup dummy;
        if ( !ann->apply(func, dummy, AnnotationArgumentList(), err) ) {
            context->throw_error_ex("annotation %s failed to apply to function %s",
                ann->name.c_str(), func->name.c_str());
        }
        auto annDecl = make_smart<AnnotationDeclaration>();
        annDecl->annotation = ann;
        func->annotations.push_back(annDecl);
    }

    void addAndApplyFunctionAnnotation ( smart_ptr_raw<Function> func, smart_ptr_raw<AnnotationDeclaration> & ann, Context * context ) {
        string err;
        if (!ann->annotation->rtti_isFunctionAnnotation()) {
            context->throw_error_ex("annotation %s failed to apply to function %s, not a FunctionAnnotation",
                ann->annotation->name.c_str(), func->name.c_str());
        }
        auto fAnn = (FunctionAnnotation*)ann->annotation.get();
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !fAnn->apply(func, *program->thisModuleGroup, ann->arguments, err) ) {
            context->throw_error_ex("annotation %s failed to apply to function %s",
                ann->annotation->name.c_str(), func->name.c_str());
        }
        func->annotations.push_back(ann);
    }

    Module * thisModule ( Context * context, LineInfoArg * lineinfo ) {
        if ( !context->thisProgram ) {
            context->throw_error_at(*lineinfo, "can't get this module past compilation");
        }
        return context->thisProgram->thisModule.get();
    }

    Module * findRttiModule ( smart_ptr<Program> THAT_PROGRAM, const char * name, Context *, LineInfoArg *) {
        auto found = THAT_PROGRAM->library.findModule(name ? name : "");
        return found ? found : Module::require(name);
    }

    smart_ptr<Function> findRttiFunction ( Module * mod, Func func, Context * context, LineInfoArg * line_info ) {
        if ( !func.PTR ) context->throw_error_at(*line_info, "function not found");
        if ( !mod ) context->throw_error_at(*line_info, "module not found");
        return mod->findFunction(func.PTR->mangledName);
    }

    smart_ptr_raw<Program> thisProgram ( Context * context ) {
        return context->thisProgram;
    }

    Module * compileModule ( Context * context ) {
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !program ) context->throw_error("compileModule only available during compilation");
        return program->thisModule.get();
    }

    smart_ptr_raw<Program> compileProgram ( Context * context ) {
        auto program = daScriptEnvironment::bound->g_Program;
        if ( !program ) context->throw_error("compileProgram only available during compilation");
        return program;
    }

    void astVisit ( smart_ptr_raw<Program> program, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(*line_info, "adapter is required");
        if (!program)
            context->throw_error_at(*line_info, "program is required");
        program->visit(*adapter);
    }

    void astVisitModulesInOrder ( smart_ptr_raw<Program> program, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(*line_info, "adapter is required");
        if (!program)
            context->throw_error_at(*line_info, "program is required");
        program->visitModulesInOrder(*adapter);
    }

    void astVisitFunction ( smart_ptr_raw<Function> func, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(*line_info, "adapter is required");
        if (!func)
            context->throw_error_at(*line_info, "func is required");
        func->visit(*adapter);
    }

    void astVisitExpression ( smart_ptr_raw<Expression> expr, smart_ptr_raw<VisitorAdapter> adapter, Context * context, LineInfoArg * line_info ) {
        if (!adapter)
            context->throw_error_at(*line_info, "adapter is required");
        if (!expr)
            context->throw_error_at(*line_info, "expr is required");
        expr->visit(*adapter);
    }

    char * ast_describe_typedecl ( smart_ptr_raw<TypeDecl> t, bool d_extra, bool d_contracts, bool d_module, Context * context ) {
        return context->stringHeap->allocateString(t->describe(
            d_extra ? TypeDecl::DescribeExtra::yes : TypeDecl::DescribeExtra::no,
            d_contracts ? TypeDecl::DescribeContracts::yes : TypeDecl::DescribeContracts::no,
            d_module ? TypeDecl::DescribeModule::yes : TypeDecl::DescribeModule::no));
    }

    char * ast_describe_typedecl_cpp ( smart_ptr_raw<TypeDecl> t, bool d_substitureRef, bool d_skipRef, bool d_skipConst, bool d_redundantConst, Context * context ) {
        return context->stringHeap->allocateString(describeCppType(t,
            d_substitureRef ? CpptSubstitureRef::yes : CpptSubstitureRef::no,
            d_skipRef ? CpptSkipRef::yes : CpptSkipRef::no,
            d_skipConst ? CpptSkipConst::yes : CpptSkipConst::no,
            d_redundantConst ? CpptRedundantConst::yes : CpptRedundantConst::no));
    }

    char * ast_describe_expression ( smart_ptr_raw<Expression> t, Context * context ) {
        TextWriter ss;
        ss << *t;
        return context->stringHeap->allocateString(ss.str());
    }

    char * ast_describe_function ( smart_ptr_raw<Function> t, Context * context ) {
        TextWriter ss;
        ss << *t;
        return context->stringHeap->allocateString(ss.str());
    }

    char * ast_das_to_string ( Type bt, Context * context ) {
        return context->stringHeap->allocateString(das_to_string(bt));
    }

    char * ast_find_bitfield_name ( smart_ptr_raw<TypeDecl> bft, Bitfield value, Context * context ) {
        return context->stringHeap->allocateString(bft->findBitfieldName(value));
    }

    int64_t ast_find_enum_value ( EnumerationPtr enu, const char * value ) {
        if ( !value ) return 0ul;
        return enu->find(value, 0ul);
    }

    void ast_error ( ProgramPtr prog, const LineInfo & at, const char * message, Context * context, LineInfoArg * lineInfo ) {
        if ( !prog ) context->throw_error_at(*lineInfo,"program can't be null (expecting compiling_program())");
        prog->error(message ? message : "macro error","","",at,CompilationError::macro_failed);
    }

    int32_t get_variant_field_offset ( smart_ptr_raw<TypeDecl> td, int32_t index ) {
        return td->getVariantFieldOffset(index);
    }

    int32_t get_tuple_field_offset ( smart_ptr_raw<TypeDecl> td, int32_t index ) {
        return td->getTupleFieldOffset(index);
    }

    void any_table_foreach ( void * _tab, int keyStride, int valueStride, const TBlock<void,void *,void *> & blk, Context * context, LineInfoArg * at ) {
        auto tab = (Table *) _tab;
        if ( !tab->data ) return;
        char * values = tab->data;
        char * keys = tab->keys;
        for ( uint32_t index=0; index!=tab->capacity; index++, keys+=keyStride, values+=valueStride ) {
            if ( tab->hashes[index] > HASH_KILLED32 ) {
                das_invoke<void>::invoke<void *,void *>(context,at,blk,(void*)keys,(void*)values);
            }
        }
    }

    void any_array_foreach ( void * _arr, int stride, const TBlock<void,void *> & blk, Context * context, LineInfoArg * at ) {
        auto arr = (Array *) _arr;
        if ( !arr->data ) return;
        char * values = arr->data;
        for ( uint32_t index=0; index!=arr->size; index++, values+=stride ) {
            das_invoke<void>::invoke<void *>(context,at,blk,(void*)values);
        }
    }

    int32_t any_array_size ( void * _arr ) {
        return int32_t(((Array *) _arr)->size);
    }

    int32_t any_table_size ( void * _tab ) {
        return int32_t(((Table *) _tab)->size);
    }

    void for_each_typedef ( Module * mod, const TBlock<void,TTemporary<char *>,TypeDeclPtr> & block, Context * context, LineInfoArg * at ) {
        mod->aliasTypes.foreach([&](auto aliasType){
            das_invoke<void>::invoke<const char *,TypeDeclPtr>(context,at,block,aliasType->alias.c_str(),aliasType);
        });
    }

    void for_each_enumeration ( Module * mod, const TBlock<void,EnumerationPtr> & block, Context * context, LineInfoArg * at ) {
        mod->enumerations.foreach([&](auto penum){
            das_invoke<void>::invoke<EnumerationPtr>(context,at,block,penum);
        });
    }

    void for_each_structure ( Module * mod, const TBlock<void,StructurePtr> & block, Context * context, LineInfoArg * at ) {
        mod->structures.foreach([&](auto pst){
            das_invoke<void>::invoke<StructurePtr>(context,at,block,pst);
        });
    }

    void for_each_generic ( Module * mod, const TBlock<void,FunctionPtr> & block, Context * context, LineInfoArg * at ) {
        mod->generics.foreach([&](auto fn){
            das_invoke<void>::invoke<FunctionPtr>(context,at,block,fn);
        });
    }

    void for_each_global ( Module * mod, const TBlock<void,VariablePtr> & block, Context * context, LineInfoArg * at ) {
        mod->globals.foreach([&](auto var){
            das_invoke<void>::invoke<VariablePtr>(context,at,block,var);
        });
    }

    void for_each_call_macro ( Module * mod, const TBlock<void,TTemporary<char *>> & block, Context * context, LineInfoArg * at ) {
        for ( auto & td : mod->callThis ) {
            das_invoke<void>::invoke<const char *>(context,at,block,td.first.c_str());
        }
    }

    bool isSameAstType ( TypeDeclPtr THIS,
                     TypeDeclPtr decl,
                     RefMatters refMatters,
                     ConstMatters constMatters,
                     TemporaryMatters temporaryMatters ) {
        return THIS->isSameType(*decl,refMatters,constMatters,temporaryMatters);
    }

    void builtin_structure_for_each_field ( const BasicStructureAnnotation & ann,
        const TBlock<void,char *,char*,TypeDeclPtr,uint32_t> & block, Context * context, LineInfoArg * at ) {
        for ( auto & it : ann.fields ) {
            const auto & fld = it.second;
            das_invoke<void>::invoke<const char *,const char *,TypeDeclPtr,uint32_t>(context,at,block,
                it.first.c_str(), fld.cppName.c_str(),fld.decl,fld.offset);
        }
    }

    char * get_mangled_name ( smart_ptr_raw<Function> func, Context * context ) {
        return context->stringHeap->allocateString(func->getMangledName());
    }

    char * get_mangled_name_t ( smart_ptr_raw<TypeDecl> typ, Context * context ) {
        return context->stringHeap->allocateString(typ->getMangledName());
    }

    class MangledNameParserCtx : public MangledNameParser {
        virtual void error ( const string & err, const char * ch ) override {
            context->throw_error_at(*at, "%s, near %s", err.c_str(), ch );
        }
    public:
        Context *   context = nullptr;
        LineInfo *  at = nullptr;
    };

    TypeDeclPtr parseMangledNameFn ( const char * txt, ModuleGroup & lib, Module * thisModule, Context * context, LineInfoArg * at ) {
        if ( !txt ) context->throw_error_at(*at, "can't parse empty mangled name");
        MangledNameParserCtx parser;
        parser.context = context;
        parser.at = at;
        return parser.parseTypeFromMangledName(txt, lib, thisModule);
    }

    void forceAtRaw ( const smart_ptr_raw<Expression> & expr, const LineInfo & at ) {
        forceAt(expr, at);
    }

    bool isExprLikeCall ( const ExpressionPtr & expr ) {
        return expr->rtti_isCallLikeExpr();
    }

    bool isExprConst ( const ExpressionPtr & expr ) {
        return expr->rtti_isConstant();
    }

    bool isTempType ( TypeDeclPtr ptr, bool refMatters ) {
        return ptr->isTempType(refMatters);
    }

    ExpressionPtr makeCall ( const LineInfo & at, const char * name ) {
        name = name ? name : "";
        auto program = daScriptEnvironment::bound->g_Program;
        return program->makeCall(at, name);
    }

    float4 evalSingleExpression ( const ExpressionPtr & expr, bool & ok ) {
        ok = true;
        das::Context ctx;
        auto node = expr->simulate(ctx);
        ctx.restart();
        vec4f result = ctx.evalWithCatch(node);
        if ( ctx.getException() ) ok = false;
        return result;
    }

    bool builtin_isVisibleDirectly ( Module * from, Module * too ) {
        return from->isVisibleDirectly(too);
    }

    bool builtin_hasField ( TypeDeclPtr ptr, const char * field, bool constant ) {
        if ( !field || !ptr ) return false;
        if ( ptr->baseType==Type::tStructure ) {
            return ptr->structType->findField(field);
        } else if ( ptr->baseType==Type::tHandle ) {
            return ptr->annotation->makeFieldType(field, constant);
        } else if ( ptr->baseType==Type::tPointer && ptr->firstType ) {
            return builtin_hasField(ptr->firstType, field, constant);
        }
        return false;
    }

    class Module_Ast : public Module {
    public:
        template <typename RecAnn>
        void addRecAnnotation ( ModuleLibrary & lib ) {
            auto rec = make_smart<RecAnn>(lib);
            addAnnotation(rec);
            initRecAnnotation(rec, lib);
        }

        template <typename TT>
        smart_ptr<TT> addExpressionAnnotation ( const smart_ptr<TT> & ann ) {
            addAnnotation(ann);
            return ann;
        }

        Module_Ast() : Module("ast") {
            DAS_PROFILE_SECTION("Module_Ast");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            lib.addModule(Module::require("rtti"));
            // THE MAGNIFICENT TWO
            addTypeInfoMacro(make_smart<AstTypeDeclMacro>());
            addTypeInfoMacro(make_smart<AstFunctionMacro>());
            // QUOTE
            addCall<ExprQuote>("quote");
            // FLAGS?
            addAlias(makeTypeDeclFlags());
            addAlias(makeFieldDeclarationFlags());
            addAlias(makeStructureFlags());
            addAlias(makeExprGenFlagsFlags());
            addAlias(makeExprFlagsFlags());
            addAlias(makeExprPrintFlagsFlags());
            addAlias(makeFunctionFlags());
            addAlias(makeMoreFunctionFlags());
            addAlias(makeFunctionSideEffectFlags());
            addAlias(makeVariableFlags());
            addAlias(makeVariableAccessFlags());
            addAlias(makeExprBlockFlags());
            addAlias(makeExprAtFlags());
            addAlias(makeExprMakeLocalFlags());
            addAlias(makeExprAscendFlags());
            addAlias(makeExprCastFlags());
            addAlias(makeExprVarFlags());
            addAlias(makeExprMakeStructFlags());
            addAlias(makeMakeFieldDeclFlags());
            addAlias(makeExprFieldDerefFlags());
            addAlias(makeExprFieldFieldFlags());
            addAlias(makeExprSwizzleFieldFlags());
            addAlias(makeExprYieldFlags());
            addAlias(makeExprReturnFlags());
            addAlias(makeExprMakeBlockFlags());
            // ENUMS
            addEnumeration(make_smart<EnumerationSideEffects>());
            addEnumeration(make_smart<EnumerationCaptureMode>());
            // modules
            addAnnotation(make_smart<AstModuleLibraryAnnotation>(lib));
            // AST TYPES (due to a lot of xrefs we declare everyone as recursive type)
            auto exa = make_smart<AstExprAnnotation<Expression>>("Expression",lib);
            addAnnotation(exa);
            auto tda = make_smart<AstTypeDeclAnnnotation>(lib);
            addAnnotation(tda);
            auto sta = make_smart<AstStructureAnnotation>(lib);
            addAnnotation(sta);
            auto fta = make_smart<AstFieldDeclarationAnnotation>(lib);
            addAnnotation(fta);
            auto ene = make_smart<AstEnumEntryAnnotation>(lib);
            addAnnotation(ene);
            auto ena = make_smart<AstEnumerationAnnotation>(lib);
            addAnnotation(ena);
            auto fna = make_smart<AstFunctionAnnotation>(lib);
            addAnnotation(fna);
            auto iha = make_smart<AstInferHistoryAnnotation>(lib);
            addAnnotation(iha);
            auto vaa = make_smart<AstVariableAnnotation>(lib);
            addAnnotation(vaa);
            initRecAnnotation(tda, lib);
            initRecAnnotation(sta, lib);
            initRecAnnotation(fta, lib);
            initRecAnnotation(ene, lib);
            initRecAnnotation(ena, lib);
            initRecAnnotation(exa, lib);
            initRecAnnotation(fna, lib);
            initRecAnnotation(iha, lib);
            initRecAnnotation(vaa, lib);
            // reader macro
            addAnnotation(make_smart<AstReaderMacroAnnotation>(lib));
            // call macro
            addAnnotation(make_smart<AstCallMacroAnnotation>(lib));
            // expressions (in order of inheritance)
            addExpressionAnnotation(make_smart<AstExprBlockAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprLetAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprStringBuilderAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstMakeFieldDeclAnnotation>(lib));
            addExpressionAnnotation(make_smart<AstMakeStructAnnotation>(lib));
            addExpressionAnnotation(make_smart<AstExprNamedCallAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprLooksLikeCallAnnotation<ExprLooksLikeCall>>("ExprLooksLikeCall",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprCallFuncAnnotation<ExprCallFunc>>("ExprCallFunc",lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprNewAnnotation>(lib))->from("ExprCallFunc");
            addExpressionAnnotation(make_smart<AstExprCallAnnotation>(lib))->from("ExprCallFunc");
            addExpressionAnnotation(make_smart<AstExprPtr2RefAnnotation<ExprPtr2Ref>>("ExprPtr2Ref",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprNullCoalescingAnnotation>(lib))->from("ExprPtr2Ref");
            addExpressionAnnotation(make_smart<AstExprAtAnnotation<ExprAt>>("ExprAt",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprAtAnnotation<ExprSafeAt>>("ExprSafeAt",lib))->from("ExprAt");
            addExpressionAnnotation(make_smart<AstExprIsAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprAnnotation<ExprOp>>("ExprOp",lib))->from("ExprCallFunc");
            addExpressionAnnotation(make_smart<AstExprOp2Annotation<ExprOp2>>("ExprOp2",lib))->from("ExprOp");
            addExpressionAnnotation(make_smart<AstExprOp3Annotation>(lib))->from("ExprOp");
            addExpressionAnnotation(make_smart<AstExprCopyAnnotation>(lib))->from("ExprOp2");
            addExpressionAnnotation(make_smart<AstExprOp2Annotation<ExprMove>>("ExprMove",lib))->from("ExprOp2");
            addExpressionAnnotation(make_smart<AstExprOp2Annotation<ExprClone>>("ExprClone",lib))->from("ExprOp2");
            addExpressionAnnotation(make_smart<AstExprWithAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprAssumeAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprWhileAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprTryCatchAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprIfThenElseAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprForAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprMakeLocalAnnotation<ExprMakeLocal>>("ExprMakeLocal",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprMakeStructAnnotation>(lib))->from("ExprMakeLocal");
            addExpressionAnnotation(make_smart<AstExprMakeVariantAnnotation>(lib))->from("ExprMakeLocal");
            addExpressionAnnotation(make_smart<AstExprMakeArrayAnnotation<ExprMakeArray>>("ExprMakeArray",lib))->from("ExprMakeLocal");
            addExpressionAnnotation(make_smart<AstExprMakeTupleAnnotation>(lib))->from("ExprMakeArray");
            addExpressionAnnotation(make_smart<AstExprArrayComprehensionAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstTypeInfoMacroAnnotation>(lib));
            addExpressionAnnotation(make_smart<AstExprTypeInfoAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprTypeDeclAnnotation>(lib))->from("Expression");
            // expressions with no extra syntax
            addExpressionAnnotation(make_smart<AstExprLabelAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprGotoAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprRef2ValueAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprRef2PtrAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprAddrAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprAssertAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprQuoteAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprStaticAssertAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprDebugAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprInvokeAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprEraseAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprFindAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprKeyExistsAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprAscendAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprCastAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprDeleteAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprVarAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprSwizzleAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprFieldAnnotation<ExprField>>("ExprField",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprSafeFieldAnnotation>(lib))->from("ExprField");
            addExpressionAnnotation(make_smart<AstExprFieldAnnotation<ExprIsVariant>>("ExprIsVariant",lib))->from("ExprField");
            addExpressionAnnotation(make_smart<AstExprFieldAnnotation<ExprAsVariant>>("ExprAsVariant",lib))->from("ExprField");
            addExpressionAnnotation(make_smart<AstExprSafeAsVariantAnnotation>(lib))->from("ExprField");
            addExpressionAnnotation(make_smart<AstExprOp1Annotation>(lib))->from("ExprOp");
            addExpressionAnnotation(make_smart<AstExprReturnAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprYieldAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExpressionAnnotation<ExprBreak>>("ExprBreak",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExpressionAnnotation<ExprContinue>>("ExprContinue",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprConstAnnotation<ExprConst>>("ExprConst",lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprConstAnnotation<ExprFakeContext>>("ExprFakeContext",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstAnnotation<ExprFakeLineInfo>>("ExprFakeLineInfo",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstPtr,void *>>("ExprConstPtr",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt8 ,int8_t>> ("ExprConstInt8",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt16,int16_t>>("ExprConstInt16",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt64,int64_t>>("ExprConstInt64",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt  ,int32_t>>("ExprConstInt",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt2 ,int2>>   ("ExprConstInt2",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt3 ,int3>>   ("ExprConstInt3",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt4 ,int4>>   ("ExprConstInt4",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt8 ,uint8_t>> ("ExprConstUInt8",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt16,uint16_t>>("ExprConstUInt16",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt64,uint64_t>>("ExprConstUInt64",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt  ,uint32_t>>("ExprConstUInt",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt2 ,uint2>>   ("ExprConstUInt2",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt3 ,uint3>>   ("ExprConstUInt3",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt4 ,uint4>>   ("ExprConstUInt4",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstRange ,range>> ("ExprConstRange",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstURange,urange>>("ExprConstURange",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat  ,float>> ("ExprConstFloat",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat2 ,float2>>("ExprConstFloat2",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat3 ,float3>>("ExprConstFloat3",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat4 ,float4>>("ExprConstFloat4",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstDouble,double>> ("ExprConstDouble",lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstTAnnotation<ExprConstBool,bool>> ("ExprConstBool",lib))->from("ExprConst");
            addAnnotation(make_smart<AstCaptureEntryAnnotation>(lib));
            addExpressionAnnotation(make_smart<AstExprMakeBlockAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprMakeGeneratorAnnotation>(lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprLikeCallAnnotation<ExprMemZero>>("ExprMemZero",lib))->from("ExprLooksLikeCall");
            addExpressionAnnotation(make_smart<AstExprConstEnumerationAnnotation>(lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstBitfieldAnnotation>(lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprConstStringAnnotation>(lib))->from("ExprConst");
            addExpressionAnnotation(make_smart<AstExprReaderAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprUnsafeAnnotation>(lib))->from("Expression");
            addExpressionAnnotation(make_smart<AstExprCallMacroAnnotation>(lib))->from("ExprLooksLikeCall");
            // vector functions for custom containers
            addExtern<DAS_BIND_FUN(mks_vector_emplace)>(*this, lib, "emplace",
                SideEffects::modifyArgument, "mks_vector_emplace")
                    ->args({"vec","value"})->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_pop)>(*this, lib, "pop",
                SideEffects::modifyArgument, "mks_vector_pop")
                    ->arg("vec")->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_clear)>(*this, lib, "clear",
                SideEffects::modifyArgument, "mks_vector_clear")
                    ->arg("vec")->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_resize)>(*this, lib, "resize",
                SideEffects::modifyArgument, "mks_vector_resize")
                    ->args({"vec","newSize"})->generated = true;
            // visitor
            addAnnotation(make_smart<AstVisitorAdapterAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeVisitor)>(*this, lib,  "make_visitor",
                SideEffects::modifyExternal, "makeVisitor")
                    ->args({"class","info","context"});
            addExtern<DAS_BIND_FUN(thisProgram)>(*this, lib,  "this_program",
                SideEffects::accessExternal, "thisProgram")
                    ->arg("context");
            addExtern<DAS_BIND_FUN(thisModule)>(*this, lib,  "this_module",
                SideEffects::accessExternal, "thisModule")
                    ->args({"context","line"});
            addExtern<DAS_BIND_FUN(findRttiModule)>(*this, lib,  "find_module_via_rtti",
                SideEffects::accessExternal, "findRttiModule")
                    ->args({"program","name","context","lineinfo"});
            addExtern<DAS_BIND_FUN(findRttiFunction)>(*this, lib,  "find_module_function_via_rtti",
                SideEffects::accessExternal, "findRttiFunction")
                    ->args({"module","function","context","lineinfo"});
            addExtern<DAS_BIND_FUN(compileProgram)>(*this, lib,  "compiling_program",
                SideEffects::accessExternal, "compileProgram")
                    ->arg("context");
            addExtern<DAS_BIND_FUN(compileModule)>(*this, lib,  "compiling_module",
                SideEffects::accessExternal, "compileModule")
                    ->arg("context");;
            addExtern<DAS_BIND_FUN(forEachFunction)>(*this, lib,  "for_each_function",
                SideEffects::accessExternal, "forEachFunction")
                    ->args({"module","name","block","context","line"});
            addExtern<DAS_BIND_FUN(forEachGenericFunction)>(*this, lib,  "for_each_generic",
                SideEffects::accessExternal, "forEachGenericFunction")
                    ->args({"module","name","block","context","line"});
            addExtern<DAS_BIND_FUN(astVisit)>(*this, lib,  "visit",
                SideEffects::accessExternal, "astVisit")
                    ->args({"program","adapter","context","line"});
            addExtern<DAS_BIND_FUN(astVisitModulesInOrder)>(*this, lib,  "visit_modules",
                SideEffects::accessExternal, "astVisitModules")
                    ->args({"program","adapter","context","line"});
            addExtern<DAS_BIND_FUN(astVisitFunction)>(*this, lib,  "visit",
                SideEffects::accessExternal, "astVisitFunction")
                    ->args({"function","adapter","context","line"});
            addExtern<DAS_BIND_FUN(astVisitExpression)>(*this, lib,  "visit",
                SideEffects::accessExternal, "astVisitExpression")
                    ->args({"expression","adapter","context","line"});
            addExtern<DAS_BIND_FUN(forceAtRaw)>(*this, lib,  "force_at",
                SideEffects::accessExternal, "forceAtRaw")
                    ->args({"expression","at"});
            addExtern<DAS_BIND_FUN(parseMangledNameFn)>(*this, lib,  "parse_mangled_name",
                SideEffects::none, "parseMangledNameFn")
                    ->args({"txt","lib","thisModule","context","line"});
            addExtern<DAS_BIND_FUN(collectDependencies)>(*this, lib,  "collect_dependencies",
                SideEffects::invoke, "collectDependencies")
                    ->args({"function","block","context","line"});;
            // function annotation
            addAnnotation(make_smart<AstFunctionAnnotationAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeFunctionAnnotation)>(*this, lib,  "make_function_annotation",
                SideEffects::modifyExternal, "makeFunctionAnnotation")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(makeBlockAnnotation)>(*this, lib,  "make_block_annotation",
                SideEffects::modifyExternal, "makeBlockAnnotation")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleFunctionAnnotation)>(*this, lib,  "add_function_annotation",
                SideEffects::modifyExternal, "addModuleFunctionAnnotation")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(addModuleFunction)>(*this, lib, "add_function",
                SideEffects::modifyExternal, "addModuleFunction")
                    ->args({"module","function","context"});
            addExtern<DAS_BIND_FUN(addModuleGeneric)>(*this, lib, "add_generic",
                SideEffects::modifyExternal, "addModuleGeneric")
                    ->args({"module","function","context"});
            addExtern<DAS_BIND_FUN(addFunctionFunctionAnnotation)>(*this, lib,  "add_function_annotation",
                SideEffects::modifyExternal, "addFunctionFunctionAnnotation")
                    ->args({"function","annotation","context"});
            addExtern<DAS_BIND_FUN(addAndApplyFunctionAnnotation)>(*this, lib,  "add_function_annotation",
                SideEffects::modifyExternal, "addAndApplyFunctionAnnotation")
                    ->args({"function","annotation","context"});
            // variables
            addExtern<DAS_BIND_FUN(addModuleVariable)>(*this, lib, "add_variable",
                SideEffects::modifyExternal, "addModuleVariable")
                    ->args({"module","variable","context"});
            addExtern<DAS_BIND_FUN(findModuleVariable)>(*this, lib, "find_variable",
                SideEffects::modifyExternal, "findModuleVariable")
                    ->args({"module","variable"});
            // structure annotation
            addAnnotation(make_smart<AstStructureAnnotationAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeStructureAnnotation)>(*this, lib,  "make_structure_annotation",
                SideEffects::modifyExternal, "makeStructureAnnotation")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleStructureAnnotation)>(*this, lib,  "add_structure_annotation",
                SideEffects::modifyExternal, "addModuleStructureAnnotation")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(addStructureStructureAnnotation)>(*this, lib,  "add_structure_annotation",
                SideEffects::modifyExternal, "addStructureStructureAnnotation")
                    ->args({"structure","annotation","context"});
            addExtern<DAS_BIND_FUN(addModuleStructure)>(*this, lib, "add_structure",
                SideEffects::modifyExternal, "addModuleStructure")
                    ->args({"module","structure","context"});
            addExtern<DAS_BIND_FUN(clone_structure)>(*this, lib,  "clone_structure",
                SideEffects::none, "clone_structure")
                    ->arg("structure");
            // enumeration annotation
            addAnnotation(make_smart<AstEnumerationAnnotationAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeEnumerationAnnotation)>(*this, lib,  "make_enumeration_annotation",
                SideEffects::modifyExternal, "makeEnumerationAnnotation")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleEnumerationAnnotation)>(*this, lib,  "add_enumeration_annotation",
                SideEffects::modifyExternal, "addModuleEnumerationAnnotation")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(addEnumerationEntry)>(*this, lib,  "add_enumeration_entry",
                SideEffects::modifyExternal, "addEnumerationEntry")
                    ->args({"enum","name"});
            // pass macro
            addAnnotation(make_smart<AstPassMacroAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makePassMacro)>(*this, lib,  "make_pass_macro",
                SideEffects::modifyExternal, "makePassMacro")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleInferMacro)>(*this, lib,  "add_infer_macro",
                SideEffects::modifyExternal, "addModuleInferMacro")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(addModuleInferDirtyMacro)>(*this, lib,  "add_dirty_infer_macro",
                SideEffects::modifyExternal, "addModuleInferDirtyMacro")
                    ->args({"module","annotation","context"});
            // lint macro
            addExtern<DAS_BIND_FUN(addModuleLintMacro)>(*this, lib,  "add_lint_macro",
                SideEffects::modifyExternal, "addModuleLintMacro")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(addModuleGlobalLintMacro)>(*this, lib,  "add_global_lint_macro",
                SideEffects::modifyExternal, "addModuleGlobalLintMacro")
                    ->args({"module","annotation","context"});
            // optimization macro
            addExtern<DAS_BIND_FUN(addModuleOptimizationMacro)>(*this, lib,  "add_optimization_macro",
                SideEffects::modifyExternal, "addModuleOptimizationMacro")
                    ->args({"module","annotation","context"});
            // reader macro
            addExtern<DAS_BIND_FUN(makeReaderMacro)>(*this, lib,  "make_reader_macro",
                SideEffects::modifyExternal, "makeReaderMacro")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleReaderMacro)>(*this, lib,  "add_reader_macro",
                SideEffects::modifyExternal, "addModuleReaderMacro")
                    ->args({"module","annotation","context"});
            // call macro
            addExtern<DAS_BIND_FUN(makeCallMacro)>(*this, lib,  "make_call_macro",
                SideEffects::modifyExternal, "makeCallMacro")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleCallMacro)>(*this, lib,  "add_call_macro",
                SideEffects::modifyExternal, "addModuleCallMacro")
                    ->args({"module","annotation","context"});
            addExtern<DAS_BIND_FUN(for_each_call_macro)>(*this, lib,  "for_each_call_macro",
                SideEffects::modifyExternal, "for_each_call_macro")
                    ->args({"module","block","context","line"});
            // type info macro
            addExtern<DAS_BIND_FUN(makeTypeInfoMacro)>(*this, lib,  "make_typeinfo_macro",
                SideEffects::modifyExternal, "makeTypeInfoMacro")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleTypeInfoMacro)>(*this, lib,  "add_typeinfo_macro",
                SideEffects::modifyExternal, "addModuleTypeInfoMacro")
                    ->args({"module","annotation","context"});
            // variant macro
            addAnnotation(make_smart<AstVariantMacroAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeVariantMacro)>(*this, lib,  "make_variant_macro",
                SideEffects::modifyExternal, "makeVariantMacro")
                    ->args({"name","class","info","context"});
            addExtern<DAS_BIND_FUN(addModuleVariantMacro)>(*this, lib,  "add_variant_macro",
                SideEffects::modifyExternal, "addModuleVariantMacro")
                    ->args({"module","annotation","context"});
            // helper functions
            addExtern<DAS_BIND_FUN(ast_describe_typedecl)>(*this, lib,  "describe_typedecl",
                SideEffects::none, "ast_describe_typedecl")
                    ->args({"type","extra","contracts","module","context"});
            addExtern<DAS_BIND_FUN(ast_describe_typedecl_cpp)>(*this, lib,  "describe_typedecl_cpp",
                SideEffects::none, "ast_describe_typedecl_cpp")
                    ->args({"type","substitueRef","skipRef","skipConst","redundantConst","context"});
            addExtern<DAS_BIND_FUN(ast_describe_expression)>(*this, lib,  "describe_expression",
                SideEffects::none, "ast_describe_expression")
                    ->args({"expression","context"});
            addExtern<DAS_BIND_FUN(ast_describe_function)>(*this, lib,  "describe_function",
                SideEffects::none, "describe_function")
                    ->args({"function","context"});
            addExtern<DAS_BIND_FUN(ast_find_bitfield_name)>(*this, lib,  "find_bitfield_name",
                SideEffects::none, "find_bitfield_name")
                    ->args({"bit","value","context"});
            addExtern<DAS_BIND_FUN(ast_find_enum_value)>(*this, lib,  "find_enum_value",
                SideEffects::none, "find_enum_value")
                    ->args({"enum","value"});
            addExtern<DAS_BIND_FUN(get_mangled_name)>(*this, lib,  "get_mangled_name",
                SideEffects::none, "get_mangled_name")
                    ->args({"function","context"});
            addExtern<DAS_BIND_FUN(get_mangled_name_t)>(*this, lib,  "get_mangled_name",
                SideEffects::none, "get_mangled_name_t")
                    ->args({"type","context"});
            // type conversion functions
            addExtern<DAS_BIND_FUN(ast_das_to_string)>(*this, lib,  "das_to_string",
                SideEffects::none, "das_to_string")
                    ->args({"type","context"});
            // clone
            addExtern<DAS_BIND_FUN(clone_expression)>(*this, lib,  "clone_expression",
                SideEffects::none, "clone_expression")
                    ->arg("expression");
            addExtern<DAS_BIND_FUN(clone_function)>(*this, lib,  "clone_function",
                SideEffects::none, "clone_function")
                    ->arg("function");
            addExtern<DAS_BIND_FUN(clone_variable)>(*this, lib,  "clone_variable",
                SideEffects::none, "clone_variable")
                    ->arg("variable");
            // type
            addExtern<DAS_BIND_FUN(isTempType)>(*this, lib,  "is_temp_type",
                SideEffects::none, "isTempType")
                    ->args({"type","refMatters"});
            addExtern<DAS_BIND_FUN(isSameAstType)>(*this, lib,  "is_same_type",
                SideEffects::none, "isSameAstType")
                    ->args({"type1","type2","refMatters","constMatters","tempMatters"});
            addExtern<DAS_BIND_FUN(clone_type)>(*this, lib,  "clone_type",
                SideEffects::none, "clone_type")
                    ->arg("type");
            addExtern<DAS_BIND_FUN(get_variant_field_offset)>(*this, lib,  "get_variant_field_offset",
                SideEffects::none, "get_variant_field_offset")
                    ->args({"variant","index"});
            addExtern<DAS_BIND_FUN(get_tuple_field_offset)>(*this, lib,  "get_tuple_field_offset",
                SideEffects::none, "get_tuple_field_offset")
                    ->args({"typle","index"});;
            addExtern<DAS_BIND_FUN(any_table_foreach)>(*this, lib,  "any_table_foreach",
                SideEffects::modifyArgumentAndExternal, "any_table_foreach")
                    ->args({"table","keyStride","valueStride","block","context","line"});
            addExtern<DAS_BIND_FUN(any_array_foreach)>(*this, lib,  "any_array_foreach",
                SideEffects::modifyArgumentAndExternal, "any_array_foreach")
                    ->args({"array","stride","block","context","line"});
            addExtern<DAS_BIND_FUN(any_array_size)>(*this, lib,  "any_array_size",
                SideEffects::none, "any_array_size")
                    ->arg("array");
            addExtern<DAS_BIND_FUN(any_table_size)>(*this, lib,  "any_table_size",
                SideEffects::none, "any_table_size")
                    ->arg("table");
            // module
            addExtern<DAS_BIND_FUN(for_each_typedef)>(*this, lib,  "for_each_typedef",
                SideEffects::modifyExternal, "for_each_typedef")
                    ->args({"module","block","context","line"});
            addExtern<DAS_BIND_FUN(for_each_enumeration)>(*this, lib,  "for_each_enumeration",
                SideEffects::modifyExternal, "for_each_enumeration")
                    ->args({"module","block","context","line"});
            addExtern<DAS_BIND_FUN(for_each_structure)>(*this, lib,  "for_each_structure",
                SideEffects::modifyExternal, "for_each_structure")
                    ->args({"module","block","context","line"});
            addExtern<DAS_BIND_FUN(for_each_generic)>(*this, lib,  "for_each_generic",
                SideEffects::modifyExternal, "for_each_generic")
                    ->args({"module","block","context","line"});
            addExtern<DAS_BIND_FUN(for_each_global)>(*this, lib,  "for_each_global",
                SideEffects::modifyExternal, "for_each_global")
                    ->args({"module","block","context","line"});
            addExtern<DAS_BIND_FUN(builtin_structure_for_each_field)>(*this, lib,  "for_each_field",
                SideEffects::modifyExternal, "builtin_structure_for_each_field")
                    ->args({"annotation","block","context","line"});
            addExtern<DAS_BIND_FUN(builtin_hasField)>(*this, lib, "has_field",
                SideEffects::modifyExternal, "builtin_hasField")
                    ->args({"type","fieldName","constant"});
            // type
            addExtern<DAS_BIND_FUN(builtin_isVisibleDirectly)>(*this, lib, "is_visible_directly",
                SideEffects::modifyExternal, "builtin_isVisibleDirectly")
                    ->args({"from_module","which_module"});
            // context
            addAnnotation(make_smart<AstContextAnnotation>(lib));
            addExtern<DAS_BIND_FUN(getAstContext)>(*this, lib,  "get_ast_context",
                SideEffects::modifyExternal, "get_ast_context")
                    ->args({"program","expression","block","context","line"});
            // code generation
            addExtern<DAS_BIND_FUN(makeClone)>(*this, lib,  "make_clone_structure",
                SideEffects::none, "makeClone")
                    ->arg("structure");
            // expression generation
            addExtern<DAS_BIND_FUN(isExprLikeCall)>(*this, lib,  "is_expr_like_call",
                SideEffects::none, "isExprLikeCall")
                    ->arg("expression");
            addExtern<DAS_BIND_FUN(isExprConst)>(*this, lib,  "is_expr_const",
                SideEffects::none, "isExprConst")
                    ->arg("expression");
            addExtern<DAS_BIND_FUN(makeCall)>(*this, lib,  "make_call",
                SideEffects::none, "makeCall")
                    ->args({"at","name"});
            addExtern<DAS_BIND_FUN(evalSingleExpression)>(*this, lib, "eval_single_expression",
                SideEffects::none, "evalSingleExpression")
                    ->args({"expr","ok"})->unsafeOperation = true;
            // errors
            addExtern<DAS_BIND_FUN(ast_error)>(*this, lib,  "macro_error",
                SideEffects::modifyArgumentAndExternal, "ast_error")
                    ->args({"porogram","at","message","context","line"});;
            // add builtin module
            compileBuiltinModule("ast.das",ast_das,sizeof(ast_das));
            // lets make sure its all aot ready
            // verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/ast/ast.h\"\n";
            tw << "#include \"daScript/simulate/aot_builtin_ast.h\"\n";
            return ModuleAotType::cpp;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Ast,das);
