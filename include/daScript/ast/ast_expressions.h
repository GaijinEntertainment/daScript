#pragma once

#include "daScript/ast/ast.h"

namespace das
{
    struct ExprClone;

    struct ExprReader : Expression {
        ExprReader () { setRtti(DAS_HASH_TAG("ExprReader")); }
        ExprReader ( const LineInfo & a, const ReaderMacroPtr & rm )
            : Expression(a), macro(rm) { setRtti(DAS_HASH_TAG("ExprReader")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ReaderMacroPtr macro = nullptr;
        string sequence;
    };

    struct ExprLabel : Expression {
        ExprLabel () { setRtti(DAS_HASH_TAG("ExprLabel")); };
        ExprLabel ( const LineInfo & a, int32_t s, const string & cm = string() )
            : Expression(a), label(s), comment(cm) { setRtti(DAS_HASH_TAG("ExprLabel")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isLabel() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        int32_t  label = -1;
        string   comment;
    };

    struct ExprGoto : Expression {
        ExprGoto () { setRtti(DAS_HASH_TAG("ExprGoto")); };
        ExprGoto ( const LineInfo & a, int32_t s )
            : Expression(a), label(s) { setRtti(DAS_HASH_TAG("ExprGoto")); }
        ExprGoto ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti(DAS_HASH_TAG("ExprGoto")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isGoto() const override { return true; }
        virtual uint32_t getEvalFlags() const override { return EvalFlags::jumpToLabel; }
        virtual void serialize( AstSerializer & ser ) override;
        int32_t  label = -1;
        ExpressionPtr   subexpr;
    };

    struct ExprRef2Value : Expression {
        ExprRef2Value() { setRtti(DAS_HASH_TAG("ExprRef2Value")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * GetR2V ( Context & context, const LineInfo & a, const TypeDeclPtr & type, SimNode * expr );
        virtual bool rtti_isR2V() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr;
    };

    struct ExprRef2Ptr : Expression {
        ExprRef2Ptr () { setRtti(DAS_HASH_TAG("ExprRef2Ptr")); };
        ExprRef2Ptr ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti(DAS_HASH_TAG("ExprRef2Ptr")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isRef2Ptr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr;
    };

    struct ExprPtr2Ref : Expression {
        ExprPtr2Ref () { setRtti(DAS_HASH_TAG("ExprPtr2Ref")); };
        ExprPtr2Ref ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti(DAS_HASH_TAG("ExprPtr2Ref")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isPtr2Ref() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        bool unsafeDeref = false;
        bool assumeNoAlias = false;
    };

    struct ExprAddr : Expression {
        ExprAddr ()  { setRtti(DAS_HASH_TAG("ExprAddr")); };
        ExprAddr ( const LineInfo & a, const string & n )
            : Expression(a), target(n) { setRtti(DAS_HASH_TAG("ExprAddr")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAddr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string target;
        TypeDeclPtr funcType;
        Function * func = nullptr;
    };

    struct ExprNullCoalescing : ExprPtr2Ref {
        ExprNullCoalescing () { setRtti(DAS_HASH_TAG("ExprNullCoalescing")); };
        ExprNullCoalescing ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & defVal )
            : ExprPtr2Ref(a,s), defaultValue(defVal) { setRtti(DAS_HASH_TAG("ExprNullCoalescing")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isNullCoalescing() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   defaultValue;
    };

    struct ExprDelete : Expression {
        ExprDelete() { setRtti(DAS_HASH_TAG("ExprDelete")); }
        ExprDelete ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti(DAS_HASH_TAG("ExprDelete")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr subexpr;
        ExpressionPtr sizeexpr;
        bool native = false;
    };

    struct ExprAt : Expression {
        ExprAt() { setRtti(DAS_HASH_TAG("ExprAt")); };
        ExprAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i, bool no_promo = false )
            : Expression(a), subexpr(s), index(i) { setRtti(DAS_HASH_TAG("ExprAt")); no_promotion = no_promo; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAt() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr, index;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
                bool        no_promotion : 1;
            };
            uint32_t atFlags = 0;
        };
    };

    struct ExprSafeAt : ExprAt {
        ExprSafeAt() { setRtti(DAS_HASH_TAG("ExprSafeAt")); };
        ExprSafeAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i, bool no_promo=false )
            : ExprAt(a,s,i,no_promo) { setRtti(DAS_HASH_TAG("ExprSafeAt")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAt() const override { return false; }
        virtual bool rtti_isSafeAt() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
    };


    struct ExprBlock : Expression {
        ExprBlock() { setRtti(DAS_HASH_TAG("ExprBlock")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        uint32_t getFinallyEvalFlags() const;
        virtual ExpressionPtr visit(Visitor & vis) override;
        void visitFinally(Visitor & vis);
        virtual bool rtti_isBlock() const override { return true; }
        VariablePtr findArgument(const string & name);
        vector<SimNode *> collectExpressions ( Context & context,
                const vector<ExpressionPtr> & list, das_map<int32_t,uint32_t> * ofsmap = nullptr ) const;
        void simulateFinal ( Context & context, SimNode_Final * sim ) const;
        void simulateBlock ( Context & context, SimNode_Block * sim ) const;
        void simulateLabels ( Context & context, SimNode_Block * sim, const das_map<int32_t,uint32_t> & ofsmap ) const;
        string getMangledName(bool includeName = false, bool includeResult = false) const;
        bool collapse();
        static void collapse ( vector<ExpressionPtr> & res, const vector<ExpressionPtr> & lst );
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr makeBlockType () const;
        vector<ExpressionPtr>   list;
        vector<ExpressionPtr>   finalList;
        TypeDeclPtr             returnType;
        vector<VariablePtr>     arguments;
        uint32_t                stackTop = 0;
        uint32_t                stackVarTop = 0;
        uint32_t                stackVarBottom = 0;
        vector<pair<uint32_t,uint32_t>> stackCleanVars;
        int32_t                 maxLabelIndex = -1;
        AnnotationList          annotations;
        uint64_t                annotationData = 0;         // to be filled with annotation
        uint64_t                annotationDataSid = 0;      // to be filled with annotation
        union {
            struct {
                bool            isClosure : 1;
                bool            hasReturn : 1;
                bool            copyOnReturn : 1;
                bool            moveOnReturn : 1;
                bool            inTheLoop : 1;
                bool            finallyBeforeBody : 1;
                bool            finallyDisabled : 1;
                bool            aotSkipMakeBlock : 1;
                bool            aotDoNotSkipAnnotationData : 1;
                bool            isCollapseable : 1;         // generated block which needs to be flattened
                bool            needCollapse : 1;           // if this block needs collapse at all
                bool            hasMakeBlock : 1;           // if this block has make block inside
                bool            hasEarlyOut : 1;            // this block has return, or other blocks with return
                bool            forLoop : 1;                // this block is a for loop
            };
            uint32_t            blockFlags = 0;
        };
        Function *              inFunction = nullptr;       // moving this to the last position of a class
                                                            // is a workaround of a compiler bug in 32-bit MVSC 2015
    };

    struct ExprOp2;

    struct ExprVar : Expression {
        ExprVar () { setRtti(DAS_HASH_TAG("ExprVar")); };
        ExprVar ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti(DAS_HASH_TAG("ExprVar")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isVar() const override { return true; }
        bool isGlobalVariable() const { return !local && !argument && !block; }
        virtual void serialize( AstSerializer & ser ) override;
        string              name;
        VariablePtr         variable;
        ExprBlock *         pBlock = nullptr;
        int                 argumentIndex = -1;
        union {
            struct {
                bool        local : 1;
                bool        argument : 1;
                bool        block : 1;
                bool        thisBlock : 1;
                bool        r2v  : 1;       // built-in ref2value   (read-only)
                bool        r2cr : 1;       // built-in ref2contref (read-only, but stay ref)
                bool        write : 1;
                bool        underClone : 1; // this variable is [var] := expr or [var] = expr
            };
            uint32_t varFlags = 0;
        };
    };

    struct ExprTag : Expression {
        ExprTag () { setRtti(DAS_HASH_TAG("ExprTag")); }
        ExprTag ( const LineInfo & a, const ExpressionPtr & se, const string & n )
            : Expression(a), subexpr(se), name(n) { setRtti(DAS_HASH_TAG("ExprTag")); }
        ExprTag ( const LineInfo & a, const ExpressionPtr & se, const ExpressionPtr & va, const string & n )
            : Expression(a), subexpr(se), value(va), name(n) { setRtti(DAS_HASH_TAG("ExprTag")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        ExpressionPtr   value;
        string          name;
    };

    struct ExprClone;

    struct ExprField : Expression {
        ExprField () { setRtti(DAS_HASH_TAG("ExprField")); };
        ExprField ( const LineInfo & a, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : Expression(a), value(val), name(n), atField(a) { setRtti(DAS_HASH_TAG("ExprField")); no_promotion = no_promo; }
        ExprField ( const LineInfo & a, const LineInfo & af, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : Expression(a), value(val), name(n), atField(af) { setRtti(DAS_HASH_TAG("ExprField")); no_promotion = no_promo; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   value;
        string          name;
        LineInfo        atField;
        const Structure::FieldDeclaration * field = nullptr;
        int             fieldIndex = -1;
        TypeAnnotationPtr annotation;
        union {
            struct {
                bool        unsafeDeref : 1;
                bool        ignoreCaptureConst : 1;
            };
            uint32_t derefFlags = 0;
        };
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
                bool        no_promotion : 1;
                bool        underClone : 1; // this field is [foo.field] := expr or [fool.field] = expr
            };
            uint32_t fieldFlags = 0;
        };
    };

    struct ExprIsVariant : ExprField {
        ExprIsVariant () { setRtti(DAS_HASH_TAG("ExprIsVariant")); };
        ExprIsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti(DAS_HASH_TAG("ExprIsVariant")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isIsVariant() const override { return true; }
    };

    struct ExprAsVariant : ExprField {
        ExprAsVariant () { setRtti(DAS_HASH_TAG("ExprAsVariant")); };
        ExprAsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti(DAS_HASH_TAG("ExprAsVariant")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isAsVariant() const override { return true; }
    };

    struct ExprSafeAsVariant : ExprField {
        ExprSafeAsVariant () { setRtti(DAS_HASH_TAG("ExprSafeAsVariant")); };
        ExprSafeAsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti(DAS_HASH_TAG("ExprSafeAsVariant")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isSafeAsVariant() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        bool skipQQ = false;
    };

    struct ExprSwizzle : Expression {
        ExprSwizzle () { setRtti(DAS_HASH_TAG("ExprSwizzle")); };
        ExprSwizzle ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), mask(n) { setRtti(DAS_HASH_TAG("ExprSwizzle")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isSwizzle() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   value;
        string          mask;
        vector<uint8_t> fields;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t fieldFlags = 0;
        };
    };

    struct ExprSafeField : ExprField {
        ExprSafeField () { setRtti(DAS_HASH_TAG("ExprSafeField")); };
        ExprSafeField ( const LineInfo & a, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : ExprField(a,val,n,no_promo) { setRtti(DAS_HASH_TAG("ExprSafeField")); }
        ExprSafeField ( const LineInfo & a, const LineInfo & af, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : ExprField(a,af,val,n,no_promo) { setRtti(DAS_HASH_TAG("ExprSafeField")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isSafeField() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        bool skipQQ = false;
    };

    struct ExprLooksLikeCall : Expression {
        ExprLooksLikeCall () { setRtti(DAS_HASH_TAG("ExprLooksLikeCall")); };
        ExprLooksLikeCall ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti(DAS_HASH_TAG("ExprLooksLikeCall")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
        virtual SimNode * simulate (Context &) const override { return nullptr; }
        SimNode * keepAlive ( Context &, SimNode * result ) const;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual string describe() const;
        virtual bool rtti_isCallLikeExpr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string                  name;
        vector<ExpressionPtr>   arguments;
        bool                    argumentsFailedToInfer = false;
        TypeDeclPtr             aliasSubstitution;  // only used during infer
        LineInfo                atEnclosure;
    };

    struct ExprCallMacro : ExprLooksLikeCall {
        ExprCallMacro () { setRtti(DAS_HASH_TAG("ExprCallMacro")); name="__call_macro__"; };
        ExprCallMacro ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti(DAS_HASH_TAG("ExprCallMacro")); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context &) const override { return nullptr; }
        virtual void serialize( AstSerializer & ser ) override;
        Function * inFunction = nullptr;
        CallMacro * macro = nullptr;
    };

    struct ExprCallFunc : ExprLooksLikeCall {
        ExprCallFunc () { setRtti(DAS_HASH_TAG("ExprCallFunc")); };
        ExprCallFunc ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti(DAS_HASH_TAG("ExprCallFunc")); }
        virtual bool rtti_isCallFunc() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        Function *      func = nullptr;
        uint32_t        stackTop = 0;
        bool            genericFunction = false; // do not clone, do not serialize. used only for infer
    };

    struct ExprOp : ExprCallFunc {
        ExprOp () { setRtti(DAS_HASH_TAG("ExprOp")); };
        ExprOp ( const LineInfo & a, const string & o )
            : ExprCallFunc(a,o), op(o) { setRtti(DAS_HASH_TAG("ExprOp")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void serialize( AstSerializer & ser ) override;
        string  op;
    };

    // unary    !subexpr
    struct ExprOp1 : ExprOp {
        ExprOp1 () { setRtti(DAS_HASH_TAG("ExprOp1")); };
        ExprOp1 ( const LineInfo & a, const string & o, const ExpressionPtr & s )
            : ExprOp(a,o), subexpr(s) { setRtti(DAS_HASH_TAG("ExprOp1")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return subexpr->tail(); }
        virtual bool swap_tail ( Expression * expr, Expression * swapExpr ) override;
        virtual bool rtti_isOp1() const override { return true; }
        virtual string describe() const override;
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr;
    };

    // binary   left < right
    struct ExprOp2 : ExprOp {
        ExprOp2 () { setRtti(DAS_HASH_TAG("ExprOp2")); };
        ExprOp2 ( const LineInfo & a, const string & o, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), left(l), right(r) { setRtti(DAS_HASH_TAG("ExprOp2")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return right->tail(); }
        virtual bool swap_tail ( Expression * expr, Expression * swapExpr ) override;
        virtual bool rtti_isOp2() const override { return true; }
        virtual string describe() const override;
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   left, right;
    };

    // this copies one object to the other
    struct ExprCopy : ExprOp2 {
        ExprCopy () { setRtti(DAS_HASH_TAG("ExprCopy")); };
        ExprCopy ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "=", l, r) { setRtti(DAS_HASH_TAG("ExprCopy")); };
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        virtual bool rtti_isCopy() const override { return true; }
        union {
            struct {
                bool allowCopyTemp : 1;
                bool takeOverRightStack : 1;
            };
            uint32_t copyFlags = 0;
        };
    };

    // this moves one object to the other
    struct ExprMove : ExprOp2 {
        ExprMove () { setRtti(DAS_HASH_TAG("ExprMove")); };
        ExprMove ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "<-", l, r) { setRtti(DAS_HASH_TAG("ExprMove")); };
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        union {
            struct {
                bool skipLockCheck : 1;
                bool takeOverRightStack : 1;
            };
            uint32_t moveFlags = 0;
        };
    };

    // this clones one object to the other
    struct ExprClone : ExprOp2 {
        ExprClone () { setRtti(DAS_HASH_TAG("ExprClone")); };
        ExprClone ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, ":=", l, r) { setRtti(DAS_HASH_TAG("ExprClone")); };
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        virtual bool rtti_isClone() const override { return true; }
    };

    // this only exists during parsing, and can't be
    // and this is why it does not have CLONE
    struct ExprSequence : ExprOp2 {
        ExprSequence ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, ",", l, r) { setRtti(DAS_HASH_TAG("ExprSequence")); }
        virtual bool rtti_isSequence() const override { return true; }
    };

    // trinary  subexpr ? left : right
    struct ExprOp3 : ExprOp {
        ExprOp3 () { setRtti(DAS_HASH_TAG("ExprOp3")); };
        ExprOp3 ( const LineInfo & a, const string & o, const ExpressionPtr & s,
                 const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) { setRtti(DAS_HASH_TAG("ExprOp3")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return right->tail(); }
        virtual bool swap_tail ( Expression * expr, Expression * swapExpr ) override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isOp3() const override { return true; }
        virtual string describe() const override;
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr, left, right;
    };

    struct ExprTryCatch : Expression {
        ExprTryCatch() { setRtti(DAS_HASH_TAG("ExprTryCatch")); };
        ExprTryCatch ( const LineInfo & a, const ExpressionPtr & t, const ExpressionPtr & c )
            : Expression(a), try_block(t), catch_block(c) { setRtti(DAS_HASH_TAG("ExprTryCatch")); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr try_block, catch_block;
    };

    struct ExprReturn : Expression {
        ExprReturn() { setRtti(DAS_HASH_TAG("ExprReturn")); };
        ExprReturn ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti(DAS_HASH_TAG("ExprReturn")); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override {
            uint32_t ef = EvalFlags::stopForReturn;
            if ( fromYield ) ef |= EvalFlags::yield;
            return ef;
        }
        virtual bool rtti_isReturn() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr subexpr;
        union {
            struct {
                bool moveSemantics      : 1;
                bool returnReference    : 1;
                bool returnInBlock      : 1;
                bool takeOverRightStack : 1;
                bool returnCallCMRES    : 1;
                bool returnCMRES        : 1;
                bool fromYield          : 1;
                bool fromComprehension  : 1;
                bool skipLockCheck      : 1;
            };
            uint32_t    returnFlags = 0;
        };
        uint32_t                stackTop = 0;
        uint32_t                refStackTop = 0;
        Function *              returnFunc = nullptr;
        ExprBlock *             block = nullptr;
        TypeDeclPtr             returnType;
    };

    struct ExprBreak : Expression {
        ExprBreak() { setRtti(DAS_HASH_TAG("ExprBreak"));} ;
        ExprBreak ( const LineInfo & a )
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprBreak")); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForBreak; }
        virtual bool rtti_isBreak() const override { return true; }
    };

    struct ExprContinue : Expression {
        ExprContinue() { setRtti(DAS_HASH_TAG("ExprContinue")); };
        ExprContinue ( const LineInfo & a )
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprContinue")); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForContinue; }
        virtual bool rtti_isContinue() const override { return true; }
    };

    struct ExprFakeContext : ExprConstT<void *, ExprFakeContext> {
        ExprFakeContext(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeContext) { setRtti(DAS_HASH_TAG("ExprFakeContext")); }
        ExprFakeContext(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeContext) { setRtti(DAS_HASH_TAG("ExprFakeContext")); }
        virtual bool rtti_isFakeContext() const override { return true; }
    };

    struct ExprFakeLineInfo : ExprConstT<void *, ExprFakeLineInfo> {
        ExprFakeLineInfo(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeLineInfo) { setRtti(DAS_HASH_TAG("ExprFakeLineInfo")); }
        ExprFakeLineInfo(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeLineInfo) { setRtti(DAS_HASH_TAG("ExprFakeLineInfo")); }
        virtual bool rtti_isFakeLineInfo() const override { return true; }
    };

    struct ExprConstPtr : ExprConstT<void *,ExprConstPtr> {
        ExprConstPtr(void * ptr = nullptr)
            : ExprConstT(ptr,Type::tPointer) { setRtti(DAS_HASH_TAG("ExprConstPtr")); }
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr)
            : ExprConstT(a,ptr,Type::tPointer) { setRtti(DAS_HASH_TAG("ExprConstPtr")); }
        bool isSmartPtr = false;
        TypeDeclPtr ptrType;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual bool rtti_isNullPtr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
    };

    struct ExprConstInt : ExprConstT<int32_t,ExprConstInt> {
        ExprConstInt(int32_t i = 0)
            : ExprConstT(i,Type::tInt) { setRtti(DAS_HASH_TAG("ExprConstInt"));}
        ExprConstInt(const LineInfo & a, int32_t i = 0)
            : ExprConstT(a,i,Type::tInt) { setRtti(DAS_HASH_TAG("ExprConstInt")); }
    };

    struct ExprConstEnumeration : ExprConst {
        ExprConstEnumeration(int val, const TypeDeclPtr & td)
            : ExprConst(Type::tEnumeration) {
            setRtti(DAS_HASH_TAG("ExprConstEnumeration"));
            enumType = td->enumType;
            text = td->enumType->find(int64_t(val),"");
        }
        ExprConstEnumeration(const string & name = string(), const TypeDeclPtr & td = nullptr)
            : ExprConst(Type::tEnumeration), text(name) {
            setRtti(DAS_HASH_TAG("ExprConstEnumeration"));
            if ( td ) enumType = td->enumType;
        }
        ExprConstEnumeration(const LineInfo & a, const string & name, const TypeDeclPtr & td)
            : ExprConst(a,Type::tEnumeration), text(name) {
            setRtti(DAS_HASH_TAG("ExprConstEnumeration"));
            enumType = td->enumType;
        }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        EnumerationPtr  enumType;
        string          text;
    };

    struct ExprConstInt8 : ExprConstT<int8_t,ExprConstInt8> {
        ExprConstInt8(int8_t i = 0)
            : ExprConstT(i,Type::tInt8) { setRtti(DAS_HASH_TAG("ExprConstInt8")); }
        ExprConstInt8(const LineInfo & a, int8_t i = 0)
            : ExprConstT(a,i,Type::tInt8) { setRtti(DAS_HASH_TAG("ExprConstInt8")); }
    };

    struct ExprConstInt16 : ExprConstT<int16_t,ExprConstInt16> {
        ExprConstInt16(int16_t i = 0)
            : ExprConstT(i,Type::tInt16) { setRtti(DAS_HASH_TAG("ExprConstInt16")); }
        ExprConstInt16(const LineInfo & a, int16_t i = 0)
            : ExprConstT(a,i,Type::tInt16) { setRtti(DAS_HASH_TAG("ExprConstInt16")); }
    };

    struct ExprConstInt64 : ExprConstT<int64_t,ExprConstInt64> {
        ExprConstInt64(int64_t i = 0)
            : ExprConstT(i,Type::tInt64) { setRtti(DAS_HASH_TAG("ExprConstInt64")); }
        ExprConstInt64(const LineInfo & a, int64_t i = 0)
            : ExprConstT(a,i,Type::tInt64) { setRtti(DAS_HASH_TAG("ExprConstInt64")); }
    };

    struct ExprConstBitfield : ExprConstT<uint32_t,ExprConstBitfield> {
        ExprConstBitfield(uint32_t i = 0)
            : ExprConstT(i,Type::tBitfield) { setRtti(DAS_HASH_TAG("ExprConstBitfield")); }
        ExprConstBitfield(const LineInfo & a, uint32_t i = 0)
            : ExprConstT(a,i,Type::tBitfield) { setRtti(DAS_HASH_TAG("ExprConstBitfield")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        TypeDeclPtr bitfieldType;
    };

    struct ExprConstInt2 : ExprConstT<int2,ExprConstInt2> {
        ExprConstInt2(int2 i = int2())
            : ExprConstT(i,Type::tInt2) { setRtti(DAS_HASH_TAG("ExprConstInt2")); }
        ExprConstInt2(const LineInfo & a, int2 i)
            : ExprConstT(a,i,Type::tInt2) { setRtti(DAS_HASH_TAG("ExprConstInt2")); }
    };

    struct ExprConstRange : ExprConstT<range,ExprConstRange> {
        ExprConstRange(range i = range())
            : ExprConstT(i,Type::tRange) { setRtti(DAS_HASH_TAG("ExprConstRange")); }
        ExprConstRange(const LineInfo & a, range i)
            : ExprConstT(a,i,Type::tRange) { setRtti(DAS_HASH_TAG("ExprConstRange")); }
    };

    struct ExprConstRange64 : ExprConstT<range64,ExprConstRange64> {
        ExprConstRange64(range64 i = range64())
            : ExprConstT(i,Type::tRange64) { setRtti(DAS_HASH_TAG("ExprConstRange64")); }
        ExprConstRange64(const LineInfo & a, range64 i)
            : ExprConstT(a,i,Type::tRange64) { setRtti(DAS_HASH_TAG("ExprConstRange64")); }
    };


    struct ExprConstInt3 : ExprConstT<int3,ExprConstInt3> {
        ExprConstInt3(int3 i = int3())
            : ExprConstT(i,Type::tInt3) { setRtti(DAS_HASH_TAG("ExprConstInt3")); }
        ExprConstInt3(const LineInfo & a, int3 i)
            : ExprConstT(a,i,Type::tInt3) { setRtti(DAS_HASH_TAG("ExprConstInt3")); }
    };

    struct ExprConstInt4 : ExprConstT<int4,ExprConstInt4> {
        ExprConstInt4(int4 i = int4())
            : ExprConstT(i,Type::tInt4) { setRtti(DAS_HASH_TAG("ExprConstInt4")); }
        ExprConstInt4(const LineInfo & a, int4 i)
            : ExprConstT(a,i,Type::tInt4) { setRtti(DAS_HASH_TAG("ExprConstInt4")); }
    };

    struct ExprConstUInt8 : ExprConstT<uint8_t,ExprConstUInt8> {
        ExprConstUInt8(uint8_t i = 0)
            : ExprConstT(i,Type::tUInt8) { setRtti(DAS_HASH_TAG("ExprConstUInt8")); }
        ExprConstUInt8(const LineInfo & a, uint8_t i = 0)
            : ExprConstT(a,i,Type::tUInt8) { setRtti(DAS_HASH_TAG("ExprConstUInt8")); }
    };

    struct ExprConstUInt16 : ExprConstT<uint16_t,ExprConstUInt16> {
        ExprConstUInt16(uint16_t i = 0)
            : ExprConstT(i,Type::tUInt16) { setRtti(DAS_HASH_TAG("ExprConstUInt16")); }
        ExprConstUInt16(const LineInfo & a, uint16_t i = 0)
            : ExprConstT(a,i,Type::tUInt16) { setRtti(DAS_HASH_TAG("ExprConstUInt16")); }
    };

    struct ExprConstUInt64 : ExprConstT<uint64_t,ExprConstUInt64> {
        ExprConstUInt64(uint64_t i = 0)
            : ExprConstT(i,Type::tUInt64) { setRtti(DAS_HASH_TAG("ExprConstUInt64")); }
        ExprConstUInt64(const LineInfo & a, uint64_t i = 0)
            : ExprConstT(a,i,Type::tUInt64) { setRtti(DAS_HASH_TAG("ExprConstUInt64")); }
    };

    struct ExprConstUInt : ExprConstT<uint32_t,ExprConstUInt> {
        ExprConstUInt(uint32_t i = 0)
            : ExprConstT(i,Type::tUInt) { setRtti(DAS_HASH_TAG("ExprConstUInt")); }
        ExprConstUInt(const LineInfo & a, uint32_t i = 0)
            : ExprConstT(a,i,Type::tUInt) { setRtti(DAS_HASH_TAG("ExprConstUInt")); }
    };

    int64_t getConstExprIntOrUInt ( const ExpressionPtr & expr );

    struct ExprConstUInt2 : ExprConstT<uint2,ExprConstUInt2> {
        ExprConstUInt2(uint2 i = uint2())
            : ExprConstT(i,Type::tUInt2) { setRtti(DAS_HASH_TAG("ExprConstUInt2")); }
        ExprConstUInt2(const LineInfo & a, uint2 i)
            : ExprConstT(a,i,Type::tUInt2) { setRtti(DAS_HASH_TAG("ExprConstUInt2")); }
    };

    struct ExprConstURange : ExprConstT<urange,ExprConstURange> {
        ExprConstURange(urange i = urange())
            : ExprConstT(i,Type::tURange) { setRtti(DAS_HASH_TAG("ExprConstURange")); }
        ExprConstURange(const LineInfo & a, urange i)
            : ExprConstT(a,i,Type::tURange) { setRtti(DAS_HASH_TAG("ExprConstURange")); }
    };

    struct ExprConstURange64 : ExprConstT<urange64,ExprConstURange64> {
        ExprConstURange64(urange64 i = urange64())
            : ExprConstT(i,Type::tURange64) { setRtti(DAS_HASH_TAG("ExprConstURange64")); }
        ExprConstURange64(const LineInfo & a, urange64 i)
            : ExprConstT(a,i,Type::tURange64) { setRtti(DAS_HASH_TAG("ExprConstURange64")); }
    };

    struct ExprConstUInt3 : ExprConstT<uint3,ExprConstUInt3> {
        ExprConstUInt3(uint3 i = uint3())
            : ExprConstT(i,Type::tUInt3) { setRtti(DAS_HASH_TAG("ExprConstUInt3")); }
        ExprConstUInt3(const LineInfo & a, uint3 i)
            : ExprConstT(a,i,Type::tUInt3) { setRtti(DAS_HASH_TAG("ExprConstUInt3")); }
    };

    struct ExprConstUInt4 : ExprConstT<uint4,ExprConstUInt4> {
        ExprConstUInt4(uint4 i = uint4())
            : ExprConstT(i,Type::tUInt4) { setRtti(DAS_HASH_TAG("ExprConstUInt4")); }
        ExprConstUInt4(const LineInfo & a, uint4 i)
            : ExprConstT(a,i,Type::tUInt4) { setRtti(DAS_HASH_TAG("ExprConstUInt4")); }
    };

    struct ExprConstBool : ExprConstT<bool,ExprConstBool> {
        ExprConstBool(bool i = false)
            : ExprConstT(i,Type::tBool) { setRtti(DAS_HASH_TAG("ExprConstBool")); }
        ExprConstBool(const LineInfo & a, bool i = false)
            : ExprConstT(a,i,Type::tBool) { setRtti(DAS_HASH_TAG("ExprConstBool")); }
    };

    struct ExprConstFloat : ExprConstT<float,ExprConstFloat> {
        ExprConstFloat(float i = 0.0f)
            : ExprConstT(i,Type::tFloat) { setRtti(DAS_HASH_TAG("ExprConstFloat")); }
        ExprConstFloat(int i)
            : ExprConstT(float(i),Type::tFloat) { setRtti(DAS_HASH_TAG("ExprConstFloat")); }
        ExprConstFloat(double i)
            : ExprConstT(float(i),Type::tFloat) { setRtti(DAS_HASH_TAG("ExprConstFloat")); }
        ExprConstFloat(const LineInfo & a, float i = 0.0f)
            : ExprConstT(a,i,Type::tFloat) { setRtti(DAS_HASH_TAG("ExprConstFloat")); }
    };

    struct ExprConstDouble : ExprConstT<double,ExprConstDouble> {
        ExprConstDouble(double i = 0.0)
            : ExprConstT(i,Type::tDouble) { setRtti(DAS_HASH_TAG("ExprConstDouble")); }
        ExprConstDouble(const LineInfo & a, double i = 0.0)
            : ExprConstT(a,i,Type::tDouble) { setRtti(DAS_HASH_TAG("ExprConstDouble")); }
    };

    struct ExprConstFloat2 : ExprConstT<float2,ExprConstFloat2> {
        ExprConstFloat2(float2 i = float2())
            : ExprConstT(i,Type::tFloat2) { setRtti(DAS_HASH_TAG("ExprConstFloat2")); }
        ExprConstFloat2(const LineInfo & a, float2 i)
            : ExprConstT(a,i,Type::tFloat2) { setRtti(DAS_HASH_TAG("ExprConstFloat2")); }
    };

    struct ExprConstFloat3 : ExprConstT<float3,ExprConstFloat3> {
        ExprConstFloat3(float3 i = float3())
            : ExprConstT(i,Type::tFloat3) { setRtti(DAS_HASH_TAG("ExprConstFloat3")); }
        ExprConstFloat3(const LineInfo & a, float3 i)
            : ExprConstT(a,i,Type::tFloat3) { setRtti(DAS_HASH_TAG("ExprConstFloat3")); }
    };

    struct ExprConstFloat4 : ExprConstT<float4,ExprConstFloat4> {
        ExprConstFloat4(float4 i = float4())
            : ExprConstT(i,Type::tFloat4) { setRtti(DAS_HASH_TAG("ExprConstFloat4")); }
        ExprConstFloat4(const LineInfo & a, float4 i)
            : ExprConstT(a,i,Type::tFloat4) { setRtti(DAS_HASH_TAG("ExprConstFloat4")); }
    };

    struct ExprConstString : ExprConst {
        ExprConstString(const string & str = string())
            : ExprConst(Type::tString), text(str) { setRtti(DAS_HASH_TAG("ExprConstString")); }
        ExprConstString(const LineInfo & a, const string & str = string())
            : ExprConst(a,Type::tString), text(str) { setRtti(DAS_HASH_TAG("ExprConstString")); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        const string & getValue() const { return text; }
        virtual bool rtti_isStringConstant() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string  text;
    };

    struct ExprStringBuilder : Expression {
        ExprStringBuilder() { setRtti(DAS_HASH_TAG("ExprStringBuilder"));  };
        ExprStringBuilder(const LineInfo & a)
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprStringBuilder")); }
        virtual bool rtti_isStringBuilder() const override { return true; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        vector<ExpressionPtr>   elements;
        union {
            struct {
                bool    isTempString : 1;       // this string is passed to a function, which does not capture it. it can be disposed of after the call
            };
            uint32_t    stringBuilderFlags = 0;
        };
    };

    struct ExprLet : Expression {
        ExprLet() { setRtti(DAS_HASH_TAG("ExprLet")); }
        Variable * find ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * simulateInit(Context & context, const VariablePtr & var, bool local);
        static vector<SimNode *> simulateInit(Context & context, const ExprLet * pLet);
        virtual bool rtti_isLet() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        vector<VariablePtr>     variables;
        LineInfo                visibility;
        LineInfo                atInit;
        union {
            struct {
                bool    inScope : 1;
                bool    hasEarlyOut : 1;
                bool    isTupleExpansion : 1;
            };
            uint32_t    letFlags = 0;
        };
    };

    // for a,b in foo,bar where a>b ...
    struct ExprFor : Expression {
        ExprFor () { setRtti(DAS_HASH_TAG("ExprFor")); };
        ExprFor ( const LineInfo & a )
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprFor")); }
        Variable * findIterator ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isFor() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        vector<string>          iterators;
        vector<string>          iteratorsAka;
        vector<LineInfo>        iteratorsAt;
        vector<ExpressionPtr>   iteratorsTags;
        vector<VariablePtr>     iteratorVariables;
        vector<ExpressionPtr>   sources;
        ExpressionPtr           body;
        LineInfo                visibility;
        bool                    allowIteratorOptimization = false;  // if enabled, unused source variables can be removed
        bool                    canShadow = false;                  // if enabled, local variables can shadow
    };

    struct ExprUnsafe : Expression {
        ExprUnsafe() { setRtti(DAS_HASH_TAG("ExprUnsafe")); };
        ExprUnsafe(const LineInfo & a)
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprUnsafe")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isUnsafe() const override { return true; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   body;
    };

    struct ExprWhile : Expression {
        ExprWhile() { setRtti(DAS_HASH_TAG("ExprWhile")); };
        ExprWhile(const LineInfo & a)
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprWhile")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isWhile() const override { return true; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        static void simulateFinal ( Context & context, const ExpressionPtr & bod, SimNode_Block * blk );
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   cond, body;
    };

    struct ExprWith : Expression {
        ExprWith() { setRtti(DAS_HASH_TAG("ExprWith")); };
        ExprWith(const LineInfo & a)
            : Expression(a) { setRtti(DAS_HASH_TAG("ExprWith")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isWith() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   with, body;
    };

    struct ExprAssume : Expression {
        ExprAssume() { setRtti(DAS_HASH_TAG("ExprAssume")); };
        ExprAssume(const LineInfo & a, const string & al, const ExpressionPtr & se )
            : Expression(a), alias(al), subexpr(se) { setRtti(DAS_HASH_TAG("ExprAssume")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAssume() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string          alias;
        ExpressionPtr   subexpr;
    };

    template <typename TT>
    struct ExprLikeCall : ExprLooksLikeCall {
        ExprLikeCall () { setRtti(DAS_HASH_TAG("ExprLikeCall")); };
        ExprLikeCall ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti(DAS_HASH_TAG("ExprLikeCall")); }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };

    enum class CaptureMode {
        capture_any
    ,   capture_by_copy
    ,   capture_by_reference
    ,   capture_by_clone
    ,   capture_by_move
    };

    struct CaptureEntry {
        string  name;
        CaptureMode mode = CaptureMode::capture_any;
        CaptureEntry() {}
        CaptureEntry( const string n, CaptureMode m ) : name(n), mode(m) {}
    };

    struct ExprMakeBlock : Expression {
        ExprMakeBlock () { setRtti(DAS_HASH_TAG("ExprMakeBlock")); };
        ExprMakeBlock ( const LineInfo & a, const ExpressionPtr & b, bool isl = false, bool islf = false )
            : Expression(a), block(b) {
            setRtti(DAS_HASH_TAG("ExprMakeBlock"));
            b->at = a;
            isLambda = isl;
            isLocalFunction = islf;
            static_pointer_cast<ExprBlock>(b)->isClosure = true;
        }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual bool rtti_isMakeBlock() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        vector<CaptureEntry>    capture;
        ExpressionPtr block;
        uint32_t stackTop = 0;
        union {
            struct {
                bool isLambda : 1;
                bool isLocalFunction : 1;
            };
            uint32_t mmFlags = 0;
        };
        string aotFunctorName;
    };

    struct ExprMakeGenerator : ExprLooksLikeCall {
        ExprMakeGenerator () { setRtti(DAS_HASH_TAG("ExprMakeGenerator")); };
        ExprMakeGenerator ( const LineInfo & a, const ExpressionPtr & b = nullptr );
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr iterType;
        vector<CaptureEntry> capture;
    };

    struct ExprYield : Expression {
        ExprYield () { setRtti(DAS_HASH_TAG("ExprYield")); };
        ExprYield ( const LineInfo & a, const ExpressionPtr & val );
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::yield; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr subexpr;
        union {
            struct {
                bool moveSemantics      : 1;
                bool skipLockCheck      : 1;
            };
            uint32_t    returnFlags = 0;
        };
    };

    struct ExprInvoke : ExprLikeCall<ExprInvoke> {
        ExprInvoke () { setRtti(DAS_HASH_TAG("ExprInvoke")); name = "invoke"; };
        ExprInvoke ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprInvoke>(a,name) { setRtti(DAS_HASH_TAG("ExprInvoke")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isInvoke() const override { return true; }
        bool isCopyOrMove() const;
        __forceinline bool allowCmresSkip() const { return !cmresAlias && isCopyOrMove(); }
        virtual void serialize( AstSerializer & ser ) override;
        uint32_t    stackTop = 0;
        bool        doesNotNeedSp = false;
        bool        isInvokeMethod = false;
        bool        cmresAlias = false;
    };

    struct ExprAssert : ExprLikeCall<ExprAssert> {
        ExprAssert ( ) { setRtti(DAS_HASH_TAG("ExprAssert")); name="assert"; };
        ExprAssert ( const LineInfo & a, const string & name, bool isV )
            : ExprLikeCall<ExprAssert>(a,name) { isVerify = isV; setRtti(DAS_HASH_TAG("ExprAssert")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        bool isVerify = false;
    };

    struct ExprQuote : ExprLikeCall<ExprQuote> {
        ExprQuote ( ) { setRtti(DAS_HASH_TAG("ExprQuote")); name="quote"; };
        ExprQuote ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprQuote>(a,name) { setRtti(DAS_HASH_TAG("ExprQuote")); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
    };

    struct ExprStaticAssert : ExprLikeCall<ExprStaticAssert> {
        ExprStaticAssert () { setRtti(DAS_HASH_TAG("ExprStaticAssert")); name="static_assert"; };
        ExprStaticAssert ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprStaticAssert>(a,name) { setRtti(DAS_HASH_TAG("ExprStaticAssert")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprDebug : ExprLikeCall<ExprDebug> {
        ExprDebug () { setRtti(DAS_HASH_TAG("ExprDebug")); name="debug"; };
        ExprDebug ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprDebug>(a, name) { setRtti(DAS_HASH_TAG("ExprDebug")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprMemZero : ExprLikeCall<ExprMemZero> {
        ExprMemZero () { setRtti(DAS_HASH_TAG("ExprMemZero")); name="memzero"; };
        ExprMemZero ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprMemZero>(a, name) { setRtti(DAS_HASH_TAG("ExprMemZero")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    template <typename It, typename SimNodeT, bool first>
    struct ExprTableKeysOrValues : ExprLooksLikeCall {
        ExprTableKeysOrValues() { setRtti(DAS_HASH_TAG("ExprTableKeysOrValues")); };
        ExprTableKeysOrValues ( const LineInfo & a, const string & name )
            : ExprLooksLikeCall(a, name) { setRtti(DAS_HASH_TAG("ExprTableKeysOrValues")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprTableKeysOrValues<It,SimNodeT,first>>(expr);
            ExprLooksLikeCall::clone(cexpr);
            return cexpr;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto subexpr = arguments[0]->simulate(context);
            auto tableType = arguments[0]->type;
            auto iterType = first ? tableType->firstType : tableType->secondType;
            auto stride = iterType->getSizeOf();
            return context.code->makeNode<SimNodeT>(at,subexpr,stride);
        }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
        virtual void serialize( AstSerializer & ser ) override;
    };

    template <typename It, typename SimNodeT>
    struct ExprArrayCallWithSizeOrIndex : ExprLooksLikeCall {
        ExprArrayCallWithSizeOrIndex() { setRtti(DAS_HASH_TAG("ExprArrayCallWithSizeOrIndex")); };
        ExprArrayCallWithSizeOrIndex ( const LineInfo & a, const string & name )
            : ExprLooksLikeCall(a, name) { setRtti(DAS_HASH_TAG("ExprArrayCallWithSizeOrIndex")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override {
            auto cexpr = clonePtr<ExprArrayCallWithSizeOrIndex<It,SimNodeT>>(expr);
            ExprLooksLikeCall::clone(cexpr);
            return cexpr;
        }
        virtual SimNode * simulate (Context & context) const override {
            auto arr = arguments[0]->simulate(context);
            auto newSize = arguments[1]->simulate(context);
            auto size = arguments[0]->type->firstType->getSizeOf();
            return context.code->makeNode<SimNodeT>(at,arr,newSize,size);
        }
        virtual ExpressionPtr visit ( Visitor & vis ) override;
        virtual void serialize( AstSerializer & ser ) override;
    };

    struct ExprErase : ExprLikeCall<ExprErase> {
        ExprErase() { setRtti(DAS_HASH_TAG("ExprErase")); name="erase"; };
        ExprErase ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprErase>(a, "erase") { setRtti(DAS_HASH_TAG("ExprErase")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprFind : ExprLikeCall<ExprFind> {
        ExprFind() { setRtti(DAS_HASH_TAG("ExprFind")); name="find"; };
        ExprFind ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprFind>(a, "find") { setRtti(DAS_HASH_TAG("ExprFind")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprKeyExists : ExprLikeCall<ExprKeyExists> {
        ExprKeyExists() { setRtti(DAS_HASH_TAG("ExprKeyExists")); name="key_exists"; };
        ExprKeyExists ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprKeyExists>(a, "key_exists") { setRtti(DAS_HASH_TAG("ExprKeyExists")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprSetInsert : ExprLikeCall<ExprSetInsert> {
        ExprSetInsert() { setRtti(DAS_HASH_TAG("ExprSetInsert")); name="insert"; };
        ExprSetInsert ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprSetInsert>(a, "insert") { setRtti(DAS_HASH_TAG("ExprSetInsert")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprTypeInfo : Expression {
        ExprTypeInfo () { setRtti(DAS_HASH_TAG("ExprTypeInfo")); }
        ExprTypeInfo ( const LineInfo & a, const string & tr, const ExpressionPtr & s,
                      const string & stt="", const string & ett="" )
            : Expression(a), trait(tr), subexpr(s), subtrait(stt), extratrait(ett) { setRtti(DAS_HASH_TAG("ExprTypeInfo")); }
        ExprTypeInfo ( const LineInfo & a, const string & tr, const TypeDeclPtr & d,
                      const string & stt="", const string & ett="" )
            : Expression(a), trait(tr), typeexpr(d), subtrait(stt), extratrait(ett) { setRtti(DAS_HASH_TAG("ExprTypeInfo")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        string              trait;
        ExpressionPtr       subexpr;
        TypeDeclPtr         typeexpr;
        string              subtrait;
        string              extratrait;
        TypeInfoMacro *     macro = nullptr;
    };

    struct ExprIs : Expression {
        ExprIs () { setRtti(DAS_HASH_TAG("ExprIs")); };
        ExprIs ( const LineInfo & a, const ExpressionPtr & s, const TypeDeclPtr & t )
            : Expression(a), subexpr(s), typeexpr(t) { setRtti(DAS_HASH_TAG("ExprIs")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprAscend : Expression {
        ExprAscend() { setRtti(DAS_HASH_TAG("ExprAscend"));};
        ExprAscend( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & as = nullptr )
            : Expression(a), subexpr(se), ascType(as) { setRtti(DAS_HASH_TAG("ExprAscend")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAscend() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     ascType;
        uint32_t        stackTop = 0;
        union {
            struct {
                bool    useStackRef : 1;
                bool    needTypeInfo : 1;
            };
            uint32_t    ascendFlags = 0;
        };
    };

    struct ExprCast : Expression {
        ExprCast() { setRtti(DAS_HASH_TAG("ExprCast")); };
        ExprCast( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & ct )
            : Expression(a), subexpr(se), castType(ct) { setRtti(DAS_HASH_TAG("ExprCast")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isCast() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     castType;
        union {
            struct {
                bool            upcast : 1;
                bool            reinterpret : 1;
            };
            uint32_t castFlags = 0;
        };
    };

    struct ExprNew : ExprCallFunc {
        ExprNew() { setRtti(DAS_HASH_TAG("ExprNew")); };
        ExprNew ( const LineInfo & a, const TypeDeclPtr & t, bool ini )
            : ExprCallFunc(a,"new"), typeexpr(t), initializer(ini) { setRtti(DAS_HASH_TAG("ExprNew")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr     typeexpr;
        bool            initializer = false;
    };

    struct ExprCall : ExprCallFunc {
        ExprCall () { setRtti(DAS_HASH_TAG("ExprCall")); };
        ExprCall ( const LineInfo & a, const string & n )
            : ExprCallFunc(a,n) { setRtti(DAS_HASH_TAG("ExprCall")); }
        virtual bool rtti_isCall() const override { return true; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode_CallBase * simulateCall (const FunctionPtr & func, const ExprLooksLikeCall * expr,
            Context & context, SimNode_CallBase * pCall);
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        bool doesNotNeedSp = false;
        bool cmresAlias = false;
        bool notDiscarded = false;
        __forceinline bool allowCmresSkip() const {
            return func && (func->copyOnReturn || func->moveOnReturn)
                && !((func->aliasCMRES || cmresAlias) && !func->neverAliasCMRES);
        }
    };

    struct ExprIfThenElse : Expression {
        ExprIfThenElse () { setRtti(DAS_HASH_TAG("ExprIfThenElse")); };
        ExprIfThenElse ( const LineInfo & a, const ExpressionPtr & c,
                        const ExpressionPtr & ift, const ExpressionPtr & iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) { setRtti(DAS_HASH_TAG("ExprIfThenElse")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isIfThenElse() const override { return true; }
        virtual uint32_t getEvalFlags() const override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   cond, if_true, if_false;
        union {
            struct {
                bool isStatic : 1;
                bool doNotFold : 1;
            };
            uint32_t ifFlags = 0;
        };
    };

    struct MakeFieldDecl;
    typedef smart_ptr<MakeFieldDecl>   MakeFieldDeclPtr;

    struct MakeFieldDecl : ptr_ref_count {
        LineInfo        at;
        string          name;
        ExpressionPtr   value;
        ExpressionPtr   tag;
        union {
            struct {
                bool    moveSemantics : 1;
                bool    cloneSemantics : 1;
            };
            uint32_t    flags = 0;
        };
        MakeFieldDecl () = default;
        MakeFieldDecl ( const LineInfo & a, const string & n, const ExpressionPtr & e, bool mv, bool cl )
            : at(a), name(n), value(e) {
            moveSemantics = mv;
            cloneSemantics = cl;
        }
        MakeFieldDeclPtr clone() const;
        void serialize( AstSerializer & ser );
    };

    class MakeStruct : public vector<MakeFieldDeclPtr>, public ptr_ref_count {
    public:
        void serialize( AstSerializer & ser );
    };
    typedef smart_ptr<MakeStruct>      MakeStructPtr;

    struct ExprNamedCall : Expression {
        ExprNamedCall () { setRtti(DAS_HASH_TAG("ExprNamedCall")); };
        ExprNamedCall ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti(DAS_HASH_TAG("ExprNamedCall"));}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isNamedCall() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string      name;
        vector<ExpressionPtr>  nonNamedArguments;
        MakeStruct  arguments;
        bool        argumentsFailedToInfer = false;
        bool        methodCall = false;
    };

    struct ExprMakeLocal : Expression {
        ExprMakeLocal() { setRtti(DAS_HASH_TAG("ExprMakeLocal")); };
        ExprMakeLocal ( const LineInfo & at )
            : Expression(at) { setRtti(DAS_HASH_TAG("ExprMakeLocal")); }
        virtual bool rtti_isMakeLocal() const override { return true; }
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off );
        virtual vector<SimNode *> simulateLocal ( Context & /*context*/ ) const;
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr                 makeType;
        uint32_t                    stackTop = 0;
        uint32_t                    extraOffset = 0;
        union {
            struct {
                bool    useStackRef : 1;
                bool    useCMRES : 1;
                bool    doesNotNeedSp : 1;
                bool    doesNotNeedInit : 1;
                bool    initAllFields : 1;
                bool    alwaysAlias : 1;
            };
            uint32_t makeFlags = 0;
        };
    };

    struct ExprMakeStruct : ExprMakeLocal {
        ExprMakeStruct() { setRtti(DAS_HASH_TAG("ExprMakeStruct")); };
        ExprMakeStruct ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti(DAS_HASH_TAG("ExprMakeStruct")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        virtual bool rtti_isMakeStruct() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        vector<MakeStructPtr>       structs;
        ExpressionPtr               block;
        Function *                  constructor = nullptr;
        union {
            struct {
                bool useInitializer : 1;
                bool isNewHandle : 1;
                bool usedInitializer : 1;
                bool nativeClassInitializer : 1;
                bool isNewClass : 1;
                bool forceClass : 1;
                bool forceStruct : 1;
                bool forceVariant : 1;
                bool forceTuple : 1;
                bool alwaysUseInitializer : 1;
                bool ignoreVisCheck : 1;
            };
            uint32_t makeStructFlags = 0;
        };
    };

    struct ExprMakeVariant : ExprMakeLocal {
        ExprMakeVariant() { setRtti(DAS_HASH_TAG("ExprMakeVariant")); };
        ExprMakeVariant ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti(DAS_HASH_TAG("ExprMakeVariant")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        virtual bool rtti_isMakeVariant() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        vector<MakeFieldDeclPtr>    variants;
    };

    struct ExprMakeArray : ExprMakeLocal {
        ExprMakeArray() { setRtti(DAS_HASH_TAG("ExprMakeArray")); };
        ExprMakeArray ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti(DAS_HASH_TAG("ExprMakeArray")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        virtual bool rtti_isMakeArray() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        TypeDeclPtr                 recordType;
        vector<ExpressionPtr>       values;
        bool                        gen2 = false;
    };

    struct ExprMakeTuple : ExprMakeArray {
        ExprMakeTuple() { setRtti(DAS_HASH_TAG("ExprMakeTuple")); };
        ExprMakeTuple ( const LineInfo & at )
            : ExprMakeArray(at) { setRtti(DAS_HASH_TAG("ExprMakeTuple")); }
        virtual bool rtti_isMakeTuple() const override { return true; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        virtual void serialize( AstSerializer & ser ) override;
        bool isKeyValue = false;
        vector <string> recordNames;
    };

    struct ExprArrayComprehension : Expression {
        ExprArrayComprehension() { setRtti(DAS_HASH_TAG("ExprArrayComprehension")); };
        ExprArrayComprehension ( const LineInfo & at )
            : Expression(at) { setRtti(DAS_HASH_TAG("ExprArrayComprehension")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   exprFor;
        ExpressionPtr   exprWhere;
        ExpressionPtr   subexpr;
        bool            generatorSyntax = false;
        bool            tableSyntax = false;
    };

    struct ExprTypeDecl : Expression {
        ExprTypeDecl () { setRtti(DAS_HASH_TAG("ExprTypeDecl")); };
        ExprTypeDecl ( const LineInfo & a, const TypeDeclPtr & d )
            : Expression(a), typeexpr(d) { setRtti(DAS_HASH_TAG("ExprTypeDecl")); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isTypeDecl() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr         typeexpr;
    };
}

