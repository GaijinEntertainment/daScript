require cbind/cbind_boost
require fio
require daslib/defer
require daslib/strings_boost

class ImguiGen : CppGenBind
    override include_parse_file = false
    override func_to_stdout = false
    override generate_constant_decl = true
    imgui_skip_func : table<string; bool>
    def ImguiGen(mod_name,das_mod_name,pfn,pfp:string)
        bind_root = "{get_das_root()}/Modules/dasImgui/src"
        bind_module = mod_name
        bind_das_module = das_mod_name
        let i_imgui = "{get_das_root()}/Modules/dasImgui/imgui/"
        let i_imnodes = "{get_das_root()}/Modules/dasImgui/imnodes/"
        let i_imgui_node_editor = "{get_das_root()}/Modules/dasImgui/imgui-node-editor/"
        let args <- [{string
            "-xc++-header";
            "-std=c++1z";
            "-I{get_full_file_name(i_imgui)}";
            "-I{get_full_file_name(i_imnodes)}";
            "-I{get_full_file_name(i_imgui_node_editor)}";
            "-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS"
        }]
        func_per_chunk = 20
        init_args(pfn,pfp,args)
        init_rename()
        setDefaultFiles()
        openAllFiles()
    def override get_module_temp_fn: string
        return ", imguiTempFn"
    def init_rename
        aot_alias <- {{
            "ImVec2"    =>  true;
            "ImVec4"    =>  true;
            "ImColor"   =>  true
        }}
        alias2type <- {{
            "ImGuiCol"                  => "ImGuiCol_";
            "ImGuiCond"                 => "ImGuiCond_";
            "ImGuiDataType"             => "ImGuiDataType_";
            "ImGuiDir"                  => "ImGuiDir_";
            "ImGuiKey"                  => "ImGuiKey_";
            "ImGuiNavInput"             => "ImGuiNavInput_";
            "ImGuiMouseButton"          => "ImGuiMouseButton_";
            "ImGuiMouseCursor"          => "ImGuiMouseCursor_";
            "ImGuiSortDirection"        => "ImGuiSortDirection_";
            "ImGuiStyleVar"             => "ImGuiStyleVar_";
            "ImGuiTableBgTarget"        => "ImGuiTableBgTarget_";
            "ImDrawFlags"               => "ImDrawFlags_";
            "ImDrawListFlags"           => "ImDrawListFlags_";
            "ImFontAtlasFlags"          => "ImFontAtlasFlags_";
            "ImGuiBackendFlags"         => "ImGuiBackendFlags_";
            "ImGuiButtonFlags"          => "ImGuiButtonFlags_";
            "ImGuiColorEditFlags"       => "ImGuiColorEditFlags_";
            "ImGuiConfigFlags"          => "ImGuiConfigFlags_";
            "ImGuiComboFlags"           => "ImGuiComboFlags_";
            "ImGuiDragDropFlags"        => "ImGuiDragDropFlags_";
            "ImGuiFocusedFlags"         => "ImGuiFocusedFlags_";
            "ImGuiHoveredFlags"         => "ImGuiHoveredFlags_";
            "ImGuiInputTextFlags"       => "ImGuiInputTextFlags_";
            "ImGuiKeyModFlags"          => "ImGuiKeyModFlags_";
            "ImGuiPopupFlags"           => "ImGuiPopupFlags_";
            "ImGuiSelectableFlags"      => "ImGuiSelectableFlags_";
            "ImGuiSliderFlags"          => "ImGuiSliderFlags_";
            "ImGuiTabBarFlags"          => "ImGuiTabBarFlags_";
            "ImGuiTabItemFlags"         => "ImGuiTabItemFlags_";
            "ImGuiTableFlags"           => "ImGuiTableFlags_";
            "ImGuiTableColumnFlags"     => "ImGuiTableColumnFlags_";
            "ImGuiTableRowFlags"        => "ImGuiTableRowFlags_";
            "ImGuiTreeNodeFlags"        => "ImGuiTreeNodeFlags_";
            "ImGuiViewportFlags"        => "ImGuiViewportFlags_";
            "ImGuiWindowFlags"          => "ImGuiWindowFlags_"
        }}
        imgui_skip_func <- {{
            "ImGui::TextUnformatted" => true;
            "ImGuiTextFilter::PassFilter" => true;
            "ImGuiTextBuffer::append" => true;
            "ImGuiInputTextCallbackData::InsertChars" => true;
            "ImGui::CalcTextSize" => true;
            "ImGui::SetNextWindowSizeConstraints" => true;
            "ImDrawList::AddText" => true
        }}
        for v in values(alias2type)
            enums_with_flags[v] = true
    def override skip_anyFunction ( var c : CXCursor; isMethod : bool )
        if AnyGenBind`skip_anyFunction(self, c, isMethod)
            return true
        let function_name = namespace_name(string(clang_getCursorSpelling(c)))
        if imgui_skip_func |> key_exists(function_name)
            to_log(LOG_INFO, "skipping IMGUI blocked function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        return false
    def override isArgByValue ( name:string )
        return true
    def override generateModuleHPrefix
        module_h_file |> fwrite("#include \"cb_dasIMGUI.h\"\n")

class ImguiSubmoduleGen : ImguiGen
    cb_extra : string = ""
    def override get_module_temp_fn: string
        return ""
    def ImguiSubmoduleGen(mod_name,das_mod_name,pfn,pfp:string)
        ImguiGen`ImguiGen(self,mod_name,das_mod_name,pfn,pfp)
    def override generateModuleHPrefix
        module_h_file |> fwrite("#include \"cb_dasIMGUI.h\"\n")
        if cb_extra != ""
            module_h_file |> fwrite(cb_extra)
        module_h_file |> fwrite("#include \"need_dasIMGUI.h\"\n")

[export]
def main
    if true
        var cgb = new ImguiGen("dasIMGUI","imgui","imgui.h","{get_das_root()}/Modules/dasImgui/imgui/")
        defer <|
            unsafe
                delete cgb
        cgb->generate()
        cgb->genCMakeDecl("DAS_IMGUI_BIND")
    if true
        var cgb = new ImguiSubmoduleGen("dasIMNODES","imnodes","imnodes.h","{get_das_root()}/Modules/dasImgui/imnodes/")
        defer <|
            unsafe
                delete cgb
        cgb.require_modules <- [{auto "imgui"}]
        cgb.already_defined <- {{
            "ImGuiContext" => true
        }}
        cgb.cb_extra = "#include \"cb_dasIMNODES.h\"\n"
        cgb->generate()
        cgb->genCMakeDecl("DAS_IMNODES_BIND")
    if true
        var cgb = new ImguiSubmoduleGen("dasIMGUI_NODE_EDITOR","imgui_node_editor","imgui_node_editor.h","{get_das_root()}/Modules/dasImgui/imgui-node-editor/")
        defer <|
            unsafe
                delete cgb
        cgb.require_modules <- [{auto "imgui"}]
        cgb.already_defined <- {{
            "ImGuiContext" => true
        }}
        cgb.aot_alias["ax::NodeEditor::NodeId"] = true
        cgb.aot_alias["ax::NodeEditor::LinkId"] = true
        cgb.aot_alias["ax::NodeEditor::PinId"] = true
        cgb.cb_extra = "#include \"cb_dasIMGUI_NODE_EDITOR.h\"\n"
        cgb->generate()
        cgb->genCMakeDecl("DAS_IMGUI_NODE_EDITOR_BIND")
