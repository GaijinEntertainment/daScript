#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das
{
    class FinAnnotationVisitor : public Visitor {
    public:
        FinAnnotationVisitor( const ProgramPtr & prog ) {
            program = prog;
        }
    protected:
        ProgramPtr  program;
    protected:
        virtual void preVisit ( Structure * var ) override { 
            Visitor::preVisit(var);
            for ( const auto & pA : var->annotations ) {
                if (pA->annotation->rtti_isStructureAnnotation()) {
                    auto sa = static_pointer_cast<StructureAnnotation>(pA->annotation);
                    string err = "";
                    if (!sa->look(var->shared_from_this(), *program->thisModuleGroup, pA->arguments, err)) {
                        program->error("can't 'look' with structure annotation\n" + err, var->at, CompilationError::invalid_annotation);
                    }
                }
            }
        }
        virtual void preVisit ( Function * fn ) override {
            Visitor::preVisit(fn);
            for ( const auto & an : fn->annotations ) {
                auto fna = static_pointer_cast<FunctionAnnotation>(an->annotation);
                string err = "";
                if ( !fna->finalize(fn->shared_from_this(), *program->thisModuleGroup, an->arguments, program->options, err) ) {
                    program->error("can't finalize annotation\n" + err, fn->at, CompilationError::invalid_annotation);
                }
            }
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            for ( const auto & an : block->annotations ) {
                auto fna = static_pointer_cast<FunctionAnnotation>(an->annotation);
                string err = "";
                if ( !fna->finalize(block, *program->thisModuleGroup, an->arguments, program->options, err) ) {
                    program->error("can't finalize annotation\n" + err, block->at, CompilationError::invalid_annotation);
                }
            }
        }
    };

    void Program::finalizeAnnotations() {
        FinAnnotationVisitor fin(shared_from_this());
        visit(fin);
    }
}
