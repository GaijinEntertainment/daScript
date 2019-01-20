#include "daScript/misc/platform.h"

#include "daScript/ast/ast_match.h"

namespace das {
    
    // recognize a==0, a!=0, 0==a, 0!=a
    bool matchEquNequZero ( const ExpressionPtr & expr, ExpressionPtr & zeroCond, bool & condIfZero ) {
        if ( expr->rtti_isOp2() ) {
            auto op2 = static_pointer_cast<ExprOp2>(expr);
            if ( op2->op=="==" || op2->op=="!=" ) {
                condIfZero = op2->op == "==";
                if ( isZeroConst(op2->left) ) {
                    zeroCond = op2->right;
                    return true;
                } else if ( isZeroConst(op2->right) ) {
                    zeroCond = op2->left;
                    return true;
                }
            }
        }
        return false;
    }
    
    bool isZeroConst ( const ExpressionPtr & expr ) {
        return isFloatConst(expr, 0.0f) || isIntOrUIntConst(expr, 0);
    }
    
    bool isFloatConst ( const ExpressionPtr & expr, float value ) {
        if ( !expr->rtti_isConstant() ) return false;
        auto ce = static_pointer_cast<ExprConst>(expr);
        switch ( ce->baseType ) {
            case Type::tFloat:      return cast<float>::to(ce->value) == value;
            default:                return false;
        }
    }
    
    bool isIntOrUIntConst ( const ExpressionPtr & expr, int64_t value ) {
        if ( !expr->rtti_isConstant() ) return false;
        auto ce = static_pointer_cast<ExprConst>(expr);
        switch ( ce->baseType ) {
            case Type::tInt:        return int64_t ( cast<int32_t>::to(ce->value) ) == value;
            case Type::tInt64:      return cast<int64_t>::to(ce->value) == value;
            case Type::tUInt:       return int64_t ( cast<uint32_t>::to(ce->value) ) == value;
            case Type::tUInt64:     return int64_t ( cast<uint64_t>::to(ce->value) ) == value;
            default:                return false;
        }
    }
    
    bool isIntConst ( const ExpressionPtr & expr, int64_t value ) {
        if ( !expr->rtti_isConstant() ) return false;
        auto ce = static_pointer_cast<ExprConst>(expr);
        switch ( ce->baseType ) {
            case Type::tInt:        return int64_t ( cast<int32_t>::to(ce->value) ) == value;
            case Type::tInt64:      return cast<int64_t>::to(ce->value) == value;
            default:                return false;
        }
    }
    
    bool isUIntConst ( const ExpressionPtr & expr, uint64_t value ) {
        if ( !expr->rtti_isConstant() ) return false;
        auto ce = static_pointer_cast<ExprConst>(expr);
        switch ( ce->baseType ) {
            case Type::tUInt:       return uint64_t ( cast<uint32_t>::to(ce->value) ) == value;
            case Type::tUInt64:     return cast<uint64_t>::to(ce->value) == value;
            default:                return false;
        }
    }
    
}
