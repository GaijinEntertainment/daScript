#include "daScript/misc/platform.h"

#include "parser_impl.h"
#include "parser_state.h"

#include "daScript/ast/ast_generate.h"

#undef yyextra
#define yyextra (*((das::DasParserState **)(scanner)))

namespace das {

    static __forceinline string inThisModule ( const string & name ) { return "_::" + name; }

    void das_yyerror ( yyscan_t scanner, const string & error, const LineInfo & at, CompilationError cerr ) {
        yyextra->g_Program->error(error,"","",at,cerr);
    }

    void das_checkName ( yyscan_t scanner, const string & name, const LineInfo &at ) {
        if ( name.length()>=2 && name[0]=='_' && name[1]=='_' ) {
            yyextra->g_Program->error("names starting with __ are reserved, " + name,"","",at,CompilationError::invalid_name);
        }
    }

    vector<ExpressionPtr> sequenceToList ( Expression * arguments ) {
        vector<ExpressionPtr> argList;
        auto arg = arguments;
        if ( arg->rtti_isSequence() ) {
            while ( arg->rtti_isSequence() ) {
                auto pSeq = static_cast<ExprSequence *>(arg);
                DAS_ASSERT(!pSeq->right->rtti_isSequence());
                argList.push_back(pSeq->right);
                arg = pSeq->left.get();
            }
            argList.push_back(arg);
            reverse(argList.begin(),argList.end());
            delete arguments;
        } else {
            argList.push_back(ExpressionPtr(arg));
        }
        return argList;
    }

    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments ) {
        pCall->arguments = sequenceToList(arguments);
        return pCall;
    }

    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list ) {
        if ( !list ) return;
        for ( auto pD : *list )
            delete pD;
        delete list;
    }

    void varDeclToTypeDecl ( yyscan_t scanner, TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames ) {
        bool anyNames = false;
        for ( auto pDecl : *list ) {
            if ( pDecl->pTypeDecl ) {
                int count = pDecl->pNameList ? int(pDecl->pNameList->size()) : 1;
                for ( int ai=0; ai!=count; ++ai ) {
                    auto pVarType = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                    if ( pDecl->pInit ) {
                        das_yyerror(scanner,"can't have default values in a type declaration",
                            (*pDecl->pNameList)[ai].at,CompilationError::cant_initialize);
                    }
                    pType->argTypes.push_back(pVarType);
                    if ( needNames && pDecl->pNameList && !(*pDecl->pNameList)[ai].name.empty() ) {
                        if ( !(*pDecl->pNameList)[ai].aka.empty() ) {
                            das_yyerror(scanner,"type declaration can't have an aka", (*pDecl->pNameList)[ai].at,
                                CompilationError::invalid_aka);
                        }
                        anyNames = true;
                    }
                }
            }
        }
        if ( anyNames ) {
            for ( auto pDecl : *list ) {
                if ( pDecl->pTypeDecl ) {
                    if ( pDecl->pNameList ) {
                        for ( const auto & name : *pDecl->pNameList ) {
                            pType->argNames.push_back(name.name);
                        }
                    } else {
                        pType->argNames.push_back(string());
                    }
                }
            }
        }
    }

    Annotation * findAnnotation ( yyscan_t scanner, const string & name, const LineInfo & at ) {
        auto ann = yyextra->g_Program->findAnnotation(name);
        if ( ann.size()==1 ) {
            return ann.back().get();
        } else if ( ann.size()==0 ) {
            das_yyerror(scanner,"annotation " + name + " not found", at, CompilationError::annotation_not_found );
            return nullptr;
        } else {
            string candidates = yyextra->g_Program->describeCandidates(ann);
            das_yyerror(scanner,"too many options for annotation " + name + "\n" + candidates, at, CompilationError::annotation_not_found );
            return nullptr;
        }
    }

    void runFunctionAnnotations ( yyscan_t scanner, Function * func, AnnotationList * annL, const LineInfo & at ) {
        if ( annL ) {
            for ( const auto & pA : *annL ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(func, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to a function " + func->name + "\n" + err, at,
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror(scanner,"functions are only allowed function macros",
                            at, CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( func->annotations, *annL );
            for ( const auto & pA : *annL ) {
                func->annotations.push_back(pA);
            }
            delete annL;
        }
    }

    Expression * ast_arrayComprehension ( yyscan_t scanner, const LineInfo & loc, vector<VariableNameAndPosition> * iters,
        Expression * srcs, Expression * subexpr, Expression * where, const LineInfo & forend, bool genSyntax   ) {
        auto pFor = make_smart<ExprFor>(loc);
        pFor->visibility = forend;
        for ( const auto & np : *iters ) {
            pFor->iterators.push_back(np.name);
            pFor->iteratorsAka.push_back(np.aka);
            if ( !np.aka.empty() ) {
                das_yyerror(scanner,"array comprehension can't have an aka",np.at,
                    CompilationError::invalid_aka);
            }
            pFor->iteratorsAt.push_back(np.at);
        }
        delete iters;
        pFor->sources = sequenceToList(srcs);
        auto pAC = new ExprArrayComprehension(loc);
        pAC->generatorSyntax = genSyntax;
        pAC->exprFor = pFor;
        pAC->subexpr = ExpressionPtr(subexpr);
        if ( where ) {
            pAC->exprWhere = ExpressionPtr(where);
        }
        return pAC;
    }

    Structure * ast_structureName ( yyscan_t scanner, bool sealed, string * name, const LineInfo & atName,
        string * parent, const LineInfo & atParent ) {
        das_checkName(scanner,*name,atName);
        StructurePtr pStruct;
        if ( parent ) {
            auto structs = yyextra->g_Program->findStructure(*parent);
            if ( structs.size()==1 ) {
                pStruct = structs.back()->clone();
                pStruct->name = *name;
                pStruct->parent = structs.back().get();
                if ( pStruct->parent->sealed ) {
                    das_yyerror(scanner,"can't derive from a sealed class or structure "+*parent,atParent,
                        CompilationError::invalid_override);
                }
                pStruct->annotations.clear();
                pStruct->genCtor = false;
                pStruct->macroInterface = pStruct->parent && pStruct->parent->macroInterface;

            } else if ( structs.size()==0 ) {
                das_yyerror(scanner,"parent structure not found " + *parent,atParent,
                    CompilationError::structure_not_found);
            } else {
                string candidates = yyextra->g_Program->describeCandidates(structs);
                das_yyerror(scanner,"too many options for " + *parent + "\n" + candidates,atParent,
                    CompilationError::structure_not_found);
            }
            delete parent;
        }
        if ( !pStruct ) {
            pStruct = make_smart<Structure>(*name);
        }
        pStruct->sealed = sealed;
        if ( !yyextra->g_Program->addStructure(pStruct) ) {
            das_yyerror(scanner,"structure is already defined "+*name,atName,
                CompilationError::structure_already_declared);
            delete name;
            return nullptr;
        } else {
            yyextra->g_thisStructure = pStruct.get();
            delete name;
            return pStruct.get();
        }
    }

    void ast_structureDeclaration (  yyscan_t scanner, AnnotationList * annL, const LineInfo & loc, Structure * ps,
        const LineInfo & atPs, vector<VariableDeclaration*> * list ) {
        if ( ps ) {
            auto pStruct = ps;
            pStruct->at = atPs;
            if ( pStruct->parent && pStruct->parent->isClass != pStruct->isClass ) {
                if ( pStruct->isClass ) {
                    das_yyerror(scanner,"class can only derive from a class", pStruct->at,
                        CompilationError::invalid_override);
                } else {
                    das_yyerror(scanner,"structure can only derive from a structure", pStruct->at,
                        CompilationError::invalid_override);
                }
            }
            if ( pStruct->isClass ) {
                makeClassRtti(pStruct);
                auto virtfin = makeClassFinalize(pStruct);
                if ( !yyextra->g_Program->addFunction(virtfin) ) {
                    das_yyerror(scanner,"built-in finalizer is already defined " + virtfin->getMangledName(),
                        virtfin->at, CompilationError::function_already_declared);
                }
            }
            for ( auto pDecl : *list ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    if ( !pStruct->isClass && pDecl->isPrivate ) {
                        das_yyerror(scanner,"only class member can be private "+name_at.name,name_at.at,
                            CompilationError::invalid_private);
                    }
                    auto oldFd = (Structure::FieldDeclaration *) pStruct->findField(name_at.name);
                    if ( !oldFd ) {
                        if ( pDecl->override ) {
                            das_yyerror(scanner,"structure field is not overriding anything "+name_at.name,name_at.at,
                                CompilationError::invalid_override);
                        } else {
                            auto td = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            pStruct->fields.emplace_back(name_at.name, td, init,
                                pDecl->annotation ? *pDecl->annotation : AnnotationArgumentList(),
                                pDecl->init_via_move, name_at.at);
                            pStruct->fields.back().privateField = pDecl->isPrivate;
                            pStruct->fields.back().sealed = pDecl->sealed;
                        }
                    } else {
                        if ( pDecl->sealed || pDecl->override ) {
                            if ( oldFd->sealed ) {
                                das_yyerror(scanner,"structure field "+name_at.name+" is sealed",
                                    name_at.at, CompilationError::invalid_override);
                            }
                            auto init = pDecl->pInit ? ExpressionPtr(pDecl->pInit->clone()) : nullptr;
                            oldFd->init = init;
                            oldFd->parentType = oldFd->type->isAuto();
                            oldFd->privateField = pDecl->isPrivate;
                            oldFd->sealed = pDecl->sealed;
                        } else {
                            das_yyerror(scanner,"structure field is already declared "+name_at.name
                                +", use override to replace initial value instead",name_at.at,
                                    CompilationError::invalid_override);
                        }
                    }
                }
            }
            if ( annL ) {
                for ( auto pA : *annL ) {
                    if ( pA->annotation ) {
                        if ( pA->annotation->rtti_isStructureAnnotation() ) {
                            auto ann = static_pointer_cast<StructureAnnotation>(pA->annotation);
                            string err;
                            if ( !ann->touch(pStruct, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                                das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to the structure " + pStruct->name + "\n" + err,
                                    loc, CompilationError::invalid_annotation);
                            }
                        } else if ( pA->annotation->rtti_isStructureTypeAnnotation() ) {
                            if ( annL->size()!=1 ) {
                                das_yyerror(scanner,"structures are only allowed one structure macro", loc,
                                    CompilationError::invalid_annotation);
                            } else {
                                if ( !yyextra->g_Program->addStructureHandle(pStruct,
                                    static_pointer_cast<StructureTypeAnnotation>(pA->annotation), pA->arguments) ) {
                                        das_yyerror(scanner,"handled structure is already defined "+pStruct->name, loc,
                                        CompilationError::structure_already_declared);
                                } else {
                                    pStruct->module->removeStructure(pStruct);
                                }
                            }
                        }
                    }
                }
                swap ( pStruct->annotations, *annL );
                delete annL;
            }
        }
        deleteVariableDeclarationList(list);
        yyextra->g_thisStructure = nullptr;
    }

    void ast_enumDeclaration (  yyscan_t scanner, AnnotationList * annL, const LineInfo & atannL, bool pubE,
        string * name, const LineInfo & atName, Enumeration * pE, const LineInfo & atpE, Type ebt ) {
        das_checkName(scanner,*name,atName);
        auto pEnum = EnumerationPtr(pE);
        pEnum->at = atName;
        pEnum->name = *name;
        pEnum->baseType = ebt;
        pEnum->isPrivate = !pubE;
        if ( annL ) {
            for ( auto pA : *annL ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isEnumerationAnnotation() ) {
                        auto ann = static_pointer_cast<EnumerationAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->touch(pEnum, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to finalize the enumeration " + pEnum->name + "\n" + err, atannL,
                                CompilationError::invalid_annotation);
                        }
                    }
                }
            }
            swap ( pEnum->annotations, *annL );
            delete annL;
        }
        if ( !yyextra->g_Program->addEnumeration(pEnum) ) {
            das_yyerror(scanner,"enumeration is already defined "+*name, atpE,
                CompilationError::enumeration_already_declared);
        }
        delete name;
    }


    void ast_globalLetList (  yyscan_t scanner, bool kwd_let, bool glob_shar, bool pub_var, vector<VariableDeclaration*> * list ) {
        for ( auto pDecl : *list ) {
            if ( pDecl->pTypeDecl ) {
                for ( const auto & name_at : *pDecl->pNameList ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    if ( !name_at.aka.empty() ) {
                        das_yyerror(scanner,"global variable " + name_at.name + " can't have an aka",name_at.at,
                            CompilationError::invalid_aka);
                    }
                    pVar->at = name_at.at;
                    pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                    if ( pDecl->pInit ) {
                        pVar->init = pDecl->pInit->clone();
                        pVar->init_via_move = pDecl->init_via_move;
                        pVar->init_via_clone = pDecl->init_via_clone;
                    }
                    if ( kwd_let ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pVar->global_shared = glob_shar;
                    pVar->private_variable = !pub_var;
                    if ( !yyextra->g_Program->addVariable(pVar) )
                        das_yyerror(scanner,"global variable is already declared " + name_at.name,name_at.at,
                            CompilationError::global_variable_already_declared);
                }
            }
        }
        deleteVariableDeclarationList(list);
    }

    void ast_globalLet ( yyscan_t scanner, bool kwd_let, bool glob_shar, bool pub_var,
        AnnotationArgumentList * ann, VariableDeclaration * decl ) {
        auto pDecl = decl;
        if ( pDecl->pTypeDecl ) {
            for ( const auto & name_at : *pDecl->pNameList ) {
                VariablePtr pVar = make_smart<Variable>();
                pVar->name = name_at.name;
                pVar->aka = name_at.aka;
                if ( !name_at.aka.empty() ) {
                    das_yyerror(scanner,"global variable " + name_at.name + " can't have an aka",name_at.at,
                        CompilationError::invalid_aka);
                }
                pVar->at = name_at.at;
                pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                if ( pDecl->pInit ) {
                    pVar->init = pDecl->pInit->clone();
                    pVar->init_via_move = pDecl->init_via_move;
                    pVar->init_via_clone = pDecl->init_via_clone;
                }
                if ( kwd_let ) {
                    pVar->type->constant = true;
                } else {
                    pVar->type->removeConstant = true;
                }
                pVar->global_shared = glob_shar;
                pVar->private_variable = !pub_var;
                if ( ann ) {
                    pVar->annotation = move(*ann);
                    delete ann;
                }
                if ( !yyextra->g_Program->addVariable(pVar) )
                    das_yyerror(scanner,"global variable is already declared " + name_at.name,name_at.at,
                        CompilationError::global_variable_already_declared);
            }
        }
        delete pDecl;
    }

    vector<VariableDeclaration*> * ast_structVarDefAbstract ( yyscan_t scanner, vector<VariableDeclaration*> * list,
        AnnotationList * annL, bool isPrivate, Function * func ) {
        if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror(scanner,"structure can't have a member function",
                func->at, CompilationError::invalid_member_function);
        } else if ( func->isGeneric() ) {
            das_yyerror(scanner,"generic function can't be a member of a class " + func->getMangledName(),
                func->at, CompilationError::invalid_member_function);
        } else if ( func->name==yyextra->g_thisStructure->name || func->name=="finalize" ) {
            das_yyerror(scanner,"initializers and finalizers can't be abstract " + func->getMangledName(),
                func->at, CompilationError::invalid_member_function);
        } else if ( annL!=nullptr ) {
            das_yyerror(scanner,"abstract functions can't have annotations " + func->getMangledName(),
                func->at, CompilationError::invalid_member_function);
            delete annL;
        } else if ( func->result->baseType==Type::autoinfer ) {
            das_yyerror(scanner,"abstract functions must specify return type explicitly " + func->getMangledName(),
                func->at, CompilationError::invalid_member_function);
        } else {
            auto varName = func->name;
            func->name = yyextra->g_thisStructure->name + "`" + func->name;
            auto vars = new vector<VariableNameAndPosition>();
            vars->emplace_back(VariableNameAndPosition{varName,"",func->at});
            TypeDecl * funcType = new TypeDecl(Type::tFunction);
            funcType->at = func->at;
            swap ( funcType->firstType, func->result );
            funcType->argTypes.reserve ( func->arguments.size() );
            if ( yyextra->g_thisStructure->isClass ) {
                funcType->argTypes.push_back(make_smart<TypeDecl>(yyextra->g_thisStructure));
                funcType->argNames.push_back("self");
            }
            for ( auto & arg : func->arguments ) {
                funcType->argTypes.push_back(arg->type);
                funcType->argNames.push_back(arg->name);
            }
            VariableDeclaration * decl = new VariableDeclaration(
                vars,
                funcType,
                nullptr
            );
            decl->isPrivate = isPrivate;
            list->push_back(decl);
        }
        func->delRef();
        return list;
    }

    vector<VariableDeclaration*> * ast_structVarDef ( yyscan_t scanner, vector<VariableDeclaration*> * list,
        AnnotationList * annL, bool isPrivate, int ovr, bool cnst, Function * func, Expression * block,
            const LineInfo & fromBlock, const LineInfo & annLAt ) {
        func->atDecl = fromBlock;
        func->body = block;
        if ( !yyextra->g_thisStructure ) {
            das_yyerror(scanner,"internal error or invalid macro. member function is declared outside of a class",
                func->at, CompilationError::invalid_member_function);
        } else if ( !yyextra->g_thisStructure->isClass ) {
            das_yyerror(scanner,"structure can't have a member function",
                func->at, CompilationError::invalid_member_function);
        } else if ( func->isGeneric() ) {
            das_yyerror(scanner,"generic function can't be a member of a class " + func->getMangledName(),
                func->at, CompilationError::invalid_member_function);
        } else {
            func->privateFunction = yyextra->g_thisStructure->privateStructure;
            if ( func->name != yyextra->g_thisStructure->name && func->name != "finalize") {
                auto varName = func->name;
                func->name = yyextra->g_thisStructure->name + "`" + func->name;
                auto vars = new vector<VariableNameAndPosition>();
                vars->emplace_back(VariableNameAndPosition{varName,"",func->at});
                Expression * finit = new ExprAddr(func->at, inThisModule(func->name));
                if ( ovr == OVERRIDE_OVERRIDE ) {
                    finit = new ExprCast(func->at, finit, make_smart<TypeDecl>(Type::autoinfer));
                }
                VariableDeclaration * decl = new VariableDeclaration(
                    vars,
                    new TypeDecl(Type::autoinfer),
                    finit
                );
                decl->override = ovr == OVERRIDE_OVERRIDE;
                decl->sealed = ovr == OVERRIDE_SEALED;
                decl->isPrivate = isPrivate;
                list->push_back(decl);
                modifyToClassMember(func, yyextra->g_thisStructure, false, cnst);
            } else {
                if ( ovr ) {
                    das_yyerror(scanner,"can't override an initializer or a finalizer " + func->getMangledName(),
                        func->at, CompilationError::invalid_member_function);
                }
                if ( cnst ) {
                    das_yyerror(scanner,"can't have a constant initializer or a finalizer " + func->getMangledName(),
                        func->at, CompilationError::invalid_member_function);
                }
                if ( func->name!="finalize" ) {
                    auto ctr = makeClassConstructor(yyextra->g_thisStructure, func);
                    if ( !yyextra->g_Program->addFunction(ctr) ) {
                        das_yyerror(scanner,"intializer is already defined " + ctr->getMangledName(),
                            ctr->at, CompilationError::function_already_declared);
                    }
                    func->name = yyextra->g_thisStructure->name + "`" + yyextra->g_thisStructure->name;
                    modifyToClassMember(func, yyextra->g_thisStructure, false, false);
                } else {
                    modifyToClassMember(func, yyextra->g_thisStructure, true, false);
                }
            }
            assignDefaultArguments(func);
            runFunctionAnnotations(scanner, func, annL, annLAt);
            if ( !yyextra->g_Program->addFunction(func) ) {
                das_yyerror(scanner,"function is already defined " + func->getMangledName(),
                    func->at, CompilationError::function_already_declared);
            }
            func->delRef();
        }
        return list;
    }

    Expression * ast_NameName ( yyscan_t scanner, string * ena, string * eni, const LineInfo & enaAt, const LineInfo & eniAt ) {
        Enumeration * pEnum = nullptr;
        Expression * resConst = nullptr;
        auto enums = yyextra->g_Program->findEnum(*ena);
        auto aliases = yyextra->g_Program->findAlias(*ena);
        if ( enums.size()+aliases.size() > 1 ) {
            string candidates;
            if ( enums.size() ) candidates += yyextra->g_Program->describeCandidates(enums);
            if ( aliases.size() ) candidates += yyextra->g_Program->describeCandidates(aliases);
            das_yyerror(scanner,"too many options for the " + *ena + "\n" + candidates, enaAt,
                CompilationError::type_not_found);
        } else if ( enums.size()==0 && aliases.size()==0 ) {
            das_yyerror(scanner,"enumeration or bitfield not found " + *ena, enaAt,
                CompilationError::type_not_found);
        } else if ( enums.size()==1 ) {
            pEnum = enums.back().get();
        } else if ( aliases.size()==1 ) {
            auto alias = aliases.back();
            if ( alias->isEnum() ) {
                pEnum = alias->enumType;
            } else if ( alias->isBitfield() ) {
                int bit = alias->findArgumentIndex(*eni);
                if ( bit!=-1 ) {
                    auto td = make_smart<TypeDecl>(*alias);
                    td->ref = false;
                    auto bitConst = new ExprConstBitfield(eniAt, 1u << bit);
                    bitConst->bitfieldType = make_smart<TypeDecl>(*alias);
                    resConst = bitConst;
                } else {
                    das_yyerror(scanner,"enumeration or bitfield not found " + *ena, enaAt,
                        CompilationError::bitfield_not_found);
                }
            } else {
                das_yyerror(scanner,"expecting enumeration or bitfield " + *ena, enaAt,
                    CompilationError::syntax_error);
            }
        }
        if ( pEnum ) {
            auto ff = pEnum->find(*eni);
            if ( ff.second ) {
                auto td = make_smart<TypeDecl>(pEnum);
                resConst = new ExprConstEnumeration(eniAt, *eni, td);
            } else {
                das_yyerror(scanner,"enumeraiton value not found " + *ena + " " + *eni, eniAt,
                    CompilationError::enumeration_not_found);
            }
        }
        delete ena;
        delete eni;
        if ( resConst ) {
            return resConst;
        } else {
            return new ExprConstInt(0);   // dummy
        }
    }


    Expression * ast_makeBlock ( yyscan_t scanner, int bal, AnnotationList * annL, vector<CaptureEntry> * clist,
        vector<VariableDeclaration*> * list, TypeDecl * result, Expression * block, const LineInfo & blockAt, const LineInfo & annLAt ) {
        auto mkb = new ExprMakeBlock(blockAt,ExpressionPtr(block), bal==1, bal==2);
        ExprBlock * closure = (ExprBlock *) block;
        closure->returnType = TypeDeclPtr(result);
        if ( list ) {
            for ( auto pDecl : *list ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !closure->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
                            if ( pDecl->annotation ) {
                                pVar->annotation = *pDecl->annotation;
                            }
                            closure->arguments.push_back(pVar);
                        } else {
                            das_yyerror(scanner,"block argument is already declared " + name_at.name,
                                name_at.at,CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList(list);
        }
        if ( clist ) {
            swap ( mkb->capture, *clist );
            delete clist;
            if ( bal != 1 ) {   // if its not lambda, can't capture
                das_yyerror(scanner,"can only have capture section for the lambda",
                    mkb->at,CompilationError::invalid_capture);
            }
        }
        if ( annL ) {
            for ( auto pA : *annL ) {
                if ( pA->annotation ) {
                    if ( pA->annotation->rtti_isFunctionAnnotation() ) {
                        auto ann = static_pointer_cast<FunctionAnnotation>(pA->annotation);
                        string err;
                        if ( !ann->apply(closure, *yyextra->g_Program->thisModuleGroup, pA->arguments, err) ) {
                            das_yyerror(scanner,"macro [" +pA->annotation->name + "] failed to apply to the block\n" + err, annLAt,
                                CompilationError::invalid_annotation);
                        }
                    } else {
                        das_yyerror(scanner,"blocks are only allowed function macros", annLAt,
                            CompilationError::invalid_annotation);
                    }
                }
            }
            swap ( closure->annotations, *annL );
            delete annL;
        }
        return mkb;
    }

    Expression * ast_Let ( yyscan_t scanner, bool kwd_let, VariableDeclaration * decl, const LineInfo & kwd_letAt, const LineInfo & declAt ) {
        auto pLet = new ExprLet();
        pLet->at = kwd_letAt;
        pLet->atInit = declAt;
        if ( decl->pTypeDecl ) {
            for ( const auto & name_at : *decl->pNameList ) {
                if ( !pLet->find(name_at.name) ) {
                    VariablePtr pVar = make_smart<Variable>();
                    pVar->name = name_at.name;
                    pVar->aka = name_at.aka;
                    pVar->at = name_at.at;
                    pVar->type = make_smart<TypeDecl>(*decl->pTypeDecl);
                    if ( decl->pInit ) {
                        pVar->init = decl->pInit->clone();
                        pVar->init_via_move = decl->init_via_move;
                        pVar->init_via_clone = decl->init_via_clone;
                    }
                    if ( kwd_let ) {
                        pVar->type->constant = true;
                    } else {
                        pVar->type->removeConstant = true;
                    }
                    pLet->variables.push_back(pVar);
                } else {
                    das_yyerror(scanner,"local variable is already declared " + name_at.name,name_at.at,
                        CompilationError::local_variable_already_declared);
                }
            }
        }
        delete decl;
        return pLet;
    }

    Function * ast_functionDeclarationHeader ( yyscan_t scanner, string * name, vector<VariableDeclaration*> * list,
        TypeDecl * result, const LineInfo & nameAt ) {
        auto pFunction = make_smart<Function>();
        pFunction->at = nameAt;
        pFunction->name = *name;
        pFunction->result = TypeDeclPtr(result);
        if ( list ) {
            for ( auto pDecl : *list ) {
                if ( pDecl->pTypeDecl ) {
                    for ( const auto & name_at : *pDecl->pNameList ) {
                        if ( !pFunction->findArgument(name_at.name) ) {
                            VariablePtr pVar = make_smart<Variable>();
                            pVar->name = name_at.name;
                            pVar->aka = name_at.aka;
                            pVar->at = name_at.at;
                            pVar->type = make_smart<TypeDecl>(*pDecl->pTypeDecl);
                            if ( pDecl->pInit ) {
                                pVar->init = ExpressionPtr(pDecl->pInit->clone());
                                pVar->init_via_move = pDecl->init_via_move;
                                pVar->init_via_clone = pDecl->init_via_clone;
                            }
                            if ( pDecl->annotation ) {
                                pVar->annotation = *pDecl->annotation;
                            }
                            pFunction->arguments.push_back(pVar);
                        } else {
                            das_yyerror(scanner,"function argument is already declared " + name_at.name,name_at.at,
                                CompilationError::argument_already_declared);
                        }
                    }
                }
            }
            deleteVariableDeclarationList(list);
        }
        delete name;
        return pFunction.orphan();
    }

    void ast_requireModule ( yyscan_t scanner, string * name, string * modalias, bool pub, const LineInfo & atName ) {
        auto info = yyextra->g_Access->getModuleInfo(*name, yyextra->g_FileAccessStack.back()->name);
        if ( auto mod = yyextra->g_Program->addModule(info.moduleName) ) {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple(mod,*name,"",pub,atName));
            yyextra->g_Program->thisModule->addDependency(mod, pub);
            if ( !info.importName.empty() ) {
                auto malias = modalias ? *modalias : info.importName;
                auto ita = yyextra->das_module_alias.find(malias);
                if ( ita !=yyextra->das_module_alias.end() ) {
                    if ( ita->second != info.moduleName ) {
                        das_yyerror(scanner,"module alias already used " + malias + " as " + ita->second,atName,
                            CompilationError::module_not_found);
                    }
                } else {
                    yyextra->das_module_alias[malias] = info.moduleName;
                }
            }
        } else {
            yyextra->g_Program->allRequireDecl.push_back(make_tuple((Module *)nullptr,*name,"",pub,atName));
            das_yyerror(scanner,"required module not found " + *name,atName,
                CompilationError::module_not_found);
        }
        delete name;
    }
 }