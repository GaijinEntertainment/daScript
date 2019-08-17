#pragma once

#define FUSION_OP_PTR_VALUE(CTYPE,expr)    (v_ldu((const float *)(expr)))
#define FUSION_OP_ARG_VALUE(CTYPE,expr)    (expr)
#define FUSION_OP_EVAL_CAST(expr)          (expr)
#define FUSION_OP_PTR_RVALUE(CTYPE,expr)   (expr)
