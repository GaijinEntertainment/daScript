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
void Module_dasIMGUI::initFunctions_23() {
	using _method_62 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int),&ImDrawListSplitter::Split >;
// from imgui.h:2793:33
	makeExtern<DAS_CALL_METHOD(_method_62), SimNode_ExtFuncCall , imguiTempFn>(lib,"Split","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int) , &ImDrawListSplitter::Split >::invoke")
		->args({"self","draw_list","count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_63 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *),&ImDrawListSplitter::Merge >;
// from imgui.h:2794:33
	makeExtern<DAS_CALL_METHOD(_method_63), SimNode_ExtFuncCall , imguiTempFn>(lib,"Merge","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *) , &ImDrawListSplitter::Merge >::invoke")
		->args({"self","draw_list"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_64 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int),&ImDrawListSplitter::SetCurrentChannel >;
// from imgui.h:2795:33
	makeExtern<DAS_CALL_METHOD(_method_64), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCurrentChannel","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int) , &ImDrawListSplitter::SetCurrentChannel >::invoke")
		->args({"self","draw_list","channel_idx"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImDrawList,ImDrawListSharedData *>(*this,lib,"ImDrawList","ImDrawList")
		->args({"shared_data"});
	using _method_65 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,bool),&ImDrawList::PushClipRect >;
// from imgui.h:2863:21
	makeExtern<DAS_CALL_METHOD(_method_65), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushClipRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,bool) , &ImDrawList::PushClipRect >::invoke")
		->args({"self","clip_rect_min","clip_rect_max","intersect_with_current_clip_rect"})
		->arg_init(3,new ExprConstBool(false))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_66 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PushClipRectFullScreen >;
// from imgui.h:2864:21
	makeExtern<DAS_CALL_METHOD(_method_66), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushClipRectFullScreen","das_call_member< void (ImDrawList::*)() , &ImDrawList::PushClipRectFullScreen >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_67 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PopClipRect >;
// from imgui.h:2865:21
	makeExtern<DAS_CALL_METHOD(_method_67), SimNode_ExtFuncCall , imguiTempFn>(lib,"PopClipRect","das_call_member< void (ImDrawList::*)() , &ImDrawList::PopClipRect >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_68 = das::das_call_member< void (ImDrawList::*)(void *),&ImDrawList::PushTextureID >;
// from imgui.h:2866:21
	makeExtern<DAS_CALL_METHOD(_method_68), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushTextureID","das_call_member< void (ImDrawList::*)(void *) , &ImDrawList::PushTextureID >::invoke")
		->args({"self","texture_id"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_69 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PopTextureID >;
// from imgui.h:2867:21
	makeExtern<DAS_CALL_METHOD(_method_69), SimNode_ExtFuncCall , imguiTempFn>(lib,"PopTextureID","das_call_member< void (ImDrawList::*)() , &ImDrawList::PopTextureID >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_70 = das::das_call_member< ImVec2 (ImDrawList::*)() const,&ImDrawList::GetClipRectMin >;
// from imgui.h:2868:21
	makeExtern<DAS_CALL_METHOD(_method_70), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetClipRectMin","das_call_member< ImVec2 (ImDrawList::*)() const , &ImDrawList::GetClipRectMin >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_71 = das::das_call_member< ImVec2 (ImDrawList::*)() const,&ImDrawList::GetClipRectMax >;
// from imgui.h:2869:21
	makeExtern<DAS_CALL_METHOD(_method_71), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetClipRectMax","das_call_member< ImVec2 (ImDrawList::*)() const , &ImDrawList::GetClipRectMax >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_72 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddLine >;
// from imgui.h:2878:21
	makeExtern<DAS_CALL_METHOD(_method_72), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddLine","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddLine >::invoke")
		->args({"self","p1","p2","col","thickness"})
		->arg_init(4,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_73 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float),&ImDrawList::AddRect >;
// from imgui.h:2879:21
	makeExtern<DAS_CALL_METHOD(_method_73), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float) , &ImDrawList::AddRect >::invoke")
		->args({"self","p_min","p_max","col","rounding","flags","thickness"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_type(5,makeType<ImDrawFlags_>(lib))
		->arg_init(5,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->arg_init(6,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_74 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddRectFilled >;
// from imgui.h:2880:21
	makeExtern<DAS_CALL_METHOD(_method_74), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRectFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddRectFilled >::invoke")
		->args({"self","p_min","p_max","col","rounding","flags"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_type(5,makeType<ImDrawFlags_>(lib))
		->arg_init(5,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_75 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,unsigned int,unsigned int,unsigned int),&ImDrawList::AddRectFilledMultiColor >;
// from imgui.h:2881:21
	makeExtern<DAS_CALL_METHOD(_method_75), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRectFilledMultiColor","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,unsigned int,unsigned int,unsigned int) , &ImDrawList::AddRectFilledMultiColor >::invoke")
		->args({"self","p_min","p_max","col_upr_left","col_upr_right","col_bot_right","col_bot_left"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_76 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddQuad >;
// from imgui.h:2882:21
	makeExtern<DAS_CALL_METHOD(_method_76), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddQuad","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddQuad >::invoke")
		->args({"self","p1","p2","p3","p4","col","thickness"})
		->arg_init(6,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_77 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddQuadFilled >;
// from imgui.h:2883:21
	makeExtern<DAS_CALL_METHOD(_method_77), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddQuadFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddQuadFilled >::invoke")
		->args({"self","p1","p2","p3","p4","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_78 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddTriangle >;
// from imgui.h:2884:21
	makeExtern<DAS_CALL_METHOD(_method_78), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddTriangle","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddTriangle >::invoke")
		->args({"self","p1","p2","p3","col","thickness"})
		->arg_init(5,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_79 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int),&ImDrawList::AddTriangleFilled >;
// from imgui.h:2885:21
	makeExtern<DAS_CALL_METHOD(_method_79), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddTriangleFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,const ImVec2 &,unsigned int) , &ImDrawList::AddTriangleFilled >::invoke")
		->args({"self","p1","p2","p3","col"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_80 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float),&ImDrawList::AddCircle >;
// from imgui.h:2886:21
	makeExtern<DAS_CALL_METHOD(_method_80), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddCircle","das_call_member< void (ImDrawList::*)(const ImVec2 &,float,unsigned int,int,float) , &ImDrawList::AddCircle >::invoke")
		->args({"self","center","radius","col","num_segments","thickness"})
		->arg_init(4,new ExprConstInt(0))
		->arg_init(5,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
}
}

