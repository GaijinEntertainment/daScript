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
void Module_dasIMGUI::initFunctions_30() {
	using _method_182 = das::das_call_member< void (ImFontAtlas::*)(int),&ImFontAtlas::RemoveCustomRect >;
// from imgui.h:3866:33
	makeExtern<DAS_CALL_METHOD(_method_182), SimNode_ExtFuncCall , imguiTempFn>(lib,"RemoveCustomRect","das_call_member< void (ImFontAtlas::*)(int) , &ImFontAtlas::RemoveCustomRect >::invoke")
		->args({"self","id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_183 = das::das_call_member< bool (ImFontAtlas::*)(int,ImFontAtlasRect *) const,&ImFontAtlas::GetCustomRect >;
// from imgui.h:3867:33
	makeExtern<DAS_CALL_METHOD(_method_183), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCustomRect","das_call_member< bool (ImFontAtlas::*)(int,ImFontAtlasRect *) const , &ImFontAtlas::GetCustomRect >::invoke")
		->args({"self","id","out_r"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFontBaked>(*this,lib,"ImFontBaked","ImFontBaked");
	using _method_184 = das::das_call_member< void (ImFontBaked::*)(),&ImFontBaked::ClearOutputData >;
// from imgui.h:3960:33
	makeExtern<DAS_CALL_METHOD(_method_184), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearOutputData","das_call_member< void (ImFontBaked::*)() , &ImFontBaked::ClearOutputData >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_185 = das::das_call_member< ImFontGlyph * (ImFontBaked::*)(unsigned int),&ImFontBaked::FindGlyph >;
// from imgui.h:3961:33
	makeExtern<DAS_CALL_METHOD(_method_185), SimNode_ExtFuncCall , imguiTempFn>(lib,"FindGlyph","das_call_member< ImFontGlyph * (ImFontBaked::*)(unsigned int) , &ImFontBaked::FindGlyph >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_186 = das::das_call_member< ImFontGlyph * (ImFontBaked::*)(unsigned int),&ImFontBaked::FindGlyphNoFallback >;
// from imgui.h:3962:33
	makeExtern<DAS_CALL_METHOD(_method_186), SimNode_ExtFuncCall , imguiTempFn>(lib,"FindGlyphNoFallback","das_call_member< ImFontGlyph * (ImFontBaked::*)(unsigned int) , &ImFontBaked::FindGlyphNoFallback >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_187 = das::das_call_member< float (ImFontBaked::*)(unsigned int),&ImFontBaked::GetCharAdvance >;
// from imgui.h:3963:33
	makeExtern<DAS_CALL_METHOD(_method_187), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetCharAdvance","das_call_member< float (ImFontBaked::*)(unsigned int) , &ImFontBaked::GetCharAdvance >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_188 = das::das_call_member< bool (ImFontBaked::*)(unsigned int),&ImFontBaked::IsGlyphLoaded >;
// from imgui.h:3964:33
	makeExtern<DAS_CALL_METHOD(_method_188), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsGlyphLoaded","das_call_member< bool (ImFontBaked::*)(unsigned int) , &ImFontBaked::IsGlyphLoaded >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFont>(*this,lib,"ImFont","ImFont");
	using _method_189 = das::das_call_member< bool (ImFont::*)(unsigned int),&ImFont::IsGlyphInFont >;
// from imgui.h:4007:33
	makeExtern<DAS_CALL_METHOD(_method_189), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsGlyphInFont","das_call_member< bool (ImFont::*)(unsigned int) , &ImFont::IsGlyphInFont >::invoke")
		->args({"self","c"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_190 = das::das_call_member< bool (ImFont::*)() const,&ImFont::IsLoaded >;
// from imgui.h:4008:33
	makeExtern<DAS_CALL_METHOD(_method_190), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsLoaded","das_call_member< bool (ImFont::*)() const , &ImFont::IsLoaded >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_191 = das::das_call_member< const char * (ImFont::*)() const,&ImFont::GetDebugName >;
// from imgui.h:4009:33
	makeExtern<DAS_CALL_METHOD(_method_191), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetDebugName","das_call_member< const char * (ImFont::*)() const , &ImFont::GetDebugName >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_192 = das::das_call_member< ImFontBaked * (ImFont::*)(float,float),&ImFont::GetFontBaked >;
// from imgui.h:4014:33
	makeExtern<DAS_CALL_METHOD(_method_192), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetFontBaked","das_call_member< ImFontBaked * (ImFont::*)(float,float) , &ImFont::GetFontBaked >::invoke")
		->args({"self","font_size","density"})
		->arg_init(2,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_193 = das::das_call_member< ImVec2 (ImFont::*)(float,float,float,const char *,const char *,const char **),&ImFont::CalcTextSizeA >;
// from imgui.h:4015:33
	makeExtern<DAS_CALL_METHOD(_method_193), SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcTextSizeA","das_call_member< ImVec2 (ImFont::*)(float,float,float,const char *,const char *,const char **) , &ImFont::CalcTextSizeA >::invoke")
		->args({"self","size","max_width","wrap_width","text_begin","text_end","out_remaining"})
		->arg_init(5,new ExprConstString(""))
		->arg_init(6,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_194 = das::das_call_member< const char * (ImFont::*)(float,const char *,const char *,float),&ImFont::CalcWordWrapPosition >;
// from imgui.h:4016:33
	makeExtern<DAS_CALL_METHOD(_method_194), SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcWordWrapPosition","das_call_member< const char * (ImFont::*)(float,const char *,const char *,float) , &ImFont::CalcWordWrapPosition >::invoke")
		->args({"self","size","text","text_end","wrap_width"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_195 = das::das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,unsigned int,const ImVec4 *),&ImFont::RenderChar >;
// from imgui.h:4017:33
	makeExtern<DAS_CALL_METHOD(_method_195), SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderChar","das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,unsigned int,const ImVec4 *) , &ImFont::RenderChar >::invoke")
		->args({"self","draw_list","size","pos","col","c","cpu_fine_clip"})
		->arg_init(6,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_196 = das::das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,const ImVec4 &,const char *,const char *,float,int),&ImFont::RenderText >;
// from imgui.h:4018:33
	makeExtern<DAS_CALL_METHOD(_method_196), SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderText","das_call_member< void (ImFont::*)(ImDrawList *,float,const ImVec2 &,unsigned int,const ImVec4 &,const char *,const char *,float,int) , &ImFont::RenderText >::invoke")
		->args({"self","draw_list","size","pos","col","clip_rect","text_begin","text_end","wrap_width","flags"})
		->arg_init(8,new ExprConstFloat(0))
		->arg_init(9,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_197 = das::das_call_member< void (ImFont::*)(),&ImFont::ClearOutputData >;
// from imgui.h:4024:33
	makeExtern<DAS_CALL_METHOD(_method_197), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearOutputData","das_call_member< void (ImFont::*)() , &ImFont::ClearOutputData >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_198 = das::das_call_member< void (ImFont::*)(unsigned int,unsigned int),&ImFont::AddRemapChar >;
// from imgui.h:4025:33
	makeExtern<DAS_CALL_METHOD(_method_198), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRemapChar","das_call_member< void (ImFont::*)(unsigned int,unsigned int) , &ImFont::AddRemapChar >::invoke")
		->args({"self","from_codepoint","to_codepoint"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_199 = das::das_call_member< bool (ImFont::*)(unsigned int,unsigned int),&ImFont::IsGlyphRangeUnused >;
// from imgui.h:4026:33
	makeExtern<DAS_CALL_METHOD(_method_199), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsGlyphRangeUnused","das_call_member< bool (ImFont::*)(unsigned int,unsigned int) , &ImFont::IsGlyphRangeUnused >::invoke")
		->args({"self","c_begin","c_last"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

