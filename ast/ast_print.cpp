#include "precomp.h"

#include "ast.h"

namespace yzg {
    
    class SetPrinterFlags : public Visitor {
    // ExprBlock
        virtual void preVisitBlockExpression ( ExprBlock * block, Expression * expr ) override {
            Visitor::preVisitBlockExpression(block, expr);
            expr->topLevel = true;
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
        // const
        virtual ExpressionPtr visit ( ExprConst * c ) override {
            c->bottomLevel = true;
            return Visitor::visit(c);
        }
    };

    class Printer : public Visitor {
    public:
        string str() const { return ss.str(); };
        const bool printRef = true;
        const bool printVarAccess = true;
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
    // let
        virtual void preVisit ( ExprLet * let ) override {
            Visitor::preVisit(let);
            ss << (let->subexpr ? "let (" : "let ");
        }
        virtual void preVisitLet ( ExprLet * let, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(let, var, last);
            if ( printVarAccess && !var->access_ref ) ss << "$";
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
                ss << "*0x" << hex << intptr_t(c->getValue()) << dec;
            } else {
                ss << "null";
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstInt * c ) override {
            ss << c->getValue();
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt64 * c ) override {
            ss << "0x" << hex << c->getValue() << dec;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstUInt * c ) override {
            ss << "0x" << hex << c->getValue() << dec;
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstBool * c ) override {
            ss << (c->getValue() ? "true" : "false");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat * c ) override {
            ss << c->getValue();
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
            ss << "float2(" << val.x << "," << val.y << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            ss << "float3(" << val.x << "," << val.y << "," << val.z << ")";
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit ( ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            ss << "float4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
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
        stringstream        ss;
        ostream::pos_type   lastNewLine = -1;
        int                 tab = 0;
    };
    
    template <typename TT>
    __forceinline ostream&  print ( ostream& stream, const TT & value ) {
        SetPrinterFlags flags;
        const_cast<TT&>(value).visit(flags);
        Printer log;
        const_cast<TT&>(value).visit(log);
        stream << log.str();
        return stream;
    }
    
    ostream& operator<< (ostream& stream, const Program & program) {
        return print(stream,program);
    }
    
    ostream& operator<< (ostream& stream, const Expression & expr) {
        return print(stream,expr);
    }
    
    ostream& operator<< (ostream& stream, const Function & func) {
        return print(stream,func);
    }
}
