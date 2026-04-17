#include <daScript/misc/platform.h>

#include "daScript/ast/ast.h"
#include "daScript/daScriptModule.h"
#include "daScript/misc/fpe.h"
#include "daScript/simulate/aot_builtin_jit.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/simulate_visit_op.h"

#include <inttypes.h>

namespace das {
    // forward declarations from module_builtin_fio.cpp
    void * register_dynamic_module(const char *, const char *, int, Context *, LineInfoArg *);
    void register_native_path(const char *, const char *, const char *, Context *, LineInfoArg *);
}


// This file provides JIT runtime helpers.
// All interaction with daScript runtime from JIT side happens
// through these functions.
namespace das {

    DAS_SUPPRESS_UB vec4f SimNode_Jit::eval ( Context & context ) {
        auto result = func(&context, context.abiArg, context.abiCMRES);
        context.result = result;
        return result;
    }

    SimNode * SimNode_JitBlock::visit ( SimVisitor & vis ) {
        uint64_t fptr = (uint64_t) func;
        V_BEGIN();
        V_OP(JitBlock);
        V_ARG(fptr);
        V_END();
    }

    DAS_SUPPRESS_UB vec4f SimNode_JitBlock::eval ( Context & context ) {
        auto ba = (BlockArguments *) ( context.stack.bottom() + blockPtr->argumentsOffset );
        return func(&context, ba->arguments, ba->copyOrMoveResult, blockPtr );
    }

    SimNode * SimNode_Jit::visit ( SimVisitor & vis ) {
        uint64_t fptr = (uint64_t) func;
        V_BEGIN();
        V_OP(Jit);
        V_ARG(fptr);
        V_END();
    }


    template <typename KeyType>
    int32_t jit_table_at ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context, LineInfoArg * at ) {
        if ( tab->isLocked() ) context->throw_error_at(at, "can't insert to a locked table");
        TableHash<KeyType> thh(context,valueTypeSize);
        auto hfn = hash_function(*context, key);
        return thh.reserve(*tab, key, hfn, at);
    }

    void * das_get_jit_table_at ( int32_t baseType, Context * context, LineInfoArg * at ) {
        JIT_TABLE_FUNCTION(&jit_table_at);
    }

    template <typename KeyType>
    bool jit_table_erase ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context, LineInfoArg * at ) {
        if ( tab->isLocked() ) context->throw_error_at(at, "can't erase from locked table");
        TableHash<KeyType> thh(context,valueTypeSize);
        auto hfn = hash_function(*context, key);
        return thh.erase(*tab, key, hfn) != -1;
    }

    void * das_get_jit_table_erase ( int32_t baseType, Context * context, LineInfoArg * at ) {
        JIT_TABLE_FUNCTION(&jit_table_erase);
    }

    template <typename KeyType>
    int32_t jit_table_find ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context ) {
        TableHash<KeyType> thh(context,valueTypeSize);
        auto hfn = hash_function(*context, key);
        return thh.find(*tab, key, hfn);
    }

    void * das_get_jit_table_find ( int32_t baseType, Context * context, LineInfoArg * at ) {
        JIT_TABLE_FUNCTION(&jit_table_find);
    }
