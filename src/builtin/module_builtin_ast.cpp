#include "daScript/misc/platform.h"

#include "module_builtin_rtti.h"

#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/misc/performance_time.h"

using namespace das;

namespace das {
    class VisitorAdapter;
};

MAKE_TYPE_FACTORY(TypeDecl,TypeDecl)
MAKE_TYPE_FACTORY(FieldDeclaration, Structure::FieldDeclaration)
MAKE_TYPE_FACTORY(Structure,Structure)
MAKE_TYPE_FACTORY(Enumeration,Enumeration)
MAKE_TYPE_FACTORY(Expression,Expression)
MAKE_TYPE_FACTORY(Function,Function)
MAKE_TYPE_FACTORY(InferHistory, Function::InferHistory)
MAKE_TYPE_FACTORY(Variable,Variable)
MAKE_TYPE_FACTORY(VisitorAdapter,VisitorAdapter)
MAKE_TYPE_FACTORY(FunctionAnnotation,FunctionAnnotation)
MAKE_TYPE_FACTORY(StructureAnnotation,StructureAnnotation)
MAKE_TYPE_FACTORY(PassMacro,PassMacro)
MAKE_TYPE_FACTORY(VariantMacro,VariantMacro)
MAKE_TYPE_FACTORY(ReaderMacro,ReaderMacro)
MAKE_TYPE_FACTORY(ModuleGroup,ModuleGroup)
MAKE_TYPE_FACTORY(ModuleLibrary,ModuleLibrary)

MAKE_TYPE_FACTORY(ExprBlock,ExprBlock)
MAKE_TYPE_FACTORY(ExprLet,ExprLet)
MAKE_TYPE_FACTORY(ExprStringBuilder,ExprStringBuilder)
MAKE_TYPE_FACTORY(ExprNew,ExprNew)
MAKE_TYPE_FACTORY(ExprNamedCall,ExprNamedCall)
MAKE_TYPE_FACTORY(MakeFieldDecl,MakeFieldDecl)
MAKE_TYPE_FACTORY(MakeStruct,MakeStruct)
MAKE_TYPE_FACTORY(ExprCall,ExprCall)
MAKE_TYPE_FACTORY(ExprLooksLikeCall,ExprLooksLikeCall)
MAKE_TYPE_FACTORY(ExprNullCoalescing,ExprNullCoalescing)
MAKE_TYPE_FACTORY(ExprAt,ExprAt)
MAKE_TYPE_FACTORY(ExprSafeAt,ExprSafeAt)
MAKE_TYPE_FACTORY(ExprPtr2Ref,ExprPtr2Ref)
MAKE_TYPE_FACTORY(ExprIs,ExprIs)
MAKE_TYPE_FACTORY(ExprOp2,ExprOp2)
MAKE_TYPE_FACTORY(ExprOp3,ExprOp3)
MAKE_TYPE_FACTORY(ExprCopy,ExprCopy)
MAKE_TYPE_FACTORY(ExprMove,ExprMove)
MAKE_TYPE_FACTORY(ExprClone,ExprClone)
MAKE_TYPE_FACTORY(ExprWith,ExprWith)
MAKE_TYPE_FACTORY(ExprWhile,ExprWhile)
MAKE_TYPE_FACTORY(ExprTryCatch,ExprTryCatch)
MAKE_TYPE_FACTORY(ExprIfThenElse,ExprIfThenElse)
MAKE_TYPE_FACTORY(ExprFor,ExprFor)
MAKE_TYPE_FACTORY(ExprMakeLocal,ExprMakeLocal)
MAKE_TYPE_FACTORY(ExprMakeVariant,ExprMakeVariant)
MAKE_TYPE_FACTORY(ExprMakeStruct,ExprMakeStruct)
MAKE_TYPE_FACTORY(ExprMakeArray,ExprMakeArray)
MAKE_TYPE_FACTORY(ExprMakeTuple,ExprMakeTuple)
MAKE_TYPE_FACTORY(ExprArrayComprehension,ExprArrayComprehension)
MAKE_TYPE_FACTORY(TypeInfoMacro,TypeInfoMacro);
MAKE_TYPE_FACTORY(ExprTypeInfo,ExprTypeInfo)
MAKE_TYPE_FACTORY(ExprLabel,ExprLabel);
MAKE_TYPE_FACTORY(ExprGoto,ExprGoto);
MAKE_TYPE_FACTORY(ExprRef2Value,ExprRef2Value);
MAKE_TYPE_FACTORY(ExprRef2Ptr,ExprRef2Ptr);
MAKE_TYPE_FACTORY(ExprAddr,ExprAddr);
MAKE_TYPE_FACTORY(ExprAssert,ExprAssert);
MAKE_TYPE_FACTORY(ExprStaticAssert,ExprStaticAssert);
MAKE_TYPE_FACTORY(ExprDebug,ExprDebug);
MAKE_TYPE_FACTORY(ExprInvoke,ExprInvoke);
MAKE_TYPE_FACTORY(ExprErase,ExprErase);
MAKE_TYPE_FACTORY(ExprFind,ExprFind);
MAKE_TYPE_FACTORY(ExprKeyExists,ExprKeyExists);
MAKE_TYPE_FACTORY(ExprAscend,ExprAscend);
MAKE_TYPE_FACTORY(ExprCast,ExprCast);
MAKE_TYPE_FACTORY(ExprDelete,ExprDelete);
MAKE_TYPE_FACTORY(ExprVar,ExprVar);
MAKE_TYPE_FACTORY(ExprSwizzle,ExprSwizzle);
MAKE_TYPE_FACTORY(ExprField,ExprField);
MAKE_TYPE_FACTORY(ExprSafeField,ExprSafeField);
MAKE_TYPE_FACTORY(ExprIsVariant,ExprIsVariant);
MAKE_TYPE_FACTORY(ExprAsVariant,ExprAsVariant);
MAKE_TYPE_FACTORY(ExprSafeAsVariant,ExprSafeAsVariant);
MAKE_TYPE_FACTORY(ExprOp1,ExprOp1);
MAKE_TYPE_FACTORY(ExprReturn,ExprReturn);
MAKE_TYPE_FACTORY(ExprYield,ExprYield);
MAKE_TYPE_FACTORY(ExprBreak,ExprBreak);
MAKE_TYPE_FACTORY(ExprContinue,ExprContinue);
MAKE_TYPE_FACTORY(ExprConst,ExprConst);
MAKE_TYPE_FACTORY(ExprFakeContext,ExprFakeContext);
MAKE_TYPE_FACTORY(ExprFakeLineInfo,ExprFakeLineInfo);
MAKE_TYPE_FACTORY(ExprConstPtr,ExprConstPtr);
MAKE_TYPE_FACTORY(ExprConstEnumeration,ExprConstEnumeration);
MAKE_TYPE_FACTORY(ExprConstBitfield,ExprConstBitfield);
MAKE_TYPE_FACTORY(ExprConstInt8,ExprConstInt8);
MAKE_TYPE_FACTORY(ExprConstInt16,ExprConstInt16);
MAKE_TYPE_FACTORY(ExprConstInt64,ExprConstInt64);
MAKE_TYPE_FACTORY(ExprConstInt,ExprConstInt);
MAKE_TYPE_FACTORY(ExprConstInt2,ExprConstInt2);
MAKE_TYPE_FACTORY(ExprConstInt3,ExprConstInt3);
MAKE_TYPE_FACTORY(ExprConstInt4,ExprConstInt4);
MAKE_TYPE_FACTORY(ExprConstUInt8,ExprConstUInt8);
MAKE_TYPE_FACTORY(ExprConstUInt16,ExprConstUInt16);
MAKE_TYPE_FACTORY(ExprConstUInt64,ExprConstUInt64);
MAKE_TYPE_FACTORY(ExprConstUInt,ExprConstUInt);
MAKE_TYPE_FACTORY(ExprConstUInt2,ExprConstUInt2);
MAKE_TYPE_FACTORY(ExprConstUInt3,ExprConstUInt3);
MAKE_TYPE_FACTORY(ExprConstUInt4,ExprConstUInt4);
MAKE_TYPE_FACTORY(ExprConstRange,ExprConstRange);
MAKE_TYPE_FACTORY(ExprConstURange,ExprConstURange);
MAKE_TYPE_FACTORY(ExprConstBool,ExprConstBool);
MAKE_TYPE_FACTORY(ExprConstFloat,ExprConstFloat);
MAKE_TYPE_FACTORY(ExprConstFloat2,ExprConstFloat2);
MAKE_TYPE_FACTORY(ExprConstFloat3,ExprConstFloat3);
MAKE_TYPE_FACTORY(ExprConstFloat4,ExprConstFloat4);
MAKE_TYPE_FACTORY(ExprConstString,ExprConstString);
MAKE_TYPE_FACTORY(ExprConstDouble,ExprConstDouble);
MAKE_TYPE_FACTORY(ExprMakeBlock,ExprMakeBlock);
MAKE_TYPE_FACTORY(ExprMakeGenerator,ExprMakeGenerator);
MAKE_TYPE_FACTORY(ExprMemZero,ExprMemZero);
MAKE_TYPE_FACTORY(ExprReader,ExprReader);

