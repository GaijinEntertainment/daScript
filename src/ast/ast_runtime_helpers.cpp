#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    void describeFunction ( TextWriter & ss, const FunctionPtr & function, const string & sectionName ) {
        if ( !sectionName.empty() ) ss << sectionName << ":";
        ss  << "{\n\"name\":\"" << function->describe(Function::DescribeModule::yes,Function::DescribeExtra::yes) << "\",\n"
            << "\"shortname\":\"" << function->describeName(Function::DescribeModule::yes) << "\",\n";
        if ( function->fromGeneric ) {
            describeFunction(ss, function->fromGeneric, "\"generic\"");
            ss << ",\n";
        }
        ss << function->atDecl.describeJson() <<  "}\n";
    }

    void describeConstant ( TextWriter & ss, const ExpressionPtr & expr, Function * func ) {
        TextWriter tw;
        tw << *expr;
        ss << "{\n\"value\":\"" << escapeString(tw.str()) << "\",\n";
        if ( func ) {
            describeFunction(ss, func, "\"function\"");
        } else {
            ss << "\"function:\"null";
        }
        ss << "}\n";
    }

    void describeVariable ( TextWriter & ss, const ExpressionPtr & variable, int variableIndex, Function * func, const string & sectionName ) {
        if ( !sectionName.empty() ) ss  <<  sectionName << ":";
        ss  << "{\n";
        if ( variable->rtti_isVar() ) {
            auto vexpr = static_pointer_cast<ExprVar>(variable);
            ss  << "\"name\":\"" << vexpr->name << "\",\n";
            if ( vexpr->variable ) {
                ss  << "\"type\":\"" << vexpr->variable->type->describe() << "\",\n";
                if ( vexpr->local ) {
                    ss  << "\"category\":\"local\",\n";
                } else if ( vexpr->pBlock ) {
                    ss  << "\"category\":\"block argument\",\n"
                        << "\"index\":" << vexpr->argumentIndex << ",\n";
                } else if ( vexpr->argument ) {
                    ss  << "\"category\":\"function argument\",\n"
                        << "\"index\":" << vexpr->argumentIndex << ",\n";
                } else {
                    ss  << "\"category\":\"global\",\n";
                }
                if ( func && (vexpr->local || vexpr->pBlock || vexpr->argument) ) {
                     describeFunction(ss, func, "\"function\"");
                     ss << ",\n";
                }
                if ( vexpr->variable->init ) {
                    TextWriter tw;
                    tw << *(vexpr->variable->init);
                    ss << "\"init\":\"" << escapeString(tw.str()) << "\",";
                }
                ss << vexpr->variable->at.describeJson();
            } else {
                ss << "\"category\":\"uncategorized variable\"\n";
            }
        } else if ( variable->rtti_isField() || variable->rtti_isSafeField() ) {
            auto fexpr = static_pointer_cast<ExprField>(variable);
            ss  << "\"name\":\"" << fexpr->name << "\",\n";
            if ( fexpr->field ) {
                ss  << "\"type\":\"" << fexpr->field->type->describe() << "\",\n";
                if ( fexpr->field->init) {
                    TextWriter tw;
                    tw << *(fexpr->field->init);
                    ss << "\"init\":\"" << escapeString(tw.str()) << "\",";
                }
                ss  << fexpr->field->at.describeJson() << ",\n";
            }
            TypeDeclPtr categoryType;
            if ( fexpr->value->type ) {
                if ( fexpr->value->type->isPointer() ) {
                    categoryType = fexpr->value->type->firstType;
                } else {
                    categoryType = fexpr->value->type;
                }
            }
            if ( !categoryType ) {
                ss << "\"category\":\"uncategorized field or swizzle\",\n";
            } else if ( categoryType->isStructure() ) {
                if ( categoryType->structType && categoryType->structType->generated ) {
                    ss << "\"category\":\"lambda or generator capture\",\n";
                } else {
                    ss << "\"category\":\"structure field\",\n";
                }
            } else if ( categoryType->isHandle() ) {
                ss << "\"category\":\"annotation field or property\",\n";
            } else if ( categoryType->isTuple() ) {
                ss << "\"category\":\"tuple field\",\n";
            } else if ( categoryType->isVariant() ) {
                ss << "\"category\":\"variant field\",\n";
            } else if ( categoryType->isVectorType() ) {
                ss << "\"category\":\"vector field lookup or swizzle\",\n";
            } else {
                ss << "\"category\":\"unknown field lookup\",\n";
            }
            if ( func ) {
                describeFunction(ss, func, "\"function\"");
            } else {
                ss << "\"function:\"null";
            }
        } else if ( variable->rtti_isLet() ) {
            auto lexpr = static_pointer_cast<ExprLet>(variable);
            auto lvar = lexpr->variables[variableIndex];
            ss  << "\"name\":\"" << lvar->name << "\",\n";
            ss  << "\"type\":\"" <<lvar->type->describe() << "\",\n"
                << "\"category\":\"local\",\n";
            if ( func ) {
                describeFunction(ss, func, "\"function\"");
                ss << ",\n";
            }
            if ( lvar->init) {
                TextWriter tw;
                tw << *(lvar->init);
                ss << "\"init\":\"" << escapeString(tw.str()) << "\",";
            }
            ss  << lvar->at.describeJson();
        } else {
            ss << variable->at.describeJson();
        }
        ss  <<  "}\n";
    }

   void describeCall ( TextWriter & ss, const ExpressionPtr & call, const string & sectionName ) {
        if ( !sectionName.empty() ) ss << sectionName << ":";
        ss  << "{\n";
        if ( call->rtti_isCall() ) {
            auto cexpr = static_pointer_cast<ExprCall>(call);
            ss  << "\"name\":\"" << cexpr->name << "\",\n";
            if ( cexpr->func ) {
                describeFunction(ss,cexpr->func, "\"function\"");
                ss << ",";
            }
        } else if ( call->rtti_isOp1() || call->rtti_isOp2() || call->rtti_isOp3() ) {
            auto cexpr = static_pointer_cast<ExprCallFunc>(call);
            ss  << "\"name\":\"" << cexpr->name << "\",\n";
            if ( cexpr->func ) {
                describeFunction(ss,cexpr->func, "\"function\"");
                ss << ",";
            }
        }
        ss  <<  call->at.describeJson() << "}\n";
    }

    string CursorInfo::reportJson() const {
        TextWriter ss;
        ss  <<  "{\n"
            <<  "\"cursor\": {\n"
            <<   at.describeJson()
            <<  "},\n";
        if ( !function.empty() ) {
            describeFunction(ss, function.back(), "\"function\"");
            ss << ",\n\"functions\":[\n";
            for ( size_t idx=0; idx!=function.size(); ++idx) {
                if ( idx!=0 ) ss << ",\n";
                describeFunction(ss, function[idx], "");
            }
            ss << "],\n";
        } else {
            ss  << "\"function\":null,\n";
        }
        if ( !call.empty() ) {
            describeCall(ss, call.back(), "\"call\"");
            ss << ",\n\"calls\":[\n";
            for ( size_t idx=0; idx!=call.size(); ++idx) {
                if ( idx!=0 ) ss << ",\n";
                describeCall(ss, call[idx], "");
            }
            ss << "],\n";
        } else {
            ss  <<  "\"call\":null,\n";
        }
        if ( !variable.empty() ) {
            const auto & vvar = variable.back();
            describeVariable(ss, vvar.expr, vvar.index, vvar.function, "\"variable\"");
            ss << ",\n\"variables\":[\n";
            for ( size_t idx=0; idx!=variable.size(); ++idx) {
                if ( idx!=0 ) ss << ",\n";
                const auto & ivar = variable[idx];
                describeVariable(ss, ivar.expr, ivar.index, ivar.function, "");
            }
            ss << "],\n";
        } else {
            ss  <<  "\"variable\":null,\n";
        }
        if ( !constants.empty() ) {
            ss << "\"constants\":[\n";
            for ( size_t idx=0; idx!=constants.size(); ++idx) {
                if ( idx!=0 ) ss << ",\n";
                describeConstant(ss, constants[idx].expr, constants[idx].function);
            }
            ss << "]";
        } else {
            ss  <<  "\"constants\":null\n";
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
                if ( function->generated  ) {
                    if ( !function->generator && !function->lambda) {
                        return false;
                    }
                }
            }
            return true;
        }
        virtual void preVisit ( Function * func ) override {
            Visitor::preVisit (func);
            function = func;
            if ( canPointAt() && cursor.inside(func->atDecl) ) {
                info.function.push_back(func);
            }
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitArgument(fn, var, lastArg);
            if ( !var->generated && canPointAt() && cursor.inside(var->at) ) {
                int index = -1;
                for ( int t=0; t!=int(fn->arguments.size()); t++ ) {
                    if ( fn->arguments[t]==var ) {
                        index = t;
                        break;
                    }
                }
                auto expr = make_smart<ExprVar>(var->at, var->name);
                expr->variable = var;
                expr->argumentIndex = index;
                expr->argument =  true;
                expr->type = make_smart<TypeDecl>(*var->type);
                info.variable.emplace_back(expr.get(),index,function);
            }
        }
        virtual void preVisitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitBlockArgument(block, var, lastArg);
            if ( !var->generated && canPointAt() && cursor.inside(var->at) ) {
                int index = -1;
                for ( int t=0; t!=int(block->arguments.size()); t++ ) {
                    if ( block->arguments[t]==var ) {
                        index = t;
                        break;
                    }
                }
                auto expr = make_smart<ExprVar>(var->at, var->name);
                expr->variable = var;
                expr->argumentIndex = index;
                expr->argument =  true;
                expr->type = make_smart<TypeDecl>(*var->type);
                info.variable.emplace_back(expr.get(),index,function);
            }
        }
        virtual FunctionPtr visit ( Function * func ) override {
            function = nullptr;
            return Visitor::visit(func);
        }
        virtual void preVisit ( ExprCall * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->at) ) {
                info.call.push_back(expr);
            }
        }
        virtual void preVisit ( ExprOp1 * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->at) ) {
                info.call.push_back(expr);
            }
        }
        virtual void preVisit ( ExprOp2 * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->at) ) {
                info.call.push_back(expr);
            }
        }
        virtual void preVisit ( ExprOp3 * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->at) ) {
                info.call.push_back(expr);
            }
        }
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->at) ) {
                if ( !expr->variable || !expr->variable->generated ) {
                    info.variable.emplace_back(expr,-1,function);
                }
            }
        }
        virtual void preVisit ( ExprField * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->atField) ) {
                if ( !expr->field || !expr->field->generated ) {
                    info.variable.emplace_back(expr,-1,function);
                }
            }
        }
        virtual void preVisit ( ExprSafeField * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && cursor.inside(expr->atField) ) {
                if ( !expr->field || !expr->field->generated ) {
                    info.variable.emplace_back(expr,-1,function);
                }
            }
        }
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() ) {
                int vi = 0;
                for ( const auto & v : expr->variables ) {
                    if ( cursor.inside(v->at) && !v->generated ) {
                        info.variable.emplace_back(expr,vi,function);
                    }
                    vi ++;
                }
            }
        }
        virtual void preVisit ( ExprConst * expr ) override {
            Visitor::preVisit(expr);
            if ( !expr->generated && canPointAt() && expr->baseType!=Type::fakeContext && expr->baseType!=Type::fakeLineInfo ) {
                if ( cursor.inside(expr->at) ) {
                    info.constants.emplace_back(expr,function);
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
        visit(vis, true);   // note - we visit generics
        return vis.info;
    }
}
