#ifndef DAS_AST_SIM_CONVERTERS_H
#define DAS_AST_SIM_CONVERTERS_H

#include <daScript/misc/platform.h>
#include <daScript/simulate/simulate.h>
#include <daScript/ast/ast.h>

#include <optional>

namespace das {
    using MangledNameHash = uint64_t;

    struct FunctionInfo {
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
            : name(move(name))
            , mangledNameHash(Variable::getMNHash(mangledName))
            , typeSize(typeSize)
            , globalShared(globalShared) {}

        string name;
        uint64_t mangledNameHash;
        size_t typeSize;
        bool globalShared;
    };
    struct SizeDiff {
        uint64_t sharedSizeDiff;
        uint64_t globalsSizeDiff;
    };

    /**
     * Methods to init aot variables
     */
    MangledNameHash InitAotFunction(const Context &ctx, SimFunction* gfun, FunctionInfo info);
    SizeDiff InitGlobalVariable(const Context &ctx, GlobalVariable* gvar, GlobalVarInfo info);
    void InitGlobalVar(Context &ctx, GlobalVariable* gvar, GlobalVarInfo info);

    /**
     * Set code, aot, aotFunction for all function in @ref functions
     */
    void FillFunction(Context &ctx, AotLibrary &aotLib, vector<pair<uint64_t, SimFunction*>> functions);
}

#endif
