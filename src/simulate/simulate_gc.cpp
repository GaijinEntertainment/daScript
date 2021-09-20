#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/data_walker.h"
#include "daScript/simulate/debug_print.h"
namespace das
{
    char * presentStr ( char * buf, char * ch, int size );

    struct HeapReporter : DataWalker {
        struct PtrRange {
            char * from = nullptr;
            char * to = nullptr;
            PtrRange () {}
            PtrRange ( char * F, size_t size ) : from(F), to(F+size) {}
            void clear() { from = to = nullptr; }
            bool empty() const { return from==nullptr; }
            bool contains ( const PtrRange & r ) { return !empty() && !r.empty() && from<=r.from && to>=r.to; }
        };
        bool reportStringHeap = true;
        bool reportHeap = true;
        bool heapOnly = false;
        Context * context = nullptr;
        using loop_point = pair<void *,uint32_t>;
        vector<loop_point> visited;
        vector<loop_point> visited_handles;
        vector<string>      history;
        vector<string>      keys;
        vector<PtrRange>    ptrRangeStack;
        PtrRange            currentRange;
        void prepare ( const string & walk_from ) {
            DAS_ASSERT(keys.size()==0);
            DAS_ASSERT(ptrRangeStack.size()==0);
            history.clear();
            history.push_back(walk_from);
            currentRange.clear();
        }
        bool markRange ( const PtrRange & r ) {
            if ( currentRange.empty() ) return false;
            if ( currentRange.contains(r) ) return false;
            if ( heapOnly ) {
                int ssize = int(r.to-r.from);
                ssize = (ssize + 15) & ~15;
                return context->heap->isOwnPtr(r.from, ssize);
            }
            return true;
        }
        bool isVoid ( TypeInfo * ti ) {
            return ti==nullptr || (ti->dimSize==0 && ti->type==Type::tVoid);
        }
        void pushRange ( const PtrRange & rdata ) {
            ptrRangeStack.push_back(currentRange);
            currentRange = rdata;
        }
        void popRange() {
            currentRange = ptrRangeStack.back();
            ptrRangeStack.pop_back();
        }
        void describe_ptr ( TextPrinter & tp, char * pa, int tsize, bool isHandle = false ) {
            tp << " " << tsize <<  " bytes, at 0x" << HEX << uint64_t(pa) << DEC << " ";
            auto ssize = (tsize+15) & ~15;
            if ( context->stack.is_stack_ptr(pa) ) {
                tp << "STACK";
            } else if ( context->isGlobalPtr(pa) ) {
                tp << "GLOBAL";
            } else if ( context->isSharedPtr(pa) ) {
                tp << "SHAREDGLOBAL";
            } else if ( context->heap->isOwnPtr(pa, ssize) ) {
                tp << "HEAP";
            } else if ( context->stringHeap->isOwnPtr(pa, ssize) ) {
                tp << "STRINGHEAP";
            } else if ( context->constStringHeap->isOwnPtr(pa) ) {
                tp << "CONSTSTRINGHEAP";
            } else if  ( context->code->isOwnPtr(pa) ) {
                tp << "CODE";
            } else if ( context->debugInfo->isOwnPtr(pa) ) {
                tp << "DEBUGINFO";
            } else if ( isHandle ) {
                tp << "HANDLED";
            } else {
                tp << "!!!UNCATEGORIZED!!!";
            }
            tp << "\n";
        }
        virtual void beforeHandle ( char * pa, TypeInfo * ti ) override {
            DataWalker::beforeHandle(pa, ti);
            visited_handles.emplace_back(make_pair(pa,ti->hash));
            auto tsize = getTypeSize(ti);
            PtrRange rdata(pa, tsize );
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "HANDLE " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize, true);
            }
            pushRange(rdata);
        }
        virtual void afterHandle ( char * pa, TypeInfo * ti ) override {
            popRange();
            visited_handles.pop_back();
            DataWalker::afterHandle(pa, ti);
        }
        virtual void beforeDim ( char * pa, TypeInfo * ti ) override {
            DataWalker::beforeDim(pa,ti);
            auto tsize = getTypeSize(ti);
            PtrRange rdata(pa, tsize );
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "DIM " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterDim ( char * pa, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterDim(pa,ti);
        }
        virtual void beforeArray ( Array * PA, TypeInfo * ti ) override {
            DataWalker::beforeArray(PA,ti);
            auto tsize = getTypeSize(ti->firstType) * PA->capacity;
            char * pa = PA->data;
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "ARRAY " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterArray ( Array * pa, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterArray(pa,ti);
        }
        virtual void beforeTable ( Table * PT, TypeInfo * ti ) override {
            DataWalker::beforeTable(PT, ti);
            auto tsize = (getTypeSize(ti->firstType) + getTypeSize(ti->secondType) + sizeof(uint32_t)) * PT->capacity;
            char * pa = PT->data;
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "TABLE " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, int(tsize));
            }
            pushRange(rdata);
        }
        virtual void afterTable ( Table * pa, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterTable(pa, ti);
        }
        virtual void beforeRef ( char * pa, TypeInfo * ti ) override {
            DataWalker::beforeRef(pa, ti);
            auto tsize = getTypeSize(ti);
            PtrRange rdata(pa, tsize );
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "& " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterRef ( char * pa, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterRef(pa, ti);
        }
        virtual void beforePtr ( char * pa, TypeInfo * ti ) override {
            DataWalker::beforePtr(pa, ti);
            auto tsize = getTypeSize(ti);
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && !isVoid(ti->firstType) && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "? " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterPtr ( char * pa, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterPtr(pa, ti);
        }
        virtual void beforeStructure ( char * ps, StructInfo * si ) override {
            DataWalker::beforeStructure(ps, si);
            visited.emplace_back(make_pair(ps,si->hash));
            char * pa = ps;
            auto tsize = si->size;
            if ( si->flags & StructInfo::flag_lambda ) {
                pa -= 16;
                tsize += 16;
            }
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "STRUCTURE " << si->module_name << "::" << si->name;
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterStructure ( char * ps, StructInfo * si ) override {
            popRange();
            visited.pop_back();
            DataWalker::afterStructure(ps,si);
        }
        virtual void beforeStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            DataWalker::beforeStructureField(ps,si,pv,vi,last);
            history.push_back(vi->name);
        }
        virtual void afterStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            history.pop_back();
            DataWalker::afterStructureField(ps,si,pv,vi,last);
        }
        virtual void beforeVariant ( char * ps, TypeInfo * ti ) override {
            DataWalker::beforeTuple(ps, ti);
            char * pa = ps;
            auto tsize = getTypeSize(ti);
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "VARIANT " << getTypeInfoMangledName(ti);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterVariant ( char * ps, TypeInfo * ti ) override {
            popRange();
            DataWalker::afterVariant(ps,ti);
        }
        virtual void beforeTuple ( char * ps, TypeInfo * si ) override {
            DataWalker::beforeTuple(ps, si);
            char * pa = ps;
            auto tsize = getTypeSize(si);
            PtrRange rdata(pa, tsize);
            if ( reportHeap && tsize && markRange(rdata) ) {
                TextPrinter tp;
                ReportHistory(tp);
                tp << "TUPLE " << getTypeInfoMangledName(si);
                describe_ptr(tp, pa, tsize);
            }
            pushRange(rdata);
        }
        virtual void afterTuple ( char * ps, TypeInfo * si ) override {
            popRange();
            DataWalker::afterTuple(ps,si);
        }
        virtual void beforeTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            DataWalker::beforeTupleEntry(ps,ti,pv,vi,last);
            uint32_t VI = -1u;
            for ( uint32_t i=0; i!=ti->argCount; ++i ) {
                if ( ti->argTypes[i]==vi ) {
                    VI = i;
                    break;
                }
            }
            history.push_back(to_string(VI));
        }
        virtual void afterTupleEntry ( char * ps, TypeInfo * ti, char * pv, TypeInfo * vi, bool last ) override {
            history.pop_back();
            DataWalker::afterTupleEntry(ps,ti,pv,vi,last);
        }
        virtual void beforeArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            DataWalker::beforeArrayElement(pa,ti,pe,index,last);
            history.push_back("["+to_string(index)+"]");
        }
        virtual void afterArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) override {
            history.pop_back();
            DataWalker::afterArrayElement(pa,ti,pe,index,last);
        }
        virtual void beforeTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
            DataWalker::beforeTableKey(pa,ti,pk,ki,index,last);
            string keyText = debug_value ( pk, ki, PrintFlags::none );
            keys.push_back(keyText);
            history.push_back("=>key=>");
        }
        virtual void afterTableKey ( Table * pa, TypeInfo * ti, char * pk, TypeInfo * ki, uint32_t index, bool last ) override {
            history.pop_back();
            DataWalker::afterTableKey(pa,ti,pk,ki,index,last);
        }
        virtual void beforeTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
            DataWalker::beforeTableValue(pa,ti,pv,kv,index,last);
            string keyText = keys.back();
            history.push_back("[\""+escapeString(keyText)+"\"]");
        }
        virtual void afterTableValue ( Table * pa, TypeInfo * ti, char * pv, TypeInfo * kv, uint32_t index, bool last ) override {
            keys.pop_back();
            history.pop_back();
            DataWalker::afterTableValue(pa,ti,pv,kv,index,last);
        }
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
        virtual void String ( char * & st ) override {
            DataWalker::String(st);
            if ( !reportStringHeap ) return;
            if ( !st ) return;
            if ( context->constStringHeap->isOwnPtr(st) ) return;
            TextPrinter tp;
            ReportHistory(tp);
            char buf[32];
            tp << "STRING = " << presentStr(buf, st, 32) << ", at 0x" << HEX << uint64_t(st) << DEC << "\n";
        }
        void ReportHistory ( TextPrinter & tp ) {
            tp << "\t";
            for (size_t i=0; i!=history.size(); ++i ) {
                if ( i!=0 && (history[i].empty() || history[i][0]!='[') ) tp << ".";
                tp << history[i];
            }
            tp << " : ";
        }
    };

    void Context::reportAnyHeap(LineInfo * at, bool sth, bool rgh, bool rghOnly) {
        TextPrinter tp;
        // now
        HeapReporter walker;
        walker.context = this;
        walker.reportStringHeap = sth;
        walker.reportHeap = rgh;
        walker.heapOnly = rghOnly;
        // mark globals
        if ( sharedOwner ) {
            tp << "SHARED GLOBALS:\n";
            for ( int i=0; i!=totalVariables; ++i ) {
                auto & pv = globalVariables[i];
                if ( !pv.shared ) continue;
                walker.prepare(pv.name);
                walker.walk(shared + pv.offset, pv.debugInfo);
            }
        }
        tp << "GLOBALS:\n";
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
            if ( pv.shared ) continue;
            if ( strcmp(pv.name,"g_instructionsByCategory")==0 ) {
                printf("");
            }
            walker.prepare(pv.name);
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
                tp << "FUNCTION " << info->name << "\n";
            }
            if ( info ) {
                if ( info->count ) {
                    tp << "ARGUMENTS:\n";
                    for ( uint32_t i = 0; i != info->count; ++i ) {
                        walker.prepare(info->fields[i]->name);
                        walker.walk(pp->arguments[i], info->fields[i]);
                    }
                }
                if ( info->locals ) {
                    tp << "LOCALS:\n";
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
                            walker.prepare(string("FUNCTION: ")+info->name+" LOCAL:"+lv->name);
                            walker.walk(addr, lv);
                        }
                    }
                }
            }
            lineAt = info ? pp->line : nullptr;
            sp += info ? info->stackSize : pp->stackSize;
        }
    }

    void Context::reportStringHeap ( LineInfo * at ) {
        reportAnyHeap(at, true, false, false);
    }

    void Context::reportHeap ( LineInfo * at ) {
        reportAnyHeap(at, false, true, true);
    }
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
        if ( sharedOwner ) {
            for ( int i=0; i!=totalVariables; ++i ) {
                auto & pv = globalVariables[i];
                if ( !pv.shared ) continue;
                walker.walk(shared + pv.offset, pv.debugInfo);
            }
        }
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
            if ( pv.shared ) continue;
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

    void Context::collectHeap ( LineInfo * at ) {
    }
}

