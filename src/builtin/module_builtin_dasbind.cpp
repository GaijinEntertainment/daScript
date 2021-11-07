#include "daScript/misc/platform.h"

#include "daScript/misc/performance_time.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

#include "daScript/simulate/simulate_visit_op.h"

#include "daScript/misc/sysos.h"

namespace das {

#if DAS_BIND_EXTERNAL

#ifdef _MSC_VER
    typedef uint64_t ( __stdcall * StdCallFunction )( ... );
    typedef uint64_t ( __cdecl * CdeclCallFunction )( ... );
    typedef uint64_t ( __stdcall * OpenglCallFunction )( ... );
#else
    // TODO: how to support this on MAC? Unix?
    // this is here so that it compiles on non-windows platforms
    typedef uint64_t (* StdCallFunction )( ... );
    typedef uint64_t (* CdeclCallFunction )( ... );
#endif

#ifdef _MSC_VER
    void * getFunction ( const char * fun, const char * lib ) {
        void * libhandle = nullptr;
        libhandle = getLibraryHandle(lib);
        if ( !libhandle ) {
            libhandle = loadDynamicLibrary(lib);
        }
        if ( !libhandle ) {
            return nullptr;
        }
        return getFunctionAddress(libhandle, fun);
    }

    typedef void * ( __stdcall * FN_wglGetProcAddress ) ( const char * );
    void * openGetFunctionAddress ( const char * name ) {
        auto _wglGetProcAddress = (FN_wglGetProcAddress) getFunction("wglGetProcAddress","Opengl32");
        if  ( _wglGetProcAddress!=nullptr ) {
            if ( auto fn = _wglGetProcAddress(name) ) {
                return fn;
            }
        }
        return getFunction(name,"Opengl32");
    }
#else
    void * openGetFunctionAddress ( const char * name ) {
        return nullptr;
    }
#endif

    typedef vec4f ( * FastCallWrapper ) ( void * fn, vec4f * args );

    __forceinline vec4f   Rx ( int64_t x ) { return v_cast_vec4f(v_splatsi64(x)); }

    #define AX(i)   (*(uint64_t *)(args+i))
    #define AD(i)   (*(double *)(args+i))
#include "x86_64_wrapper.inc"
    #undef  AX
    #undef  AD

    struct SimNode_ExtCall : SimNode {
        SimNode_ExtCall ( const LineInfo & a, const char * _library, const char * _name, FastCallWrapper wrp, void * fun ) : SimNode(a) {
            strncpy(library, _library, sizeof(library));
            strncpy(name,_name,sizeof(name));
            wrapper = wrp;
            fnptr = fun;
        }
        SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(ExtCall);
            V_ARG(library);
            V_ARG(name);
            V_END();
        }
        vec4f eval ( Context & context ) {
            context.result = wrapper(fnptr, context.abiArg);
            return context.result;
        }
        char library[32];
        char name[64];
        FastCallWrapper wrapper = nullptr;
        void * fnptr = nullptr;
    };

    struct SimNode_ExtCallOpenGL : SimNode_ExtCall {
        SimNode_ExtCallOpenGL ( const LineInfo & a, const char * _library, const char * _name, FastCallWrapper wrp, void * fun ) :
            SimNode_ExtCall(a,_library,_name,wrp,fun) {
        }
        vec4f eval ( Context & context ) {
            if ( !fnptr ) fnptr = openGetFunctionAddress(name);
            if ( !fnptr ) context.throw_error_ex("function %s : %s failed to bind", name, library);
            context.result = wrapper(fnptr, context.abiArg);
            return context.result;
        }
    };

    FastCallWrapper getWrapper ( Function * fun ) {
        int args = ( fun->result->baseType==Type::tFloat || fun->result->baseType==Type::tDouble ) ? (1<<4) : 0;
        for ( int a=0; a<int(fun->arguments.size()); ++a ) {
            if ( a==4 ) break;
            auto tp = fun->arguments[a]->type->baseType;
            if ( tp==Type::tFloat || tp==Type::tDouble ) {
                args |= (1<<a);
            }
        }
        args += 32 * int(fun->arguments.size());
        return fastcall64_table[args];
    }

