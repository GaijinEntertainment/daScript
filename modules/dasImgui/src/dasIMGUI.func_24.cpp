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
void Module_dasIMGUI::initFunctions_24() {
	addCtorAndUsing<ImDrawCmd>(*this,lib,"ImDrawCmd","ImDrawCmd");
	using _method_72 = das::das_call_member< ImTextureID (ImDrawCmd::*)() const,&ImDrawCmd::GetTexID >;
// from imgui.h:3307:24
	makeExtern<DAS_CALL_METHOD(_method_72), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetTexID","das_call_member< ImTextureID (ImDrawCmd::*)() const , &ImDrawCmd::GetTexID >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImDrawListSplitter>(*this,lib,"ImDrawListSplitter","ImDrawListSplitter");
	using _method_73 = das::das_call_member< void (ImDrawListSplitter::*)(),&ImDrawListSplitter::Clear >;
// from imgui.h:3351:33
	makeExtern<DAS_CALL_METHOD(_method_73), SimNode_ExtFuncCall , imguiTempFn>(lib,"Clear","das_call_member< void (ImDrawListSplitter::*)() , &ImDrawListSplitter::Clear >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_74 = das::das_call_member< void (ImDrawListSplitter::*)(),&ImDrawListSplitter::ClearFreeMemory >;
// from imgui.h:3352:33
	makeExtern<DAS_CALL_METHOD(_method_74), SimNode_ExtFuncCall , imguiTempFn>(lib,"ClearFreeMemory","das_call_member< void (ImDrawListSplitter::*)() , &ImDrawListSplitter::ClearFreeMemory >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_75 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int),&ImDrawListSplitter::Split >;
// from imgui.h:3353:33
	makeExtern<DAS_CALL_METHOD(_method_75), SimNode_ExtFuncCall , imguiTempFn>(lib,"Split","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int) , &ImDrawListSplitter::Split >::invoke")
		->args({"self","draw_list","count"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_76 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *),&ImDrawListSplitter::Merge >;
// from imgui.h:3354:33
	makeExtern<DAS_CALL_METHOD(_method_76), SimNode_ExtFuncCall , imguiTempFn>(lib,"Merge","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *) , &ImDrawListSplitter::Merge >::invoke")
		->args({"self","draw_list"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_77 = das::das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int),&ImDrawListSplitter::SetCurrentChannel >;
// from imgui.h:3355:33
	makeExtern<DAS_CALL_METHOD(_method_77), SimNode_ExtFuncCall , imguiTempFn>(lib,"SetCurrentChannel","das_call_member< void (ImDrawListSplitter::*)(ImDrawList *,int) , &ImDrawListSplitter::SetCurrentChannel >::invoke")
		->args({"self","draw_list","channel_idx"})
		->addToModule(*this, SideEffects::worstDefault);
	addCtorAndUsing<ImDrawList,ImDrawListSharedData *>(*this,lib,"ImDrawList","ImDrawList")
		->args({"shared_data"});
	using _method_78 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,bool),&ImDrawList::PushClipRect >;
// from imgui.h:3425:21
	makeExtern<DAS_CALL_METHOD(_method_78), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushClipRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,bool) , &ImDrawList::PushClipRect >::invoke")
		->args({"self","clip_rect_min","clip_rect_max","intersect_with_current_clip_rect"})
		->arg_init(3,new ExprConstBool(false))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_79 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PushClipRectFullScreen >;
// from imgui.h:3426:21
	makeExtern<DAS_CALL_METHOD(_method_79), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushClipRectFullScreen","das_call_member< void (ImDrawList::*)() , &ImDrawList::PushClipRectFullScreen >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_80 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PopClipRect >;
// from imgui.h:3427:21
	makeExtern<DAS_CALL_METHOD(_method_80), SimNode_ExtFuncCall , imguiTempFn>(lib,"PopClipRect","das_call_member< void (ImDrawList::*)() , &ImDrawList::PopClipRect >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_81 = das::das_call_member< void (ImDrawList::*)(ImTextureRef),&ImDrawList::PushTexture >;
// from imgui.h:3428:21
	makeExtern<DAS_CALL_METHOD(_method_81), SimNode_ExtFuncCall , imguiTempFn>(lib,"PushTexture","das_call_member< void (ImDrawList::*)(ImTextureRef) , &ImDrawList::PushTexture >::invoke")
		->args({"self","tex_ref"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_82 = das::das_call_member< void (ImDrawList::*)(),&ImDrawList::PopTexture >;
// from imgui.h:3429:21
	makeExtern<DAS_CALL_METHOD(_method_82), SimNode_ExtFuncCall , imguiTempFn>(lib,"PopTexture","das_call_member< void (ImDrawList::*)() , &ImDrawList::PopTexture >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_83 = das::das_call_member< ImVec2 (ImDrawList::*)() const,&ImDrawList::GetClipRectMin >;
// from imgui.h:3430:21
	makeExtern<DAS_CALL_METHOD(_method_83), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetClipRectMin","das_call_member< ImVec2 (ImDrawList::*)() const , &ImDrawList::GetClipRectMin >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_84 = das::das_call_member< ImVec2 (ImDrawList::*)() const,&ImDrawList::GetClipRectMax >;
// from imgui.h:3431:21
	makeExtern<DAS_CALL_METHOD(_method_84), SimNode_ExtFuncCall , imguiTempFn>(lib,"GetClipRectMax","das_call_member< ImVec2 (ImDrawList::*)() const , &ImDrawList::GetClipRectMax >::invoke")
		->args({"self"})
		->addToModule(*this, SideEffects::worstDefault);
	using _method_85 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float),&ImDrawList::AddLine >;
// from imgui.h:3440:21
	makeExtern<DAS_CALL_METHOD(_method_85), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddLine","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float) , &ImDrawList::AddLine >::invoke")
		->args({"self","p1","p2","col","thickness"})
		->arg_init(4,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_86 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float),&ImDrawList::AddRect >;
// from imgui.h:3441:21
	makeExtern<DAS_CALL_METHOD(_method_86), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRect","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int,float) , &ImDrawList::AddRect >::invoke")
		->args({"self","p_min","p_max","col","rounding","flags","thickness"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_type(5,makeType<ImDrawFlags_>(lib))
		->arg_init(5,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->arg_init(6,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_87 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int),&ImDrawList::AddRectFilled >;
// from imgui.h:3442:21
	makeExtern<DAS_CALL_METHOD(_method_87), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRectFilled","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,float,int) , &ImDrawList::AddRectFilled >::invoke")
		->args({"self","p_min","p_max","col","rounding","flags"})
		->arg_init(4,new ExprConstFloat(0))
		->arg_type(5,makeType<ImDrawFlags_>(lib))
		->arg_init(5,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
	using _method_88 = das::das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,unsigned int,unsigned int,unsigned int),&ImDrawList::AddRectFilledMultiColor >;
// from imgui.h:3443:21
	makeExtern<DAS_CALL_METHOD(_method_88), SimNode_ExtFuncCall , imguiTempFn>(lib,"AddRectFilledMultiColor","das_call_member< void (ImDrawList::*)(const ImVec2 &,const ImVec2 &,unsigned int,unsigned int,unsigned int,unsigned int) , &ImDrawList::AddRectFilledMultiColor >::invoke")
		->args({"self","p_min","p_max","col_upr_left","col_upr_right","col_bot_right","col_bot_left"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

