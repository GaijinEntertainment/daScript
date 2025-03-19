#pragma once

#include "daScript/ast/ast.h"

namespace das
{
    struct ExprClone;

    struct ExprReader : Expression {
        ExprReader () { setRtti("ExprReader"); }
        ExprReader ( const LineInfo & a, const ReaderMacroPtr & rm )
            : Expression(a), macro(rm) { setRtti("ExprReader"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ReaderMacroPtr macro = nullptr;
        string sequence;
    };

    struct ExprLabel : Expression {
        ExprLabel () { setRtti("ExprLabel"); };
        ExprLabel ( const LineInfo & a, int32_t s, const string & cm = string() )
            : Expression(a), label(s), comment(cm) { setRtti("ExprLabel"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isLabel() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        int32_t  label = -1;
        string   comment;
    };

    struct ExprGoto : Expression {
        ExprGoto () { setRtti("ExprGoto"); };
        ExprGoto ( const LineInfo & a, int32_t s )
            : Expression(a), label(s) { setRtti("ExprGoto"); }
        ExprGoto ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti("ExprGoto"); }
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
        ExprRef2Value() { setRtti("ExprRef2Value"); }
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
        ExprRef2Ptr () { setRtti("ExprRef2Ptr"); };
        ExprRef2Ptr ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti("ExprRef2Ptr"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isRef2Ptr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   subexpr;
    };

    struct ExprPtr2Ref : Expression {
        ExprPtr2Ref () { setRtti("ExprPtr2Ref"); };
        ExprPtr2Ref ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti("ExprPtr2Ref"); }
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
        ExprAddr ()  { setRtti("ExprAddr"); };
        ExprAddr ( const LineInfo & a, const string & n )
            : Expression(a), target(n) { setRtti("ExprAddr"); }
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
        ExprNullCoalescing () { setRtti("ExprNullCoalescing"); };
        ExprNullCoalescing ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & defVal )
            : ExprPtr2Ref(a,s), defaultValue(defVal) { setRtti("ExprNullCoalescing"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isNullCoalescing() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        virtual void markNoDiscard() override;
        ExpressionPtr   defaultValue;
    };

    struct ExprDelete : Expression {
        ExprDelete() { setRtti("ExprDelete"); }
        ExprDelete ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti("ExprDelete"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr subexpr;
        ExpressionPtr sizeexpr;
        bool native = false;
    };

    struct ExprAt : Expression {
        ExprAt() { setRtti("ExprAt"); };
        ExprAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i, bool no_promo = false )
            : Expression(a), subexpr(s), index(i) { setRtti("ExprAt"); no_promotion = no_promo; }
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
        ExprSafeAt() { setRtti("ExprSafeAt"); };
        ExprSafeAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i, bool no_promo=false )
            : ExprAt(a,s,i,no_promo) { setRtti("ExprSafeAt"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, const TypeDeclPtr & r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAt() const override { return false; }
        virtual bool rtti_isSafeAt() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
    };


    struct ExprBlock : Expression {
        ExprBlock() { setRtti("ExprBlock"); }
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
        ExprVar () { setRtti("ExprVar"); };
        ExprVar ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti("ExprVar"); }
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
        ExprTag () { setRtti("ExprTag"); }
        ExprTag ( const LineInfo & a, const ExpressionPtr & se, const string & n )
            : Expression(a), subexpr(se), name(n) { setRtti("ExprTag"); }
        ExprTag ( const LineInfo & a, const ExpressionPtr & se, const ExpressionPtr & va, const string & n )
            : Expression(a), subexpr(se), value(va), name(n) { setRtti("ExprTag"); }
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
        ExprField () { setRtti("ExprField"); };
        ExprField ( const LineInfo & a, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : Expression(a), value(val), name(n), atField(a) { setRtti("ExprField"); no_promotion = no_promo; }
        ExprField ( const LineInfo & a, const LineInfo & af, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : Expression(a), value(val), name(n), atField(af) { setRtti("ExprField"); no_promotion = no_promo; }
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
        ExprIsVariant () { setRtti("ExprIsVariant"); };
        ExprIsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti("ExprIsVariant"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isIsVariant() const override { return true; }
    };

    struct ExprAsVariant : ExprField {
        ExprAsVariant () { setRtti("ExprAsVariant"); };
        ExprAsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti("ExprAsVariant"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isAsVariant() const override { return true; }
    };

    struct ExprSafeAsVariant : ExprField {
        ExprSafeAsVariant () { setRtti("ExprSafeAsVariant"); };
        ExprSafeAsVariant ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) { setRtti("ExprSafeAsVariant"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return false; }
        virtual bool rtti_isSafeAsVariant() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        bool skipQQ = false;
    };

    struct ExprSwizzle : Expression {
        ExprSwizzle () { setRtti("ExprSwizzle"); };
        ExprSwizzle ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), mask(n) { setRtti("ExprSwizzle"); }
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
        ExprSafeField () { setRtti("ExprSafeField"); };
        ExprSafeField ( const LineInfo & a, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : ExprField(a,val,n,no_promo) { setRtti("ExprSafeField"); }
        ExprSafeField ( const LineInfo & a, const LineInfo & af, const ExpressionPtr & val, const string & n, bool no_promo=false )
            : ExprField(a,af,val,n,no_promo) { setRtti("ExprSafeField"); }
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
        ExprLooksLikeCall () { setRtti("ExprLooksLikeCall"); };
        ExprLooksLikeCall ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti("ExprLooksLikeCall"); }
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
        ExprCallMacro () { setRtti("ExprCallMacro"); name="__call_macro__"; };
        ExprCallMacro ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti("ExprCallMacro"); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context &) const override { return nullptr; }
        virtual void serialize( AstSerializer & ser ) override;
        Function * inFunction = nullptr;
        CallMacro * macro = nullptr;
    };

    struct ExprCallFunc : ExprLooksLikeCall {
        ExprCallFunc () { setRtti("ExprCallFunc"); };
        ExprCallFunc ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti("ExprCallFunc"); }
        virtual bool rtti_isCallFunc() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        Function *      func = nullptr;
        uint32_t        stackTop = 0;
        bool            genericFunction = false; // do not clone, do not serialize. used only for infer
    };

    struct ExprOp : ExprCallFunc {
        ExprOp () { setRtti("ExprOp"); };
        ExprOp ( const LineInfo & a, const string & o )
            : ExprCallFunc(a,o), op(o) { setRtti("ExprOp"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual void serialize( AstSerializer & ser ) override;
        string  op;
    };

    // unary    !subexpr
    struct ExprOp1 : ExprOp {
        ExprOp1 () { setRtti("ExprOp1"); };
        ExprOp1 ( const LineInfo & a, const string & o, const ExpressionPtr & s )
            : ExprOp(a,o), subexpr(s) { setRtti("ExprOp1"); }
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
        ExprOp2 () { setRtti("ExprOp2"); };
        ExprOp2 ( const LineInfo & a, const string & o, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), left(l), right(r) { setRtti("ExprOp2"); }
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
        ExprCopy () { setRtti("ExprCopy"); };
        ExprCopy ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "=", l, r) { setRtti("ExprCopy"); };
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
        ExprMove () { setRtti("ExprMove"); };
        ExprMove ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "<-", l, r) { setRtti("ExprMove"); };
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
        ExprClone () { setRtti("ExprClone"); };
        ExprClone ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, ":=", l, r) { setRtti("ExprClone"); };
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
            : ExprOp2(a, ",", l, r) { setRtti("ExprSequence"); }
        virtual bool rtti_isSequence() const override { return true; }
    };

    // trinary  subexpr ? left : right
    struct ExprOp3 : ExprOp {
        ExprOp3 () { setRtti("ExprOp3"); };
        ExprOp3 ( const LineInfo & a, const string & o, const ExpressionPtr & s,
                 const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) { setRtti("ExprOp3"); }
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
        ExprTryCatch() { setRtti("ExprTryCatch"); };
        ExprTryCatch ( const LineInfo & a, const ExpressionPtr & t, const ExpressionPtr & c )
            : Expression(a), try_block(t), catch_block(c) { setRtti("ExprTryCatch"); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr try_block, catch_block;
    };

    struct ExprReturn : Expression {
        ExprReturn() { setRtti("ExprReturn"); };
        ExprReturn ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) { setRtti("ExprReturn"); }
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
        ExprBreak() { setRtti("ExprBreak");} ;
        ExprBreak ( const LineInfo & a )
            : Expression(a) { setRtti("ExprBreak"); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForBreak; }
        virtual bool rtti_isBreak() const override { return true; }
    };

    struct ExprContinue : Expression {
        ExprContinue() { setRtti("ExprContinue"); };
        ExprContinue ( const LineInfo & a )
            : Expression(a) { setRtti("ExprContinue"); }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForContinue; }
        virtual bool rtti_isContinue() const override { return true; }
    };

    struct ExprFakeContext : ExprConstT<void *, ExprFakeContext> {
        ExprFakeContext(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeContext) { setRtti("ExprFakeContext"); }
        ExprFakeContext(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeContext) { setRtti("ExprFakeContext"); }
        virtual bool rtti_isFakeContext() const override { return true; }
    };

    struct ExprFakeLineInfo : ExprConstT<void *, ExprFakeLineInfo> {
        ExprFakeLineInfo(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeLineInfo) { setRtti("ExprFakeLineInfo"); }
        ExprFakeLineInfo(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeLineInfo) { setRtti("ExprFakeLineInfo"); }
        virtual bool rtti_isFakeLineInfo() const override { return true; }
    };

    struct ExprConstPtr : ExprConstT<void *,ExprConstPtr> {
        ExprConstPtr(void * ptr = nullptr)
            : ExprConstT(ptr,Type::tPointer) { setRtti("ExprConstPtr"); }
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr)
            : ExprConstT(a,ptr,Type::tPointer) { setRtti("ExprConstPtr"); }
        bool isSmartPtr = false;
        TypeDeclPtr ptrType;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual bool rtti_isNullPtr() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
    };

    struct ExprConstInt : ExprConstT<int32_t,ExprConstInt> {
        ExprConstInt(int32_t i = 0)
            : ExprConstT(i,Type::tInt) { setRtti("ExprConstInt");}
        ExprConstInt(const LineInfo & a, int32_t i = 0)
            : ExprConstT(a,i,Type::tInt) { setRtti("ExprConstInt"); }
    };

    struct ExprConstEnumeration : ExprConst {
        ExprConstEnumeration(int val, const TypeDeclPtr & td)
            : ExprConst(Type::tEnumeration) {
            setRtti("ExprConstEnumeration");
            enumType = td->enumType;
            text = td->enumType->find(int64_t(val),"");
        }
        ExprConstEnumeration(const string & name = string(), const TypeDeclPtr & td = nullptr)
            : ExprConst(Type::tEnumeration), text(name) {
            setRtti("ExprConstEnumeration");
            if ( td ) enumType = td->enumType;
        }
        ExprConstEnumeration(const LineInfo & a, const string & name, const TypeDeclPtr & td)
            : ExprConst(a,Type::tEnumeration), text(name) {
            setRtti("ExprConstEnumeration");
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
            : ExprConstT(i,Type::tInt8) { setRtti("ExprConstInt8"); }
        ExprConstInt8(const LineInfo & a, int8_t i = 0)
            : ExprConstT(a,i,Type::tInt8) { setRtti("ExprConstInt8"); }
    };

    struct ExprConstInt16 : ExprConstT<int16_t,ExprConstInt16> {
        ExprConstInt16(int16_t i = 0)
            : ExprConstT(i,Type::tInt16) { setRtti("ExprConstInt16"); }
        ExprConstInt16(const LineInfo & a, int16_t i = 0)
            : ExprConstT(a,i,Type::tInt16) { setRtti("ExprConstInt16"); }
    };

    struct ExprConstInt64 : ExprConstT<int64_t,ExprConstInt64> {
        ExprConstInt64(int64_t i = 0)
            : ExprConstT(i,Type::tInt64) { setRtti("ExprConstInt64"); }
        ExprConstInt64(const LineInfo & a, int64_t i = 0)
            : ExprConstT(a,i,Type::tInt64) { setRtti("ExprConstInt64"); }
    };

    struct ExprConstBitfield : ExprConstT<uint32_t,ExprConstBitfield> {
        ExprConstBitfield(uint32_t i = 0)
            : ExprConstT(i,Type::tBitfield) { setRtti("ExprConstBitfield"); }
        ExprConstBitfield(const LineInfo & a, uint32_t i = 0)
            : ExprConstT(a,i,Type::tBitfield) { setRtti("ExprConstBitfield"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        TypeDeclPtr bitfieldType;
    };

    struct ExprConstInt2 : ExprConstT<int2,ExprConstInt2> {
        ExprConstInt2(int2 i = int2())
            : ExprConstT(i,Type::tInt2) { setRtti("ExprConstInt2"); }
        ExprConstInt2(const LineInfo & a, int2 i)
            : ExprConstT(a,i,Type::tInt2) { setRtti("ExprConstInt2"); }
    };

    struct ExprConstRange : ExprConstT<range,ExprConstRange> {
        ExprConstRange(range i = range())
            : ExprConstT(i,Type::tRange) { setRtti("ExprConstRange"); }
        ExprConstRange(const LineInfo & a, range i)
            : ExprConstT(a,i,Type::tRange) { setRtti("ExprConstRange"); }
    };

    struct ExprConstRange64 : ExprConstT<range64,ExprConstRange64> {
        ExprConstRange64(range64 i = range64())
            : ExprConstT(i,Type::tRange64) { setRtti("ExprConstRange64"); }
        ExprConstRange64(const LineInfo & a, range64 i)
            : ExprConstT(a,i,Type::tRange64) { setRtti("ExprConstRange64"); }
    };


    struct ExprConstInt3 : ExprConstT<int3,ExprConstInt3> {
        ExprConstInt3(int3 i = int3())
            : ExprConstT(i,Type::tInt3) { setRtti("ExprConstInt3"); }
        ExprConstInt3(const LineInfo & a, int3 i)
            : ExprConstT(a,i,Type::tInt3) { setRtti("ExprConstInt3"); }
    };

    struct ExprConstInt4 : ExprConstT<int4,ExprConstInt4> {
        ExprConstInt4(int4 i = int4())
            : ExprConstT(i,Type::tInt4) { setRtti("ExprConstInt4"); }
        ExprConstInt4(const LineInfo & a, int4 i)
            : ExprConstT(a,i,Type::tInt4) { setRtti("ExprConstInt4"); }
    };

    struct ExprConstUInt8 : ExprConstT<uint8_t,ExprConstUInt8> {
        ExprConstUInt8(uint8_t i = 0)
            : ExprConstT(i,Type::tUInt8) { setRtti("ExprConstUInt8"); }
        ExprConstUInt8(const LineInfo & a, uint8_t i = 0)
            : ExprConstT(a,i,Type::tUInt8) { setRtti("ExprConstUInt8"); }
    };

    struct ExprConstUInt16 : ExprConstT<uint16_t,ExprConstUInt16> {
        ExprConstUInt16(uint16_t i = 0)
            : ExprConstT(i,Type::tUInt16) { setRtti("ExprConstUInt16"); }
        ExprConstUInt16(const LineInfo & a, uint16_t i = 0)
            : ExprConstT(a,i,Type::tUInt16) { setRtti("ExprConstUInt16"); }
    };

    struct ExprConstUInt64 : ExprConstT<uint64_t,ExprConstUInt64> {
        ExprConstUInt64(uint64_t i = 0)
            : ExprConstT(i,Type::tUInt64) { setRtti("ExprConstUInt64"); }
        ExprConstUInt64(const LineInfo & a, uint64_t i = 0)
            : ExprConstT(a,i,Type::tUInt64) { setRtti("ExprConstUInt64"); }
    };

    struct ExprConstUInt : ExprConstT<uint32_t,ExprConstUInt> {
        ExprConstUInt(uint32_t i = 0)
            : ExprConstT(i,Type::tUInt) { setRtti("ExprConstUInt"); }
        ExprConstUInt(const LineInfo & a, uint32_t i = 0)
            : ExprConstT(a,i,Type::tUInt) { setRtti("ExprConstUInt"); }
    };

    int64_t getConstExprIntOrUInt ( const ExpressionPtr & expr );

    struct ExprConstUInt2 : ExprConstT<uint2,ExprConstUInt2> {
        ExprConstUInt2(uint2 i = uint2())
            : ExprConstT(i,Type::tUInt2) { setRtti("ExprConstUInt2"); }
        ExprConstUInt2(const LineInfo & a, uint2 i)
            : ExprConstT(a,i,Type::tUInt2) { setRtti("ExprConstUInt2"); }
    };

    struct ExprConstURange : ExprConstT<urange,ExprConstURange> {
        ExprConstURange(urange i = urange())
            : ExprConstT(i,Type::tURange) { setRtti("ExprConstURange"); }
        ExprConstURange(const LineInfo & a, urange i)
            : ExprConstT(a,i,Type::tURange) { setRtti("ExprConstURange"); }
    };

    struct ExprConstURange64 : ExprConstT<urange64,ExprConstURange64> {
        ExprConstURange64(urange64 i = urange64())
            : ExprConstT(i,Type::tURange64) { setRtti("ExprConstURange64"); }
        ExprConstURange64(const LineInfo & a, urange64 i)
            : ExprConstT(a,i,Type::tURange64) { setRtti("ExprConstURange64"); }
    };

    struct ExprConstUInt3 : ExprConstT<uint3,ExprConstUInt3> {
        ExprConstUInt3(uint3 i = uint3())
            : ExprConstT(i,Type::tUInt3) { setRtti("ExprConstUInt3"); }
        ExprConstUInt3(const LineInfo & a, uint3 i)
            : ExprConstT(a,i,Type::tUInt3) { setRtti("ExprConstUInt3"); }
    };

    struct ExprConstUInt4 : ExprConstT<uint4,ExprConstUInt4> {
        ExprConstUInt4(uint4 i = uint4())
            : ExprConstT(i,Type::tUInt4) { setRtti("ExprConstUInt4"); }
        ExprConstUInt4(const LineInfo & a, uint4 i)
            : ExprConstT(a,i,Type::tUInt4) { setRtti("ExprConstUInt4"); }
    };

    struct ExprConstBool : ExprConstT<bool,ExprConstBool> {
        ExprConstBool(bool i = false)
            : ExprConstT(i,Type::tBool) { setRtti("ExprConstBool"); }
        ExprConstBool(const LineInfo & a, bool i = false)
            : ExprConstT(a,i,Type::tBool) { setRtti("ExprConstBool"); }
    };

    struct ExprConstFloat : ExprConstT<float,ExprConstFloat> {
        ExprConstFloat(float i = 0.0f)
            : ExprConstT(i,Type::tFloat) { setRtti("ExprConstFloat"); }
        ExprConstFloat(int i)
            : ExprConstT(float(i),Type::tFloat) { setRtti("ExprConstFloat"); }
        ExprConstFloat(double i)
            : ExprConstT(float(i),Type::tFloat) { setRtti("ExprConstFloat"); }
        ExprConstFloat(const LineInfo & a, float i = 0.0f)
            : ExprConstT(a,i,Type::tFloat) { setRtti("ExprConstFloat"); }
    };

    struct ExprConstDouble : ExprConstT<double,ExprConstDouble> {
        ExprConstDouble(double i = 0.0)
            : ExprConstT(i,Type::tDouble) { setRtti("ExprConstDouble"); }
        ExprConstDouble(const LineInfo & a, double i = 0.0)
            : ExprConstT(a,i,Type::tDouble) { setRtti("ExprConstDouble"); }
    };

    struct ExprConstFloat2 : ExprConstT<float2,ExprConstFloat2> {
        ExprConstFloat2(float2 i = float2())
            : ExprConstT(i,Type::tFloat2) { setRtti("ExprConstFloat2"); }
        ExprConstFloat2(const LineInfo & a, float2 i)
            : ExprConstT(a,i,Type::tFloat2) { setRtti("ExprConstFloat2"); }
    };

    struct ExprConstFloat3 : ExprConstT<float3,ExprConstFloat3> {
        ExprConstFloat3(float3 i = float3())
            : ExprConstT(i,Type::tFloat3) { setRtti("ExprConstFloat3"); }
        ExprConstFloat3(const LineInfo & a, float3 i)
            : ExprConstT(a,i,Type::tFloat3) { setRtti("ExprConstFloat3"); }
    };

    struct ExprConstFloat4 : ExprConstT<float4,ExprConstFloat4> {
        ExprConstFloat4(float4 i = float4())
            : ExprConstT(i,Type::tFloat4) { setRtti("ExprConstFloat4"); }
        ExprConstFloat4(const LineInfo & a, float4 i)
            : ExprConstT(a,i,Type::tFloat4) { setRtti("ExprConstFloat4"); }
    };

    struct ExprConstString : ExprConst {
        ExprConstString(const string & str = string())
            : ExprConst(Type::tString), text(str) { setRtti("ExprConstString"); }
        ExprConstString(const LineInfo & a, const string & str = string())
            : ExprConst(a,Type::tString), text(str) { setRtti("ExprConstString"); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        const string & getValue() const { return text; }
        virtual bool rtti_isStringConstant() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        string  text;
    };

    struct ExprStringBuilder : Expression {
        ExprStringBuilder() { setRtti("ExprStringBuilder");  };
        ExprStringBuilder(const LineInfo & a)
            : Expression(a) { setRtti("ExprStringBuilder"); }
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
        ExprLet() { setRtti("ExprLet"); }
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
        ExprFor () { setRtti("ExprFor"); };
        ExprFor ( const LineInfo & a )
            : Expression(a) { setRtti("ExprFor"); }
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
        ExprUnsafe() { setRtti("ExprUnsafe"); };
        ExprUnsafe(const LineInfo & a)
            : Expression(a) { setRtti("ExprUnsafe"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isUnsafe() const override { return true; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   body;
    };

    struct ExprWhile : Expression {
        ExprWhile() { setRtti("ExprWhile"); };
        ExprWhile(const LineInfo & a)
            : Expression(a) { setRtti("ExprWhile"); }
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
        ExprWith() { setRtti("ExprWith"); };
        ExprWith(const LineInfo & a)
            : Expression(a) { setRtti("ExprWith"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isWith() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   with, body;
    };

    struct ExprAssume : Expression {
        ExprAssume() { setRtti("ExprAssume"); };
        ExprAssume(const LineInfo & a, const string & al, const ExpressionPtr & se )
            : Expression(a), alias(al), subexpr(se) { setRtti("ExprAssume"); }
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
        ExprLikeCall () { setRtti("ExprLikeCall"); };
        ExprLikeCall ( const LineInfo & a, const string & n )
            : ExprLooksLikeCall(a,n) { setRtti("ExprLikeCall"); }
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
        ExprMakeBlock () { setRtti("ExprMakeBlock"); };
        ExprMakeBlock ( const LineInfo & a, const ExpressionPtr & b, bool isl = false, bool islf = false )
            : Expression(a), block(b) {
            setRtti("ExprMakeBlock");
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
        ExprMakeGenerator () { setRtti("ExprMakeGenerator"); };
        ExprMakeGenerator ( const LineInfo & a, const ExpressionPtr & b = nullptr );
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr iterType;
        vector<CaptureEntry> capture;
    };

    struct ExprYield : Expression {
        ExprYield () { setRtti("ExprYield"); };
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
        ExprInvoke () { setRtti("ExprInvoke"); name = "invoke"; };
        ExprInvoke ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprInvoke>(a,name) { setRtti("ExprInvoke"); }
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
        ExprAssert ( ) { setRtti("ExprAssert"); name="assert"; };
        ExprAssert ( const LineInfo & a, const string & name, bool isV )
            : ExprLikeCall<ExprAssert>(a,name) { isVerify = isV; setRtti("ExprAssert"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        bool isVerify = false;
    };

    struct ExprQuote : ExprLikeCall<ExprQuote> {
        ExprQuote ( ) { setRtti("ExprQuote"); name="quote"; };
        ExprQuote ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprQuote>(a,name) { setRtti("ExprQuote"); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
    };

    struct ExprStaticAssert : ExprLikeCall<ExprStaticAssert> {
        ExprStaticAssert () { setRtti("ExprStaticAssert"); name="static_assert"; };
        ExprStaticAssert ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprStaticAssert>(a,name) { setRtti("ExprStaticAssert"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprDebug : ExprLikeCall<ExprDebug> {
        ExprDebug () { setRtti("ExprDebug"); name="debug"; };
        ExprDebug ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprDebug>(a, name) { setRtti("ExprDebug"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprMemZero : ExprLikeCall<ExprMemZero> {
        ExprMemZero () { setRtti("ExprMemZero"); name="memzero"; };
        ExprMemZero ( const LineInfo & a, const string & name )
            : ExprLikeCall<ExprMemZero>(a, name) { setRtti("ExprMemZero"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    template <typename It, typename SimNodeT, bool first>
    struct ExprTableKeysOrValues : ExprLooksLikeCall {
        ExprTableKeysOrValues() { setRtti("ExprTableKeysOrValues"); };
        ExprTableKeysOrValues ( const LineInfo & a, const string & name )
            : ExprLooksLikeCall(a, name) { setRtti("ExprTableKeysOrValues"); }
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
        ExprArrayCallWithSizeOrIndex() { setRtti("ExprArrayCallWithSizeOrIndex"); };
        ExprArrayCallWithSizeOrIndex ( const LineInfo & a, const string & name )
            : ExprLooksLikeCall(a, name) { setRtti("ExprArrayCallWithSizeOrIndex"); }
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
        ExprErase() { setRtti("ExprErase"); name="erase"; };
        ExprErase ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprErase>(a, "erase") { setRtti("ExprErase"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprFind : ExprLikeCall<ExprFind> {
        ExprFind() { setRtti("ExprFind"); name="find"; };
        ExprFind ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprFind>(a, "find") { setRtti("ExprFind"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprKeyExists : ExprLikeCall<ExprKeyExists> {
        ExprKeyExists() { setRtti("ExprKeyExists"); name="key_exists"; };
        ExprKeyExists ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprKeyExists>(a, "key_exists") { setRtti("ExprKeyExists"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprSetInsert : ExprLikeCall<ExprSetInsert> {
        ExprSetInsert() { setRtti("ExprSetInsert"); name="insert"; };
        ExprSetInsert ( const LineInfo & a, const string & )
            : ExprLikeCall<ExprSetInsert>(a, "insert") { setRtti("ExprSetInsert"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprTypeInfo : Expression {
        ExprTypeInfo () { setRtti("ExprTypeInfo"); }
        ExprTypeInfo ( const LineInfo & a, const string & tr, const ExpressionPtr & s,
                      const string & stt="", const string & ett="" )
            : Expression(a), trait(tr), subexpr(s), subtrait(stt), extratrait(ett) { setRtti("ExprTypeInfo"); }
        ExprTypeInfo ( const LineInfo & a, const string & tr, const TypeDeclPtr & d,
                      const string & stt="", const string & ett="" )
            : Expression(a), trait(tr), typeexpr(d), subtrait(stt), extratrait(ett) { setRtti("ExprTypeInfo"); }
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
        ExprIs () { setRtti("ExprIs"); };
        ExprIs ( const LineInfo & a, const ExpressionPtr & s, const TypeDeclPtr & t )
            : Expression(a), subexpr(s), typeexpr(t) { setRtti("ExprIs"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        virtual void serialize( AstSerializer & ser ) override;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
    };

    struct ExprAscend : Expression {
        ExprAscend() { setRtti("ExprAscend");};
        ExprAscend( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & as = nullptr )
            : Expression(a), subexpr(se), ascType(as) { setRtti("ExprAscend"); }
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
        ExprCast() { setRtti("ExprCast"); };
        ExprCast( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & ct )
            : Expression(a), subexpr(se), castType(ct) { setRtti("ExprCast"); }
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
        ExprNew() { setRtti("ExprNew"); };
        ExprNew ( const LineInfo & a, const TypeDeclPtr & t, bool ini )
            : ExprCallFunc(a,"new"), typeexpr(t), initializer(ini) { setRtti("ExprNew"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr     typeexpr;
        bool            initializer = false;
    };

    struct ExprCall : ExprCallFunc {
        ExprCall () { setRtti("ExprCall"); };
        ExprCall ( const LineInfo & a, const string & n )
            : ExprCallFunc(a,n) { setRtti("ExprCall"); }
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
        ExprIfThenElse () { setRtti("ExprIfThenElse"); };
        ExprIfThenElse ( const LineInfo & a, const ExpressionPtr & c,
                        const ExpressionPtr & ift, const ExpressionPtr & iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) { setRtti("ExprIfThenElse"); }
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
        ExprNamedCall () { setRtti("ExprNamedCall"); };
        ExprNamedCall ( const LineInfo & a, const string & n )
            : Expression(a), name(n) { setRtti("ExprNamedCall");}
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
        ExprMakeLocal() { setRtti("ExprMakeLocal"); };
        ExprMakeLocal ( const LineInfo & at )
            : Expression(at) { setRtti("ExprMakeLocal"); }
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
        ExprMakeStruct() { setRtti("ExprMakeStruct"); };
        ExprMakeStruct ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti("ExprMakeStruct"); }
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
        ExprMakeVariant() { setRtti("ExprMakeVariant"); };
        ExprMakeVariant ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti("ExprMakeVariant"); }
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
        ExprMakeArray() { setRtti("ExprMakeArray"); };
        ExprMakeArray ( const LineInfo & at )
            : ExprMakeLocal(at) { setRtti("ExprMakeArray"); }
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
        ExprMakeTuple() { setRtti("ExprMakeTuple"); };
        ExprMakeTuple ( const LineInfo & at )
            : ExprMakeArray(at) { setRtti("ExprMakeTuple"); }
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
        ExprArrayComprehension() { setRtti("ExprArrayComprehension"); };
        ExprArrayComprehension ( const LineInfo & at )
            : Expression(at) { setRtti("ExprArrayComprehension"); }
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
        ExprTypeDecl () { setRtti("ExprTypeDecl"); };
        ExprTypeDecl ( const LineInfo & a, const TypeDeclPtr & d )
            : Expression(a), typeexpr(d) { setRtti("ExprTypeDecl"); }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isTypeDecl() const override { return true; }
        virtual void serialize( AstSerializer & ser ) override;
        TypeDeclPtr         typeexpr;
    };
}

