#pragma once

#include "imgui_stub.h"
#include "daScript/simulate/cast.h"

namespace das {

template <> struct typeFactory<ImColor> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = new TypeDecl(Type::tFloat4);
		t->alias = "ImColor";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<ImColor> { constexpr static const char * name() { return "ImColor"; } };
template <>
struct cast_arg<const ImColor &> {
    static __forceinline ImColor to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        ImColor col; memcpy(&col,&res,sizeof(ImColor));
        return col;
    }
};

template <> struct typeFactory<ImVec2> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = new TypeDecl(Type::tFloat2);
		t->alias = "ImVec2";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<ImVec2> { constexpr static const char * name() { return "ImVec2"; } };
template <> struct cast_arg<const ImVec2 &> {
    static __forceinline ImVec2 to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        ImVec2 v2; memcpy(&v2,&res,sizeof(ImVec2));
        return v2;
    }
};

template <> struct typeFactory<ImVec4> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = new TypeDecl(Type::tFloat4);
		t->alias = "ImVec4";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<ImVec4> { constexpr static const char * name() { return "ImVec4"; } };
template <> struct cast_arg<const ImVec4 &> {
    static __forceinline ImVec4 to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        ImVec4 v4; memcpy(&v4,&res,sizeof(ImVec4));
        return v4;
    }
};

template<> struct cast <ImVec2>  : cast_fVec_half<ImVec2> {};
template<> struct cast <ImVec4>  : cast_fVec<ImVec4> {};
template<> struct cast <ImColor> : cast_fVec<ImColor> {};

template<> struct WrapType<ImVec2> { enum { value = true }; typedef vec4f type; typedef vec4f rettype; };
template<> struct WrapType<ImVec4> { enum { value = true }; typedef vec4f type; typedef vec4f rettype; };
template<> struct WrapArgType<ImVec2> { typedef WrapVec2Arg<ImVec2, float> type; };
template<> struct WrapArgType<ImVec4> { typedef WrapVec4Arg<ImVec4, float> type; };
template<> struct WrapRetType<ImVec2> { typedef WrapVec2Arg<ImVec2, float> type; };
template<> struct WrapRetType<ImVec4> { typedef WrapVec4Arg<ImVec4, float> type; };

template <> struct JitConstRefByValue<const ImVec2&> { enum { value = true }; };
template<> struct WrapArgType<const ImVec2&> { using type = const WrapArgType<ImVec2>::type&; };
template <> struct JitConstRefByValue<const ImVec4&> { enum { value = true }; };
template<> struct WrapArgType<const ImVec4&> { using type = const WrapArgType<ImVec4>::type&; };

// ImRect (imgui_internal.h) is { ImVec2 Min; ImVec2 Max } — 4 contiguous floats,
// layout-identical to float4/vec4f, so it aliases to float4 like ImVec4 and internal
// rect-taking helpers (ItemSize, RenderTextClipped, ...) bind without pulling ImRect
// in as a struct. Unlike ImVec4 it has no x/y/z/w members, so the Wrap*Arg path can't
// reuse WrapVec4Arg (which defaults its field pointer to &T::x) — bridge vec4f<->ImRect
// by memcpy instead.
static_assert(sizeof(ImRect) == sizeof(float) * 4, "ImRect must be 4 contiguous floats to alias float4");
template <> struct typeFactory<ImRect> {
	static TypeDeclPtr make(const ModuleLibrary &) {
		auto t = new TypeDecl(Type::tFloat4);
		t->alias = "ImRect";
		t->aotAlias = true;
		return t;
	}
};
template <> struct typeName<ImRect> { constexpr static const char * name() { return "ImRect"; } };
template <> struct cast_arg<const ImRect &> {
    static __forceinline ImRect to ( Context & ctx, SimNode * node ) {
        vec4f res = node->eval(ctx);
        ImRect r; memcpy(&r,&res,sizeof(ImRect));
        return r;
    }
};
template<> struct cast <ImRect> : cast_fVec<ImRect> {};
template<> struct WrapType<ImRect> { enum { value = true }; typedef vec4f type; typedef vec4f rettype; };
struct WrapImRectArg : ImRect {
    __forceinline WrapImRectArg(vec4f t) { memcpy(static_cast<ImRect*>(this), &t, sizeof(ImRect)); }
    __forceinline operator vec4f() const { vec4f r; memcpy(&r, static_cast<const ImRect*>(this), sizeof(ImRect)); return r; }
};
template<> struct WrapArgType<ImRect> { typedef WrapImRectArg type; };
template<> struct WrapRetType<ImRect> { typedef WrapImRectArg type; };
template <> struct JitConstRefByValue<const ImRect&> { enum { value = true }; };
template<> struct WrapArgType<const ImRect&> { using type = const WrapArgType<ImRect>::type&; };

template <>
struct typeName<char> {
    static string name() {
        return string("char");
    }
};

template <typename TT>
struct typeName<ImVector<TT>> {
    static string name() {
        return string("ImVector`") + typeName<TT>::name();
    }
};

template <typename TT>
struct typeFactory<ImVector<TT>> {
    using VT = ImVector<TT>;
    static TypeDeclPtr make(const ModuleLibrary & library ) {
        string declN = typeName<VT>::name();
        if ( library.findAnnotation(declN,nullptr).size()==0 ) {
            auto declT = makeType<TT>(library);
            auto ann = new ManagedVectorAnnotation<VT>(declN,const_cast<ModuleLibrary &>(library));
            ann->cppName = "ImVector<" + describeCppType(declT) + ">";
            auto mod = library.back(); // lets try like this
            mod->addAnnotation(ann);
            registerVectorFunctions<VT,has_cast<TT>::value>::init(mod,library,
                declT->canCopy(),
                declT->canMove()
            );
        }
        return makeHandleType(library,declN.c_str());
    }
};

struct imguiTempFn {
    imguiTempFn() = default;
    ___noinline bool operator()( Function * fn ) {
        if ( tempArgs || implicitArgs ) {
            for ( auto &arg : fn->arguments ) {
                if ( arg->type->isTempType() ) {
                    arg->type->temporary = tempArgs;
                    arg->type->implicit = implicitArgs;
                    arg->type->explicitConst = explicitConstArgs;
                }
            }
        }
        if ( tempResult ) {
            if ( fn->result->isTempType() ) {
                fn->result->temporary = true;
            }
        }

        bool anyString = false;
        for ( auto &arg : fn->arguments ) {
            if ( arg->type->constant && arg->type->ref && !arg->type->isArray() ) {
                if ( arg->type->baseType == Type::tFloat2 || arg->type->baseType == Type::tFloat4 ) {
                    arg->type->ref = false;
                }
            }
            if ( arg->type->isString() && !arg->type->ref ) {
                anyString = true;
            }
        }

        if (anyString) {
            fn->needStringCast = true;
        }

        return true;
    }
    bool tempArgs = false;
    bool implicitArgs = true;
    bool tempResult = false;
    bool explicitConstArgs = false;
};

}
