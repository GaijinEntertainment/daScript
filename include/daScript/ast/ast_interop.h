#pragma once

#include "daScript/ast/ast.h"
#include "daScript/simulate/interop.h"

namespace das
{
    using namespace std;

    template  <typename FuncT, FuncT fn, typename SimNodeT>
    class ExternalFn : public BuiltInFunction {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4100)
#endif
        template <typename ArgumentsType, size_t... I>
        inline vector<TypeDeclPtr> makeArgs ( const ModuleLibrary & lib, index_sequence<I...> ) {
            return { makeType< typename tuple_element<I, ArgumentsType>::type>(lib)... };
        }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

    public:
        ExternalFn(const string & name, const ModuleLibrary & lib) : BuiltInFunction(name) {
            using FunctionTrait = function_traits<FuncT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            using Result  = typename FunctionTrait::return_type;
            auto args = makeArgs<Arguments>(lib, Indices());
            for ( int argi=0; argi!=nargs; ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                if ( arg->type->baseType==Type::fakeContext ) {
                    arg->init = make_shared<ExprFakeContext>(at);
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
                } else {
                    assert(0 && "we should not even be here");
                }
            }
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.code.makeNode<SimNodeT>(at);
        }
    };

    template  <typename RetT, typename ...Args>
    class InteropFnBase : public BuiltInFunction {
    public:
        InteropFnBase(const string & name, const ModuleLibrary & lib) : BuiltInFunction(name) {
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( size_t argi=0; argi!=args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                if ( arg->type->baseType==Type::fakeContext ) {
                    arg->init = make_shared<ExprConstPtr>(at);
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
        InteropFn(const string & name, const ModuleLibrary & lib) : InteropFnBase<RetT,Args...>(name,lib) {}
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.code.makeNode<SimNode_InteropFuncCall<func>>(BuiltInFunction::at);
        }
    };

    template <typename FuncT, FuncT fn, template <typename FuncTT, FuncTT fnt> class SimNodeT = SimNode_ExtFuncCall>
    __forceinline void addExtern ( Module & mod, const ModuleLibrary & lib, const string & name, bool hasSideEffects = true ) {
        mod.addFunction(make_shared<ExternalFn<FuncT,fn, SimNodeT<FuncT,fn>>>(name,lib)->sideEffects(hasSideEffects));
    }

    template <InteropFunction func, typename RetT, typename ...Args>
    __forceinline void addInterop ( Module & mod, const ModuleLibrary & lib, const string & name, bool hasSideEffects = true ) {
        mod.addFunction(make_shared<InteropFn<func,RetT,Args...>>(name,lib)->sideEffects(hasSideEffects));
    }
}

