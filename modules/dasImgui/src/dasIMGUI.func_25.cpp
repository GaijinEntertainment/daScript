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
void Module_dasIMGUI::initFunctions_25() {
	using _method_101 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int),&ImDrawList::PathArcToFast >;
// from imgui.h:2922:21
	makeExtern<DAS_CALL_METHOD(_method_101), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathArcToFast","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int) , &ImDrawList::PathArcToFast >::invoke")
		->args({"self","center","radius","a_min_of_12","a_max_of_12"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_102 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,float,float,int),&ImDrawList::PathEllipticalArcTo >;
// from imgui.h:2923:21
	makeExtern<DAS_CALL_METHOD(_method_102), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathEllipticalArcTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,float,float,int) , &ImDrawList::PathEllipticalArcTo >::invoke")
		->args({"self","center","radius","rot","a_min","a_max","num_segments"})
		->arg_init(6,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_103 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,int),&ImDrawList::PathBezierCubicCurveTo >;
// from imgui.h:2924:21
	makeExtern<DAS_CALL_METHOD(_method_103), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathBezierCubicCurveTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,int) , &ImDrawList::PathBezierCubicCurveTo >::invoke")
		->args({"self","p2","p3","p4","num_segments"})
		->arg_init(4,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_104 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,int),&ImDrawList::PathBezierQuadraticCurveTo >;
// from imgui.h:2925:21
	makeExtern<DAS_CALL_METHOD(_method_104), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathBezierQuadraticCurveTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,int) , &ImDrawList::PathBezierQuadraticCurveTo >::invoke")
		->args({"self","p2","p3","num_segments"})
		->arg_init(3,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_105 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,int),&ImDrawList::PathRect >;
// from imgui.h:2926:21
	makeExtern<DAS_CALL_METHOD(_method_105), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,int) , &ImDrawList::PathRect >::invoke")
		->args({"self","rect_min","rect_max","rounding","flags"})
		->arg_init(3,new ExprConstFloat(0))
		->arg_type(4,makeType<ImDrawFlags_>(lib))
		->arg_init(4,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_106 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::AddDrawCmd >;
// from imgui.h:2930:21
	makeExtern<DAS_CALL_METHOD(_method_106), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddDrawCmd","das_call_member< void (ImDrawList::*)() , &ImDrawList::AddDrawCmd >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_107 = das::das_call_member< ImDrawList * (ImDrawList::*)() const,&ImDrawList::CloneOutput >;
// from imgui.h:2931:27
	makeExtern<DAS_CALL_METHOD(_method_107), SimNode_ExtFuncCall , imguiTempFn>(lib,"CloneOutput","das_call_member< ImDrawList * (ImDrawList::*)() const , &ImDrawList::CloneOutput >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_108 = das::das_call_member< void (ImDrawList::*)(int),&ImDrawList::ChannelsSplit >;
// from imgui.h:2939:21
	makeExtern<DAS_CALL_METHOD(_method_108), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsSplit","das_call_member< void (ImDrawList::*)(int) , &ImDrawList::ChannelsSplit >::invoke")
		->args({"self","count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_109 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::ChannelsMerge >;
// from imgui.h:2940:21
	makeExtern<DAS_CALL_METHOD(_method_109), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsMerge","das_call_member< void (ImDrawList::*)() , &ImDrawList::ChannelsMerge >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_110 = das::das_call_member< void (ImDrawList::*)(int),&ImDrawList::ChannelsSetCurrent >;
// from imgui.h:2941:21
	makeExtern<DAS_CALL_METHOD(_method_110), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsSetCurrent","das_call_member< void (ImDrawList::*)(int) , &ImDrawList::ChannelsSetCurrent >::invoke")
		->args({"self","n"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_111 = das::das_call_member< void (ImDrawList::*)(int,int),&ImDrawList::PrimReserve >;
// from imgui.h:2946:21
	makeExtern<DAS_CALL_METHOD(_method_111), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimReserve","das_call_member< void (ImDrawList::*)(int,int) , &ImDrawList::PrimReserve >::invoke")
		->args({"self","idx_count","vtx_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_112 = das::das_call_member< void (ImDrawList::*)(int,int),&ImDrawList::PrimUnreserve >;
// from imgui.h:2947:21
	makeExtern<DAS_CALL_METHOD(_method_112), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimUnreserve","das_call_member< void (ImDrawList::*)(int,int) , &ImDrawList::PrimUnreserve >::invoke")
		->args({"self","idx_count","vtx_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_113 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimRect >;
// from imgui.h:2948:21
	makeExtern<DAS_CALL_METHOD(_method_113), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimRect >::invoke")
		->args({"self","a","b","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_114 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimRectUV >;
// from imgui.h:2949:21
	makeExtern<DAS_CALL_METHOD(_method_114), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimRectUV","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimRectUV >::invoke")
		->args({"self","a","b","uv_a","uv_b","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_115 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimQuadUV >;
// from imgui.h:2950:21
	makeExtern<DAS_CALL_METHOD(_method_115), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimQuadUV","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimQuadUV >::invoke")
		->args({"self","a","b","c","d","uv_a","uv_b","uv_c","uv_d","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_116 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimWriteVtx >;
// from imgui.h:2951:21
	makeExtern<DAS_CALL_METHOD(_method_116), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimWriteVtx","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimWriteVtx >::invoke")
		->args({"self","pos","uv","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_117 = das::das_call_member< void (ImDrawList::*)(unsigned short),&ImDrawList::PrimWriteIdx >;
// from imgui.h:2952:21
	makeExtern<DAS_CALL_METHOD(_method_117), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimWriteIdx","das_call_member< void (ImDrawList::*)(unsigned short) , &ImDrawList::PrimWriteIdx >::invoke")
		->args({"self","idx"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_118 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimVtx >;
// from imgui.h:2953:21
	makeExtern<DAS_CALL_METHOD(_method_118), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimVtx","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimVtx >::invoke")
		->args({"self","pos","uv","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_119 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_ResetForNewFrame >;
// from imgui.h:2963:21
	makeExtern<DAS_CALL_METHOD(_method_119), SimNode_ExtFuncCall , imguiTempFn>(lib,"_ResetForNewFrame","das_call_member< void (ImDrawList::*)() , &ImDrawList::_ResetForNewFrame >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_120 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::_ClearFreeMemory >;
// from imgui.h:2964:21
	makeExtern<DAS_CALL_METHOD(_method_120), SimNode_ExtFuncCall , imguiTempFn>(lib,"_ClearFreeMemory","das_call_member< void (ImDrawList::*)() , &ImDrawList::_ClearFreeMemory >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

