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
	using _method_109 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &),&ImDrawList::PathLineToMergeDuplicate >;
// from imgui.h:3479:21
	makeExtern<DAS_CALL_METHOD(_method_109), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathLineToMergeDuplicate","das_call_member< void (ImDrawList::*)(const ImVec2 &) , &ImDrawList::PathLineToMergeDuplicate >::invoke")
		->args({"self","pos"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_110 = das::das_call_member< void (ImDrawList::*)(unsigned int),&ImDrawList::PathFillConvex >;
// from imgui.h:3480:21
	makeExtern<DAS_CALL_METHOD(_method_110), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathFillConvex","das_call_member< void (ImDrawList::*)(unsigned int) , &ImDrawList::PathFillConvex >::invoke")
		->args({"self","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_111 = das::das_call_member< void (ImDrawList::*)(unsigned int),&ImDrawList::PathFillConcave >;
// from imgui.h:3481:21
	makeExtern<DAS_CALL_METHOD(_method_111), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathFillConcave","das_call_member< void (ImDrawList::*)(unsigned int) , &ImDrawList::PathFillConcave >::invoke")
		->args({"self","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_112 = das::das_call_member< void (ImDrawList::*)(unsigned int,int,float),&ImDrawList::PathStroke >;
// from imgui.h:3482:21
	makeExtern<DAS_CALL_METHOD(_method_112), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathStroke","das_call_member< void (ImDrawList::*)(unsigned int,int,float) , &ImDrawList::PathStroke >::invoke")
		->args({"self","col","flags","thickness"})
		->arg_type(2,makeType<ImDrawFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->arg_init(3,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_113 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,float,float,int),&ImDrawList::PathArcTo >;
// from imgui.h:3483:21
	makeExtern<DAS_CALL_METHOD(_method_113), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathArcTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,float,float,int) , &ImDrawList::PathArcTo >::invoke")
		->args({"self","center","radius","a_min","a_max","num_segments"})
		->arg_init(5,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_114 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int),&ImDrawList::PathArcToFast >;
// from imgui.h:3484:21
	makeExtern<DAS_CALL_METHOD(_method_114), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathArcToFast","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,int,int) , &ImDrawList::PathArcToFast >::invoke")
		->args({"self","center","radius","a_min_of_12","a_max_of_12"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_115 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,float,float,int),&ImDrawList::PathEllipticalArcTo >;
// from imgui.h:3485:21
	makeExtern<DAS_CALL_METHOD(_method_115), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathEllipticalArcTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,float,float,int) , &ImDrawList::PathEllipticalArcTo >::invoke")
		->args({"self","center","radius","rot","a_min","a_max","num_segments"})
		->arg_init(6,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_116 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,int),&ImDrawList::PathBezierCubicCurveTo >;
// from imgui.h:3486:21
	makeExtern<DAS_CALL_METHOD(_method_116), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathBezierCubicCurveTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,int) , &ImDrawList::PathBezierCubicCurveTo >::invoke")
		->args({"self","p2","p3","p4","num_segments"})
		->arg_init(4,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_117 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,int),&ImDrawList::PathBezierQuadraticCurveTo >;
// from imgui.h:3487:21
	makeExtern<DAS_CALL_METHOD(_method_117), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathBezierQuadraticCurveTo","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,int) , &ImDrawList::PathBezierQuadraticCurveTo >::invoke")
		->args({"self","p2","p3","num_segments"})
		->arg_init(3,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_118 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,int),&ImDrawList::PathRect >;
// from imgui.h:3488:21
	makeExtern<DAS_CALL_METHOD(_method_118), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,float,int) , &ImDrawList::PathRect >::invoke")
		->args({"self","rect_min","rect_max","rounding","flags"})
		->arg_init(3,new ExprConstFloat(0))
		->arg_type(4,makeType<ImDrawFlags_>(lib))
		->arg_init(4,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_119 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::AddDrawCmd >;
// from imgui.h:3502:21
	makeExtern<DAS_CALL_METHOD(_method_119), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddDrawCmd","das_call_member< void (ImDrawList::*)() , &ImDrawList::AddDrawCmd >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_120 = das::das_call_member< ImDrawList * (ImDrawList::*)() const,&ImDrawList::CloneOutput >;
// from imgui.h:3503:27
	makeExtern<DAS_CALL_METHOD(_method_120), SimNode_ExtFuncCall , imguiTempFn>(lib,"CloneOutput","das_call_member< ImDrawList * (ImDrawList::*)() const , &ImDrawList::CloneOutput >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_121 = das::das_call_member< void (ImDrawList::*)(int),&ImDrawList::ChannelsSplit >;
// from imgui.h:3511:21
	makeExtern<DAS_CALL_METHOD(_method_121), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsSplit","das_call_member< void (ImDrawList::*)(int) , &ImDrawList::ChannelsSplit >::invoke")
		->args({"self","count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_122 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::ChannelsMerge >;
// from imgui.h:3512:21
	makeExtern<DAS_CALL_METHOD(_method_122), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsMerge","das_call_member< void (ImDrawList::*)() , &ImDrawList::ChannelsMerge >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_123 = das::das_call_member< void (ImDrawList::*)(int),&ImDrawList::ChannelsSetCurrent >;
// from imgui.h:3513:21
	makeExtern<DAS_CALL_METHOD(_method_123), SimNode_ExtFuncCall , imguiTempFn>(lib,"ChannelsSetCurrent","das_call_member< void (ImDrawList::*)(int) , &ImDrawList::ChannelsSetCurrent >::invoke")
		->args({"self","n"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_124 = das::das_call_member< void (ImDrawList::*)(int,int),&ImDrawList::PrimReserve >;
// from imgui.h:3518:21
	makeExtern<DAS_CALL_METHOD(_method_124), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimReserve","das_call_member< void (ImDrawList::*)(int,int) , &ImDrawList::PrimReserve >::invoke")
		->args({"self","idx_count","vtx_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_125 = das::das_call_member< void (ImDrawList::*)(int,int),&ImDrawList::PrimUnreserve >;
// from imgui.h:3519:21
	makeExtern<DAS_CALL_METHOD(_method_125), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimUnreserve","das_call_member< void (ImDrawList::*)(int,int) , &ImDrawList::PrimUnreserve >::invoke")
		->args({"self","idx_count","vtx_count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_126 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimRect >;
// from imgui.h:3520:21
	makeExtern<DAS_CALL_METHOD(_method_126), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimRect >::invoke")
		->args({"self","a","b","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_127 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimRectUV >;
// from imgui.h:3521:21
	makeExtern<DAS_CALL_METHOD(_method_127), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimRectUV","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimRectUV >::invoke")
		->args({"self","a","b","uv_a","uv_b","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_128 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::PrimQuadUV >;
// from imgui.h:3522:21
	makeExtern<DAS_CALL_METHOD(_method_128), SimNode_ExtFuncCall , imguiTempFn>(lib,"PrimQuadUV","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::PrimQuadUV >::invoke")
		->args({"self","a","b","c","d","uv_a","uv_b","uv_c","uv_d","col"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

