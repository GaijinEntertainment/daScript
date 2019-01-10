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
        virtual void preVisit ( Function * fn ) {
            Visitor::preVisit(fn);
            for ( const auto & an : fn->annotations ) {
                auto fna = static_pointer_cast<FunctionAnnotation>(an->annotation);
                string err = "";
                if ( !fna->finalize(fn->shared_from_this(), an->arguments, err) ) {
                    program->error("can't finalize annotation\n" + err, fn->at, CompilationError::invalid_annotation);
                }
            }
        }
        virtual void preVisit ( ExprBlock * block ) {
            Visitor::preVisit(block);
            for ( const auto & an : block->annotations ) {
                auto fna = static_pointer_cast<FunctionAnnotation>(an->annotation);
                string err = "";
                if ( !fna->finalize(block, an->arguments, err) ) {
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