DAS_BASE_BIND_ENUM(das::SideEffects, SideEffects,
    none, unsafe, userScenario, modifyExternal, accessExternal, modifyArgument,
    modifyArgumentAndExternal, worstDefault, accessGlobal, invoke, inferedSideEffects)


namespace das {
    TypeDeclPtr makeExprGenFlagsFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprGenFlags";
        ft->argNames = { "alwaysSafe", "generated" };
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

    template <typename EXPR>
    struct AstExprAnnotation : ManagedStructureAnnotation <EXPR> {
        AstExprAnnotation(const string & en, ModuleLibrary & ml)
            : ManagedStructureAnnotation<EXPR> (en, ml) {
        }
        void init() {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            this->template addField<DAS_BIND_MANAGED_FIELD(type)>("_type","type");
            this->template addField<DAS_BIND_MANAGED_FIELD(__rtti)>("__rtti");
            this->addFieldEx ( "genFlags", "genFlags", offsetof(Expression, genFlags), makeExprGenFlagsFlags() );
            this->addFieldEx ( "flags", "flags", offsetof(Expression, flags), makeExprFlagsFlags() );
            this->addFieldEx ( "printFlags", "printFlags", offsetof(Expression, printFlags), makeExprPrintFlagsFlags() );
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
            addField<DAS_BIND_MANAGED_FIELD(inScope)>("inScope");
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

    __forceinline void mks_vector_push ( MakeStruct & vec, MakeFieldDeclPtr value ) {
        vec.push_back(value);
    }
    void mks_vector_pop ( MakeStruct & vec ) {
        vec.pop_back();
    }
    void mks_vector_clear ( MakeStruct & vec ) {
        vec.clear();
    }
    void mks_vector_resize ( MakeStruct & vec, int32_t newSize ) {
        vec.resize(newSize);
    }

    struct AstMakeStructAnnotation : ManagedVectorAnnotation<MakeStruct> {
        AstMakeStructAnnotation(ModuleLibrary & ml)
            :  ManagedVectorAnnotation<MakeStruct> ("MakeStruct", ml) {
        };
        virtual bool isSmart() const { return true; }
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

    template <typename EXPR>
    struct AstExprLooksLikeCallAnnotation : AstExpressionAnnotation<EXPR> {
        AstExprLooksLikeCallAnnotation(const string & na, ModuleLibrary & ml)
            :  AstExpressionAnnotation<EXPR> (na, ml) {
            using ManagedType = EXPR;
            this->template addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            this->template addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            this->template addField<DAS_BIND_MANAGED_FIELD(argumentsFailedToInfer)>("argumentsFailedToInfer");
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
        ft->argNames = { "r2v", "r2cr", "write" };
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
            addField<DAS_BIND_MANAGED_FIELD(iteratorsAt)>("iteratorsAt");
            addField<DAS_BIND_MANAGED_FIELD(iteratorVariables)>("iteratorVariables");
            addField<DAS_BIND_MANAGED_FIELD(sources)>("sources");
            addField<DAS_BIND_MANAGED_FIELD(body)>("body");
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

    struct AstTypeInfoMacroAnnotation : ManagedStructureAnnotation<TypeInfoMacro,false> {
        AstTypeInfoMacroAnnotation(ModuleLibrary & ml)
            :  ManagedStructureAnnotation<TypeInfoMacro,false> ("TypeInfoMacro", ml) {
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
        ft->argNames = { "local", "argument", "block",
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
        ft->argNames = { "r2v", "r2cr", "write" };
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

    TypeDeclPtr makeExprMakeBlockFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "ExprMakeBlockFlags";
        ft->argNames = { "isLambda", "isLocalFunction" };
        return ft;
    }

    struct AstExprMakeBlockAnnotation : AstExpressionAnnotation<ExprMakeBlock> {
        AstExprMakeBlockAnnotation(ModuleLibrary & ml)
            :  AstExpressionAnnotation<ExprMakeBlock> ("ExprMakeBlock", ml) {
            addField<DAS_BIND_MANAGED_FIELD(block)>("block");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addFieldEx ( "mmFlags", "mmFlags", offsetof(ExprMakeBlock, mmFlags), makeExprMakeBlockFlags() );
        }
    };

    struct AstExprMakeGeneratorAnnotation : AstExprLooksLikeCallAnnotation<ExprMakeGenerator> {
        AstExprMakeGeneratorAnnotation(ModuleLibrary & ml)
            :  AstExprLooksLikeCallAnnotation<ExprMakeGenerator> ("ExprMakeGenerator", ml) {
            addField<DAS_BIND_MANAGED_FIELD(iterType)>("iterType");
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

     // TYPE STUFF

    struct AstModuleLibraryAnnotation : ManagedStructureAnnotation<ModuleLibrary,false> {
        AstModuleLibraryAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ModuleLibrary", ml) {
        }
    };

    struct AstModuleGroupAnnotation : ManagedStructureAnnotation<ModuleGroup,false> {
        AstModuleGroupAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ModuleGroup", ml) {
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
            addField<DAS_BIND_MANAGED_FIELD(module)>("enumModule");
            addField<DAS_BIND_MANAGED_FIELD(external)>("external");
            addField<DAS_BIND_MANAGED_FIELD(baseType)>("baseType");
        }
    };

    TypeDeclPtr makeTypeDeclFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "TypeDeclFlags";
        ft->argNames = { "ref", "constant", "temporary", "implicit",
            "removeRef", "removeConstant", "removeDim",
            "removeTemporary", "explicitConst", "aotAlias", "smartPtr" };
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
            addProperty<DAS_BIND_MANAGED_PROP(canCopy)>("canCopy","canCopy");
            addProperty<DAS_BIND_MANAGED_PROP(canMove)>("canMove","canMove");
            addProperty<DAS_BIND_MANAGED_PROP(isVoid)>("isVoid","isVoid");
            addProperty<DAS_BIND_MANAGED_PROP(isCtorType)>("isCtorType","isCtorType");
            addProperty<DAS_BIND_MANAGED_PROP(isExprType)>("isExprType","isExprType");
            addProperty<DAS_BIND_MANAGED_PROP(getSizeOf)>("sizeOf","getSizeOf");
            addProperty<DAS_BIND_MANAGED_PROP(getBaseSizeOf)>("baseSizeOf","getBaseSizeOf");
            addProperty<DAS_BIND_MANAGED_PROP(getCountOf)>("countOf","getCountOf");
            addProperty<DAS_BIND_MANAGED_PROP(getAlignOf)>("alignOf","getAlignOf");
        }
    };

    TypeDeclPtr makeFieldDeclarationFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FieldDeclarationFlags";
        ft->argNames = { "moveSemantics", "parentType", "capturedConstant", "generated" };
        return ft;
    }

    struct AstFieldDeclarationAnnotation : ManagedStructureAnnotation<Structure::FieldDeclaration> {
        AstFieldDeclarationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FieldDeclaration", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(type)>("_type", "type");
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
            "generated", "persistent", "isLambda" };
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
        ft->argNames = { "builtIn", "policyBased", "callBased", "interopFn",
            "hasReturn", "copyOnReturn", "moveOnReturn", "exports", "init",
            "addr", "used", "fastCall", "knownSideEffects", "hasToRunAtCompileTime",
            "unsafe", "unsafeOperation", "unsafeDeref", "hasMakeBlock", "aotNeedPrologue",
            "noAot", "aotHybrid", "aotTemplate", "generated", "privateFunction",
            "_generator", "_lambda", "firstArgReturnType", "isClassMethod"
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

    struct AstInferHistoryAnnotation : ManagedStructureAnnotation<Function::InferHistory> {
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
            // addField<DAS_BIND_MANAGED_FIELD(useFunctions)>("useGlobalVariables");
            // use global v
            addFieldEx ( "flags", "flags",
                offsetof(Function, flags), makeFunctionFlags() );
            addFieldEx ( "sideEffectFlags", "sideEffectFlags",
                offsetof(Function, sideEffectFlags), makeFunctionSideEffectFlags() );
            addField<DAS_BIND_MANAGED_FIELD(inferStack)>("inferStack");
            addField<DAS_BIND_MANAGED_FIELD(fromGeneric)>("fromGeneric");
            addField<DAS_BIND_MANAGED_FIELD(hash)>("hash");
            addField<DAS_BIND_MANAGED_FIELD(aotHash)>("aotHash");
        }
    };

    TypeDeclPtr makeVariableFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "VariableFlags";
        ft->argNames = { "init_via_move", "init_via_clone", "used", "aliasCMRES",
            "marked_used", "global_shared", "do_not_delete", "generated" };
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
            addField<DAS_BIND_MANAGED_FIELD(type)>("_type", "type");
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
        SimNode_AstGetTypeDecl ( const LineInfo & at, const TypeDeclPtr & d )
            : SimNode_CallBase(at) {
            typeExpr = d.get();
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(AstGetTypeDecl);
            V_ARG(typeExpr->getMangledName().c_str());
            V_END();
        }
        __forceinline char * compute(Context &) {
            DAS_PROFILE_NODE
            return (char *) typeExpr;
        }
        TypeDecl *  typeExpr;   // requires RTTI
    };

    struct SimNode_AstGetExpression : SimNode_CallBase {
        DAS_PTR_NODE;
        SimNode_AstGetExpression ( const LineInfo & at, const ExpressionPtr & e, char * d )
            : SimNode_CallBase(at) {
            expr = e.get();
            descr = d;
        }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override {
            auto that = (SimNode_AstGetExpression *) SimNode::copyNode(context, code);
            that->descr = code->allocateName(descr);
            return that;
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(AstGetExpression);
            V_ARG(descr);
            V_END();
        }
        __forceinline char * compute(Context &) {
            DAS_PROFILE_NODE
            return (char *) expr;
        }
        Expression *  expr;   // requires RTTI
        char *        descr;
    };

    struct SimNode_AstGetFunction : SimNode_CallBase {
        DAS_PTR_NODE;
        SimNode_AstGetFunction ( const LineInfo & at, Function * f )
            : SimNode_CallBase(at) {
            func = f;
        }
        virtual SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(AstGetTypeDecl);
            V_ARG(func->getMangledName().c_str());
            V_END();
        }
        __forceinline char * compute(Context &) {
            DAS_PROFILE_NODE
            return (char *) func;
        }
        Function *  func;   // requires RTTI
    };

    struct AstTypeDeclMacro : TypeInfoMacro {
        AstTypeDeclMacro() : TypeInfoMacro("ast_typedecl") {}
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr &, string & ) override {
            return typeFactory<smart_ptr<TypeDecl>>::make(lib);
        }
        virtual SimNode * simluate ( Context * context, const ExpressionPtr & expr, string & ) override {
            auto exprTypeInfo = static_pointer_cast<ExprTypeInfo>(expr);
            return context->code->makeNode<SimNode_AstGetTypeDecl>(expr->at, exprTypeInfo->typeexpr);
        }
        virtual bool noAot ( const ExpressionPtr & ) const override {
            return true;
        }
    };

