#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    void describeFunction ( TextWriter & ss, const FunctionPtr & function, const string & sectionName ) {
        ss  <<  sectionName << " : {\n"
            <<  "\"name\" : \"" << function->describe(true) << "\",\n"
            ;
        if ( function->fromGeneric ) {
            describeFunction(ss, function->fromGeneric, "\"generic\"");
        }
        ss  <<  function->atDecl.describeJson()
            <<  "},\n"
            ;
    }

    string CursorInfo::reportJson() const {
        TextWriter ss;
        ss  <<  "{\n"
            <<  "\"cursor\": {\n"
            <<   at.describeJson()
            <<  "},\n";
        if ( function ) {
            describeFunction(ss, function, "\"function\"");
        } else {
            ss  << "\"function\" : null,\n";
        }
        if ( call ) {
            ss  <<  "\"call\" : {\n";
            if ( call->rtti_isCall() ) {
                auto cexpr = static_pointer_cast<ExprCall>(call);
                ss  << "\"name\" : \"" << cexpr->name << "\",\n";
                if ( cexpr->func ) {
                    describeFunction(ss,cexpr->func, "\"function\"");
                }
            } else if ( call->rtti_isOp1() || call->rtti_isOp2() || call->rtti_isOp3() ) {
                auto cexpr = static_pointer_cast<ExprCallFunc>(call);
                ss  << "\"name\" : \"" << cexpr->name << "\",\n";
                if ( cexpr->func ) {
                    describeFunction(ss,cexpr->func, "\"function\"");
                }
            }
            ss  <<  call->at.describeJson()
                <<  "},\n"
            ;
        } else {
            ss  <<  "\"call\" : null,\n";
        }
        if ( variable ) {
            ss  <<  "\"variable\" : {\n";
            if ( variable->rtti_isVar() ) {
                auto vexpr = static_pointer_cast<ExprVar>(variable);
                ss  << "\"name\" : \"" << vexpr->name << "\"\n";
                if ( vexpr->variable ) {
                    ss  << ",\"type\" : \"" << vexpr->variable->type->describe() << "\",\n";
                    if ( vexpr->local ) {
                        ss  << "\"category\" : \"local\",\n";
                    } else if ( vexpr->pBlock ) {
                        ss  << "\"category\" : \"block argument\",\n"
                            << "\"index\" : " << vexpr->argumentIndex << ",\n";
                    } else if ( vexpr->argument ) {
                        ss  << "\"category\" : \"function argument\",\n"
                            << "\"index\" : " << vexpr->argumentIndex << ",\n";
                    } else {
                        ss  << "\"category\" : \"global\",\n";
                    }
                }
                ss << vexpr->variable->at.describeJson();
            } else if ( variable->rtti_isField() ) {
                auto fexpr = static_pointer_cast<ExprField>(variable);
                ss  << "\"name\" : \"" << fexpr->name << "\"\n";
                if ( fexpr->field ) {
                    ss  << ",\"type\" : \"" << fexpr->field->type->describe() << "\",\n"
                        << "\"category\" : \"structure field\",\n"
                        << fexpr->field->at.describeJson();
                }
            } else if ( variable->rtti_isLet() ) {
                auto lexpr = static_pointer_cast<ExprLet>(variable);
                auto lvar = lexpr->variables[variableIndex];
                ss  << "\"name\" : \"" << lvar->name << "\",\n";
                ss  << "\"type\" : \"" <<lvar->type->describe() << "\",\n"
                    << "\"category\" : \"local\",\n"
                    << lvar->at.describeJson();
            } else {
                ss << variable->at.describeJson();
            }
            ss  <<  "}\n";
        } else {
            ss  <<  "\"variable\" : null\n";
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
        bool canPointAt () const  {
            if ( function ) {
                if ( function->generated /*&& !function->generator*/ ) return false;
                // if ( function->fromGeneric )
            }
            return true;
        }
        virtual void preVisit ( Function * func ) override {
            Visitor::preVisit (func);
            function = func;
            if ( canPointAt() && cursor.inside(func->atDecl) ) {
                info.function = func;
            }
        }
        virtual FunctionPtr visit ( Function * func ) override {
            function = nullptr;
            return Visitor::visit(func);
        }
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->at) ) {
                info.call = expr;
            }
        }
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->at) ) {
                info.variable = expr;
            }
        }
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() && cursor.inside(expr->atField) ) {
                info.variable = expr;
            }
        }
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            if ( canPointAt() ) {
                int vi = 0;
                for ( const auto v : expr->variables ) {
                    if ( cursor.inside(v->at) ) {
                        info.variable = expr;
                        info.variableIndex = vi;
                    }
                    vi ++;
                }
            }
        }
    protected:
        ProgramPtr  prog;
        LineInfo    cursor;
        Function *  function = nullptr;
    public:
        CursorInfo  info;
    };

    CursorInfo Program::cursor ( const LineInfo & info ) {
        CursorVisitor vis(this, info);
        visit(vis);
        return vis.info;
    }
}
