#include "precomp.h"

#include "ast.h"

namespace yzg {

    class Printer : public Visitor {
    public:
        Printer() {}
        string str() const { return ss.str(); };
        void newLine () {
            auto nlPos = ss.tellp();
            if ( nlPos != lastNewLine ) {
                ss << "\n";
                lastNewLine = ss.tellp();
            }
        }
    // function
        virtual void preVisit ( Function * fn) override {
            ss << "def " << fn->name;
            if ( fn->arguments.size() ) ss << " ( ";
        }
        virtual void preVisitFunctionBody ( Function * fn,Expression * ) override {
            if ( fn->arguments.size() ) ss << " )";
            if ( fn->result && !fn->result->isVoid() ) ss << " : " << fn->result->describe();
            ss << "\n";
        }
        virtual void preVisitArgument ( Function *, const VariablePtr & var, bool ) override {
            ss << var->name << ":" << var->type->describe();
        }
        virtual void preVisitArgumentInit ( Function *, Expression * ) override {
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
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * ) override {
            if ( !block->closure ) ss << string(tab,'\t');
        }
        virtual ExpressionPtr visitBlockExpression ( ExprBlock * block, Expression * that ) override {
            if ( block->closure ) ss << ";"; else newLine();
            return Visitor::visitBlockExpression(block, that);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            if ( block->closure ) ss << "{"; else tab ++;
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            if ( block->closure ) ss << "}"; else tab --;
            return Visitor::visit(block);
        }
    // let
        virtual void preVisit ( ExprLet * let ) override {
            ss << (let->subexpr ? "let (" : "let ");
        }
        virtual void preVisitLet ( ExprLet *, const VariablePtr & var, bool ) override {
            ss << var->name << ":" << var->type->describe();
        }
        virtual VariablePtr visitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            if ( !last ) ss << "; ";
            if ( last && let->subexpr ) ss << ")\n";
            return Visitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( ExprLet *, Expression * ) override {
            ss << " = ";
        }
    // if then else
        virtual void preVisit ( ExprIfThenElse * ) override {
            ss << "if ";
        }
        virtual void preVisitIfBlock ( ExprIfThenElse *, Expression * ) override {
            ss << "\n";
        }
        virtual void preVisitElseBlock ( ExprIfThenElse *, Expression * ) override {
            ss << string(tab,'\t') << "else\n";
        }
        // try-catch
        virtual void preVisit ( ExprTryCatch * ) override {
            ss << "try\n";
        }
        virtual void preVisitCatch ( ExprTryCatch *, Expression * ) override {
            ss << string(tab,'\t') << "catch\n";
        }
    // for
        virtual void preVisit ( ExprFor * ) override {
            ss << "for ";
        }
        virtual void preVisitFor ( ExprFor *, const VariablePtr & var, bool last ) override {
            ss << var->name;
            if ( !last ) ss << ","; else ss << " in ";
        }
        virtual void preVisitForFilter ( ExprFor *, Expression * ) override {
            ss << " where ";
        }
        virtual void preVisitForBody ( ExprFor *, Expression * ) override {
            ss << "\n";
        }
        virtual ExpressionPtr visitForSource ( ExprFor * ffor, Expression * that , bool last ) override {
            if ( !last ) ss << ",";
            return Visitor::visitForSource(ffor, that, last);
        }
    // while
        virtual void preVisit ( ExprWhile * ) override {
            ss << "while ";
        }
        virtual void preVisitWhileBody ( ExprWhile *, Expression * ) override {
            ss << "\n";
        }
    // call
        virtual void preVisit ( ExprCall * call ) override {
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
        virtual void preVisitNullCoaelescingDefault ( ExprNullCoalescing *, Expression * ) override {
            ss << " ?? ";
        }
    // at
        virtual void preVisitAtIndex ( ExprAt *, Expression * ) override {
            ss << "[";
        }
        virtual ExpressionPtr visit ( ExprAt * that ) override {
            ss << "]";
            return Visitor::visit(that);
        }
    // op1
        virtual void preVisit ( ExprOp1 * that ) override {
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
            if ( !(that->topLevel || that->argLevel) ) ss << "(";
        }
        virtual void preVisitRight ( ExprOp2 * op2, Expression * ) override {
            ss << " " << op2->op << " ";
        }
        virtual ExpressionPtr visit ( ExprOp2 * that ) override {
            if ( !(that->topLevel || that->argLevel) ) ss << ")";
            return Visitor::visit(that);
        }
    // op3
        virtual void preVisit ( ExprOp3 * that ) override {
            if ( !(that->topLevel || that->argLevel) ) ss << "(";
        }
        virtual void preVisitLeft  ( ExprOp3 *, Expression * ) override {
            ss << " ? ";
        }
        virtual void preVisitRight ( ExprOp3 *, Expression * ) override {
            ss << " : ";
        }
        virtual ExpressionPtr visit ( ExprOp3 * that ) override {
            if ( !(that->topLevel || that->argLevel) ) ss << ")";
            return Visitor::visit(that);
        }
    // copy & move
        virtual void preVisitRight ( ExprCopy *, Expression * ) override {
            ss << " = ";
        }
        virtual void preVisitRight ( ExprMove *, Expression * ) override {
            ss << " <- ";
        }
    // return
        virtual void preVisit ( ExprReturn *) override {
            ss << "return ";
        }
    // break
        virtual void preVisit ( ExprBreak *) override {
            ss << "break";
        }
    // sizeof
        virtual void preVisit ( ExprSizeOf * that ) override {
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
