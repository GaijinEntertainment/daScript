#include "daScript/misc/platform.h"

#include "module_builtin_rtti.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/simulate/fs_file_info.h"
#include "daScript/ast/ast_serializer.h"
#include "daScript/ast/dyn_modules.h"
#include "daScript/misc/gc_node.h"
#include "daScript/simulate/aot_builtin_ast.h"

das::Context * get_context ( int stackSize=0 );

namespace das {

    void rtti_builtin_simulate ( const smart_ptr<Program> & program,
            const TBlock<void,bool,smart_ptr_raw<Context>,string> & block, Context * context, LineInfoArg * lineinfo ) {
        TextWriter issues;
        auto ctx = get_context(program->getContextStackSize());
        ctx->addRef();
        bool failed = !program->simulate(*ctx, issues);
        if ( failed ) {
            for ( auto & err : program->errors ) {
                issues << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            string istr = issues.str();
            das_invoke<void>::invoke<bool,smart_ptr_raw<Context>,const string &>(context,lineinfo,block,false,nullptr,istr);
        } else {
            das_invoke<void>::invoke<bool,smart_ptr_raw<Context>,const string &>(context,lineinfo,block,true,ctx,"");
        }
        ctx->delRef();
    }

    Module * rtti_get_this_module ( smart_ptr_raw<Program> program ) {
        return program->thisModule.get();
    }

    // a member's guard answers as the require's would (src/ast/ARCHITECTURE_MODULE_SCAN.md#module-scan-manifest)
    static bool moduleGroupMemberAvailable ( const ModuleGroupMember & m ) {
        if ( m.guard.empty() ) return true;
        if ( m.guard.find('/') == string::npos ) return guardModuleAvailable(m.guard);
        auto env = daScriptEnvironment::getBound();
        auto program = env->g_Program;
        if ( !program || !program->access ) return false;
        // the base the collector and the parser resolve a relative guard against: the file being compiled
        auto ginfo = program->access->getModuleInfo(m.guard, env->g_compilingFileName ? env->g_compilingFileName : "");
        return !ginfo.fileName.empty() && program->access->getFileInfo(ginfo.fileName) != nullptr;
    }

    void rtti_module_group_for_each_member ( const char * group, const TBlock<void,const char *> & block, Context * context, LineInfoArg * at ) {
        for ( const auto & member : getModuleGroupMembers(group ? group : "") ) {
            if ( !moduleGroupMemberAvailable(member) ) continue;
            vec4f args[1] = { cast<const char *>::from(member.member.c_str()) };
            context->invoke(block, args, nullptr, at);
        }
    }

    void rtti_builtin_program_for_each_module ( smart_ptr_raw<Program> program, const TBlock<void,Module *> & block, Context * context, LineInfoArg * at ) {
        program->library.foreach([&](Module * pm) -> bool {
            vec4f args[1] = { cast<Module *>::from(pm) };
            context->invoke(block, args, nullptr, at);
            return true;
        }, "*");
    }

    void rtti_builtin_module_for_each_dependency ( Module * module, const TBlock<void,Module *,bool> & block, Context * context, LineInfoArg * at ) {
        for ( auto it : module->requireModule ) {
            vec4f args[2] = { cast<Module *>::from(it.first), cast<bool>::from(it.second) };
            context->invoke(block, args, nullptr, at);
        }
    }

    void rtti_builtin_module_for_each_enumeration ( Module * module, const TBlock<void,const EnumInfo> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        module->enumerations.foreach([&](auto penum){
            EnumInfo * info = helper.makeEnumDebugInfo(*penum);
            vec4f args[1] = {
                cast<EnumInfo *>::from(info)
            };
            context->invoke(block, args, nullptr, at);
        });
    }

    void rtti_builtin_module_for_each_structure ( Module * module, const TBlock<void,const StructInfo> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        module->structures.foreach([&](auto structPtr){
            if ( structPtr->isTemplate ) return;
            StructInfo * info = helper.makeStructureDebugInfo(*structPtr);
            vec4f args[1] = {
                cast<StructInfo *>::from(info)
            };
            context->invoke(block, args, nullptr, at);
        });
    }

    void rtti_builtin_module_for_each_function ( Module * module, const TBlock<void,const FuncInfo> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        module->functions.foreach([&](auto funcPtr){
            if ( funcPtr->isTemplate ) return;
            FuncInfo * info = helper.makeFunctionDebugInfo(*funcPtr);
            vec4f args[1] = {
                cast<FuncInfo *>::from(info)
            };
            context->invoke(block, args, nullptr, at);
        });
    }

    void rtti_builtin_module_for_each_generic ( Module * module, const TBlock<void,const FuncInfo> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        module->generics.foreach([&](auto funcPtr){
            FuncInfo * info = helper.makeFunctionDebugInfo(*funcPtr);
            vec4f args[1] = {
                cast<FuncInfo *>::from(info)
            };
            context->invoke(block, args, nullptr, at);
        });
    }

    void rtti_builtin_module_for_each_global ( Module * module, const TBlock<void,const VarInfo> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        module->globals.foreach([&](auto var){
            VarInfo * info = helper.makeVariableDebugInfo(*var);
            vec4f args[1] = {
                cast<VarInfo *>::from(info)
            };
            context->invoke(block, args, nullptr, at);
        });
    }

    void rtti_builtin_module_for_each_annotation ( Module * module, const TBlock<void,const Annotation> & block, Context * context, LineInfoArg * at ) {
        for ( auto & [key, annotationPtr] : module->handleTypes ) {
            vec4f args[1] = {
                cast<Annotation*>::from(annotationPtr)
            };
            context->invoke(block, args, nullptr, at);
        }
    }

    void rtti_builtin_basic_struct_for_each_parent ( const BasicStructureAnnotation & ann, const TBlock<void,Annotation *> & block, Context * context, LineInfoArg * at ) {
        for ( auto & it : ann.parents ) {
            vec4f args[1] = { cast<Annotation *>::from(it) };
            context->invoke(block, args, nullptr, at);
        }
    }

    void rtti_builtin_basic_struct_for_each_field ( const BasicStructureAnnotation & ann,
        const TBlock<void,char *,char*,const TypeInfo,uint32_t> & block, Context * context, LineInfoArg * at ) {
        DebugInfoHelper helper;
        for ( auto & it : ann.fields ) {
            const auto & fld = it.second;
            TypeInfo * info = helper.makeTypeInfo(nullptr, fld.decl);
            vec4f args[4] = {
                cast<char *>::from(it.first.c_str()),
                cast<char *>::from(fld.cppName.c_str()),
                cast<TypeInfo *>::from(info),
                cast<uint32_t>::from(fld.offset)
            };
            context->invoke(block, args, nullptr, at);
        }
    }

    int rtti_add_annotation_argument(AnnotationArgumentList& list, const char* name) {
        list.emplace_back(name != nullptr ? name : "", 0);
        return (int)list.size() - 1;
    }

    bool rtti_add_file_access_root ( smart_ptr<FileAccess> access, const char * mod, const char * path ) {
        if ( !mod ) return false;
        if ( !path ) return false;
        return access->addFsRoot(mod, path);
    }

    void rtti_add_extra_module ( smart_ptr_raw<FileAccess> access, const char * modName, const char * modFile, Context * context, LineInfoArg * at ) {
        if ( !modName ) context->throw_error_at(at, "expecting module name");
        if ( !modFile ) context->throw_error_at(at, "expecting module file path");
        access->addExtraModule(modName, modFile);
    }

#if !DAS_NO_FILEIO

    bool introduceFile ( smart_ptr_raw<FileAccess> access, char * fname, char * str, Context * context, LineInfoArg * at ) {
        if ( !fname ) context->throw_error_at(at, "expecting file name");
        const char * safeStr = str ? str : "";
        uint32_t str_len = stringLength(*context, safeStr);
        auto fileInfo = make_unique<TextFileInfo>(safeStr, str_len, false);
        return access->setFileInfo(fname, das::move(fileInfo)) != nullptr;
    }

#else
    bool introduceFile ( smart_ptr_raw<FileAccess>, char *, char *, Context * context, LineInfoArg * at ) {
        context->throw_error_at(at, "not supported with DAS_NO_FILEIO");
        return false;
    }
#endif

    void builtin_expected_errors ( ProgramPtr prog, const TBlock<void,CompilationError,int> & block, Context * context, LineInfoArg * lineInfo ) {
        for ( auto er : prog->expectErrors ) {
            das_invoke<void>::invoke<CompilationError,int>(context,lineInfo,block,er.first,er.second);
        }
    }

    void builtin_list_require ( ProgramPtr prog, const TBlock<void,Module *,TTemporary<const char *>,TTemporary<const char *>,bool,const LineInfo &> & block,
            Context * context, LineInfoArg * lineInfo ) {
        for ( auto & allR : prog->allRequireDecl ) {
            das_invoke<void>::invoke<Module *,const char *,const char *,bool,const LineInfo &>(context,lineInfo,block,
                get<0>(allR),get<1>(allR).c_str(),get<2>(allR).c_str(),get<3>(allR),get<4>(allR));
        }
    }

    AstSerializerState * rtti_create_ast_serializer () {
        auto state = new AstSerializerState();
        state->storage = make_unique<SerializationStorageVector>();
        state->serializer = make_unique<AstSerializer>(state->storage.get(), true);
        return state;
    }

    AstSerializerState * rtti_create_ast_deserializer ( const TArray<uint8_t> & data ) {
        auto state = new AstSerializerState();
        state->storage = make_unique<SerializationStorageVector>();
        state->storage->buffer.assign(data.data, data.data + data.size);
        state->serializer = make_unique<AstSerializer>(state->storage.get(), false);
        return state;
    }

    void rtti_delete_ast_serializer ( AstSerializerState * state ) {
        if ( state ) {
            state->serializer->moduleLibrary = nullptr;
            delete state;
        }
    }

    bool rtti_ast_serializer_serialize_program (
            AstSerializerState * state,
            const smart_ptr<Program> & program ) {
        auto & prog = const_cast<smart_ptr<Program> &>(program);
        prog->serialize(*state->serializer);
        return !prog->failToCompile;
    }

    void rtti_ast_serializer_deserialize_program_ex (
            AstSerializerState * state, smart_ptr<FileAccess> access, ModuleGroup * libGroup,
            const TBlock<void,bool,smart_ptr<Program>,const string> & block,
            Context * context, LineInfoArg * at ) {
        auto prog = make_smart<Program>();
        prog->access = access;      // the reader's: a stream carries none (src/ast/ARCHITECTURE.md#require-after-walk)
        state->serializer->thisModuleGroup = libGroup;
        {
            gc_guard deserialize_gc_scope;
            // same-version streams can still be truncated/corrupt: the stream readers throw
            // dasException — turn it into the clean failToCompile path (mirrors serializeScript)
            try {
                prog->serialize(*state->serializer);
            } catch ( const dasException & r ) {
                prog->failToCompile = true;
                LOG(LogLevel::warning) << "das: deserialize: " << r.what() << "\n";
            }
            /*
            // THIS ONES ARE FROM THE "already exist" MODULES
            auto leftover = deserialize_gc_scope.guard_root.gc_count;
            if ( leftover ) {
                LOG(LogLevel::warning) << "das: deserialize: " << leftover << " gc_node(s) left after deserialization\n";
                deserialize_gc_scope.guard_root.gc_dump_to_thread_root();
            }
            */
        }
        // Module::serialize leaves g_Program pointing to a temporary program with a
        // released thisModule. Restore it so compiling_module() sees the correct module
        // during simulate() and while tests run inside the block.
        auto & bound = *daScriptEnvironment::getBound();
        auto savedProg = bound.g_Program;
        bound.g_Program = prog.get();
        if ( prog->failToCompile ) {
            string err = "deserialization failed";
            das_invoke<void>::invoke<bool,smart_ptr<Program>,const string &>(
                context, at, block, false, ProgramPtr(), err);
            (void)prog->thisModule.release();
            prog->library.reset();
            bound.g_Program = savedProg;
            return;
        }
        string okStr;
        das_invoke<void>::invoke<bool,smart_ptr<Program>,const string &>(
            context, at, block, true, prog, okStr);
        (void)prog->thisModule.release();
        prog->library.reset();
        bound.g_Program = savedProg;
    }

    void rtti_ast_serializer_deserialize_program (
            AstSerializerState * state, ModuleGroup * libGroup,
            const TBlock<void,bool,smart_ptr<Program>,const string> & block,
            Context * context, LineInfoArg * at ) {
        rtti_ast_serializer_deserialize_program_ex(state, nullptr, libGroup, block, context, at);
    }

    int64_t rtti_ast_serializer_finalize_usec ( AstSerializerState * state ) {
        return state ? int64_t(state->serializer->totFinalizeTime) : 0;
    }

    int64_t rtti_ast_serializer_setup_usec ( AstSerializerState * state ) {
        return state ? int64_t(state->serializer->totSetupTime) : 0;
    }

    void rtti_ast_serializer_get_data (
            AstSerializerState * state,
            const TBlock<void,TTemporary<TArray<uint8_t> const>> & block,
            Context * context, LineInfoArg * at ) {
        state->storage->flush();
        Array arr;
        array_mark_locked(arr, state->storage->buffer.data(),
            uint64_t(state->storage->buffer.size()), uint64_t(state->storage->buffer.size()));
        vec4f args[1] = { cast<Array *>::from(&arr) };
        context->invoke(block, args, nullptr, at);
    }
}
