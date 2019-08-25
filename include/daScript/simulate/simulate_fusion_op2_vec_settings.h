#pragma once

#define FUSION_OP2_PTR(CTYPE,expr)              ((expr))
#define FUSION_OP2_RVALUE_LEFT(CTYPE,expr)      (v_ldu((const float *)(expr)))
#define FUSION_OP2_RVALUE_RIGHT(CTYPE,expr)     (v_ldu((const float *)(expr)))
#define FUSION_OP2_PTR_TO_LVALUE(expr)          ((expr))
