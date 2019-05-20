#pragma once

#include "daScript/ast/ast.h"

namespace das
{
    struct ExprRef2Value : Expression {
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * GetR2V ( Context & context, const LineInfo & a, const TypeDeclPtr & type, SimNode * expr );
        virtual bool rtti_isR2V() const override { return true; }
        ExpressionPtr   subexpr;
    };

    struct ExprRef2Ptr : Expression {
        ExprRef2Ptr () = default;
        ExprRef2Ptr ( const LineInfo & a, const ExpressionPtr & s ) : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isRef2Ptr() const override { return true; }
        ExpressionPtr   subexpr;
    };

    struct ExprPtr2Ref : Expression {
        ExprPtr2Ref () = default;
        ExprPtr2Ref ( const LineInfo & a, const ExpressionPtr & s ) : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isPtr2Ref() const override { return true; }
        ExpressionPtr   subexpr;
    };

    struct ExprAddr : Expression {
        ExprAddr () = default;
        ExprAddr ( const LineInfo & a, const string & n ) : Expression(a), target(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAddr() const override { return true; }
        string target;
        Function * func = nullptr;
    };

    struct ExprNullCoalescing : ExprPtr2Ref {
        ExprNullCoalescing () = default;
        ExprNullCoalescing ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & defVal )
            : ExprPtr2Ref(a,s), defaultValue(defVal) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isNullCoalescing() const override { return true; }
        ExpressionPtr   defaultValue;
    };

    struct ExprDelete : Expression {
        ExprDelete() = default;
        ExprDelete ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr subexpr;
    };

