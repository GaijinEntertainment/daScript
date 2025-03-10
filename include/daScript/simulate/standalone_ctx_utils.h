#ifndef DAS_AST_SIM_CONVERTERS_H
#define DAS_AST_SIM_CONVERTERS_H

#include <daScript/misc/platform.h>
#include <daScript/simulate/simulate.h>
#include <daScript/ast/ast.h>

#include <optional>

namespace das {
    using MangledNameHash = size_t;

    struct FunctionInfo {
        FunctionInfo(FunctionPtr fPtr)
            : name(fPtr->name)
            , mangledName(fPtr->getMangledName())
            , stackSize(fPtr->totalStackSize)
            , unsafeOperation(fPtr->unsafeOperation)
            , fastCall(fPtr->fastCall)
            , builtin(fPtr->module->builtIn)
            , promoted(fPtr->module->promoted)
            , res_ref(fPtr->result->isRefType() && !fPtr->result->ref)
            , pinvoke(fPtr->pinvoke)
        {}
        FunctionInfo(string name, string mangledName, size_t stackSize,
                     bool unsafeOperation, bool fastCall, bool builtin,
                     bool promoted, bool isResRef, bool pinvoke)
            : name(name)
            , mangledName(mangledName)
            , stackSize(stackSize)
            , unsafeOperation(unsafeOperation)
            , fastCall(fastCall)
            , builtin(builtin)
            , promoted(promoted)
            , res_ref(isResRef)
            , pinvoke(pinvoke) {}
        string name;
        string mangledName;
        size_t stackSize;
        bool unsafeOperation;
        bool fastCall;
        bool builtin;
        bool promoted;
        bool res_ref;
        bool pinvoke;
    };

    struct GlobalVarInfo {
        GlobalVarInfo(string name, const string &mangledName, size_t typeSize, bool globalShared)
            : name(std::move(name))
            , mangledNameHash(Variable::getMangledNameHash(mangledName))
            , typeSize(typeSize)
            , globalShared(globalShared) {}

        string name;
        uint64_t mangledNameHash;
        size_t typeSize;
        bool globalShared;
    };

    MangledNameHash ConvertFunction(const Context &ctx, SimFunction* gfun, FunctionInfo info);


    struct SizeDiff {
        size_t sharedSizeDiff;
        size_t globalsSizeDiff;
    };
    SizeDiff ConvertGlobalVariable(const Context &ctx, GlobalVariable* gvar, GlobalVarInfo info);
    void ConvertGlobalVar(Context &ctx, GlobalVariable* gvar, GlobalVarInfo info);

    void FillFunction(Context &ctx, uint64_t semHash, AotLibrary &aotLib, SimFunction *fn);
}

#endif
