#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_const_visitor.h"
#include "daScript/ast/ast_visitor.h"

#if defined(STANDALONE_MODE)
#include "../das/ast/_standalone_ctx_generated/ast_print.das.h"
#endif


namespace das {

    // print flags are sticky node bits; without a wipe the output of a subtree print depends on
    // what larger trees the node was printed inside before (describe() must be pure — CSE keys on it).
    // opts into quote AND assume interiors — everything the Printer below descends into.
    class ClearPrinterFlags : public Visitor {
        virtual bool canVisitQuoteSubexpression ( ExprQuote * ) override { return true; }
        virtual bool canVisitWithAliasSubexpression ( ExprAssume * ) override { return true; }
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            expr->printFlags = 0;
        }
    };

    // src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-workers
    struct PrintLevels {
        bool    top = false;
        bool    arg = false;
        bool    bottom = false;
    };
    typedef das_hash_map<const Expression *, PrintLevels> PrintLevelMap;

    class SetPrinterFlags : public ConstVisitor {
    public:
        PrintLevelMap levels;
    protected:
    // ExprBlock
        virtual void preVisitBlockExpression ( const ExprBlock * block, const Expression * expr ) override {
            ConstVisitor::preVisitBlockExpression(block, expr);
            levels[expr].top = true;
        }
    // ExprNew
        virtual void preVisitNewArg ( const ExprNew * call, const Expression * expr , bool last ) override {
            ConstVisitor::preVisitNewArg(call,expr,last);
            levels[expr].arg = true;
        }
    // ExprCall
        virtual void preVisitCallArg ( const ExprCall * call, const Expression * expr , bool last ) override {
            ConstVisitor::preVisitCallArg(call,expr,last);
            levels[expr].arg = true;
        }
    // ExprLooksLikeCall
        virtual void preVisitLooksLikeCallArg ( const ExprLooksLikeCall * call, const Expression * expr , bool last ) override {
            ConstVisitor::preVisitLooksLikeCallArg(call,expr,last);
            levels[expr].arg = true;
        }
    // ExprIfThenElse
        virtual void preVisit ( const ExprIfThenElse * expr ) override {
            ConstVisitor::preVisit(expr);
            levels[expr->cond].arg = true;
        }
    // ExprWhile
        virtual void preVisit ( const ExprWhile * expr ) override {
            ConstVisitor::preVisit(expr);
            levels[expr->cond].arg = true;
        }
    // ExprReturn
        virtual void preVisit ( const ExprReturn * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->subexpr ) levels[expr->subexpr].arg = true;
        }
    // ExprCopy
        virtual void preVisit ( const ExprCopy * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( topOrArg(expr) ) levels[expr->right].arg = true;
        }
    // ExprClone
        virtual void preVisit ( const ExprClone * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( topOrArg(expr) ) levels[expr->right].arg = true;
        }
    // ExprVar
        virtual void preVisit ( const ExprVar * expr ) override {
            ConstVisitor::preVisit(expr);
            levels[expr].bottom = true;
        }
    // ExprTypeInfo
        virtual void preVisit ( const ExprTypeInfo * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->subexpr ) levels[expr->subexpr].arg = true;
        }
    // ExprArrayComprehension
        virtual void preVisit ( const ExprArrayComprehension * expr ) override {
            ConstVisitor::preVisit(expr);
            levels[expr->subexpr].arg = true;
            if ( expr->exprWhere ) levels[expr->exprWhere].arg = true;
        }
        bool topOrArg ( const Expression * expr ) const {
            auto it = levels.find(expr);
            return it != levels.end() && (it->second.top || it->second.arg);
        }
    };

    class Printer : public ConstVisitor {
    public:
        Printer ( const PrintLevelMap & lv, const Program * program = nullptr ) : levels(lv) {
            if ( program ) {
                printRef = program->options.getBoolOption("print_ref");
                printVarAccess = program->options.getBoolOption("print_var_access");
                printAliases= program->options.getBoolOption("log_aliasing");
                printUse = program->options.getBoolOption("print_use");
                gen2 = program->policies.version_2_syntax;
                printCStyle = program->options.getBoolOption("print_c_style");
            }
            // gen2 always implies C-style braces; keep these consistent even when
            // there is no program (describe_function / describe_expression path).
            printCStyle = printCStyle || gen2;
        }
        string str() const { return ss.str(); };
        bool printRef = false;
        bool printVarAccess = false;
        bool printCStyle = false;
        bool printAliases = false;
        bool printUse = false;
        bool gen2 = true;
    protected:
        void newLine () {
            auto nlPos = ss.tellp();
            if ( nlPos != lastNewLine ) {
                ss << "\n";
                lastNewLine = ss.tellp();
            }
        }
        const PrintLevelMap & levels;
        PrintLevels level ( const Expression * expr ) const {
            auto it = levels.find(expr);
            return it != levels.end() ? it->second : PrintLevels();
        }
        bool noBracket ( const Expression * expr ) const {
            auto lv = level(expr);
            return lv.top || lv.bottom || lv.arg;
        }
    // can we see inside quote
        virtual bool canVisitQuoteSubexpression ( const ExprQuote * ) override { return true; }
    // TYPE
        bool ET = false;
        virtual void preVisit ( const TypeDecl * td ) override {
            ConstVisitor::preVisit(td);
            ET = td->isExprType();
            if ( ET ) ss << "/*[";
        }
        virtual void visit ( const TypeDecl * td ) override {
            if ( ET ) ss << "]*/";
            return ConstVisitor::visit(td);

        }
    // enumeration
        virtual void preVisit ( const Enumeration * enu ) override {
            ConstVisitor::preVisit(enu);
            ss << "enum ";
            ss << (enu->isPrivate ? "private " : "public ") << enu->name << " : " << das_to_string(enu->baseType) << "\n";
            if ( gen2 ) {
                ss << "{\n";
            }
        }
        virtual void preVisitEnumerationValue ( const Enumeration * enu, const string & name, const Expression * value, bool last ) override {
            ConstVisitor::preVisitEnumerationValue(enu, name, value, last);
            ss << "\t" << name << " = ";
        }
        virtual void visitEnumerationValue ( const Enumeration * enu, const string & name, const Expression * value, bool last ) override {
            if ( gen2 ) {
                if ( !last ) ss << ",";
                ss << "\n";
            } else {
                ss << "\n";
            }
            return ConstVisitor::visitEnumerationValue(enu, name, value, last);
        }
        virtual void visit ( const Enumeration * enu ) override {
            if ( gen2 ) ss << "}\n";
            ss << "\n";
            return ConstVisitor::visit(enu);
        }
    // strcuture
        void logAnnotationArguments(const AnnotationArgumentList & arguments) {
            bool first = true;
            for (const auto & aarg : arguments) {
                if (first) first = false; else ss << ",";
                ss << aarg.name << "=";
                switch (aarg.type) {
                case Type::tInt:    ss << aarg.iValue; break;
                case Type::tInt64:    ss << aarg.lValue << "l"; break;
                case Type::tUInt64:    ss << aarg.ulValue << "ul"; break;
                case Type::tFloat:  ss << aarg.fValue; break;
                case Type::tBool:   ss << aarg.bValue; break;
                case Type::tString: ss << "\"" << aarg.sValue << "\""; break;
                default:
                    DAS_ASSERT(0 && "we should not even be here. "
                        "somehow annotation has unsupported argument type");
                    break;
                }
            }
        }
        void logAnnotations(const AnnotationList & annList) {
            if (!annList.empty()) {
                ss << "[";
                bool first = true;
                for (const auto & ann : annList) {
                    if ( first ) first = false; else ss << ",";
                    ann->annotation->log(ss, *ann);
                }
                ss << "]\n";
            }
        }
        virtual void preVisit ( const Structure * that ) override {
            ConstVisitor::preVisit(that);
            logAnnotations(that->annotations);
            if ( that->hasDefaultInitializer ) ss << "// has default initializer " << that->name << "()\n";
            if ( that->macroInterface ) ss << "[macro_interface]\n";
            if ( that->isTemplate ) ss << "template ";
            ss << (that->isClass ? "class " : "struct ");
            ss << (that->privateStructure ? "private " : "public ") << that->name;
            if ( that->parent ) {
                ss << " : " << that->parent->name;
            }
            ss << "\n";
            if ( gen2 ) ss << "{\n";
        }
        void outputVariableAnnotation ( const AnnotationArgumentList & annotation ) {
            if ( annotation.size() ) {
                ss << "[";
                int ai = 0;
                for ( auto & arg : annotation ) {
                    if ( ai++ ) ss << ",";
                    ss << arg.name << "=";
                    switch ( arg.type ) {
                        case Type::tInt:        ss << arg.iValue; break;
                        case Type::tInt64:        ss << arg.lValue << "l"; break;
                        case Type::tUInt64:        ss << arg.ulValue << "ul"; break;
                        case Type::tFloat:      ss << arg.fValue; break;
                        case Type::tBool:       ss << (arg.bValue ? "true" : "false"); break;
                        case Type::tString:     ss << "\"" << arg.sValue << "\""; break;
                        default:    DAS_ASSERTF(false,"this type is not allowed in annotation");
                    }
                }
                ss << "] ";
            }
        }
        virtual void preVisitStructureAlias ( const Structure * var, const string & name, const TypeDecl * at ) override {
            ConstVisitor::preVisitStructureAlias(var, name, at);
            ss << "\ttypedef " << name << " = " << at->describe();
            if ( gen2 ) ss << ";";
            ss << "\n";
        }

        virtual void preVisitStructureField ( const Structure * that, const Structure::FieldDeclaration & decl, bool last ) override {
            ConstVisitor::preVisitStructureField(that, decl, last);
            ss << "\t";
            outputVariableAnnotation(decl.annotation);
            if ( decl.privateField ) ss << "private ";
            ss << decl.name << " : " << decl.type->describe();
            if ( decl.parentType ) {
                ss << " /* from " << that->parent->name << " */";
            }
            if ( decl.init ) {
                ss << (decl.moveSemantics ? " <- " : " = ");
            }
        }
        virtual void visitStructureField ( const Structure * var, const Structure::FieldDeclaration & decl, bool last ) override {
            if ( decl.capturedConstant ) ss << " // captured constant";
            if ( gen2 ) ss << ";";
            ss << "\n";
            ConstVisitor::visitStructureField(var, decl, last);
        }
        virtual void visit ( const Structure * that ) override {
            if ( gen2 ) ss << "}\n";
            ss << "\n";
            return ConstVisitor::visit(that);
        }
    // alias
        virtual void preVisitAlias ( const TypeDecl * td, const string & name ) override {
            ConstVisitor::preVisitAlias(td,name);
            ss  << "typedef " << name << " = " << td->describe();
            if ( gen2 ) ss << ";";
            ss << "\n\n";
        }
    // global
        virtual void preVisitGlobalLet ( const Variable * var ) override {
            ConstVisitor::preVisitGlobalLet(var);
            if ( printUse ) {
                if ( var->useFunctions.size() ) {
                    ss << "// use functions";
                    for ( auto & ufn : var->useFunctions ) {
                        ss << " " << ufn->getMangledName();
                    }
                    ss << "\n";
                }
                if ( var->useGlobalVariables.size() ) {
                    ss << "// use global variables";
                    for ( auto & uvar : var->useGlobalVariables ) {
                        ss << " " << uvar->getMangledName();
                    }
                    ss << "\n";
                }
            }
            ss  << (var->type->constant ? "let" : "var")
                << (var->global_shared ? " shared" : "")
                << (var->private_variable ? " private" : "")
                << "\n\t";
            outputVariableAnnotation(var->annotation);
            if ( printUse && var->isAccessUnused() ) ss << " /*unused*/ ";
            if ( printVarAccess && var->access_ref ) ss << "/*ref*/";
            if ( printVarAccess && var->access_pass ) ss << "/*pass*/";
            if ( printVarAccess && var->access_get ) ss << "/*get*/";
            if ( printVarAccess && var->access_info_pass_mutable ) ss << "/*pass_mut*/";
            ss << var->name << " : " << var->type->describe();
        }
        virtual void visitGlobalLet ( const Variable * var ) override {
            ss << "\n\n";
            return ConstVisitor::visitGlobalLet(var);
        }
        virtual void preVisitGlobalLetInit ( const Variable * var, const Expression * init ) override {
            ConstVisitor::preVisitGlobalLetInit(var, init);
            if ( var->init_via_move ) ss << " <- ";
            else if ( var->init_via_clone ) ss << " := ";
            else ss << " = ";
        }
    // function
        virtual void preVisit ( const Function * fn) override {
            ConstVisitor::preVisit(fn);
            if ( fn->isFullyInferred ) {
                ss << "/*FULLY INFERRED*/\n";
            }
            if ( fn->unsafeFunction ) {
                ss << "/*unsafe*/ ";
            }
            if ( fn->knownSideEffects ) {
                if ( !fn->hasObservableEffects() ) {
                ss << "[nosideeffects]\n";
                } else {
                    ss << "// ";
                    if ( fn->userScenario ) { ss << "[user_scenario]"; }
                    if ( fn->modifyExternal ) { ss << "[modify_external]"; }
                    if ( fn->modifyArgument ) { ss << "[modify_argument]"; }
                    if ( fn->accessGlobal ) { ss << "[access_global]"; }
                    if ( fn->invoke ) { ss << "[invoke]"; }
                    if ( fn->captureString ) { ss << "[capture_string]"; }
                    if ( fn->callCaptureString ) { ss << "[call_capture_string]"; }
                    ss << "\n";
                }
            }
            if ( fn->moreFlags ) {
                ss << "// ";
                if ( fn->macroFunction ) { ss << "[macro_function]"; }
                if ( fn->aotHashDeppendsOnArguments ) { ss << "[aot_hash_deppends_on_arguments]"; }
                if ( fn->requestJit ) { ss << "[jit]"; }
                if ( fn->requestNoJit ) { ss << "[no_jit]"; }
                if ( fn->nodiscard ) { ss << "[nodiscard]"; }
                ss << "\n";
            }
            if ( fn->tempStringResult ) { ss << "// [temp_string_result]\n"; }
            if ( fn->noCaptureString ) { ss << "// [no_capture_string]\n"; }
            if ( fn->fastCall ) { ss << "[fastcall]\n"; }
            // if ( fn->addr ) { ss << "[addr]\n"; }
            if ( fn->exports ) { ss << "[export]\n"; }
            if ( fn->init ) { ss << "[init" << (fn->lateInit ? "(late)" : "") << "]\n"; }
            if ( fn->macroInit ) { ss << "[macro_init]\n"; }
            if ( fn->macroFunction ) { ss << "[macro_function]\n"; }
            if ( fn->shutdown ) { ss << "[finalize" << (fn->lateShutdown ? "(late)" : "") << "]\n"; }
            if ( fn->unsafeDeref ) { ss << "[unsafe_deref]\n"; }
            if ( fn->unsafeOperation ) { ss << "[unsafe_operation]\n"; }
            if ( fn->isClassMethod ) {
                ss << "[";
                ss << (fn->isStaticClassMethod ? "static_class_method" : "class_method");
                if ( fn->isConstClassMethod ) {
                    ss << " const";
                }
                ss << "(" << fn->classParent->getMangledName() << ")]\n";
            }
            if ( fn->generator ) { ss << "[GENERATOR]\n"; }
            if ( fn->lambda ) { ss << "[LAMBDA]\n"; }
            if ( fn->hasTryRecover ) { ss << "[has_try_recover]\n"; }
            logAnnotations(fn->annotations);
            if ( printAliases ) {
                if ( fn->resultAliases.size() ) {
                    ss << "// cmres aliases arguments";
                    for ( auto ai : fn->resultAliases ) {
                        ss << " " << fn->arguments[ai]->name;
                    }
                    ss << "\n";
                }
                if ( fn->resultAliasesGlobals.size() ) {
                    for ( auto & vinfo : fn->resultAliasesGlobals ) {
                        ss << "// cmres " << (vinfo.viaPointer ? "always " : "") << "aliases " << vinfo.var->getMangledName();
                        if ( fn != vinfo.func ) {
                            ss << " via function " << vinfo.func->getMangledName();
                        }
                        ss << "\n";
                    }
                }
            }
            if ( printUse ) {
                if ( fn->useFunctions.size() ) {
                    ss << "// use functions";
                    for ( auto & ufn : fn->useFunctions ) {
                        ss << " " << ufn->getMangledName();
                    }
                    ss << "\n";
                }
                if ( fn->useGlobalVariables.size() ) {
                    ss << "// use global variables";
                    for ( auto & uvar : fn->useGlobalVariables ) {
                        ss << " " << uvar->getMangledName();
                    }
                    ss << "\n";
                }
            }
            if ( fn->fromGeneric ) {
                ss << "// from generic " << fn->fromGeneric->module->name << "::" << fn->fromGeneric->describe() << "\n";
            }
            if ( fn->isTemplate ) {
                ss << "template ";
            }
            ss << "def " << (fn->privateFunction ? "private " : "public ") << fn->name;
            if ( fn->arguments.size() ) ss << "(";
        }
        virtual void preVisitFunctionBody ( const Function * fn,const Expression * expr ) override {
            ConstVisitor::preVisitFunctionBody(fn,expr);
            if ( fn->arguments.size() ) ss << ")";
            if ( fn->result ) ss << " : " << fn->result->describe();
            if ( !gen2 ) ss << "\n";
        }
        virtual void preVisitArgument ( const Function * fn, const Variable * arg, bool last ) override {
            ConstVisitor::preVisitArgument(fn,arg,last);
            if (!arg->annotation.empty()) {
                ss << "[";
                logAnnotationArguments(arg->annotation);
                ss << "] ";
            }
            if ( !arg->type->isConst() ) ss << "var ";
            if ( printUse && arg->isAccessUnused() ) ss << " /*unused*/ ";
            if ( printUse && arg->no_capture ) ss << " /*no_capture*/ ";
            if ( printVarAccess && arg->access_ref ) ss << "/*ref*/";
            if ( printVarAccess && arg->access_pass ) ss << "/*pass*/";
            if ( printVarAccess && arg->access_get ) ss << "/*get*/";
            if ( printVarAccess && arg->access_info_pass_mutable ) ss << "/*pass_mut*/";
            ss << arg->name;
            if ( !arg->aka.empty() ) ss << " aka " << arg->aka;
            ss << ":" << arg->type->describe();
        }
        virtual void preVisitArgumentInit ( const Function * fn, const Variable * arg, const Expression * expr ) override {
            ConstVisitor::preVisitArgumentInit(fn,arg,expr);
            ss << " = ";
        }
        virtual void visitArgument ( const Function * fn, const Variable * that, bool last ) override {
            if ( !last ) ss << "; ";
            return ConstVisitor::visitArgument(fn, that, last);
        }
        virtual void visit ( const Function * fn ) override {
            ss << "\n\n";
            return ConstVisitor::visit(fn);
        }
    // Ref2Value
        virtual void preVisit ( const ExprRef2Value * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( printRef ) ss << "r2v(";
        }
        virtual void visit ( const ExprRef2Value * expr ) override {
            if ( printRef ) ss << ")";
            return ConstVisitor::visit(expr);
        }
    // make block
        void describeCapture ( const vector<CaptureEntry> & capture ) {
            if ( !capture.empty() ) {
                ss << " [[";
                bool first = true;
                for ( const auto & ce : capture ) {
                    if ( first ) first = false; else ss << ",";
                    switch ( ce.mode ) {
                        case CaptureMode::capture_by_clone:     ss << ":="; break;
                        case CaptureMode::capture_by_copy:      ss << "="; break;
                        case CaptureMode::capture_by_move:      ss << "<-"; break;
                        case CaptureMode::capture_by_reference: ss << "&"; break;
                        default:;
                    }
                    ss << ce.name;
                }
                ss << "]] ";
            }
        }
        virtual void preVisit ( const ExprMakeBlock * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->isLambda ) ss << "@";
            else if ( expr->isLocalFunction ) ss << "@@";
            else ss << "$";
            describeCapture(expr->capture);
        }
    // block
        virtual void preVisitBlockExpression ( const ExprBlock * block, const Expression * expr ) override {
            ConstVisitor::preVisitBlockExpression(block, expr);
            ss << string(tab,'\t');
        }
        bool needsTrailingSemicolumn ( const Expression * that ) {
            if ( that->rtti_isIfThenElse() ) return false;
            if ( that->rtti_isFor() ) return false;
            if ( that->rtti_isWhile() ) return false;
            if ( that->rtti_isWith() ) return false;
            if ( gen2 ) {
                if ( that->rtti_isBlock() ) return false;
                if ( that->rtti_isUnsafe() ) return false;
                if ( strcmp(that->__rtti,"ExprTryCatch")==0 ) return false;
            }
            return true;
        }
        virtual void visitBlockExpression ( const ExprBlock * block, const Expression * that ) override {
            if ( printCStyle || block->isClosure ) {
                if ( needsTrailingSemicolumn(that) ) {
                    ss << ";";
                }
            }
            newLine();
            return ConstVisitor::visitBlockExpression(block, that);
        }
        virtual void preVisit ( const ExprBlock * block ) override {
            ConstVisitor::preVisit(block);
            if ( block->isClosure ) {
                logAnnotations(block->annotations);
                if ( block->returnType || block->arguments.size() ) {
                    ss << "(";
                    for ( auto & arg : block->arguments ) {
                        if (!arg->annotation.empty()) {
                            ss << "[";
                            logAnnotationArguments(arg->annotation);
                            ss << "] ";
                        }
                        if ( !arg->type->isConst() ) ss << "var ";
                        ss << arg->name;
                        if ( !arg->aka.empty() ) ss << " aka " << arg->aka;
                        ss << ":" << arg->type->describe();
                        if ( arg != block->arguments.back() )
                            ss << "; ";
                    }
                    ss << ")";
                    if ( block->returnType ) {
                        ss << ":" << block->returnType->describe();
                    }
                    if ( !gen2 ) ss << "\n";
                }
            }
            if ( printCStyle || block->isClosure ) {
                if ( block->isClosure ) {
                    if ( gen2 ) {
                        ss << " {\n";
                    } else {
                        ss << string(tab,'\t') << "{\n";
                    }
                } else if ( level(block).top ) {
                    ss << "{\n";
                } else if ( gen2 ) {
                    ss << " {\n";
                } else {
                    ss << string(tab,'\t') << "{\n";
                }
            }
            tab ++;
        }
        virtual void visit ( const ExprBlock * block ) override {
            tab --;
            if ( printCStyle || block->isClosure ) ss << string(tab,'\t') << "}";
            return ConstVisitor::visit(block);
        }
        virtual void preVisitBlockFinal ( const ExprBlock * block ) override {
            ConstVisitor::preVisitBlockFinal(block);
            if ( printCStyle || block->isClosure ) {
                ss << string(tab-1,'\t') << "} finally {\n";
            } else {
                ss << string(tab-1,'\t') << "finally\n";
            }
        }
        virtual void preVisitBlockFinalExpression ( const ExprBlock * block, const Expression * expr ) override {
            ConstVisitor::preVisitBlockFinalExpression(block, expr);
            ss << string(tab,'\t');
        }
        virtual void visitBlockFinalExpression ( const ExprBlock * block, const Expression * that ) override {
            if ( printCStyle || block->isClosure ) {
                if ( needsTrailingSemicolumn(that) ) {
                    ss << ";";
                }
            }
            newLine();
            return ConstVisitor::visitBlockFinalExpression(block, that);
        }
    // string builder
        virtual void preVisit ( const ExprStringBuilder * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "string_builder";
            if ( expr->isTempString )
                ss << "_temp";
            ss << "(";
        }
        virtual void visitStringBuilderElement ( const ExprStringBuilder * sb, const Expression * expr, bool last ) override {
            if ( !last ) ss << ", ";
            return ConstVisitor::visitStringBuilderElement(sb, expr, last);
        }
        virtual void visit ( const ExprStringBuilder * expr ) override {
            ss << ")";
            return ConstVisitor::visit(expr);
        }
    // label
        virtual void preVisit ( const ExprReader * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "#" << expr->macro->name << " " << expr->sequence << " ";
        }
    // label
        virtual void preVisit ( const ExprLabel * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "label " << expr->label << ":";
            if ( !expr->comment.empty() ) {
                ss << " /*" << expr->comment << "*/";
            }
        }
    // goto
        virtual void preVisit ( const ExprGoto * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "goto ";
            if ( !expr->subexpr )
                ss << "label " << expr->label;
        }
    // let
        virtual void preVisit ( const ExprLet * let ) override {
            ConstVisitor::preVisit(let);
            bool isLet = true;
            if ( let->variables.size() ) {
                auto pVar = let->variables.back();
                if ( pVar->type && !pVar->type->isConst() ) {
                    isLet = false;
                }
            }
            ss << (isLet ? "let " : "var ") << (let->inScope ? "inscope " : "");
        }
        virtual void preVisitLet ( const ExprLet * let, const Variable * var, bool last ) override {
            ConstVisitor::preVisitLet(let, var, last);
            if ( printUse && var->isAccessUnused() ) ss << " /*unused*/ ";
            if ( printVarAccess && var->access_ref ) ss << "/*ref*/";
            if ( printVarAccess && var->access_pass ) ss << "/*pass*/";
            if ( printVarAccess && var->access_get ) ss << "/*get*/";
            if ( printVarAccess && var->access_info_pass_mutable ) ss << "/*pass_mut*/";
            outputVariableAnnotation(var->annotation);
            ss << var->name;
            if ( !var->aka.empty() ) ss << " aka " << var->aka;
            if ( printAliases && var->aliasCMRES ) ss << "/*cmres*/";
            if ( var->early_out ) ss << "/*early_out*/";
            if ( var->used_in_finally ) ss << "/*used_in_finally*/";
            if ( var->consumed ) ss << "/*consumed*/";
            if ( var->single_return_via_move ) ss << "/*single_return_via_move*/";
            ss << ":" << var->type->describe();
        }
        virtual void visitLet ( const ExprLet * let, const Variable * var, bool last ) override {
            if ( !last ) ss << "; ";
            return ConstVisitor::visitLet(let, var, last);
        }
        virtual void preVisitLetInit ( const ExprLet * let, const Variable * var, const Expression * expr ) override {
            ConstVisitor::preVisitLetInit(let,var,expr);
            if ( var->init_via_move ) ss << " <- ";
            else if ( var->init_via_clone ) ss << " := ";
            else ss << " = ";
        }
    // if then else
        virtual void preVisit ( const ExprIfThenElse * ifte ) override {
            ConstVisitor::preVisit(ifte);
            ss << (ifte->isStatic ? "static_if " : "if ");
            if ( gen2 ) ss << "( ";
        }
        virtual void preVisitIfBlock ( const ExprIfThenElse * ifte, const Expression * block ) override {
            ConstVisitor::preVisitIfBlock(ifte,block);
            if ( gen2 ) ss <<  " )";
            if ( !gen2 ) ss << "\n";
        }
        virtual void preVisitElseBlock ( const ExprIfThenElse * ifte, const Expression * block ) override {
            ConstVisitor::preVisitElseBlock(ifte, block);
            if ( gen2 ) {
                ss << " ";
            } else {
                ss << "\n" << string(tab,'\t');
            }
            if (block && block->rtti_isIfThenElse()) {
                ss << (ifte->isStatic ? "static_el" : "el");
            } else {
                ss << (ifte->isStatic ? "static_else" : "else");
                if ( !gen2 ) ss << "\n";
            }
        }
    // try-catch
        virtual void preVisit ( const ExprTryCatch * tc ) override {
            ConstVisitor::preVisit(tc);
            ss << "try";
            if ( !gen2 ) ss << "\n";
        }
        virtual void preVisitCatch ( const ExprTryCatch * tc, const Expression * block ) override {
            ConstVisitor::preVisitCatch(tc, block);
            if ( gen2 ) {
                ss << " recover";
            } else {
                ss << string(tab,'\t') << "recover\n";
            }
        }
    // for
        virtual void preVisit ( const ExprFor * ffor ) override {
            ConstVisitor::preVisit(ffor);
            ss << "for ";
            if ( gen2 ) ss << "( ";
            bool first = true;
            for ( auto i : ffor->iterators ) {
                if ( first) first = false; else ss << ", ";
                ss << i;
            }
            ss << " in ";
        }
        virtual void preVisitForBody ( const ExprFor * ffor, const Expression * body ) override {
            ConstVisitor::preVisitForBody(ffor, body);
            if ( gen2 ) ss << " )";
            if ( !gen2 ) ss << "\n";
        }
        virtual void visitForSource ( const ExprFor * ffor, const Expression * that , bool last ) override {
            if ( !last ) ss << ",";
            return ConstVisitor::visitForSource(ffor, that, last);
        }
    // with
        virtual void preVisit ( const ExprWith * wh ) override {
            ConstVisitor::preVisit(wh);
            ss << "with ";
            // module flavor is gen2-only surface, but generated nodes can land in a
            // gen1 program (the inliner's scope wraps) - always parenthesize it
            if ( gen2 || wh->isModuleWith() ) ss << "( ";
            if ( wh->isModuleWith() ) ss << "module " << wh->moduleName;
        }
        virtual void preVisitWithBody ( const ExprWith * wh, const Expression * body ) override {
            ConstVisitor::preVisitWithBody(wh,body);
            if ( gen2 || wh->isModuleWith() ) ss << " )";
            if ( !gen2 && !wh->isModuleWith() ) ss << "\n";
        }
    // with alias
        virtual bool canVisitWithAliasSubexpression ( const ExprAssume * ) override {
            return true;
        }
        virtual void preVisit ( const ExprAssume * wh ) override {
            ConstVisitor::preVisit(wh);
            ss << "assume ";
            if ( wh->assumeType ) ss << "type ";
            ss << wh->alias << " = ";
            if ( wh->assumeType ) ss << wh->assumeType->describe();
        }
    // tag
        virtual void preVisit ( const ExprTag * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->name=="...") {
                ss << "...";
            } else {
                ss << "$" << expr->name << "(";
            }
        }
        virtual void preVisitTagValue ( const ExprTag * expr, const Expression * value ) override {
            ConstVisitor::preVisitTagValue(expr,value);
            if ( expr->name!="...") {
                ss << ")(";
            }
        }
        virtual void visit ( const ExprTag * expr ) override {
            if ( expr->name!="...") {
                ss << ")";
            }
            return ConstVisitor::visit(expr);
        }
    // while
        virtual void preVisit ( const ExprWhile * wh ) override {
            ConstVisitor::preVisit(wh);
            ss << "while ";
            if ( gen2 ) ss << "( ";
        }
        virtual void preVisitWhileBody ( const ExprWhile * wh, const Expression * body ) override {
            ConstVisitor::preVisitWhileBody(wh,body);
            if ( gen2 ) ss << " )";
            if ( !gen2 ) ss << "\n";
        }
    // while
        virtual void preVisit ( const ExprUnsafe * wh ) override {
            ConstVisitor::preVisit(wh);
            ss << "unsafe";
            if ( !gen2 ) ss << "\n";
        }
    // call
        virtual void preVisit ( const ExprCall * call ) override {
            ConstVisitor::preVisit(call);
            ss << call->name;
            if ( printAliases && call->doesNotNeedSp ) ss << " /*no_sp*/ ";
            ss << "(";
        }
        virtual void visitCallArg ( const ExprCall * call, const Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return ConstVisitor::visitCallArg(call, arg, last);
        }
        virtual void visit ( const ExprCall * c ) override {
            ss << ")";
            return ConstVisitor::visit(c);
        }
    // named call
        virtual void preVisit ( const ExprNamedCall * call ) override {
            ConstVisitor::preVisit(call);
            ss << call->name << "(";
        }
        virtual void preVisitNamedCallArg ( const ExprNamedCall * call, const MakeFieldDecl * arg, bool last ) override {
            if ((*call->arguments)[0] == arg) {
                if (call->nonNamedArguments.size() > 0) ss << ",";
                ss << "[";
            }
            ConstVisitor::preVisitNamedCallArg(call, arg, last);
            ss << arg->name << (arg->moveSemantics ? "<-" : "=" );
        }
        virtual void visitNamedCallArg ( const ExprNamedCall * call, const MakeFieldDecl * arg, bool last ) override {
            if ( !last ) ss << ",";
            return ConstVisitor::visitNamedCallArg(call, arg, last);
        }
        virtual void visit ( const ExprNamedCall * c ) override {
            ss << "])";
            return ConstVisitor::visit(c);
        }
    // looks like call
        virtual void preVisit ( const ExprCallMacro * call ) override {
            ConstVisitor::preVisit(call);
            ss << call->name << "(";
        }
        virtual void visit ( const ExprCallMacro * c ) override {
            ss << ")";
            return ConstVisitor::visit(c);
        }
    // looks like call
        virtual void preVisit ( const ExprLooksLikeCall * call ) override {
            ConstVisitor::preVisit(call);
            ss << call->name;
            if ( call->rtti_isInvoke() ) {
                auto * inv = (ExprInvoke *) call;
                if ( printAliases && inv->doesNotNeedSp ) ss << " /*no_sp*/ ";
                if ( inv->isInvokeMethod ) ss << "/*method*/";
            }
            ss << "(";
        }
        virtual void visitLooksLikeCallArg ( const ExprLooksLikeCall * call, const Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return ConstVisitor::visitLooksLikeCallArg(call, arg, last);
        }
        virtual void visit ( const ExprLooksLikeCall * c ) override {
            ss << ")";
            return ConstVisitor::visit(c);
        }
    // const
        virtual void visit(const ExprFakeContext * c) override {
            ss << "__context__";
            return ConstVisitor::visit(c);
        }
        virtual void visit(const ExprFakeLineInfo * c) override {
            ss << "__lineinfo__";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstPtr * c ) override {
            if ( c->ptrType ) {
                ss << "const_pointer<" << c->ptrType->describe() << ">(";
            }
            if ( c->getValue() ) {
                ss << "*0x" << HEX << intptr_t(c->getValue()) << DEC;
            } else {
                ss << "null";
            }
            if ( c->ptrType ) {
                ss << ")";
            }
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstEnumeration * c ) override {
            if ( c->enumType->module && !c->enumType->module->name.empty() ) {
                ss << c->enumType->module->name << "::";
            }
            ss << c->enumType->name << "." << c->text;
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt * c ) override {
            ss << c->getValue();
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt8 * c ) override {
            ss << int32_t(c->getValue()) << "/*i8*/";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt8 * c ) override {
            ss << "0x" << HEX << uint32_t(c->getValue()) << DEC << "u8";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt16 * c ) override {
            ss << int32_t(c->getValue()) << "/*i16*/";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt16 * c ) override {
            ss << "0x" << HEX << uint32_t(c->getValue()) << DEC <<  "/*u16*/";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt64 * c ) override {
            ss << c->getValue();
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt64 * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC;
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt * c ) override {
            ss << "0x" << HEX << c->getValue() << DEC;
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstBitfield * c ) override {
            string name;
            if ( c->bitfieldType && !c->bitfieldType->alias.empty() ) {
                name = c->bitfieldType->findBitfieldName(c->getValue());
            }
            if ( !name.empty() ) {
                ss << c->bitfieldType->alias << "." << name;
            } else {
                ss << "bitfield";
                switch ( c->bitfieldType ? c->bitfieldType->baseType : Type::tBitfield  ) {
                    case Type::tBitfield8: ss << "8"; break;
                    case Type::tBitfield16: ss << "16"; break;
                    case Type::tBitfield64: ss << "64"; break;
                    default: break;
                }
                ss << "(0x" << HEX << c->getValue() << DEC << ")";
            }
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstBool * c ) override {
            ss << (c->getValue() ? "true" : "false");
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstDouble * c ) override {
            ss << c->getValue() << "lf";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstFloat * c ) override {
            ss << c->getValue() << "f";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstFloat16 * c ) override {
            ss << c->getValue().toFloat() << "h";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstString * c ) override {
            ss << "\"" << escapeString(c->text) << "\"";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt2 * c ) override {
            auto val = c->getValue();
            ss << "int2(" << val.x << "," << val.y << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstRange * c ) override {
            auto val = c->getValue();
            ss << "range(" << val.from << "," << val.to << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstRange64 * c ) override {
            auto val = c->getValue();
            ss << "range64(" << val.from << "," << val.to << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt3 * c ) override {
            auto val = c->getValue();
            ss << "int3(" << val.x << "," << val.y << "," << val.z << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstInt4 * c ) override {
            auto val = c->getValue();
            ss << "int4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt2 * c ) override {
            auto val = c->getValue();
            ss << "uint2(" << val.x << "," << val.y << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstURange * c ) override {
            auto val = c->getValue();
            ss << "urange(" << val.from << "," << val.to << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstURange64 * c ) override {
            auto val = c->getValue();
            ss << "urange64(" << val.from << "," << val.to << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt3 * c ) override {
            auto val = c->getValue();
            ss << "uint3(" << val.x << "," << val.y << "," << val.z << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstUInt4 * c ) override {
            auto val = c->getValue();
            ss << "uint4(" << val.x << "," << val.y << "," << val.z << "," << val.w << ")";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstFloat2 * c ) override {
            auto val = c->getValue();
            ss << "float2(" << val.x << "f," << val.y << "f)";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstFloat3 * c ) override {
            auto val = c->getValue();
            ss << "float3(" << val.x << "f," << val.y << "f," << val.z << "f)";
            return ConstVisitor::visit(c);
        }
        virtual void visit ( const ExprConstFloat4 * c ) override {
            auto val = c->getValue();
            ss << "float4(" << val.x << "f," << val.y << "f," << val.z << "f," << val.w << "f)";
            return ConstVisitor::visit(c);
        }
    // var
        virtual void visit ( const ExprVar * var ) override {
            if ( printRef && var->r2v ) ss << "@";
            if ( printRef && var->r2cr ) ss << "$";
            if ( printRef && var->write ) ss << "#";
            ss << var->name;
            return ConstVisitor::visit(var);
        }
    // swizzle
        virtual void visit ( const ExprSwizzle * expr ) override {
            ss << (expr->no_promotion ? "!." : ".");
            if ( printRef && expr->r2v ) ss << "@";
            if ( printRef && expr->r2cr ) ss << "$";
            if ( printRef && expr->write ) ss << "#";
            if ( expr->fields.size() ) {
                for ( auto f : expr->fields ) {
                    switch ( f ) {
                        case 0:     ss << "x"; break;
                        case 1:     ss << "y"; break;
                        case 2:     ss << "z"; break;
                        case 3:     ss << "w"; break;
                        default:    ss << "?"; break;
                    }
                }
            } else {
                ss << expr->mask;
                if ( expr->value->type && expr->value->type->isVectorType() ) {
                    auto dim = expr->value->type->getVectorDim();
                    vector<uint8_t> dummy;
                    if ( !TypeDecl::buildSwizzleMask(expr->mask, dim, dummy) ) {
                        ss << "/*invalid swizzle*/";
                        return ConstVisitor::visit(expr);
                    }
                }
            }
            return ConstVisitor::visit(expr);
        }
    // field
        virtual void visit ( const ExprField * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << (field->no_promotion ? "!." : ".") << field->name;
            return ConstVisitor::visit(field);
        }
        virtual void visit ( const ExprSafeField * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << (field->no_promotion ? "!?." : ".?") << field->name;
            return ConstVisitor::visit(field);
        }
        virtual void visit ( const ExprIsVariant * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << (field->no_promotion ? " !is " : " is ") << field->name;
            return ConstVisitor::visit(field);
        }
        virtual void visit ( const ExprAsVariant * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << (field->no_promotion ? " !as " : " as ") << field->name;
            return ConstVisitor::visit(field);
        }
        virtual void visit ( const ExprSafeAsVariant * field ) override {
            if ( printRef && field->r2v ) ss << "@";
            if ( printRef && field->r2cr ) ss << "$";
            if ( printRef && field->write ) ss << "#";
            ss << (field->no_promotion ? " !?as " : " ?as ") << field->name;
            return ConstVisitor::visit(field);
        }
    // addr
        virtual void preVisit ( const ExprAddr * expr ) override {
            ss << "@@";
            if ( expr->funcType ) {
                ss << "<" << expr->funcType->describe() << ">";
            }
            ss << expr->target;
        }
    // ptr2ref
        virtual void preVisit ( const ExprPtr2Ref * ptr2ref ) override {
            ConstVisitor::preVisit(ptr2ref);
            ss << "deref(";
        }
        virtual void visit ( const ExprPtr2Ref * ptr2ref ) override {
            ss << ")";
            return ConstVisitor::visit(ptr2ref);
        }
     // ref2ptr
        virtual void preVisit ( const ExprRef2Ptr * ptr2ref ) override {
            ConstVisitor::preVisit(ptr2ref);
            ss << "addr(";
        }
        virtual void visit ( const ExprRef2Ptr * ptr2ref ) override {
            ss << ")";
            return ConstVisitor::visit(ptr2ref);
        }
    // delete
        virtual void preVisit ( const ExprDelete * edel ) override {
            ConstVisitor::preVisit(edel);
            ss << "delete ";
            if ( edel->native ) ss << "/*native*/ ";
        }
        virtual void preVisitDeleteSizeExpression ( const ExprDelete *, const Expression * ) override {
            ss << ", rtti.size = ";
        }
    // cast
        virtual void preVisit ( const ExprCast * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->reinterpret ) {
                ss << "reinterpret";
            } else if ( expr->upcast ) {
                ss << "upcast";
            } else {
                ss << "cast";
            }
            ss << "<" << expr->castType->describe() << "> ";
        }
    // ascend
        virtual void preVisit ( const ExprAscend * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->ascType ) {
                ss << "new<" << expr->ascType->describe() << "> ";
            } else {
                ss << "new ";
            }
        }
    // new
        virtual void preVisit ( const ExprNew * enew ) override {
            ConstVisitor::preVisit(enew);
            if ( enew->typeexpr ) {
                ss << "new " << enew->typeexpr->describe();
            } else {
                ss << "new ???";
            }
            if ( enew->initializer ) ss << "(";
        }
        virtual void visitNewArg ( const ExprNew * call, const Expression * arg, bool last ) override {
            if ( !last ) ss << ",";
            return ConstVisitor::visitNewArg(call, arg, last);
        }
        virtual void visit ( const ExprNew * enew ) override {
            if ( enew->initializer ) ss << ")";
            return ConstVisitor::visit(enew);
        }
    // null coaelescing
        virtual void preVisitNullCoaelescingDefault ( const ExprNullCoalescing * nc, const Expression * expr ) override {
            ConstVisitor::preVisitNullCoaelescingDefault(nc,expr);
            ss << (nc->no_promotion ? " !?? " : " ?? ");
        }
    // at
        virtual void preVisitAtIndex ( const ExprAt * expr, const Expression * index ) override {
            ConstVisitor::preVisitAtIndex(expr, index);
            if ( printRef && expr->r2v ) ss << "@";
            if ( printRef && expr->r2cr ) ss << "$";
            if ( printRef && expr->write ) ss << "#";
            ss << (expr->no_promotion ? "![" : "[");

        }
        virtual void visit ( const ExprAt * that ) override {
            ss << "]";
            return ConstVisitor::visit(that);
        }
    // safe at
        virtual void preVisitSafeAtIndex ( const ExprSafeAt * expr, const Expression * index ) override {
            ConstVisitor::preVisitAtIndex(expr, index);
            if ( printRef && expr->r2v ) ss << "@";
            if ( printRef && expr->r2cr ) ss << "$";
            if ( printRef && expr->write ) ss << "#";
            ss << (expr->no_promotion ? "!?[" : "?[");

        }
        virtual void visit ( const ExprSafeAt * that ) override {
            ss << "]";
            return ConstVisitor::visit(that);
        }
    // op1
        virtual void preVisit ( const ExprOp1 * that ) override {
            ConstVisitor::preVisit(that);
            if ( that->op!="+++" && that->op!="---" ) {
                ss << that->op;
            }
            if ( !noBracket(that) && !level(that->subexpr).bottom ) ss << "(";
        }
        virtual void visit ( const ExprOp1 * that ) override {
            if ( that->op=="+++" || that->op=="---" ) {
                ss << that->op[0] << that->op[1];
            }
            if ( !noBracket(that) && !level(that->subexpr).bottom ) ss << ")";
            return ConstVisitor::visit(that);
        }
    // op2
        virtual void preVisit ( const ExprOp2 * that ) override {
            ConstVisitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitRight ( const ExprOp2 * op2, const Expression * right ) override {
            ConstVisitor::preVisitRight(op2,right);
            ss << " " << op2->op << " ";
        }
        virtual void visit ( const ExprOp2 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return ConstVisitor::visit(that);
        }
    // op3
        virtual void preVisit ( const ExprOp3 * that ) override {
            ConstVisitor::preVisit(that);
            if ( !noBracket(that) ) ss << "(";
        }
        virtual void preVisitLeft  ( const ExprOp3 * that, const Expression * left ) override {
            ConstVisitor::preVisitLeft(that,left);
            ss << " ? ";
        }
        virtual void preVisitRight ( const ExprOp3 * that, const Expression * right ) override {
            ConstVisitor::preVisitRight(that,right);
            ss << " : ";
        }
        virtual void visit ( const ExprOp3 * that ) override {
            if ( !noBracket(that) ) ss << ")";
            return ConstVisitor::visit(that);
        }
    // copy & move, clone
        virtual void preVisitRight ( const ExprCopy * that, const Expression * right ) override {
            ConstVisitor::preVisitRight(that,right);
            ss << (that->no_promotion ? " !== " : " = ");
        }
        virtual void preVisitRight ( const ExprMove * that, const Expression * right ) override {
            ConstVisitor::preVisitRight(that,right);
            ss << (that->no_promotion ? " !<- " : " <- ");
        }
        virtual void preVisitRight ( const ExprClone * that, const Expression * right ) override {
            ConstVisitor::preVisitRight(that,right);
            ss << (that->no_promotion ? " !:= " : " := ");
        }
    // return
        virtual void preVisit ( const ExprReturn * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "return ";
            if ( expr->fromYield ) ss << "/*yield*/ ";
            if ( expr->moveSemantics ) ss << "<- ";
        }
    // yield
        virtual void preVisit ( const ExprYield * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "yield ";
            if ( expr->moveSemantics ) ss << "<- ";
        }
    // break
        virtual void preVisit ( const ExprBreak * that ) override {
            ConstVisitor::preVisit(that);
            ss << "break";
        }
    // continue
        virtual void preVisit ( const ExprContinue * that ) override {
            ConstVisitor::preVisit(that);
            ss << "continue";
        }
    // typedecl
        virtual void preVisit ( const ExprTypeDecl * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "type<" << *expr->typeexpr << ">";
        }
    // typeinfo
        virtual void preVisit ( const ExprTypeInfo * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "typeinfo(" << expr->trait << " ";
            if ( !expr->subexpr ) {
                ss << "type<" << *expr->typeexpr << ">";
            }
        }
        virtual void visit ( const ExprTypeInfo * expr ) override {
            ss << ")";
            return ConstVisitor::visit(expr);
        }
    // is
        virtual void preVisitType ( const ExprIs * expr, const TypeDecl * decl ) override {
            ConstVisitor::preVisit(expr);
            ss << (expr->no_promotion ? " !is type<" : " is type<") << decl->describe(TypeDecl::DescribeExtra::no, TypeDecl::DescribeContracts::yes) << ">";
        }
    // make variant
        virtual void preVisit ( const ExprMakeVariant * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "[[";
            if ( expr->type ) {
                ss << expr->type->describe() << " ";
            }
        }
        virtual void preVisitMakeVariantField ( const ExprMakeVariant * expr, int index, const MakeFieldDecl * decl, bool last ) override {
            ConstVisitor::preVisitMakeVariantField(expr,index,decl,last);
            ss << decl->name << (decl->moveSemantics ? " <- " : " = ");
        }
        virtual void visitMakeVariantField ( const ExprMakeVariant * expr, int index, const MakeFieldDecl * decl, bool last ) override {
            if (!last) ss << "; ";
            return ConstVisitor::visitMakeVariantField(expr,index,decl,last);
        }
        virtual void visit ( const ExprMakeVariant * expr ) override {
            ss << "]]";
            return ConstVisitor::visit(expr);
        }
    // make structure
        virtual void preVisit ( const ExprMakeStruct * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( gen2 ) {
                if ( expr->structs.empty() ) {
                    ss << "default<";
                    if ( expr->type ) {
                        ss << expr->type->describe();
                    } else {
                        ss << "/* undefined */";
                    }
                    ss << ">";
                    if ( !expr->useInitializer && !expr->constructor ) {
                        ss << " uninitialized";
                    }
                } else {
                    ss << "struct<";
                    if ( expr->type ) {
                        ss << expr->type->describe();
                    } else {
                        ss << "/* undefined */";
                    }
                    ss << ">(";
                    if ( !expr->useInitializer && !expr->constructor ) {
                        ss << "uninitialized ";
                    }
                }
            } else {
                ss << "[[";
                if ( expr->type ) {
                    ss << expr->type->describe();
                    if ( expr->useInitializer || expr->constructor ) {
                        ss << "()";
                    }
                    ss << " ";
                }
            }
        }
        virtual void preVisitMakeStructureField ( const ExprMakeStruct * expr, int index, const MakeFieldDecl * decl, bool last ) override {
            ConstVisitor::preVisitMakeStructureField(expr,index,decl,last);
            ss << decl->name << (decl->moveSemantics ? " <- " : " = ");
        }
        virtual void visitMakeStructureField ( const ExprMakeStruct * expr, int index, const MakeFieldDecl * decl, bool last ) override {
            if ( !last ) ss << ", ";
            return ConstVisitor::visitMakeStructureField(expr,index,decl,last);
        }
        virtual void visitMakeStructureIndex ( const ExprMakeStruct * expr, int index, bool lastField ) override {
            if ( !lastField ) ss << "; ";
            ConstVisitor::visitMakeStructureIndex(expr, index, lastField);
        }
        virtual void preVisitMakeStructureBlock ( const ExprMakeStruct *, const Expression * ) override {
            ss << " where ";
        }
        virtual void visit ( const ExprMakeStruct * expr ) override {
            if ( gen2 ) {
                if ( !expr->structs.empty() ) {
                    ss << ")";
                }
            } else {
                ss << "]]";
            }
            return ConstVisitor::visit(expr);
        }
    // make tuple
        virtual void preVisit ( const ExprMakeTuple * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( gen2 ) {
                ss << "tuple";
                if ( expr->type ) {
                    ss << "<" << expr->type->describe() << ">";
                } else {
                    ss << "</* undefined */>";
                }
                ss << "(";
            } else {
                ss << "[[";
                if ( expr->type ) {
                    ss << expr->type->describe();
                } else {
                    ss << "/* undefined */";
                }
                ss << " ";
            }
        }
        virtual void visitMakeTupleIndex ( const ExprMakeTuple * expr, int index, const Expression * init, bool lastField ) override {
            if ( !lastField ) ss << ",";
            return ConstVisitor::visitMakeTupleIndex(expr, index, init, lastField);
        }
        virtual void visit ( const ExprMakeTuple * expr ) override {
            if ( gen2 ) {
                ss << ")";
            } else {
                ss << "]]";
            }
            return ConstVisitor::visit(expr);
        }
    // make array
        virtual void preVisit ( const ExprMakeArray * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( gen2 ) {
                // if ( expr->type ) { ss << "/*" << expr->type->describe() << "*/ "; }
                ss << "fixed_array";
                if ( expr->makeType ) {
                    ss << "<" << expr->makeType->describe() << ">";
                } else {
                    ss << "</* undefined */>";
                }
                ss << "(";
            } else {
                ss << "[[";
                if ( expr->type ) {
                    ss << expr->type->describe();
                } else {
                    ss << "/* undefined */";
                }
                ss << " ";
            }
        }
        virtual void visitMakeArrayIndex ( const ExprMakeArray * expr, int index, const Expression * init, bool lastField ) override {
            if ( !lastField ) {
                ss << (gen2 ? ", " : "; ");
            }
            return ConstVisitor::visitMakeArrayIndex(expr, index, init, lastField);
        }
        virtual void visit ( const ExprMakeArray * expr ) override {
            ss << (gen2 ? ")" : "]]");
            return ConstVisitor::visit(expr);
        }
    // array comprehension
        virtual void preVisit ( const ExprArrayComprehension * expr ) override {
            ConstVisitor::preVisit(expr);
            if ( expr->generatorSyntax ) {
                ss << "[[";
            } else if ( expr->tableSyntax ) {
                ss << "{";
            } else {
                ss << "[{";
            }
        }
        virtual void preVisitArrayComprehensionSubexpr ( const ExprArrayComprehension * expr, const Expression * subexpr ) override {
            ConstVisitor::preVisitArrayComprehensionSubexpr(expr, subexpr);
            ss << "; ";
        }
        virtual void preVisitArrayComprehensionWhere ( const ExprArrayComprehension * expr, const Expression * where ) override {
            ConstVisitor::preVisitArrayComprehensionWhere(expr, where);
            ss << "; where ";
        }
        virtual void visit ( const ExprArrayComprehension * expr ) override {
            if ( expr->generatorSyntax ) {
                ss << "]]";
            } else if ( expr->tableSyntax ) {
                ss << "}";
            } else {
                ss << "}]";
            }
            return ConstVisitor::visit(expr);
        }
    // quote
        virtual void preVisit ( const ExprQuote * expr ) override {
            ConstVisitor::preVisit(expr);
            ss << "quote(";
        }
        virtual void visit ( const ExprQuote * expr ) override {
            ss << ")";
            return ConstVisitor::visit(expr);
        }
    protected:
        TextWriter          ss;
        uint64_t            lastNewLine = -1ul;
        int                 tab = 0;
    };

    void Program::setPrintFlags() {
#if defined(STANDALONE_MODE)
        ctx_ast_print::Standalone ctx;
        ctx.setFlags(this);
#else
        ClearPrinterFlags cflags;
        visit(cflags);
        SetPrinterFlags pflags;
        constVisitProgram(pflags, this, false);
        for ( auto & [expr, lv] : pflags.levels ) {
            auto node = const_cast<Expression *>(expr);
            node->topLevel = lv.top;
            node->argLevel = lv.arg;
            node->bottomLevel = lv.bottom;
        }
#endif
    }

    static void printVisit ( const Expression & expr, ConstVisitor & vis ) { expr.visit(vis); }
    static void printVisit ( const Function & func, ConstVisitor & vis ) { constVisitFunction(vis, &func); }

    template <typename TT>
    __forceinline StringWriter&  print ( StringWriter& stream, const TT & value ) {
        SetPrinterFlags flags;
        printVisit(value, flags);
        Printer log(flags.levels);
        printVisit(value, log);
        stream << log.str();
        return stream;
    }

    StringWriter& operator<< (StringWriter& stream, const Program & program) {
        bool any = false;
        program.library.foreach([&](Module * pm) {
            if ( !pm->name.empty() && pm->name!="$" ) {
                stream << "require " << pm->name << "\n";
                any = true;
            }
            return true;
        }, "*");
        if (any) stream << "\n";
        bool logGenerics = program.options.getBoolOption("log_generics");
        SetPrinterFlags flags;
        constVisitProgram(flags, &program, logGenerics);
        Printer log(flags.levels, &program);
        constVisitProgram(log, &program, logGenerics);
        stream << log.str();
        return stream;
    }

    StringWriter& operator<< (StringWriter& stream, const Expression & expr) {
        return print(stream,expr);
    }

    StringWriter& operator<< (StringWriter& stream, const Function & func) {
        return print(stream,func);
    }
}
