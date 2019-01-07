#include "precomp.h"

#include "ast.h"

namespace das {
    
    // auto or generic type conversion
    
    void applyAutoContracts ( TypeDeclPtr TT, TypeDeclPtr autoT ) {
        if ( !autoT->isAuto() ) return;
        TT->ref |= autoT->ref;
        TT->constant |= autoT->constant;
        if ( autoT->isPointer() ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tArray ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
        } else if ( autoT->baseType==Type::tTable ) {
            applyAutoContracts(TT->firstType, autoT->firstType);
            applyAutoContracts(TT->secondType, autoT->secondType);
        } else if ( autoT->baseType==Type::tBlock ) {
            if ( TT->firstType ) {
                applyAutoContracts(TT->firstType, autoT->firstType);
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                applyAutoContracts(TT->argTypes[i], autoT->argTypes[i]);
            }
        }
    }
    
    TypeDeclPtr inferAutoType ( TypeDeclPtr autoT, TypeDeclPtr initT ) {
        // can't infer from the type, which is already 'auto'
        if ( initT->isAuto() )
            return nullptr;
        // if its not an auto type, return as is
        if ( !autoT->isAuto() ) {
            if ( autoT->isSameType(*initT) ) {
                return make_shared<TypeDecl>(*autoT);
            } else {
                return nullptr;
            }
        }
        // auto & can't be infered from non-ref
        if ( autoT->ref && !initT->ref )
            return nullptr;
        // auto[][][] can't be infered from non-array
        if ( autoT->dim.size() && autoT->dim!=initT->dim )
            return nullptr;
        // auto? can't be infered from non-pointer
        if ( autoT->isPointer() && (!initT->isPointer() || !initT->firstType) )
            return nullptr;
        // array has to match array
        if ( autoT->baseType==Type::tArray && (initT->baseType!=Type::tArray || !initT->firstType) )
            return nullptr;
        // table has to match table
        if ( autoT->baseType==Type::tTable && (initT->baseType!=Type::tTable || !initT->firstType || !initT->secondType) )
            return nullptr;
        // block has to match block
        if ( autoT->baseType==Type::tBlock ) {
            if ( initT->baseType!=Type::tBlock )
                return nullptr;
            if ( (autoT->firstType!=nullptr) != (initT->firstType!=nullptr) )   // both do or don't have return type
                return nullptr;
            if ( autoT->argTypes.size() != initT->argTypes.size() )             // both have same number of arguments
                return nullptr;
        }
        // now, lets make the type
        auto TT = make_shared<TypeDecl>(*initT);
        TT->at = autoT->at;
        TT->alias = autoT->alias;
        if ( autoT->isPointer() ) {
            // if it's a pointer, infer pointer-to separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tArray ) {
            // if it's an array, infer array type separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
        } else if ( autoT->baseType==Type::tTable ) {
            // if it's a table, infer table keys and values types separately
            TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
            if ( !TT->firstType ) return nullptr;
            if ( !TT->firstType->isWorkhorseType() ) return nullptr;            // table key has to be hashable too
            TT->secondType = inferAutoType(autoT->secondType, initT->secondType);
            if ( !TT->secondType ) return nullptr;
        } else if ( autoT->baseType==Type::tBlock ) {
            // if it's a block, infer argument and return types
            if ( autoT->firstType ) {
                TT->firstType = inferAutoType(autoT->firstType, initT->firstType);
                if ( !TT->firstType ) return nullptr;
            }
            for ( size_t i=0; i!=autoT->argTypes.size(); ++i ) {
                TT->argTypes[i] = inferAutoType(autoT->argTypes[i], initT->argTypes[i]);
                if ( !TT->argTypes[i] ) return nullptr;
            }
        }
        return TT;
    }
    
    // type inference

    class InferTypes : public Visitor {
    public:
        InferTypes( const ProgramPtr & prog ) {
            program = prog;
        }
        bool finished() const { return !needRestart; }
    protected:
        ProgramPtr              program;
        FunctionPtr             func;
        vector<VariablePtr>     local;
        vector<ExpressionPtr>   loop;
        vector<ExprBlock *>     blocks;
        vector<size_t>          varStack;
        int                     fieldOffset = 0;
        int                     globalVarIndex = 0;
        bool                    needRestart = false;
    protected:
        void pushVarStack() { varStack.push_back(local.size()); }
        void popVarStack()  { local.resize(varStack.back()); varStack.pop_back(); }
        void error ( const string & err, const LineInfo & at, CompilationError cerr = CompilationError::unspecified ) {
            if ( func ) {
                string extra = func->getLocationExtra();
                program->error(err + extra,at,cerr);
            } else {
                program->error(err,at,cerr);
            }
        }
        void reportGenericInfer() {
            needRestart = true;
        }
    protected:
        
        // find type alias name, and resolve it to type
        // without one generic function
        const TypeDecl * findFuncAlias ( const FunctionPtr & fptr, const string & name ) const {
            for ( auto & arg : fptr->arguments ) {
                if ( auto aT = arg->type->findAlias(name,true) ) {
                    return aT;
                }
            }
            for ( auto & gvKV : program->thisModule->globals ) {
                auto & gvar = gvKV.second;
                if ( auto vT = gvar->type->findAlias(name,false) ) {
                    return vT;
                }
            }
            return nullptr;
        }
        
        // find type alias name, and resolve it to type
        // within current context
        const TypeDecl * findAlias ( const string & name ) const {
            if ( func ) {
                for ( auto it = local.rbegin(); it!=local.rend(); ++it ) {
                    auto & var = *it;
                    if ( auto vT = var->type->findAlias(name) ) {
                        return vT;
                    }
                }
                for ( auto & arg : func->arguments ) {
                    if ( auto aT = arg->type->findAlias(name) ) {
                        return aT;
                    }
                }
            }
            for ( auto & gvKV : program->thisModule->globals ) {
                auto & gvar = gvKV.second;
                if ( auto vT = gvar->type->findAlias(name) ) {
                    return vT;
                }
            }
            return nullptr;
        }
        
        // infer alias type
        TypeDeclPtr inferAlias ( const TypeDeclPtr & decl, const FunctionPtr & fptr = nullptr ) const {
            if ( decl->baseType==Type::autoinfer ) {    // until alias is fully resolved, can't infer
                return nullptr;
            }
            if ( decl->baseType==Type::alias ) {
                if ( auto aT = fptr ? findFuncAlias(fptr, decl->alias) : findAlias(decl->alias) ) {
                    auto resT = make_shared<TypeDecl>(*aT);
                    resT->ref |= decl->ref;
                    resT->constant |= decl->constant;
                    resT->dim.insert(resT->dim.end(), decl->dim.begin(), decl->dim.end());
                    resT->alias = decl->alias;
                    return resT;
                } else {
                    return nullptr;
                }
            }
            auto resT = make_shared<TypeDecl>(*decl);
            if ( decl->baseType==Type::tPointer ) {
                if ( decl->firstType ) {
                    resT->firstType = inferAlias(decl->firstType,fptr);
                    if ( !resT->firstType ) return nullptr;
                }
            } else if ( decl->baseType==Type::tArray ) {
                if ( decl->firstType ) {
                    resT->firstType = inferAlias(decl->firstType,fptr);
                    if ( !resT->firstType ) return nullptr;
                }
            } else if ( decl->baseType==Type::tTable ) {
                if ( decl->firstType ) {
                    resT->firstType = inferAlias(decl->firstType,fptr);
                    if ( !resT->firstType ) return nullptr;
                }
                if ( decl->secondType ) {
                    resT->secondType = inferAlias(decl->secondType,fptr);
                    if ( !resT->secondType ) return nullptr;
                }
            } else if ( decl->baseType==Type::tBlock ) {
                for ( size_t iA=0; iA!=decl->argTypes.size(); ++iA ) {
                    auto & declAT = decl->argTypes[iA];
                    if ( auto infAT = inferAlias(declAT,fptr) ) {
                        resT->argTypes[iA] = infAT;
                    } else {
                        return nullptr;
                    }
                }
                resT->firstType = inferAlias(decl->firstType,fptr);
                if ( !resT->firstType ) return nullptr;
            }
            return resT;
        }
        
