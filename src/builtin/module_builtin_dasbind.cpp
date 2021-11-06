#include "daScript/misc/platform.h"

#include "daScript/misc/performance_time.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"

#include "daScript/simulate/simulate_visit_op.h"

#include "daScript/misc/sysos.h"

namespace das {

#ifdef _MSC_VER
    typedef uint64_t ( __stdcall * StdCallFunction )( ... );
    typedef uint64_t ( __cdecl * CdeclCallFunction )( ... );
#else
    // TODO: how to support this on MAC? Unix?
    // this is here so that it compiles on non-windows platforms
    typedef uint64_t (* StdCallFunction )( ... );
    typedef uint64_t (* CdeclCallFunction )( ... );
#endif

    struct SimNode_ExtCallBase : SimNode {
        SimNode_ExtCallBase ( const LineInfo & a, const char * _library, const char * _name, void * fun ) : SimNode(a) {
            library = _library;
            name = _name;
            fnptr = fun;
        }
        SimNode * visit ( SimVisitor & vis ) override {
            V_BEGIN();
            V_OP(StdCall);
            V_ARG(library);
            V_ARG(name);
            V_END();
        }
        SimNode * copyNode ( Context & context, NodeAllocator * code ) override {
            auto that = (SimNode_ExtCallBase *) SimNode::copyNode(context, code);
            that->library = context.code->allocateName(library);
            that->name = context.code->allocateName(name);
            return that;
        }
        const char * library = nullptr;
        const char * name = nullptr;
        void * fnptr = nullptr;
    };

    template <typename FunctionType, int nArgs>
    struct SimNode_ExtCall : SimNode_ExtCallBase {
        SimNode_ExtCall ( const LineInfo & a, const char * _library, const char * _name, void * fun )
            : SimNode_ExtCallBase(a,_library,_name,fun) {
        }
        template <size_t ...I>
        __forceinline uint64_t CallFunction ( vec4f * args, index_sequence<I...> ) {
            auto call = reinterpret_cast<FunctionType>(fnptr);
            return call(cast<void *>::to(args[I])...);
        }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            uint64_t result = CallFunction(context.abiArguments(),make_index_sequence<nArgs>());
            return cast<uint64_t>::from(result);
        }
    };

    template <typename FunctionType>
    struct SimNode_ExtCall<FunctionType,0> : SimNode_ExtCallBase {
        SimNode_ExtCall ( const LineInfo & a, const char * _library, const char * _name, void * fun )
            : SimNode_ExtCallBase(a,_library,_name,fun) {
        }
        virtual vec4f eval ( Context & ) override {
            DAS_PROFILE_NODE
            auto call = reinterpret_cast<FunctionType>(fnptr);
            uint64_t result = call();
            return cast<uint64_t>::from(result);
        }
    };

    template <int nArgs>
    struct SimNode_StdCall : SimNode_ExtCall<StdCallFunction,nArgs> {
        SimNode_StdCall ( const LineInfo & a, const char * _library, const char * _name, void * fun )
            : SimNode_ExtCall<StdCallFunction,nArgs>(a,_library,_name, fun) {
        }
    };

    template <int nArgs>
    struct SimNode_CdeclCall : SimNode_ExtCall<CdeclCallFunction,nArgs> {
        SimNode_CdeclCall ( const LineInfo & a, const char * _library, const char * _name, void * fun )
            : SimNode_ExtCall<CdeclCallFunction,nArgs>(a,_library,_name, fun) {
        }
    };

    struct ExternFunctionAnnotation : FunctionAnnotation {
        ExternFunctionAnnotation() : FunctionAnnotation("extern") { }
        virtual bool apply(ExprBlock *, ModuleGroup &, const AnnotationArgumentList &, string & err) override {
            err = "not supported for block";
            return false;
        }
        virtual SimNode * simulate ( Context * context, Function * fun, const AnnotationArgumentList & args, string & err ) override {
            string fn_name;
            string library;
            string api;
            for ( auto & arg : args ) {
                        if ( arg.name=="name" && arg.type==Type::tString ) {
                    fn_name = arg.sValue;
                } else if ( arg.name=="library" && arg.type==Type::tString ) {
                    library = arg.sValue;
                } else if ( arg.name=="WINAPI" || arg.name=="winapi" || arg.name=="stdcall" || arg.name=="__stdcall" || arg.name=="STDCALL" ) {
                    api = "stdcall";
                } else if ( arg.name=="CDECL" || arg.name=="cdecl" || arg.name=="__cdecl" ) {
                    api = "cdecl";
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
            void * funptr = getFunctionAddress(libhandle, fn_name.c_str());
            if ( !funptr ) {
                err = "can't find function " + fn_name + " in library " + library;
                return nullptr;
            }
            auto nlib = context->code->allocateName(library);
            auto nname = context->code->allocateName(fn_name);
            if ( api=="stdcall") {
                return context->code->makeNodeUnroll<SimNode_StdCall>(int(fun->arguments.size()),fun->at,nlib,nname,funptr);
            } else if ( api=="cdecl" ) {
                return context->code->makeNodeUnroll<SimNode_CdeclCall>(int(fun->arguments.size()),fun->at,nlib,nname,funptr);
            } else {
                err = "unsupported api " + api;
                return nullptr;
            }
        }
        virtual bool apply ( const FunctionPtr & fun, ModuleGroup &, const AnnotationArgumentList &, string & )  override {
            fun->userScenario = true;
            return true;
        }
        virtual bool finalize ( const FunctionPtr &, ModuleGroup &, const AnnotationArgumentList &, const AnnotationArgumentList &, string & ) override {
            return true;
        }
        virtual bool finalize(ExprBlock *, ModuleGroup &,const AnnotationArgumentList &, const AnnotationArgumentList &, string &) override {
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
