#include "daScript/misc/platform.h"

#include "parser_impl.h"
#include "parser_state.h"

#include "daScript/ast/ast_generate.h"

#undef yyextra
#define yyextra (*((das::DasParserState **)(scanner)))

namespace das {

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

 }