        vector<FunctionPtr> findCandidates ( const string & name, const vector<TypeDeclPtr> & types ) const {
            string moduleName, funcName;
            splitTypeName(name, moduleName, funcName);
            vector<FunctionPtr> result;
            program->library.foreach([&](Module * mod) -> bool {
                auto itFnList = mod->functionsByName.find(funcName);
                if ( itFnList != mod->functionsByName.end() ) {
                    auto & goodFunctions = itFnList->second;
                    result.insert(result.end(), goodFunctions.begin(), goodFunctions.end());
                }
                return true;
            },moduleName);
            return result;
        }
        
        vector<FunctionPtr> findGenericCandidates ( const string & name, const vector<TypeDeclPtr> & types ) const {
            string moduleName, funcName;
            splitTypeName(name, moduleName, funcName);
            vector<FunctionPtr> result;
            program->library.foreach([&](Module * mod) -> bool {
                auto itFnList = mod->genericsByName.find(funcName);
                if ( itFnList != mod->genericsByName.end() ) {
                    auto & goodFunctions = itFnList->second;
                    result.insert(result.end(), goodFunctions.begin(), goodFunctions.end());
                }
                return true;
            },moduleName);
            return result;
        }

		bool isMatchingArgument(const FunctionPtr & pFn, TypeDeclPtr argType, TypeDeclPtr passType, bool inferAuto, bool inferBlock) const {
			if (!passType) {
				return false;
			}
			if (inferAuto) {
				// if it's an alias, we de'alias it, and see if it matches at all
				if (argType->isAlias()) {
					argType = inferAlias(argType, pFn);
					if ( !argType ) {
						return false;
					}
				}
				// match auto argument
				if (argType->isAuto()) {
					return inferAutoType(argType, passType) != nullptr;
				}
			}
			// match inferable block
			if (inferBlock && passType->isAuto() && passType->isGoodBlockType()) {
				return inferAutoType(passType, argType) != nullptr;
			}
			// compare types which don't need inference
			if (passType && ((argType->isRef() && !passType->isRef()) || !argType->isSameType(*passType, false, false))) {
				return false;
			}
			// ref types can only add constness
			if (argType->isRef() && !argType->constant && passType->constant) {
				return false;
			}
			// pointer types can only add constant
			if (argType->isPointer() && !argType->constant && passType->constant) {
				return false;
			}
			// all good
			return true;
		}
        
        bool isFunctionCompatible ( const FunctionPtr & pFn, const vector<TypeDeclPtr> & types, bool inferAuto, bool inferBlock ) const {
            if ( pFn->arguments.size() < types.size() ) {
                return false;
            }
            for ( auto ai = 0; ai != types.size(); ++ai ) {
				if (!isMatchingArgument(pFn, pFn->arguments[ai]->type, types[ai],inferAuto,inferBlock)) {
					return false;
				}
            }
            for ( auto ti = types.size(); ti != pFn->arguments.size(); ++ti ) {
                if ( !pFn->arguments[ti]->init ) {
					return false;
                }
            }
			return true;
        }

		string describeMismatchingFunction(const FunctionPtr & pFn, const vector<TypeDeclPtr> & types, bool inferAuto, bool inferBlock) const {
			stringstream ss;
			for (auto ai = 0; ai != types.size(); ++ai) {
				auto & arg = pFn->arguments[ai];
				auto & passType = types[ai];
				if (!isMatchingArgument(pFn, arg->type, passType, inferAuto, inferBlock)) {
					ss << "\ninvalid argument " << arg->name << ", expecting (" << arg->type->describe() << ") passing (" << passType->describe() << ")";
				}
			}
			return ss.str();
		}
        
        vector<FunctionPtr> findMatchingFunctions ( const string & name, const vector<TypeDeclPtr> & types, bool inferBlock = false ) const {
            string moduleName, funcName;
            splitTypeName(name, moduleName, funcName);
            vector<FunctionPtr> result;
            program->library.foreach([&](Module * mod) -> bool {
                auto itFnList = mod->functionsByName.find(funcName);
                if ( itFnList != mod->functionsByName.end() ) {
                    auto & goodFunctions = itFnList->second;
                    for ( auto & pFn : goodFunctions ) {
                        if ( isFunctionCompatible(pFn, types, false, inferBlock) ) {
                            result.push_back(pFn);
                        }
                    }
                }
                return true;
            },moduleName);
            return result;
        }
        
        vector<FunctionPtr> findMatchingGenerics ( const string & name, const vector<TypeDeclPtr> & types ) const {
            string moduleName, funcName;
            splitTypeName(name, moduleName, funcName);
            vector<FunctionPtr> result;
            program->library.foreach([&](Module * mod) -> bool {
                auto itFnList = mod->genericsByName.find(funcName);
                if ( itFnList != mod->genericsByName.end() ) {
                    auto & goodFunctions = itFnList->second;
                    for ( auto & pFn : goodFunctions ) {
                        if ( isFunctionCompatible(pFn, types, true, true) ) {   // infer block here?
                            result.push_back(pFn);
                        }
                    }
                }
                return true;
            },moduleName);
            return result;
        }

		void reportFunctionNotFound( const string & extra, const LineInfo & at, const vector<FunctionPtr> & candidateFunctions, 
			const vector<TypeDeclPtr> & types, bool inferAuto, bool inferBlocks ) {
			if (candidateFunctions.size() == 1) {
				auto missFn = candidateFunctions.back();
				auto problems = describeMismatchingFunction(missFn, types, inferAuto, inferBlocks);
				error(extra + "\ncandidate function:\n\t"
					+ missFn->describe() + problems, at, CompilationError::function_not_found);
			}
			else {
				string candidates = program->describeCandidates(candidateFunctions);
				error(extra + "\n" + candidates, at, CompilationError::function_not_found);
			}
		}

