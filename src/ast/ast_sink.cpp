#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_expressions.h"

#include "ast_effects.h"
#include "ast_sink.h"

namespace das {

    // src/ast/ARCHITECTURE_EFFECTS.md#sink-pure-let

    namespace {

        struct SinkCandidate {
            ExprBlock *     ifTrue = nullptr;
            ExprBlock *     ifFalse = nullptr;
            int             total = 0;
            int             inTrue = 0;
            int             inFalse = 0;
        };

        struct SinkArm {
            Variable *  var = nullptr;
            bool        isTrue = false;
        };

        bool sinkableLet ( Expression * stmt ) {
            if ( !stmt->rtti_isLet() ) return false;
            auto let = static_cast<ExprLet *>(stmt);
            if ( let->variables.size()!=1 ) return false;
            auto v = let->variables[0];
            if ( !v->init || !v->type || v->type->ref ) return false;
            if ( v->inScope || v->pod_delete ) return false;
            if ( v->init_via_move && (v->init->type==nullptr || v->init->type->isRef()) ) return false;
            return droppable(v->init);
        }

        ExprBlock * armBlock ( Expression * e ) {
            return ( e && e->rtti_isBlock() ) ? static_cast<ExprBlock *>(e) : nullptr;
        }

        class SinkPureLets : public PassVisitor {
        public:
            using PassVisitor::PassVisitor;
            using PassVisitor::visit;
            using PassVisitor::preVisit;
        protected:
            das_hash_map<Variable *, SinkCandidate> candidates;
            das_hash_map<ExprBlock *, SinkArm> arms;
            vector<SinkArm> open;
            virtual bool canVisitStructure ( Structure * ) override { return false; }
            virtual bool canVisitStructureFieldInit ( Structure * ) override { return false; }
            virtual bool canVisitArgumentInit ( Function *, const VariablePtr &, Expression * ) override { return false; }
            virtual bool canVisitQuoteSubexpression ( ExprQuote * ) override { return false; }
            virtual bool canVisitGlobalVariable ( Variable * ) override { return false; }
            virtual bool canVisitEnumeration ( Enumeration * ) override { return false; }
            virtual void preVisit ( Function * fun ) override {
                PassVisitor::preVisit(fun);
                candidates.clear();
                arms.clear();
                open.clear();
            }
            static pair<Variable *, ExprIfThenElse *> pairedLet ( ExprBlock * block, size_t i ) {
                auto & list = block->list;
                if ( i+1>=list.size() || !sinkableLet(list[i]) || !list[i+1]->rtti_isIfThenElse() ) return {nullptr, nullptr};
                return {static_cast<ExprLet *>(list[i])->variables[0], static_cast<ExprIfThenElse *>(list[i+1])};
            }
            virtual void preVisit ( ExprBlock * block ) override {
                PassVisitor::preVisit(block);
                auto arm = arms.find(block);
                if ( arm!=arms.end() ) open.push_back(arm->second);
                for ( size_t i=0; i<block->list.size(); ++i ) {
                    auto [var, ite] = pairedLet(block, i);
                    if ( !var || ite->isStatic || !droppable(ite->cond) ) continue;
                    auto & c = candidates[var];
                    c.ifTrue = armBlock(ite->if_true);
                    c.ifFalse = armBlock(ite->if_false);
                    if ( c.ifTrue ) arms[c.ifTrue] = SinkArm{var, true};
                    if ( c.ifFalse ) arms[c.ifFalse] = SinkArm{var, false};
                }
            }
            virtual void preVisit ( ExprVar * expr ) override {
                PassVisitor::preVisit(expr);
                auto it = candidates.find(expr->variable);
                if ( it==candidates.end() ) return;
                auto & c = it->second;
                c.total ++;
                for ( auto & a : open ) {
                    if ( a.var!=expr->variable ) continue;
                    if ( a.isTrue ) c.inTrue ++;
                    else c.inFalse ++;
                }
            }
            ExprBlock * targetArm ( Variable * v ) {
                auto it = candidates.find(v);
                if ( it==candidates.end() ) return nullptr;
                auto c = it->second;
                candidates.erase(it);
                if ( c.total==c.inTrue && c.inTrue ) return c.ifTrue;
                if ( c.total==c.inFalse && c.inFalse ) return c.ifFalse;
                return nullptr;
            }
            virtual ExpressionPtr visit ( ExprBlock * block ) override {
                auto & list = block->list;
                for ( size_t i=0; i<list.size(); ++i ) {
                    auto var = pairedLet(block, i).first;
                    if ( !var ) continue;
                    auto arm = targetArm(var);
                    if ( !arm ) continue;
                    arm->list.insert(arm->list.begin(), list[i]);
                    list.erase(list.begin() + i);
                    reportFolding();
                }
                if ( arms.erase(block) ) open.pop_back();
                return PassVisitor::visit(block);
            }
        };
    }

    bool optimizationSink ( Program * program, int32_t round ) {
        if ( program->options.getBoolOption("disable_sink", program->policies.disable_sink) ) return false;
        SinkPureLets pass(round);
        program->visit(pass);
        return pass.didAnything();
    }
}
