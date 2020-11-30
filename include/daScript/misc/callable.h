#pragma once

#ifndef DAS_CALLABLE_BYTES
#define DAS_CALLABLE_BYTES  128
#endif

namespace das {
    template <typename RetT, typename ...Args>
    struct CallableLambdaBase {
        virtual RetT invoke ( Args... ) = 0;
        virtual ~CallableLambdaBase() {}
    };

    template <typename LL, typename RetT, typename ...Args>
    struct CallableLambda : CallableLambdaBase<RetT,Args...> {
        CallableLambda ( LL && o ) : obj(o) {}
        virtual RetT invoke ( Args ...args ) override { return obj(args...); };
        LL  obj;
    };

    template <typename LL, typename ...Args>
    struct CallableLambda<LL,void,Args...> : CallableLambdaBase<void,Args...> {
        CallableLambda ( LL && o ) : obj(o) {}
        virtual void invoke ( Args ...args ) override { obj(args...); };
        LL  obj;
    };

    template <typename RetT, typename ...Args>
    struct CallableBase {
        using Callable  = CallableLambdaBase<RetT,Args...>;
        CallableBase ( const CallableBase & ) = delete;
        CallableBase & operator = ( const CallableBase & ) = delete;
        template <typename LL>
        CallableBase ( LL && obj ) {
            using CallableObject = CallableLambda<LL,RetT,Args...>;
            auto size = sizeof(CallableObject);
            if ( size <= DAS_CALLABLE_BYTES ) {
                callobj = (CallableObject *) bytes;
                new (callobj) CallableObject(obj);
            } else {
                callobj = new CallableObject(obj);
            }

        }
        ~CallableBase() {
            if ( (void*)bytes != (void*)callobj ) {
                delete callobj;
            } else {
                callobj->~Callable();
            }
        }
        Callable *  callobj;
        vec4f       bytes[DAS_CALLABLE_BYTES/16];
    };

    template <typename Func>
    struct callable;

    template <typename RetT, typename ...Args>
    struct callable<RetT (Args...)> : CallableBase<RetT,Args...> {
        template <typename LL>
        __forceinline callable ( LL && obj ) : CallableBase<RetT,Args...>(obj) {}
        __forceinline RetT operator () ( Args... args ) { return this->callobj->invoke(args...); }
    };

    template <typename ...Args>
    struct callable<void (Args...)> : CallableBase<void,Args...> {
        template <typename LL>
        __forceinline callable ( LL && obj ) : CallableBase<void,Args...>(obj) {}
        __forceinline void operator () ( Args... args ) { this->callobj->invoke(args...); }
    };
}

