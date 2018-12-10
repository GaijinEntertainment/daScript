#pragma once

#include "ast.h"

namespace yzg
{
    using namespace std;
    
    template  <typename FuncT, FuncT fn>
    class ExternalFn : public BuiltInFunction {
        template <typename ArgumentsType, size_t... I>
        __forceinline vector<TypeDeclPtr> makeArgs ( const ModuleLibrary & lib, index_sequence<I...> ) {
            return { makeType< typename tuple_element<I, ArgumentsType>::type>(lib)... };
        }
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
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            this->result = makeType<Result>(lib);
            this->totalStackSize = sizeof(Prologue);
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            auto pCall = context.makeNode<SimNode_ExtFuncCall<FuncT,fn>>(at);
            pCall->info = context.thisProgram->makeFunctionDebugInfo(context, *this);
            return pCall;
        }
    };
    
    template  <InteropFunction func, typename RetT, typename ...Args>
    class InteropFn : public BuiltInFunction {
    public:
        InteropFn(const string & name, const ModuleLibrary & lib) : BuiltInFunction(name) {
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( int argi=0; argi!=args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            this->result = makeType<RetT>(lib);
            this->totalStackSize = sizeof(Prologue);
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            auto pCall = context.makeNode<SimNode_InteropFuncCall<func>>(at);
            pCall->info = context.thisProgram->makeFunctionDebugInfo(context, *this);
            return pCall;
        }
    };
    
    template <typename FuncT, FuncT fn>
    __forceinline void addExtern ( Module & mod, const ModuleLibrary & lib, const string & name ) {
        mod.addFunction(make_shared<ExternalFn<FuncT,fn>>(name,lib));
    }
    
    template <InteropFunction func, typename RetT, typename ...Args>
    __forceinline void addInterop ( Module & mod, const ModuleLibrary & lib, const string & name ) {
        mod.addFunction(make_shared<InteropFn<func,RetT,Args...>>(name,lib));
    }
}