#endif

    struct ExternFunctionAnnotation : FunctionAnnotation {
        ExternFunctionAnnotation() : FunctionAnnotation("extern") { }
        virtual bool apply(ExprBlock *, ModuleGroup &, const AnnotationArgumentList &, string & err) override {
            err = "not supported for block";
            return false;
        }
        virtual bool finalize ( const FunctionPtr &, ModuleGroup &, const AnnotationArgumentList &, const AnnotationArgumentList &, string & ) override {
            return true;
        }
        virtual bool finalize(ExprBlock *, ModuleGroup &,const AnnotationArgumentList &, const AnnotationArgumentList &, string &) override {
            return true;
        }
#if !DAS_BIND_EXTERNAL
        virtual bool apply ( const FunctionPtr & fun, ModuleGroup &, const AnnotationArgumentList &, string & err )  override {
            err = "daScript is configured with extern functions disabled";
            return nullptr;
        }
#else
        virtual bool apply ( const FunctionPtr & fun, ModuleGroup &, const AnnotationArgumentList &, string & err )  override {
            if ( fun->arguments.size() >= MAX_WRAPPER_ARGUMENTS ) {
                err = "function has too many arguments for the current wrapper config";
                return nullptr;
            }
            fun->userScenario = true;
            fun->exports = true;
            return true;
        }
        virtual ExpressionPtr transformCall ( ExprCallFunc * call, string & ) {
            if ( !call->func ) return nullptr;
            auto needToTransform = false;
            for ( auto & arg : call->arguments ) {
                if ( arg->type->isString() ) {
                    if ( arg->rtti_isCallFunc() ) {
                        auto pCall = static_pointer_cast<ExprCallFunc>(arg);
                        if ( pCall->func->name!="safe_pass_string") {
                            needToTransform = true;
                            break;
                        }
                    } else if ( strcmp(arg->__rtti,"ExprConstString")==0 ) {
                        // do nothing
                    } else {
                        needToTransform = true;
                        break;
                    }
                }
            }
            if ( needToTransform ) {
                auto newCall = static_pointer_cast<ExprCallFunc>(call->clone());
                for ( auto & arg : newCall->arguments ) {
                    bool needToWrap = false;
                    if ( arg->type->isString() ) {
                        if ( arg->rtti_isCallFunc() ) {
                            auto pCall = static_pointer_cast<ExprCallFunc>(arg);
                            if ( pCall->func->name=="safe_pass_string") {
                                needToWrap = true;
                            }
                        } else if ( strcmp(arg->__rtti,"ExprConstString")==0 ) {
                            // do nothing
                        } else {
                            needToWrap = true;
                        }
                    }
                    if ( needToWrap ) {
                        auto wrapCall = make_smart<ExprCall>(arg->at,"safe_pass_string");
                        wrapCall->arguments.push_back(arg->clone());
                        arg = wrapCall;
                    }
                }
                return newCall;
            }
            return nullptr;
        }
        virtual SimNode * simulate ( Context * context, Function * fun, const AnnotationArgumentList & args, string & err ) override {
            bool anyTypeErrors = false;
            for ( const auto & arg : fun->arguments ) {
                if ( !arg->type->isRef() && arg->type->isVectorType() ) {
                    err += "argument " + arg->name + " is vector type, which is currently not supported by specified binding";
                    anyTypeErrors = true;
                }
            }
            if ( fun->result && !fun->result->isRef() && fun->result->isVectorType() ) {
                err += "function returns vector type, which is currently not supported by the specified binding";
                anyTypeErrors = true;
            }
            if ( anyTypeErrors ) return false;
            string fn_name;
            string library;
            string api;
            bool late = false;
            for ( auto & arg : args ) {
                        if ( arg.name=="name" && arg.type==Type::tString ) {
                    fn_name = arg.sValue;
                } else if ( arg.name=="library" && arg.type==Type::tString ) {
                    library = arg.sValue;
                } else if ( arg.name=="WINAPI" || arg.name=="winapi" || arg.name=="stdcall" || arg.name=="__stdcall" || arg.name=="STDCALL" ) {
                    api = "stdcall";
                } else if ( arg.name=="CDECL" || arg.name=="cdecl" || arg.name=="__cdecl" ) {
                    api = "cdecl";
                } else if ( arg.name=="opengl" || arg.name=="OPENGL" ) {
                    api = "opengl";
                } else if ( arg.name=="late" ) {
                    late = true;
                }
            }
            if ( fn_name.empty() ) {
                err = "missing name";
                return nullptr;
            }
            if ( api.empty() ) {
                err = "need to specify calling convention (like stdcall (aka WINAPI),cdecl,etc)";
                return nullptr;
            }
            void * libhandle = nullptr;
            if ( !library.empty() ) {
                libhandle = getLibraryHandle(library.c_str());
                if ( !libhandle ) {
                    libhandle = loadDynamicLibrary(library.c_str());
                }
                if ( !libhandle ) {
                    err = "can't load library " + library;
                    return nullptr;
                }
            }
            void * funptr = nullptr;
            if ( api=="opengl" ) {
                funptr = openGetFunctionAddress(fn_name.c_str());
            }
            if ( !funptr ) {
                funptr = getFunctionAddress(libhandle, fn_name.c_str());
            }
            if ( !funptr && !late ) {
                err = "can't find function " + fn_name + " in library " + library;
                return nullptr;
            }
            auto wrp = getWrapper(fun);
            if ( api=="stdcall") {
                return context->code->makeNode<SimNode_ExtCall>(fun->at,library.c_str(),fn_name.c_str(),wrp,funptr);
            } else if ( api=="cdecl" ) {
                return context->code->makeNode<SimNode_ExtCall>(fun->at,library.c_str(),fn_name.c_str(),wrp,funptr);
            } else if ( api=="opengl" ) {
                return context->code->makeNode<SimNode_ExtCallOpenGL>(fun->at,library.c_str(),fn_name.c_str(),wrp,funptr);
            } else {
                err = "unsupported api " + api;
                return nullptr;
            }
        }
#endif
    };

    char * safe_pass_string ( const char * str ) {
        return (char *)(str ? str : "");
    }

    class Module_DASBIND : public Module {
    public:
        Module_DASBIND() : Module("dasbind") {
            DAS_PROFILE_SECTION("Module_DASBIND");
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            addAnnotation(make_smart<ExternFunctionAnnotation>());
            addExtern<DAS_BIND_FUN(safe_pass_string)>(*this, lib, "safe_pass_string",
                SideEffects::none, "safe_pass_string")
                    ->args({"string"});
        };
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_DASBIND,das);
