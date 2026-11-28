#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_expressions.h"

#include "ast_effects.h"
#include "ast_generate_typed.h"
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

        struct ForwardCandidate {
            Expression *    nextStatement = nullptr;
            ExprVar *       read = nullptr;
            int             total = 0;
            bool            forwardable = false;
        };

        bool siblingsArePure ( const vector<ExpressionPtr> & operands, Expression * child ) {
            bool found = false;
            for ( auto & operand : operands ) {
                if ( operand==child ) found = true;
                else if ( !droppable(operand) ) return false;
            }
            return found;
        }

        // src/ast/ARCHITECTURE_EFFECTS.md#forward-read-once-let
        bool evaluatesAfterPureSiblings ( Expression * parent, Expression * child ) {
            if ( parent->rtti_isNullCoalescing() || parent->rtti_isSafeAt() ) return false;
            if ( parent->rtti_isR2V() ) return static_cast<ExprRef2Value *>(parent)->subexpr==child;
            if ( parent->rtti_isCast() ) return static_cast<ExprCast *>(parent)->subexpr==child;
            if ( parent->rtti_isPtr2Ref() ) return static_cast<ExprPtr2Ref *>(parent)->subexpr==child;
            if ( parent->rtti_isReturn() ) return static_cast<ExprReturn *>(parent)->subexpr==child;
            if ( parent->rtti_isOp1() ) return static_cast<ExprOp1 *>(parent)->subexpr==child;
            if ( parent->rtti_isField() ) return static_cast<ExprField *>(parent)->value==child;
            if ( parent->rtti_isOp2() ) {
                auto op = static_cast<ExprOp2 *>(parent);
                if ( op->op=="&&" || op->op=="||" ) return false;
                return siblingsArePure({op->left, op->right}, child);
            }
            if ( parent->rtti_isAt() ) {
                auto at = static_cast<ExprAt *>(parent);
                return siblingsArePure({at->subexpr, at->index}, child);
            }
            if ( parent->rtti_isCall() ) return siblingsArePure(static_cast<ExprCall *>(parent)->arguments, child);
            if ( parent->rtti_isStringBuilder() ) {
                return siblingsArePure(static_cast<ExprStringBuilder *>(parent)->elements, child);
            }
            if ( parent->rtti_isFor() ) return siblingsArePure(static_cast<ExprFor *>(parent)->sources, child);
            if ( parent->rtti_isLet() ) {
                auto let = static_cast<ExprLet *>(parent);
                return let->variables.size()==1 && let->variables[0]->init==child;
            }
            if ( parent->rtti_isIfThenElse() ) return static_cast<ExprIfThenElse *>(parent)->cond==child;
            return false;
        }

        // src/ast/ARCHITECTURE_EFFECTS.md#forward-read-once-let
        bool forwardableLet ( Expression * stmt ) {
            if ( !sinkableLet(stmt) ) return false;
            auto v = static_cast<ExprLet *>(stmt)->variables[0];
            if ( v->init_via_move || v->init_via_clone || !v->init->type || v->init->type->ref ) return false;
            return v->type->isSameType(*v->init->type, RefMatters::no, ConstMatters::no, TemporaryMatters::no);
        }

        class SinkPureLets : public PassVisitor {
        public:
            using PassVisitor::PassVisitor;
            using PassVisitor::visit;
            using PassVisitor::preVisit;
            TextWriter * logs = nullptr;
            bool forwardEnabled = true;
        protected:
            das_hash_map<Variable *, SinkCandidate> candidates;
            das_hash_map<ExprBlock *, SinkArm> arms;
            vector<SinkArm> open;
            das_hash_map<Variable *, ForwardCandidate> forwards;
            vector<Expression *> path;
            bool forwarding = false;
            virtual void preVisitExpression ( Expression * expr ) override {
                PassVisitor::preVisitExpression(expr);
                path.push_back(expr);
            }
            virtual ExpressionPtr visitExpression ( Expression * expr ) override {
                path.pop_back();
                return PassVisitor::visitExpression(expr);
            }
            // src/ast/ARCHITECTURE_EFFECTS.md#forward-read-once-let
            bool evaluatesAfterPureSiblingsFrom ( Expression * statement ) const {
                size_t depth = path.size();
                while ( depth>0 && path[depth-1]!=statement ) depth --;
                if ( depth==0 ) return false;
                for ( size_t i=depth-1; i+1<path.size(); ++i ) {
                    if ( !evaluatesAfterPureSiblings(path[i], path[i+1]) ) return false;
                }
                return true;
            }
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
                forwards.clear();
                path.clear();
                forwarding = forwardEnabled && !hasMacroOwnedShape(fun);
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
                for ( size_t i=0; forwarding && i+1<block->list.size(); ++i ) {
                    if ( !forwardableLet(block->list[i]) ) continue;
                    forwards[static_cast<ExprLet *>(block->list[i])->variables[0]].nextStatement = block->list[i+1];
                }
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
                auto fwd = forwards.find(expr->variable);
                if ( fwd!=forwards.end() ) {
                    auto & f = fwd->second;
                    f.total ++;
                    f.read = expr;
                    f.forwardable = expr->r2v && !expr->write && evaluatesAfterPureSiblingsFrom(f.nextStatement);
                }
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
            // src/ast/ARCHITECTURE_EFFECTS.md#forward-read-once-let
            bool forwardLet ( ExprBlock * block, size_t i ) {
                auto & list = block->list;
                if ( i+1>=list.size() || !list[i]->rtti_isLet() ) return false;
                auto let = static_cast<ExprLet *>(list[i]);
                if ( let->variables.size()!=1 ) return false;
                auto it = forwards.find(let->variables[0]);
                if ( it==forwards.end() ) return false;
                auto f = it->second;
                forwards.erase(it);
                if ( f.total!=1 || !f.forwardable || f.nextStatement!=list[i+1] ) return false;
                ReplaceNode replace(f.read, let->variables[0]->init);
                list[i+1] = list[i+1]->visit(replace);
                if ( !replace.done ) return false;
                if ( logs ) *logs << "FORWARD " << let->variables[0]->name << " at " << let->at.describe() << "\n";
                list.erase(list.begin() + i);
                return true;
            }
            virtual ExpressionPtr visit ( ExprBlock * block ) override {
                auto & list = block->list;
                for ( size_t i=0; i<list.size(); ++i ) {
                    auto var = pairedLet(block, i).first;
                    auto arm = var ? targetArm(var) : nullptr;
                    if ( arm ) {
                        arm->list.insert(arm->list.begin(), list[i]);
                        list.erase(list.begin() + i);
                        reportFolding();
                    } else if ( forwardLet(block, i) ) {
                        reportFolding();
                    }
                }
                if ( arms.erase(block) ) open.pop_back();
                return PassVisitor::visit(block);
            }
        };
    }

    bool optimizationSink ( Program * program, int32_t round ) {
        if ( program->options.getBoolOption("disable_sink", program->policies.disable_sink) ) return false;
        SinkPureLets pass(round);
        pass.forwardEnabled = !program->options.getBoolOption("disable_forward", program->policies.disable_forward);
        if ( program->options.getBoolOption("log_optimization", program->policies.log_optimization) ) pass.logs = program->compileLog;
        program->visit(pass);
        return pass.didAnything();
    }
}
