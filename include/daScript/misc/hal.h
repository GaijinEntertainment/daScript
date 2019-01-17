#pragma once

#include <vecmath/dag_vecMath.h>

__forceinline void * v_extract_ptr(vec4i a) {
#if INTPTR_MAX == INT32_MAX
	return (void*)v_extract_xi(a);
#else
	return (void*)v_extract_xi64(a);
#endif

}
__forceinline vec4i v_splats_ptr(void * a) {
#if INTPTR_MAX == INT32_MAX
	return v_splatsi((int32_t)a);
#else
	return v_splatsi64((int64_t)a);
#endif
}

__forceinline vec4f v_mod(vec4f  a, vec4f aDiv) {
	vec4f c = _mm_div_ps(a, aDiv);
	vec4i i = _mm_cvttps_epi32(c);
	vec4f cTrunc = _mm_cvtepi32_ps(i);
	vec4f base = _mm_mul_ps(cTrunc, aDiv);
	vec4f r = _mm_sub_ps(a, base);
	return r;
}

__forceinline vec4i v_muli(vec4i a, vec4i b) {
	vec4i c;
	int32_t * A = (int32_t *)&a;
	int32_t * B = (int32_t *)&b;
	int32_t * C = (int32_t *)&c;
	C[0] = A[0] * B[0];   C[1] = A[1] * B[1];   C[2] = A[2] * B[2];   C[3] = A[3] * B[3];
	return c;
}

__forceinline vec4i v_divi(vec4i a, vec4i b) {
	vec4i c;
	uint32_t * A = (uint32_t *)&a;
	uint32_t * B = (uint32_t *)&b;
	uint32_t * C = (uint32_t *)&c;
	C[0] = A[0] / B[0];   C[1] = A[1] / B[1];   C[2] = A[2] / B[2];   C[3] = A[3] / B[3];
	return c;
}

__forceinline vec4i v_modi(vec4i a, vec4i b) {
	vec4i c;
	int32_t * A = (int32_t *)&a;
	int32_t * B = (int32_t *)&b;
	int32_t * C = (int32_t *)&c;
	C[0] = A[0] % B[0];   C[1] = A[1] % B[1];   C[2] = A[2] % B[2];   C[3] = A[3] % B[3];
	return c;
}

__forceinline vec4i v_mulu(vec4i a, vec4i b) {
	vec4i c;
	uint32_t * A = (uint32_t *)&a;
	uint32_t * B = (uint32_t *)&b;
	uint32_t * C = (uint32_t *)&c;
	C[0] = A[0] * B[0];   C[1] = A[1] * B[1];   C[2] = A[2] * B[2];   C[3] = A[3] * B[3];
	return c;
}

__forceinline vec4i v_divu(vec4i a, vec4i b) {
	vec4i c;
	uint32_t * A = (uint32_t *)&a;
	uint32_t * B = (uint32_t *)&b;
	uint32_t * C = (uint32_t *)&c;
	C[0] = A[0] / B[0];   C[1] = A[1] / B[1];   C[2] = A[2] / B[2];   C[3] = A[3] / B[3];
	return c;
}

__forceinline vec4i v_modu(vec4i a, vec4i b) {
	vec4i c;
	uint32_t * A = (uint32_t *)&a;
	uint32_t * B = (uint32_t *)&b;
	uint32_t * C = (uint32_t *)&c;
	C[0] = A[0] % B[0];   C[1] = A[1] % B[1];   C[2] = A[2] % B[2];   C[3] = A[3] % B[3];
	return c;
}
