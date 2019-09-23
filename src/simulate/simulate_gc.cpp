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
            for ( auto & book : context->stringHeap.shelf ) {   // not a short string
                if ( book.isOwnPtr(st) ) {
                    book.mark(st);
                    return;
                }
            }
            auto it = context->stringHeap.bigStuff.find(st - sizeof(StringHeader));
            if ( it != context->stringHeap.bigStuff.end() ) {
                it->second |= DAS_PAGE_GC_MASK;
                return;
            }
            // TODO: add debug code to verify uncategorized pointer
            //  see if its in the big stuff, or heap, or something - and report accordingly
            //  this is going to be part of DAS-ASAN
        }
    };

    void Context::collectStringHeap() {
        GcMarkStringHeap walker;
        walker.context = this;
        // mark globals
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
            walker.walk(globals + pv.offset, pv.debugInfo);
        }
        // sweep
        stringHeap.sweep();
    }
}

