#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/interop.h"
#include "daScript/simulate/aot.h"

namespace das
{
    class ExternalFnBase : public BuiltInFunction {
    public:
        ExternalFnBase(const char * name, const char * cppName)
            : BuiltInFunction(name, cppName) {
            callBased = true;
        };
    };

    template<typename F> struct makeFuncArgs;
    template<typename R, typename ...Args> struct makeFuncArgs<R (*)(Args...)> : makeFuncArgs<R (Args...)> {};
    template<typename R, typename ...Args>
    struct makeFuncArgs<R (Args...)> {
        static __forceinline vector<TypeDeclPtr> make ( const ModuleLibrary & lib ) {
            return makeBuiltinArgs<R,Args...>(lib);
        }
    };

    template  <typename FuncT, FuncT fn, typename SimNodeT, typename FuncArgT>
    class ExternalFn : public ExternalFnBase {
        static_assert ( is_base_of<SimNode_CallBase, SimNodeT>::value, "only call-based nodes allowed" );
    public:
        __forceinline ExternalFn(const char * name, const ModuleLibrary & lib, const char * cppName = nullptr)
        : ExternalFnBase(name,cppName) {
            constructExternal(makeFuncArgs<FuncArgT>::make(lib));
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            const char * fnName = context.code->allocateName(this->name);
            return context.code->makeNode<SimNodeT>(at, fnName);
        }
    };

    template  <InteropFunction func, typename RetT, typename ...Args>
    class InteropFn : public BuiltInFunction {
    public:
        __forceinline InteropFn(const char * name, const ModuleLibrary & lib, const char * cppName = nullptr)
            : BuiltInFunction(name,cppName) {
            this->callBased = true;
            this->interopFn = true;
            constructInterop(makeBuiltinArgs<RetT, Args...>(lib));
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            const char * fnName = context.code->allocateName(this->name);
            return context.code->makeNode<SimNode_InteropFuncCall<func>>(BuiltInFunction::at,fnName);
        }
    };

    struct defaultTempFn {
        defaultTempFn() = default;
        defaultTempFn ( bool args, bool impl, bool result, bool econst )
            : tempArgs(args), implicitArgs(impl), tempResult(result), explicitConstArgs(econst) {}
        ___noinline bool operator () ( Function * fn ) {
            if ( tempArgs || implicitArgs ) {
                for ( auto & arg : fn->arguments ) {
                    if ( arg->type->isTempType() ) {
                        arg->type->temporary = tempArgs;
                        arg->type->implicit = implicitArgs;
                        arg->type->explicitConst = explicitConstArgs;
                    }
                }
            }
            if ( tempResult ) {
                if ( fn->result->isTempType() ) {
                    fn->result->temporary = true;
                }
            }
            return true;
        }
        bool tempArgs = false;
        bool implicitArgs = true;
        bool tempResult = false;
        bool explicitConstArgs = false;
    };

    struct permanentArgFn : defaultTempFn {
        permanentArgFn() : defaultTempFn(false,false,false,false) {}
    };

    struct temporaryArgFn : defaultTempFn {
        temporaryArgFn() : defaultTempFn(true,false,false,false) {}
    };

    struct explicitConstArgFn : defaultTempFn {
        explicitConstArgFn() : defaultTempFn(false,true,false,true) {}
    };

    template  <typename CType, typename ...Args>
    class BuiltIn_PlacementNew : public BuiltInFunction {
    public:
        __forceinline BuiltIn_PlacementNew(const char * fn, const ModuleLibrary & lib, const char * cna = nullptr)
        : BuiltInFunction(fn,cna), fnName(fn) {
            this->modifyExternal = true;
            this->isTypeConstructor = true;
            this->copyOnReturn = true;
            this->moveOnReturn = true;
            construct(makeBuiltinArgs<CType,Args...>(lib));
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            return context.code->makeNode<SimNode_PlacementNew<CType,Args...>>(at,fnName);
        }
        const char * fnName = nullptr;
    };

    template  <typename CType, typename ...Args>
    class BuiltIn_Using : public BuiltInFunction {
    public:
        __forceinline BuiltIn_Using(const ModuleLibrary & lib, const char * cppName)
        : BuiltInFunction("using","das_using") {
            this->cppName = string("das_using<") + cppName + ">::use";
            this->aotTemplate = true;
            this->modifyExternal = true;
            this->invoke = true;
            vector<TypeDeclPtr> args = makeBuiltinArgs<void,Args...>(lib);
            args.emplace_back(makeType<const TBlock<void,TTemporary<TExplicit<CType>>>>(lib));
            construct(args);
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            return context.code->makeNode<SimNode_Using<CType,Args...>>(at);
        }
    };

    void addExternFunc(Module& mod, const FunctionPtr & fx, bool isCmres, SideEffects seFlags);

