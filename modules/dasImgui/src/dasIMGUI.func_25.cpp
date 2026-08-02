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
	using _method_89 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddQuad >;
// from imgui.h:3444:21
	makeExtern<DAS_CALL_METHOD(_method_89), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddQuad","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddQuad >::invoke")
		->args({"self","p1","p2","p3","p4","col","thickness"})
		->arg_init(6,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_90 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddQuadFilled >;
// from imgui.h:3445:21
	makeExtern<DAS_CALL_METHOD(_method_90), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddQuadFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddQuadFilled >::invoke")
		->args({"self","p1","p2","p3","p4","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_91 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddTriangle >;
// from imgui.h:3446:21
	makeExtern<DAS_CALL_METHOD(_method_91), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddTriangle","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddTriangle >::invoke")
		->args({"self","p1","p2","p3","col","thickness"})
		->arg_init(5,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_92 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddTriangleFilled >;
// from imgui.h:3447:21
	makeExtern<DAS_CALL_METHOD(_method_92), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddTriangleFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddTriangleFilled >::invoke")
		->args({"self","p1","p2","p3","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_93 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float),&ImDrawList::AddCircle >;
// from imgui.h:3448:21
	makeExtern<DAS_CALL_METHOD(_method_93), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddCircle","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float) , &ImDrawList::AddCircle >::invoke")
		->args({"self","center","radius","col","num_segments","thickness"})
		->arg_init(4,new ExprConstInt(0))
		->arg_init(5,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_94 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int),&ImDrawList::AddCircleFilled >;
// from imgui.h:3449:21
	makeExtern<DAS_CALL_METHOD(_method_94), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddCircleFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int) , &ImDrawList::AddCircleFilled >::invoke")
		->args({"self","center","radius","col","num_segments"})
		->arg_init(4,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_95 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float),&ImDrawList::AddNgon >;
// from imgui.h:3450:21
	makeExtern<DAS_CALL_METHOD(_method_95), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddNgon","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float) , &ImDrawList::AddNgon >::invoke")
		->args({"self","center","radius","col","num_segments","thickness"})
		->arg_init(5,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_96 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int),&ImDrawList::AddNgonFilled >;
// from imgui.h:3451:21
	makeExtern<DAS_CALL_METHOD(_method_96), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddNgonFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int) , &ImDrawList::AddNgonFilled >::invoke")
		->args({"self","center","radius","col","num_segments"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_97 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float),&ImDrawList::AddEllipse >;
// from imgui.h:3452:21
	makeExtern<DAS_CALL_METHOD(_method_97), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddEllipse","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float) , &ImDrawList::AddEllipse >::invoke")
		->args({"self","center","radius","col","rot","num_segments","thickness"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_init(5,new ExprConstInt(0))
		->arg_init(6,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_98 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddEllipseFilled >;
// from imgui.h:3453:21
	makeExtern<DAS_CALL_METHOD(_method_98), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddEllipseFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddEllipseFilled >::invoke")
		->args({"self","center","radius","col","rot","num_segments"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_init(5,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_99 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddBezierCubic >;
// from imgui.h:3456:21
	makeExtern<DAS_CALL_METHOD(_method_99), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddBezierCubic","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddBezierCubic >::invoke")
		->args({"self","p1","p2","p3","p4","col","thickness","num_segments"})
		->arg_init(7,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_100 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddBezierQuadratic >;
// from imgui.h:3457:21
	makeExtern<DAS_CALL_METHOD(_method_100), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddBezierQuadratic","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddBezierQuadratic >::invoke")
		->args({"self","p1","p2","p3","col","thickness","num_segments"})
		->arg_init(6,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_101 = das::das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int,int,float),&ImDrawList::AddPolyline >;
// from imgui.h:3462:21
	makeExtern<DAS_CALL_METHOD(_method_101), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddPolyline","das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int,int,float) , &ImDrawList::AddPolyline >::invoke")
		->args({"self","points","num_points","col","flags","thickness"})
		->arg_type(4,makeType<ImDrawFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_102 = das::das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int),&ImDrawList::AddConvexPolyFilled >;
// from imgui.h:3463:21
	makeExtern<DAS_CALL_METHOD(_method_102), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddConvexPolyFilled","das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int) , &ImDrawList::AddConvexPolyFilled >::invoke")
		->args({"self","points","num_points","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_103 = das::das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int),&ImDrawList::AddConcavePolyFilled >;
// from imgui.h:3464:21
	makeExtern<DAS_CALL_METHOD(_method_103), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddConcavePolyFilled","das_call_member< void (ImDrawList::*)(const ImVec2 *,int,unsigned int) , &ImDrawList::AddConcavePolyFilled >::invoke")
		->args({"self","points","num_points","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_104 = das::das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddImage >;
// from imgui.h:3470:21
	makeExtern<DAS_CALL_METHOD(_method_104), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddImage","das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddImage >::invoke")
		->args({"self","tex_ref","p_min","p_max","uv_min","uv_max","col"})
		->arg_init(6,new ExprConstUInt(0xffffffff))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_105 = das::das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddImageQuad >;
// from imgui.h:3471:21
	makeExtern<DAS_CALL_METHOD(_method_105), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddImageQuad","das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddImageQuad >::invoke")
		->args({"self","tex_ref","p1","p2","p3","p4","uv1","uv2","uv3","uv4","col"})
		->arg_init(10,new ExprConstUInt(0xffffffff))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_106 = das::das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddImageRounded >;
// from imgui.h:3472:21
	makeExtern<DAS_CALL_METHOD(_method_106), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddImageRounded","das_call_member< void (ImDrawList::*)(ImTextureRef,const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddImageRounded >::invoke")
		->args({"self","tex_ref","p_min","p_max","uv_min","uv_max","col","rounding","flags"})
		->arg_type(8,makeType<ImDrawFlags_>(lib))
		->arg_init(8,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_107 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PathClear >;
// from imgui.h:3477:21
	makeExtern<DAS_CALL_METHOD(_method_107), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathClear","das_call_member< void (ImDrawList::*)() , &ImDrawList::PathClear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_108 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &),&ImDrawList::PathLineTo >;
// from imgui.h:3478:21
	makeExtern<DAS_CALL_METHOD(_method_108), SimNode_ExtFuncCall , imguiTempFn>(lib,"PathLineTo","das_call_member< void (ImDrawList::*)(const ImVec2 &) , &ImDrawList::PathLineTo >::invoke")
		->args({"self","pos"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

