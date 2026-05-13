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
void Module_dasIMGUI::initFunctions_29() {
	using _method_175 = das::das_call_member< ImVec2 (ImFont::*)(float,float,float,const char *,const char *,const char **) const,&ImFont::CalcTextSizeA >;
// from imgui.h:3239:33
	makeExtern<DAS_CALL_METHOD(_method_175), SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcTextSizeA","das_call_member< ImVec2 (ImFont::*)(float,float,float,const char *,const char *,const char **) const , &ImFont::CalcTextSizeA >::invoke")
		->args({"self","size","max_width","wrap_width","text_begin","text_end","remaining"})
		->arg_init(5,new ExprConstString(""))
		->arg_init(6,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_176 = das::das_call_member< const char * (ImFont::*)(float,const char *,const char *,float) const,&ImFont::CalcWordWrapPositionA >;
// from imgui.h:3240:33
	makeExtern<DAS_CALL_METHOD(_method_176), SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcWordWrapPositionA","das_call_member< const char * (ImFont::*)(float,const char *,const char *,float) const , &ImFont::CalcWordWrapPositionA >::invoke")
		->args({"self","scale","text","text_end","wrap_width"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_177 = das::das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,unsigned short) const,&ImFont::RenderChar >;
// from imgui.h:3241:33
	makeExtern<DAS_CALL_METHOD(_method_177), SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderChar","das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,unsigned short) const , &ImFont::RenderChar >::invoke")
		->args({"self","draw_list","size","pos","col","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_178 = das::das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,const ImVec4 &,const char *,const char *,float,bool) const,&ImFont::RenderText >;
// from imgui.h:3242:33
	makeExtern<DAS_CALL_METHOD(_method_178), SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderText","das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,const ImVec4 &,const char *,const char *,float,bool) const , &ImFont::RenderText >::invoke")
		->args({"self","draw_list","size","pos","col","clip_rect","text_begin","text_end","wrap_width","cpu_fine_clip"})
		->arg_init(8,new ExprConstFloat(0))
		->arg_init(9,new ExprConstBool(false))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_179 = das::das_call_member< void (ImFont::*)(),&ImFont::BuildLookupTable >;
// from imgui.h:3245:33
	makeExtern<DAS_CALL_METHOD(_method_179), SimNode_ExtFuncCall , imguiTempFn>(lib,"BuildLookupTable","das_call_member< void (ImFont::*)() , &ImFont::BuildLookupTable >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_180 = das::das_call_member< void (ImFont::*)(),&ImFont::ClearOutputData >;
// from imgui.h:3246:33
	makeExtern<DAS_CALL_METHOD(_method_180), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearOutputData","das_call_member< void (ImFont::*)() , &ImFont::ClearOutputData >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_181 = das::das_call_member< void (ImFont::*)(int),&ImFont::GrowIndex >;
// from imgui.h:3247:33
	makeExtern<DAS_CALL_METHOD(_method_181), SimNode_ExtFuncCall , imguiTempFn>(lib,"GrowIndex","das_call_member< void (ImFont::*)(int) , &ImFont::GrowIndex >::invoke")
		->args({"self","new_size"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_182 = das::das_call_member< void (ImFont::*)(const ImFontConfig *,unsigned short,float,float,float,float,float,float,float,float,float),&ImFont::AddGlyph >;
// from imgui.h:3248:33
	makeExtern<DAS_CALL_METHOD(_method_182), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddGlyph","das_call_member< void (ImFont::*)(const ImFontConfig *,unsigned short,float,float,float,float,float,float,float,float,float) , &ImFont::AddGlyph >::invoke")
		->args({"self","src_cfg","c","x0","y0","x1","y1","u0","v0","u1","v1","advance_x"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_183 = das::das_call_member< void (ImFont::*)(unsigned short,unsigned short,bool),&ImFont::AddRemapChar >;
// from imgui.h:3249:33
	makeExtern<DAS_CALL_METHOD(_method_183), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRemapChar","das_call_member< void (ImFont::*)(unsigned short,unsigned short,bool) , &ImFont::AddRemapChar >::invoke")
		->args({"self","dst","src","overwrite_dst"})
		->arg_init(3,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_184 = das::das_call_member< void (ImFont::*)(unsigned short,bool),&ImFont::SetGlyphVisible >;
// from imgui.h:3250:33
	makeExtern<DAS_CALL_METHOD(_method_184), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetGlyphVisible","das_call_member< void (ImFont::*)(unsigned short,bool) , &ImFont::SetGlyphVisible >::invoke")
		->args({"self","c","visible"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_185 = das::das_call_member< bool (ImFont::*)(unsigned int,unsigned int),&ImFont::IsGlyphRangeUnused >;
// from imgui.h:3251:33
	makeExtern<DAS_CALL_METHOD(_method_185), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsGlyphRangeUnused","das_call_member< bool (ImFont::*)(unsigned int,unsigned int) , &ImFont::IsGlyphRangeUnused >::invoke")
		->args({"self","c_begin","c_last"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiViewport>(*this,lib,"ImGuiViewport","ImGuiViewport");
	using _method_186 = das::das_call_member< ImVec2 (ImGuiViewport::*)() const,&ImGuiViewport::GetCenter >;
// from imgui.h:3317:25
	makeExtern<DAS_CALL_METHOD(_method_186), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCenter","das_call_member< ImVec2 (ImGuiViewport::*)() const , &ImGuiViewport::GetCenter >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_187 = das::das_call_member< ImVec2 (ImGuiViewport::*)() const,&ImGuiViewport::GetWorkCenter >;
// from imgui.h:3318:25
	makeExtern<DAS_CALL_METHOD(_method_187), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetWorkCenter","das_call_member< ImVec2 (ImGuiViewport::*)() const , &ImGuiViewport::GetWorkCenter >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImGuiPlatformIO>(*this,lib,"ImGuiPlatformIO","ImGuiPlatformIO");
	addCtorAndUsing<ImGuiPlatformMonitor>(*this,lib,"ImGuiPlatformMonitor","ImGuiPlatformMonitor");
	addCtorAndUsing<ImGuiPlatformImeData>(*this,lib,"ImGuiPlatformImeData","ImGuiPlatformImeData");
}
}

