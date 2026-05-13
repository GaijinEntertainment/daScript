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
void Module_dasIMGUI::initFunctions_18() {
// from imgui.h:986:32
	makeExtern< int (*)() , ImGui::GetMouseCursor , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetMouseCursor","ImGui::GetMouseCursor")
		->res_type(makeType<ImGuiMouseCursor_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:987:29
	makeExtern< void (*)(int) , ImGui::SetMouseCursor , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetMouseCursor","ImGui::SetMouseCursor")
		->args({"cursor_type"})
		->arg_type(0,makeType<ImGuiMouseCursor_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:988:29
	makeExtern< void (*)(bool) , ImGui::SetNextFrameWantCaptureMouse , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetNextFrameWantCaptureMouse","ImGui::SetNextFrameWantCaptureMouse")
		->args({"want_capture_mouse"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:992:29
	makeExtern< const char * (*)() , ImGui::GetClipboardText , SimNode_ExtFuncCall , imguiTempFn>(lib,"GetClipboardText","ImGui::GetClipboardText")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:993:29
	makeExtern< void (*)(const char *) , ImGui::SetClipboardText , SimNode_ExtFuncCall , imguiTempFn>(lib,"SetClipboardText","ImGui::SetClipboardText")
		->args({"text"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:999:29
	makeExtern< void (*)(const char *) , ImGui::LoadIniSettingsFromDisk , SimNode_ExtFuncCall , imguiTempFn>(lib,"LoadIniSettingsFromDisk","ImGui::LoadIniSettingsFromDisk")
		->args({"ini_filename"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1000:29
	makeExtern< void (*)(const char *,size_t) , ImGui::LoadIniSettingsFromMemory , SimNode_ExtFuncCall , imguiTempFn>(lib,"LoadIniSettingsFromMemory","ImGui::LoadIniSettingsFromMemory")
		->args({"ini_data","ini_size"})
		->arg_init(1,new ExprConstUInt64(0x0))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1001:29
	makeExtern< void (*)(const char *) , ImGui::SaveIniSettingsToDisk , SimNode_ExtFuncCall , imguiTempFn>(lib,"SaveIniSettingsToDisk","ImGui::SaveIniSettingsToDisk")
		->args({"ini_filename"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1002:29
	makeExtern< const char * (*)(size_t *) , ImGui::SaveIniSettingsToMemory , SimNode_ExtFuncCall , imguiTempFn>(lib,"SaveIniSettingsToMemory","ImGui::SaveIniSettingsToMemory")
		->args({"out_ini_size"})
		->arg_init(0,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1006:29
	makeExtern< void (*)(const char *) , ImGui::DebugTextEncoding , SimNode_ExtFuncCall , imguiTempFn>(lib,"DebugTextEncoding","ImGui::DebugTextEncoding")
		->args({"text"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1007:29
	makeExtern< void (*)(int) , ImGui::DebugFlashStyleColor , SimNode_ExtFuncCall , imguiTempFn>(lib,"DebugFlashStyleColor","ImGui::DebugFlashStyleColor")
		->args({"idx"})
		->arg_type(0,makeType<ImGuiCol_>(lib))
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1008:29
	makeExtern< void (*)() , ImGui::DebugStartItemPicker , SimNode_ExtFuncCall , imguiTempFn>(lib,"DebugStartItemPicker","ImGui::DebugStartItemPicker")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1009:29
	makeExtern< bool (*)(const char *,size_t,size_t,size_t,size_t,size_t,size_t) , ImGui::DebugCheckVersionAndDataLayout , SimNode_ExtFuncCall , imguiTempFn>(lib,"DebugCheckVersionAndDataLayout","ImGui::DebugCheckVersionAndDataLayout")
		->args({"version_str","sz_io","sz_style","sz_vec2","sz_vec4","sz_drawvert","sz_drawidx"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1017:29
	makeExtern< void * (*)(size_t) , ImGui::MemAlloc , SimNode_ExtFuncCall , imguiTempFn>(lib,"MemAlloc","ImGui::MemAlloc")
		->args({"size"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1018:29
	makeExtern< void (*)(void *) , ImGui::MemFree , SimNode_ExtFuncCall , imguiTempFn>(lib,"MemFree","ImGui::MemFree")
		->args({"ptr"})
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1023:33
	makeExtern< ImGuiPlatformIO & (*)() , ImGui::GetPlatformIO , SimNode_ExtFuncCallRef , imguiTempFn>(lib,"GetPlatformIO","ImGui::GetPlatformIO")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1024:33
	makeExtern< void (*)() , ImGui::UpdatePlatformWindows , SimNode_ExtFuncCall , imguiTempFn>(lib,"UpdatePlatformWindows","ImGui::UpdatePlatformWindows")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1025:33
	makeExtern< void (*)(void *,void *) , ImGui::RenderPlatformWindowsDefault , SimNode_ExtFuncCall , imguiTempFn>(lib,"RenderPlatformWindowsDefault","ImGui::RenderPlatformWindowsDefault")
		->args({"platform_render_arg","renderer_render_arg"})
		->arg_init(0,new ExprConstPtr())
		->arg_init(1,new ExprConstPtr())
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1026:33
	makeExtern< void (*)() , ImGui::DestroyPlatformWindows , SimNode_ExtFuncCall , imguiTempFn>(lib,"DestroyPlatformWindows","ImGui::DestroyPlatformWindows")
		->addToModule(*this, SideEffects::worstDefault);
// from imgui.h:1027:33
	makeExtern< ImGuiViewport * (*)(unsigned int) , ImGui::FindViewportByID , SimNode_ExtFuncCall , imguiTempFn>(lib,"FindViewportByID","ImGui::FindViewportByID")
		->args({"id"})
		->addToModule(*this, SideEffects::worstDefault);
}
}

