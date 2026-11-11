#pragma once

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_visitor.h"
#include "daScript/ast/ast_expressions.h"

// src/ast/ARCHITECTURE.md#devirtualization

namespace das {

    namespace devirt {

        inline const Structure * classOf ( TypeDecl * type ) {
            if ( type && type->isPointer() ) type = type->firstType;
            if ( !type || type->baseType != Type::tStructure ) return nullptr;
            return type->structType;
        }

        inline const Structure * familyRoot ( const Structure * cls, const string & name ) {
            auto root = cls;
            while ( root->parent && root->parent->findField(name) ) root = root->parent;
            return root;
        }

        inline bool derivesFrom ( const Structure * cls, const Structure * base ) {
            for ( auto s = cls; s; s = s->parent ) {
                if ( s == base ) return true;
            }
            return false;
        }

        inline const Structure * invokedClass ( ExprField * eField ) {
            if ( !eField->value->rtti_isTypeDecl() ) return nullptr;
            return classOf(static_cast<ExprTypeDecl *>(eField->value)->typeexpr);
        }

        struct MethodFieldWrites : Visitor {
            das_hash_set<string> written;
            das_hash_set<Expression *> reads;
            vector<pair<Expression *, string>> fields;
            static string family ( const Structure * cls, const string & name ) {
                return familyRoot(cls, name)->getMangledName() + "`" + name;
            }
            static bool isMethod ( const Structure * cls, const string & name ) {
                auto field = cls ? cls->findField(name) : nullptr;
                return field && field->classMethod;
            }
            void scan ( Module * mod ) {
                mod->functions.foreach([&](auto fn) {
                    if ( !fn->isTemplate && fn->body ) fn->visit(*this);
                });
                mod->globals.foreach([&](auto var) {
                    if ( var->init ) var->init->visit(*this);
                });
                for ( auto & f : fields ) {
                    if ( reads.find(f.first) == reads.end() ) written.insert(f.second);
                }
            }
            void noteField ( Expression * expr, const Structure * cls, const string & name ) {
                if ( isMethod(cls, name) ) fields.emplace_back(expr, family(cls, name));
            }
            virtual void preVisit ( ExprField * expr ) override {
                Visitor::preVisit(expr);
                auto value = expr->value;
                noteField(expr, value->rtti_isTypeDecl() ? classOf(static_cast<ExprTypeDecl *>(value)->typeexpr) : classOf(value->type), expr->name);
            }
            virtual void preVisit ( ExprSafeField * expr ) override {
                Visitor::preVisit(expr);
                noteField(expr, classOf(expr->value->type), expr->name);
            }
            virtual void preVisit ( ExprRef2Value * expr ) override {
                Visitor::preVisit(expr);
                reads.insert(expr->subexpr);
            }
            virtual void preVisit ( ExprInvoke * expr ) override {
                Visitor::preVisit(expr);
                if ( auto eField = expr->methodField() ) reads.insert(eField);
            }
            virtual void preVisit ( ExprMakeStruct * expr ) override {
                Visitor::preVisit(expr);
                auto cls = classOf(expr->makeType);
                if ( !cls ) return;
                for ( auto & st : expr->structs ) {
                    for ( auto & decl : *st ) {
                        if ( !isMethod(cls, decl->name) ) continue;
                        auto value = decl->value;
                        if ( value && value->rtti_isCast() ) value = static_cast<ExprCast *>(value)->subexpr;
                        bool ownMethod = value && value->rtti_isAddr()
                            && static_cast<ExprAddr *>(value)->func == cls->findMethodFunction(decl->name);
                        if ( !ownMethod ) written.insert(family(cls, decl->name));
                    }
                }
            }
        };

    }

    bool devirtualize ( Module * mod );

}
