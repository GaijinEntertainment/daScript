#include "daScript/misc/platform.h"

#include "module_builtin_rtti.h"

#include "daScript/simulate/simulate_visit_op.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_expressions.h"
#include "daScript/simulate/aot_builtin_ast.h"

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

MAKE_TYPE_FACTORY(ExprBlock,ExprBlock)
MAKE_TYPE_FACTORY(ExprLet,ExprLet)
MAKE_TYPE_FACTORY(ExprStringBuilder,ExprStringBuilder)

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
			this->template addField<DAS_BIND_MANAGED_FIELD(type)>("typeDecl");
			this->addFieldEx ( "genFlags", "genFlags", offsetof(Expression, genFlags), makeExprGenFlagsFlags() );
			this->addFieldEx ( "flags", "flags", offsetof(Expression, flags), makeExprFlagsFlags() );
			this->addFieldEx ( "printFlags", "printFlags", offsetof(Expression, printFlags), makeExprPrintFlagsFlags() );
        }
    };

    struct AstExpressionAnnotation : AstExprAnnotation<Expression> {
        AstExpressionAnnotation(ModuleLibrary & ml)
            :  AstExprAnnotation<Expression> ("Expression", ml) {
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

    struct AstExprBlockAnnotation : AstExprAnnotation<ExprBlock> {
        AstExprBlockAnnotation(ModuleLibrary & ml)
            :  AstExprAnnotation<ExprBlock> ("ExprBlock", ml) {
            addField<DAS_BIND_MANAGED_FIELD(list)>("list");
            addField<DAS_BIND_MANAGED_FIELD(finalList)>("finalList");
            addField<DAS_BIND_MANAGED_FIELD(returnType)>("returnType");
            addField<DAS_BIND_MANAGED_FIELD(arguments)>("arguments");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(maxLabelIndex)>("maxLabelIndex");
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addField<DAS_BIND_MANAGED_FIELD(annotationData)>("annotationData");
            addField<DAS_BIND_MANAGED_FIELD(annotationDataSid)>("annotationDataSid");
            // properties
            addFieldEx ( "blockFlags", "blockFlags", offsetof(ExprBlock, blockFlags), makeExprBlockFlags() );
        }
    };

    struct AstExprLetAnnotation : AstExprAnnotation<ExprLet> {
        AstExprLetAnnotation(ModuleLibrary & ml)
            :  AstExprAnnotation<ExprLet> ("ExprLet", ml) {
            addField<DAS_BIND_MANAGED_FIELD(variables)>("variables");
            addField<DAS_BIND_MANAGED_FIELD(inScope)>("inScope");
        }
    };

    struct AstExprStringBuilderAnnotation : AstExprAnnotation<ExprStringBuilder> {
        AstExprStringBuilderAnnotation(ModuleLibrary & ml)
            :  AstExprAnnotation<ExprStringBuilder> ("ExprStringBuilder", ml) {
            addField<DAS_BIND_MANAGED_FIELD(elements)>("elements");
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
            addField<DAS_BIND_MANAGED_FIELD(module)>("module");
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
            addField<DAS_BIND_MANAGED_FIELD(module)>("module");
            // properties
            addProperty<DAS_BIND_MANAGED_PROP(isVoid)>("isVoid","isVoid");
            addProperty<DAS_BIND_MANAGED_PROP(isExprType)>("isExprType","isExprType");
        }
    };

    TypeDeclPtr makeFieldDeclarationFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FieldDeclarationFlags";
        ft->argNames = { "moveSemantic", "parentType", "capturedConstant", "generated" };
        return ft;
    }

    struct AstFieldDeclarationAnnotation : ManagedStructureAnnotation<Structure::FieldDeclaration> {
        AstFieldDeclarationAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("FieldDeclaration", ml) {
        }
        void init () {
            addField<DAS_BIND_MANAGED_FIELD(name)>("name");
            addField<DAS_BIND_MANAGED_FIELD(type)>("typeDecl");
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
        ft->argNames = { "isClass", "genCtor", "cppLayout", "cppLayoutNotPod", "generated" };
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
            addField<DAS_BIND_MANAGED_FIELD(module)>("module");
            addField<DAS_BIND_MANAGED_FIELD(parent)>("parent");
            addField<DAS_BIND_MANAGED_FIELD(annotations)>("annotations");
            addFieldEx ( "flags", "flags", offsetof(Structure, flags), makeStructureFlags() );
        }
    };

    TypeDeclPtr makeFunctionFlags() {
        auto ft = make_smart<TypeDecl>(Type::tBitfield);
        ft->alias = "FunctionFlags";
        ft->argNames = { "builtIn", "policyBased", "callBased", "interopFn",
            "hasRetur", "copyOnReturn", "moveOnReturn", "exports", "init",
            "addr", "used", "fastCall", "knownSideEffects", "hasToRunAtCompileTime",
            "unsafe", "unsafeOperation", "unsafeDeref", "hasMakeBlock", "aotNeedPrologue",
            "noAot", "aotHybrid", "aotTemplate", "generated", "privateFunction",
            "generatorFunction", "lambda"
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
            addField<DAS_BIND_MANAGED_FIELD(module)>("module");
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
            addField<DAS_BIND_MANAGED_FIELD(type)>("typeDecl");
            addField<DAS_BIND_MANAGED_FIELD(init)>("init");
            addField<DAS_BIND_MANAGED_FIELD(source)>("source");
            addField<DAS_BIND_MANAGED_FIELD(at)>("at");
            addField<DAS_BIND_MANAGED_FIELD(index)>("index");
            addField<DAS_BIND_MANAGED_FIELD(stackTop)>("stackTop");
            addField<DAS_BIND_MANAGED_FIELD(module)>("module");
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

    // TODO:
    //  optimize multiple-adapt-calls
    Func adapt ( const char * funcName, char * pClass, const StructInfo * info ) {
        for ( uint32_t i=0; i!=info->count; ++i ) {
            if ( strcmp(info->fields[i]->name,funcName)==0 ) {
                return *(Func *)(pClass + info->fields[i]->offset);
            }
        }
        DAS_ASSERTF(0,"mapping %s not found. not fully implemented dervied class %s", funcName, info->name);
        return 0;
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

#define DECL_VISIT(WHAT) \
    Func        FN_PREVISIT(WHAT); \
    Func        FN_VISIT(WHAT);

#define IMPL_ADAPT(WHAT) \
    FN_PREVISIT(WHAT) = adapt("preVisit" #WHAT,pClass,info); \
    FN_VISIT(WHAT) = adapt("visit" #WHAT,pClass,info);

    class VisitorAdapter : public Visitor {
    public:
        VisitorAdapter ( char * pClass, const StructInfo * info, Context * ctx ) {
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
        }
    protected:
        void *      classPtr;
        Context *   context;
    protected:
        DECL_VISIT(Program);
        Func FN_PREVISIT(ProgramBody);
        DECL_VISIT(TypeDecl);
        DECL_VISIT(Expression);
        DECL_VISIT(Alias);
        DECL_VISIT(Enumeration);
        DECL_VISIT(EnumerationValue);
        DECL_VISIT(Structure);
        DECL_VISIT(StructureField);
        DECL_VISIT(Function);
        DECL_VISIT(FunctionArgument);
        DECL_VISIT(FunctionArgumentInit);
        DECL_VISIT(FunctionBody);
        DECL_VISIT(ExprBlock);
        DECL_VISIT(ExprBlockArgument);
        DECL_VISIT(ExprBlockArgumentInit);
        DECL_VISIT(ExprBlockExpression);
        DECL_VISIT(ExprBlockFinal);
        DECL_VISIT(ExprBlockFinalExpression);
        DECL_VISIT(ExprLet);
        DECL_VISIT(ExprLetVariable);
        DECL_VISIT(ExprLetVariableInit);
        DECL_VISIT(GlobalLet);
        DECL_VISIT(GlobalLetVariable);
        DECL_VISIT(GlobalLetVariableInit);
        DECL_VISIT(ExprStringBuilder);
        DECL_VISIT(ExprStringBuilderElement);
    protected:
    // whole program
        virtual void preVisitProgram ( Program * expr ) override
            { IMPL_PREVISIT(Program); }
        virtual void visitProgram ( Program * expr ) override
            { IMPL_VISIT_VOID(Program); }
        virtual void preVisitProgramBody ( Program * expr ) override
            { IMPL_PREVISIT1(ProgramBody,Program); }
    // type
        virtual void preVisit ( TypeDecl * expr ) override
            { IMPL_PREVISIT(TypeDecl); }
        virtual TypeDeclPtr visit ( TypeDecl * expr ) override
            { IMPL_VISIT(TypeDecl); }
    // alias
        virtual void preVisitAlias ( TypeDecl * expr, const string & name ) override
            { IMPL_PREVISIT2(Alias,TypeDecl,const string &,name); }
        virtual TypeDeclPtr visitAlias ( TypeDecl * expr, const string & name ) override
            { IMPL_VISIT2(Alias,TypeDecl,TypeDecl,expr,const string &,name); }
    // enumeration
        virtual void preVisit ( Enumeration * expr ) override
            { IMPL_PREVISIT(Enumeration); }
        virtual EnumerationPtr visit ( Enumeration * expr ) override
            { IMPL_VISIT(Enumeration); }
        virtual void preVisitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last ) override
            { IMPL_PREVISIT4(EnumerationValue,Enumeration,const string &,name,ExpressionPtr,value,bool,last); }
        virtual ExpressionPtr visitEnumerationValue ( Enumeration * expr, const string & name, Expression * value, bool last ) override
            { IMPL_VISIT4(EnumerationValue,Enumeration,Expression,value,const string &,name,ExpressionPtr,value,bool,last); }
    // structure
        virtual void preVisit ( Structure * expr ) override
            { IMPL_PREVISIT(Structure); }
        virtual void preVisitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last ) override
            { IMPL_PREVISIT3(StructureField,Structure,Structure::FieldDeclaration &,decl,bool,last); }
        virtual void visitStructureField ( Structure * expr, Structure::FieldDeclaration & decl, bool last ) override {
            if ( FN_VISIT(StructureField) ) {
                das_invoke_function<void>::invoke<void *,StructurePtr,Structure::FieldDeclaration&,bool>
                    (context,FN_VISIT(StructureField),classPtr,expr,decl,last);
            }
        }
        virtual StructurePtr visit ( Structure * expr ) override
            { IMPL_VISIT(Structure); }
    // function
        virtual void preVisit ( Function * expr ) override
            { IMPL_PREVISIT(Function); }
        virtual FunctionPtr visit ( Function * expr ) override
            { IMPL_VISIT(Function); }
        virtual void preVisitArgument ( Function * expr, const VariablePtr & var, bool lastArg ) override
            { IMPL_PREVISIT3(FunctionArgument,Function,VariablePtr,var,bool,lastArg); }
        virtual VariablePtr visitArgument ( Function * expr, const VariablePtr & var, bool lastArg ) override
            { IMPL_VISIT3(FunctionArgument,Function,Variable,var,VariablePtr,var,bool,lastArg); }
        virtual void preVisitArgumentInit ( Function * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_PREVISIT3(FunctionArgument,Function,VariablePtr,var,ExpressionPtr,init); }
        virtual ExpressionPtr visitArgumentInit ( Function * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_VISIT3(FunctionArgument,Function,Expression,init,VariablePtr,var,ExpressionPtr,init); }
        virtual void preVisitFunctionBody ( Function * expr, Expression * that ) override
            { IMPL_PREVISIT2(FunctionBody,Function,ExpressionPtr,that); }
        virtual ExpressionPtr visitFunctionBody ( Function * expr, Expression * that ) override
            { IMPL_VISIT2(FunctionBody,Function,Expression,that,ExpressionPtr,that); }
    // expression
        virtual void preVisitExpression ( Expression * expr ) override
            { IMPL_PREVISIT(Expression); }
        virtual ExpressionPtr visitExpression ( Expression * expr ) override
            { IMPL_VISIT(Expression); }
    // block
        virtual void preVisit ( ExprBlock * expr ) override
            { IMPL_PREVISIT(ExprBlock); }
        virtual ExpressionPtr visit ( ExprBlock * expr ) override
            { IMPL_VISIT(ExprBlock); }
        virtual void preVisitBlockArgument ( ExprBlock * expr, const VariablePtr & var, bool lastArg ) override
            { IMPL_PREVISIT3(ExprBlockArgument,ExprBlock,VariablePtr,var,bool,lastArg); }
        virtual VariablePtr visitBlockArgument ( ExprBlock * expr, const VariablePtr & var, bool lastArg ) override
            { IMPL_VISIT3(ExprBlockArgument,ExprBlock,Variable,var,VariablePtr,var,bool,lastArg); }
        virtual void preVisitBlockArgumentInit ( ExprBlock * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_PREVISIT3(ExprBlockArgumentInit,ExprBlock,VariablePtr,var,ExpressionPtr,init); }
        virtual ExpressionPtr visitBlockArgumentInit ( ExprBlock * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_VISIT3(ExprBlockArgumentInit,ExprBlock,Expression,init,VariablePtr,var,ExpressionPtr,init); }
        virtual void preVisitBlockExpression ( ExprBlock * expr, Expression * bexpr ) override
            { IMPL_PREVISIT2(ExprBlockExpression,ExprBlock,ExpressionPtr,bexpr); }
        virtual ExpressionPtr visitBlockExpression (  ExprBlock * expr, Expression * bexpr ) override
            { IMPL_VISIT2(ExprBlockExpression,ExprBlock,Expression,bexpr,ExpressionPtr,bexpr); }
        virtual void preVisitBlockFinal ( ExprBlock * expr ) override
            { IMPL_PREVISIT1(ExprBlockFinal,ExprBlock); }
        virtual void visitBlockFinal ( ExprBlock * expr ) override {
            if ( FN_VISIT(ExprBlockFinal) ) {
                das_invoke_function<void>::invoke<void *,smart_ptr<ExprBlock>>
                    (context,FN_VISIT(ExprBlockFinal),classPtr,expr);
            }
        }
        virtual void preVisitBlockFinalExpression ( ExprBlock * expr, Expression * bexpr ) override
            { IMPL_PREVISIT2(ExprBlockFinalExpression,ExprBlock,ExpressionPtr,bexpr); }
        virtual ExpressionPtr visitBlockFinalExpression (  ExprBlock * expr, Expression * bexpr ) override
            { IMPL_VISIT2(ExprBlockFinalExpression,ExprBlock,Expression,expr,ExpressionPtr,bexpr); }
    // let
        virtual void preVisit ( ExprLet * expr ) override
            { IMPL_PREVISIT(ExprLet); }
        virtual ExpressionPtr visit ( ExprLet * expr ) override
            { IMPL_VISIT(ExprLet); }
        virtual void preVisitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override
            { IMPL_PREVISIT3(ExprLetVariable,ExprLet,VariablePtr,var,bool,last); }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override
            { IMPL_VISIT3(ExprLetVariable,ExprLet,Variable,var,VariablePtr,var,bool,last); }
        virtual void preVisitLetInit ( ExprLet * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_PREVISIT3(ExprLetVariableInit,ExprLet,VariablePtr,var,ExpressionPtr,init); }
        virtual ExpressionPtr visitLetInit ( ExprLet * expr, const VariablePtr & var, Expression * init ) override
            { IMPL_VISIT3(ExprLetVariableInit,ExprLet,Expression,init,VariablePtr,var,ExpressionPtr,init); }
    // global let
        virtual void preVisitGlobalLetBody ( Program * expr ) override
            { IMPL_PREVISIT1(GlobalLet,Program); }
        virtual void visitGlobalLetBody ( Program * expr ) override
            { IMPL_VISIT_VOID1(GlobalLet,Program); }
        virtual void preVisitGlobalLet ( const VariablePtr & expr ) override
            { IMPL_PREVISIT1(GlobalLetVariable,Variable); }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & expr ) override
            { IMPL_VISIT1(GlobalLetVariable,Variable,Variable,expr); }
        virtual void preVisitGlobalLetInit ( const VariablePtr & expr, Expression * init ) override
            { IMPL_PREVISIT2(GlobalLetVariableInit,Variable,ExpressionPtr,init); }
        virtual ExpressionPtr visitGlobalLetInit ( const VariablePtr & expr, Expression * init ) override
            { IMPL_VISIT2(GlobalLetVariableInit,Variable,Expression,init,ExpressionPtr,init); }
    // string builder
        virtual void preVisit ( ExprStringBuilder * expr ) override
            { IMPL_PREVISIT(ExprStringBuilder); }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) override
            { IMPL_VISIT(ExprStringBuilder); }
        virtual void preVisitStringBuilderElement ( ExprStringBuilder * expr, Expression * element, bool last ) override
            { IMPL_PREVISIT3(ExprStringBuilderElement,ExprStringBuilder,ExpressionPtr,element,bool,last); }
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * expr, Expression * element, bool last ) override
            { IMPL_VISIT3(ExprStringBuilderElement,ExprStringBuilder,Expression,element,ExpressionPtr,element,bool,last); }
    };

    struct AstVisitorAdapterAnnotation : ManagedStructureAnnotation<VisitorAdapter,false> {
        AstVisitorAdapterAnnotation(ModuleLibrary & ml)
            : ManagedStructureAnnotation ("VisitorAdapter", ml) {
        }
    };

    #include "ast.das.inc"

    smart_ptr<VisitorAdapter> makeVisitor ( void * pClass, const StructInfo * info, Context * context ) {
        return make_smart<VisitorAdapter>((char *)pClass,info,context);
    }

    smart_ptr_raw<Program> thisProgram ( Context * context ) {
        return context->thisProgram;
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

    class Module_Ast : public Module {
    public:
        template <typename RecAnn>
        void addRecAnnotation ( ModuleLibrary & lib ) {
            auto rec = make_smart<RecAnn>(lib);
            addAnnotation(rec);
            initRecAnnotation(rec, lib);
        }
        Module_Ast() : Module("ast") {
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
            // ENUMS
            addEnumeration(make_smart<EnumerationSideEffects>());
            // AST TYPES (due to a lot of xrefs we declare everyone as recursive type)
            auto exa = make_smart<AstExpressionAnnotation>(lib);
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
            // expressions
            addAnnotation(make_smart<AstExprBlockAnnotation>(lib));
            addAnnotation(make_smart<AstExprLetAnnotation>(lib));
            addAnnotation(make_smart<AstExprStringBuilderAnnotation>(lib));
            // visitor
            addAnnotation(make_smart<AstVisitorAdapterAnnotation>(lib));
            addExtern<DAS_BIND_FUN(makeVisitor)>(*this, lib,  "make_visitor",
                SideEffects::modifyExternal, "makeVisitor");
            addExtern<DAS_BIND_FUN(thisProgram)>(*this, lib,  "ast_this_program",
                SideEffects::accessExternal, "thisProgram");
            addExtern<DAS_BIND_FUN(astVisit)>(*this, lib,  "ast_visit",
                SideEffects::accessExternal, "astVisit");
            // helper functions
            addExtern<DAS_BIND_FUN(ast_describe_typedecl)>(*this, lib,  "ast_describe_typedecl",
                SideEffects::none, "ast_describe_typedecl");
            addExtern<DAS_BIND_FUN(ast_describe_expression)>(*this, lib,  "ast_describe_expression",
                SideEffects::none, "ast_describe_expression");
            addExtern<DAS_BIND_FUN(ast_describe_function)>(*this, lib,  "ast_describe_function",
                SideEffects::none, "ast_describe_function");
            // type conversion functions
            addExtern<DAS_BIND_FUN(ast_das_to_string)>(*this, lib,  "ast_das_to_string",
                SideEffects::none, "ast_das_to_string");
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
