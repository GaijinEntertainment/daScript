#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_generate.h"
#include "ast_inscope_pod.h"

namespace das {

    string inScopePodSite ( const Function * func ) {
        return " in function '" + func->module->name + "::" + func->name + "'\n";
    }

    string inScopePodRefusal ( const Function * func, InScopePodRewrite rewrite ) {
        string why;
        bool collects = rewrite!=InScopePodRewrite::stackLiteral;
        if ( rewrite!=InScopePodRewrite::moveCollect ) {
            if ( func->generated && !func->lambda && !func->localFunction ) why += "\tfunction is generated\n";
            if ( func->generator ) why += "\tfunction is generator\n";
            if ( collects && func->hasTryRecover ) why += "\tfunction has try-recover\n";
        }
        if ( collects && func->hasUnsafe ) why += "\tfunction has unsafe\n";
        if ( !func->module->allowPodInscope ) why += "\tmodule " + func->module->name + " does not allow in-scope POD\n";
        if ( func->fromGeneric && !func->fromGeneric->module->allowPodInscope )
            why += "\tgeneric function module " + func->fromGeneric->module->name + " does not allow in-scope POD\n";
        return why;
    }

    void Program::logInScopePodOnce ( TextWriter & logs, const LineInfo & at, const string & message ) {
        string line;
        if ( !at.empty() && at.fileInfo ) {
            line = at.fileInfo->name + ":" + to_string(at.line) + ":" + to_string(at.column) + " ";
        }
        line += message;
        if ( inScopePodLogged.insert(line).second ) logs << line;
    }

    ExprMakeArray * heapArrayLiteral ( Expression * expr ) {
        if ( !expr->rtti_isCall() ) return nullptr;
        auto call = static_cast<ExprCall *>(expr);
        if ( !call->func || !call->func->isBuiltinGenericInstance("to_array_move") ) return nullptr;
        if ( call->arguments.size()!=1 || !call->arguments[0]->rtti_isMakeArray() ) return nullptr;
        auto mka = static_cast<ExprMakeArray *>(call->arguments[0]);
        return ( mka->gen2 && mka->makeArrayOnHeap ) ? mka : nullptr;
    }

    bool literalElementsOwnHeap ( const ExprMakeArray * literal ) {
        auto & elemT = literal->recordType;
        return !elemT || !( elemT->isPointer() || elemT->isNoHeapType() );
    }

    // src/ast/ARCHITECTURE_LOOP_SOURCE.md#stack-loop-literal
    ExprMakeArray * stackLoopLiteral ( Expression * source ) {
        const uint64_t maxStackLiteralBytes = 1024;
        auto mka = heapArrayLiteral(source);
        if ( !mka || literalElementsOwnHeap(mka) ) return nullptr;
        auto bytes = uint64_t(mka->values.size()) * mka->recordType->getSizeOf64();
        return bytes <= maxStackLiteralBytes ? mka : nullptr;
    }

    static bool canHoldArray ( const TypeDeclPtr & type ) {
        if ( type->ref || type->isPointer() || type->isGoodIteratorType() || type->isGoodLambdaType() || type->isGoodBlockType() ) return true;
        if ( type->isGoodArrayType() || type->isGoodTableType() ) {
            return ( type->firstType && canHoldArray(type->firstType) ) || ( type->secondType && canHoldArray(type->secondType) );
        }
        return !type->isNoHeapType();
    }

