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
}
