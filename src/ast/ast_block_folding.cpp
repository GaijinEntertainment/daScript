#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    class UnsafeFolding : public PassVisitor {
    public:
        using PassVisitor::visit;
        UnsafeFolding() : PassVisitor(0) {}
    protected:
        virtual ExpressionPtr visit ( ExprUnsafe * expr ) {
            return expr->body;
        }
    };

    void Program::foldUnsafe() {
        UnsafeFolding context;
        visit(context);
    }


    // this folds the following, by setting r2v flag on expressions
    //  r2v(var)            = @var
    //  r2v(expr.field)     = expr.@field
    //  r2v(expr[index])    = expr@[index]
    //  r2v(a ? b : c)      = a ? r2v(b) : r2v(c)
    //  r2v(cast(x))        = cast(r2v(x))
    class RefFolding : public PassVisitor {
    public:
        using PassVisitor::PassVisitor;
        using PassVisitor::visit;

        virtual bool canVisitStructure ( Structure * /*st*/ ) override { return false; }
        virtual bool canVisitEnumeration ( Enumeration * /*en*/ ) override { return false; }
        virtual bool canVisitStructureFieldInit ( Structure * /*var*/ ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * /*fun*/, const VariablePtr & /*var*/, Expression * /*init*/ ) override { return false; }

    protected:
        // note: loop_source is set during type inference (ast_infer_type.cpp)
        virtual ExpressionPtr visit ( ExprRef2Value * expr ) override {
            if ( expr->subexpr->rtti_isCast() ) {
                reportFolding();
                auto ecast = static_cast<ExprCast*>(expr->subexpr);
                auto nr2v = new ExprRef2Value();
                nr2v->at = expr->at;
                nr2v->subexpr = ecast->subexpr;
                nr2v->type = new TypeDecl(*nr2v->subexpr->type);
                nr2v->type->ref = false;
                ecast->subexpr = nr2v;
                ecast->type->ref = false;
                return ecast;
            } else if ( expr->subexpr->rtti_isVar() ) {
                reportFolding();
                auto evar = static_cast<ExprVar*>(expr->subexpr);
                evar->r2v = true;
                evar->type->ref = false;
                return evar;
            } else if ( expr->subexpr->rtti_isField() ) {
                reportFolding();
                auto efield = static_cast<ExprField*>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isAsVariant() ) {
                reportFolding();
                auto efield = static_cast<ExprAsVariant*>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isSafeAsVariant() ) {
                reportFolding();
                auto efield = static_cast<ExprSafeAsVariant*>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isSwizzle() ) {
                reportFolding();
                auto eswiz = static_cast<ExprSwizzle*>(expr->subexpr);
                eswiz->r2v = true;
                eswiz->type->ref = false;
                if (!TypeDecl::isSequencialMask(eswiz->fields)) {
                    eswiz->value = Expression::autoDereference(eswiz->value);
                }
                return eswiz;
            } else if ( expr->subexpr->rtti_isSafeField() ) {
                DAS_ASSERTF(false, "we should not be here. R2V of ?. is strange indeed");
                reportFolding();
                auto efield = static_cast<ExprSafeField*>(expr->subexpr);
                efield->r2v = true;
                efield->type->ref = false;
                return efield;
            } else if ( expr->subexpr->rtti_isAt() ) {
                reportFolding();
                auto eat = static_cast<ExprAt*>(expr->subexpr);
                eat->r2v = true;
                eat->type->ref = false;
                return eat;
            } else if ( expr->subexpr->rtti_isSafeAt() ) {
                DAS_ASSERTF(false, "we should not be here. R2V of ?[ is strange indeed");
                reportFolding();
                auto eat = static_cast<ExprSafeAt*>(expr->subexpr);
                eat->r2v = true;
                eat->type->ref = false;
                return eat;
            } else if ( expr->subexpr->rtti_isOp3() ) {
                reportFolding();
                auto op3 = static_cast<ExprOp3*>(expr->subexpr);
                op3->left = Expression::autoDereference(op3->left);
                op3->right = Expression::autoDereference(op3->right);
                op3->type->ref = false;
                return expr->subexpr;
            } else if ( expr->subexpr->rtti_isNullCoalescing() ) {
                reportFolding();
                auto nc = static_cast<ExprNullCoalescing*>(expr->subexpr);
                nc->defaultValue = Expression::autoDereference(nc->defaultValue);
                nc->type->ref = false;
                return nc;
            } else {
                return Visitor::visit(expr);
            }
        }
    };

    bool doesExprTerminates ( ExpressionPtr expr ) {
        if ( !expr ) return false;
        if ( expr->rtti_isBlock() ) {
            auto pBlock = static_cast<ExprBlock*>(expr);
            for ( auto & e : pBlock->list ) {
                if ( e->rtti_isReturn() || e->rtti_isBreak() || e->rtti_isContinue() ) {
                    return true;
                }
                if ( e->rtti_isBlock() ) {
                    if ( doesExprTerminates(e) ) {
                        return true;
                    }
                }
            }
        } else if ( expr->rtti_isIfThenElse() ) {
            auto pIte = static_cast<ExprIfThenElse*>(expr);
            if ( doesExprTerminates(pIte->if_true) && doesExprTerminates(pIte->if_false) ) {
                return true;
            }
        } else if ( expr->rtti_isWith() ) {
            auto pWith = static_cast<ExprWith*>(expr);
            if ( pWith->body && doesExprTerminates(pWith->body) ) {
                return true;
            }
        }
        return false;
    }

    class BlockFolding : public PassVisitor {
    public:
        using PassVisitor::PassVisitor;
        using PassVisitor::preVisit;

        virtual bool canVisitStructure ( Structure * /*st*/ ) override { return false; }
        virtual bool canVisitGlobalVariable ( Variable * /*fun*/ ) override { return false; }
        virtual bool canVisitEnumeration ( Enumeration * /*en*/ ) override { return false; }
        virtual bool canVisitStructureFieldInit ( Structure * /*var*/ ) override { return false; }
        virtual bool canVisitExpr ( ExprTypeInfo * /*expr*/, Expression * /*subexpr*/ ) override { return false; }
        virtual bool canVisitMakeStructureBlock ( ExprMakeStruct * /*expr*/, Expression * /*blk*/ ) override { return false; }
        virtual bool canVisitMakeStructureBody ( ExprMakeStruct * /*expr*/ ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * /*fun*/, const VariablePtr & /*var*/, Expression * /*init*/ ) override { return false; }
        virtual bool canVisitNamedCall ( ExprNamedCall * /*expr*/ ) override { return false; }

    protected:
        das_set<int32_t> labels;
        bool allLabels = false;
    protected:
        bool hasLabels ( vector<ExpressionPtr> & blockList ) const {
            for ( auto & expr : blockList ) {
                if ( !expr ) continue;
                if ( expr->rtti_isLabel() ) {
                    return true;
                } else  if ( expr->rtti_isBlock() ) {
                    auto pBlock = static_cast<ExprBlock*>(expr);
                    if ( !pBlock->isClosure ) {
                        if ( hasLabels(pBlock->list) ) return true;
                    }
                }
            }
            return false;
        }
        void collect ( vector<ExpressionPtr> & list, vector<ExpressionPtr> & blockList ) {
            bool stopAtExit = !hasLabels(blockList);
            bool skipTilLabel = false;
            for ( auto & expr : blockList ) {
                if ( !expr ) continue;
                if ( expr->rtti_isLabel() ) {
                    auto lexpr = static_cast<ExprLabel*>(expr);
                    if ( allLabels || (labels.find(lexpr->label)!=labels.end()) ) {
                        list.push_back(expr);
                        skipTilLabel = false;
                    }
                    continue;
                }
                if ( skipTilLabel ) continue;
                if ( expr->rtti_isGoto() ) {
                    list.push_back(expr);
                    skipTilLabel = true;
                    continue;
                }
                if ( expr->rtti_isBreak() || expr->rtti_isReturn() || expr->rtti_isContinue() ) {
                    if ( stopAtExit ) {
                        list.push_back(expr);
                        break;
                    } else {
                        list.push_back(expr);
                        skipTilLabel = true;
                        continue;
                    }
                }
                if ( expr->rtti_isIfThenElse() && doesExprTerminates(expr) ) {
                    if ( stopAtExit ) {
                        list.push_back(expr);
                        break;
                    } else {
                        list.push_back(expr);
                        skipTilLabel = true;
                        continue;
                    }
                }
                if ( expr->rtti_isBlock() ) {
                    auto pBlock = static_cast<ExprBlock*>(expr);
                    if ( !pBlock->isClosure && !pBlock->finalList.size() ) {
                        collect(list, pBlock->list);
                    } else {
                        list.push_back(expr);
                    }
                } else if ( expr->rtti_isWith() ) {
                    auto pWith = static_cast<ExprWith*>(expr);
                    if ( pWith->body ) {
                        list.push_back(pWith->body);
                    }
                } else if ( expr->rtti_isAssume() ) {
                    // do nothing with assume
                } else {
                    list.push_back(expr);
                }
            }
        }
    protected:
        virtual void preVisit ( ExprGoto * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->subexpr ) {
                allLabels = true;
            } else {
                labels.insert(expr->label);
            }
        }
    // ExprBlock
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            vector<ExpressionPtr> list;
            collect(list, block->list);
            if ( list!=block->list ) {
                swap ( block->list, list );
                reportFolding();
            }
            vector<ExpressionPtr> finalList;
            collect(finalList, block->finalList);
            if ( finalList!=block->finalList ) {
                swap ( block->finalList, finalList );
                reportFolding();
            }
            return Visitor::visit(block);
        }
    // ExprLet
        virtual ExpressionPtr visit ( ExprLet * let ) override {
            if ( let->variables.size()==0 ) {
                reportFolding();
                return nullptr;
            }
            return Visitor::visit(let);
        }
    // function
        virtual FunctionPtr visit ( Function * func ) override {
            labels.clear();
            allLabels = false;
            if ( func->body && func->result->isVoid() ) {   // remove trailing return on the void function
                if ( func->body->rtti_isBlock() ) {
                    auto block = static_cast<ExprBlock*>(func->body);
                    if ( block->list.size() && block->list.back()->rtti_isReturn() ) {
                        block->list.resize(block->list.size()-1);
                        reportFolding();
                        return Visitor::visit(func);
                    }
                }
            }
            return Visitor::visit(func);
        }
    };

    class CondFolding : public PassVisitor {
    public:
        using PassVisitor::PassVisitor;
        using PassVisitor::visit;

        virtual bool canVisitStructure ( Structure * /*st*/ ) override { return false; }
        virtual bool canVisitGlobalVariable ( Variable * /*fun*/ ) override { return false; }
        virtual bool canVisitEnumeration ( Enumeration * /*en*/ ) override { return false; }
        virtual bool canVisitStructureFieldInit ( Structure * /*var*/ ) override { return false; }
        virtual bool canVisitArgumentInit ( Function * /*fun*/, const VariablePtr & /*var*/, Expression * /*init*/ ) override { return false; }

    protected:
        virtual ExpressionPtr visit ( ExprIfThenElse * expr ) override {
            // if ( func && func->generator ) return Visitor::visit(expr);
            // if (cond) return x; else return y; => (cond ? x : y)
            if (expr->if_false) {
                ExprReturn * lr = nullptr; ExprReturn * rr = nullptr;
                if (expr->if_true->rtti_isBlock()) {
                    auto tb = static_cast<ExprBlock*>(expr->if_true);
                    if (tb->list.size() == 1 && tb->list.back()->rtti_isReturn()) {
                        lr = static_cast<ExprReturn*>(tb->list.back());
                        if ( lr->subexpr && lr->subexpr->rtti_isMakeLocal() ) {
                            lr = nullptr;   // we don't touch CMRES stuff
                        }
                    }
                }
                if (expr->if_false->rtti_isBlock()) {
                    auto fb = static_cast<ExprBlock*>(expr->if_false);
                    if (fb->list.size() == 1 && fb->list.back()->rtti_isReturn()) {
                        rr = static_cast<ExprReturn*>(fb->list.back());
                        if ( rr->subexpr && rr->subexpr->rtti_isMakeLocal() ) {
                            rr = nullptr;   // we don't touch CMRES stuff
                        }
                    }
                }
                if (lr && rr) {
                    if ( lr->moveSemantics != rr->moveSemantics ) {
                        lr = nullptr; // move semantics must match
                        rr = nullptr;
                    } else if ( lr->subexpr && rr->subexpr && (lr->subexpr->type->isRef() || rr->subexpr->type->isRef()) ) {
                        lr = nullptr; // ref types must match
                        rr = nullptr;
                    }
                }
                if (lr && rr) {
                    if ( lr->subexpr ) {
                        auto newCond = new ExprOp3(expr->at, "?", expr->cond, lr->subexpr, rr->subexpr);
                        newCond->type = new TypeDecl(*lr->subexpr->type);
                        auto newRet = new ExprReturn(expr->at, newCond);
                        newRet->moveSemantics = lr->moveSemantics;
                        reportFolding();
                        return newRet;
                    } else {
                        // this is actually if ( a ) return; else return;
                        reportFolding();
                        return lr;
                    }
                }
            }
            return Visitor::visit(expr);
        }
        // ExprBlock
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( func && func->generator ) return Visitor::visit(block);
            /*
            if ( cond )
                ...
                break or return or continue
            b
                =>
            if ( cond )
                ...
                break or return or continue
            else
                b
            */
            if (block->isClosure) {
                return Visitor::visit(block);
            }
            // Traverse the block in reversed order to handle all transformations
            // in a single pass. This makes it easier to apply or rewrites
            // without doing a ping-pong with optimization runner per every if
            // statement being rewritten. Instead, we'll report changes only
            // once per the entire batch of rewrites.
            // This reduces the approx run time of optimize phase over
            // a ~400 if statement function from ~0.500s to ~0.005s
            // and have ~2 optimization passes instead of ~402
            bool anyChange = false;
            for (int i = block->list.size() - 2; i >= 0; i--) {
                auto expr = block->list[i];
                if (!expr->rtti_isIfThenElse()) {
                    continue;
                }
                auto ite = static_cast<ExprIfThenElse*>(expr);
                if (ite->if_false || !ite->if_true->rtti_isBlock()) {
                    continue;
                }
                auto tb = static_cast<ExprBlock*>(ite->if_true);
                if (tb->list.size() == 0) {
                    continue;
                }
                auto lastE = tb->list.back();
                if (lastE->rtti_isReturn() || lastE->rtti_isBreak() || lastE->rtti_isContinue()) {
                    vector<ExpressionPtr> tail;
                    tail.insert(tail.begin(), block->list.begin() + i + 1, block->list.end());
                    auto fb = new ExprBlock();
                    fb->at = tail.front()->at;
                    swap(fb->list, tail);
                    ite->if_false = fb;
                    block->list.resize(i + 1);
                    anyChange = true;
                }
            }
            if (anyChange) {
                reportFolding();
            }
            return Visitor::visit(block);
        }
    };

    // marks ExprAt as noBoundCheck when the index is provably in range.
    // conservative, provable-only: relies solely on compile-time constants
    // versus compile-time fixed-array / vector dimensions (which never change).
    //  - constant index into a fixed-size array or vector
    //  - loop induction variable over a constant range, indexing a fixed-size array or vector
    class MarkBoundCheck : public Visitor {
        struct Induction {
            Variable *  var = nullptr;
            int64_t     lo = 0;
            int64_t     hi = 0;         // exclusive; used when lenOf==nullptr
            Variable *  lenOf = nullptr;// if set, upper bound is length(lenOf), exclusive
        };
        vector<Induction>   facts;
        vector<size_t>      forFacts;
    protected:
        // static fixed dimension that `index` ranges over (fixed array size, or vector dim)
        static bool fixedDim ( const TypeDeclPtr & t, int64_t & dim ) {
            if ( !t ) return false;
            if ( t->baseType==Type::tFixedArray && t->fixedDim>0 ) { dim = int64_t(t->fixedDim); return true; }
            if ( t->isVectorType() ) { dim = t->getVectorDim(); return true; }
            return false;
        }
        static bool constIntValue ( const Expression * e, int64_t & out ) {
            if ( !e->rtti_isConstant() || !e->type ) return false;
            switch ( e->type->baseType ) {
            case Type::tInt:    out = ((ExprConstInt *)e)->getValue(); return true;
            case Type::tInt8:   out = ((ExprConstInt8 *)e)->getValue(); return true;
            case Type::tInt16:  out = ((ExprConstInt16 *)e)->getValue(); return true;
            case Type::tInt64:  out = ((ExprConstInt64 *)e)->getValue(); return true;
            case Type::tUInt:   out = ((ExprConstUInt *)e)->getValue(); return true;
            case Type::tUInt8:  out = ((ExprConstUInt8 *)e)->getValue(); return true;
            case Type::tUInt16: out = ((ExprConstUInt16 *)e)->getValue(); return true;
            case Type::tUInt64: out = int64_t(((ExprConstUInt64 *)e)->getValue()); return true;
            default:            return false;
            }
        }
        // [lo,hi) of a constant range source, either folded or a range(...) call with constant args
        static bool constRange ( const Expression * src, int64_t & lo, int64_t & hi ) {
            if ( src->rtti_isConstant() && src->type && src->type->isRange() ) {
                if ( src->type->baseType==Type::tRange ) {
                    auto r = ((ExprConstRange *)src)->getValue(); lo = r.from; hi = r.to; return true;
                } else if ( src->type->baseType==Type::tRange64 ) {
                    auto r = ((ExprConstRange64 *)src)->getValue(); lo = r.from; hi = r.to; return true;
                }
                return false;
            }
            if ( src->rtti_isCall() ) {
                auto call = (ExprCall *) src;
                if ( call->name=="range" || call->name=="urange" || call->name=="range64" || call->name=="urange64" ) {
                    if ( call->arguments.size()==1 ) {
                        if ( !constIntValue(call->arguments[0], hi) ) return false;
                        lo = 0; return true;
                    } else if ( call->arguments.size()==2 ) {
                        return constIntValue(call->arguments[0], lo) && constIntValue(call->arguments[1], hi);
                    }
                }
            }
            return false;
        }
        // `length(v)` on a plain variable -> v, else nullptr
        static Variable * lengthOfVar ( const Expression * e ) {
            if ( !e->rtti_isCall() ) return nullptr;
            auto call = (ExprCall *) e;
            if ( call->name!="length" || call->arguments.size()!=1 ) return nullptr;
            auto a = call->arguments[0];
            if ( !a->rtti_isVar() ) return nullptr;
            return ((ExprVar *)a)->variable;
        }
        // range(length(v)) / range(const_lo, length(v)) -> lo>=0 and lenOf=v
        static bool symbolicLenRange ( const Expression * src, int64_t & lo, Variable * & lenOf ) {
            if ( !src->rtti_isCall() ) return false;
            auto call = (ExprCall *) src;
            if ( call->name!="range" && call->name!="urange" ) return false;
            if ( call->arguments.size()==1 ) {
                lenOf = lengthOfVar(call->arguments[0]);
                lo = 0;
                return lenOf!=nullptr;
            } else if ( call->arguments.size()==2 ) {
                lenOf = lengthOfVar(call->arguments[1]);
                return lenOf!=nullptr && constIntValue(call->arguments[0], lo);
            }
            return false;
        }
        // does subtree resize or rebind variable v ?
        // a write through v as the container of an element access (v[i] = ...) does NOT
        // change v's length, so such uses are excused; only a direct rebind (v = ...) or a
        // mutable-ref pass (resize/erase/push/...) resizes v, and those keep v out of the
        // element-address context while still carrying the write flag.
        class VarWriteScan : public Visitor {
        public:
            Variable * v = nullptr;
            bool written = false;
        protected:
            das_set<Expression *>   excused;    // ExprVar containers of element accesses
            virtual void preVisit ( ExprAt * expr ) override {
                if ( expr->subexpr && expr->subexpr->rtti_isVar() && ((ExprVar *)expr->subexpr)->variable==v )
                    excused.insert(expr->subexpr);
            }
            virtual void preVisit ( ExprVar * expr ) override {
                if ( expr->variable==v && expr->write && excused.find(expr)==excused.end() ) written = true;
            }
        };
        static bool writesToVar ( Expression * body, Variable * v ) {
            if ( !body ) return false;
            VarWriteScan scan; scan.v = v;
            body->visit(scan);
            return scan.written;
        }
        // does subtree resize / rebind ANY dynamic array? we assume everything may alias everything,
        // so any length-changing op on any array invalidates every length(...) fact. element writes
        // (v[i] = ...) do not change length and are excused.
        class ArrayMutScan : public Visitor {
        public:
            bool mutated = false;
        protected:
            das_set<Expression *>   excused;
            virtual void preVisit ( ExprAt * expr ) override {
                if ( expr->subexpr && expr->subexpr->rtti_isVar() ) excused.insert(expr->subexpr);
            }
            virtual void preVisit ( ExprVar * expr ) override {
                if ( expr->write && excused.find(expr)==excused.end()
                     && expr->variable && expr->variable->type && expr->variable->type->isGoodArrayType() )
                    mutated = true;
            }
        };
        static bool arrayMutated ( Expression * body ) {
            if ( !body ) return false;
            ArrayMutScan scan;
            body->visit(scan);
            return scan.mutated;
        }
    protected:
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            size_t pushed = 0;
            auto nsrc = expr->iteratorVariables.size()<expr->sources.size() ? expr->iteratorVariables.size() : expr->sources.size();
            for ( size_t i=0; i!=nsrc; ++i ) {
                int64_t lo, hi;
                Variable * lenOf = nullptr;
                auto var = expr->iteratorVariables[i];
                if ( !var || writesToVar(expr->body, var) ) continue;
                if ( constRange(expr->sources[i], lo, hi) ) {
                    facts.push_back({var, lo, hi, nullptr});
                    pushed ++;
                } else if ( symbolicLenRange(expr->sources[i], lo, lenOf) && !arrayMutated(expr->body) ) {
                    // length(lenOf) is stable only if NO array is resized/rebound in the body: any
                    // array may alias lenOf, so any length-changing op invalidates this fact.
                    facts.push_back({var, lo, 0, lenOf});
                    pushed ++;
                }
            }
            forFacts.push_back(pushed);
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            if ( !forFacts.empty() ) {
                size_t pushed = forFacts.back();
                forFacts.pop_back();
                while ( pushed-- ) facts.pop_back();
            }
            return Visitor::visit(expr);
        }
        virtual ExpressionPtr visit ( ExprAt * expr ) override {
            int64_t dim;
            if ( fixedDim(expr->subexpr->type, dim) ) {
                // constant index, or induction var over a constant range, into a fixed array / vector
                int64_t idxC;
                if ( constIntValue(expr->index, idxC) ) {
                    if ( idxC>=0 && idxC<dim ) expr->noBoundCheck = true;
                } else if ( expr->index->rtti_isVar() ) {
                    auto evar = (ExprVar *) expr->index;
                    for ( auto it=facts.rbegin(); it!=facts.rend(); ++it ) {
                        if ( it->var==evar->variable ) {
                            if ( it->lenOf==nullptr && it->lo>=0 && it->hi<=dim ) expr->noBoundCheck = true;
                            break;
                        }
                    }
                }
            } else if ( expr->subexpr->type && expr->subexpr->type->isGoodArrayType()
                        && expr->index->rtti_isVar() && expr->subexpr->rtti_isVar() ) {
                // dynamic array indexed by an induction var bounded by that same array's length
                auto evar = (ExprVar *) expr->index;
                auto avar = (ExprVar *) expr->subexpr;
                for ( auto it=facts.rbegin(); it!=facts.rend(); ++it ) {
                    if ( it->var==evar->variable ) {
                        if ( it->lenOf!=nullptr && it->lenOf==avar->variable && it->lo>=0 ) expr->noBoundCheck = true;
                        break;
                    }
                }
            }
            return Visitor::visit(expr);
        }
    };

    // program

    void Program::markNoBoundCheck() {
        if ( !options.getBoolOption("bound_check_elision", false) ) return;
        MarkBoundCheck context;
        visit(context);
    }

    bool Program::optimizationRefFolding(int32_t round) {
        bool any = false, anything = false;
        do {
            RefFolding context(round);
            visit(context);
            any = context.didAnything();
            anything |= any;
        } while ( any );
        return anything;
    }

    bool Program::optimizationBlockFolding(int32_t round) {
        BlockFolding context(round);
        visit(context);
        return context.didAnything();
    }

    bool Program::optimizationCondFolding(int32_t round) {
        CondFolding context(round);
        visit(context);
        return context.didAnything();
    }

}

