#include "precomp.h"

#include "module_builtin.h"
#include "ast_interop.h"
#include "ast_policy_types.h"
#include "ast_handle.h"

#include <ctime>

namespace yzg {

    template <>
    struct typeFactory<time_t> {
        static TypeDeclPtr make(const ModuleLibrary & library ) {
            return library.makeHandleType("clock");
        }
    };
    
#ifndef _MSC_VER
    template <>
    struct cast <time_t> {
        static __forceinline time_t to ( __m128 x )    { return *((time_t *)&x); }
        static __forceinline __m128 from ( time_t x )  { __m128 a; *((time_t *)&a) = x; return a; }
    };
#endif
    
    struct ClockTypeAnnotation : ManagedValueAnnotation<time_t> {
        ClockTypeAnnotation() : ManagedValueAnnotation<time_t>("clock") {}
        virtual void debug ( stringstream & ss, void * data ) const override {
            ss << std::asctime(std::localtime((time_t *)data));
        }
    };
    
    struct SimPolicy_time_t : SimPolicy_CoreType<time_t>, SimPolicy_Ordered<time_t> {
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) {
            time_t A = cast<time_t>::to(a);
            time_t B = cast<time_t>::to(b);
            return cast<float>::from( float(difftime(A, B)) );
        }
    };
    
    time_t builtin_clock() {
        return time(nullptr);
    }
    
    char * builtin_clockToString( time_t t, Context * context ) {
        stringstream ss;
        ss << std::asctime(std::localtime(&t));
        return context->allocateName(ss.str());
    }
    
    void Module_BuiltIn::addTime(ModuleLibrary & lib) {
        addAnnotation(make_shared<ClockTypeAnnotation>());
        addExtern<decltype(builtin_clock),builtin_clock>(*this, lib, "getClock");
        addExtern<decltype(builtin_clockToString),builtin_clockToString>(*this, lib, "string", false);
        // clock operators
        addFunctionBasic<time_t,SimPolicy_time_t>(*this,lib);
        addFunctionOrdered<time_t, SimPolicy_time_t>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_Sub<SimPolicy_time_t>, float, time_t, time_t>  >("-", lib) );
    }
}
