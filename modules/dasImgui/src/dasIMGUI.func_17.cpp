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
void Module_dasIMGUI::initFunctions_17() {
// from imgui.h:961:29
	makeExtern< bool (*)(ImGuiKey) , ImGui::IsKeyDown , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsKeyDown","ImGui::IsKeyDown")
		->args({"key"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:962:29
	makeExtern< bool (*)(ImGuiKey,bool) , ImGui::IsKeyPressed , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsKeyPressed","ImGui::IsKeyPressed")
		->args({"key","repeat"})
		->arg_init(1,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:963:29
	makeExtern< bool (*)(ImGuiKey) , ImGui::IsKeyReleased , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsKeyReleased","ImGui::IsKeyReleased")
		->args({"key"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:964:29
	makeExtern< bool (*)(int) , ImGui::IsKeyChordPressed , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsKeyChordPressed","ImGui::IsKeyChordPressed")
		->args({"key_chord"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:965:29
	makeExtern< int (*)(ImGuiKey,float,float) , ImGui::GetKeyPressedAmount , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetKeyPressedAmount","ImGui::GetKeyPressedAmount")
		->args({"key","repeat_delay","rate"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:966:29
	makeExtern< const char * (*)(ImGuiKey) , ImGui::GetKeyName , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetKeyName","ImGui::GetKeyName")
		->args({"key"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:967:29
	makeExtern< void (*)(bool) , ImGui::SetNextFrameWantCaptureKeyboard , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextFrameWantCaptureKeyboard","ImGui::SetNextFrameWantCaptureKeyboard")
		->args({"want_capture_keyboard"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:973:29
	makeExtern< bool (*)(int) , ImGui::IsMouseDown , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseDown","ImGui::IsMouseDown")
		->args({"button"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:974:29
	makeExtern< bool (*)(int,bool) , ImGui::IsMouseClicked , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseClicked","ImGui::IsMouseClicked")
		->args({"button","repeat"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->arg_init(1,new ExprConstBool(false))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:975:29
	makeExtern< bool (*)(int) , ImGui::IsMouseReleased , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseReleased","ImGui::IsMouseReleased")
		->args({"button"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:976:29
	makeExtern< bool (*)(int) , ImGui::IsMouseDoubleClicked , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseDoubleClicked","ImGui::IsMouseDoubleClicked")
		->args({"button"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:977:29
	makeExtern< int (*)(int) , ImGui::GetMouseClickedCount , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMouseClickedCount","ImGui::GetMouseClickedCount")
		->args({"button"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:978:29
	makeExtern< bool (*)(const ImVec2 &,const ImVec2 &,bool) , ImGui::IsMouseHoveringRect , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseHoveringRect","ImGui::IsMouseHoveringRect")
		->args({"r_min","r_max","clip"})
		->arg_init(2,new ExprConstBool(true))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:979:29
	makeExtern< bool (*)(const ImVec2 *) , ImGui::IsMousePosValid , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMousePosValid","ImGui::IsMousePosValid")
		->args({"mouse_pos"})
		->arg_init(0,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:980:29
	makeExtern< bool (*)() , ImGui::IsAnyMouseDown , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsAnyMouseDown","ImGui::IsAnyMouseDown")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:981:29
	makeExtern< ImVec2 (*)() , ImGui::GetMousePos , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMousePos","ImGui::GetMousePos")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:982:29
	makeExtern< ImVec2 (*)() , ImGui::GetMousePosOnOpeningCurrentPopup , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMousePosOnOpeningCurrentPopup","ImGui::GetMousePosOnOpeningCurrentPopup")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:983:29
	makeExtern< bool (*)(int,float) , ImGui::IsMouseDragging , SimNode_ExtFuncCall , imguiTempFn>(lib,"IsMouseDragging","ImGui::IsMouseDragging")
		->args({"button","lock_threshold"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->arg_init(1,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:984:29
	makeExtern< ImVec2 (*)(int,float) , ImGui::GetMouseDragDelta , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMouseDragDelta","ImGui::GetMouseDragDelta")
		->args({"button","lock_threshold"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->arg_init(0,new ExprConstEnumeration(0,makeType<ImGuiMouseButton_>(lib)))
		->arg_init(1,new ExprConstFloat(-1))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:985:29
	makeExtern< void (*)(int) , ImGui::ResetMouseDragDelta , SimNode_ExtFuncCall , imguiTempFn>(lib,"ResetMouseDragDelta","ImGui::ResetMouseDragDelta")
		->args({"button"})
		->arg_type(0,makeType<ImGuiMouseButton_>(lib))
		->arg_init(0,new ExprConstEnumeration(0,makeType<ImGuiMouseButton_>(lib)))
		->addToModule(*this, SideEffects::worstDefault);
}
}

