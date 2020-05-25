#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"

namespace das {

    namespace GCH {
        const string flow_color = "green";
        const string flow_else_color = "darkgreen";
        const string flow_loop_color = "blue";
        const string flow_catch = "red";
        const string flow_in = "darkgreen";
        const string flow_read = "orange";
        const string flow_write = "brown";
        const string function_subgraph_color = "blue";
        const string function_header_color = "blue";
        const string block_subgraph_color = "white";
        const string closure_subgraph_color = "lightblue";
        const string let_subgraph_color = "lightgrey";
        const string if_subgraph_color = "grey";
        const string for_subgraph_color = "grey";
        const string while_subgraph_color = "grey";
        const string call_subgraph_color = "grey";
        const string return_subgraph_color = "grey";
        const string trycatch_subgraph_color = "cyan";
    };

    using namespace GCH;

    class GraphPrinter : public Visitor {
    public:
        string str() const {
            return "digraph{\n" + sgraph.str() + "\n" + psgraph.str() + "\n}\n";
        };
    public:
        bool                plot_read_write = false;
    protected:
        TextWriter        sgraph;
        TextWriter        psgraph;
    protected:
        template <typename TT>
        string id(TT * p) const { return "id_" + to_string(intptr_t(p)); }
        template <typename TT>
        string id(const smart_ptr<TT> & value) const { return "id_" + to_string(intptr_t(value.get())); }
    protected:
        template <typename TF, typename TT>
        void connect(TF a, TT b, const string & extra = "") {
            sgraph << id(a) << " -> " << id(b);
            if (!extra.empty()) {
                sgraph << " [" << extra << "]";
            }
            sgraph << ";\n";
        }
        template <typename TF, typename TT>
        void post_connect(TF a, TT b, const string & extra = "") {
            psgraph << id(a) << " -> " << id(b);
            if (!extra.empty()) {
                psgraph << " [" << extra << "]";
            }
            psgraph << ";\n";
        }
        template <typename TF>
        void connect_block(TF a, Expression* b, string color = flow_in, const string & extra = "") {
            DAS_ASSERT(b->rtti_isBlock());
            auto block = (ExprBlock *)b;
            sgraph << id(a) << " -> ";
            if (block->list.size()) {
                sgraph << id(block->list[0]);
            }
            else {
                sgraph << id(b);
            }
            sgraph << " [color=" << color;
            if (!extra.empty()) {
                sgraph << ", " << extra;
            }
            sgraph << "]; \n";
        }
        template <typename TT>
        void label(TT a, const string & t, const string & extra = "") {
            sgraph << id(a) << " [label=\"" << t << "\"";
            if (!extra.empty()) {
                sgraph << "," << extra;
            }
            sgraph << "];\n";
        }
        template <typename TT>
        void subgraph(TT it, const string & gtype, const string & gcolor = "white") {
            sgraph << "subgraph cluster_" << gtype << "_" << id(it) << " {\n color = " << gcolor << ";\n";
        }
        void endsubgraph() {
            sgraph << "}\n";
        }
        template <typename TT>
        void connect_loop(TT loop, Expression * body, const string & lcolor = flow_loop_color) {
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
            sgraph << "node[style = filled];\n";
            label(fn, "def " + fn->name, "shape=polygon,sides=4,skew=.4");
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
            subgraph(block, "block", block->isClosure ? closure_subgraph_color : block_subgraph_color);
            if (block->isClosure) {
                if (block->type) {
                    label(block, "BLOCK : " + block->type->describe(), "shape=hexagon");
                }
                else {
                    label(block, "BLOCK : void", "shape=hexagon");
                }
            }
            if (block->list.size()) {
                if (block->isClosure) {
                    connect(block, block->list[0], "color=" + flow_color + " constraint=false");
                }
                for (size_t i = 1; i < block->list.size(); ++i) {
                    connect(block->list[i - 1], block->list[i], "color=" + flow_color);
                }
            }
        }
        virtual void preVisitBlockArgument(ExprBlock * block, const VariablePtr & var, bool lastArg) override {
            Visitor::preVisitBlockArgument(block, var, lastArg);
            label(var, var->name + " : " + var->type->describe());
            connect(var, block);
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
            label(ifte, "if", "shape=Mdiamond");
            connect(ifte->cond, ifte);
        }
        virtual void preVisitIfBlock(ExprIfThenElse * ifte, Expression * block) override {
            Visitor::preVisitIfBlock(ifte, block);
            connect_block(ifte, block, flow_in, "label=\"true\"");
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
            subgraph(tc, "trycatch", trycatch_subgraph_color);
            label(tc, "try-catch", "shape=Msquare");
            connect_block(tc, tc->try_block.get());
            connect_block(tc, tc->catch_block.get(), flow_catch);
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
            subgraph(call, "call", call_subgraph_color);
            label(call, call->name, "shape=box");
        }
        virtual ExpressionPtr visitCallArg(ExprCall * call, Expression * arg, bool last) override {
            string argName;
            if (call->func) {
                auto it = find_if(call->arguments.begin(), call->arguments.end(), [&](const ExpressionPtr & a) {
                    return a.get() == arg;
                });
                DAS_ASSERT(it != call->arguments.end());
                auto idx = it - call->arguments.begin();
                argName = call->func->arguments[idx]->name;
            }
            connect(arg, call, argName.empty() ? "" : ("label=\"" + argName + "\""));
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit(ExprCall * call) override {
            endsubgraph();
            return Visitor::visit(call);
        }
        // looks like call
        virtual void preVisit(ExprLooksLikeCall * call) override {
            Visitor::preVisit(call);
            subgraph(call, "like_call", call_subgraph_color);
            label(call, call->name, "shape=box");
        }
        virtual ExpressionPtr visitLooksLikeCallArg(ExprLooksLikeCall * call, Expression * arg, bool last) override {
            connect(arg, call);
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit(ExprLooksLikeCall * call) override {
            endsubgraph();
            return Visitor::visit(call);
        }
        // const
        virtual ExpressionPtr visit(ExprFakeContext * c) override {
            label(c, "__context__");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprFakeLineInfo * c) override {
            label(c, "LineInfo");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstPtr * c) override {
            if (c->getValue()) {
                TextWriter ss;
                ss << "*0x" << HEX << intptr_t(c->getValue()) << DEC;
                label(c, ss.str());
            }
            else {
                label(c, "null");
            }
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt8 * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt16 * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt64 * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt8 * c) override {
            TextWriter ss;
            ss << "0x" << HEX << intptr_t(c->getValue()) << DEC;
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt16 * c) override {
            TextWriter ss;
            ss << "0x" << HEX << intptr_t(c->getValue()) << DEC;
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt64 * c) override {
            TextWriter ss;
            ss << "0x" << HEX << intptr_t(c->getValue()) << DEC;
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt * c) override {
            TextWriter ss;
            ss << "0x" << HEX << intptr_t(c->getValue()) << DEC;
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstBitfield * c) override {
            TextWriter ss;
            ss << "0x" << HEX << intptr_t(c->getValue()) << DEC;
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstBool * c) override {
            label(c, c->getValue() ? "true" : "false");
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstFloat * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstDouble * c) override {
            label(c, to_string(c->getValue()));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstString * c) override {
            label(c, escapeString(c->text));
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt2 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "int2(" << val.x << "," << val.y << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstRange * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "range(" << val.from << "," << val.to << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt3 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "int3(" << val.x << "," << val.y << "," << val.z << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstInt4 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "int4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt2 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "uint2(" << val.x << "," << val.y << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstURange * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "urange(" << val.from << "," << val.to << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt3 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "uint3(" << val.x << "," << val.y << "," << val.z << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstUInt4 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "uint4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstFloat2 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "float2(" << val.x << "," << val.y << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstFloat3 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "float3(" << val.x << "," << val.y << "," << val.z << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        virtual ExpressionPtr visit(ExprConstFloat4 * c) override {
            auto val = c->getValue();
            TextWriter ss;
            ss << "float4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            label(c, ss.str());
            return Visitor::visit(c);
        }
        // var
        virtual ExpressionPtr visit(ExprVar * var) override {
            label(var, var->name);
            if (plot_read_write) {
                if (var->write) {
                    post_connect(var, var->variable, "color=" + flow_write + " constraint=false");
                } else {
                    post_connect(var->variable, var, "color=" + flow_read + " constraint=false");
                }
            }
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
        virtual ExpressionPtr visit(ExprIsVariant * field) override {
            label(field, "is " + field->name);
            connect(field->value, field);
            return Visitor::visit(field);
        }
        virtual ExpressionPtr visit(ExprAsVariant * field) override {
            label(field, "as " + field->name);
            connect(field->value, field);
            return Visitor::visit(field);
        }
        virtual ExpressionPtr visit(ExprSafeAsVariant * field) override {
            label(field, "?as " + field->name);
            connect(field->value, field);
            return Visitor::visit(field);
        }
        // ptr2ref
        virtual ExpressionPtr visit(ExprPtr2Ref * ptr2ref) override {
            label(ptr2ref, "->");
            connect(ptr2ref->subexpr, ptr2ref);
            return Visitor::visit(ptr2ref);
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
        // safe at
        virtual void preVisit(ExprSafeAt * expr) override {
            Visitor::preVisit(expr);
            label(expr, "?[]");
            connect(expr->subexpr, expr);
            connect(expr->index, expr);
        }
        // op1
        virtual void preVisit(ExprOp1 * that) override {
            Visitor::preVisit(that);
            label(that, that->op, "shape=box");
            connect(that->subexpr, that);
        }
        // op2
        virtual void preVisit(ExprOp2 * that) override {
            Visitor::preVisit(that);
            label(that, that->op, "shape=box");
            connect(that->left, that);
            connect(that->right, that);
        }
        // op3
        virtual void preVisit(ExprOp3 * that) override {
            Visitor::preVisit(that);
            label(that, that->op, "shape=box");
            connect(that->subexpr, that);
            connect(that->left, that);
            connect(that->right, that);
        }
        // copy & move
        virtual void preVisit(ExprCopy * that) override {
            Visitor::preVisit(that);
            label(that, "=", "shape=box");
            connect(that->left, that);
            connect(that->right, that);
        }
        virtual void preVisit(ExprClone * that) override {
            Visitor::preVisit(that);
            label(that, ":=", "shape=box");
            connect(that->left, that);
            connect(that->right, that);
        }
        virtual void preVisit(ExprMove * that) override {
            Visitor::preVisit(that);
            label(that, "<-", "shape=box");
            connect(that->left, that);
            connect(that->right, that);
        }
        // return
        virtual void preVisit(ExprReturn * that) override {
            Visitor::preVisit(that);
            subgraph(that, "return", return_subgraph_color);
            label(that, "return", "shape=Msquare");
            if (that->subexpr)
                connect(that->subexpr, that);
        }
        virtual ExpressionPtr visit(ExprReturn * that) override {
            endsubgraph();
            return Visitor::visit(that);
        }
        // break
        virtual void preVisit(ExprBreak * that) override {
            Visitor::preVisit(that);
            label(that, "break", "shape=Msquare");
        }
        // break
        virtual void preVisit(ExprContinue * that) override {
            Visitor::preVisit(that);
            label(that, "continue", "shape=Msquare");
        }
        // make block
        virtual void preVisit(ExprMakeBlock * that) override {
            Visitor::preVisit(that);
            label(that, "{}", "shape=box");
            connect(that->block, that);
        }
    };

    string Program::dotGraph() {
        GraphPrinter graph;
        graph.plot_read_write = options.getBoolOption("plot_read_write", false);
        visit(graph);
        return graph.str();
    }

}
