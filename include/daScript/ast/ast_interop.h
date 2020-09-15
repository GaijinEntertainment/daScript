#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/simulate/interop.h"

namespace das
{
    template  <typename FuncT, FuncT fn, typename SimNodeT, typename FuncArgT>
    class ExternalFn : public BuiltInFunction {

        static_assert ( is_base_of<SimNode_CallBase, SimNodeT>::value, "only call-based nodes allowed" );

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif
        template <typename ArgumentsType, size_t... I>
        inline vector<TypeDeclPtr> makeArgs ( const ModuleLibrary & lib, index_sequence<I...> ) {
            return { makeArgumentType< typename tuple_element<I, ArgumentsType>::type>(lib)... };
        }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    public:
        ExternalFn(const string & name, const ModuleLibrary & lib, const string & cppName = string())
        : BuiltInFunction(name,cppName) {
            callBased = true;
            using FunctionTrait = function_traits<FuncArgT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            using Result  = typename FunctionTrait::return_type;
            auto args = makeArgs<Arguments>(lib, Indices());
            for ( int argi=0; argi!=nargs; ++argi ) {
                auto arg = make_smart<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                if ( arg->type->baseType==Type::fakeContext ) {
                    arg->init = make_smart<ExprFakeContext>(at);
                } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                    arg->init = make_smart<ExprFakeLineInfo>(at);
                }
                this->arguments.push_back(arg);
            }
            this->result = makeType<Result>(lib);
            this->totalStackSize = sizeof(Prologue);
            if ( result->isRefType() ) {
                if ( result->canCopy() ) {
                    copyOnReturn = true;
                    moveOnReturn = false;
                } else if ( result->canMove() ) {
                    copyOnReturn = false;
                    moveOnReturn = true;
                } else if ( !result->ref ) {
                    DAS_FATAL_LOG("ExternalFn %s can't be bound. It returns values which can't be copied or moved\n", name.c_str());
                    DAS_FATAL_ERROR;
                }
            }
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            const char * fnName = context.code->allocateName(this->name);
            return context.code->makeNode<SimNodeT>(at, fnName);
        }
    };

    template  <typename RetT, typename ...Args>
    class InteropFnBase : public BuiltInFunction {
    public:
        InteropFnBase(const string & name, const ModuleLibrary & lib, const string & cppName = string())
            : BuiltInFunction(name,cppName) {
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( size_t argi=0; argi!=args.size(); ++argi ) {
                auto arg = make_smart<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                if ( arg->type->baseType==Type::fakeContext ) {
                    arg->init = make_smart<ExprFakeContext>(at); // arg->init = make_smart<ExprConstPtr>(at);
                } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                    arg->init = make_smart<ExprFakeLineInfo>(at);
                }
                this->arguments.push_back(arg);
            }
            this->result = makeType<RetT>(lib);
            this->totalStackSize = sizeof(Prologue);
        }
    };

    template  <InteropFunction func, typename RetT, typename ...Args>
    class InteropFn : public InteropFnBase<RetT,Args...> {
    public:
        InteropFn(const string & name, const ModuleLibrary & lib, const string & cppName = string())
            : InteropFnBase<RetT,Args...>(name,lib,cppName) {
            this->callBased = true;
            this->interopFn = true;
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            const char * fnName = context.code->allocateName(this->name);
            return context.code->makeNode<SimNode_InteropFuncCall<func>>(BuiltInFunction::at,fnName);
        }
    };

    struct defaultTempFn {
        defaultTempFn() = default;
        defaultTempFn ( bool args, bool impl, bool result )
            : tempArgs(args), implicitArgs(impl), tempResult(result) {}
        bool operator () ( Function * fn ) {
            if ( tempArgs || implicitArgs ) {
                for ( auto & arg : fn->arguments ) {
                    if ( arg->type->isTempType() ) {
                        arg->type->temporary = tempArgs;
                        arg->type->implicit = implicitArgs;
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
    };

    struct permanentArgFn : defaultTempFn {
        permanentArgFn() : defaultTempFn(false,false,false) {}
    };

    struct temporaryArgFn : defaultTempFn {
        temporaryArgFn() : defaultTempFn(true,false,false) {}
    };

    template <typename FuncT, FuncT fn, template <typename FuncTT, FuncTT fnt> class SimNodeT = SimNode_ExtFuncCall, typename QQ = defaultTempFn>
    inline auto addExtern ( Module & mod, const ModuleLibrary & lib, const string & name, SideEffects seFlags,
                                  const string & cppName = string(), QQ && tempFn = QQ() ) {
        auto fnX = make_smart<ExternalFn<FuncT, fn, SimNodeT<FuncT, fn>, FuncT>>(name, lib, cppName);
        if ( !SimNodeT<FuncT,fn>::IS_CMRES ) {
            if ( fnX->result->isRefType() && !fnX->result->ref ) {
                DAS_FATAL_LOG(
                    "addExtern(%s)::tempFn failed in module %s\n"
                    "  this function should be bound with SimNode_ExtFuncCallAndCopyOrMove option\n"
                    "  likely cast<> is implemented for the return type, and it should not\n",
                name.c_str(), mod.name.c_str());
                DAS_FATAL_ERROR;
            }
        }
        fnX->setSideEffects(seFlags);
        if ( !tempFn(fnX.get()) ) {
            DAS_FATAL_LOG("addExtern(%s)::tempFn failed in module %s\n", name.c_str(), mod.name.c_str());
            DAS_FATAL_ERROR;
        }
        if ( !mod.addFunction(fnX) ) {
            DAS_FATAL_LOG("addExtern(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
            DAS_FATAL_ERROR;
        }
        return fnX;
    }

    template <typename FuncArgT, typename FuncT, FuncT fn, template <typename FuncTT, FuncTT fnt> class SimNodeT = SimNode_ExtFuncCall>
    inline auto addExternEx ( Module & mod, const ModuleLibrary & lib, const string & name, SideEffects seFlags,
                                  const string & cppName = string()) {
        auto fnX = make_smart<ExternalFn<FuncT, fn, SimNodeT<FuncT, fn>, FuncArgT>>(name, lib, cppName);
        if ( !SimNodeT<FuncT,fn>::IS_CMRES ) {
            if ( fnX->result->isRefType() && !fnX->result->ref ) {
                DAS_FATAL_LOG(
                    "addExtern(%s)::tempFn failed in module %s\n"
                    "  this function should be bound with SimNode_ExtFuncCallAndCopyOrMove option\n"
                    "  likely cast<> is implemented for the return type, and it should not\n",
                name.c_str(), mod.name.c_str());
                DAS_FATAL_ERROR;
            }
        }
        fnX->setSideEffects(seFlags);
        if ( !mod.addFunction(fnX) ) {
            DAS_FATAL_LOG("addExternEx(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
            DAS_FATAL_ERROR;
        }
        return fnX;
    }

    template <InteropFunction func, typename RetT, typename ...Args>
    inline auto addInterop ( Module & mod, const ModuleLibrary & lib, const string & name, SideEffects seFlags,
                                   const string & cppName = string() ) {
        auto fnX = make_smart<InteropFn<func, RetT, Args...>>(name, lib, cppName);
        fnX->setSideEffects(seFlags);
        if ( !mod.addFunction(fnX) ) {
            DAS_FATAL_LOG("addInterop(%s) failed in module %s\n", name.c_str(), mod.name.c_str());
            DAS_FATAL_ERROR;
        }
        return fnX;
    }
}