    struct AstExpressionMacro : TypeInfoMacro {
        AstExpressionMacro() : TypeInfoMacro("ast_expression") {}
        virtual TypeDeclPtr getAstType ( ModuleLibrary & lib, const ExpressionPtr &, string & ) override {
            return typeFactory<smart_ptr<Expression>>::make(lib);
        }
        virtual SimNode * simluate ( Context * context, const ExpressionPtr & expr, string & errors ) override {
            auto exprTypeInfo = static_pointer_cast<ExprTypeInfo>(expr);
            if ( exprTypeInfo->subexpr ) {
                TextWriter ss;
                ss << *exprTypeInfo->subexpr;
                char * descr = context->code->allocateName(ss.str());
                return context->code->makeNode<SimNode_AstGetExpression>(expr->at, exprTypeInfo->subexpr, descr);
            } else {
                errors = "ast_expression requires expression, not just type";
                return nullptr;
            }
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
                return context->code->makeNode<SimNode_AstGetFunction>(expr->at, exprAddr->func);
            } else {
                errors = "ast_expression requires @@func expression";
                return nullptr;
            }
        }
        virtual bool noAot ( const ExpressionPtr & ) const override {
            return true;
        }
    };

    char * adapt_field ( const char * fName, char * pClass, const StructInfo * info ) {
        for ( uint32_t i=0; i!=info->count; ++i ) {
            if ( strcmp(info->fields[i]->name,fName)==0 ) {
                return pClass + info->fields[i]->offset;
            }
        }
        DAS_ASSERTF(0,"mapping %s not found. not fully implemented dervied class %s", fName, info->name);
        return 0;
    }

    Func adapt ( const char * funcName, char * pClass, const StructInfo * info ) {
        char * field = adapt_field(funcName, pClass, info);
        return field ? *(Func*)field : Func(0);
    }

#define FN_PREVISIT(WHAT)  fnPreVisit##WHAT
#define FN_VISIT(WHAT)      fnVisit##WHAT

#define IMPL_PREVISIT1(WHAT,WHATTYPE) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>> \
            (context,FN_PREVISIT(WHAT),classPtr,expr); \
    }

