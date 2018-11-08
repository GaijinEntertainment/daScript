//
//  interop.h
//  yzg
//
//  Created by Boris Batkin on 11/7/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef interop_h
#define interop_h

#include "simulate.hpp"

namespace yzg
{
    // convert arguments into a tuple of appropriate types
    template <typename ArgumentsType, size_t... I>
    __forceinline auto cast_args (__m128 * args, index_sequence<I...> )
    {
        return make_tuple( cast< typename tuple_element<I, ArgumentsType>::type  >::to ( args[ I ] )... );
    }
    
    // void static function or lambda
    template <typename FunctionType, typename TupleType, size_t... I>
    __forceinline void callStaticVoidFunction(FunctionType & fn, TupleType && args, index_sequence<I...> ) {
        fn(get<I>(forward<TupleType>(args))...);
    }
    
    // void static function or lambda
    template <typename FunctionType, typename TupleType, size_t... I>
    __forceinline auto callStaticFunction(FunctionType & fn, TupleType && args, index_sequence<I...> ) {
        return fn(get<I>(forward<TupleType>(args))...);
    }
    
    template <typename FuncT, typename RetT>
    struct SimNode_ExtFuncCallImpl : public SimNode_Call
    {
        virtual __m128 eval ( Context & context ) override {
            
            using FunctionTrait = function_traits<FuncT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            using Result  = typename FunctionTrait::result_type;
            evalArgs(context);
            auto cpp_args = cast_args<Arguments>(argValues, Indices());
            return cast<Result>::from ( callStaticFunction(*fn, move(cpp_args), Indices()) );
        }
        FuncT * fn;
    };
    
    template <typename FuncT>
    struct SimNode_ExtFuncCallImpl<FuncT,void> : public SimNode_Call
    {
        virtual __m128 eval ( Context & context ) override {
            
            using FunctionTrait = function_traits<FuncT>;
            const int nargs = tuple_size<typename FunctionTrait::arguments>::value;
            using Indices = make_index_sequence<nargs>;
            using Arguments = typename FunctionTrait::arguments;
            evalArgs(context);
            auto cpp_args = cast_args<Arguments>(argValues, Indices());
            callStaticVoidFunction(*fn, move(cpp_args), Indices());
            return _mm_setzero_ps();
        }
        FuncT * fn;
    };
    
    template <typename FuncT>
    struct SimNode_ExtFuncCall : public SimNode_ExtFuncCallImpl<FuncT, typename function_traits<FuncT>::return_type>
    {
        SimNode_ExtFuncCall ( FuncT * func ) { this->fn = func; }
    };
}

#endif /* interop_h */