extern "C" {
    DAS_RT_API void jit_exception ( const char * text, Context * context, LineInfoArg * at ) {
        context->throw_error_at(at, "%s", text ? text : "");
    }

    DAS_RT_API vec4f jit_call_or_fastcall ( SimFunction * fn, vec4f * args, Context * context ) {
        return context->callOrFastcall(fn, args, nullptr);
    }

    DAS_RT_API vec4f jit_invoke_block ( const Block & blk, vec4f * args, Context * context ) {
        return context->invoke(blk, args, nullptr, nullptr);
    }

    DAS_RT_API vec4f jit_invoke_block_with_cmres ( const Block & blk, vec4f * args, void * cmres, Context * context ) {
        return context->invoke(blk, args, cmres, nullptr);
    }

    DAS_RT_API vec4f jit_call_with_cmres ( SimFunction * fn, vec4f * args, void * cmres, Context * context ) {
        return context->callWithCopyOnReturn(fn, args, cmres, nullptr);
    }

    DAS_RT_API char * jit_string_builder ( Context & context, int32_t nArgs, TypeInfo ** types, LineInfoArg * at, vec4f * args ) {
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i < nArgs; ++i )
            walker.walk(args[i], types[i]);
        auto length = writer.tellp();
        if ( length ) {
            return context.allocateString(writer.c_str(), uint32_t(length), at);
        } else {
            return nullptr;
        }
    }

    DAS_RT_API char * jit_string_builder_temp ( Context & context, int32_t nArgs, TypeInfo ** types, LineInfoArg * at, vec4f * args ) {
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i = 0; i < nArgs; ++i )
            walker.walk(args[i], types[i]);
        auto length = writer.tellp();
        if ( length ) {
            auto str = context.allocateTempString(writer.c_str(), uint32_t(length), at);
            context.freeTempString(str, at);
            return str;
        } else {
            return nullptr;
        }
    }

    DAS_RT_API void jit_simnode_interop(void *ptr, int argCount, TypeInfo **types) {
        auto res = new(ptr) SimNode_AotInteropBase();
        res->argumentValues = nullptr;
        res->nArguments = argCount;
        res->types = types;
    }

    DAS_RT_API void jit_free_simnode_interop(SimNode_AotInteropBase *ptr) {
        ptr->~SimNode_AotInteropBase();
    }

    DAS_RT_API void *das_get_jit_simnode_interop() {
        return (void *) &jit_simnode_interop;
    }

    DAS_RT_API void *das_get_jit_free_simnode_interop() {
        return (void *) &jit_free_simnode_interop;
    }

    // We need it to bypass protected/private.
    class JitContext : public Context {
    public:
        ~JitContext() = default;
        JitContext(size_t totalVariables, size_t totalFunctions, size_t globalStringHeapSize,
                  size_t globSize, size_t shrSize, bool pinvoke, uint32_t stackSize = 16*1024)
            : Context(stackSize) {
            auto &context = *this;
            CodeOfPolicies policies;
            policies.debugger = false;
            context.setup(totalVariables, globalStringHeapSize, policies, {});
            context.globalsSize = globSize;
            context.sharedSize = shrSize;
            context.sharedOwner = true;
            for (int i = 0; i < totalVariables; i++) {
                globalVariables[i] = GlobalVariable{};
            }
            context.allocateGlobalsAndShared();
            memset(context.globals, 0, context.globalsSize);
            memset(context.shared, 0, context.sharedSize);
            if ( pinvoke ) {
                context.contextMutex = new recursive_mutex;
            }
        }

        void allocFunctions ( uint64_t count ) {
            functions = (SimFunction *) code->allocate(count * sizeof(SimFunction));
            memset(functions, 0, count * sizeof(SimFunction));
            totalFunctions = (int) count;
            // Allocate stub debugInfo for all function slots so that
            // runShutdownScript can safely iterate them.
            auto stubInfo = (FuncInfo *) code->allocate(count * sizeof(FuncInfo));
            memset(stubInfo, 0, count * sizeof(FuncInfo));
            for ( uint64_t i = 0; i < count; i++ ) {
                stubInfo[i].name = (char *) "unimplemented";
                functions[i].name = (char *) "unimplemented";
                functions[i].debugInfo = &stubInfo[i];
            }
            tabMnLookup = make_shared<das_hash_map<uint64_t,SimFunction *>>();
            tabGMnLookup = make_shared<das_hash_map<uint64_t,uint32_t>>();
        }

        void *registerJitFunction ( uint64_t index, const char * name, const char * mangledName,
                                   uint64_t mnh, uint32_t stackSize, void * fnPtr,
                                   bool cmres, bool fastcall, bool pinvoke, uint32_t nArguments ) {
            DAS_ASSERT(index < (uint64_t) totalFunctions);
            auto & fn = functions[index];
            fn.name = code->allocateName(name);
            fn.mangledName = code->allocateName(mangledName);
            fn.mangledNameHash = mnh;
            fn.stackSize = stackSize;
            fn.flags = 0;
            fn.cmres = cmres;
            fn.fastcall = fastcall;
            fn.pinvoke = pinvoke;
            fn.jit = true;
            auto finfo = (FuncInfo *) code->allocate(sizeof(FuncInfo));
            memset(finfo, 0, sizeof(FuncInfo));
            finfo->name = fn.name;
            finfo->stackSize = stackSize;
            finfo->count = nArguments;
            fn.debugInfo = finfo;
            auto node = code->makeNode<SimNode_Jit>(LineInfo{}, (JitFunction) fnPtr);
            fn.code = node;
            (*tabMnLookup)[mnh] = &fn;
            return &fn;
        }

        void registerJitGlobalVariable(uint64_t mnh, size_t offset) {
            (*tabGMnLookup)[mnh] = offset;
        }

        void initFunctionAddr ( uint64_t index, void * globPtr ) {
            DAS_ASSERT(index < (uint64_t) totalFunctions);
            *((SimFunction **) globPtr) = &functions[index];
        }
    };

    // Note: this function called in runtime, before main.
    // When we built executable from das.
    DAS_RT_API Context * jit_create_standalone_ctx ( uint64_t totalVariables,
                                                  uint64_t totalFunctions,
                                                  uint64_t globalStringHeapSize,
                                                  uint64_t globalsSize,
                                                  uint64_t sharedSize,
                                                  bool pinvoke,
                                                  uint64_t stackSize) {
        Context *context = new JitContext(totalVariables, totalFunctions, globalStringHeapSize,
                                         globalsSize, sharedSize, pinvoke,
                                         stackSize ? (uint32_t)stackSize : 16*1024);
        static_cast<JitContext *>(context)->allocFunctions(totalFunctions);
        return context;
    }

    DAS_RT_API void *jit_register_standalone_function ( Context * ctx, uint64_t index,
                                             const char * name, const char * mangledName,
                                             uint64_t mnh, uint32_t stackSize,
                                             void * fnPtr,
                                             bool cmres, bool fastcall, bool pinvoke,
                                             uint32_t nArguments ) {
        return static_cast<JitContext *>(ctx)->registerJitFunction(index, name, mangledName, mnh, stackSize,
                                                            fnPtr, cmres, fastcall, pinvoke, nArguments);
    }

    DAS_RT_API void jit_register_standalone_variable ( Context * ctx, uint64_t mangledNameHash, uint64_t offset ) {
        static_cast<JitContext *>(ctx)->registerJitGlobalVariable(mangledNameHash, offset);
    }

    DAS_RT_API void jit_set_init_script ( Context * ctx, Context::JitInitScriptFn fn ) {
        ctx->jitInitScript = fn;
    }

    DAS_RT_API void jit_init_function_addr ( Context * ctx, uint64_t index, void * globPtr ) {
        static_cast<JitContext *>(ctx)->initFunctionAddr(index, globPtr);
    }

    DAS_RT_API void jit_init_extern_function ( const char * moduleName,
                                            const char * funcMangledName,
                                            void ** dllGlobal ) {
        bool found = false;
        Module::foreach([&](Module * module) -> bool {
            if ( module->name != moduleName ) return true;
            auto fn = module->findFunction(funcMangledName);
            if ( fn && fn->builtIn ) {
                *dllGlobal = static_cast<BuiltInFunction *>(fn)->getBuiltinAddress();
                found = *dllGlobal != nullptr;
                return !found;
            }
            return true;
        });
        if ( !found && strcmp(moduleName, "dasbind") == 0 && strncmp(funcMangledName, "@dasbind::__dasbind__", 21) == 0 ) {
            Module::foreach([&](Module * module) -> bool {
                if ( module->name != "dasbind" ) return true;
                auto resolverFn = module->findUniqueFunction("__dasbind_resolve");
                if ( resolverFn && resolverFn->builtIn ) {
                    auto resolver = (void * (*)(const char *))
                        static_cast<BuiltInFunction *>(resolverFn)->getBuiltinAddress();
                    if ( resolver ) {
                        *dllGlobal = resolver(funcMangledName);
                        found = *dllGlobal != nullptr;
                    }
                }
                return false;
            });
        }
        if (!found) {
            DAS_FATAL_ERROR("Failed to find %s in module %s.\n", funcMangledName, moduleName);
        }
    }

    DAS_RT_API Annotation *jit_get_annotation ( const char * moduleName,
                                                 const char * annName ) {
        Annotation *result = nullptr;
        Module::foreach([&](Module * module) -> bool {
            if ( module->name != moduleName ) return true;
            result = module->findAnnotation(annName);
            return false;
        });
        if (!result) {
            DAS_FATAL_ERROR("Failed to find annotation %s in module %s.\n", annName, moduleName);
        }
        return result;
    }

    DAS_RT_API void jit_trap() {
        DAS_FATAL_ERROR("FATAL: Unresolved dynamic function call in compiled code. This indicates a missing JIT symbol. Disable `strict` mode or remove this call.\n");
    }

    DAS_RT_API void jit_set_command_line_arguments( int argc, char * argv[] ) {
        setCommandLineArguments(argc, argv);
    }

    DAS_RT_API void *das_get_jit_init_extern_function() {
        return (void *) &jit_init_extern_function;
    }

    DAS_RT_API void * jit_get_global_mnh ( uint64_t mnh, Context & context ) {
        return context.globals + context.globalOffsetByMangledName(mnh);
    }

    DAS_RT_API void * jit_get_shared_mnh ( uint64_t mnh, Context & context ) {
        return context.shared + context.globalOffsetByMangledName(mnh);
    }

    DAS_RT_API void * jit_alloc_heap ( uint32_t bytes, Context * context ) {
        return context->allocate(bytes);
    }

    DAS_RT_API void * jit_alloc_persistent ( uint32_t bytes, Context * context ) {
        if ( !bytes ) context->throw_out_of_memory(false, bytes);
        return das_aligned_alloc16(bytes);
    }

    DAS_RT_API void jit_free_heap ( void * bytes, uint32_t size, Context * context ) {
        context->free((char *)bytes,size);
    }

    DAS_RT_API void jit_free_persistent ( void * bytes, Context * ) {
        das_aligned_free16(bytes);
    }

    DAS_RT_API void jit_array_lock ( const Array & arr, Context * context, LineInfoArg * at ) {
        builtin_array_lock_mutable(arr, context, at);
    }

    DAS_RT_API void jit_array_unlock ( const Array & arr, Context * context, LineInfoArg * at ) {
        builtin_array_unlock_mutable(arr, context, at);
    }

    DAS_RT_API int32_t jit_str_cmp ( char * a, char * b ) {
        return strcmp(a ? a : "",b ? b : "");
    }

    DAS_RT_API char * jit_str_cat ( const char * sA, const char * sB, Context * context, LineInfoArg * at ) {
        sA = sA ? sA : "";
        sB = sB ? sB : "";
        auto la = stringLength(*context, sA);
        auto lb = stringLength(*context, sB);
        uint32_t commonLength = la + lb;
        if ( !commonLength ) {
            return nullptr;
        } else {
            char * sAB = (char * ) context->allocateString(nullptr, commonLength, at);
            memcpy ( sAB, sA, la );
            memcpy ( sAB+la, sB, lb+1 );
            context->stringHeap->recognize(sAB);
            return sAB;
        }
    }

    struct JitStackState {
        char * EP;
        char * SP;
    };

    DAS_RT_API void jit_prologue ( const char *funcName, void * funcLineInfo,
            int32_t stackSize, JitStackState * stackState,
            Context * context, LineInfoArg * at ) {
        if (!context->stack.push(stackSize, stackState->EP, stackState->SP)) {
            context->throw_error_at(at, "stack overflow");
        }
#if DAS_ENABLE_STACK_WALK
        Prologue * pp = (Prologue *)context->stack.sp();
        pp->info = nullptr;
        pp->fileName = funcName;
        pp->functionLine = (LineInfo *) funcLineInfo;
        pp->stackSize = stackSize;
        pp->is_jit = true;
#endif
    }

    DAS_RT_API void jit_epilogue ( JitStackState * stackState, Context * context ) {
        context->stack.pop(stackState->EP, stackState->SP);
    }

    DAS_RT_API void jit_make_block ( Block * blk, int32_t argStackTop, uint64_t ad, void * bodyNode, void * jitImpl, void * funcInfo, void * lineInfo, Context * context ) {
        DAS_ASSERTF(lineInfo != nullptr, "Line info should not be null");

        JitBlock * block = (JitBlock *) blk;
        block->stackOffset = context->stack.spi();
        block->argumentsOffset = argStackTop ? (context->stack.spi() + argStackTop) : 0;
        block->body = (SimNode *)(void*) block->node;
        block->aotFunction = nullptr;
        block->jitFunction = jitImpl;
        block->functionArguments = context->abiArguments();
        block->info = (FuncInfo *) funcInfo;
        new (block->node) SimNode_JitBlock(*static_cast<LineInfo*>(lineInfo), (JitBlockFunction) bodyNode, blk, ad);
    }

    DAS_RT_API void jit_debug ( vec4f res, TypeInfo * typeInfo, char * message, Context * context, LineInfoArg * at ) {
        FPE_DISABLE;
        TextWriter ssw;
        if ( message ) ssw << message << " ";
        ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo, PrintFlags::debugger) << "\n";
        context->to_out(at, ssw.str().c_str());
    }

    DAS_RT_API bool jit_iterator_iterate ( das::Sequence &it, void *data, das::Context *context ) {
        return builtin_iterator_iterate(it, data, context);
    }

    DAS_RT_API void jit_iterator_delete ( das::Sequence &it, das::Context *context ) {
        return builtin_iterator_delete(it, context);
    }

    DAS_RT_API void jit_iterator_close ( das::Sequence &it, void *data, das::Context *context ) {
        return builtin_iterator_close(it, data, context);
    }

    DAS_RT_API bool jit_iterator_first ( das::Sequence &it, void *data, das::Context *context, das::LineInfoArg *at ) {
        return builtin_iterator_first(it, data, context, at);
    }

    DAS_RT_API bool jit_iterator_next ( das::Sequence &it, void *data, das::Context *context, das::LineInfoArg *at ) {
        return builtin_iterator_next(it, data, context, at);
    }

    DAS_RT_API void jit_debug_enter ( char * message, Context * context, LineInfoArg * at ) {
        TextWriter tw;
        tw << string(context->fnDepth, '\t'); context->fnDepth ++;
        tw << ">>";
        if ( !context->name.empty() ) tw << "(" << context->name << ")";
        tw << ": ";
        if ( message ) tw << message;
        if ( at && at->line ) tw << " at " << at->describe();
        tw << "\n";
        context->to_out(at, tw.str().c_str());
    }

    DAS_RT_API void jit_debug_exit ( char * message, Context * context, LineInfoArg * at ) {
        TextWriter tw;
        context->fnDepth --; tw << string(context->fnDepth, '\t');
        tw << " -";
        if ( !context->name.empty() ) tw << "(" << context->name << ")";
        tw << ": ";
        if ( message ) tw << message;
        if ( at && at->line ) tw << " at " << at->describe();
        tw << "\n";
        context->to_out(at, tw.str().c_str());
    }

    DAS_RT_API void jit_debug_line ( char * message, Context * context, LineInfoArg * at ) {
        TextWriter tw;
        tw << string(context->fnDepth + 1, '\t');
        tw << ">>";
        if ( !context->name.empty() ) tw << "(" << context->name << ")";
        tw << ": ";
        if ( message ) tw << message;
        if ( at && at->line ) tw << " at " << at->describe();
        tw << "\n";
        context->to_out(at, tw.str().c_str());
    }

    DAS_RT_API void jit_initialize_fileinfo ( void * dummy, const char *filename ) {
        new (dummy) FileInfo();
        auto fileInfoPtr = reinterpret_cast<FileInfo*>(dummy);
        fileInfoPtr->name = filename;
    }

    DAS_RT_API void jit_free_fileinfo ( void * dummy ) {
        reinterpret_cast<FileInfo*>(dummy)->~FileInfo();
    }

    struct JitAnnotationArgPod {
        const char * name;
        const char * sValue;
        int32_t      type;
        int32_t      iValue;  // covers bool/int/float (union bit-cast)
    };

    DAS_RT_API void jit_initialize_varinfo_annotations ( void * varinfo_ptr, int32_t nArgs, JitAnnotationArgPod * args ) {
        auto vi = (VarInfo *) varinfo_ptr;
        auto aa = new AnnotationArguments();
        aa->reserve(nArgs);
        for ( int32_t i = 0; i < nArgs; i++ ) {
            AnnotationArgument arg;
            arg.type   = (Type) args[i].type;
            arg.name   = args[i].name   ? args[i].name   : "";
            arg.sValue = args[i].sValue ? args[i].sValue : "";
            arg.iValue = args[i].iValue;
            aa->push_back(std::move(arg));
        }
        vi->annotation_arguments = aa;
    }

    DAS_RT_API void jit_free_varinfo_annotations ( void * varinfo_ptr ) {
        auto vi = (VarInfo *) varinfo_ptr;
        if ( vi->annotation_arguments ) {
            delete (AnnotationArguments *) vi->annotation_arguments;
            vi->annotation_arguments = nullptr;
        }
    }

    DAS_RT_API void * jit_ast_typedecl ( uint64_t hash, Context * context, LineInfoArg * at ) {
        if ( !context->thisProgram ) context->throw_error_at(at, "can't get ast_typeinfo, no program. is 'options rtti' missing?");
        auto ti = context->thisProgram->astTypeInfo.find(hash);
        if ( ti==context->thisProgram->astTypeInfo.end() ) {
            context->throw_error_at(at, "can't find ast_typeinfo for hash %" PRIx64, hash);
        }
        auto info = ti->second;
        return (void*) new TypeDecl(*info);
    }
}

    extern "C" {
        DAS_RT_API void * get_jit_table_find ( int32_t baseType, Context * context, LineInfoArg * at ) {
            return das_get_jit_table_find(baseType, context, at);
        }
        DAS_RT_API void * get_jit_table_at ( int32_t baseType, Context * context, LineInfoArg * at ) {
            return das_get_jit_table_at(baseType, context, at);
        }
        DAS_RT_API void * get_jit_table_erase ( int32_t baseType, Context * context, LineInfoArg * at ) {
            return das_get_jit_table_erase(baseType, context, at);
        }
        DAS_RT_API void * das_get_jit_new ( TypeAnnotation *annotation ) {
            return annotation->jitGetNew();
        }
        DAS_RT_API void * das_get_jit_delete ( TypeAnnotation *annotation ) {
            return annotation->jitGetDelete();
        }
        DAS_RT_API void * das_get_jit_clone ( TypeAnnotation *annotation ) {
            return annotation->jitGetClone();
        }
    }
}

static void init() {
    NEED_MODULE(Module_BuiltIn);
    NEED_MODULE(Module_Strings);
    NEED_MODULE(Module_FIO);
    NEED_MODULE(Module_DASBIND);
    NEED_MODULE(Module_UriParser);
}

extern "C" {
    DAS_RT_API void das_ensure_environment () {
        das::daScriptEnvironment::ensure();
    }

    DAS_RT_API void jit_initialize_modules () {
        das::daScriptEnvironment::ensure();
        init();
    }

    DAS_RT_API void jit_initialize_modules_done () {
        das::Module::Initialize();
    }

    DAS_RT_API void * jit_register_dynamic_module ( const char * path, const char * mod_name ) {
        return das::register_dynamic_module(path, mod_name, 0/*Quiet*/, nullptr, nullptr);
    }

    DAS_RT_API void jit_register_native_path ( const char * mod_name, const char * src_path, const char * dst_path ) {
        das::register_native_path(mod_name, src_path, dst_path, nullptr, nullptr);
    }
}
