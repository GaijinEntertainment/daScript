#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"

namespace das {

    class SetPrinterFlags : public Visitor {
    // ExprBlock
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockExpression(block, expr);
            expr->topLevel = true;
        }
    // ExprNew
        virtual void preVisitNewArg ( ExprNew * call, Expression * expr , bool last ) override {
            Visitor::preVisitNewArg(call,expr,last);
            expr->argLevel = true;
        }
    // ExprCall
        virtual void preVisitCallArg ( ExprCall * call, Expression * expr , bool last ) override {
            Visitor::preVisitCallArg(call,expr,last);
            expr->argLevel = true;
        }
    // ExprLooksLikeCall
        virtual void preVisitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * expr , bool last ) override {
            Visitor::preVisitLooksLikeCallArg(call,expr,last);
            expr->argLevel = true;
        }
    // ExprIfThenElse
        virtual void preVisit ( ExprIfThenElse * expr ) override {
            Visitor::preVisit(expr);
            expr->cond->argLevel = true;
        }
    // ExprWhile
        virtual void preVisit ( ExprWhile * expr ) override {
            Visitor::preVisit(expr);
            expr->cond->argLevel = true;
        }
    // ExprReturn
        virtual void preVisit ( ExprReturn * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->subexpr ) expr->subexpr->argLevel = true;
        }
    // ExprCopy
        virtual void preVisit ( ExprCopy * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->topLevel || expr->argLevel )
                expr->right->argLevel = true;
        }
    // ExprVar
        virtual void preVisit ( ExprVar * expr ) override {
            Visitor::preVisit(expr);
            expr->bottomLevel = true;
        }
    // ExprSizeOf
        virtual void preVisit ( ExprSizeOf * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->subexpr ) {
                expr->subexpr->argLevel = true;
            }
        }
    // ExprTypeName
        virtual void preVisit ( ExprTypeName * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->subexpr ) {
                expr->subexpr->argLevel = true;
            }
        }
    // const
        virtual ExpressionPtr visit ( ExprConst * c ) override {
            c->bottomLevel = true;
            return Visitor::visit(c);
        }
    };

    class Printer : public Visitor {
    public:
        Printer ( const Program * program = nullptr ) {
            if ( program ) {
                printRef = program->options.getOption("printRef");
                printVarAccess = program->options.getOption("printVarAccess");
            }
        }
        string str() const { return ss.str(); };
        bool printRef = true;
        bool printVarAccess = true;
    protected:
        void newLine () {
            auto nlPos = ss.tellp();
            if ( nlPos != lastNewLine ) {
                ss << "\n";
                lastNewLine = ss.tellp();
            }
        }
        __forceinline static bool noBracket ( Expression * expr ) {
            return expr->topLevel || expr->bottomLevel || expr->argLevel;
        }
    // enumeration
        virtual void preVisit ( Enumeration * enu ) override {
            Visitor::preVisit(enu);
            ss << "enum " << enu->name << "\n";
        }
        virtual void preVisitEnumerationValue ( Enumeration * enu, const string & name, int value, bool last ) override {
            Visitor::preVisitEnumerationValue(enu, name, value, last);
            ss << "\t" << name << " = " << value << "\n";
        }
    // strcuture
        virtual void preVisit ( Structure * that ) override {
            Visitor::preVisit(that);
            ss << "struct " << that->name << "\n";
        }
        virtual void preVisitStructureField ( Structure * that, Structure::FieldDeclaration & decl, bool last ) override {
            Visitor::preVisitStructureField(that, decl, last);
            ss << "\t" << decl.name << " : " << decl.type->describe() << "\n";
            if ( last ) ss << "\n";
        }
    // global
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            ss << "let\n\t";
            if ( printVarAccess && !var->access_ref ) ss << "$";
            if ( printVarAccess && !var->access_pass ) ss << "%";
            ss << var->name << " : " << var->type->describe();
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            ss << "\n\n";
            return Visitor::visitGlobalLet(var);
        }
        virtual void preVisitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            Visitor::preVisitGlobalLetInit(var, init);
            ss << " = ";
        }
    // function
        virtual void preVisit ( Function * fn) override {
            Visitor::preVisit(fn);
            if ( fn->knownSideEffects ) {
                if ( !fn->sideEffectFlags ) {
                ss << "[nosideeffects]\n";
                } else {
                    ss << "// ";
                    if ( fn->sideEffectFlags & uint32_t(SideEffects::userScenario) ) {
                        ss << "[user_scenario]";
                    }
                    if ( fn->sideEffectFlags & uint32_t(SideEffects::modifyExternal) ) {
                        ss << "[modify_external]";
                    }
                    if ( fn->sideEffectFlags & uint32_t(SideEffects::modifyArgument) ) {
                        ss << "[modify_arugment]";
                    }
                    if ( fn->sideEffectFlags & uint32_t(SideEffects::accessGlobal) ) {
                        ss << "[access_global]";
                    }
                    if ( fn->sideEffectFlags & uint32_t(SideEffects::invokeBloke) ) {
                        ss << "[invoke_block]";
                    }
                    ss << "\n";
                }
            }
            if ( fn->fastCall ) { ss << "[fastcall]\n"; }
            if ( fn->exports ) { ss << "[export]\n"; }
            ss << "def " << fn->name;
            if ( fn->arguments.size() ) ss << " ( ";
        }
        virtual void preVisitFunctionBody ( Function * fn,Expression * expr ) override {
            Visitor::preVisitFunctionBody(fn,expr);
            if ( fn->arguments.size() ) ss << " )";
            if ( fn->result && !fn->result->isVoid() ) ss << " : " << fn->result->describe();
            ss << "\n";
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & arg, bool last ) override {
            Visitor::preVisitArgument(fn,arg,last);
            if ( printVarAccess && !arg->access_ref ) ss << "$";
            if ( printVarAccess && !arg->access_pass ) ss << "%";
            ss << arg->name << ":" << arg->type->describe();
        }
        virtual void preVisitArgumentInit ( Function * fn, const VariablePtr & arg, Expression * expr ) override {
            Visitor::preVisitArgumentInit(fn,arg,expr);
            ss << " = ";
        }
        virtual VariablePtr visitArgument ( Function * fn, const VariablePtr & that, bool last ) override {
            if ( !last ) ss << "; ";
            return Visitor::visitArgument(fn, that, last);
        }
        virtual FunctionPtr visit ( Function * fn ) override {
            ss << "\n";
            return Visitor::visit(fn);
        }
        virtual void preVisit ( ExprRef2Value * expr ) override {
            Visitor::preVisit(expr);
            if ( printRef ) ss << "r2v(";
        }
        virtual ExpressionPtr visit ( ExprRef2Value * expr ) override {
            if ( printRef ) ss << ")";
            return Visitor::visit(expr);
        }
    // block
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockExpression(block, expr);
            if ( !block->isClosure ) ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockExpression ( ExprBlock * block, Expression * that ) override {
            if ( block->isClosure ) ss << ";"; else newLine();
            return Visitor::visitBlockExpression(block, that);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            if ( block->isClosure ) {
                if ( block->returnType || block->arguments.size() ) {
                    ss << "$(";
                    for ( auto & arg : block->arguments ) {
                        ss << arg->name << ":" << arg->type->describe();
                        if ( arg != block->arguments.back() )
                            ss << "; ";
                    }
                    ss << ")";
                    if ( block->returnType ) {
                        ss << ":" << block->returnType->describe();
                    }
                }
                ss << "{";
            } else {
                tab ++;
            }
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( block->isClosure ) ss << "}"; else tab --;
            return Visitor::visit(block);
        }
        virtual void preVisitBlockFinal ( ExprBlock * block ) override {
            Visitor::preVisitBlockFinal(block);
            if ( block->isClosure ) {
                ss << "} finally {";
            } else {
                ss << string(tab-1,'\t') << "finally\n";
            }
        }
        virtual void preVisitBlockFinalExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockFinalExpression(block, expr);
            if ( !block->isClosure ) ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockFinalExpression ( ExprBlock * block, Expression * that ) override {
            if ( block->isClosure ) ss << ";"; else newLine();
            return Visitor::visitBlockFinalExpression(block, that);
        }
    // string builder
        virtual void preVisit ( ExprStringBuilder * expr ) override {
            Visitor::preVisit(expr);
            ss << "string_builder(";
        }
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            if ( !last ) ss << ", ";
            return Visitor::visitStringBuilderElement(sb, expr, last);
        }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // let
        virtual void preVisit ( ExprLet * let ) override {
            Visitor::preVisit(let);
            if ( let->subexpr ) {
                ss << "let (";
            } else {
                ss << "let ";
                if ( let->inScope ) ss << "in scope ";
            }
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            if ( printVarAccess && !var->access_ref ) ss << "$";
            if ( printVarAccess && !var->access_pass ) ss << "%";
            ss << var->name << ":" << var->type->describe();
        }
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !last ) ss << "; ";
            if ( last && let->subexpr ) ss << ")\n";
            return Visitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( ExprLet * let, const VariablePtr & var, Expression * expr ) override {
            Visitor::preVisitLetInit(let,var,expr);
            ss << " = ";
        }
    // if then else
        virtual void preVisit ( ExprIfThenElse * ifte ) override {
            Visitor::preVisit(ifte);
            ss << "if ";
        }
        virtual void preVisitIfBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitIfBlock(ifte,block);
            ss << "\n";
        }
        virtual void preVisitElseBlock ( ExprIfThenElse * ifte, Expression * block ) override {
            Visitor::preVisitElseBlock(ifte, block);
            ss << string(tab,'\t') << "else\n";
        }
    // try-catch
        virtual void preVisit ( ExprTryCatch * tc ) override {
            Visitor::preVisit(tc);
            ss << "try\n";
        }
        virtual void preVisitCatch ( ExprTryCatch * tc, Expression * block ) override {
            Visitor::preVisitCatch(tc, block);
            ss << string(tab,'\t') << "catch\n";
        }
    // for
        virtual void preVisit ( ExprFor * ffor ) override {
            Visitor::preVisit(ffor);
            ss << "for ";
        }
        virtual void preVisitFor ( ExprFor * ffor, const VariablePtr & var, bool last ) override {
            Visitor::preVisitFor(ffor,var,last);
            ss << var->name;
            if ( !last ) ss << ","; else ss << " in ";
        }
        virtual void preVisitForBody ( ExprFor * ffor, Expression * body ) override {
            Visitor::preVisitForBody(ffor, body);
            ss << "\n";
        }
        virtual ExpressionPtr visitForSource ( ExprFor * ffor, Expression * that , bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitForSource(ffor, that, last);
        }
    // while
        virtual void preVisit ( ExprWhile * wh ) override {
            Visitor::preVisit(wh);
            ss << "while ";
        }
        virtual void preVisitWhileBody ( ExprWhile * wh, Expression * body ) override {
            Visitor::preVisitWhileBody(wh,body);
            ss << "\n";
        }
    // call
        virtual void preVisit ( ExprCall * call ) override {
            Visitor::preVisit(call);
            ss << call->name << "(";
        }
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprCall * c ) override {
            ss << ")";
            return Visitor::visit(c);
        }
    // looks like call
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            ss << call->name << "(";
        }
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprLooksLikeCall * c ) override {
            ss << ")";
            return Visitor::visit(c);
        }
    // const
        virtual ExpressionPtr visit(ExprFakeContext * c) override {
            ss << "__context__";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstPtr * c ) override {
            if ( c->getValue() ) {
                ss << "*0x" << HEX << intptr_t(c->getValue()) << DEC;
            } else {
                ss << "null";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstEnumeration * c ) override {
            auto value = c->getValue();
            ss << c->enumType->name << " " << c->enumType->find(value,to_string(value));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt * c ) override {
            ss << c->getValue();
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt64 * c ) override {
            ss << c->getValue();
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt64 * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstBool * c ) override {
            ss << (c->getValue() ? "true" : "false");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstDouble * c ) override {
            ss << c->getValue() << "lf";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat * c ) override {
            ss << c->getValue() << "f";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstString * c ) override {
            ss << "\"" << escapeString(c->text) << "\"";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt2 * c ) override {
            auto val = c->getValue();
            ss << "int2(" << val.x << "," << val.y << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt3 * c ) override {
            auto val = c->getValue();
            ss << "int3(" << val.x << "," << val.y << "," << val.z << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt4 * c ) override {
            auto val = c->getValue();
            ss << "int4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt2 * c ) override {
            auto val = c->getValue();
            ss << "uint2(" << val.x << "," << val.y << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt3 * c ) override {
            auto val = c->getValue();
            ss << "uint3(" << val.x << "," << val.y << "," << val.z << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt4 * c ) override {
            auto val = c->getValue();
            ss << "uint4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat2 * c ) override {
            auto val = c->getValue();
            ss << "float2(" << val.x << "f," << val.y << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            ss << "float3(" << val.x << "f," << val.y << "f," << val.z << "f)";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            ss << "float4(" << val.x << "f," << val.y << "f," << val.z << "f," << val.w << "f)";
            return Visitor::visit(c);
        }
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            if ( printRef && var->r2v ) ss << "@";
            if ( printRef && var->r2cr ) ss << "$";
            if ( printRef && var->write ) ss << "#";
            ss << var->name;
            return Visitor::visit(var);
        }
    // swizzle
        virtual ExpressionPtr visit ( ExprSwizzle * expr ) override {
            ss << ".";
            if ( printRef && expr->r2v ) ss << "@";
            if ( printRef && expr->r2cr ) ss << "$";
            if ( printRef && expr->write ) ss << "#";
            for ( auto f : expr->fields ) {
                switch ( f ) {
                    case 0:     ss << "x"; break;
                    case 1:     ss << "y"; break;
                    case 2:     ss << "z"; break;
                    case 3:     ss << "w"; break;
                    default:    ss << "?"; break;
                }
            }
            return Visitor::visit(expr);
        }
    // field
        virtual ExpressionPtr visit ( ExprField * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << "." << field->name;
            return Visitor::visit(field);
        }
        virtual ExpressionPtr visit ( ExprSafeField * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << ".?" << field->name;
            return Visitor::visit(field);
        }
    // ptr2ref
        virtual void preVisit ( ExprPtr2Ref * ptr2ref ) override {
            Visitor::preVisit(ptr2ref);
            ss << "deref(";
        }
        virtual ExpressionPtr visit ( ExprPtr2Ref * ptr2ref ) override {
            ss << ")";
            return Visitor::visit(ptr2ref);
        }
    // delete
        virtual void preVisit ( ExprDelete * edel ) override {
            Visitor::preVisit(edel);
            ss << "delete ";
        }
    // new
        virtual void preVisit ( ExprNew * enew ) override {
            Visitor::preVisit(enew);
            ss << "new " << enew->typeexpr->describe();
            if ( enew->initializer ) ss << "(";
        }
        virtual ExpressionPtr visitNewArg ( ExprNew * call, Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitNewArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprNew * enew ) override {
            if ( enew->initializer ) ss << ")";
            return Visitor::visit(enew);
        }
    // null coaelescing
        virtual void preVisitNullCoaelescingDefault ( ExprNullCoalescing * nc, Expression * expr ) override {
            Visitor::preVisitNullCoaelescingDefault(nc,expr);
            ss << " ?? ";
        }
    // at
        virtual void preVisitAtIndex ( ExprAt * expr, Expression * index ) override {
            Visitor::preVisitAtIndex(expr, index);
            if ( printRef && expr->r2v ) ss << "@";
            if ( printRef && expr->r2cr ) ss << "$";
            if ( printRef && expr->write ) ss << "#";
            ss << "[";

        }
        virtual ExpressionPtr visit ( ExprAt * that ) override {
            ss << "]";
            return Visitor::visit(that);
        }
    // op1
        virtual void preVisit ( ExprOp1 * that ) override {
            Visitor::preVisit(that);
            if ( that->op!="+++" && that->op!="---" ) {
                ss << that->op;
            }
            if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << "(";
        }
        virtual ExpressionPtr visit ( ExprOp1 * that ) override {
            if ( that->op=="+++" || that->op=="---" ) {
                ss << that->op[0] << that->op[1];
            }
            if ( !noBracket(that) && !that->subexpr->bottomLevel ) ss << ")";
            return Visitor::visit(that);
        }
    // op2
        virtual void preVisit ( ExprOp2 * that ) override {
            Visitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitRight ( ExprOp2 * op2, Expression * right ) override {
            Visitor::preVisitRight(op2,right);
            ss << " " << op2->op << " ";
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return Visitor::visit(that);
        }
    // op3
        virtual void preVisit ( ExprOp3 * that ) override {
            Visitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitLeft  ( ExprOp3 * that, Expression * left ) override {
            Visitor::preVisitLeft(that,left);
            ss << " ? ";
        }
        virtual void preVisitRight ( ExprOp3 * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << " : ";
        }
        virtual ExpressionPtr visit ( ExprOp3 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return Visitor::visit(that);
        }
    // copy & move
        virtual void preVisitRight ( ExprCopy * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << " = ";
        }
        virtual void preVisitRight ( ExprMove * that, Expression * right ) override {
            Visitor::preVisitRight(that,right);
            ss << " <- ";
        }
    // return
        virtual void preVisit ( ExprReturn * expr ) override {
            Visitor::preVisit(expr);
            ss << "return ";
            if ( expr->moveSemantics ) ss << "<- ";
        }
    // break
        virtual void preVisit ( ExprBreak * that ) override {
            Visitor::preVisit(that);
            ss << "break";
        }
    // sizeof
        virtual void preVisit ( ExprSizeOf * expr ) override {
            Visitor::preVisit(expr);
            ss << "sizeof(";
            if ( !expr->subexpr ) {
                ss << "type " << *expr->typeexpr;
            }
        }
        virtual ExpressionPtr visit ( ExprSizeOf * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // typename
        virtual void preVisit ( ExprTypeName * expr ) override {
            Visitor::preVisit(expr);
            ss << "typename(";
            if ( !expr->subexpr ) {
                ss << "type " << *expr->typeexpr;
            }
        }
        virtual ExpressionPtr visit ( ExprTypeName * expr ) override {
            ss << ")";
            return Visitor::visit(expr);
        }
    // make structure
        virtual void preVisit ( ExprMakeStructure * expr ) override {
            Visitor::preVisit(expr);
            ss << "[[" << expr->type->describe() << " ";
        }
        virtual void preVisitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool last ) override {
            Visitor::preVisitMakeStructureField(expr,index,decl,last);
            ss << decl->name << " = ";
        }
        virtual MakeFieldDeclPtr visitMakeStructureField ( ExprMakeStructure * expr, int index, MakeFieldDecl * decl, bool last ) override {
            if ( !last ) ss << ", ";
            return Visitor::visitMakeStructureField(expr,index,decl,last);
        }
        virtual void visitMakeStructureIndex ( ExprMakeStructure * expr, int index, bool lastField ) override {
            if ( !lastField ) ss << "; ";
            Visitor::visitMakeStructureIndex(expr, index, lastField);
        }
        virtual ExpressionPtr visit ( ExprMakeStructure * expr ) override {
            ss << "]]";
            return Visitor::visit(expr);
        }
    // make array
        virtual void preVisit ( ExprMakeArray * expr ) override {
            Visitor::preVisit(expr);
            ss << "[[" << expr->type->describe() << " ";
        }
        virtual ExpressionPtr visitMakeArrayIndex ( ExprMakeArray * expr, int index, Expression * init, bool lastField ) override {
            if ( !lastField ) ss << "; ";
            return Visitor::visitMakeArrayIndex(expr, index, init, lastField);
        }
        virtual ExpressionPtr visit ( ExprMakeArray * expr ) override {
            ss << "]]";
            return Visitor::visit(expr);
        }
    protected:
        TextWriter			ss;
        int					lastNewLine = -1;
        int                 tab = 0;
    };

    template <typename TT>
    __forceinline TextWriter&  print ( TextWriter& stream, const TT & value ) {
        SetPrinterFlags flags;
        const_cast<TT&>(value).visit(flags);
        Printer log(nullptr);
        const_cast<TT&>(value).visit(log);
        stream << log.str();
        return stream;
    }

    TextWriter& operator<< (TextWriter& stream, const Program & program) {
        bool logGenerics = program.options.getOption("logGenerics");
        SetPrinterFlags flags;
        const_cast<Program&>(program).visit(flags, logGenerics);
        Printer log(&program);
        const_cast<Program&>(program).visit(log, logGenerics);
        stream << log.str();
        return stream;
    }

    TextWriter& operator<< (TextWriter& stream, const Expression & expr) {
        return print(stream,expr);
    }

    TextWriter& operator<< (TextWriter& stream, const Function & func) {
        return print(stream,func);
    }
}
