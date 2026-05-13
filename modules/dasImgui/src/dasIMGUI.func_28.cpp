// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasIMGUI.h"
#include "need_dasIMGUI.h"
namespace das {
#include "dasIMGUI.func.aot.decl.inc"
void Module_dasIMGUI::initFunctions_28() {
	using _method_156 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesDefault >;
// from imgui.h:3131:33
	makeExtern<DAS_CALL_METHOD(_method_156), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesDefault","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesDefault >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_157 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesGreek >;
// from imgui.h:3132:33
	makeExtern<DAS_CALL_METHOD(_method_157), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesGreek","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesGreek >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_158 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesKorean >;
// from imgui.h:3133:33
	makeExtern<DAS_CALL_METHOD(_method_158), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesKorean","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesKorean >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_159 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesJapanese >;
// from imgui.h:3134:33
	makeExtern<DAS_CALL_METHOD(_method_159), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesJapanese","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesJapanese >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_160 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesChineseFull >;
// from imgui.h:3135:33
	makeExtern<DAS_CALL_METHOD(_method_160), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesChineseFull","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesChineseFull >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_161 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesChineseSimplifiedCommon >;
// from imgui.h:3136:33
	makeExtern<DAS_CALL_METHOD(_method_161), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesChineseSimplifiedCommon","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesChineseSimplifiedCommon >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_162 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesCyrillic >;
// from imgui.h:3137:33
	makeExtern<DAS_CALL_METHOD(_method_162), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesCyrillic","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesCyrillic >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_163 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesThai >;
// from imgui.h:3138:33
	makeExtern<DAS_CALL_METHOD(_method_163), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesThai","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesThai >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_164 = das::das_call_member< const unsigned short * (ImFontAtlas::*)(),&ImFontAtlas::GetGlyphRangesVietnamese >;
// from imgui.h:3139:33
	makeExtern<DAS_CALL_METHOD(_method_164), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetGlyphRangesVietnamese","das_call_member< const unsigned short * (ImFontAtlas::*)() , &ImFontAtlas::GetGlyphRangesVietnamese >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_165 = das::das_call_member< int (ImFontAtlas::*)(int,int),&ImFontAtlas::AddCustomRectRegular >;
// from imgui.h:3152:33
	makeExtern<DAS_CALL_METHOD(_method_165), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddCustomRectRegular","das_call_member< int (ImFontAtlas::*)(int,int) , &ImFontAtlas::AddCustomRectRegular >::invoke")
		->args({"self","width","height"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_166 = das::das_call_member< int (ImFontAtlas::*)(ImFont *,unsigned short,int,int,float,const ImVec2 &),&ImFontAtlas::AddCustomRectFontGlyph >;
// from imgui.h:3153:33
	makeExtern<DAS_CALL_METHOD(_method_166), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddCustomRectFontGlyph","das_call_member< int (ImFontAtlas::*)(ImFont *,unsigned short,int,int,float,const ImVec2 &) , &ImFontAtlas::AddCustomRectFontGlyph >::invoke")
		->args({"self","font","id","width","height","advance_x","offset"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_167 = das::das_call_member< ImFontAtlasCustomRect * (ImFontAtlas::*)(int),&ImFontAtlas::GetCustomRectByIndex >;
// from imgui.h:3154:33
	makeExtern<DAS_CALL_METHOD(_method_167), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCustomRectByIndex","das_call_member< ImFontAtlasCustomRect * (ImFontAtlas::*)(int) , &ImFontAtlas::GetCustomRectByIndex >::invoke")
		->args({"self","index"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_168 = das::das_call_member< void (ImFontAtlas::*)(const ImFontAtlasCustomRect *,ImVec2 *,ImVec2 *) const,&ImFontAtlas::CalcCustomRectUV >;
// from imgui.h:3157:33
	makeExtern<DAS_CALL_METHOD(_method_168), SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcCustomRectUV","das_call_member< void (ImFontAtlas::*)(const ImFontAtlasCustomRect *,ImVec2 *,ImVec2 *) const , &ImFontAtlas::CalcCustomRectUV >::invoke")
		->args({"self","rect","out_uv_min","out_uv_max"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_169 = das::das_call_member< bool (ImFontAtlas::*)(int,ImVec2 *,ImVec2 *,ImVec2[2],ImVec2[2]),&ImFontAtlas::GetMouseCursorTexData >;
// from imgui.h:3158:33
	makeExtern<DAS_CALL_METHOD(_method_169), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMouseCursorTexData","das_call_member< bool (ImFontAtlas::*)(int,ImVec2 *,ImVec2 *,ImVec2[2],ImVec2[2]) , &ImFontAtlas::GetMouseCursorTexData >::invoke")
		->args({"self","cursor","out_offset","out_size","out_uv_border","out_uv_fill"})
		->arg_type(1,makeType<ImGuiMouseCursor_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFont>(*this,lib,"ImFont","ImFont");
	using _method_170 = das::das_call_member< const ImFontGlyph * (ImFont::*)(unsigned short) const,&ImFont::FindGlyph >;
// from imgui.h:3231:33
	makeExtern<DAS_CALL_METHOD(_method_170), SimNode_ExtFuncCall , imguiTempFn>(lib,"FindGlyph","das_call_member< const ImFontGlyph * (ImFont::*)(unsigned short) const , &ImFont::FindGlyph >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_171 = das::das_call_member< const ImFontGlyph * (ImFont::*)(unsigned short) const,&ImFont::FindGlyphNoFallback >;
// from imgui.h:3232:33
	makeExtern<DAS_CALL_METHOD(_method_171), SimNode_ExtFuncCall , imguiTempFn>(lib,"FindGlyphNoFallback","das_call_member< const ImFontGlyph * (ImFont::*)(unsigned short) const , &ImFont::FindGlyphNoFallback >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_172 = das::das_call_member< float (ImFont::*)(unsigned short) const,&ImFont::GetCharAdvance >;
// from imgui.h:3233:33
	makeExtern<DAS_CALL_METHOD(_method_172), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCharAdvance","das_call_member< float (ImFont::*)(unsigned short) const , &ImFont::GetCharAdvance >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_173 = das::das_call_member< bool (ImFont::*)() const,&ImFont::IsLoaded >;
// from imgui.h:3234:33
	makeExtern<DAS_CALL_METHOD(_method_173), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsLoaded","das_call_member< bool (ImFont::*)() const , &ImFont::IsLoaded >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_174 = das::das_call_member< const char * (ImFont::*)() const,&ImFont::GetDebugName >;
// from imgui.h:3235:33
	makeExtern<DAS_CALL_METHOD(_method_174), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetDebugName","das_call_member< const char * (ImFont::*)() const , &ImFont::GetDebugName >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

