#include "daScript/misc/platform.h"

#include "module_builtin.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_policy_types.h"

#include "daScript/simulate/sim_policy.h"

#include "daScript/simulate/aot_builtin_time.h"

MAKE_TYPE_FACTORY(clock, das::Time)// use MAKE_TYPE_FACTORY out of namespace. Some compilers not happy otherwise

namespace das {

    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(GtEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(LessEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Gt,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Less,Time);
    IMPLEMENT_OP2_EVAL_POLICY(Sub, Time);

    struct TimeAnnotation : ManagedValueAnnotation<Time> {
        TimeAnnotation() : ManagedValueAnnotation<Time>("clock","das::Time") {}
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

    Time builtin_clock() {
        Time t;
        t.time = time(nullptr);
        return t;
    }

    void Module_BuiltIn::addTime(ModuleLibrary & lib) {
        addAnnotation(make_shared<TimeAnnotation>());
        addFunctionBasic<Time>(*this,lib);
        addFunctionOrdered<Time>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_Sub<Time>,float,Time,Time>>("-",lib,"Sub"));
        addExtern<DAS_BIND_FUN(builtin_clock)>(*this, lib, "getClock", SideEffects::modifyExternal, "builtin_clock");
    }
}
