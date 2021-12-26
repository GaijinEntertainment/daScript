#pragma once

#include "sfml_stub.h"

namespace das {

// sf::String
template <> struct cast_arg<const sf::String &> {
    static __forceinline sf::String to ( Context & ctx, SimNode * node ) {
        char * pstr = node->evalPtr(ctx);
        return sf::String(pstr ? pstr : "");
    }
};
template <> struct typeFactory<sf::String> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = make_smart<TypeDecl>(Type::tString);
		t->alias = "String";
		t->aotAlias = true;
		return t;
	}
};


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

}
