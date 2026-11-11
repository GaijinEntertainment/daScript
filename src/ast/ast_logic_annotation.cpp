#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_serializer.h"

namespace das
{
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif

    struct LogicOpAnnotation : FunctionAnnotation {
        LogicOpAnnotation(const string & name) : FunctionAnnotation(name) { }
        virtual bool apply ( const FunctionPtr &, ModuleGroup &, AnnotationArgumentList &, string & ) override { return true; }
        virtual bool apply(ExprBlock *, ModuleGroup &, AnnotationArgumentList &, string &) override { return true; }
        virtual bool finalize(ExprBlock *, ModuleGroup &, AnnotationArgumentList &, const AnnotationArgumentList &, string &) override { return true; }
        virtual bool finalize(const FunctionPtr &, ModuleGroup &, AnnotationArgumentList &, const AnnotationArgumentList &, string &) override { return true; }
        virtual bool isSpecialized() const override { return true; }
    };

    struct LogicOpNotAnnotation : LogicOpAnnotation {
        LogicOpNotAnnotation() : LogicOpAnnotation("!") { }
        LogicOpNotAnnotation(const AnnotationDeclarationPtr & decl) : LogicOpAnnotation("!"), subexpr(decl) { }
        virtual bool isCompatible ( const FunctionPtr & fun, const vector<TypeDeclPtr> & types, const AnnotationDeclaration &, string & errors  ) const override  {
            if ( !subexpr ) return false;
            return !((FunctionAnnotation *)(subexpr->annotation))->isCompatible(fun, types, *subexpr, errors);
        }
        virtual void appendToMangledName( const FunctionPtr & fun, const AnnotationDeclaration &, string & mangledName ) const override {
            if ( !subexpr ) return;
            string mna ;
            ((FunctionAnnotation *)(subexpr->annotation))->appendToMangledName(fun, *subexpr, mna);
            mangledName = "!(" + mna + ")";
        }
        virtual void log ( TextWriter & ss, const AnnotationDeclaration & ) const override {
            if ( !subexpr ) return;
            ss << "!(";
            subexpr->annotation->log(ss, *subexpr);
            ss << ")";
        }
        virtual void serialize ( AstSerializer & ser ) override {
            ser << subexpr;
        }
        virtual void gc_collect ( gc_root * target, gc_root * from ) override {
            Annotation::gc_collect(target, from);
            if ( subexpr ) subexpr->gc_collect(target, from);
        }
        AnnotationDeclarationPtr subexpr = nullptr;
    };

    struct LogicOp2Annotation : LogicOpAnnotation {
        LogicOp2Annotation ( const string & name ) : LogicOpAnnotation(name) { }
        LogicOp2Annotation ( const string & name, const AnnotationDeclarationPtr & arg0, const AnnotationDeclarationPtr & arg1 )
            : LogicOpAnnotation(name), left(arg0), right(arg1) { }
        virtual void appendToMangledName( const FunctionPtr & fun, const AnnotationDeclaration &, string & mangledName ) const override {
            if ( !left || !right ) return;
            string mna1, mna2;
            if ( left->annotation ) {
                ((FunctionAnnotation *)(left->annotation))->appendToMangledName(fun, *left, mna1);
            } else {
                mna1 = "NULL";
            }
            if ( right->annotation ) {
                ((FunctionAnnotation *)(right->annotation))->appendToMangledName(fun, *right, mna2);
            } else {
                mna2 = "NULL";
            }
            mangledName = "(" + mna1 + name + mna2 + ")";
        }
        virtual void log ( TextWriter & ss, const AnnotationDeclaration & ) const override  {
            if ( !left || !right ) return;
            ss << "(";
            if ( left->annotation ) {
                left->annotation->log(ss, *left);
            } else {
                ss << "NULL";
            }
            ss << " " << name << " ";
            if ( right->annotation ) {
                right->annotation->log(ss, *right);
            } else {
                ss << "NULL";
            }
            ss << ")";
        }
        virtual void serialize ( AstSerializer & ser ) override {
            ser << left << right;
        }
        virtual void gc_collect ( gc_root * target, gc_root * from ) override {
            Annotation::gc_collect(target, from);
            if ( left ) left->gc_collect(target, from);
            if ( right ) right->gc_collect(target, from);
        }
        AnnotationDeclarationPtr left = nullptr, right = nullptr;
    };

