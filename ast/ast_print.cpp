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
    // ExprFor
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            if ( expr->filter ) expr->filter->argLevel = true;
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

	namespace GCH {
		const string flow_color = "green";
		const string flow_else_color = "darkgreen";
		const string flow_loop_color = "blue";
		const string function_subgraph_color = "blue";
		const string function_header_color = "blue";
		const string block_subgraph_color = "white";
		const string let_subgraph_color = "lightgrey";
		const string if_subgraph_color = "grey";
		const string for_subgraph_color = "grey";
		const string while_subgraph_color = "grey";
		const string trycatch_subgraph_color = "cyan";
	};

	using namespace GCH;

	class GraphPrinter : public Visitor {


	public:
		string str() const { 
			return "digraph{\n" + sgraph.str() + "\n}\n"; 
		};
	protected:
		template <typename TT>
		string id(TT * p) const { return "id_" + std::to_string(intptr_t(p)); }
		template <typename TT>
		string id(const shared_ptr<TT> & value ) const { return "id_" + std::to_string(intptr_t(value.get())); }
	protected:
		template <typename TF, typename TT>
		void connect(TF a, TT b, const string & extra = "" ) {
			sgraph << id(a) << " -> " << id(b);
			if (!extra.empty()) {
				sgraph << " [" << extra << "]";
			}
			sgraph << ";\n";
		}
		template <typename TF>
		void connect_block(TF a, Expression* b, string color = flow_color, const string & extra = "" ) {
			assert(b->rtti_isBlock());
			auto block = (ExprBlock *)b;
			sgraph << id(a) << " -> ";
			if ( block->list.size() ) {
				sgraph << id(block->list[0]);
			} else {
				sgraph << id(b);
			}
			sgraph << " [color=" << color;
			if (!extra.empty()) {
				sgraph << ", " << extra;
			}
			sgraph << "]; \n"; 
		}
		template <typename TT>
		void label(TT a, const string & t, const string & extra = "" ) {
			sgraph << id(a) << " [label=\"" << t << "\"";
			if (!extra.empty()) {
				sgraph << "," << extra;
			}
			sgraph << "];\n";
		}
		template <typename TT>
		void subgraph( TT it, const string & gtype, const string & gcolor = "white" ) {
			sgraph << "subgraph cluster_" << gtype << "_" << id(it) << " {\n color = " << gcolor << ";\n";
		}
		void endsubgraph() {
			sgraph << "}\n";
		}
		template <typename TT>
		void connect_loop(TT loop, Expression * body, const string & lcolor = flow_loop_color ) {
			if (body->rtti_isBlock()) {
				auto block = (ExprBlock *)body;
				if (block->list.size()) {
					connect(block->list.back(), loop, "color=" + lcolor);
				}
			}
			else {
				connect(body, loop, "color=" + lcolor);
			}
		}
	protected:
	// global variale
		virtual void preVisitGlobalLet(const VariablePtr & var) override {
			Visitor::preVisitGlobalLet(var);
			label(var, var->name + " : " + var->type->describe());
		}
		// function
		virtual void preVisit(Function * fn) override {
			Visitor::preVisit(fn);
			subgraph(fn, "function", function_subgraph_color);
			sgraph << "node[style = filled];\n";			label(fn, "def " + fn->name, "shape=polygon,sides=4,skew=.4");
			subgraph(fn, "function_header", function_header_color);
		}
		virtual void preVisitFunctionBody(Function * fn, Expression * expr) override {
			Visitor::preVisitFunctionBody(fn, expr);
			endsubgraph();
			connect_block(fn, expr);
		}
		virtual void preVisitArgument(Function * fn, const VariablePtr & arg, bool last) override {
			Visitor::preVisitArgument(fn, arg, last);
			label(arg, arg->name + " : " + arg->type->describe());
			connect(arg, fn);
		}
		virtual void preVisitArgumentInit(Function * fn, const VariablePtr & arg, Expression * expr) override {
			Visitor::preVisitArgumentInit(fn, arg, expr);
			connect(expr, arg);
		}
		virtual FunctionPtr visit(Function * fn) override {
			endsubgraph();
			return Visitor::visit(fn);
		}
		// block
		virtual void preVisit(ExprBlock * block) override {
			Visitor::preVisit(block);
			subgraph(block, "block", block_subgraph_color);
			if (block->list.size()) {
				for (int i = 1; i < block->list.size(); ++i) {
					connect(block->list[i - 1], block->list[i], "color="+flow_color);
				}
			}
		}
		virtual ExpressionPtr visit(ExprBlock * block) override {
			endsubgraph();
			return Visitor::visit(block);
		}
		// let
		virtual void preVisit(ExprLet * let) override {
			Visitor::preVisit(let);
			subgraph(let, "let", let_subgraph_color);
			label(let, "let", "shape=box");
			if (let->subexpr) connect_block(let, let->subexpr.get());
		}
		virtual void preVisitLet(ExprLet * let, const VariablePtr & var, bool last) override {
			Visitor::preVisitLet(let, var, last);
			label(var, var->name + " : " + var->type->describe());
			connect(var, let);
		}
		virtual void preVisitLetInit(ExprLet * let, const VariablePtr & var, Expression * expr) override {
			Visitor::preVisitLetInit(let, var, expr);
			connect(expr, var);
		}
		virtual ExpressionPtr visit(ExprLet * let) override {
			endsubgraph();
			return Visitor::visit(let);
		}
		// if then else
		virtual void preVisit(ExprIfThenElse * ifte) override {
			Visitor::preVisit(ifte);
			subgraph(ifte, "if", if_subgraph_color);
			label(ifte, "if", "shape=Msquare");
			connect(ifte->cond, ifte);
		}
		virtual void preVisitIfBlock(ExprIfThenElse * ifte, Expression * block) override {
			Visitor::preVisitIfBlock(ifte, block);
			connect_block(ifte, block, flow_color, "label=\"true\"");
		}
		virtual void preVisitElseBlock(ExprIfThenElse * ifte, Expression * block) override {
			Visitor::preVisitElseBlock(ifte, block);
			connect_block(ifte, block, flow_else_color, "label=\"false\"");
		}
		virtual ExpressionPtr visit(ExprIfThenElse * ifte) override {
			endsubgraph();
			return Visitor::visit(ifte);
		}
		// try-catch
		virtual void preVisit(ExprTryCatch * tc) override {
			Visitor::preVisit(tc);
			subgraph(tc, "try-catch", trycatch_subgraph_color);
			label(tc, "try-catch", "shape=Msquare");
			connect_block(tc, tc->try_block.get());
			connect_block(tc, tc->catch_block.get());
		}
		virtual ExpressionPtr visit(ExprTryCatch * ifte) override {
			endsubgraph();
			return Visitor::visit(ifte);
		}
		// for
		virtual void preVisit(ExprFor * ffor) override {
			Visitor::preVisit(ffor);
			subgraph(ffor, "for", for_subgraph_color);
			label(ffor, "for", "shape = Msquare");
			for (size_t x = 0; x != ffor->sources.size(); ++x) {
				auto & source = ffor->sources[x];
				auto & var = ffor->iteratorVariables[x];
				label(var, var->name + " : " + var->type->describe());
				connect(var, ffor);
				connect(source, var);
			}
		}
		virtual void preVisitForFilter(ExprFor * ffor, Expression * filter) override {
			Visitor::preVisitForFilter(ffor, filter);
			connect(filter, ffor);
		}
		virtual void preVisitForBody(ExprFor * ffor, Expression * body) override {
			Visitor::preVisitForBody(ffor, body);
			connect_block(ffor, body);
			connect_loop(ffor, body);
		}
		virtual ExpressionPtr visit(ExprFor * ffor) override {
			endsubgraph();
			return Visitor::visit(ffor);
		}
		// while
		virtual void preVisit(ExprWhile * wh) override {
			Visitor::preVisit(wh);
			subgraph(wh, "while", while_subgraph_color);
			label(wh, "while", "shape = Msquare");
			connect(wh->cond, wh);
		}
		virtual void preVisitWhileBody(ExprWhile * wh, Expression * body) override {
			Visitor::preVisitWhileBody(wh, body);
			connect_block(wh, body);
			connect_loop(wh, body);
		}
		virtual ExpressionPtr visit(ExprWhile * wg) override {
			endsubgraph();
			return Visitor::visit(wg);
		}
		// call
		virtual void preVisit(ExprCall * call) override {
			Visitor::preVisit(call);
			sgraph << "subgraph cluster_call_" << id(call) << " {\n";
			sgraph << "color = grey;\n";
			label(call, call->name, "shape=box");
		}
		virtual ExpressionPtr visitCallArg(ExprCall * call, Expression * arg, bool last) override {
			connect(arg, call);
			return Visitor::visitCallArg(call, arg, last);
		}
		virtual ExpressionPtr visit(ExprCall * call) override {
			sgraph << "}\n";
			return Visitor::visit(call);
		}
		// looks like call
		virtual void preVisit(ExprLooksLikeCall * call) override {
			Visitor::preVisit(call);
			sgraph << "subgraph cluster_like_call_" << id(call) << " {\n";
			sgraph << "color = grey;\n";
			label(call, call->name, "shape=box");
		}
		virtual ExpressionPtr visitLooksLikeCallArg(ExprLooksLikeCall * call, Expression * arg, bool last) override {
			connect(arg, call);
			return Visitor::visitLooksLikeCallArg(call, arg, last);
		}
		virtual ExpressionPtr visit(ExprLooksLikeCall * call) override {
			sgraph << "}\n";
			return Visitor::visit(call);
		}
		// const
		virtual ExpressionPtr visit(ExprFakeContext * c) override {
			label(c, "__context__");
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstPtr * c) override {
			if (c->getValue()) {
				stringstream ss;
				ss << "*0x" << hex << intptr_t(c->getValue()) << dec;
				label(c, ss.str());
			}
			else {
				label(c, "null");
			}
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstInt * c) override {
			label(c, std::to_string(c->getValue()));
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstUInt64 * c) override {
			stringstream ss;
			ss << "0x" << hex << intptr_t(c->getValue()) << dec;
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstUInt * c) override {
			stringstream ss;
			ss << "0x" << hex << intptr_t(c->getValue()) << dec;
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstBool * c) override {
			label(c, c->getValue() ? "true" : "false");
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstFloat * c) override {
			label(c, std::to_string(c->getValue()));
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstString * c) override {
			label(c, escapeString(c->text));
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstInt2 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "int2(" << val.x << "," << val.y << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstInt3 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "int3(" << val.x << "," << val.y << "," << val.z << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstInt4 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "int4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstUInt2 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "uint2(" << val.x << "," << val.y << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstUInt3 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "uint3(" << val.x << "," << val.y << "," << val.z << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstUInt4 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "uint4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstFloat2 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "float2(" << val.x << "," << val.y << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstFloat3 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "float3(" << val.x << "," << val.y << "," << val.z << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		virtual ExpressionPtr visit(ExprConstFloat4 * c) override {
			auto val = c->getValue();
			stringstream ss;
			ss << "float4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
			label(c, ss.str());
			return Visitor::visit(c);
		}
		// var
		virtual ExpressionPtr visit(ExprVar * var) override {
			label(var, var->name);
			// connect(var->variable, var, "color=grey");
			return Visitor::visit(var);
		}
		// field
		virtual ExpressionPtr visit(ExprField * field) override {
			label(field, "." + field->name);
			connect(field->value, field);
			return Visitor::visit(field);
		}
		virtual ExpressionPtr visit(ExprSafeField * field) override {
			label(field, "?." + field->name);
			connect(field->value, field);
			return Visitor::visit(field);
		}
		// new
		virtual ExpressionPtr visit(ExprNew * enew) override {
			label(enew, "new " + enew->typeexpr->describe());
			return Visitor::visit(enew);
		}
		// null coaelescing
		virtual void preVisit(ExprNullCoalescing * nc) override {
			Visitor::preVisit(nc);
			label(nc, "??");
			connect(nc->subexpr, nc);
			connect(nc->defaultValue, nc);
		}
		// at
		virtual void preVisit(ExprAt * expr) override {
			Visitor::preVisit(expr);
			label(expr, "[]");
			connect(expr->subexpr, expr);
			connect(expr->index, expr);
		}
		// op1
		virtual void preVisit(ExprOp1 * that) override {
			Visitor::preVisit(that);
			label(that, that->op);
			connect(that->subexpr, that);
		}
		// op2
		virtual void preVisit(ExprOp2 * that) override {
			Visitor::preVisit(that);
			label(that, that->op);
			connect(that->left, that);
			connect(that->right, that);
		}
		// op3
		virtual void preVisit(ExprOp3 * that) override {
			Visitor::preVisit(that);
			label(that, that->op);
			connect(that->subexpr, that);
			connect(that->left, that);
			connect(that->right, that);
		}
		// copy & move
		virtual void preVisit(ExprCopy * that) override {
			Visitor::preVisit(that);
			label(that, "=");
			connect(that->left, that);
			connect(that->right, that);
		}
		virtual void preVisit(ExprMove * that) override {
			Visitor::preVisit(that);
			label(that, "<-");
			connect(that->left, that);
			connect(that->right, that);
		}
		// return
		virtual void preVisit(ExprReturn * that) override {
			Visitor::preVisit(that);
			label(that, "return");
			if (that->subexpr)
				connect(that->subexpr, that);
		}
		// break
		virtual void preVisit(ExprBreak * that) override {
			Visitor::preVisit(that);
			label(that, "break");
		}
		// sizeof
		virtual void preVisit(ExprSizeOf * that) override {
			Visitor::preVisit(that);
			label(that, "sizeof " + that->typeexpr->describe());
		}

	protected:
		stringstream        sgraph;
	};

	string Program::dotGraph() {
		GraphPrinter graph;
		visit(graph);
		return graph.str();
	}

}
