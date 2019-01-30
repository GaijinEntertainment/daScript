#pragma once

namespace das
{
    // tail of the tuple
    template<typename T>
    struct tuple_tail;
    template<typename Head, typename... Tail>
    struct tuple_tail<tuple<Head, Tail...>> {
        using type = tuple<Tail... >;
    };

    // function traits
    template<typename F>
    struct function_traits;
    template<typename R, typename ...Args>
    struct function_traits<R (Args...)> {
        using return_type = R;
        using arguments = tuple<Args...>;
    };
    template<typename R, typename ...Args>
    struct function_traits<R (*)(Args...)> : function_traits<R (Args...)> {};       // function pointer
    template<typename C, typename R, typename ...Args>
    struct function_traits<R (C::*)(Args...)> : function_traits<R (C&, Args...)> {};        // member function pointer
    template<typename C, typename R, typename ...Args>
    struct function_traits<R (C::*)(Args...) const> : function_traits<R (C const&, Args...)> {};        // const member function pointer
    template<typename C, typename R, typename ...Args>
    struct function_traits<R(C::*)(Args...) volatile> : function_traits<R(C volatile&, Args...)> {};        // volatile member function pointer
    template<typename C, typename R, typename ...Args>
    struct function_traits<R(C::*)(Args...) const volatile> : function_traits<R(C const volatile&, Args...)> {};   // const volatile member function pointer
    template<typename C, typename R>
    struct function_traits<R (C::*)> : function_traits<R (C&)> {};      // member object pointer
    template<typename F>
    struct function_traits {    // function object, function, lambda
    #if DAS_STD_HAS_BIND
        static_assert(!is_bind_expression<F>::value, "bind result is not supported yet");
    #endif
    private:
        using callable_traits = function_traits<decltype(&F::operator())>;
    public:
        using return_type = typename callable_traits::return_type;
        using arguments = typename tuple_tail<typename callable_traits::arguments>::type;
    };
    template<typename F>
    struct function_traits<F&> : function_traits<F> {};
    template<typename F>
    struct function_traits<F&&> : function_traits<F> {};
}

