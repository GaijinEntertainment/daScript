#pragma once

#include "sfml_stub.h"

#include "daScript/misc/type_name.h"

namespace das {

// sf::String
template <> struct cast_arg<const sf::String &> {
    static __forceinline sf::String to ( Context & ctx, SimNode * node ) {
        char * pstr = node->evalPtr(ctx);
        return sf::String(pstr ? pstr : "");
    }
};
template <> struct cast_res<sf::String> {
    static __forceinline vec4f from ( const sf::String & str, Context * context ) {
		auto text = context->stringHeap->allocateString(str);
        return cast<char *>::from(text);
    }
};
template <> struct typeFactory<sf::String> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tString);
		t->alias = "String";
		return t;
	}
};
template <> struct typeName<sf::String> : typeName<char *> {};

// Vector2u
template<> struct cast <sf::Vector2u> : cast_fVec<sf::Vector2u> {};
template <> struct typeFactory<sf::Vector2u> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tUInt2);
		t->alias = "Vector2u";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Vector2u> { constexpr static const char * name() { return "Vector2u"; } };
template <> struct cast_arg<const sf::Vector2u &> {
    static __forceinline sf::Vector2u to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Vector2u v2; memcpy(&v2,&res,sizeof(sf::Vector2u));
        return v2;
    }
};

// Vector2i
template<> struct cast <sf::Vector2i> : cast_fVec<sf::Vector2i> {};
template <> struct typeFactory<sf::Vector2i> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tInt2);
		t->alias = "Vector2i";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Vector2i> { constexpr static const char * name() { return "Vector2i"; } };
template <> struct cast_arg<const sf::Vector2i &> {
    static __forceinline sf::Vector2i to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Vector2i v2; memcpy(&v2,&res,sizeof(sf::Vector2i));
        return v2;
    }
};

// Vector3i
template<> struct cast <sf::Vector3i> : cast_fVec<sf::Vector3i> {};
template <> struct typeFactory<sf::Vector3i> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tInt4);
		t->alias = "Vector3i";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Vector3i> { constexpr static const char * name() { return "Vector3i"; } };
template <> struct cast_arg<const sf::Vector3i &> {
    static __forceinline sf::Vector3i to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Vector3i v2; memcpy(&v2,&res,sizeof(sf::Vector3i));
        return v2;
    }
};

// Glsl::Ivec4
template<> struct cast <sf::Glsl::Ivec4> : cast_fVec<sf::Glsl::Ivec4> {};
template <> struct typeFactory<sf::Glsl::Ivec4> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tInt4);
		t->alias = "Ivec4";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Glsl::Ivec4> { constexpr static const char * name() { return "Ivec4"; } };
template <> struct cast_arg<const sf::Glsl::Ivec4 &> {
    static __forceinline sf::Glsl::Ivec4 to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Glsl::Ivec4 v2; memcpy(&v2,&res,sizeof(sf::Glsl::Ivec4));
        return v2;
    }
};


// Vector2f
template<> struct cast <sf::Vector2f> : cast_fVec<sf::Vector2f> {};
template <> struct typeFactory<sf::Vector2f> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tFloat2);
		t->alias = "Vector2f";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Vector2f> { constexpr static const char * name() { return "Vector2f"; } };
template <> struct cast_arg<const sf::Vector2f &> {
    static __forceinline sf::Vector2f to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Vector2f v2; memcpy(&v2,&res,sizeof(sf::Vector2f));
        return v2;
    }
};

// Vector3f
template<> struct cast <sf::Vector3f> : cast_fVec<sf::Vector3f> {};
template <> struct typeFactory<sf::Vector3f> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tFloat3);
		t->alias = "Vector3f";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Vector3f> { constexpr static const char * name() { return "Vector3f"; } };
template <> struct cast_arg<const sf::Vector3f &> {
    static __forceinline sf::Vector3f to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Vector3f v2; memcpy(&v2,&res,sizeof(sf::Vector3f));
        return v2;
    }
};

// Glsl::Vec4
template<> struct cast <sf::Glsl::Vec4> : cast_fVec<sf::Glsl::Vec4> {};
template <> struct typeFactory<sf::Glsl::Vec4> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tFloat4);
		t->alias = "Vec4";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::Glsl::Vec4> { constexpr static const char * name() { return "Vec4"; } };
template <> struct cast_arg<const sf::Glsl::Vec4 &> {
    static __forceinline sf::Glsl::Vec4 to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::Glsl::Vec4 v2; memcpy(&v2,&res,sizeof(sf::Glsl::Vec4));
        return v2;
    }
};

// IntRect
template<> struct cast <sf::IntRect> : cast_fVec<sf::IntRect> {};
template <> struct typeFactory<sf::IntRect> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tInt4);
		t->alias = "IntRect";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::IntRect> { constexpr static const char * name() { return "IntRect"; } };
template <> struct cast_arg<const sf::IntRect &> {
    static __forceinline sf::IntRect to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::IntRect v2; memcpy(&v2,&res,sizeof(sf::IntRect));
        return v2;
    }
};

// FloatRect
template<> struct cast <sf::FloatRect> : cast_fVec<sf::FloatRect> {};
template <> struct typeFactory<sf::FloatRect> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tFloat4);
		t->alias = "FloatRect";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<sf::FloatRect> { constexpr static const char * name() { return "FloatRect"; } };
template <> struct cast_arg<const sf::FloatRect &> {
    static __forceinline sf::FloatRect to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        sf::FloatRect v2; memcpy(&v2,&res,sizeof(sf::FloatRect));
        return v2;
    }
};

}

// TODO: some sort of boolean vector goo
MAKE_EXTERNAL_TYPE_FACTORY(Bvec2,sf::Glsl::Bvec2);
MAKE_EXTERNAL_TYPE_FACTORY(Bvec3,sf::Glsl::Bvec3);
MAKE_EXTERNAL_TYPE_FACTORY(Bvec4,sf::Glsl::Bvec4);

// TODO: support proper matrices
MAKE_EXTERNAL_TYPE_FACTORY(Mat3,sf::Glsl::Mat3);
MAKE_EXTERNAL_TYPE_FACTORY(Mat4,sf::Glsl::Mat4);
