#include <daScript/simulate/standalone_ctx_utils.h>

namespace das {
    MangledNameHash ConvertFunction(const Context &ctx, SimFunction* gfun, FunctionInfo info) {
        auto MNH = hash_blockz64((uint8_t *)info.mangledName.c_str());
        gfun->name = ctx.code->allocateName(info.name);
        gfun->mangledName = ctx.code->allocateName(info.mangledName);
        gfun->stackSize = info.stackSize;
        gfun->mangledNameHash = MNH;
        gfun->aotFunction = nullptr;
        gfun->flags = 0;
        gfun->fastcall = info.fastCall;
        gfun->unsafe = info.unsafeOperation;
        gfun->cmres = info.res_ref;
        gfun->builtin = info.builtin && !info.promoted;
        gfun->pinvoke = info.pinvoke;
        return MNH;
    }

    SizeDiff ConvertGlobalVariable(const Context &ctx, GlobalVariable* gvar, GlobalVarInfo info) {
        gvar->name = ctx.code->allocateName(info.name);
        gvar->size = info.typeSize;
        const auto sizeDiff = (uint64_t(gvar->size) + 0xful) & ~0xfull;
        gvar->flags = 0;
        gvar->mangledNameHash = info.mangledNameHash;
        gvar->init = nullptr;
        if ( info.globalShared ) {
            gvar->offset = (uint32_t) ctx.getSharedSize();
            gvar->shared = true;
            return {sizeDiff, 0};
        } else {
            gvar->offset = (uint32_t) ctx.getGlobalSize();
            return {0, sizeDiff};
        }
    }

    void ConvertGlobalVar(Context &ctx, GlobalVariable *gvar, GlobalVarInfo info) {
        auto sizeDiff = ConvertGlobalVariable(ctx, gvar, std::move(info));
        ctx.updateSharedGlobalSize(sizeDiff.sharedSizeDiff, sizeDiff.globalsSizeDiff);
    }

    void FillFunction(Context &ctx, uint64_t semHash, AotLibrary &aotLib, SimFunction *fn) {
        auto it = aotLib.find(semHash);
        if ( it != aotLib.end() ) {
            fn->code = (it->second)(ctx);
            fn->aot = true;
            auto fcb = (SimNode_CallBase *) fn->code;
            fn->aotFunction = fcb->aotFunction;
        }

    }
}
