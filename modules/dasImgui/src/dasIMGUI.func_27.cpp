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
void Module_dasIMGUI::initFunctions_27() {
	using _method_138 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(const unsigned short *),&ImFontGlyphRangesBuilder::AddRanges >;
// from imgui.h:3054:21
	makeExtern<DAS_CALL_METHOD(_method_138), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRanges","das_call_member< void (ImFontGlyphRangesBuilder::*)(const unsigned short *) , &ImFontGlyphRangesBuilder::AddRanges >::invoke")
		->args({"self","ranges"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_139 = das::das_call_member< void (ImFontGlyphRangesBuilder::*)(ImVector<unsigned short> *),&ImFontGlyphRangesBuilder::BuildRanges >;
// from imgui.h:3055:21
	makeExtern<DAS_CALL_METHOD(_method_139), SimNode_ExtFuncCall , imguiTempFn>(lib,"BuildRanges","das_call_member< void (ImFontGlyphRangesBuilder::*)(ImVector<unsigned short> *) , &ImFontGlyphRangesBuilder::BuildRanges >::invoke")
		->args({"self","out_ranges"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFontAtlasCustomRect>(*this,lib,"ImFontAtlasCustomRect","ImFontAtlasCustomRect");
	using _method_140 = das::das_call_member< bool (ImFontAtlasCustomRect::*)() const,&ImFontAtlasCustomRect::IsPacked >;
// from imgui.h:3068:10
	makeExtern<DAS_CALL_METHOD(_method_140), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsPacked","das_call_member< bool (ImFontAtlasCustomRect::*)() const , &ImFontAtlasCustomRect::IsPacked >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImFontAtlas>(*this,lib,"ImFontAtlas","ImFontAtlas");
	using _method_141 = das::das_call_member< ImFont * (ImFontAtlas::*)(const ImFontConfig *),&ImFontAtlas::AddFont >;
// from imgui.h:3101:33
	makeExtern<DAS_CALL_METHOD(_method_141), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFont","das_call_member< ImFont * (ImFontAtlas::*)(const ImFontConfig *) , &ImFontAtlas::AddFont >::invoke")
		->args({"self","font_cfg"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_142 = das::das_call_member< ImFont * (ImFontAtlas::*)(const ImFontConfig *),&ImFontAtlas::AddFontDefault >;
// from imgui.h:3102:33
	makeExtern<DAS_CALL_METHOD(_method_142), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFontDefault","das_call_member< ImFont * (ImFontAtlas::*)(const ImFontConfig *) , &ImFontAtlas::AddFontDefault >::invoke")
		->args({"self","font_cfg"})
		->arg_init(1,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_143 = das::das_call_member< ImFont * (ImFontAtlas::*)(const char *,float,const ImFontConfig *,const unsigned short *),&ImFontAtlas::AddFontFromFileTTF >;
// from imgui.h:3103:33
	makeExtern<DAS_CALL_METHOD(_method_143), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFontFromFileTTF","das_call_member< ImFont * (ImFontAtlas::*)(const char *,float,const ImFontConfig *,const unsigned short *) , &ImFontAtlas::AddFontFromFileTTF >::invoke")
		->args({"self","filename","size_pixels","font_cfg","glyph_ranges"})
		->arg_init(3,new ExprConstPtr())
		->arg_init(4,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_144 = das::das_call_member< ImFont * (ImFontAtlas::*)(void *,int,float,const ImFontConfig *,const unsigned short *),&ImFontAtlas::AddFontFromMemoryTTF >;
// from imgui.h:3104:33
	makeExtern<DAS_CALL_METHOD(_method_144), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFontFromMemoryTTF","das_call_member< ImFont * (ImFontAtlas::*)(void *,int,float,const ImFontConfig *,const unsigned short *) , &ImFontAtlas::AddFontFromMemoryTTF >::invoke")
		->args({"self","font_data","font_data_size","size_pixels","font_cfg","glyph_ranges"})
		->arg_init(4,new ExprConstPtr())
		->arg_init(5,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_145 = das::das_call_member< ImFont * (ImFontAtlas::*)(const void *,int,float,const ImFontConfig *,const unsigned short *),&ImFontAtlas::AddFontFromMemoryCompressedTTF >;
// from imgui.h:3105:33
	makeExtern<DAS_CALL_METHOD(_method_145), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFontFromMemoryCompressedTTF","das_call_member< ImFont * (ImFontAtlas::*)(const void *,int,float,const ImFontConfig *,const unsigned short *) , &ImFontAtlas::AddFontFromMemoryCompressedTTF >::invoke")
		->args({"self","compressed_font_data","compressed_font_data_size","size_pixels","font_cfg","glyph_ranges"})
		->arg_init(4,new ExprConstPtr())
		->arg_init(5,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_146 = das::das_call_member< ImFont * (ImFontAtlas::*)(const char *,float,const ImFontConfig *,const unsigned short *),&ImFontAtlas::AddFontFromMemoryCompressedBase85TTF >;
// from imgui.h:3106:33
	makeExtern<DAS_CALL_METHOD(_method_146), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddFontFromMemoryCompressedBase85TTF","das_call_member< ImFont * (ImFontAtlas::*)(const char *,float,const ImFontConfig *,const unsigned short *) , &ImFontAtlas::AddFontFromMemoryCompressedBase85TTF >::invoke")
		->args({"self","compressed_font_data_base85","size_pixels","font_cfg","glyph_ranges"})
		->arg_init(3,new ExprConstPtr())
		->arg_init(4,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_147 = das::das_call_member< void (ImFontAtlas::*)(),&ImFontAtlas::ClearInputData >;
// from imgui.h:3107:33
	makeExtern<DAS_CALL_METHOD(_method_147), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearInputData","das_call_member< void (ImFontAtlas::*)() , &ImFontAtlas::ClearInputData >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_148 = das::das_call_member< void (ImFontAtlas::*)(),&ImFontAtlas::ClearTexData >;
// from imgui.h:3108:33
	makeExtern<DAS_CALL_METHOD(_method_148), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearTexData","das_call_member< void (ImFontAtlas::*)() , &ImFontAtlas::ClearTexData >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_149 = das::das_call_member< void (ImFontAtlas::*)(),&ImFontAtlas::ClearFonts >;
// from imgui.h:3109:33
	makeExtern<DAS_CALL_METHOD(_method_149), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearFonts","das_call_member< void (ImFontAtlas::*)() , &ImFontAtlas::ClearFonts >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_150 = das::das_call_member< void (ImFontAtlas::*)(),&ImFontAtlas::Clear >;
// from imgui.h:3110:33
	makeExtern<DAS_CALL_METHOD(_method_150), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImFontAtlas::*)() , &ImFontAtlas::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_151 = das::das_call_member< bool (ImFontAtlas::*)(),&ImFontAtlas::Build >;
// from imgui.h:3117:33
	makeExtern<DAS_CALL_METHOD(_method_151), SimNode_ExtFuncCall , imguiTempFn>(lib,"Build","das_call_member< bool (ImFontAtlas::*)() , &ImFontAtlas::Build >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_152 = das::das_call_member< void (ImFontAtlas::*)(unsigned char **,int *,int *,int *),&ImFontAtlas::GetTexDataAsAlpha8 >;
// from imgui.h:3118:33
	makeExtern<DAS_CALL_METHOD(_method_152), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTexDataAsAlpha8","das_call_member< void (ImFontAtlas::*)(unsigned char **,int *,int *,int *) , &ImFontAtlas::GetTexDataAsAlpha8 >::invoke")
		->args({"self","out_pixels","out_width","out_height","out_bytes_per_pixel"})
		->arg_init(4,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_153 = das::das_call_member< void (ImFontAtlas::*)(unsigned char **,int *,int *,int *),&ImFontAtlas::GetTexDataAsRGBA32 >;
// from imgui.h:3119:33
	makeExtern<DAS_CALL_METHOD(_method_153), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTexDataAsRGBA32","das_call_member< void (ImFontAtlas::*)(unsigned char **,int *,int *,int *) , &ImFontAtlas::GetTexDataAsRGBA32 >::invoke")
		->args({"self","out_pixels","out_width","out_height","out_bytes_per_pixel"})
		->arg_init(4,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
	using _method_154 = das::das_call_member< bool (ImFontAtlas::*)() const,&ImFontAtlas::IsBuilt >;
// from imgui.h:3120:33
	makeExtern<DAS_CALL_METHOD(_method_154), SimNode_ExtFuncCall , imguiTempFn>(lib,"IsBuilt","das_call_member< bool (ImFontAtlas::*)() const , &ImFontAtlas::IsBuilt >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_155 = das::das_call_member< void (ImFontAtlas::*)(void *),&ImFontAtlas::SetTexID >;
// from imgui.h:3121:33
	makeExtern<DAS_CALL_METHOD(_method_155), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetTexID","das_call_member< void (ImFontAtlas::*)(void *) , &ImFontAtlas::SetTexID >::invoke")
		->args({"self","id"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

