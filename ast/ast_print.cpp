#include "precomp.h"

#include "ast.h"

namespace yzg {

    class Printer : public Visitor {
    public:
        string str() const { return ss.str(); };
    protected:
        void newLine () {
            auto nlPos = ss.tellp();
            if ( nlPos != lastNewLine ) {
                ss << "\n";
                lastNewLine = ss.tellp();
            }
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
            ss << "let\n\t" << var->name << " : " << var->type->describe();
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
    // block
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockExpression(block, expr);
            if ( !block->returnsValue ) ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockExpression ( ExprBlock * block, Expression * that ) override {
            if ( block->returnsValue ) ss << ";"; else newLine();
            return Visitor::visitBlockExpression(block, that);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            if ( block->returnsValue ) ss << "{"; else tab ++;
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( block->returnsValue ) ss << "}"; else tab --;
            return Visitor::visit(block);
        }
    // let
        virtual void preVisit ( ExprLet * let ) override {
            Visitor::preVisit(let);
            ss << (let->subexpr ? "let (" : "let ");
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
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
        virtual void preVisitForFilter ( ExprFor * ffor, Expression * filter ) override {
            Visitor::preVisitForFilter(ffor,filter);
            ss << " where ";
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
        virtual ExpressionPtr visit ( ExprConstPtr * c ) override {
            if ( c->value ) {
                ss << "*0x" << hex << intptr_t(c->value) << dec;
            } else {
                ss << "null";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt * c ) override {
            ss << c->value;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt * c ) override {
            ss << "0x" << hex << c->value << dec;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstBool * c ) override {
            ss << (c->value ? "true" : "false");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat * c ) override {
            ss << c->value;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstString * c ) override {
            ss << "\"" << escapeString(c->value) << "\"";
            return Visitor::visit(c);
        }
    // var
        virtual ExpressionPtr visit ( ExprVar * var ) override {
            ss << var->name;
            return Visitor::visit(var);
        }
    // field
        virtual ExpressionPtr visit ( ExprField * field ) override {
            ss << "." << field->name;
            return Visitor::visit(field);
        }
        virtual ExpressionPtr visit ( ExprSafeField * field ) override {
            ss << ".?" << field->name;
            return Visitor::visit(field);
        }
    // ptr2ref
        virtual ExpressionPtr visit ( ExprPtr2Ref * ptr2ref ) override {
            ss << "->";
            return Visitor::visit(ptr2ref);
        }
    // new
        virtual ExpressionPtr visit ( ExprNew * enew ) override {
            ss << "new " << enew->typeexpr->describe();
            return Visitor::visit(enew);
        }
    // null coaelescing
        virtual void preVisitNullCoaelescingDefault ( ExprNullCoalescing * nc, Expression * expr ) override {
            Visitor::preVisitNullCoaelescingDefault(nc,expr);
            ss << " ?? ";
        }
    // at
        virtual void preVisitAtIndex ( ExprAt * that, Expression * index ) override {
            Visitor::preVisitAtIndex(that, index);
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
            if ( !(that->topLevel || that->argLevel) ) ss << "(";
        }
        virtual ExpressionPtr visit ( ExprOp1 * that ) override {
            if ( !(that->topLevel || that->argLevel) ) ss << ")";
            if ( that->op=="+++" || that->op=="---" ) {
                ss << that->op[0] << that->op[1];
            }
            return Visitor::visit(that);
        }
    // op2
        virtual void preVisit ( ExprOp2 * that ) override {
            Visitor::preVisit(that);
            if ( !(that->topLevel || that->argLevel) ) ss << "(";
        }
        virtual void preVisitRight ( ExprOp2 * op2, Expression * right ) override {
            Visitor::preVisitRight(op2,right);
            ss << " " << op2->op << " ";
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( !(that->topLevel || that->argLevel) ) ss << ")";
            return Visitor::visit(that);
        }
    // op3
        virtual void preVisit ( ExprOp3 * that ) override {
            Visitor::preVisit(that);
            if ( !(that->topLevel || that->argLevel) ) ss << "(";
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
            if ( !(that->topLevel || that->argLevel) ) ss << ")";
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
        virtual void preVisit ( ExprReturn * that ) override {
            Visitor::preVisit(that);
            ss << "return ";
        }
    // break
        virtual void preVisit ( ExprBreak * that ) override {
            Visitor::preVisit(that);
            ss << "break";
        }
    // sizeof
        virtual void preVisit ( ExprSizeOf * that ) override {
            Visitor::preVisit(that);
            ss << "sizeof(";
        }
        virtual ExpressionPtr visit ( ExprSizeOf * that ) override {
            ss << ")";
            return Visitor::visit(that);
        }
    protected:
        stringstream ss;
        ostream::pos_type   lastNewLine = -1;
        int tab = 0;
    };
    
    ostream& operator<< (ostream& stream, const Program & program) {
        Printer log;
        const_cast<Program&>(program).visit(log);
        stream << log.str();
        return stream;
    }
    
    ostream& operator<< (ostream& stream, const Expression & expr) {
        Printer log;
        const_cast<Expression&>(expr).visit(log);
        stream << log.str();
        return stream;
    }
    
    ostream& operator<< (ostream& stream, const Function & func) {
        Printer log;
        const_cast<Function&>(func).visit(log);
        stream << log.str();
        return stream;
    }
}
