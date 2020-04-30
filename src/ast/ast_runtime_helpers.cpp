#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    void describeFunction ( TextWriter & ss, const FunctionPtr & function, const string & sectionName ) {
        ss  <<  sectionName << " : {\n"
            <<  "name : \"" << function->describe(true) << "\",\n"
            ;
        if ( function->fromGeneric ) {
            describeFunction(ss, function->fromGeneric, "generic");
        }
        ss  <<  function->atDecl.describeJson()
            <<  "},\n"
            ;
    }

    string CursorInfo::reportJson() const {
        TextWriter ss;
        ss  <<  "{\n"
            <<  "cursor: {\n"
            <<   at.describeJson()
            <<  "},\n";
        if ( function ) {
            describeFunction(ss, function, "function");
        } else {
            ss  << "function : null,\n";
        }
        if ( call ) {
            ss  <<  "call : {\n";
            if ( call->rtti_isCall() ) {
                auto cexpr = static_pointer_cast<ExprCall>(call);
                ss  << "name : \"" << cexpr->name << "\",\n";
                if ( cexpr->func ) {
                    describeFunction(ss,cexpr->func, "function");
                }
            } else if ( call->rtti_isOp1() || call->rtti_isOp2() || call->rtti_isOp3() ) {
                auto cexpr = static_pointer_cast<ExprCallFunc>(call);
                ss  << "name : \"" << cexpr->name << "\",\n";
                if ( cexpr->func ) {
                    describeFunction(ss,cexpr->func, "function");
                }
            }
            ss  <<  call->at.describeJson()
                <<  "}\n"
            ;
        } else {
            ss  <<  "call : null\n";
        }
        ss  << "}\n";
        return ss.str();
    }

    class CursorVisitor : public Visitor {
    public:
        CursorVisitor(const ProgramPtr & p, const LineInfo & i ) {
            prog = p;
            cursor = i;
            info.at = i;
        }
    protected:
        virtual void preVisit ( Function * func ) override {
            Visitor::preVisit (func);
            if ( cursor.inside(func->atDecl) ) {
                info.function = func;
            }
        }
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            if ( cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            if ( cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            if ( cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
    protected:
        ProgramPtr  prog;
        LineInfo    cursor;
    public:
        CursorInfo  info;
    };

    CursorInfo Program::cursor ( const LineInfo & info ) {
        CursorVisitor vis(this, info);
        visit(vis);
        return vis.info;
    }
}