#define IMPL_PREVISIT2(WHAT,WHATTYPE,ARG1T,ARG1) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T> \
            (context,FN_PREVISIT(WHAT),classPtr,expr,ARG1); \
    }

#define IMPL_PREVISIT3(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T> \
            (context,FN_PREVISIT(WHAT),classPtr,expr,ARG1,ARG2); \
    }

#define IMPL_PREVISIT4(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_PREVISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,FN_PREVISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3); \
    }

#define IMPL_PREVISIT(WHAT) IMPL_PREVISIT1(WHAT,WHAT)

#define IMPL_VISIT_VOID1(WHAT,WHATTYPE) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>> \
            (context,FN_VISIT(WHAT),classPtr,expr); \
    }

#define IMPL_VISIT_VOID2(WHAT,WHATTYPE,ARG1T,ARG1) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1); \
    }

#define IMPL_VISIT_VOID3(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2); \
    }

#define IMPL_VISIT_VOID4(WHAT,WHATTYPE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_VISIT(WHAT) ) { \
        das_invoke_function<void>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3); \
    }

#define IMPL_VISIT1(WHAT,WHATTYPE,RETTYPE,RETVALUE) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr<WHATTYPE>> \
            (context,FN_VISIT(WHAT),classPtr,expr); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT2(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT3(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1,ARG2T,ARG2) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2); \
    } else { \
        return RETVALUE; \
    }

#define IMPL_VISIT4(WHAT,WHATTYPE,RETTYPE,RETVALUE,ARG1T,ARG1,ARG2T,ARG2,ARG3T,ARG3) \
    if ( FN_VISIT(WHAT) ) { \
        return das_invoke_function<smart_ptr_raw<RETTYPE>>::invoke<void *,smart_ptr<WHATTYPE>,ARG1T,ARG2T,ARG3T> \
            (context,FN_VISIT(WHAT),classPtr,expr,ARG1,ARG2,ARG3); \
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
        IMPL_ADAPT(Enumeration);
        IMPL_ADAPT(EnumerationValue);
        IMPL_ADAPT(Structure);
        IMPL_ADAPT(StructureField);
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
    }
// whole program
    void VisitorAdapter::preVisitProgram ( Program * expr )
        { IMPL_PREVISIT(Program); }
    void VisitorAdapter::visitProgram ( Program * expr )
        { IMPL_VISIT_VOID(Program); }
    void VisitorAdapter::preVisitProgramBody ( Program * expr )
        { IMPL_PREVISIT1(ProgramBody,Program); }
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
    void VisitorAdapter::preVisit ( Enumeration * expr )
        { IMPL_PREVISIT(Enumeration); }
    EnumerationPtr VisitorAdapter::visit ( Enumeration * expr )
        { IMPL_VISIT(Enumeration); }
    void VisitorAdapter::preVisitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last )
        { IMPL_PREVISIT4(EnumerationValue,Enumeration,const string &,name,ExpressionPtr,value,bool,last); }
    ExpressionPtr VisitorAdapter::visitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last )
        { IMPL_VISIT4(EnumerationValue,Enumeration,Expression,value,const string &,name,ExpressionPtr,value,bool,last); }
// structure
    void VisitorAdapter::preVisit ( Structure * expr )
        { IMPL_PREVISIT(Structure); }
    void VisitorAdapter::preVisitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last )
        { IMPL_PREVISIT3(StructureField,Structure,Structure::FieldDeclaration &,decl,bool,last); }
    void VisitorAdapter::visitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last )  {
        if ( FN_VISIT(StructureField) ) {
            das_invoke_function<void>::invoke<void *,StructurePtr,Structure::FieldDeclaration&,bool>
                (context,FN_VISIT(StructureField),classPtr,expr,decl,last);
        }
    }
    StructurePtr VisitorAdapter::visit ( Structure * expr )
        { IMPL_VISIT(Structure); }