    template <typename FuncT, FuncT fn, template <typename FuncTT, FuncTT fnt> class SimNodeT = SimNode_ExtFuncCall, typename QQ = defaultTempFn>
    inline auto addExtern ( Module & mod, const ModuleLibrary & lib, const char * name, SideEffects seFlags,
                                  const char * cppName = nullptr, QQ && tempFn = QQ() ) {
        auto fnX = make_smart<ExternalFn<FuncT, fn, SimNodeT<FuncT, fn>, FuncT>>(name, lib, cppName);
        tempFn(fnX.get());
        addExternFunc(mod, fnX, SimNodeT<FuncT, fn>::IS_CMRES, seFlags);
        return fnX;
    }

    template <typename FuncArgT, typename FuncT, FuncT fn, template <typename FuncTT, FuncTT fnt> class SimNodeT = SimNode_ExtFuncCall>
    inline auto addExternEx ( Module & mod, const ModuleLibrary & lib, const char * name, SideEffects seFlags,
                                  const char * cppName = nullptr ) {
        auto fnX = make_smart<ExternalFn<FuncT, fn, SimNodeT<FuncT, fn>, FuncArgT>>(name, lib, cppName);
        addExternFunc(mod, fnX, SimNodeT<FuncT, fn>::IS_CMRES, seFlags);
        return fnX;
    }

    template <InteropFunction func, typename RetT, typename ...Args>
    inline auto addInterop ( Module & mod, const ModuleLibrary & lib, const char * name, SideEffects seFlags,
                                   const char * cppName = nullptr ) {
        auto fnX = make_smart<InteropFn<func, RetT, Args...>>(name, lib, cppName);
        addExternFunc(mod, fnX, true, seFlags);
        return fnX;
    }

    template <typename CType, typename ...Args>
    inline auto addCtor ( Module & mod, const ModuleLibrary & lib, const char * name, const char * cppName = nullptr ) {
        auto fn = make_smart<BuiltIn_PlacementNew<CType,Args...>>(name,lib,cppName);
        DAS_ASSERT(fn->result->isRefType() && "can't add ctor to by-value types");
        mod.addFunction(fn);
        return fn;
    }

    template <typename CType, typename ...Args>
    inline auto addUsing ( Module & mod, const ModuleLibrary & lib, const char * cppName ) {
        auto fn = make_smart<BuiltIn_Using<CType,Args...>>(lib,cppName);
        mod.addFunction(fn);
        return fn;
    }

    template <typename CType, typename ...Args>
    inline auto addCtorAndUsing ( Module & mod, const ModuleLibrary & lib, const char * name, const char * cppName ) {
        auto fn = make_smart<BuiltIn_PlacementNew<CType,Args...>>(name,lib,cppName);
        DAS_ASSERT(fn->result->isRefType() && "can't add ctor to by-value types");
        mod.addFunction(fn);
        mod.addFunction(make_smart<BuiltIn_Using<CType,Args...>>(lib,cppName));
    }

    template <typename ET>
    inline void addEnumFlagOps ( Module & mod, ModuleLibrary & lib, const string & cppName ) {
        using method_not = das_operator_enum_NOT<ET>;
        addExtern<ET (*)(ET a),method_not::invoke>(mod, lib, "~", SideEffects::none,
            ("das_operator_enum_NOT<" + cppName + ">::invoke").c_str());
        using method_or = das_operator_enum_OR<ET>;
        addExtern<ET (*)(ET,ET),method_or::invoke>(mod, lib, "|", SideEffects::none,
            ("das_operator_enum_OR<" + cppName + ">::invoke").c_str());
        using method_xor = das_operator_enum_XOR<ET>;
        addExtern<ET (*)(ET,ET),method_xor::invoke>(mod, lib, "^", SideEffects::none,
            ("das_operator_enum_XOR<" + cppName + ">::invoke").c_str());
        using method_and = das_operator_enum_AND<ET>;
        addExtern<ET (*)(ET,ET),method_and::invoke>(mod, lib, "&", SideEffects::none,
            ("das_operator_enum_AND<" + cppName + ">::invoke").c_str());
        using method_and_and = das_operator_enum_AND_AND<ET>;
        addExtern<bool (*)(ET,ET),method_and_and::invoke>(mod, lib, "&&", SideEffects::none,
            ("das_operator_enum_AND_AND<" + cppName + ">::invoke").c_str());
        using method_or_equ = das_operator_enum_OR_EQU<ET>;
        addExtern<void (*)(ET&,ET),method_or_equ::invoke>(mod, lib, "|=", SideEffects::modifyArgument,
            ("das_operator_enum_OR_EQU<" + cppName + ">::invoke").c_str());
        using method_xor_equ = das_operator_enum_XOR_EQU<ET>;
        addExtern<void (*)(ET&,ET),method_xor_equ::invoke>(mod, lib, "^=", SideEffects::modifyArgument,
            ("das_operator_enum_XOR_EQU<" + cppName + ">::invoke").c_str());
        using method_and_equ = das_operator_enum_AND_EQU<ET>;
        addExtern<void (*)(ET&,ET),method_and_equ::invoke>(mod, lib, "&=", SideEffects::modifyArgument,
            ("das_operator_enum_AND_EQU<" + cppName + ">::invoke").c_str());
    }
}

