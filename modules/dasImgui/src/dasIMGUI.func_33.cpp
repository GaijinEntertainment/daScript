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
void Module_dasIMGUI::initFunctions_33() {
// from imgui_internal.h:3763:29
	makeExtern< void (*)(const ImRect &) , ImGui::RenderDragDropTargetRectForItem , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderDragDropTargetRectForItem","ImGui::RenderDragDropTargetRectForItem")
		->args({"bb"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3887:29
	makeExtern< void (*)(ImVec2,const char *,const char *,bool) , ImGui::RenderText , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderText","ImGui::RenderText")
		->args({"pos","text","text_end","hide_text_after_hash"})
		->arg_init(2,new ExprConstString(""))
		->arg_init(3,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3888:29
	makeExtern< void (*)(ImVec2,const char *,const char *,float) , ImGui::RenderTextWrapped , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderTextWrapped","ImGui::RenderTextWrapped")
		->args({"pos","text","text_end","wrap_width"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3892:29
	makeExtern< void (*)(ImVec2,ImVec2,unsigned int,bool,float) , ImGui::RenderFrame , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderFrame","ImGui::RenderFrame")
		->args({"p_min","p_max","fill_col","borders","rounding"})
		->arg_init(3,new ExprConstBool(true))
		->arg_init(4,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3893:29
	makeExtern< void (*)(ImVec2,ImVec2,float) , ImGui::RenderFrameBorder , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderFrameBorder","ImGui::RenderFrameBorder")
		->args({"p_min","p_max","rounding"})
		->arg_init(2,new ExprConstFloat(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3895:29
	makeExtern< void (*)(ImDrawList *,ImVec2,ImVec2,unsigned int,float,ImVec2,float,int) , ImGui::RenderColorRectWithAlphaCheckerboard , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderColorRectWithAlphaCheckerboard","ImGui::RenderColorRectWithAlphaCheckerboard")
		->args({"draw_list","p_min","p_max","fill_col","grid_step","grid_off","rounding","flags"})
		->arg_init(6,new ExprConstFloat(0))
		->arg_type(7,makeType<ImDrawFlags_>(lib))
		->arg_init(7,new ExprConstEnumeration(0,makeType<ImDrawFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3896:29
	makeExtern< void (*)(const ImRect &,unsigned int,int) , ImGui::RenderNavCursor , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderNavCursor","ImGui::RenderNavCursor")
		->args({"bb","id","flags"})
		->arg_init(2,new ExprConstInt(0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3901:29
	makeExtern< void (*)(ImVec2,float,int,unsigned int,unsigned int,unsigned int) , ImGui::RenderMouseCursor , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderMouseCursor","ImGui::RenderMouseCursor")
		->args({"pos","scale","mouse_cursor","col_fill","col_border","col_shadow"})
		->arg_type(2,makeType<ImGuiMouseCursor_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3904:29
	makeExtern< void (*)(ImDrawList *,ImVec2,unsigned int,ImGuiDir,float) , ImGui::RenderArrow , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderArrow","ImGui::RenderArrow")
		->args({"draw_list","pos","col","dir","scale"})
		->arg_init(4,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3905:29
	makeExtern< void (*)(ImDrawList *,ImVec2,unsigned int) , ImGui::RenderBullet , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderBullet","ImGui::RenderBullet")
		->args({"draw_list","pos","col"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3906:29
	makeExtern< void (*)(ImDrawList *,ImVec2,unsigned int,float) , ImGui::RenderCheckMark , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderCheckMark","ImGui::RenderCheckMark")
		->args({"draw_list","pos","col","sz"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3907:29
	makeExtern< void (*)(ImDrawList *,ImVec2,ImVec2,ImGuiDir,unsigned int) , ImGui::RenderArrowPointingAt , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderArrowPointingAt","ImGui::RenderArrowPointingAt")
		->args({"draw_list","pos","half_sz","direction","col"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3908:29
	makeExtern< void (*)(ImDrawList *,ImVec2,float,unsigned int) , ImGui::RenderArrowDockMenu , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderArrowDockMenu","ImGui::RenderArrowDockMenu")
		->args({"draw_list","p_min","sz","col"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3910:29
	makeExtern< void (*)(ImDrawList *,const ImRect &,const ImRect &,unsigned int,float) , ImGui::RenderRectFilledWithHole , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderRectFilledWithHole","ImGui::RenderRectFilledWithHole")
		->args({"draw_list","outer","inner","col","rounding"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3911:29
	makeExtern< int (*)(const ImRect &,const ImRect &,float) , ImGui::CalcRoundingFlagsForRectInRect , SimNode_ExtFuncCall , imguiTempFn>(lib,"CalcRoundingFlagsForRectInRect","ImGui::CalcRoundingFlagsForRectInRect")
		->args({"r_in","r_outer","threshold"})
		->res_type(makeType<ImDrawFlags_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3919:29
	makeExtern< bool (*)(const char *,const ImVec2 &,int) , ImGui::ButtonEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"ButtonEx","ImGui::ButtonEx")
		->args({"label","size_arg","flags"})
		->arg_type(2,makeType<ImGuiButtonFlags_>(lib))
		->arg_init(2,new ExprConstEnumeration(0,makeType<ImGuiButtonFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3920:29
	makeExtern< bool (*)(const char *,ImGuiDir,ImVec2,int) , ImGui::ArrowButtonEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"ArrowButtonEx","ImGui::ArrowButtonEx")
		->args({"str_id","dir","size_arg","flags"})
		->arg_type(3,makeType<ImGuiButtonFlags_>(lib))
		->arg_init(3,new ExprConstEnumeration(0,makeType<ImGuiButtonFlags_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3922:29
	makeExtern< void (*)(int,float) , ImGui::SeparatorEx , SimNode_ExtFuncCall , imguiTempFn>(lib,"SeparatorEx","ImGui::SeparatorEx")
		->args({"flags","thickness"})
		->arg_init(1,new ExprConstFloat(1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3930:29
	makeExtern< void (*)(ImGuiAxis) , ImGui::Scrollbar , SimNode_ExtFuncCall , imguiTempFn>(lib,"Scrollbar","ImGui::Scrollbar")
		->args({"axis"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui_internal.h:3947:29
	makeExtern< void (*)(unsigned int) , ImGui::TreePushOverrideID , SimNode_ExtFuncCall , imguiTempFn>(lib,"TreePushOverrideID","ImGui::TreePushOverrideID")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

