#include "daScript/misc/platform.h"

#include "ast_generate_typed.h"
#include "devirtualize.h"

// src/ast/ARCHITECTURE.md#devirtualization

namespace das {

    using namespace devirt;

    namespace {

        class FixedMethods {
        public:
            explicit FixedMethods ( Module * mod ) : world(mod) {
                MethodFieldWrites scan;
                scan.scan(world);
                written = das::move(scan.written);
            }
            Function * target ( ExprField * eField ) const {
                auto cls = invokedClass(eField);
                if ( !cls ) return nullptr;
                const auto & name = eField->name;
                auto fn = cls->findMethodFunction(name);
                if ( !fn || fn->arguments.empty() || !fn->result || fn->result->isAutoOrAlias() ) return nullptr;
                bool sealed = true;
                for ( auto s = cls; s && s->findField(name); s = s->parent ) sealed = sealed && s->isSealedMethod(name);
                if ( sealed ) return fn;
                auto root = familyRoot(cls, name);
                if ( root->module != world || written.find(MethodFieldWrites::family(root, name)) != written.end() ) return nullptr;
                bool fixed = true;
                world->structures.foreach([&](auto st) {
                    if ( !derivesFrom(st, root) ) return;
                    if ( !st->privateStructure ) fixed = false;
                    if ( derivesFrom(st, cls) && st->findMethodFunction(name) != fn ) fixed = false;
                });
                return fixed ? fn : nullptr;
            }
        private:
            Module * world;
            das_hash_set<string> written;
        };

        struct Devirtualize : Visitor {
            explicit Devirtualize ( const FixedMethods & f ) : fixed(f) {}
            const FixedMethods & fixed;
            bool changed = false;
            virtual ExpressionPtr visit ( ExprInvoke * expr ) override {
                auto eField = expr->arguments.size() > 1 ? expr->methodField() : nullptr;
                auto methodFn = eField ? fixed.target(eField) : nullptr;
                if ( !methodFn ) return Visitor::visit(expr);
                if ( expr->arguments[1]->rtti_isCast() ) {
                    auto selfCast = static_cast<ExprCast *>(expr->arguments[1]);
                    auto castT = new TypeDecl(*methodFn->arguments[0]->type);
                    castT->constant = selfCast->castType->constant;
                    castT->ref = false;
                    selfCast->castType = castT;
                    selfCast->type = makeStructCastType(selfCast->subexpr->type, castT);
                }
                vector<ExpressionPtr> args(expr->arguments.begin() + 1, expr->arguments.end());
                auto directCall = makeTypedCall(expr->at, methodFn, das::move(args));
                directCall->atEnclosure = expr->atEnclosure;
                changed = true;
                return directCall;
            }
        };

    }

    bool devirtualize ( Module * mod ) {
        FixedMethods fixed(mod);
        Devirtualize vis(fixed);
        mod->functions.foreach([&](auto fn) {
            if ( !fn->isTemplate && fn->body ) fn->visit(vis);
        });
        return vis.changed;
    }
}
