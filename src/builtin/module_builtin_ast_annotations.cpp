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
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ForLoopMacro,ForLoopMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(ReaderMacro,ReaderMacro)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(CommentReader,CommentReader)
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
            addField<DAS_BIND_MANAGED_FIELD(canShadow)>("canShadow");
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

    struct AstExprAscendAnnotation : AstExpressionAnnotation<ExprAscend> {
        AstExprAscendAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprAscend> ("ExprAscend", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addField<DAS_BIND_MANAGED_FIELD(ascType)>("ascType");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addFieldEx ( "ascendFlags", "ascendFlags", offsetof(ExprAscend, ascendFlags), makeExprAscendFlags() );
        }
    };

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

    struct AstExprYieldAnnotation : AstExpressionAnnotation<ExprYield> {
        AstExprYieldAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprYield> ("ExprYield", ml) {
            addField<DAS_BIND_MANAGED_FIELD(subexpr)>("subexpr");
            addFieldEx ( "returnFlags", "returnFlags", offsetof(ExprYield, returnFlags), makeExprYieldFlags() );
        }
    };

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

    template <typename TT>
    smart_ptr<TT> Module_Ast::addExpressionAnnotation ( const smart_ptr<TT> & ann ) {
        addAnnotation(ann);
        return ann;
    }

    void Module_Ast::registerAnnotations(ModuleLibrary & lib) {
        // THE MAGNIFICENT TWO
        addTypeInfoMacro(make_smart<AstTypeDeclMacro>());
        addTypeInfoMacro(make_smart<AstFunctionMacro>());
        // QUOTE
        addCall<ExprQuote>("quote");
        // flags
        registerFlags(lib);
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
        // misc
        addAnnotation(make_smart<AstContextAnnotation>(lib));
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
        addExpressionAnnotation(make_smart<AstExprUnsafeAnnotation>(lib))->from("Expression");
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
    }
    void Module_Ast::registerMacroExpressions(ModuleLibrary & lib){
        addExpressionAnnotation(make_smart<AstExprReaderAnnotation>(lib))->from("Expression");
        addExpressionAnnotation(make_smart<AstExprCallMacroAnnotation>(lib))->from("ExprLooksLikeCall");
    }
}
