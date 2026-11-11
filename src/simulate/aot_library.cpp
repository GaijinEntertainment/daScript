#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

namespace das {

    AotListBase * AotListBase::head = nullptr;

    SimNode_AotInteropBase::SimNode_AotInteropBase() : SimNode_CallBase(LineInfo(),"") {}

    SimNode* AotFactory::operator()(Context& ctx) const
    {
        if (declineReason) {
            return nullptr;
        } else if (is_jit) {
            return makeAotJitNode(ctx, fn);
        } else if (is_cmres) {
            return ctx.code->makeNode<SimNode_AotCMRES>(fn, wrappedFn);
        } else {
            return ctx.code->makeNode<SimNode_Aot>(fn, wrappedFn);
        }
    }

    void aotLibraryEmplace ( AotLibrary & lib, uint64_t hash, const AotFactory & factory ) {
        lib.emplace(hash, factory);
    }

    AotListBase::AotListBase( RegisterAotFunctions prfn ) {
        tail = head;
        head = this;
        regFn = prfn;
    }

    void AotListBase::registerAot ( AotLibrary & lib ) {
        auto it = head;
        while ( it ) {
            (*it->regFn)(lib);
            it = it->tail;
        }
    }

    DAS_THREAD_LOCAL(unique_ptr<AotLibrary>) g_AOT_lib;

    AotLibrary & getGlobalAotLibrary() {
        if ( !*g_AOT_lib ) {
            *g_AOT_lib = make_unique<AotLibrary>();
            AotListBase::registerAot(**g_AOT_lib);
        }
        return **g_AOT_lib;
    }

    void clearGlobalAotLibrary() {
        g_AOT_lib->reset();
    }

    char * das_string_builder ( Context * __context__, const SimNode_AotInteropBase & node ) {
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i=0, is=node.nArguments; i!=is; ++i ) {
            walker.walk(node.argumentValues[i], node.types[i]);
        }
        auto length = writer.tellp();
        if ( length ) {
            return __context__->allocateString(writer.c_str(),uint32_t(length), &node.debugInfo);
        } else {
            return nullptr;
        }
    }

    char * das_string_builder_temp ( Context * __context__, const SimNode_AotInteropBase & node ) {
        StringBuilderWriter writer;
        DebugDataWalker<StringBuilderWriter> walker(writer, PrintFlags::string_builder);
        for ( int i=0, is=node.nArguments; i!=is; ++i ) {
            walker.walk(node.argumentValues[i], node.types[i]);
        }
        auto length = writer.tellp();
        if ( length ) {
            auto str = __context__->allocateTempString(writer.c_str(), uint32_t(length), &node.debugInfo);
            __context__->freeTempString(str, &node.debugInfo);
            return str;
        } else {
            return nullptr;
        }
    }

}
