// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

template <> struct cast_arg<GLFWvidmode> {
	static __forceinline const GLFWvidmode & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<GLFWvidmode *>::to(res);
	}
};
template <> struct cast_arg<GLFWgammaramp> {
	static __forceinline const GLFWgammaramp & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<GLFWgammaramp *>::to(res);
	}
};
template <> struct cast_arg<GLFWimage> {
	static __forceinline const GLFWimage & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<GLFWimage *>::to(res);
	}
};
template <> struct cast_arg<GLFWgamepadstate> {
	static __forceinline const GLFWgamepadstate & to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<GLFWgamepadstate *>::to(res);
	}
};
