#include "daScript/misc/platform.h"

#include "module_builtin.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_policy_types.h"

#include <ctime>

namespace das {
    struct Time {
        time_t time;
    };
}

MAKE_TYPE_FACTORY(clock, das::Time)// use MAKE_TYPE_FACTORY out of namespace. Some compilers not happy otherwise

namespace das {
    struct TimeAnnotation : ManagedValueAnnotation<Time> {
        TimeAnnotation() : ManagedValueAnnotation<Time>("clock") {}
        virtual void walk ( DataWalker & walker, void * data ) override {
            if ( walker.reading ) {
                // there shuld be a way to read time from the stream here
            } else {
                Time * t = (Time *) data;
                char mbstr[100];
                if ( strftime(mbstr, sizeof(mbstr), "%c", localtime(&t->time)) ) {
                    char * str = mbstr;
                    walker.String(str);
                }
            }
        }
    };

    template <>
    struct cast <Time> {
        static __forceinline Time to ( vec4f x )               { union { Time t; vec4f vec; } T; T.vec = x; return T.t; }
        static __forceinline vec4f from ( Time x )             { union { Time t; vec4f vec; } T; T.t = x; return T.vec; }
    };

    template<>
    struct SimPolicy<Time> {
        static __forceinline auto to_time ( vec4f a ) {
            return cast<Time>::to(a).time;
        }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return to_time(a) == to_time(b);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) != to_time(b);
        }
        static __forceinline bool GtEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) >= to_time(b);
        }
        static __forceinline bool LessEqu  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) <= to_time(b);
        }
        static __forceinline bool Gt  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) > to_time(b);
        }
        static __forceinline bool Less  ( vec4f a, vec4f b, Context & ) {
            return to_time(a) < to_time(b);
        }
        static __forceinline vec4f Sub  ( vec4f a, vec4f b, Context & ) {
            double dt = difftime(to_time(a), to_time(b));
            return cast<float>::from((float)dt);
        }
    };

    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(GtEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(LessEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Gt,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Less,Time);
    IMPLEMENT_OP2_EVAL_POLICY(Sub, Time);

    Time builtin_clock() {
        Time t;
        t.time = time(nullptr);
        return t;
    }

    void Module_BuiltIn::addTime(ModuleLibrary & lib) {
        addAnnotation(make_shared<TimeAnnotation>());
        addFunctionBasic<Time>(*this,lib);
        addFunctionOrdered<Time>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_Sub<Time>,float,Time,Time>>("-",lib));
        addExtern<DAS_BIND_FUN(builtin_clock)>(*this, lib, "getClock", SideEffects::modifyExternal);
    }
}