    struct LogicOpAndAnnotation : LogicOp2Annotation {
        LogicOpAndAnnotation ( ) : LogicOp2Annotation("&&") { }
        LogicOpAndAnnotation ( const AnnotationDeclarationPtr & arg0, const AnnotationDeclarationPtr & arg1 )
            : LogicOp2Annotation("&&",arg0,arg1) { }
        virtual bool isCompatible ( const FunctionPtr & fun, const vector<TypeDeclPtr> & types, const AnnotationDeclaration &, string & errors  ) const override  {
            if ( !left || !right ) return false;
            return ((FunctionAnnotation *)(left->annotation))->isCompatible(fun, types, *left, errors) &&
                ((FunctionAnnotation *)(right->annotation))->isCompatible(fun, types, *right, errors);
        }
    };

    struct LogicOpOrAnnotation : LogicOp2Annotation {
        LogicOpOrAnnotation ( ) : LogicOp2Annotation("||") { }
        LogicOpOrAnnotation ( const AnnotationDeclarationPtr & arg0, const AnnotationDeclarationPtr & arg1 )
            : LogicOp2Annotation("||",arg0,arg1) { }
        virtual bool isCompatible ( const FunctionPtr & fun, const vector<TypeDeclPtr> & types, const AnnotationDeclaration &, string & errors  ) const override  {
            if ( !left || !right ) return false;
            return ((FunctionAnnotation *)(left->annotation))->isCompatible(fun, types, *left, errors) ||
                ((FunctionAnnotation *)(right->annotation))->isCompatible(fun, types, *right, errors);
        }
    };

    struct LogicOpXOrAnnotation : LogicOp2Annotation {
        LogicOpXOrAnnotation ( ) : LogicOp2Annotation("^^") { }
        LogicOpXOrAnnotation ( const AnnotationDeclarationPtr & arg0, const AnnotationDeclarationPtr & arg1 )
            : LogicOp2Annotation("^^",arg0,arg1) { }
        virtual bool isCompatible ( const FunctionPtr & fun, const vector<TypeDeclPtr> & types, const AnnotationDeclaration &, string & errors  ) const override {
            if ( !left || !right ) return false;
            return ((FunctionAnnotation *)(left->annotation))->isCompatible(fun, types, *left, errors) !=
                ((FunctionAnnotation *)(right->annotation))->isCompatible(fun, types, *right, errors);
        }
    };

    AnnotationPtr newLogicAnnotation ( LogicAnnotationOp op, const AnnotationDeclarationPtr & arg0, const AnnotationDeclarationPtr & arg1 ) {
        switch ( op ) {
        case LogicAnnotationOp::Not:    return new LogicOpNotAnnotation(arg0);
        case LogicAnnotationOp::And:    return new LogicOpAndAnnotation(arg0,arg1);
        case LogicAnnotationOp::Or:     return new LogicOpOrAnnotation(arg0,arg1);
        case LogicAnnotationOp::Xor:    return new LogicOpXOrAnnotation(arg0,arg1);
        }
        return nullptr;
    }

    AnnotationPtr newLogicAnnotation ( LogicAnnotationOp op ) {
        switch ( op ) {
        case LogicAnnotationOp::Not:    return new LogicOpNotAnnotation();
        case LogicAnnotationOp::And:    return new LogicOpAndAnnotation();
        case LogicAnnotationOp::Or:     return new LogicOpOrAnnotation();
        case LogicAnnotationOp::Xor:    return new LogicOpXOrAnnotation();
        }
        return nullptr;
    }

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
}