// function
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
        { IMPL_VISIT3(FunctionArgument,Function,Expression,init,VariablePtr,var,ExpressionPtr,init); }
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
            das_invoke_function<void>::invoke<void *,smart_ptr<ExprBlock>>
                (context,FN_VISIT(ExprBlockFinal),classPtr,expr);
        }
    }
    void VisitorAdapter::preVisitBlockFinalExpression ( ExprBlock * expr, Expression * bexpr )
        { IMPL_PREVISIT2(ExprBlockFinalExpression,ExprBlock,ExpressionPtr,bexpr); }
    ExpressionPtr VisitorAdapter::visitBlockFinalExpression (  ExprBlock * expr, Expression * bexpr )
        { IMPL_VISIT2(ExprBlockFinalExpression,ExprBlock,Expression,expr,ExpressionPtr,bexpr); }
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

    struct AstVisitorAdapterAnnotation : ManagedStructureAnnotation<VisitorAdapter,false,true> {
        AstVisitorAdapterAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VisitorAdapter", ml) {
        }
    };

    class FunctionAnnotationAdapter : public FunctionAnnotation {
    public:
        FunctionAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
        : FunctionAnnotation(n), classPtr(pClass), context(ctx) {
            fnTransformCall = adapt("transform",pClass,info);
            fnApply = adapt("apply",pClass,info);
            fnFinish = adapt("finish",pClass,info);
        }
        virtual bool apply ( const FunctionPtr & func, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) {
            if ( fnApply ) {
                return das_invoke_function<bool>::invoke<void *,FunctionPtr,
                    ModuleGroup &,const AnnotationArgumentList &,string &>
                        (context,fnApply,classPtr,func,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool finalize ( const FunctionPtr & func, ModuleGroup & group,
                               const AnnotationArgumentList & args,
                               const AnnotationArgumentList & progArgs, string & errors ) {
            if ( fnFinish ) {
                return das_invoke_function<bool>::invoke<void *,FunctionPtr,
                    ModuleGroup &,const AnnotationArgumentList &,const AnnotationArgumentList &,string &>
                        (context,fnFinish,classPtr,func,group,args,progArgs,errors);
            } else {
                return true;
            }
        }
        virtual bool apply ( ExprBlock *, ModuleGroup &,
                            const AnnotationArgumentList &, string & err ) {
            err = "not a block annotation";
            return false;
        }
        virtual bool finalize ( ExprBlock *, ModuleGroup &,
                               const AnnotationArgumentList &,
                               const AnnotationArgumentList &, string & err ) {
            err = "not a block annotation";
            return false;
        }
        virtual ExpressionPtr transformCall ( ExprCallFunc * call, string & err ) {
            if ( fnTransformCall ) {
                auto res = das_invoke_function<ExpressionPtr>::invoke<void *,ExprCallFunc *,string &>
                    (context,fnTransformCall,classPtr,call,err);
                return res;
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnTransformCall;
        Func    fnApply;
        Func    fnFinish;
    };

    struct AstFunctionAnnotationAnnotation : ManagedStructureAnnotation<FunctionAnnotation,false,true> {
        AstFunctionAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FunctionAnnotation", ml) {
        }
    };

    struct StructureAnnotationAdapter : StructureAnnotation {
        StructureAnnotationAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : StructureAnnotation(n), classPtr(pClass), context(ctx) {
            fnApply = adapt("apply",pClass,info);
            fnFinish = adapt("finish",pClass,info);
        }
        virtual bool touch ( const StructurePtr & st, ModuleGroup & group,
                            const AnnotationArgumentList & args, string & errors ) {
            if ( fnApply ) {
                return das_invoke_function<bool>::invoke<void *,StructurePtr,
                    ModuleGroup &,const AnnotationArgumentList &,string &>
                        (context,fnApply,classPtr,st,group,args,errors);
            } else {
                return true;
            }
        }
        virtual bool look (const StructurePtr & st, ModuleGroup & group,
            const AnnotationArgumentList & args, string & errors ) {
            if ( fnFinish ) {
                return das_invoke_function<bool>::invoke<void *,StructurePtr,
                    ModuleGroup &,const AnnotationArgumentList &,string &>
                        (context,fnFinish,classPtr,st,group,args,errors);
            } else {
                return true;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnApply;
        Func    fnFinish;
    };

    struct AstStructureAnnotationAnnotation : ManagedStructureAnnotation<StructureAnnotation,false,true> {
        AstStructureAnnotationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("StructureAnnotation", ml) {
        }
    };
    struct PassMacroAdapter : PassMacro {
        PassMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : PassMacro(n), classPtr(pClass), context(ctx) {
            fnApply = adapt("apply",pClass,info);
        }
        virtual bool apply ( Program * prog, Module * mod ) override {
            if ( fnApply ) {
                return das_invoke_function<bool>::invoke<void *,ProgramPtr,Module *>
                        (context,fnApply,classPtr,prog,mod);
            } else {
                return false;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnApply;
    };

    struct AstPassMacroAnnotation : ManagedStructureAnnotation<PassMacro,false,true> {
        AstPassMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("PassMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct VariantMacroAdapter : VariantMacro {
        VariantMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : VariantMacro(n), classPtr(pClass), context(ctx) {
            fnVisitIs = adapt("visitExprIsVariant",pClass,info);
            fnVisitAs = adapt("visitExprAsVariant",pClass,info);
            fnVisitSafeAs = adapt("visitExprSafeAsVariant",pClass,info);
        }
        virtual ExpressionPtr visitIs ( Program * prog, Module * mod, ExprIsVariant * expr ) override {
            if ( fnVisitIs ) {
                return das_invoke_function<smart_ptr_raw<Expression>>::invoke<void *,ProgramPtr,Module *,smart_ptr<ExprIsVariant>>
                        (context,fnVisitIs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitAs ( Program * prog, Module * mod, ExprAsVariant * expr ) override {
            if ( fnVisitAs ) {
                return das_invoke_function<smart_ptr_raw<Expression>>::invoke<void *,ProgramPtr,Module *,smart_ptr<ExprAsVariant>>
                        (context,fnVisitAs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
        virtual ExpressionPtr visitSafeAs ( Program * prog, Module * mod, ExprSafeAsVariant * expr ) override {
            if ( fnVisitIs ) {
                return das_invoke_function<smart_ptr_raw<Expression>>::invoke<void *,ProgramPtr,Module *,smart_ptr<ExprSafeAsVariant>>
                        (context,fnVisitSafeAs,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func        fnVisitIs;
        Func        fnVisitAs;
        Func        fnVisitSafeAs;
    };

    struct AstVariantMacroAnnotation : ManagedStructureAnnotation<VariantMacro,false,true> {
        AstVariantMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VariantMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
        }
    };

    struct ReaderMacroAdapter : ReaderMacro {
        ReaderMacroAdapter ( const string & n, char * pClass, const StructInfo * info, Context * ctx )
            : ReaderMacro(n), classPtr(pClass), context(ctx) {
            fnAccept = adapt("accept",pClass,info);
            fnVisit = adapt("visit",pClass,info);
        }
        virtual bool accept ( Program * prog, Module * mod, ExprReader * expr, int Ch, const LineInfo & info ) override {
            if ( fnAccept ) {
                return das_invoke_function<bool>::invoke<void *,ProgramPtr,Module *,ExprReader *,int32_t,const LineInfo&>
                        (context,fnAccept,classPtr,prog,mod,expr,Ch,info);
            } else {
                return false;
            }
        }
        virtual ExpressionPtr visit (  Program * prog, Module * mod, ExprReader * expr ) override {
            if ( fnVisit ) {
                return das_invoke_function<smart_ptr_raw<Expression>>::invoke<void *,ProgramPtr,Module *,smart_ptr<ExprReader>>
                        (context,fnVisit,classPtr,prog,mod,expr);
            } else {
                return nullptr;
            }
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        Func    fnAccept;
        Func    fnVisit;
    };

    struct AstReaderMacroAnnotation : ManagedStructureAnnotation<ReaderMacro,false,true> {
        AstReaderMacroAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("ReaderMacro", ml) {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(module)>("_module", "module");
        }
    };

    #include "ast.das.inc"

    ReaderMacroPtr makeReaderMacro ( const char * name, const void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<ReaderMacroAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleReaderMacro ( Module * module, ReaderMacroPtr newM, Context * context ) {
        if ( !module->addReaderMacro(newM, true) ) {
            context->throw_error_ex("can't add reader macro %s to module %s", newM->name.c_str(), module->name.c_str());
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

    void addModuleVariantMacro ( Module * module, VariantMacroPtr newM, Context * ) {
        module->variantMacros.push_back(newM);
    }

    void addModuleInferDirtyMacro ( Module * module, PassMacroPtr newM, Context * ) {
        module->inferMacros.push_back(newM);
    }

    StructureAnnotationPtr makeStructureAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<StructureAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleStructureAnnotation ( Module * module, StructureAnnotationPtr ann, Context * context ) {
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_ex("can't add structure annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addStructureStructureAnnotation ( smart_ptr_raw<Structure> st, StructureAnnotationPtr ann, Context * context ) {
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
        vec4f args[1];
        if ( builtin_empty(name) ) {
            const auto & fnbn = module->functions;
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                for ( auto & nv : fnbn ) {
                    args[0] = cast<FunctionPtr>::from(nv.second);
                    code->eval(*context);
                }
            },lineInfo);
        } else {
            const auto & fnbn = module->functionsByName[name];
            context->invokeEx(block, args, nullptr, [&](SimNode * code){
                for ( auto & nv : fnbn ) {
                    args[0] = cast<FunctionPtr>::from(nv);
                    code->eval(*context);
                }
            },lineInfo);
        }
    }

    FunctionAnnotationPtr makeFunctionAnnotation ( const char * name, void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<FunctionAnnotationAdapter>(name,(char *)pClass,info,context);
    }

    void addModuleFunction ( Module * module, FunctionPtr func, Context * context ) {
        if ( !module->addFunction(func, true) ) {
            context->throw_error_ex("can't add function s to module %s",
                func->name.c_str(), module->name.c_str());
        }
    }

    void addModuleFunctionAnnotation ( Module * module, FunctionAnnotationPtr ann, Context * context ) {
        if ( !module->addAnnotation(ann, true) ) {
            context->throw_error_ex("can't add function annotation %s to module %s",
                ann->name.c_str(), module->name.c_str());
        }
    }

    void addFunctionFunctionAnnotation ( smart_ptr_raw<Function> func, FunctionAnnotationPtr ann, Context * context ) {
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

    Module * thisModule ( Context * context, LineInfoArg * lineinfo ) {
        if ( !context->thisProgram ) {
            context->throw_error_at(*lineinfo, "can't get this module past compilation");
        }
        return context->thisProgram->thisModule.get();
    }

    smart_ptr_raw<Program> thisProgram ( Context * context ) {
        return context->thisProgram;
    }

    extern ProgramPtr g_Program;

    Module * compileModule ( ) {
        return g_Program->thisModule.get();
    }

    smart_ptr_raw<Program> compileProgram ( ) {
        return g_Program;
    }

    void astVisit ( smart_ptr_raw<Program> program, smart_ptr_raw<VisitorAdapter> adapter ) {
        program->visit(*adapter);
    }

    char * ast_describe_typedecl ( smart_ptr_raw<TypeDecl> t, bool d_extra, bool d_contracts, bool d_module, Context * context ) {
        return context->stringHeap.allocateString(t->describe(
            d_extra ? TypeDecl::DescribeExtra::yes : TypeDecl::DescribeExtra::no,
            d_contracts ? TypeDecl::DescribeContracts::yes : TypeDecl::DescribeContracts::no,
            d_module ? TypeDecl::DescribeModule::yes : TypeDecl::DescribeModule::no));
    }

    char * ast_describe_expression ( smart_ptr_raw<Expression> t, Context * context ) {
        TextWriter ss;
        ss << *t;
        return context->stringHeap.allocateString(ss.str());
    }

    char * ast_describe_function ( smart_ptr_raw<Function> t, Context * context ) {
        TextWriter ss;
        ss << *t;
        return context->stringHeap.allocateString(ss.str());
    }

    char * ast_das_to_string ( Type bt, Context * context ) {
        return context->stringHeap.allocateString(das_to_string(bt));
    }

    char * ast_find_bitfield_name ( smart_ptr_raw<TypeDecl> bft, Bitfield value, Context * context ) {
        return context->stringHeap.allocateString(bft->findBitfieldName(value));
    }

    void ast_error ( ProgramPtr prog, const LineInfo & at, const char * message ) {
        prog->error(message ? message : "macro error","","",at,CompilationError::macro_failed);
    }

    int32_t get_variant_field_offset ( smart_ptr_raw<TypeDecl> td, int32_t index ) {
        return td->getVariantFieldOffset(index);
    }

    int32_t get_tuple_field_offset ( smart_ptr_raw<TypeDecl> td, int32_t index ) {
        return td->getTupleFieldOffset(index);
    }

    void any_table_foreach ( void * _tab, int keyStride, int valueStride, const TBlock<void,void *,void *> & blk, Context * context ) {
        auto tab = (Table *) _tab;
        if ( !tab->data ) return;
        char * values = tab->data;
        char * keys = tab->keys;
        for ( uint32_t index=0; index!=tab->capacity; index++, keys+=keyStride, values+=valueStride ) {
            if ( tab->hashes[index] > HASH_KILLED32 ) {
                das_invoke<void>::invoke<void *,void *>(context,blk,(void*)keys,(void*)values);
            }
        }
    }

    void any_array_foreach ( void * _arr, int stride, const TBlock<void,void *> & blk, Context * context ) {
        auto arr = (Array *) _arr;
        if ( !arr->data ) return;
        char * values = arr->data;
        for ( uint32_t index=0; index!=arr->size; index++, values+=stride ) {
            das_invoke<void>::invoke<void *>(context,blk,(void*)values);
        }
    }

    int32_t any_array_size ( void * _arr ) {
        return int32_t(((Array *) _arr)->size);
    }

    int32_t any_table_size ( void * _tab ) {
        return int32_t(((Table *) _tab)->size);
    }

    void for_each_typedef ( Module * mod, const TBlock<void,TTemporary<char *>,TypeDeclPtr> & block, Context * context ) {
        for ( auto & td : mod->aliasTypes ) {
            das_invoke<void>::invoke<const char *,TypeDeclPtr>(context,block,td.first.c_str(),td.second);
        }
    }

    void for_each_enumeration ( Module * mod, const TBlock<void,EnumerationPtr> & block, Context * context ) {
        for ( auto & td : mod->enumerations ) {
            das_invoke<void>::invoke<EnumerationPtr>(context,block,td.second);
        }
    }

    void for_each_structure ( Module * mod, const TBlock<void,StructurePtr> & block, Context * context ) {
        for ( auto & td : mod->structures ) {
            das_invoke<void>::invoke<StructurePtr>(context,block,td.second);
        }
    }

    void for_each_generic ( Module * mod, const TBlock<void,FunctionPtr> & block, Context * context ) {
        for ( auto & td : mod->generics ) {
            das_invoke<void>::invoke<FunctionPtr>(context,block,td.second);
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
        const TBlock<void,char *,char*,TypeDeclPtr,uint32_t> & block, Context * context ) {
        for ( auto & it : ann.fields ) {
            const auto & fld = it.second;
            das_invoke<void>::invoke<const char *,const char *,TypeDeclPtr,uint32_t>(context,block,
                it.first.c_str(), fld.cppName.c_str(),fld.decl,fld.offset);
        }
    }

    class Module_Ast : public Module {
    public:
        template <typename RecAnn>
        void addRecAnnotation ( ModuleLibrary & lib ) {
            auto rec = make_smart<RecAnn>(lib);
            addAnnotation(rec);
            initRecAnnotation(rec, lib);
        }
        Module_Ast() : Module("ast") {
            DAS_PROFILE_SECTION("Module_Ast");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            lib.addModule(Module::require("rtti"));
            // THE MAGNIFICENT TWO
            addTypeInfoMacro(make_smart<AstTypeDeclMacro>());
            addTypeInfoMacro(make_smart<AstExpressionMacro>());
            addTypeInfoMacro(make_smart<AstFunctionMacro>());
            // FLAGS?
            addAlias(makeTypeDeclFlags());
            addAlias(makeFieldDeclarationFlags());
            addAlias(makeStructureFlags());
            addAlias(makeExprGenFlagsFlags());
            addAlias(makeExprFlagsFlags());
            addAlias(makeExprPrintFlagsFlags());
            addAlias(makeFunctionFlags());
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
            // modules
            addAnnotation(make_smart<AstModuleLibraryAnnotation>(lib));
            addAnnotation(make_smart<AstModuleGroupAnnotation>(lib));
            // AST TYPES (due to a lot of xrefs we declare everyone as recursive type)
            auto exa = make_smart<AstExprAnnotation<Expression>>("Expression",lib);
            addAnnotation(exa);
            auto tda = make_smart<AstTypeDeclAnnnotation>(lib);
            addAnnotation(tda);
            auto sta = make_smart<AstStructureAnnotation>(lib);
            addAnnotation(sta);
            auto fta = make_smart<AstFieldDeclarationAnnotation>(lib);
            addAnnotation(fta);
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
            initRecAnnotation(ena, lib);
            initRecAnnotation(exa, lib);
            initRecAnnotation(fna, lib);
            initRecAnnotation(iha, lib);
            initRecAnnotation(vaa, lib);
            // reader macro
            addAnnotation(make_smart<AstReaderMacroAnnotation>(lib));
            // expressions
            addAnnotation(make_smart<AstExprBlockAnnotation>(lib));
            addAnnotation(make_smart<AstExprLetAnnotation>(lib));
            addAnnotation(make_smart<AstExprStringBuilderAnnotation>(lib));
            addAnnotation(make_smart<AstExprNewAnnotation>(lib));
            addAnnotation(make_smart<AstMakeFieldDeclAnnotation>(lib));
            addAnnotation(make_smart<AstMakeStructAnnotation>(lib));
            addAnnotation(make_smart<AstExprNamedCallAnnotation>(lib));
            addAnnotation(make_smart<AstExprLooksLikeCallAnnotation<ExprLooksLikeCall>>("ExprLooksLikeCall",lib));
            addAnnotation(make_smart<AstExprCallAnnotation>(lib));
            addAnnotation(make_smart<AstExprPtr2RefAnnotation<ExprPtr2Ref>>("ExprPtr2Ref",lib));
            addAnnotation(make_smart<AstExprNullCoalescingAnnotation>(lib));
            addAnnotation(make_smart<AstExprAtAnnotation<ExprAt>>("ExprAt",lib));
            addAnnotation(make_smart<AstExprAtAnnotation<ExprSafeAt>>("ExprSafeAt",lib));
            addAnnotation(make_smart<AstExprIsAnnotation>(lib));
            addAnnotation(make_smart<AstExprOp2Annotation<ExprOp2>>("ExprOp2",lib));
            addAnnotation(make_smart<AstExprOp3Annotation>(lib));
            addAnnotation(make_smart<AstExprCopyAnnotation>(lib));
            addAnnotation(make_smart<AstExprOp2Annotation<ExprMove>>("ExprMove",lib));
            addAnnotation(make_smart<AstExprOp2Annotation<ExprClone>>("ExprClone",lib));
            addAnnotation(make_smart<AstExprWithAnnotation>(lib));
            addAnnotation(make_smart<AstExprWhileAnnotation>(lib));
            addAnnotation(make_smart<AstExprTryCatchAnnotation>(lib));
            addAnnotation(make_smart<AstExprIfThenElseAnnotation>(lib));
            addAnnotation(make_smart<AstExprForAnnotation>(lib));
            addAnnotation(make_smart<AstExprMakeLocalAnnotation<ExprMakeLocal>>("ExprMakeLocal",lib));
            addAnnotation(make_smart<AstExprMakeStructAnnotation>(lib));
            addAnnotation(make_smart<AstExprMakeVariantAnnotation>(lib));
            addAnnotation(make_smart<AstExprMakeArrayAnnotation<ExprMakeArray>>("ExprMakeArray",lib));
            addAnnotation(make_smart<AstExprMakeTupleAnnotation>(lib));
            addAnnotation(make_smart<AstExprArrayComprehensionAnnotation>(lib));
            addAnnotation(make_smart<AstTypeInfoMacroAnnotation>(lib));
            addAnnotation(make_smart<AstExprTypeInfoAnnotation>(lib));
            // vector functions for custom containers
            addExtern<DAS_BIND_FUN(mks_vector_push)>(*this, lib, "push",
                SideEffects::modifyArgument, "mks_vector_push")->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_pop)>(*this, lib, "pop",
                SideEffects::modifyArgument, "mks_vector_pop")->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_clear)>(*this, lib, "clear",
                SideEffects::modifyArgument, "mks_vector_clear")->generated = true;
            addExtern<DAS_BIND_FUN(mks_vector_resize)>(*this, lib, "resize",
                SideEffects::modifyArgument, "mks_vector_resize")->generated = true;
            // expressions with no extra syntax
            addAnnotation(make_smart<AstExprLabelAnnotation>(lib));
            addAnnotation(make_smart<AstExprGotoAnnotation>(lib));
            addAnnotation(make_smart<AstExprRef2ValueAnnotation>(lib));
            addAnnotation(make_smart<AstExprRef2PtrAnnotation>(lib));
            addAnnotation(make_smart<AstExprAddrAnnotation>(lib));
            addAnnotation(make_smart<AstExprAssertAnnotation>(lib));
            addAnnotation(make_smart<AstExprStaticAssertAnnotation>(lib));
            addAnnotation(make_smart<AstExprDebugAnnotation>(lib));
            addAnnotation(make_smart<AstExprInvokeAnnotation>(lib));
            addAnnotation(make_smart<AstExprEraseAnnotation>(lib));
            addAnnotation(make_smart<AstExprFindAnnotation>(lib));
            addAnnotation(make_smart<AstExprKeyExistsAnnotation>(lib));
            addAnnotation(make_smart<AstExprAscendAnnotation>(lib));
            addAnnotation(make_smart<AstExprCastAnnotation>(lib));
            addAnnotation(make_smart<AstExprDeleteAnnotation>(lib));
            addAnnotation(make_smart<AstExprVarAnnotation>(lib));
            addAnnotation(make_smart<AstExprSwizzleAnnotation>(lib));
            addAnnotation(make_smart<AstExprFieldAnnotation<ExprField>>("ExprField",lib));
            addAnnotation(make_smart<AstExprSafeFieldAnnotation>(lib));
            addAnnotation(make_smart<AstExprFieldAnnotation<ExprIsVariant>>("ExprIsVariant",lib));
            addAnnotation(make_smart<AstExprFieldAnnotation<ExprAsVariant>>("ExprAsVariant",lib));
            addAnnotation(make_smart<AstExprSafeAsVariantAnnotation>(lib));
            addAnnotation(make_smart<AstExprOp1Annotation>(lib));
            addAnnotation(make_smart<AstExprReturnAnnotation>(lib));
            addAnnotation(make_smart<AstExprYieldAnnotation>(lib));
            addAnnotation(make_smart<AstExpressionAnnotation<ExprBreak>>("ExprBreak",lib));
            addAnnotation(make_smart<AstExpressionAnnotation<ExprContinue>>("ExprContinue",lib));
            addAnnotation(make_smart<AstExprConstAnnotation<ExprConst>>("ExprConst",lib));
            addAnnotation(make_smart<AstExprConstAnnotation<ExprFakeContext>>("ExprFakeContext",lib));
            addAnnotation(make_smart<AstExprConstAnnotation<ExprFakeLineInfo>>("ExprFakeLineInfo",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstPtr,void *>>("ExprConstPtr",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt8 ,int8_t>> ("ExprConstInt8",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt16,int16_t>>("ExprConstInt16",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt64,int64_t>>("ExprConstInt64",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt  ,int32_t>>("ExprConstInt",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt2 ,int2>>   ("ExprConstInt2",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt3 ,int3>>   ("ExprConstInt3",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstInt4 ,int4>>   ("ExprConstInt4",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt8 ,uint8_t>> ("ExprConstUInt8",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt16,uint16_t>>("ExprConstUInt16",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt64,uint64_t>>("ExprConstUInt64",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt  ,uint32_t>>("ExprConstUInt",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt2 ,uint2>>   ("ExprConstUInt2",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt3 ,uint3>>   ("ExprConstUInt3",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstUInt4 ,uint4>>   ("ExprConstUInt4",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstRange ,range>> ("ExprConstRange",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstURange,urange>>("ExprConstURange",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat  ,float>> ("ExprConstFloat",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat2 ,float2>>("ExprConstFloat2",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat3 ,float3>>("ExprConstFloat3",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstFloat4 ,float4>>("ExprConstFloat4",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstDouble,double>> ("ExprConstDouble",lib));
            addAnnotation(make_smart<AstExprConstTAnnotation<ExprConstBool,bool>> ("ExprConstBool",lib));
            addAnnotation(make_smart<AstExprMakeBlockAnnotation>(lib));
            addAnnotation(make_smart<AstExprMakeGeneratorAnnotation>(lib));
            addAnnotation(make_smart<AstExprLikeCallAnnotation<ExprMemZero>>("ExprMemZero",lib));
            addAnnotation(make_smart<AstExprConstEnumerationAnnotation>(lib));
            addAnnotation(make_smart<AstExprConstBitfieldAnnotation>(lib));
            addAnnotation(make_smart<AstExprConstStringAnnotation>(lib));
            addAnnotation(make_smart<AstExprReaderAnnotation>(lib));
            // visitor
            addAnnotation(make_smart<AstVisitorAdapterAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeVisitor)>(*this, lib,  "make_visitor",
                SideEffects::modifyExternal, "makeVisitor");
            addExtern<DAS_BIND_FUN(thisProgram)>(*this, lib,  "this_program",
                SideEffects::accessExternal, "thisProgram");
            addExtern<DAS_BIND_FUN(thisModule)>(*this, lib,  "this_module",
                SideEffects::accessExternal, "thisModule");
            addExtern<DAS_BIND_FUN(compileProgram)>(*this, lib,  "compiling_program",
                SideEffects::accessExternal, "compileProgram");
            addExtern<DAS_BIND_FUN(compileModule)>(*this, lib,  "compiling_module",
                SideEffects::accessExternal, "compileModule");
            addExtern<DAS_BIND_FUN(forEachFunction)>(*this, lib,  "for_each_function",
                SideEffects::accessExternal, "forEachFunction");
            addExtern<DAS_BIND_FUN(astVisit)>(*this, lib,  "visit",
                SideEffects::accessExternal, "astVisit");
            // function annotation
            addAnnotation(make_smart<AstFunctionAnnotationAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeFunctionAnnotation)>(*this, lib,  "make_function_annotation",
                SideEffects::modifyExternal, "makeFunctionAnnotation");
            addExtern<DAS_BIND_FUN(addModuleFunctionAnnotation)>(*this, lib,  "add_function_annotation",
                SideEffects::modifyExternal, "addModuleFunctionAnnotation");
            addExtern<DAS_BIND_FUN(addModuleFunction)>(*this, lib,  "add_function",
                SideEffects::modifyExternal, "addModuleFunction");
            addExtern<DAS_BIND_FUN(addFunctionFunctionAnnotation)>(*this, lib,  "add_function_annotation",
                SideEffects::modifyExternal, "addFunctionFunctionAnnotation");
            // structure annotation
            addAnnotation(make_smart<AstStructureAnnotationAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeStructureAnnotation)>(*this, lib,  "make_structure_annotation",
                SideEffects::modifyExternal, "makeStructureAnnotation");
            addExtern<DAS_BIND_FUN(addModuleStructureAnnotation)>(*this, lib,  "add_structure_annotation",
                SideEffects::modifyExternal, "addModuleStructureAnnotation");
            addExtern<DAS_BIND_FUN(addStructureStructureAnnotation)>(*this, lib,  "add_structure_annotation",
                SideEffects::modifyExternal, "addStructureStructureAnnotation");
            // pass macro
            addAnnotation(make_smart<AstPassMacroAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makePassMacro)>(*this, lib,  "make_pass_macro",
                SideEffects::modifyExternal, "makePassMacro");
            addExtern<DAS_BIND_FUN(addModuleInferDirtyMacro)>(*this, lib,  "add_dirty_infer_macro",
                SideEffects::modifyExternal, "addModuleInferDirtyMacro");
            // reader macro
            addExtern<DAS_BIND_FUN(makeReaderMacro)>(*this, lib,  "make_reader_macro",
                SideEffects::modifyExternal, "makeReaderMacro");
            addExtern<DAS_BIND_FUN(addModuleReaderMacro)>(*this, lib,  "add_reader_macro",
                SideEffects::modifyExternal, "addModuleReaderMacro");
            // variant macro
            addAnnotation(make_smart<AstVariantMacroAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeVariantMacro)>(*this, lib,  "make_variant_macro",
                SideEffects::modifyExternal, "makeVariantMacro");
            addExtern<DAS_BIND_FUN(addModuleVariantMacro)>(*this, lib,  "add_variant_macro",
                SideEffects::modifyExternal, "addModuleVariantMacro");
            // helper functions
            addExtern<DAS_BIND_FUN(ast_describe_typedecl)>(*this, lib,  "describe_typedecl",
                SideEffects::none, "describe_typedecl");
            addExtern<DAS_BIND_FUN(ast_describe_expression)>(*this, lib,  "describe_expression",
                SideEffects::none, "describe_expression");
            addExtern<DAS_BIND_FUN(ast_describe_function)>(*this, lib,  "describe_function",
                SideEffects::none, "describe_function");
            addExtern<DAS_BIND_FUN(ast_find_bitfield_name)>(*this, lib,  "find_bitfield_name",
                SideEffects::none, "find_bitfield_name");
            // type conversion functions
            addExtern<DAS_BIND_FUN(ast_das_to_string)>(*this, lib,  "das_to_string",
                SideEffects::none, "das_to_string");
            // expression
            addExtern<DAS_BIND_FUN(clone_expression)>(*this, lib,  "clone_expression",
                SideEffects::none, "clone_expression");
            // type
            addExtern<DAS_BIND_FUN(isSameAstType)>(*this, lib,  "is_same_type",
                SideEffects::none, "isSameAstType");
            addExtern<DAS_BIND_FUN(clone_type)>(*this, lib,  "clone_type",
                SideEffects::none, "clone_type");
            addExtern<DAS_BIND_FUN(get_variant_field_offset)>(*this, lib,  "get_variant_field_offset",
                SideEffects::none, "get_variant_field_offset");
            addExtern<DAS_BIND_FUN(get_tuple_field_offset)>(*this, lib,  "get_tuple_field_offset",
                SideEffects::none, "get_tuple_field_offset");
            addExtern<DAS_BIND_FUN(any_table_foreach)>(*this, lib,  "any_table_foreach",
                SideEffects::modifyArgumentAndExternal, "any_table_foreach");
            addExtern<DAS_BIND_FUN(any_array_foreach)>(*this, lib,  "any_array_foreach",
                SideEffects::modifyArgumentAndExternal, "any_array_foreach");
            addExtern<DAS_BIND_FUN(any_array_size)>(*this, lib,  "any_array_size",
                SideEffects::none, "any_array_size");
            addExtern<DAS_BIND_FUN(any_table_size)>(*this, lib,  "any_table_size",
                SideEffects::none, "any_table_size");
            // module
            addExtern<DAS_BIND_FUN(for_each_typedef)>(*this, lib,  "for_each_typedef",
                SideEffects::modifyExternal, "for_each_typedef");
            addExtern<DAS_BIND_FUN(for_each_enumeration)>(*this, lib,  "for_each_enumeration",
                SideEffects::modifyExternal, "for_each_enumeration");
            addExtern<DAS_BIND_FUN(for_each_structure)>(*this, lib,  "for_each_structure",
                SideEffects::modifyExternal, "for_each_structure");
            addExtern<DAS_BIND_FUN(for_each_generic)>(*this, lib,  "for_each_generic",
                SideEffects::modifyExternal, "for_each_generic");
            addExtern<DAS_BIND_FUN(builtin_structure_for_each_field)>(*this, lib,  "for_each_field",
                SideEffects::modifyExternal, "builtin_structure_for_each_field");
            // errors
            addExtern<DAS_BIND_FUN(ast_error)>(*this, lib,  "macro_error",
                SideEffects::modifyArgumentAndExternal, "ast_error");
            // add builtin module
            compileBuiltinModule("ast.das",ast_das,sizeof(ast_das));
            // lets make sure its all aot ready
            // verifyAotReady();
        }
        virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_builtin_ast.h\"\n";
            return ModuleAotType::hybrid;
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_Ast,das);
