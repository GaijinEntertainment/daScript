#include "daScript/misc/platform.h"

#include "devirtualize.h"
#include "ast_whole_program.h"

// src/ast/ARCHITECTURE.md#whole-program-facts

namespace das {

    using namespace devirt;

    namespace {

        class WholeProgramMethods {
        public:
            explicit WholeProgramMethods ( Program * prog ) : program(prog) {
                program->library.foreach([&](Module * mod) {
                    MethodFieldWrites scan;
                    scan.scan(mod);
                    for ( auto & family : scan.written ) written.insert(family);
                    mod->structures.foreach([&](auto st) { classes.push_back(st); });
                    return true;
                }, "*");
            }
            Function * target ( ExprField * eField ) const {
                auto cls = invokedClass(eField);
                if ( !cls ) return nullptr;
                const auto & name = eField->name;
                auto fn = cls->findMethodFunction(name);
                if ( !fn || !program->isUsed(fn) ) return nullptr;
                if ( written.find(MethodFieldWrites::family(cls, name)) != written.end() ) return nullptr;
                for ( auto st : classes ) {
                    if ( derivesFrom(st, cls) && st->findMethodFunction(name) != fn ) return nullptr;
                }
                return fn;
            }
        private:
            Program * program;
            das_hash_set<string> written;
            vector<const Structure *> classes;
        };

        struct WholeProgramCallSites : Visitor {
            WholeProgramCallSites ( const WholeProgramMethods & m, WholeProgramFacts & f ) : methods(m), facts(f) {}
            const WholeProgramMethods & methods;
            WholeProgramFacts & facts;
            virtual void preVisit ( ExprInvoke * expr ) override {
                Visitor::preVisit(expr);
                auto eField = expr->isInvokeMethod ? expr->methodField() : nullptr;
                if ( auto fn = eField ? methods.target(eField) : nullptr ) facts.devirtualizedCalls[expr] = fn;
            }
        };

        void collectDevirtualizedCalls ( Program * program, WholeProgramFacts & facts ) {
            WholeProgramMethods methods(program);
            WholeProgramCallSites sites(methods, facts);
            program->library.foreach([&](Module * mod) {
                mod->functions.foreach([&](auto fn) {
                    if ( !fn->isTemplate && fn->body && program->isUsed(fn) ) fn->visit(sites);
                });
                return true;
            }, "*");
        }

        struct WholeProgramPass {
            const char * name;
            void (* run) ( Program * program, WholeProgramFacts & facts );
        };

        const WholeProgramPass wholeProgramPasses[] = {
            { "devirtualize", collectDevirtualizedCalls },
        };

    }

    void analyzeWholeProgram ( Program * program ) {
        program->wholeProgram.reset();
        if ( !program->getOptimize() ) return;
        auto facts = make_shared<WholeProgramFacts>();
        for ( const auto & pass : wholeProgramPasses ) {
            pass.run(program, *facts);
        }
        program->wholeProgram = das::move(facts);
    }

}
