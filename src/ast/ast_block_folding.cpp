#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_cfg.h"

#include <set>
#include <algorithm>
#include <iterator>

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

    // ===== bound-check elision: CFG-based range facts =====
    // For each function we run a forward "must" dataflow over its CFG. A fact is `0 <= idx < BOUND`
    // where BOUND is a compile-time constant or length(arrayVar). Facts are genned by loop induction
    // (for i in range(..)) at the loop-body block and by branch guards (if (i<length(a)) ...) on the
    // taken edge; merged by INTERSECTION at joins; killed by any array mutation (any resize/rebind -
    // we assume everything may alias everything) or by reassigning the index. An ExprAt whose
    // (index, array) matches a live fact - plus a constant index provably in a fixed dim - is marked
    // noBoundCheck. Conservative and provable-only: when unsure, no fact, so the runtime check stays.
    namespace {
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
        static Variable * lengthOfVar ( const Expression * e ) {
            if ( !e->rtti_isCall() ) return nullptr;
            auto call = (ExprCall *) e;
            if ( call->name!="length" || call->arguments.size()!=1 ) return nullptr;
            auto a = call->arguments[0];
            if ( !a->rtti_isVar() ) return nullptr;
            return ((ExprVar *)a)->variable;
        }
        static bool symbolicLenRange ( const Expression * src, int64_t & lo, Variable * & lenOf ) {
            if ( !src->rtti_isCall() ) return false;
            auto call = (ExprCall *) src;
            if ( call->name!="range" && call->name!="urange" ) return false;
            if ( call->arguments.size()==1 ) {
                lenOf = lengthOfVar(call->arguments[0]); lo = 0;
                return lenOf!=nullptr;
            } else if ( call->arguments.size()==2 ) {
                lenOf = lengthOfVar(call->arguments[1]);
                return lenOf!=nullptr && constIntValue(call->arguments[0], lo);
            }
            return false;
        }
        static bool isUnsignedVar ( const Variable * v ) {
            if ( !v || !v->type ) return false;
            switch ( v->type->baseType ) {
            case Type::tUInt: case Type::tUInt8: case Type::tUInt16: case Type::tUInt64: return true;
            default: return false;
            }
        }
        static Expression * unwrap ( Expression * e ) {
            for ( ;; ) {
                if ( !e ) return e;
                if ( e->rtti_isR2V() ) { e = ((ExprRef2Value *)e)->subexpr; continue; }
                if ( e->rtti_isCast() ) { e = ((ExprCast *)e)->subexpr; continue; }
                if ( e->rtti_isCall() ) {
                    auto c = (ExprCall *)e;
                    // non-truncating numeric casts preserve a non-negative value (int8/int16 would truncate)
                    if ( c->arguments.size()==1 && (c->name=="int"||c->name=="uint"||c->name=="int64"||c->name=="uint64") ) {
                        e = c->arguments[0]; continue;
                    }
                }
                return e;
            }
        }
        static Variable * asVar ( Expression * e ) {
            e = unwrap(e);
            return (e && e->rtti_isVar()) ? ((ExprVar *)e)->variable : nullptr;
        }
        static bool boundOf ( Expression * e, bool & isLen, Variable * & lv, int64_t & cv ) {
            e = unwrap(e);
            if ( (lv = lengthOfVar(e))!=nullptr ) { isLen = true; return true; }
            if ( constIntValue(e, cv) ) { isLen = false; return true; }
            return false;
        }

        // A proven bound: 0 <= idx < (lenOf ? length(lenOf) : hi).
        struct UB {
            Variable * idx = nullptr;
            Variable * lenOf = nullptr;
            int64_t    hi = 0;
            bool operator < ( const UB & o ) const {
                if ( idx != o.idx ) return idx < o.idx;
                if ( lenOf != o.lenOf ) return lenOf < o.lenOf;
                return hi < o.hi;
            }
            bool operator == ( const UB & o ) const {
                return idx==o.idx && lenOf==o.lenOf && hi==o.hi;
            }
        };
        struct FactSet {
            std::set<UB>          ub;   // proven idx < bound
            std::set<Variable *>  nn;   // proven idx >= 0
            bool operator == ( const FactSet & o ) const { return ub==o.ub && nn==o.nn; }
        };
        static FactSet meet ( const FactSet & a, const FactSet & b ) {
            FactSet r;
            std::set_intersection(a.ub.begin(),a.ub.end(),b.ub.begin(),b.ub.end(),std::inserter(r.ub,r.ub.end()));
            std::set_intersection(a.nn.begin(),a.nn.end(),b.nn.begin(),b.nn.end(),std::inserter(r.nn,r.nn.end()));
            return r;
        }

        // vars written (rebound / mutable-ref passed) in a straight-line stmt; element containers
        // (v[i]=...) are excused - they don't change v.
        struct WriteScan : Visitor {
            das_set<Variable *> * out = nullptr;
            das_set<Expression *> excused;
            virtual void preVisit ( ExprAt * e ) override {
                if ( e->subexpr && e->subexpr->rtti_isVar() ) excused.insert(e->subexpr);
            }
            virtual void preVisit ( ExprVar * e ) override {
                if ( e->write && e->variable && excused.find(e)==excused.end() ) out->insert(e->variable);
            }
        };
        static void writtenVars ( Expression * e, das_set<Variable *> & out ) {
            if ( !e ) return;
            WriteScan s; s.out = &out; e->visit(s);
        }
        static bool arrayMutatedIn ( Expression * e ) {
            das_set<Variable *> w; writtenVars(e, w);
            for ( auto v : w ) if ( v->type && v->type->isGoodArrayType() ) return true;
            return false;
        }

        // facts genned by a for-loop's induction variables (valid throughout the loop body block)
        static void loopGen ( ExprFor * fr, FactSet & out ) {
            if ( !fr ) return;
            bool bodyMut = arrayMutatedIn(fr->body);   // a length bound is loop-carried: needs a mutation-free body
            auto n = fr->iteratorVariables.size()<fr->sources.size() ? fr->iteratorVariables.size() : fr->sources.size();
            for ( size_t i=0; i<n; ++i ) {
                auto v = fr->iteratorVariables[i];
                if ( !v ) continue;
                int64_t lo, hi; Variable * lenOf = nullptr;
                if ( constRange(fr->sources[i], lo, hi) && lo>=0 ) {
                    out.ub.insert(UB{v, nullptr, hi}); out.nn.insert(v);
                } else if ( symbolicLenRange(fr->sources[i], lo, lenOf) && lo>=0 && !bodyMut ) {
                    out.ub.insert(UB{v, lenOf, 0}); out.nn.insert(v);
                }
            }
        }

        // facts that hold on the edge where `cond` is `isTrue`
        static void condFacts ( Expression * cond, bool isTrue, FactSet & out ) {
            if ( !cond || !cond->rtti_isOp2() ) return;
            auto op2 = (ExprOp2 *) cond;
            int pred = 0;   // 1:<  2:<=  3:>  4:>=
            if ( op2->name=="<" ) pred=1; else if ( op2->name=="<=" ) pred=2;
            else if ( op2->name==">" ) pred=3; else if ( op2->name==">=" ) pred=4;
            if ( !pred ) return;
            if ( !isTrue ) pred = (pred==1)?4 : (pred==2)?3 : (pred==3)?2 : 1;   // negate on the false edge
            Expression * L = op2->left; Expression * R = op2->right;
            Variable * vL = asVar(L); Variable * vR = asVar(R);
            bool isLen; Variable * lv; int64_t cv;
            // upper bound  idx < UPPER
            if ( pred==1 && vL && boundOf(R,isLen,lv,cv) ) out.ub.insert(UB{vL, isLen?lv:nullptr, isLen?0:cv});          // v < R
            if ( pred==3 && vR && boundOf(L,isLen,lv,cv) ) out.ub.insert(UB{vR, isLen?lv:nullptr, isLen?0:cv});          // L > v => v < L
            if ( pred==2 && vL && boundOf(R,isLen,lv,cv) && !isLen ) out.ub.insert(UB{vL, nullptr, cv+1});               // v <= c => v < c+1
            if ( pred==4 && vR && boundOf(L,isLen,lv,cv) && !isLen ) out.ub.insert(UB{vR, nullptr, cv+1});               // c >= v => v < c+1
            // lower bound  idx >= 0
            if ( pred==4 && vL && boundOf(R,isLen,lv,cv) && !isLen && cv>=0 )  out.nn.insert(vL);                        // v >= c>=0
            if ( pred==2 && vR && boundOf(L,isLen,lv,cv) && !isLen && cv>=0 )  out.nn.insert(vR);                        // c<=v, c>=0
            if ( pred==3 && vL && boundOf(R,isLen,lv,cv) && !isLen && cv>=-1 ) out.nn.insert(vL);                        // v > c>=-1
            if ( pred==1 && vR && boundOf(L,isLen,lv,cv) && !isLen && cv>=-1 ) out.nn.insert(vR);                        // c<v, c>=-1
        }

        static void applyKill ( Expression * stmt, FactSet & f ) {
            das_set<Variable *> w; writtenVars(stmt, w);
            bool mutArr = false;
            for ( auto v : w ) if ( v->type && v->type->isGoodArrayType() ) { mutArr = true; break; }
            if ( mutArr ) {   // any array may alias any length bound
                for ( auto it=f.ub.begin(); it!=f.ub.end(); ) { if ( it->lenOf ) it=f.ub.erase(it); else ++it; }
            }
            for ( auto v : w ) {
                for ( auto it=f.ub.begin(); it!=f.ub.end(); ) { if ( it->idx==v ) it=f.ub.erase(it); else ++it; }
                f.nn.erase(v);
            }
        }

        // mark ExprAt nodes in a straight-line stmt against the facts holding at that point.
        // does not descend into deferred bodies (lambdas/blocks) - their facts are not this scope's.
        struct MarkScan : Visitor {
            const FactSet * f = nullptr;
            TextWriter *    logs = nullptr;   // when set, report each elided access
            const char *    fnName = "";
            int lam = 0;
            void report ( ExprAt * e, const char * why ) {
                if ( logs ) *logs << "[bound-check-elision] " << fnName << ": " << e->at.describe()
                                  << " " << e->describe() << " (" << why << ")\n";
            }
            virtual void preVisit ( ExprMakeBlock * e ) override { Visitor::preVisit(e); lam++; }
            virtual ExpressionPtr visit ( ExprMakeBlock * e ) override { lam--; return Visitor::visit(e); }
            virtual void preVisit ( ExprAt * e ) override {
                Visitor::preVisit(e);
                if ( lam || e->noBoundCheck ) return;
                int64_t dim;
                if ( fixedDim(e->subexpr->type, dim) ) {
                    int64_t c;
                    if ( constIntValue(e->index, c) ) { if ( c>=0 && c<dim ) { e->noBoundCheck = true; report(e,"const index"); } return; }
                    auto v = asVar(e->index);
                    if ( v && (isUnsignedVar(v) || f->nn.count(v)) )
                        for ( const auto & u : f->ub ) if ( u.idx==v && !u.lenOf && u.hi<=dim ) { e->noBoundCheck = true; report(e,"induction/guard in fixed dim"); break; }
                } else if ( e->subexpr->type && e->subexpr->type->isGoodArrayType() ) {
                    auto v = asVar(e->index); auto a = asVar(e->subexpr);
                    if ( v && a && (isUnsignedVar(v) || f->nn.count(v)) )
                        for ( const auto & u : f->ub ) if ( u.idx==v && u.lenOf==a ) { e->noBoundCheck = true; report(e,"index bounded by length"); break; }
                }
            }
        };

        struct AbcAnalysis {
            const Cfg & cfg;
            TextWriter * logs = nullptr;   // when set, report each elided access
            explicit AbcAnalysis ( const Cfg & c ) : cfg(c) {}

            static bool isBranch ( CfgBlock * b ) { return b->succ.size()==2 && !b->stmts.empty(); }

            FactSet transfer ( const FactSet & in, CfgBlock * b ) const {
                FactSet f = in;
                loopGen(b->loopSource, f);
                for ( auto s : b->stmts ) applyKill(s, f);
                return f;
            }
            FactSet edgeOut ( CfgBlock * from, CfgBlock * to, const FactSet & out ) const {
                FactSet r = out;
                if ( isBranch(from) ) {
                    bool isTrue = ( from->succ[0]==to );
                    bool isFalse = ( from->succ[1]==to );
                    if ( isTrue != isFalse ) condFacts(from->stmts.back(), isTrue, r);   // skip if ambiguous (both)
                }
                return r;
            }

            void run () {
                size_t n = cfg.blocks.size();
                if ( !n ) return;
                // universe of all facts that could ever be genned - the optimistic top for intersection.
                FactSet U;
                for ( auto b : cfg.blocks ) {
                    loopGen(b->loopSource, U);
                    if ( isBranch(b) ) { condFacts(b->stmts.back(), true, U); condFacts(b->stmts.back(), false, U); }
                }
                vector<FactSet> OUT(n, U);
                auto inOf = [&]( CfgBlock * b ) -> FactSet {
                    if ( b==cfg.entry || b->pred.empty() ) return FactSet();
                    FactSet in; bool first = true;
                    for ( auto p : b->pred ) {
                        FactSet eo = edgeOut(p, b, OUT[p->id]);
                        if ( first ) { in = eo; first = false; } else in = meet(in, eo);
                    }
                    return in;
                };
                bool changed = true;
                while ( changed ) {
                    changed = false;
                    for ( auto b : cfg.blocks ) {
                        FactSet out = transfer(inOf(b), b);
                        if ( !(out==OUT[b->id]) ) { OUT[b->id] = out; changed = true; }
                    }
                }
                // mark: replay the transfer per block, marking accesses against the facts at each stmt.
                for ( auto b : cfg.blocks ) {
                    FactSet cur = inOf(b);
                    loopGen(b->loopSource, cur);
                    for ( auto s : b->stmts ) {
                        applyKill(s, cur);          // kill first, then mark: an access in a mutating stmt is not elided
                        MarkScan ms; ms.f = &cur; ms.logs = logs; ms.fnName = cfg.func ? cfg.func->name.c_str() : "?";
                        s->visit(ms);
                    }
                }
            }
        };
    }

    // program

    void Program::markNoBoundCheck ( const ProgramCfg * pcfg, TextWriter & logs ) {
        if ( !options.getBoolOption("bound_check_elision", false) || !pcfg ) return;
        bool doLog = options.getBoolOption("log_bound_check_elision", false);
        thisModule->functions.foreach([&](auto & fn){
            if ( auto c = pcfg->forFunction(fn) ) {
                AbcAnalysis a(*c);
                a.logs = doLog ? &logs : nullptr;
                a.run();
            }
        });
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

