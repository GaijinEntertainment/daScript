#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/data_walker.h"

namespace das
{
    struct GcMarkStringHeap : DataWalker {
        Context * context = nullptr;
        virtual void String ( char * & st ) {
            DataWalker::String(st);
            if ( !st ) {
                return;
            }
            if ( context->constStringHeap->isOwnPtr(st) ) {     // not a const string
                return;
            }
            auto it = context->stringHeap.bigStuff.find(st);    // not a big allocation
            if ( it != context->stringHeap.bigStuff.end() ) {
                it->second |= DAS_PAGE_GC_MASK;
                return;
            }
            auto len = strlen(st) + 1;
            len = (len + 15) & ~15;
            if ( len < DAS_MAX_SHOE_ALLOCATION ) {              // not a small allocation
                if ( context->stringHeap.shoe.mark(st, uint32_t(len)) ) {
                    return;
                }
            }
            // not part of regular heap
            // DAS_ASSERT(0 && "collecting string which is not in const or regular string heap");
        }
    };

    void Context::collectStringHeap ( LineInfo * at ) {
        // clean up, so that all small allocations are marked as 'free'
        stringHeap.shoe.beforeGC();
        // now
        GcMarkStringHeap walker;
        walker.context = this;
        // mark globals
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
            walker.walk(globals + pv.offset, pv.debugInfo);
        }
        // mark stack
        char * sp = stack.ap();
        const LineInfo * lineAt = at;
        while (  sp < stack.top() ) {
            Prologue * pp = (Prologue *) sp;
            Block * block = nullptr;
            FuncInfo * info = nullptr;
            char * SP = sp;
            if ( pp->info ) {
                intptr_t iblock = intptr_t(pp->block);
                if ( iblock & 1 ) {
                    block = (Block *) (iblock & ~1);
                    info = block->info;
                    SP = stack.bottom() + block->stackOffset;
                } else {
                    info = pp->info;
                }
            }
            if ( info ) {
                for ( uint32_t i = 0; i != info->count; ++i ) {
                    walker.walk(pp->arguments[i], info->fields[i]);
                }
                if ( info->locals ) {
                    for ( uint32_t i = 0; i != info->localCount; ++i ) {
                        auto lv = info->locals[i];
                        bool inScope = lineAt ? lineAt->inside(lv->visibility) : false;
                        if ( !inScope ) continue;
                        char * addr = nullptr;
                        if ( lv->cmres ) {
                            addr = (char *)pp->cmres;
                        } else if ( lv->isRefValue( ) ) {
                            addr = SP + lv->stackTop;
                        } else {
                            addr = SP + lv->stackTop;
                        }
                        if ( addr ) {
                            walker.walk(addr, lv);
                        }
                    }
                }
            }
            lineAt = info ? pp->line : nullptr;
            sp += info ? info->stackSize : pp->stackSize;
        }
        // sweep
        stringHeap.sweep();
    }
}