    protected:
    // strcuture
        virtual void preVisit ( Structure * that ) override {
            Visitor::preVisit(that);
            fieldOffset = 0;
        }
        virtual void preVisitStructureField ( Structure * that, Structure::FieldDeclaration & decl, bool last ) override {
            Visitor::preVisitStructureField(that, decl, last);
            decl.offset = fieldOffset;
            fieldOffset += decl.type->getSizeOf();
        }
    // globals
        virtual void preVisitGlobalLet ( const VariablePtr & var ) override {
            Visitor::preVisitGlobalLet(var);
            var->index = globalVarIndex ++;
            if ( var->type->isAuto() && !var->init) {
                error("global variable type can't be infered, need initializer",
                      var->at, CompilationError::cant_infer_missing_initializer );
            }
        }
        virtual ExpressionPtr visitGlobalLetInit ( const VariablePtr & var, Expression * init ) override {
            if ( !var->init->type ) return Visitor::visitGlobalLetInit(var, init);
            if ( var->type->isAuto() ) {
                auto varT = inferAutoType(var->type, var->init->type);
                if ( !varT ) {
                    error("global variable initialization type can't be infered, "
                          + var->type->describe() + " = " + var->init->type->describe(),
                          var->at, CompilationError::cant_infer_mismatching_restrictions );
                } else {
                    varT->ref = false;
                    applyAutoContracts(varT, var->type);
                    var->type = varT;
                }
            } else if ( !var->type->isSameType(*var->init->type,false) ) {
                error("global variable initialization type mismatch, "
                      + var->type->describe() + " = " + var->init->type->describe(), var->at );
            } else if ( var->type->baseType==Type::tStructure ) {
                error("can't initialize structures", var->at );
            } else if ( !var->init->type->canCopy() && !var->init->type->canMove() ) {
                error("this global variable can't be initialized at all", var->at);
            }
            return Visitor::visitGlobalLetInit(var, init);
        }
        virtual VariablePtr visitGlobalLet ( const VariablePtr & var ) override {
            if ( var->type->ref )
                error("global variable can't be reference", var->at, CompilationError::invalid_variable_type);
            if ( var->type->isVoid() )
                error("global variable can't be void", var->at, CompilationError::invalid_variable_type);
            if ( var->type->isHandle() && !var->type->annotation->isLocal() )
                error("handled type " + var->type->annotation->name + "can't be global", var->at, CompilationError::invalid_variable_type);
            return Visitor::visitGlobalLet(var);
        }
    // function
        virtual void preVisit ( Function * f ) override {
            Visitor::preVisit(f);
            func = f->shared_from_this();
            func->hasReturn = false;
        }
        virtual void preVisitArgument ( Function * fn, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitArgument(fn, var, lastArg);
            if ( var->type->isAlias() ) {
                if ( auto aT = inferAlias(var->type) ) {
                    var->type = aT;
                    reportGenericInfer();
                } else {
                    error("undefined type " + var->type->describe(), var->at, CompilationError::type_not_found );
                }
            }
            if ( var->type->isAuto() ) {
                error("generics are not supported yet", var->at, CompilationError::cant_infer_generic );
            }
        }
        virtual ExpressionPtr visitArgumentInit ( Function * f, const VariablePtr & arg, Expression * that ) override {
            if ( !arg->init->type || !arg->type->isSameType(*arg->init->type, true, false) ) {
                error("function argument default value type mismatch", arg->init->at);
            }
            return Visitor::visitArgumentInit(f, arg, that);
        }
        virtual FunctionPtr visit ( Function * that ) override {
            // if function got no 'result', function is a void function
            if ( !func->hasReturn ) {
                if ( func->result->isAuto() ) {
                    func->result = make_shared<TypeDecl>(Type::tVoid);
                    reportGenericInfer();
                } else if ( !func->result->isVoid() ){
                    error("function does not return value", func->at, CompilationError::expecting_return_value);
                }
            }
            assert(blocks.size()==0);
            func.reset();
            return Visitor::visit(that);
        }
    // any expression
        virtual void preVisitExpression ( Expression * expr ) override {
            Visitor::preVisitExpression(expr);
            expr->type.reset();
        }
    // const
        virtual ExpressionPtr visit ( ExprConst * c ) override {
            c->type = make_shared<TypeDecl>(c->baseType);
            return Visitor::visit(c);
        }
    // ExprRef2Value
        virtual ExpressionPtr visit ( ExprRef2Value * expr ) override {
            if ( !expr->subexpr->type ) return Visitor::visit(expr);
            // infer
            if ( !expr->subexpr->type->isRef() ) {
                error("can only dereference ref", expr->at);
            } else if ( !expr->subexpr->type->isSimpleType() && !expr->subexpr->type->isPointer() ) {
                error("can only dereference a simple type, " + expr->subexpr->type->describe(), expr->at);
            } else {
                expr->type = make_shared<TypeDecl>(*expr->subexpr->type);
                expr->type->ref = false;
            }
            return Visitor::visit(expr);
        }
    // ExprPtr2Ref
        virtual ExpressionPtr visit ( ExprPtr2Ref * expr ) override {
            if ( !expr->subexpr->type ) return Visitor::visit(expr);
            // infer
            expr->subexpr = Expression::autoDereference(expr->subexpr);
            if ( !expr->subexpr->type->isPointer() ) {
                error("can only dereference pointer", expr->at, CompilationError::cant_dereference);
            } else if ( !expr->subexpr->type->firstType || expr->subexpr->type->firstType->isVoid() ) {
                error("can only dereference pointer to something", expr->at, CompilationError::cant_dereference);
            } else {
                expr->type = make_shared<TypeDecl>(*expr->subexpr->type->firstType);
                expr->type->ref = true;
                expr->type->constant |= expr->subexpr->type->constant;
            }
            return Visitor::visit(expr);
        }
    // ExprNullCoalescing
        virtual ExpressionPtr visit ( ExprNullCoalescing * expr ) override {
            if ( !expr->subexpr->type | !expr->defaultValue->type ) return Visitor::visit(expr);
            // infer
            expr->subexpr = Expression::autoDereference(expr->subexpr);
            auto seT = expr->subexpr->type;
            auto dvT = expr->defaultValue->type;
            if ( !seT->isPointer() ) {
                error("can only dereference pointer", expr->at, CompilationError::cant_dereference);
            } else if ( !seT->firstType || seT->firstType->isVoid() ) {
                error("can only dereference pointer to something", expr->at, CompilationError::cant_dereference);
            } else if ( !seT->firstType->isSameType(*dvT,false,false) ) {
                error("default value type mismatch in " + seT->firstType->describe() + " vs " + dvT->describe(),
                      expr->at, CompilationError::cant_dereference);
            } else if ( !seT->isConst() && dvT->isConst() ) {
                error("default value type mismatch, constant matters in " + seT->describe() + " vs " + dvT->describe(),
                      expr->at, CompilationError::cant_dereference);
            } else {
                expr->type = make_shared<TypeDecl>(*dvT);
                expr->type->constant |= expr->subexpr->type->constant;
            }
            return Visitor::visit(expr);
        }
    // ExprStaticAssert
        virtual ExpressionPtr visit ( ExprStaticAssert * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()<1 || expr->arguments.size()>2  ) {
                error("static_assert(expr) or static_assert(expr,string)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->autoDereference();
            if ( !expr->arguments[0]->type->isSimpleType(Type::tBool) )
                error("static assert condition must be boolean", expr->at, CompilationError::invalid_argument_type);
            if ( expr->arguments.size()==2 && !expr->arguments[1]->rtti_isStringConstant() ) {
                error("static assert comment must be string constant", expr->at, CompilationError::invalid_argument_type);
                return nullptr;
            }
            expr->type = make_shared<TypeDecl>(Type::tVoid);
            return Visitor::visit(expr);
        }
    // ExprAssert
        virtual ExpressionPtr visit ( ExprAssert * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()<1 || expr->arguments.size()>2  ) {
                error("assert(expr) or assert(expr,string)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->autoDereference();
            if ( !expr->arguments[0]->type->isSimpleType(Type::tBool) )
                error("assert condition must be boolean", expr->at, CompilationError::invalid_argument_type);
            if ( expr->arguments.size()==2 && !expr->arguments[1]->rtti_isStringConstant() ) {
                error("assert comment must be string constant", expr->at, CompilationError::invalid_argument_type);
            }
            expr->type = make_shared<TypeDecl>(Type::tVoid);
            return Visitor::visit(expr);
        }
    // ExprDebug
        virtual ExpressionPtr visit ( ExprDebug * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()<1 || expr->arguments.size()>2 ) {
                error("debug(expr) or debug(expr,string)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            if ( expr->arguments.size()==2 && !expr->arguments[1]->rtti_isStringConstant() )
                error("debug comment must be string constant", expr->at, CompilationError::invalid_argument_type);
            expr->type = make_shared<TypeDecl>(*expr->arguments[0]->type);
            return Visitor::visit(expr);
        }
    // ExprMakeBlock
        virtual ExpressionPtr visit ( ExprMakeBlock * expr ) override {
            // infer
            auto block = static_pointer_cast<ExprBlock>(expr->block);
            expr->type = make_shared<TypeDecl>(Type::tBlock);
            if ( block->type ) {
                expr->type->firstType = make_shared<TypeDecl>(*block->type);
            }
            for ( auto & arg : block->arguments ) {
                if ( arg->type ) {
                    expr->type->argTypes.push_back(make_shared<TypeDecl>(*arg->type));
                }
            }
            return Visitor::visit(expr);
        }
    // ExprInvoke
        virtual ExpressionPtr visit ( ExprInvoke * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()<1 ) {
                error("invoke(block) or invoke(block,...)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->arguments[0] = Expression::autoDereference(expr->arguments[0]);
            auto blockT = expr->arguments[0]->type;
            if ( !blockT->isGoodBlockType() ) {
                error("expecting block", expr->at, CompilationError::invalid_argument_type);
            }
            if ( expr->arguments.size()-1 != blockT->argTypes.size() ) {
                error("invalid number of arguments", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            for ( size_t i=0; i != blockT->argTypes.size(); ++i ) {
                auto & passType = expr->arguments[i+1]->type;
                auto & argType = blockT->argTypes[i];
                // same type only
                if ( passType && ((argType->isRef() && !passType->isRef()) || !argType->isSameType(*passType, false, false)) ) {
                    error("incomaptible argument", expr->at, CompilationError::invalid_argument_type);
                }
                // ref types can only add constness
                if ( argType->isRef() && !argType->constant && passType->constant ) {
                    error("incomaptible argument, const matters", expr->at, CompilationError::invalid_argument_type);
                }
                // pointer types can only add constant
                if ( argType->isPointer() && !argType->constant && passType->constant ) {
                    error("incomaptible argument, pointer const matters", expr->at, CompilationError::invalid_argument_type);
                }
                if ( !argType->isRef() )
                    expr->arguments[i+1] = Expression::autoDereference(expr->arguments[i+1]);
            }
            if ( blockT->firstType ) {
                expr->type = make_shared<TypeDecl>(*blockT->firstType);
            } else {
                expr->type = make_shared<TypeDecl>();
            }
            return Visitor::visit(expr);
        }
    // ExprHash
        virtual ExpressionPtr visit ( ExprHash * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=1 ) {
                error("hash(expr)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->arguments[0]->type->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tUInt64);
            return Visitor::visit(expr);
        }
    // ExprArrayPush
        virtual ExpressionPtr visit ( ExprArrayPush * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=2 && expr->arguments.size()!=3 ) {
                error("push(array,value) or push(array,value,at)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            auto arrayType = expr->arguments[0]->type;
            auto valueType = expr->arguments[1]->type;
            if ( !arrayType->isGoodArrayType() ) {
                error("push first argument must be fully qualified array", expr->at, CompilationError::invalid_argument_type);
                return Visitor::visit(expr);
            }
            if ( !arrayType->firstType->isSameType(*valueType,false) )
                error("can't push value of different type", expr->at, CompilationError::invalid_argument_type);
            if ( expr->arguments.size()==3 && !expr->arguments[2]->type->isIndex() )
                error("push at must be an index", expr->at, CompilationError::invalid_argument_type);
            valueType->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tVoid);
            return Visitor::visit(expr);
        }
    // ExprErase
        virtual ExpressionPtr visit ( ExprErase * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=2 ) {
                error("erase(table,key) or erase(array,index)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->arguments[1] = Expression::autoDereference(expr->arguments[1]);
            auto containerType = expr->arguments[0]->type;
            auto valueType = expr->arguments[1]->type;
            if ( containerType->isGoodArrayType() ) {
                if ( !valueType->isIndex() )
                    error("size must be int or uint", expr->at, CompilationError::invalid_argument_type);
                expr->type = make_shared<TypeDecl>(Type::tVoid);
            } else if ( containerType->isGoodTableType() ) {
                if ( !containerType->firstType->isSameType(*valueType,false) )
                    error("key must be of the same type as table<key,...>", expr->at, CompilationError::invalid_argument_type);
                expr->type = make_shared<TypeDecl>(Type::tBool);
            } else {
                error("first argument must be fully qualified array or table", expr->at, CompilationError::invalid_argument_type);
            }
            valueType->constant = true;
            return Visitor::visit(expr);
        }
    // ExprFind
        virtual ExpressionPtr visit ( ExprFind * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=2 ) {
                error("find(table,key) or find(array,value)", expr->at, CompilationError::invalid_argument_count);
                return Visitor::visit(expr);
            }
            // infer
            expr->arguments[1] = Expression::autoDereference(expr->arguments[1]);
            auto containerType = expr->arguments[0]->type;
            auto valueType = expr->arguments[1]->type;
            if ( containerType->isGoodArrayType() ) {
                if ( !valueType->isSameType(*containerType->firstType) )
                    error("value must be of the same type as array<value>", expr->at, CompilationError::invalid_argument_type);
            } else if ( containerType->isGoodTableType() ) {
                if ( !containerType->firstType->isSameType(*valueType,false) )
                    error("key must be of the same type as table<key,...>", expr->at, CompilationError::invalid_argument_type);
                expr->type = make_shared<TypeDecl>(Type::tPointer);
                expr->type->firstType = make_shared<TypeDecl>(*containerType->secondType);
            } else {
                error("first argument must be fully qualified array or table", expr->at, CompilationError::invalid_argument_type);
            }
            containerType->constant = true;
            valueType->constant = true;
            return Visitor::visit(expr);
        }
    // ExprSizeOf
        virtual ExpressionPtr visit ( ExprSizeOf * expr ) override {
            // check subexpression
            if ( expr->subexpr && expr->subexpr->type ) {
                expr->typeexpr = make_shared<TypeDecl>(*expr->subexpr->type);
            }
            // verify
            if ( !expr->typeexpr ) {
                error("sizeof(...) can't be infered", expr->at, CompilationError::type_not_found);
                return Visitor::visit(expr);
            }
            if ( expr->typeexpr->isAlias() ) {
                if ( auto eT = inferAlias(expr->typeexpr) ) {
                    expr->typeexpr = eT;
                    reportGenericInfer();
                } else {
                    error("udefined type " + expr->typeexpr->describe(), expr->at, CompilationError::type_not_found);
                }
            }
            if ( expr->typeexpr->ref ) {
                error("szieof(ref) is prohibited", expr->at,CompilationError::sizeof_reference);
            } else if ( expr->typeexpr->isAuto() ) {
                error("sizeof(auto) is undefined", expr->at, CompilationError::sizeof_auto);
            }
            // infer
            expr->type = make_shared<TypeDecl>(Type::tInt);
            return Visitor::visit(expr);
        }
    // ExprTypeName
        virtual ExpressionPtr visit ( ExprTypeName * expr ) override {
            // check subexpression
            if ( expr->subexpr && expr->subexpr->type ) {
                expr->typeexpr = make_shared<TypeDecl>(*expr->subexpr->type);
            }
            // verify
            if ( !expr->typeexpr ) {
                error("typename(...) can't be infered", expr->at, CompilationError::type_not_found);
                return Visitor::visit(expr);
            }
            if ( expr->typeexpr->isAlias() ) {
                if ( auto eT = inferAlias(expr->typeexpr) ) {
                    expr->typeexpr = eT;
                    reportGenericInfer();
                } else {
                    error("udefined type " + expr->typeexpr->describe(), expr->at, CompilationError::type_not_found);
                }
            }
            if ( expr->typeexpr->isAuto() ) {
                error("typename(auto) is undefined", expr->at, CompilationError::typename_auto);
            }
            // infer
            expr->type = make_shared<TypeDecl>(Type::tString);
            return Visitor::visit(expr);
        }
    // ExprNew
        virtual ExpressionPtr visit ( ExprNew * expr ) override {
            // infer
            if ( expr->typeexpr->isAlias() ) {
                if ( auto aT = findAlias(expr->typeexpr->alias) ) {
                    expr->typeexpr = make_shared<TypeDecl>(*aT);
                    expr->typeexpr->ref = false;      // drop a ref
                    expr->typeexpr->constant = false; // drop a const
                    reportGenericInfer();
                } else {
                    error("undefined type " + expr->typeexpr->describe(), expr->at, CompilationError::type_not_found);
                    return Visitor::visit(expr);
                }
            }
            if ( expr->typeexpr->ref ) {
                error("can't new a ref", expr->at, CompilationError::invalid_new_type);
            } else if ( expr->typeexpr->dim.size() ) {
                error("can only new single object", expr->at, CompilationError::invalid_new_type);
            } else if ( expr->typeexpr->baseType==Type::tStructure || expr->typeexpr->isHandle() ) {
                expr->type = make_shared<TypeDecl>(Type::tPointer);
                expr->type->firstType = make_shared<TypeDecl>(*expr->typeexpr);
            } else {
                error("can only new structures or handles", expr->at, CompilationError::invalid_new_type);
            }
            return Visitor::visit(expr);
        }
    // ExprAt
        virtual ExpressionPtr visit ( ExprAt * expr ) override {
            if ( !expr->subexpr->type || !expr->index->type) return Visitor::visit(expr);
            // infer
            expr->index = Expression::autoDereference(expr->index);
            auto seT = expr->subexpr->type;
            auto ixT = expr->index->type;
            if ( seT->isGoodTableType() ) {
                if ( !seT->firstType->isSameType(*ixT) ) {
                    error("table index type mismatch", expr->index->at, CompilationError::invalid_index_type);
                    return Visitor::visit(expr);
                }
                expr->type = make_shared<TypeDecl>(*seT->secondType);
                expr->type->ref = true;
                expr->type->constant |= seT->constant;
            } else if ( seT->isHandle() ) {
                if ( !seT->annotation->isIndexable(ixT) ) {
                    error("handle does not support this index type", expr->index->at, CompilationError::invalid_index_type);
                    return Visitor::visit(expr);
                }
                expr->type = seT->annotation->makeIndexType(ixT);
                expr->type->constant |= seT->constant;
            } else {
                if ( !ixT->isIndex() ) {
                    error("index is int or uint", expr->index->at, CompilationError::invalid_index_type);
                    return Visitor::visit(expr);
                }
                if ( seT->isGoodArrayType() ) {
                    expr->type = make_shared<TypeDecl>(*seT->firstType);
                    expr->type->ref = true;
                    expr->type->constant |= seT->constant;
                } else if ( !seT->isRef() ) {
                    error("can only index ref", expr->subexpr->at, CompilationError::cant_index);
                } else if ( !seT->dim.size() ) {
                    error("can only index arrays", expr->subexpr->at, CompilationError::cant_index);
                } else {
                    expr->type = make_shared<TypeDecl>(*seT);
                    expr->type->ref = true;
                    expr->type->dim.pop_back();
                }
            }
            return Visitor::visit(expr);
        }
    // ExprBlock
        virtual void preVisit ( ExprBlock * block ) override {
            Visitor::preVisit(block);
            block->hasReturn = false;
            if ( block->isClosure ) {
                blocks.push_back(block);
                block->type = make_shared<TypeDecl>(*block->returnType);
            }
            pushVarStack();
            
        }
        virtual void preVisitBlockArgument ( ExprBlock * block, const VariablePtr & var, bool lastArg ) override {
            Visitor::preVisitBlockArgument(block, var, lastArg);
            if ( var->type->isAuto() && !var->init) {
                error("block argument type can't be infered, need initializer",
                      var->at, CompilationError::cant_infer_missing_initializer );
            }
        }
        virtual ExpressionPtr visitBlockArgumentInit (ExprBlock * block, const VariablePtr & arg, Expression * that ) override {
            if ( arg->type->isAuto() ) {
                auto argT = inferAutoType(arg->type, arg->init->type);
                if ( !argT ) {
                    error("block argument initialization type can't be infered, "
                          + arg->type->describe() + " = " + arg->init->type->describe(),
                          arg->at, CompilationError::cant_infer_mismatching_restrictions );
                } else {
                    applyAutoContracts(argT, arg->type);
                    arg->type = argT;
                }
            }
            if ( !arg->init->type || !arg->type->isSameType(*arg->init->type, true, false) ) {
                error("block argument default value type mismatch", arg->init->at,
                  CompilationError::invalid_argument_type);
            }
            return Visitor::visitBlockArgumentInit(block, arg, that);
        }
        virtual ExpressionPtr visit ( ExprBlock * block ) override {
            popVarStack();
            if ( block->isClosure )
                blocks.pop_back();
            if ( block->isClosure ) {
                if ( block->list.size() ) {
                    uint32_t flags = block->getEvalFlags();
                    if ( flags & EvalFlags::stopForBreak ) {
                        error("captured block can't break outside of the block", block->at, CompilationError::invalid_block);
                    }
                }
                if ( !block->hasReturn && block->type->isAuto() ) {
                    block->returnType = make_shared<TypeDecl>(Type::tVoid);
                    block->type = make_shared<TypeDecl>(Type::tVoid);
                    reportGenericInfer();
                }
            }
            return Visitor::visit(block);
        }
    // ExprField
        virtual ExpressionPtr visit ( ExprField * expr ) override {
            if ( !expr->value->type ) return Visitor::visit(expr);
            // infer
            auto valT = expr->value->type;
            if ( valT->isArray() ) {
                error("can't get field of array", expr->at, CompilationError::cant_get_field);
                return Visitor::visit(expr);
            }
            if ( valT->isHandle() ) {
                expr->annotation = valT->annotation;
                expr->type = expr->annotation->makeFieldType(expr->name);
            } else if ( valT->baseType==Type::tStructure ) {
                expr->field = valT->structType->findField(expr->name);
            } else if ( valT->isPointer() ) {
                expr->value = Expression::autoDereference(expr->value);
                if ( valT->firstType->baseType==Type::tStructure ) {
                    expr->field = valT->firstType->structType->findField(expr->name);
                } else if ( valT->firstType->isHandle() ) {
                    expr->annotation = valT->firstType->annotation;
                    expr->type = expr->annotation->makeFieldType(expr->name);
                }
            }
            // handle
            if ( expr->field ) {
                expr->type = make_shared<TypeDecl>(*expr->field->type);
                expr->type->ref = true;
                expr->type->constant |= valT->constant;
            } else if ( !expr->type ) {
                error("field " + expr->name + " not found", expr->at, CompilationError::cant_get_field);
            } else {
                expr->type->constant |= valT->constant;
            }
            return Visitor::visit(expr);
        }
    // ExprSafeField
        virtual ExpressionPtr visit ( ExprSafeField * expr ) override {
            if ( !expr->value->type ) return Visitor::visit(expr);
            // infer
            auto valT = expr->value->type;
            if ( !valT->isPointer() || !valT->firstType ) {
                error("can only safe dereference a pointer to a structure or handle " + valT->describe(),
                      expr->at, CompilationError::cant_get_field);
                return Visitor::visit(expr);
            }
            expr->value = Expression::autoDereference(expr->value);
            if ( valT->firstType->structType ) {
                expr->field = valT->firstType->structType->findField(expr->name);
                if ( !expr->field ) {
                    error("can't get field " + expr->name, expr->at, CompilationError::cant_get_field);
                    return Visitor::visit(expr);
                }
                expr->type = make_shared<TypeDecl>(*expr->field->type);
            } else if ( valT->firstType->isHandle() ) {
                expr->annotation = valT->firstType->annotation;
                expr->type = expr->annotation->makeSafeFieldType(expr->name);
                if ( !expr->type ) {
                    error("can't get field " + expr->name, expr->at, CompilationError::cant_get_field);
                    return Visitor::visit(expr);
                }
            } else {
                error("can only safe dereference a pointer to a structure or handle " + valT->describe(),
                      expr->at, CompilationError::cant_get_field);
                return Visitor::visit(expr);
            }
            expr->skipQQ = expr->type->isPointer();
            if ( !expr->skipQQ ) {
                auto fieldType = expr->type;
                expr->type = make_shared<TypeDecl>(Type::tPointer);
                expr->type->firstType = fieldType;
            }
            expr->type->constant |= valT->constant;
            return Visitor::visit(expr);
        }
    // ExprVar
        virtual ExpressionPtr visit ( ExprVar * expr ) override {
            // local (that on the stack)
            for ( auto it = local.rbegin(); it!=local.rend(); ++it ) {
                auto var = *it;
                if ( var->name==expr->name ) {
                    expr->variable = var;
                    expr->local = true;
                    expr->type = make_shared<TypeDecl>(*var->type);
                    expr->type->ref = true;
                    return Visitor::visit(expr);
                }
            }
            // block arguments
            for ( auto it = blocks.rbegin(); it!=blocks.rend(); ++it ) {
                auto block = *it;
                int argumentIndex = 0;
                for ( auto & arg : block->arguments ) {
                    if ( arg->name==expr->name ) {
                        expr->variable = arg;
                        expr->argumentIndex = argumentIndex;
                        expr->block = true;
                        expr->type = make_shared<TypeDecl>(*arg->type);
                        if (!expr->type->isRefType())
                            expr->type->ref = true;
                        expr->pBlock = static_pointer_cast<ExprBlock>(block->shared_from_this());
                        return Visitor::visit(expr);
                    }
                    argumentIndex ++;
                }
            }
            // function argument
            if ( func ) {
                int argumentIndex = 0;
                for ( auto & arg : func->arguments ) {
                    if ( arg->name==expr->name ) {
                        expr->variable = arg;
                        expr->argumentIndex = argumentIndex;
                        expr->argument = true;
                        expr->type = make_shared<TypeDecl>(*arg->type);
                        if (!expr->type->isRefType())
                            expr->type->ref = true;
                        return Visitor::visit(expr);
                    }
                    argumentIndex ++;
                }
            }
            // global
            auto var = program->findVariable(expr->name);
            if ( !var ) {
                error("can't locate variable " + expr->name, expr->at, CompilationError::variable_not_found);
            } else {
                expr->variable = var;
                expr->type = make_shared<TypeDecl>(*var->type);
                expr->type->ref = true;
            }
            return Visitor::visit(expr);
        }
    // ExprOp1
        virtual ExpressionPtr visit ( ExprOp1 * expr ) override {
            if ( !expr->subexpr->type ) return Visitor::visit(expr);
            // infer
            vector<TypeDeclPtr> types = { expr->subexpr->type };
            auto functions = findMatchingFunctions(expr->op, types);
            if ( functions.size()==0 ) {
                string candidates = program->describeCandidates(findCandidates(expr->op,types));
                error("no matching operator '" + expr->op
                      + "' with argument (" + expr->subexpr->type->describe() + ")", expr->at, CompilationError::operator_not_found);
            } else if ( functions.size()>1 ) {
                string candidates = program->describeCandidates(functions);
                error("too many matching operators '" + expr->op
                      + "' with argument (" + expr->subexpr->type->describe() + ")", expr->at, CompilationError::operator_not_found);
            } else {
                expr->func = functions[0];
                expr->type = make_shared<TypeDecl>(*expr->func->result);
                if ( !expr->func->arguments[0]->type->isRef() )
                    expr->subexpr = Expression::autoDereference(expr->subexpr);
            }
            return Visitor::visit(expr);
        }
    // ExprOp2
        virtual ExpressionPtr visit ( ExprOp2 * expr ) override {
            if ( !expr->left->type || !expr->right->type ) return Visitor::visit(expr);
            // infer
            if ( expr->left->type->isPointer() && expr->right->type->isPointer() )
                if ( !expr->left->type->isSameType(*expr->right->type,false) )
                    error("operations on incompatible pointers are prohibited", expr->at);
            vector<TypeDeclPtr> types = { expr->left->type, expr->right->type };
            auto functions = findMatchingFunctions(expr->op, types);
            if ( functions.size()==0 ) {
                string candidates = program->describeCandidates(findCandidates(expr->op,types));
                error("no matching operator '" + expr->op
                      + "' with arguments (" + expr->left->type->describe() + ", " + expr->right->type->describe()
                      + ")\n" + candidates, expr->at, CompilationError::operator_not_found);
            } else if ( functions.size()>1 ) {
                string candidates = program->describeCandidates(functions);
                error("too many matching operators '" + expr->op
                      + "' with arguments (" + expr->left->type->describe() + ", " + expr->right->type->describe()
                      + ")\n" + candidates, expr->at, CompilationError::operator_not_found);
            } else {
                expr->func = functions[0];
                expr->type = make_shared<TypeDecl>(*expr->func->result);
                if ( !expr->func->arguments[0]->type->isRef() )
                    expr->left = Expression::autoDereference(expr->left);
                if ( !expr->func->arguments[1]->type->isRef() )
                    expr->right = Expression::autoDereference(expr->right);
            }
            return Visitor::visit(expr);
        }
    // ExprOp3
        virtual ExpressionPtr visit ( ExprOp3 * expr ) override {
            if ( !expr->subexpr->type || !expr->left->type || !expr->right->type ) return Visitor::visit(expr);
            // infer
            if ( expr->op!="?" ) {
                error("Op3 currently only supports 'is'", expr->at, CompilationError::operator_not_found);
                return Visitor::visit(expr);
            }
            expr->subexpr = Expression::autoDereference(expr->subexpr);
            if ( !expr->subexpr->type->isSimpleType(Type::tBool) ) {
                error("cond operator condition must be boolean", expr->at, CompilationError::condition_must_be_bool);
            } else if ( !expr->left->type->isSameType(*expr->right->type,false,false) ) {
                error("cond operator must return same types on both sides", expr->at, CompilationError::operator_not_found);
            } else {
                if ( expr->left->type->ref ^ expr->right->type->ref ) { // if either one is not ref
                    expr->left = Expression::autoDereference(expr->left);
                    expr->right = Expression::autoDereference(expr->right);
                }
                expr->type = make_shared<TypeDecl>(*expr->left->type);
                expr->type->constant |= expr->right->type->constant;
            }
            return Visitor::visit(expr);
        }
    // ExprMove
        virtual ExpressionPtr visit ( ExprMove * expr ) override {
            if ( !expr->left->type || !expr->right->type ) return Visitor::visit(expr);
            // infer
            if ( !expr->left->type->isSameType(*expr->right->type,false,false) ) {
                error("can only move same type", expr->at, CompilationError::operator_not_found);
            } else if ( !expr->left->type->isRef() ) {
                error("can only move to reference", expr->at, CompilationError::cant_write_to_non_reference);
            } else if ( expr->left->type->constant ) {
                error("can't move to constant value", expr->at, CompilationError::cant_move_to_const);
            } else if ( !expr->left->type->canMove() ) {
                error("this type can't be moved", expr->at, CompilationError::cant_move);
            } else if ( expr->left->type->canCopy() ) {
                error("this type can be copied, use = instead", expr->at, CompilationError::cant_move);
            }
            expr->type = make_shared<TypeDecl>();  // we return nothing
            return Visitor::visit(expr);
        }
    // ExprCopy
        virtual ExpressionPtr visit ( ExprCopy * expr ) override {
            if ( !expr->left->type || !expr->right->type ) return Visitor::visit(expr);
            // infer
            if ( !expr->left->type->isSameType(*expr->right->type,false,false) ) {
                error("can only copy same type " + expr->left->type->describe() + " vs " + expr->right->type->describe(),
                      expr->at, CompilationError::operator_not_found);
            } else if ( !expr->left->type->isRef() ) {
                error("can only copy to reference", expr->at, CompilationError::cant_write_to_non_reference);
            } else if ( expr->left->type->constant ) {
                error("can't write to constant value", expr->at, CompilationError::cant_write_to_const);
            }
            if ( !expr->left->type->canCopy() ) {
                error("this type can't be copied, use <- instead", expr->at, CompilationError::cant_copy);
            }
            expr->type = make_shared<TypeDecl>();  // we return nothing
            return Visitor::visit(expr);
        }
    // ExprTryCatch
        // do nothing
    // ExprReturn
        bool inferReturnType ( TypeDeclPtr & resType, ExprReturn * expr ) {
            if ( resType->isAuto() ) {
                if ( expr->subexpr ) {
                    auto resT = inferAutoType(resType, expr->subexpr->type);
                    if ( !resT ) {
                        error("type can't be infered, "
                              + resType->describe() + ", returns " + expr->subexpr->type->describe(),
                              expr->at, CompilationError::cant_infer_mismatching_restrictions );
                    } else {
                        resT->ref = false;
                        applyAutoContracts(resT, resType);
                        resType = resT;
                        reportGenericInfer();
                        return true;
                    }
                } else {
                    resType = make_shared<TypeDecl>(Type::tVoid);
                    reportGenericInfer();
                    return true;
                }
            }
            if ( resType->isVoid() ) {
                if ( expr->subexpr ) {
                    error("not expecting return value", expr->at, CompilationError::not_expecting_return_value);
                }
            } else {
                if ( !expr->subexpr ) {
                    error("expecting return value", expr->at, CompilationError::expecting_return_value);
                } else {
                    if ( !resType->isSameType(*expr->subexpr->type,true,false) ) {
                        error("incompatible return type, expecting "
                              + resType->describe() + " vs " + expr->subexpr->type->describe(),
                              expr->at, CompilationError::invalid_return_type);
                    }
                    if ( resType->isPointer() && !resType->isConst() && expr->subexpr->type->isConst() ) {
                        error("incompatible return type, constant matters. expecting "
                              + resType->describe() + " vs " + expr->subexpr->type->describe(),
                              expr->at, CompilationError::invalid_return_type);
                    }
                }
            }
            return false;
        }
        virtual ExpressionPtr visit ( ExprReturn * expr ) override {
            if ( blocks.size() ) {
                ExprBlock * block = blocks.back();
                block->hasReturn = true;
                if ( expr->subexpr ) {
                    if ( !expr->subexpr->type ) return Visitor::visit(expr);
                    expr->subexpr = Expression::autoDereference(expr->subexpr);
                }
                if ( inferReturnType(block->type, expr) ) {
                    block->returnType = make_shared<TypeDecl>(*block->type);
                }
            } else {
                // infer
                func->hasReturn = true;
                if ( expr->subexpr ) {
                    if ( !expr->subexpr->type ) return Visitor::visit(expr);
                    expr->subexpr = Expression::autoDereference(expr->subexpr);
                }
                inferReturnType(func->result, expr);
            }
            expr->type = make_shared<TypeDecl>();
            return Visitor::visit(expr);
        }
    // ExprBreak
        virtual ExpressionPtr visit ( ExprBreak * expr ) override {
            if ( !loop.size() )
                error("break without loop", expr->at);
            return Visitor::visit(expr);
        }
    // ExprIfThenElse
        virtual ExpressionPtr visit ( ExprIfThenElse * expr ) override {
            if ( !expr->cond->type ) return Visitor::visit(expr);
            // infer
            if ( !expr->cond->type->isSimpleType(Type::tBool) ) {
                error("if-then-else condition must be boolean", expr->at, CompilationError::condition_must_be_bool);
            }
            return Visitor::visit(expr);
        }
    // ExprWhile
        virtual void preVisit ( ExprWhile * expr ) override {
            Visitor::preVisit(expr);
            loop.push_back(expr->shared_from_this());
        }
        virtual ExpressionPtr visit ( ExprWhile * expr ) override {
            loop.pop_back();
            if ( !expr->cond->type ) return Visitor::visit(expr);
            // infer
            if ( !expr->cond->type->isSimpleType(Type::tBool) ) {
                error("while loop condition must be boolean", expr->at);
            }
            return Visitor::visit(expr);
        }
    // ExprFor
        virtual void preVisit ( ExprFor * expr ) override {
            Visitor::preVisit(expr);
            loop.push_back(expr->shared_from_this());
            pushVarStack();
        }
        virtual void preVisitForStack ( ExprFor * expr ) override {
            Visitor::preVisitForStack(expr);
            if ( !expr->iterators.size() ) {
                error("for needs at least one iterator", expr->at);
                return;
            } else if ( expr->iterators.size() != expr->sources.size() ) {
                error("for needs as many iterators as there are sources", expr->at);
                return;
            } else if ( expr->sources.size()>MAX_FOR_ITERATORS ) {
                error("too many sources for now", expr->at);
                return;
            }
            // iterator variables
            int idx = 0;
            expr->iteratorVariables.clear();
            for ( auto & src : expr->sources ) {
                if ( !src->type ) continue;
                auto pVar = make_shared<Variable>();
                pVar->name = expr->iterators[idx];
                pVar->at = expr->at;
                if ( src->type->dim.size() ) {
                    pVar->type = make_shared<TypeDecl>(*src->type);
                    pVar->type->ref = true;
                    pVar->type->dim.pop_back();
                } else if ( src->type->isGoodIteratorType() ) {
                    pVar->type = make_shared<TypeDecl>(*src->type->firstType);
                } else if ( src->type->isGoodArrayType() ) {
                    pVar->type = make_shared<TypeDecl>(*src->type->firstType);
                    pVar->type->ref = true;
                } else if ( src->type->isRange() ) {
                    pVar->type = make_shared<TypeDecl>(src->type->getRangeBaseType());
                    pVar->type->ref = false;
                } else if ( src->type->isHandle() && src->type->annotation->isIterable() ) {
                    pVar->type = make_shared<TypeDecl>(*src->type->annotation->makeIteratorType());
                } else {
                    error("unsupported iteration type for " + pVar->name, expr->at);
                    return;
                }
                pVar->type->constant |= src->type->isConst();
                pVar->source = src;
                local.push_back(pVar);
                expr->iteratorVariables.push_back(pVar);
                ++ idx;
            }
        }
        virtual ExpressionPtr visit ( ExprFor * expr ) override {
            popVarStack();
            loop.pop_back();
            return Visitor::visit(expr);
        }
    // ExprLet
        virtual void preVisit ( ExprLet * expr ) override {
            Visitor::preVisit(expr);
            pushVarStack();
        }
        virtual void preVisitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            Visitor::preVisitLet(expr, var, last);
            if ( var->type->isAlias() ) {
                auto aT = inferAlias(var->type);
                if ( aT ) {
                    var->type = aT;
                    reportGenericInfer();
                } else {
                    error("udefined type " + var->type->describe(), var->at, CompilationError::type_not_found);
                }
            }
            if ( var->type->isAuto() && !var->init) {
                error("local variable type can't be infered, need initializer",
                      var->at, CompilationError::cant_infer_missing_initializer );
            }
            local.push_back(var);
        }
        virtual VariablePtr visitLet ( ExprLet * expr, const VariablePtr & var, bool last ) override {
            if ( var->type->ref )
                error("local variable can't be reference", var->at, CompilationError::invalid_variable_type);
            if ( var->type->isVoid() )
                error("local variable can't be void", var->at, CompilationError::invalid_variable_type);
            if ( var->type->isHandle() && !var->type->annotation->isLocal() )
                error("handled type " + var->type->annotation->name + " can't be local", var->at, CompilationError::invalid_variable_type);
            return Visitor::visitLet(expr,var,last);
        }
        virtual ExpressionPtr visitLetInit ( ExprLet * expr, const VariablePtr & var, Expression * init ) override {
            if ( !var->init->type ) return Visitor::visitLetInit(expr, var, init);
            if ( var->type->isAuto() ) {
                auto varT = inferAutoType(var->type, var->init->type);
                if ( !varT ) {
                    error("local variable initialization type can't be infered, "
                          + var->type->describe() + " = " + var->init->type->describe(),
                          var->at, CompilationError::cant_infer_mismatching_restrictions );
                } else {
                    varT->ref = false;
                    applyAutoContracts(varT, var->type);
                    var->type = varT;
                }
            } else if ( !var->type->isSameType(*var->init->type,false,false) ) {
                error("variable initialization type mismatch, "
                      + var->type->describe() + " = " + var->init->type->describe(), var->at );
            } else if ( var->type->baseType==Type::tStructure ) {
                error("can't initialize structures", var->at );
            } else if ( !var->init->type->canCopy() && !var->init->type->canMove() ) {
                error("this variable can't be initialized at all", var->at);
            }
            //if ( var->type->isWorkhorseType() ) {
            //    return Expression::autoDereference(init->shared_from_this());
            //} else {
                return Visitor::visitLetInit(expr, var, init);
            //}
        }
        virtual ExpressionPtr visit ( ExprLet * expr ) override {
            if ( expr->scoped ) {
                popVarStack();
            }
            return Visitor::visit(expr);
        }
    // ExprLooksLikeCall
        virtual void preVisit ( ExprLooksLikeCall * call ) override {
            Visitor::preVisit(call);
            call->argumentsFailedToInfer = false;
        }
        virtual ExpressionPtr visitLooksLikeCallArg ( ExprLooksLikeCall * call, Expression * arg , bool last ) override {
            if ( !arg->type ) call->argumentsFailedToInfer = true;
            return Visitor::visitLooksLikeCallArg(call, arg, last);
        }
    // ExprCall
        virtual void preVisit ( ExprCall * call ) override {
            Visitor::preVisit(call);
            call->argumentsFailedToInfer = false;
        }
        virtual ExpressionPtr visitCallArg ( ExprCall * call, Expression * arg , bool last ) override {
            if ( !arg->type ) call->argumentsFailedToInfer = true;
            return Visitor::visitCallArg(call, arg, last);
        }
        virtual ExpressionPtr visit ( ExprCall * expr ) override {
            // infer
            vector<TypeDeclPtr> types;
            types.reserve(expr->arguments.size());
            for ( auto & ar : expr->arguments ) {
                if ( !ar->type ) {
                    return Visitor::visit(expr);
                }
                types.push_back(ar->type);
            }
            auto functions = findMatchingFunctions(expr->name, types, true);
            if ( functions.size()==0 ) {
                // ok, time to find us a good generic
                auto generics = findMatchingGenerics(expr->name, types);
                if ( generics.size()==1 ) {
                    auto generic = generics[0];
                    auto clone = generic->clone();
                    clone->inferStack.emplace_back(expr->at, func);
                    clone->inferStack.insert(clone->inferStack.end(), func->inferStack.begin(), func->inferStack.end());
                    for ( size_t sz = 0; sz != types.size(); ++sz ) {
                        auto & argT = clone->arguments[sz]->type;
                        if ( argT->isAuto() ) {
                            auto & passT = types[sz];
                            auto resT = inferAutoType(argT, passT);
                            assert(resT && "how? we had this working at findMatchingGenerics");
                            argT = resT;
                        }
                    }
                    if ( program->addFunction(clone) ) {
                        reportGenericInfer();
                    } else {
						auto realFn = program->findFunction(clone->getMangledName());
						vector<FunctionPtr> candidates = { realFn };
						reportFunctionNotFound("no matching generic function " + expr->describe(), expr->at, candidates, types, true, true);
                    }
                } else if ( generics.size()>0 ) {
					reportFunctionNotFound("too many matching generic functions " + expr->describe(), expr->at, findGenericCandidates(expr->name, types), types, true, true);
                } else {
                    if ( auto aliasT = findAlias(expr->name) ) {
                        if ( aliasT->isCtorType() ) {
                            expr->name = to_string(aliasT->baseType);
                            reportGenericInfer();
                        }
                    } else {
						reportFunctionNotFound("no matching function " + expr->describe(), expr->at, findCandidates(expr->name, types), types, false, true);
                    }
                }
            } else if ( functions.size()>1 ) {
                string candidates = program->describeCandidates(functions);
                error("too many matching functions " + expr->describe() + "\n" + candidates, expr->at, CompilationError::function_not_found);
            } else {
                expr->func = functions[0];
                expr->type = make_shared<TypeDecl>(*expr->func->result);
                // infer FORWARD types
                for ( size_t iF=0; iF!=expr->arguments.size(); ++iF ) {
                    auto & arg = expr->arguments[iF];
                    if ( arg->type->isAuto() && arg->type->isGoodBlockType() ) {
                        assert ( arg->rtti_isMakeBlock() && "always MakeBlock" );
                        auto mkBlock = static_pointer_cast<ExprMakeBlock>(arg);
                        auto block = static_pointer_cast<ExprBlock>(mkBlock->block);
                        auto retT = inferAutoType(mkBlock->type, expr->func->arguments[iF]->type);
                        assert ( retT && "how? it matched during findMatchingFunctions the same way");
                        block->returnType = make_shared<TypeDecl>(*retT->firstType);
                        for ( size_t ba=0; ba!=retT->argTypes.size(); ++ba ) {
                            block->arguments[ba]->type = make_shared<TypeDecl>(*retT->argTypes[ba]);
                        }
                        reportGenericInfer();
                    }
                }
                // append default arguments
                for ( size_t iT = expr->arguments.size(); iT != expr->func->arguments.size(); ++iT ) {
                    auto newArg = expr->func->arguments[iT]->init->clone();
                    if ( !newArg->type ) {
                        // recursive resolve???
                        newArg = newArg->visit(*this);
                    }
                    expr->arguments.push_back(newArg);
                }
                // dereference what needs dereferences
                for ( size_t iA = 0; iA != expr->arguments.size(); ++iA )
                    if ( !expr->func->arguments[iA]->type->isRef() )
                        expr->arguments[iA] = Expression::autoDereference(expr->arguments[iA]);
            }
            return Visitor::visit(expr);
        }
    // ExprKeys
        virtual ExpressionPtr visit ( ExprKeys * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=1 ) {
                error("expecting Keys(table)", expr->at);
            }
            // infer
            auto tableType = expr->arguments[0]->type;
            if ( !tableType->isGoodTableType() ) {
                error("must be fully qualified table", expr->at);
                return Visitor::visit(expr);
            }
            auto iterType = tableType->firstType;
            expr->arguments[0]->type->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tIterator);
            expr->type->firstType = make_shared<TypeDecl>(*iterType);
            expr->type->firstType->ref = true;
            expr->type->firstType->constant = true;
            return Visitor::visit(expr);
        }
    // ExprValues
        virtual ExpressionPtr visit ( ExprValues * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=1 ) {
                error("expecting Values(table)", expr->at);
            }
            // infer
            auto tableType = expr->arguments[0]->type;
            if ( !tableType->isGoodTableType() ) {
                error("must be fully qualified table", expr->at);
                return Visitor::visit(expr);
            }
            auto iterType = tableType->secondType;
            expr->arguments[0]->type->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tIterator);
            expr->type->firstType = make_shared<TypeDecl>(*iterType);
            expr->type->firstType->ref = true;
            expr->type->firstType->constant = false;
            return Visitor::visit(expr);
        }
    // ArrayResize
        virtual ExpressionPtr visit ( ExprArrayResize * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=2 ) {
                error("expecting ArrayResize(array,size)", expr->at);
            }
            // infer
            auto arrayType = expr->arguments[0]->type;
            auto valueType = expr->arguments[1]->type;
            if ( !arrayType->isGoodArrayType() ) {
                error("must be fully qualified array", expr->at);
                return Visitor::visit(expr);
            }
            if ( !valueType->isIndex() )
                error("size must be int or uint", expr->at);
            expr->arguments[1] = Expression::autoDereference(expr->arguments[1]);
            valueType->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tVoid);
            return Visitor::visit(expr);
        }
        // ArrayReserve
        virtual ExpressionPtr visit ( ExprArrayReserve * expr ) override {
            if ( expr->argumentsFailedToInfer ) return Visitor::visit(expr);
            if ( expr->arguments.size()!=2 ) {
                error("expecting ArrayReserve(array,size)", expr->at);
            }
            // infer
            auto arrayType = expr->arguments[0]->type;
            auto valueType = expr->arguments[1]->type;
            if ( !arrayType->isGoodArrayType() ) {
                error("must be fully qualified array", expr->at);
                return Visitor::visit(expr);
            }
            if ( !valueType->isIndex() )
                error("size must be int or uint", expr->at);
            expr->arguments[1] = Expression::autoDereference(expr->arguments[1]);
            valueType->constant = true;
            expr->type = make_shared<TypeDecl>(Type::tVoid);
            return Visitor::visit(expr);
        }
        // StringBuilder
        virtual ExpressionPtr visitStringBuilderElement ( ExprStringBuilder * sb, Expression * expr, bool last ) override {
            return Expression::autoDereference(expr->shared_from_this());
        }
        virtual ExpressionPtr visit ( ExprStringBuilder * expr ) override {
            expr->type = make_shared<TypeDecl>(Type::tString);
            return Visitor::visit(expr);
        }
    };
     
    // program
    
    void Program::inferTypes() {
        const bool log = options.getOption("logInferPasses",false);
        for ( int pass = 0; ; ++pass ) {
            failToCompile = false;
            errors.clear();
            InferTypes context(shared_from_this());
            visit(context);
            if ( log ) {
                cout << "PASS " << pass << ":\n" << *this;
				sort(errors.begin(), errors.end());
				for (auto & err : errors) {
					cout << reportError(nullptr, err.at.line, err.at.column, err.what, err.cerr);
				}
                cout.flush();
            }
            if ( context.finished() )
                break;
        }
    }
}

