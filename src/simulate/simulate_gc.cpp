#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/data_walker.h"
#include "daScript/simulate/debug_print.h"
namespace das
{

    char * presentStr ( char * buf, char * ch, int size );
    struct StringHeapReporter : DataWalker {
        Context * context = nullptr;
        using loop_point = pair<void *,uint32_t>;
        vector<loop_point> visited;
        vector<loop_point> visited_handles;
        vector<string>      history;
        vector<string>      keys;
        void prepare ( const string & walk_from ) {
            history.clear();
            history.push_back(walk_from);
        }
        virtual void beforeStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            DataWalker::beforeStructureField(ps,si,pv,vi,last);
            history.push_back(vi->name);
        }
        virtual void afterStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) override {
            history.pop_back();
            DataWalker::afterStructureField(ps,si,pv,vi,last);
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
            if ( context->constStringHeap->isOwnPtr(st) ) {
                return;
            }
            TextPrinter tp;
            tp << "\t";
            for (size_t i=0; i!=history.size(); ++i ) {
                if ( i!=0 && (history[i].empty() || history[i][0]!='[') ) tp << ".";
                tp << history[i];
            }
            tp << " = ";
            char buf[32];
            tp << presentStr(buf, st, 32);
            tp << ", at 0x" << uint64_t(st) << "\n";
        }
    };

    void Context::reportStringHeap ( LineInfo * at ) {
        TextPrinter tp;
        // now
        StringHeapReporter walker;
        walker.context = this;
        // mark globals
        tp << "GLOBALS:\n";
        for ( int i=0; i!=totalVariables; ++i ) {
            auto & pv = globalVariables[i];
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