    class PodVisitor : public Visitor {
    public:
        bool anyWork = false;
        PodVisitor ( Program * prog, TextWriter * logs_ = nullptr ) : program(prog), logs(logs_) {}
    protected:
        virtual bool canVisitFunction ( Function * fun ) override {
            if ( fun->fromGeneric && fun->fromGeneric->module->name=="builtin" ) return false;
            return !fun->stub && !fun->isTemplate;    // we don't do a thing with templates
        }
        virtual void preVisit ( Function * fun ) override {
            Visitor::preVisit(fun);
            func = fun;
        }
        virtual FunctionPtr visit ( Function * fun ) override {
            func = nullptr;
            return Visitor::visit(fun);
        }
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            blocks.push_back(block);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            blocks.pop_back();
            return Visitor::visit(block);
        }
        void log ( const LineInfo & at, const string & message ) {
            if ( logs ) program->logInScopePodOnce(*logs, at, message + inScopePodSite(func));
        }
        void logRefused ( const LineInfo & at, const string & what, const string & why ) {
            if ( logs ) program->logInScopePodOnce(*logs, at, "warning: " + what + inScopePodSite(func) + why);
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            if ( func && var->pod_delete && !var->pod_delete_gen ) {
                anyWork = true;
                var->pod_delete_gen = true;
                if ( var->single_return_via_move ) {
                    log(var->at, "skipping POD optimization for single_return_via_move variable '" + var->name + "'");
                } else if ( var->consumed ) {
                    log(var->at, "skipping POD optimization for consumed variable '" + var->name + "'");
                } else if ( auto why = inScopePodRefusal(func, InScopePodRewrite::scopeCollect); !why.empty() ) {
                    logRefused(var->at, "POD optimization failed for '" + var->name + "'", why);
                } else {
                    func->notInferred();
                    auto CallCollectLocal = new ExprCall(expr->at,"_::builtin_collect_local_and_zero");
                    CallCollectLocal->arguments.push_back(new ExprVar(expr->at, var->name));
                    CallCollectLocal->arguments.push_back(new ExprConstUInt(expr->at, var->type->getSizeOf()));
                    CallCollectLocal->alwaysSafe = true;
                    blocks.back()->finalList.push_back(CallCollectLocal);
                    log(var->at, "In-scope POD applied to variable '" + var->name + "'");
                }
            }
            return Visitor::visitLet(expr,var,last);
        }
        bool isCollectableSource ( Expression * src ) const {
            if ( src->rtti_isMakeLocal() ) return true;
            if ( src->rtti_isCall() ) {
                auto csrc = (ExprCall *) src;
                return csrc->func && !csrc->func->result->ref && !csrc->func->unsafeOutsideOfFor;
            }
            if ( src->rtti_isInvoke() ) {
                auto csrc = (ExprInvoke *) src;
                return csrc->type && !csrc->type->ref;
            }
            return false;
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            if ( expr->generated || !func ) return Visitor::visit(expr);
            auto noCollect = inScopePodRefusal(func, InScopePodRewrite::scopeCollect);
            vector<int> podSourceIndices;
            for ( size_t i=0; i!=expr->sources.size(); ++i ) {
                auto src = expr->sources[i];
                if ( !src->type || !src->type->isGoodArrayType() || src->type->constant ) continue;
                if ( !isCollectableSource(src) ) continue;
                if ( !noCollect.empty() ) {
                    logRefused(src->at, "POD optimization failed for loop source '" + expr->iterators[i] + "'", noCollect);
                    continue;
                }
                podSourceIndices.push_back(int(i));
            }
            if ( podSourceIndices.size() ) {
                anyWork = true;
                func->notInferred();
                // we make a new block, we make a new variable for each pod source, and we assign it before the for
                auto newBlock = new ExprBlock();
                newBlock->at = expr->at;
                newBlock->isCollapseable = true;
                auto letPod = new ExprLet();
                letPod->at = expr->at;
                letPod->alwaysSafe = true;  // this is for the array<smart_ptr> and some such
                newBlock->list.push_back(letPod);
                for ( auto i : podSourceIndices ) {
                    auto podVar = new Variable();
                    podVar->at = expr->sources[i]->at;
                    podVar->name = "`pod`source`" + expr->iterators[i];
                    podVar->type = new TypeDecl(Type::autoinfer, podVar->at);
                    auto sourceSize = uint32_t(expr->sources[i]->type->getSizeOf());
                    podVar->init = move(expr->sources[i]);
                    podVar->init_via_move = true;
                    podVar->pod_delete = true;
                    podVar->pod_delete_gen = true;
                    letPod->variables.push_back(podVar);
                    expr->sources[i] = new ExprVar(podVar->at, podVar->name);
                    // and collect
                    auto CallCollectLocal = new ExprCall(expr->at,"_::builtin_collect_local_and_zero");
                    CallCollectLocal->arguments.push_back(new ExprVar(expr->at, podVar->name));
                    CallCollectLocal->arguments.push_back(new ExprConstUInt(expr->at, sourceSize));
                    CallCollectLocal->alwaysSafe = true;
                    newBlock->finalList.push_back(CallCollectLocal);
                    log(podVar->at, "In-scope POD applied to loop source '" + expr->iterators[i] + "'");
                }
                newBlock->list.push_back(expr);
                return newBlock;
            }
            return Visitor::visit(expr);
        }
        // src/ast/ARCHITECTURE_LOOP_SOURCE.md#stack-array-argument
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            auto callee = expr->func;
            if ( !func || !callee ) return Visitor::visit(expr);
            auto nArgs = min(expr->arguments.size(), callee->arguments.size());
            for ( size_t i=0; i!=nArgs; ++i ) {
                auto arg = expr->arguments[i];
                auto mka = heapArrayLiteral(arg);
                if ( !mka ) continue;
                auto & param = callee->arguments[i];
                auto argument = "argument '" + param->name + "' of '" + callee->name + "'";
                string why;
                if ( !param->type || !param->type->isGoodArrayType() ) why += "\tthe parameter is not declared as an array\n";
                else if ( !param->type->constant ) why += "\tthe parameter is not constant\n";
                if ( literalElementsOwnHeap(mka) ) why += "\tthe elements own heap\n";
                if ( callee->result && !callee->result->isVoid() && canHoldArray(callee->result) )
                    why += "\tthe result " + callee->result->describe() + " may hold a reference to the array\n";
                why += inScopePodRefusal(func, InScopePodRewrite::stackLiteral);
                if ( !why.empty() ) {
                    logRefused(arg->at, "stack array literal refused for " + argument, why);
                    continue;
                }
                mka->makeArrayOnHeap = false;
                auto tempArray = new ExprCall(arg->at, "_::to_array_temp");
                tempArray->arguments.push_back(mka);
                tempArray->alwaysSafe = true;
                expr->arguments[i] = tempArray;
                anyWork = true;
                func->notInferred();
                log(arg->at, "Stack array literal applied to " + argument);
            }
            return Visitor::visit(expr);
        }
    protected:
        Program * program = nullptr;
        Function * func = nullptr;
        TextWriter * logs = nullptr;
        vector<ExprBlock *> blocks;
    };

    bool Program::inScopePodAnalysis(TextWriter & logs) {
        auto forceInscopePod = options.getBoolOption("force_inscope_pod", policies.force_inscope_pod);
        if ( !forceInscopePod ) return false;
        auto logInscopePod = options.getBoolOption("log_inscope_pod", policies.log_inscope_pod);
        PodVisitor pv(this, logInscopePod ? &logs : nullptr);
        visit(pv);
        return pv.anyWork;
    }

}