    struct ExprAt : Expression {
        ExprAt() = default;
        ExprAt ( const LineInfo & a, const ExpressionPtr & s, const ExpressionPtr & i )
            : Expression(a), subexpr(s), index(i) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isAt() const override { return true; }
        ExpressionPtr   subexpr, index;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t atFlags = 0;
        };
    };

    struct ExprBlock : Expression {
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        uint32_t getFinallyEvalFlags() const;
        virtual ExpressionPtr visit(Visitor & vis) override;
        void visitFinally(Visitor & vis);
        virtual bool rtti_isBlock() const override { return true; }
        VariablePtr findArgument(const string & name);
        vector<SimNode *> collectExpressions ( Context & context, const vector<ExpressionPtr> & list ) const;
        void simulateFinal ( Context & context, SimNode_Final * sim ) const;
        void simulateBlock ( Context & context, SimNode_Block * sim ) const;
        TypeDeclPtr makeBlockType () const;
        vector<ExpressionPtr>   list;
        vector<ExpressionPtr>   finalList;
        TypeDeclPtr             returnType;
        vector<VariablePtr>     arguments;
        uint32_t                stackTop = 0;
        AnnotationList          annotations;
        uint64_t                annotationData = 0;         // to be filled with annotation
        int32_t                 annotationDataIndex = -1;
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
            };
            uint32_t            blockFlags = 0;
        };
    };

    struct ExprVar : Expression {
        ExprVar () = default;
        ExprVar ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isVar() const override { return true; }
        string              name;
        VariablePtr         variable;
        weak_ptr<ExprBlock> pBlock;
        int                 argumentIndex = -1;
        union {
            struct {
                bool        local : 1;
                bool        argument : 1;
                bool        block : 1;
                bool        r2v  : 1;       // built-in ref2value   (read-only)
                bool        r2cr : 1;       // built-in ref2contref (read-only, but stay ref)
                bool        write : 1;
            };
            uint32_t varFlags = 0;
        };
    };

    struct ExprField : Expression {
        ExprField () = default;
        ExprField ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isField() const override { return true; }
        ExpressionPtr   value;
        string          name;
        const Structure::FieldDeclaration * field = nullptr;
        TypeAnnotationPtr annotation;
        union {
            struct {
                bool        r2v : 1;
                bool        r2cr : 1;
                bool        write : 1;
            };
            uint32_t fieldFlags = 0;
        };
    };

    struct ExprSwizzle : Expression {
        ExprSwizzle () = default;
        ExprSwizzle ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : Expression(a), value(val), mask(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isSwizzle() const override { return true; }
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
        ExprSafeField () = default;
        ExprSafeField ( const LineInfo & a, const ExpressionPtr & val, const string & n )
            : ExprField(a,val,n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
         virtual bool rtti_isField() const override { return false; }
         virtual bool rtti_isSafeField() const override { return true; }
        bool skipQQ = false;
    };

    struct ExprLooksLikeCall : Expression {
        ExprLooksLikeCall () = default;
        ExprLooksLikeCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        void autoDereference();
        virtual SimNode * simulate (Context &) const override { return nullptr; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        string describe() const;
        virtual bool rtti_isCallLikeExpr() const override { return true; }
        string                  name;
        vector<ExpressionPtr>   arguments;
        bool                    argumentsFailedToInfer = false;
    };

    struct ExprCallFunc : ExprLooksLikeCall {
        ExprCallFunc () = default;
        ExprCallFunc ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) { }
        Function *      func = nullptr;
        uint32_t        stackTop = 0;
    };

    struct ExprOp : ExprCallFunc {
        ExprOp () = default;
        ExprOp ( const LineInfo & a, const string & o ) : ExprCallFunc(a,o), op(o) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        string  op;
    };

    // unary    !subexpr
    struct ExprOp1 : ExprOp {
        ExprOp1 () = default;
        ExprOp1 ( const LineInfo & a, const string & o, const ExpressionPtr & s )
            : ExprOp(a,o), subexpr(s) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return subexpr->tail(); }
        virtual bool rtti_isOp1() const override { return true; }
        ExpressionPtr   subexpr;
    };

    // binary   left < right
    struct ExprOp2 : ExprOp {
        ExprOp2 () = default;
        ExprOp2 ( const LineInfo & a, const string & o, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), left(l), right(r) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual Expression * tail() override { return right->tail(); }
        virtual bool rtti_isOp2() const override { return true; }
        ExpressionPtr   left, right;
    };

    // this copies one object to the other
    struct ExprCopy : ExprOp2 {
        ExprCopy () = default;
        ExprCopy ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "=", l, r) {};
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        uint32_t    stackTop = 0;
        bool        takeOverRightStack = false;
    };

    // this moves one object to the other
    struct ExprMove : ExprOp2 {
        ExprMove () = default;
        ExprMove ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, "<-", l, r) {};
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
    };

    // this clones one object to the other
    struct ExprClone : ExprOp2 {
        ExprClone () = default;
        ExprClone ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
        : ExprOp2(a, ":=", l, r) {};
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
    };

    // this only exists during parsing, and can't be
    // and this is why it does not have CLONE
    struct ExprSequence : ExprOp2 {
        ExprSequence ( const LineInfo & a, const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp2(a, ",", l, r) {}
        virtual bool rtti_isSequence() const override { return true; }
    };

    // trinary  subexpr ? left : right
    struct ExprOp3 : ExprOp {
        ExprOp3 () = default;
        ExprOp3 ( const LineInfo & a, const string & o, const ExpressionPtr & s,
                 const ExpressionPtr & l, const ExpressionPtr & r )
            : ExprOp(a,o), subexpr(s), left(l), right(r) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual Expression * tail() override { return right->tail(); }
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isOp3() const override { return true; }
        ExpressionPtr   subexpr, left, right;
    };

    struct ExprTryCatch : Expression {
        ExprTryCatch() = default;
        ExprTryCatch ( const LineInfo & a, const ExpressionPtr & t, const ExpressionPtr & c )
            : Expression(a), try_block(t), catch_block(c) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr try_block, catch_block;
    };

    struct ExprReturn : Expression {
        ExprReturn() = default;
        ExprReturn ( const LineInfo & a, const ExpressionPtr & s )
            : Expression(a), subexpr(s) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForReturn; }
        virtual bool rtti_isReturn() const override { return true; }
        ExpressionPtr subexpr;
        union {
            struct {
                bool moveSemantics      : 1;
                bool returnReference    : 1;
                bool returnInBlock      : 1;
                bool takeOverRightStack : 1;
                bool returnCallCMRES    : 1;
                bool returnCMRES        : 1;
            };
            uint32_t    returnFlags = 0;
        };
        uint32_t                stackTop = 0;
        uint32_t                refStackTop = 0;
        Function *              func = nullptr;
        ExprBlock *             block = nullptr;
    };

    struct ExprBreak : Expression {
        ExprBreak() = default;
        ExprBreak ( const LineInfo & a ) : Expression(a) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForBreak; }
        virtual bool rtti_isBreak() const override { return true; }
    };

    struct ExprContinue : Expression {
        ExprContinue() = default;
        ExprContinue ( const LineInfo & a ) : Expression(a) {}
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual uint32_t getEvalFlags() const override { return EvalFlags::stopForContinue; }
        virtual bool rtti_isContinue() const override { return true; }
    };

    struct ExprFakeContext : ExprConstT<void *, ExprFakeContext> {
        ExprFakeContext(void * ptr = nullptr) : ExprConstT(ptr, Type::fakeContext) {}
        ExprFakeContext(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a, ptr, Type::fakeContext) {}
    };

    struct ExprConstPtr : ExprConstT<void *,ExprConstPtr> {
        ExprConstPtr(void * ptr = nullptr) : ExprConstT(ptr,Type::tPointer) {}
        ExprConstPtr(const LineInfo & a, void * ptr = nullptr) : ExprConstT(a,ptr,Type::tPointer) {}
    };

    struct ExprConstInt : ExprConstT<int32_t,ExprConstInt> {
        ExprConstInt(int32_t i = 0)  : ExprConstT(i,Type::tInt) {}
        ExprConstInt(const LineInfo & a, int32_t i = 0)  : ExprConstT(a,i,Type::tInt) {}
    };

    struct ExprConstEnumeration : ExprConstT<int32_t,ExprConstEnumeration> {
        ExprConstEnumeration(int32_t i = 0, const TypeDeclPtr & td = nullptr)
            : ExprConstT(i,Type::tEnumeration) {
                if ( td ) {
                    enumType = td->enumType;
                }
        }
        ExprConstEnumeration(const LineInfo & a, int32_t i, const TypeDeclPtr & td)
            : ExprConstT(a,i,Type::tEnumeration) {
            enumType = td->enumType;
        }
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override {
            auto cexpr = clonePtr<ExprConstEnumeration>(expr);
            ExprConstT<int32_t,ExprConstEnumeration> ::clone(cexpr);
            cexpr->enumType = enumType;
            return cexpr;
        }
        EnumerationPtr enumType;
    };

    struct ExprConstInt64 : ExprConstT<int64_t,ExprConstInt64> {
        ExprConstInt64(int64_t i = 0)  : ExprConstT(i,Type::tInt64) {}
        ExprConstInt64(const LineInfo & a, int64_t i = 0)  : ExprConstT(a,i,Type::tInt64) {}
    };

    struct ExprConstInt2 : ExprConstT<int2,ExprConstInt2> {
        ExprConstInt2(int2 i = int2())  : ExprConstT(i,Type::tInt2) {}
        ExprConstInt2(const LineInfo & a, int2 i)  : ExprConstT(a,i,Type::tInt2) {}
    };

    struct ExprConstInt3 : ExprConstT<int3,ExprConstInt3> {
        ExprConstInt3(int3 i = int3())  : ExprConstT(i,Type::tInt3) {}
        ExprConstInt3(const LineInfo & a, int3 i)  : ExprConstT(a,i,Type::tInt3) {}
    };

    struct ExprConstInt4 : ExprConstT<int4,ExprConstInt4> {
        ExprConstInt4(int4 i = int4())  : ExprConstT(i,Type::tInt4) {}
        ExprConstInt4(const LineInfo & a, int4 i)  : ExprConstT(a,i,Type::tInt4) {}
    };

    struct ExprConstUInt64 : ExprConstT<uint64_t,ExprConstUInt64> {
        ExprConstUInt64(uint64_t i = 0) : ExprConstT(i,Type::tUInt64) {}
        ExprConstUInt64(const LineInfo & a, uint64_t i = 0) : ExprConstT(a,i,Type::tUInt64) {}
    };

    struct ExprConstUInt : ExprConstT<uint32_t,ExprConstUInt> {
        ExprConstUInt(uint32_t i = 0) : ExprConstT(i,Type::tUInt) {}
        ExprConstUInt(const LineInfo & a, uint32_t i = 0) : ExprConstT(a,i,Type::tUInt) {}
    };

    struct ExprConstUInt2 : ExprConstT<uint2,ExprConstUInt2> {
        ExprConstUInt2(uint2 i = uint2())  : ExprConstT(i,Type::tUInt2) {}
        ExprConstUInt2(const LineInfo & a, uint2 i)  : ExprConstT(a,i,Type::tUInt2) {}
    };

    struct ExprConstUInt3 : ExprConstT<uint3,ExprConstUInt3> {
        ExprConstUInt3(uint3 i = uint3())  : ExprConstT(i,Type::tUInt3) {}
        ExprConstUInt3(const LineInfo & a, uint3 i)  : ExprConstT(a,i,Type::tUInt3) {}
    };

    struct ExprConstUInt4 : ExprConstT<uint4,ExprConstUInt4> {
        ExprConstUInt4(uint4 i = uint4())  : ExprConstT(i,Type::tUInt4) {}
        ExprConstUInt4(const LineInfo & a, uint4 i)  : ExprConstT(a,i,Type::tUInt4) {}
    };

    struct ExprConstBool : ExprConstT<bool,ExprConstBool> {
        ExprConstBool(bool i = false) : ExprConstT(i,Type::tBool) {}
        ExprConstBool(const LineInfo & a, bool i = false) : ExprConstT(a,i,Type::tBool) {}
    };

    struct ExprConstFloat : ExprConstT<float,ExprConstFloat> {
        ExprConstFloat(float i = 0.0f) : ExprConstT(i,Type::tFloat) {}
        ExprConstFloat(const LineInfo & a, float i = 0.0f) : ExprConstT(a,i,Type::tFloat) {}
    };

    struct ExprConstDouble : ExprConstT<double,ExprConstDouble> {
        ExprConstDouble(double i = 0.0) : ExprConstT(i,Type::tDouble) {}
        ExprConstDouble(const LineInfo & a, double i = 0.0) : ExprConstT(a,i,Type::tDouble) {}
    };

    struct ExprConstFloat2 : ExprConstT<float2,ExprConstFloat2> {
        ExprConstFloat2(float2 i = float2())  : ExprConstT(i,Type::tFloat2) {}
        ExprConstFloat2(const LineInfo & a, float2 i)  : ExprConstT(a,i,Type::tFloat2) {}
    };

    struct ExprConstFloat3 : ExprConstT<float3,ExprConstFloat3> {
        ExprConstFloat3(float3 i = float3())  : ExprConstT(i,Type::tFloat3) {}
        ExprConstFloat3(const LineInfo & a, float3 i)  : ExprConstT(a,i,Type::tFloat3) {}
    };

    struct ExprConstFloat4 : ExprConstT<float4,ExprConstFloat4> {
        ExprConstFloat4(float4 i = float4())  : ExprConstT(i,Type::tFloat4) {}
        ExprConstFloat4(const LineInfo & a, float4 i)  : ExprConstT(a,i,Type::tFloat4) {}
    };

    struct ExprConstString : ExprConst {
        ExprConstString(const string & str = string())
            : ExprConst(Type::tString), text(str) {}
        ExprConstString(const LineInfo & a, const string & str = string())
            : ExprConst(a,Type::tString), text(str) {}
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        const string & getValue() const { return text; }
        virtual bool rtti_isStringConstant() const override { return true; }
        string  text;
    };

    struct ExprStringBuilder : Expression {
        ExprStringBuilder() = default;
        ExprStringBuilder(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        vector<ExpressionPtr>   elements;
    };

    struct ExprLet : Expression {
        Variable * find ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode * simulateInit(Context & context, const VariablePtr & var, bool local);
        static vector<SimNode *> simulateInit(Context & context, const ExprLet * pLet);
        virtual bool rtti_isLet() const override { return true; }
        vector<VariablePtr>     variables;
        vector<bool>            inScopeVar;
        bool                    inScope = false;
    };

    // for a,b in foo,bar where a>b ...
    struct ExprFor : Expression {
        ExprFor () = default;
        ExprFor ( const LineInfo & a ) : Expression(a) {}
        Variable * findIterator ( const string & name ) const;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isFor() const override { return true; }
        vector<string>          iterators;
        vector<VariablePtr>     iteratorVariables;
        vector<ExpressionPtr>   sources;
        ExpressionPtr           subexpr;
    };

    struct ExprWhile : Expression {
        ExprWhile() = default;
        ExprWhile(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual uint32_t getEvalFlags() const override;
        virtual bool rtti_isWhile() const override { return true; }
        virtual ExpressionPtr visit(Visitor & vis) override;
        static void simulateFinal ( Context & context, const ExpressionPtr & bod, SimNode_Block * blk );
        ExpressionPtr   cond, body;
    };

    struct ExprWith : Expression {
        ExprWith() = default;
        ExprWith(const LineInfo & a) : Expression(a) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isWith() const override { return true; }
        ExpressionPtr   with, body;
    };

    template <typename TT>
    struct ExprLikeCall : ExprLooksLikeCall {
        ExprLikeCall () = default;
        ExprLikeCall ( const LineInfo & a, const string & n ) : ExprLooksLikeCall(a,n) {}
        virtual ExpressionPtr visit ( Visitor & vis ) override;
    };

    struct ExprMakeBlock : Expression {
        ExprMakeBlock () = default;
        ExprMakeBlock ( const LineInfo & a, const ExpressionPtr & b )
        : Expression(a), block(b) { b->at = a; static_pointer_cast<ExprBlock>(b)->isClosure = true; }
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual bool rtti_isMakeBlock() const override { return true; }
        ExpressionPtr block;
        uint32_t stackTop = 0;
    };

    struct ExprMakeLambda : ExprLooksLikeCall {
        ExprMakeLambda () = default;
        ExprMakeLambda ( const LineInfo & a, const ExpressionPtr & b = nullptr );
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprInvoke : ExprLikeCall<ExprInvoke> {
        ExprInvoke () = default;
        ExprInvoke ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprInvoke>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual bool rtti_isInvoke() const override { return true; }
        bool isCopyOrMove() const;
        uint32_t    stackTop = 0;
        bool        doesNotNeedSp = false;
    };

    struct ExprAssert : ExprLikeCall<ExprAssert> {
        ExprAssert ( ) = default;
        ExprAssert ( const LineInfo & a, const string & name, bool isV )
            : ExprLikeCall<ExprAssert>(a,name) { isVerify = isV; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        bool isVerify = false;
    };

    struct ExprStaticAssert : ExprLikeCall<ExprStaticAssert> {
        ExprStaticAssert () = default;
        ExprStaticAssert ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprStaticAssert>(a,name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprDebug : ExprLikeCall<ExprDebug> {
        ExprDebug () = default;
        ExprDebug ( const LineInfo & a, const string & name ) : ExprLikeCall<ExprDebug>(a, name) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    template <typename It, typename SimNodeT, bool first>
    struct ExprTableKeysOrValues : ExprLooksLikeCall {
        ExprTableKeysOrValues() = default;
        ExprTableKeysOrValues ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
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
    };

    template <typename It, typename SimNodeT>
    struct ExprArrayCallWithSizeOrIndex : ExprLooksLikeCall {
        ExprArrayCallWithSizeOrIndex() = default;
        ExprArrayCallWithSizeOrIndex ( const LineInfo & a, const string & name ) : ExprLooksLikeCall(a, name) {}
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
    };

    struct ExprErase : ExprLikeCall<ExprErase> {
        ExprErase() = default;
        ExprErase ( const LineInfo & a, const string & ) : ExprLikeCall<ExprErase>(a, "erase") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprFind : ExprLikeCall<ExprFind> {
        ExprFind() = default;
        ExprFind ( const LineInfo & a, const string & ) : ExprLikeCall<ExprFind>(a, "find") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprKeyExists : ExprLikeCall<ExprKeyExists> {
        ExprKeyExists() = default;
        ExprKeyExists ( const LineInfo & a, const string & ) : ExprLikeCall<ExprKeyExists>(a, "key_exists") {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
    };

    struct ExprTypeInfo : Expression {
        ExprTypeInfo () = default;
        ExprTypeInfo ( const LineInfo & a, const string & tr, const ExpressionPtr & s, const string & stt="" )
            : Expression(a), trait(tr), subexpr(s), subtrait(stt) {}
        ExprTypeInfo ( const LineInfo & a, const string & tr, const TypeDeclPtr & d, const string & stt="" )
            : Expression(a), trait(tr), typeexpr(d), subtrait(stt) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual SimNode * simulate (Context & context) const override;
        string          trait;
        ExpressionPtr   subexpr;
        TypeDeclPtr     typeexpr;
        string          subtrait;
    };

    struct ExprAscend : Expression {
        ExprAscend() = default;
        ExprAscend( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & as = nullptr )
            : Expression(a), subexpr(se), ascType(as) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
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
        ExprCast() = default;
        ExprCast( const LineInfo & a, const ExpressionPtr & se, const TypeDeclPtr & ct, bool uc = false )
            : Expression(a), subexpr(se), castType(ct), upcast(uc) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * trySimulate (Context & context, uint32_t extraOffset, Type r2vType ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isCast() const override { return true; }
        ExpressionPtr   subexpr;
        TypeDeclPtr     castType;
        bool            upcast = false;
    };

    struct ExprNew : ExprCallFunc {
        ExprNew() = default;
        ExprNew ( const LineInfo & a, TypeDeclPtr t, bool ini )
            : ExprCallFunc(a,"new"), typeexpr(t), initializer(ini) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        TypeDeclPtr     typeexpr;
        bool            initializer = false;
    };

    struct ExprCall : ExprCallFunc {
        ExprCall () = default;
        ExprCall ( const LineInfo & a, const string & n ) : ExprCallFunc(a,n) { }
        virtual bool rtti_isCall() const override { return true; }
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        static SimNode_CallBase * simulateCall (const FunctionPtr & func, const ExprLooksLikeCall * expr,
            Context & context, SimNode_CallBase * pCall);
        bool            doesNotNeedSp = false;
    };

    struct ExprIfThenElse : Expression {
        ExprIfThenElse () = default;
        ExprIfThenElse ( const LineInfo & a, const ExpressionPtr & c,
                        const ExpressionPtr & ift, const ExpressionPtr & iff )
            : Expression(a), cond(c), if_true(ift), if_false(iff) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual bool rtti_isIfThenElse() const override { return true; }
        virtual uint32_t getEvalFlags() const override;
        ExpressionPtr   cond, if_true, if_false;
    };

    struct MakeFieldDecl;
    typedef shared_ptr<MakeFieldDecl>   MakeFieldDeclPtr;

    struct MakeFieldDecl : enable_shared_from_this<MakeFieldDecl> {
        LineInfo        at;
        string          name;
        ExpressionPtr   value;
        bool            moveSemantic;
        MakeFieldDecl () = default;
        MakeFieldDecl ( const LineInfo & a, const string & n, const ExpressionPtr & e, bool mv )
            : at(a), name(n), value(e), moveSemantic(mv) {}
        MakeFieldDeclPtr clone() const;
    };

    typedef vector<MakeFieldDeclPtr>    MakeStruct;
    typedef shared_ptr<MakeStruct>      MakeStructPtr;

    struct ExprNamedCall : Expression {
        ExprNamedCall () = default;
        ExprNamedCall ( const LineInfo & a, const string & n ) : Expression(a), name(n) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        string      name;
        MakeStruct  arguments;
        bool        argumentsFailedToInfer = false;
    };

    struct ExprMakeLocal : Expression {
        ExprMakeLocal() = default;
        ExprMakeLocal ( const LineInfo & at ) : Expression(at) {}
        virtual bool rtti_isMakeLocal() const override { return true; }
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off );
        virtual vector<SimNode *> simulateLocal ( Context & /*context*/ ) const;
        TypeDeclPtr                 makeType;
        uint32_t                    stackTop = 0;
        uint32_t                    extraOffset = 0;
        union {
            struct {
                bool    useStackRef;
                bool    useCMRES;
                bool    doesNotNeedSp;
                bool    doesNotNeedInit;
                bool    needTempSrc;
            };
            uint32_t makeFlags = 0;
        };
    };

    struct ExprMakeStructure : ExprMakeLocal {
        ExprMakeStructure() = default;
        ExprMakeStructure ( const LineInfo & at ) : ExprMakeLocal(at) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        vector<MakeStructPtr>       structs;
        bool                        useInitializer;
    };

    struct ExprMakeArray : ExprMakeLocal {
        ExprMakeArray() = default;
        ExprMakeArray ( const LineInfo & at ) : ExprMakeLocal(at) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual vector<SimNode *> simulateLocal ( Context & context ) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        virtual void setRefSp ( bool ref, bool cmres, uint32_t sp, uint32_t off ) override;
        TypeDeclPtr                 recordType;
        vector<ExpressionPtr>       values;
    };

    struct ExprArrayComprehension : Expression {
        ExprArrayComprehension() = default;
        ExprArrayComprehension ( const LineInfo & at ) : Expression(at) {}
        virtual ExpressionPtr clone( const ExpressionPtr & expr = nullptr ) const override;
        virtual SimNode * simulate (Context & context) const override;
        virtual ExpressionPtr visit(Visitor & vis) override;
        ExpressionPtr   exprFor;
        ExpressionPtr   exprWhere;
        ExpressionPtr   subexpr;
    };
}

