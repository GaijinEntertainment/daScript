// ------------
// functions
// ------------

addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_LoadFunctions)>(*this, lib, "ImGui_ImplVulkan_LoadFunctions",SideEffects::worstDefault, "ImGui_ImplVulkan_LoadFunctions")
	->args({"loader_func","user_data"})
		->arg_init(1,make_smart<ExprConstPtr>())
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_CreateFontsTexture)>(*this, lib, "ImGui_ImplVulkan_CreateFontsTexture",SideEffects::worstDefault, "ImGui_ImplVulkan_CreateFontsTexture")
	->args({"command_buffer"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkanH_GetMinImageCountFromPresentMode)>(*this, lib, "ImGui_ImplVulkanH_GetMinImageCountFromPresentMode",SideEffects::worstDefault, "ImGui_ImplVulkanH_GetMinImageCountFromPresentMode")
	->args({"present_mode"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_Init)>(*this, lib, "ImGui_ImplVulkan_Init",SideEffects::worstDefault, "ImGui_ImplVulkan_Init")
	->args({"info","render_pass"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_NewFrame)>(*this, lib, "ImGui_ImplVulkan_NewFrame",SideEffects::worstDefault, "ImGui_ImplVulkan_NewFrame")
	->args({})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_Shutdown)>(*this, lib, "ImGui_ImplVulkan_Shutdown",SideEffects::worstDefault, "ImGui_ImplVulkan_Shutdown")
	->args({})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_RenderDrawData)>(*this, lib, "ImGui_ImplVulkan_RenderDrawData",SideEffects::worstDefault, "ImGui_ImplVulkan_RenderDrawData")
	->args({"draw_data","command_buffer","pipeline"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkanH_DestroyWindow)>(*this, lib, "ImGui_ImplVulkanH_DestroyWindow",SideEffects::worstDefault, "ImGui_ImplVulkanH_DestroyWindow")
	->args({"instance","device","wnd","allocator"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_SetMinImageCount)>(*this, lib, "ImGui_ImplVulkan_SetMinImageCount",SideEffects::worstDefault, "ImGui_ImplVulkan_SetMinImageCount")
	->args({"min_image_count"})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkan_DestroyFontUploadObjects)>(*this, lib, "ImGui_ImplVulkan_DestroyFontUploadObjects",SideEffects::worstDefault, "ImGui_ImplVulkan_DestroyFontUploadObjects")
	->args({})
;
addExtern<DAS_BIND_FUN(ImGui_ImplVulkanH_CreateOrResizeWindow)>(*this, lib, "ImGui_ImplVulkanH_CreateOrResizeWindow",SideEffects::worstDefault, "ImGui_ImplVulkanH_CreateOrResizeWindow")
	->args({"instance","physical_device","device","wnd","queue_family","allocator","w","h","min_image_count"})
;
