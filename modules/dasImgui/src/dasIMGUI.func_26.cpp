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
void Module_dasIMGUI::initFunctions_26() {
	using _method_121 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_PopUnusedDrawCmd >;
// from imgui.h:2965:21
	makeExtern<DAS_CALL_METHOD(_method_121), SimNode_ExtFuncCall , imguiTempFn>(lib,"_PopUnusedDrawCmd","das_call_member< void (ImDrawList::*)() , &ImDrawList::_PopUnusedDrawCmd >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_122 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_TryMergeDrawCmds >;
// from imgui.h:2966:21
	makeExtern<DAS_CALL_METHOD(_method_122), SimNode_ExtFuncCall , imguiTempFn>(lib,"_TryMergeDrawCmds","das_call_member< void (ImDrawList::*)() , &ImDrawList::_TryMergeDrawCmds >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_123 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_OnChangedClipRect >;
// from imgui.h:2967:21
	makeExtern<DAS_CALL_METHOD(_method_123), SimNode_ExtFuncCall , imguiTempFn>(lib,"_OnChangedClipRect","das_call_member< void (ImDrawList::*)() , &ImDrawList::_OnChangedClipRect >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_124 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_OnChangedTextureID >;
// from imgui.h:2968:21
	makeExtern<DAS_CALL_METHOD(_method_124), SimNode_ExtFuncCall , imguiTempFn>(lib,"_OnChangedTextureID","das_call_member< void (ImDrawList::*)() , &ImDrawList::_OnChangedTextureID >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_125 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_OnChangedVtxOffset >;
// from imgui.h:2969:21
	makeExtern<DAS_CALL_METHOD(_method_125), SimNode_ExtFuncCall , imguiTempFn>(lib,"_OnChangedVtxOffset","das_call_member< void (ImDrawList::*)() , &ImDrawList::_OnChangedVtxOffset >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_126 = das::das_call_member< int (ImDrawList::*)(float) const,&ImDrawList::_CalcCircleAutoSegmentCount >;
// from imgui.h:2970:21
	makeExtern<DAS_CALL_METHOD(_method_126), SimNode_ExtFuncCall , imguiTempFn>(lib,"_CalcCircleAutoSegmentCount","das_call_member< int (ImDrawList::*)(float) const , &ImDrawList::_CalcCircleAutoSegmentCount >::invoke")
		->args({"self","radius"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_127 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int,int),&ImDrawList::_PathArcToFastEx >;
// from imgui.h:2971:21
	makeExtern<DAS_CALL_METHOD(_method_127), SimNode_ExtFuncCall , imguiTempFn>(lib,"_PathArcToFastEx","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int,int) , &ImDrawList::_PathArcToFastEx >::invoke")
		->args({"self","center","radius","a_min_sample","a_max_sample","a_step"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_128 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,float,float,int),&ImDrawList::_PathArcToN >;
// from imgui.h:2972:21
	makeExtern<DAS_CALL_METHOD(_method_128), SimNode_ExtFuncCall , imguiTempFn>(lib,"_PathArcToN","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,float,float,int) , &ImDrawList::_PathArcToN >::invoke")
		->args({"self","center","radius","a_min","a_max","num_segments"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImDrawData>(*this,lib,"ImDrawData","ImDrawData");
	using _method_129 = das::das_call_member< void (ImDrawData::*)(),&ImDrawData::Clear >;
// from imgui.h:2992:21
	makeExtern<DAS_CALL_METHOD(_method_129), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImDrawData::*)() , &ImDrawData::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_130 = das::das_call_member< void (ImDrawData::*)(ImDrawList *),&ImDrawData::AddDrawList >;
// from imgui.h:2993:21
	makeExtern<DAS_CALL_METHOD(_method_130), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddDrawList","das_call_member< void (ImDrawData::*)(ImDrawList *) , &ImDrawData::AddDrawList >::invoke")
		->args({"self","draw_list"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_131 = das::das_call_member< void (ImDrawData::*)(),&ImDrawData::DeIndexAllBuffers >;
// from imgui.h:2994:21
	makeExtern<DAS_CALL_METHOD(_method_131), SimNode_ExtFuncCall , imguiTempFn>(lib,"DeIndexAllBuffers","das_call_member< void (ImDrawData::*)() , &ImDrawData::DeIndexAllBuffers >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_132 = das::das_call_member< void (ImDrawData::*)(const ImVec2 &),&ImDrawData::ScaleClipRects >;
// from imgui.h:2995:21
	makeExtern<DAS_CALL_METHOD(_method_132), SimNode_ExtFuncCall , imguiTempFn>(lib,"ScaleClipRects","das_call_member< void (ImDrawData::*)(const ImVec2 &) , &ImDrawData::ScaleClipRects >::invoke")
		->args({"self","fb_scale"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFontConfig>(*this,lib,"ImFontConfig","ImFontConfig");
	addCtorAndUsing<ImFontGlyphRangesBuilder>(*this,lib,"ImFontGlyphRangesBuilder","ImFontGlyphRangesBuilder");
	using _method_133 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(),&ImFontGlyphRangesBuilder::Clear >;
// from imgui.h:3049:21
	makeExtern<DAS_CALL_METHOD(_method_133), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImFontGlyphRangesBuilder::*)() , &ImFontGlyphRangesBuilder::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_134 = das::das_call_member< bool (ImFontGlyphRangesBuilder::*)(size_t) const,&ImFontGlyphRangesBuilder::GetBit >;
// from imgui.h:3050:21
	makeExtern<DAS_CALL_METHOD(_method_134), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetBit","das_call_member< bool (ImFontGlyphRangesBuilder::*)(size_t) const , &ImFontGlyphRangesBuilder::GetBit >::invoke")
		->args({"self","n"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_135 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(size_t),&ImFontGlyphRangesBuilder::SetBit >;
// from imgui.h:3051:21
	makeExtern<DAS_CALL_METHOD(_method_135), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetBit","das_call_member< void (ImFontGlyphRangesBuilder::*)(size_t) , &ImFontGlyphRangesBuilder::SetBit >::invoke")
		->args({"self","n"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_136 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(unsigned short),&ImFontGlyphRangesBuilder::AddChar >;
// from imgui.h:3052:21
	makeExtern<DAS_CALL_METHOD(_method_136), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddChar","das_call_member< void (ImFontGlyphRangesBuilder::*)(unsigned short) , &ImFontGlyphRangesBuilder::AddChar >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_137 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(const char *,const char *),&ImFontGlyphRangesBuilder::AddText >;
// from imgui.h:3053:21
	makeExtern<DAS_CALL_METHOD(_method_137), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddText","das_call_member< void (ImFontGlyphRangesBuilder::*)(const char *,const char *) , &ImFontGlyphRangesBuilder::AddText >::invoke")
		->args({"self","text","text_end"})
		->arg_init(2,new ExprConstString(""))
		->addToModule(*this, SideEffects::worstDefault);
}
}

