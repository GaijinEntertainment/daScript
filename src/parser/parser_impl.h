#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_expressions.h"

#include "parser_state.h"

typedef void * yyscan_t;

namespace das {

    struct VariableNameAndPosition {
        string      name;
        string      aka;
        LineInfo    at;
    };

    struct VariableDeclaration {
        VariableDeclaration ( vector<VariableNameAndPosition> * n, TypeDecl * t, Expression * i )
            : pNameList(n), pTypeDecl(t), pInit(i) {}
        virtual ~VariableDeclaration () {
            if ( pNameList ) delete pNameList;
            delete pTypeDecl;
            if ( pInit ) delete pInit;
            if ( annotation ) delete annotation;
        }
        vector<VariableNameAndPosition>   *pNameList;
        TypeDecl                *pTypeDecl;
        Expression              *pInit;
        bool                    init_via_move = false;
        bool                    init_via_clone = false;
        bool                    override = false;
        bool                    sealed = false;
        bool                    isPrivate = false;
        AnnotationArgumentList  *annotation = nullptr;
    };

    void das_yyerror ( yyscan_t scanner, const string & error, const LineInfo & at, CompilationError cerr );
    void das_checkName ( yyscan_t scanner, const string & name, const LineInfo &at );

    vector<ExpressionPtr> sequenceToList ( Expression * arguments );
    ExprLooksLikeCall * parseFunctionArguments ( ExprLooksLikeCall * pCall, Expression * arguments );
    void deleteVariableDeclarationList ( vector<VariableDeclaration *> * list );
    void varDeclToTypeDecl ( yyscan_t scanner, TypeDecl * pType, vector<VariableDeclaration*> * list, bool needNames = true );
    Annotation * findAnnotation ( yyscan_t scanner, const string & name, const LineInfo & at );
    void runFunctionAnnotations ( yyscan_t scanner, Function * func, AnnotationList * annL, const LineInfo & at );

    Expression * ast_arrayComprehension (yyscan_t scanner, const LineInfo & loc, vector<VariableNameAndPosition> * iters,
        Expression * srcs, Expression * subexpr, Expression * where, const LineInfo & forend, bool genSyntax   );
    Structure * ast_structureName ( yyscan_t scanner, bool sealed, string * name, const LineInfo & atName,
        string * parent, const LineInfo & atParent );
    void ast_structureDeclaration (  yyscan_t scanner, AnnotationList * annL, const LineInfo & loc, Structure * ps,
        const LineInfo & atPs, vector<VariableDeclaration*> * list );

}