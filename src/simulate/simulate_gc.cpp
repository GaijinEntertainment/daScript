#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/data_walker.h"

namespace das
{
    struct GcMarkStringHeap : DataWalker {
        Context * context = nullptr;
        using loop_point = pair<void *,uint32_t>;
        vector<loop_point> visited;
        vector<loop_point> visited_handles;
        virtual bool canVisitStructure ( char * ps, StructInfo * info ) override {
            return find_if(visited.begin(),visited.end(),[&]( const loop_point & t ){
                    return t.first==ps && t.second==info->hash;
                }) == visited.end();
        }
        virtual bool canVisitHandle ( char * ps, TypeInfo * info ) override {
            return find_if(visited.begin(),visited.end(),[&]( const loop_point & t ){
                    return t.first==ps && t.second==info->hash;
                }) == visited.end();
        }
        virtual void beforeStructure ( char * ps, StructInfo * info ) override {
            visited.emplace_back(make_pair(ps,info->hash));
        }
        virtual void afterStructure ( char *, StructInfo * ) override {
            visited.pop_back();
        }
        virtual void beforeHandle ( char * ps, TypeInfo * ti ) override {
            visited_handles.emplace_back(make_pair(ps,ti->hash));
        }
        virtual void afterHandle ( char *, TypeInfo * ) override {
            visited_handles.pop_back();
        }
        virtual void String ( char * & st ) override {
            DataWalker::String(st);
            if ( !st ) {
                return;
            }
            if ( context->constStringHeap->isOwnPtr(st) ) {     // not a const string
                return;
            }
            uint32_t len = uint32_t(strlen(st)) + 1;
            len = (len + 15) & ~15;
            context->stringHeap->mark(st, len);
        }
    };

    void Context::collectStringHeap ( LineInfo * at ) {
        // clean up, so that all small allocations are marked as 'free'
        if ( !stringHeap->mark() ) return;
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
        stringHeap->sweep();
    }
}

