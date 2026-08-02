options gen2
options _comment_hygiene = true
require cbind/cbind_boost
require daslib/fio
require daslib/defer
require daslib/strings_boost
require daslib/clargs

[CommandLineArgs]
struct Config {
    @clarg_short = "i"
    @clarg_doc = "Path to imgui headers dir (default: <das_root>/modules/dasImgui/imgui/)"
    input : string = "{get_das_root()}/modules/dasImgui/imgui/"

    @clarg_short = "o"
    @clarg_doc = "Output folder (default: <das_root>/modules/dasImgui/src)"
    output : string = "{get_das_root()}/modules/dasImgui/src"

    @clarg_short = "p"
    @clarg_doc = "Prefix in comment path (default is empty, affects: // from <prefix>/imgui.h:3574:8). "
    prefix : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

class ImguiGen : CppGenBind {
    override include_parse_file = false
    override func_to_stdout = false
    override generate_constant_decl = true
    imgui_skip_func : table<string; bool>
    // imgui_internal.h allow-lists — curated, grown one family per commit.
    // A symbol from imgui_internal.h is bound only if it appears here; everything
    // else internal is skipped, so the public imgui.h surface stays unchanged and
    // the opaque forward-declared types (ImGuiContext, ...) keep their dummies.
    internal_allow_func : table<string>
    internal_allow_struct : table<string>
    internal_allow_enum : table<string>
    def ImguiGen(mod_name : string, das_mod_name : string, pfn : string, cfg : Config) {
        let i_imgui = "{get_das_root()}/modules/dasImgui/imgui/"
        let args <- array<string>(
            "-xc++-header",
            "-std=c++1z",
            "-I{get_full_file_name(i_imgui)}",
            "-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS",
            "-DIMGUI_USE_WCHAR32"
        )
        // CppGenBind constructor runs init_args(...) + setDefaultFiles()
        super(pfn, cfg.input, args)
        bind_root = cfg.output
        bind_module = mod_name
        bind_das_module = das_mod_name
        prefix = cfg.prefix
        func_per_chunk = 20
        // We parse imgui_internal.h (which includes imgui.h), but constant
        // generation regexes the file's raw #defines — pin it to imgui.h so
        // dasIMGUI.const.inc stays identical to the imgui.h-only output.
        CONST_FILE_NAMES <- [ "{PARSE_FILE_PREFIX}imgui.h" ]
        init_rename()
        init_internal()
        setDefaultFiles()
        openAllFiles()
    }
    def override get_module_temp_fn : string {
        return ", imguiTempFn"
    }
    def init_rename {
        aot_alias <- {
            "ImVec2"    =>  true,
            "ImVec4"    =>  true,
            "ImColor"   =>  true,
            // ImRect (imgui_internal.h) is { ImVec2 Min; ImVec2 Max } — 4 floats,
            // aliased to float4 like ImVec4 (glue in src/cb_dasIMGUI.h /
            // aot_dasIMGUI.h). Lets internal rect-taking helpers bind directly.
            "ImRect"    =>  true
        }
        alias2type <- {
            "ImGuiCol"                  => "ImGuiCol_",
            "ImGuiCond"                 => "ImGuiCond_",
            "ImGuiDataType"             => "ImGuiDataType_",
            "ImGuiNavInput"             => "ImGuiNavInput_",
            "ImGuiMouseButton"          => "ImGuiMouseButton_",
            "ImGuiMouseCursor"          => "ImGuiMouseCursor_",
            "ImGuiStyleVar"             => "ImGuiStyleVar_",
            "ImGuiTableBgTarget"        => "ImGuiTableBgTarget_",
            "ImDrawFlags"               => "ImDrawFlags_",
            "ImDrawListFlags"           => "ImDrawListFlags_",
            "ImFontAtlasFlags"          => "ImFontAtlasFlags_",
            "ImGuiBackendFlags"         => "ImGuiBackendFlags_",
            "ImGuiButtonFlags"          => "ImGuiButtonFlags_",
            "ImGuiColorEditFlags"       => "ImGuiColorEditFlags_",
            "ImGuiConfigFlags"          => "ImGuiConfigFlags_",
            "ImGuiComboFlags"           => "ImGuiComboFlags_",
            "ImGuiDockNodeFlags"        => "ImGuiDockNodeFlags_",
            "ImGuiDragDropFlags"        => "ImGuiDragDropFlags_",
            "ImGuiFocusedFlags"         => "ImGuiFocusedFlags_",
            "ImGuiHoveredFlags"         => "ImGuiHoveredFlags_",
            "ImGuiInputTextFlags"       => "ImGuiInputTextFlags_",
            "ImGuiKeyModFlags"          => "ImGuiKeyModFlags_",
            "ImGuiPopupFlags"           => "ImGuiPopupFlags_",
            "ImGuiSelectableFlags"      => "ImGuiSelectableFlags_",
            "ImGuiSliderFlags"          => "ImGuiSliderFlags_",
            "ImGuiTabBarFlags"          => "ImGuiTabBarFlags_",
            "ImGuiTabItemFlags"         => "ImGuiTabItemFlags_",
            "ImGuiTableFlags"           => "ImGuiTableFlags_",
            "ImGuiTableColumnFlags"     => "ImGuiTableColumnFlags_",
            "ImGuiTableRowFlags"        => "ImGuiTableRowFlags_",
            "ImGuiTreeNodeFlags"        => "ImGuiTreeNodeFlags_",
            "ImGuiViewportFlags"        => "ImGuiViewportFlags_",
            "ImGuiWindowFlags"          => "ImGuiWindowFlags_",
            "ImGuiChildFlags"           => "ImGuiChildFlags_"
        }
        local_type_names <- {
            "ImGuiTableColumnSortSpecs" => true,
            "ImGuiTextFilter" => true,
            "Config" => true}
        // ImGuiTextFilter is bitwise zero-init-safe (empty InputBuf, empty Filters
        // ImVector, CountGrep=0) with a well-defined copy ctor, so daslang state
        // structs can hold one inline. Emitted by the binder now (was a manual
        // post-regen patch on src/dasIMGUI.struct.class.inc).
        // ImTextureRef is { ImTextureData* _TexData; ImTextureID _TexID }
        // — trivially copyable, passed by value to Image / ImageButton / ImageWithBg.
        // Mark copyable so a const ImTextureRef (e.g. GetIO().Fonts.TexRef) binds to
        // those by-value params.
        trivial_ctor_type_names <- {"ImGuiTextFilter" => true, "ImTextureRef" => true}
        copyable_type_names <- {"ImGuiTextFilter" => true, "ImTextureRef" => true}
        imgui_skip_func <- {
            // Image / ImageButton / ImageWithBg take ImTextureRef BY VALUE (non-const);
            // the auto-binding makes the das arg a non-const managed handle, which a
            // const ImTextureRef (e.g. GetIO().Fonts.TexRef) can't bind to. Manual
            // wrappers in dasIMGUI.main.cpp take `const ImTextureRef&` so the arg is
            // const (like ImVec2), matching every call site.
            "ImGui::Image" => true,
            "ImGui::ImageButton" => true,
            "ImGui::ImageWithBg" => true,
            "ImGui::TextUnformatted" => true,
            "ImGuiTextFilter::PassFilter" => true,
            "ImGuiTextBuffer::append" => true,
            "ImGuiInputTextCallbackData::InsertChars" => true,
            "ImGui::CalcTextSize" => true,
            "ImGui::SetNextWindowSizeConstraints" => true,
            "ImDrawList::AddText" => true
        }
        for (v in values(alias2type)) {
            enums_with_flags.insert(v, true)
        }
    }
    def init_internal {
        // ImGuiButtonFlagsPrivate_ (imgui_internal.h) extends the PUBLIC ImGuiButtonFlags
        // set — its VALUES carry the public `ImGuiButtonFlags_` prefix, not their own
        // enum name. Override the strip-prefix so they bind as ImGuiButtonFlagsPrivate.Repeat
        // etc. (else enum_entry_name leaves the full `ImGuiButtonFlags_Repeat`).
        enum_prefix |> insert("ImGuiButtonFlagsPrivate_", "ImGuiButtonFlags")
        // The private item flags (MixedValue, ReadOnly, AllowOverlap, NoNav, …) in
        // ImGuiItemFlagsPrivate_ carry the public ImGuiItemFlags_ prefix — strip it
        // so they bind as ImGuiItemFlags.MixedValue etc.
        enum_prefix |> insert("ImGuiItemFlagsPrivate_", "ImGuiItemFlags")
        // ImGuiComboFlagsPrivate_'s one value is ImGuiComboFlags_CustomPreview (carries
        // the public ImGuiComboFlags_ prefix) — strip it so it binds as CustomPreview.
        enum_prefix |> insert("ImGuiComboFlagsPrivate_", "ImGuiComboFlags")
        // ImGuiAxis's tag has no trailing underscore but its values are ImGuiAxis_X /
        // ImGuiAxis_Y, so strip the full "ImGuiAxis_" prefix (default would leave "_X").
        enum_prefix |> insert("ImGuiAxis", "ImGuiAxis_")
        // Family 1 — Render* draw helpers from imgui_internal.h. Every argument is
        // an already-bound public type (ImVec2, ImU32, ImDrawList*, ImGuiDir,
        // ImGuiMouseCursor, ImDrawFlags), so no internal struct/enum is pulled in.
        //
        // Family 2 — ShadeVerts* vertex-shading helpers. Same discipline: every arg
        // is already bound (ImDrawList*, int, ImVec2, ImU32, float, bool), so no
        // internal type is pulled in. They write over already-created draw vertices.
        //
        // Family 3 — PushItemFlag/PopItemFlag item-flag stack. First family pulling
        // an internal enum (ImGuiItemFlags_) into internal_allow_enum; v2 surface is
        // the with_item_flag scope guard (imgui_scope_builtin.das), mirroring
        // with_disabled — a push/pop pair, not a raw allow-list call.
        //
        // Family 4 — item / layout helpers (CalcItemSize / CalcWrapWidthForPos /
        // GetContentRegionMaxAbs / ItemSize). The three calculators are pure geometry
        // math (ImVec2/float in/out); ItemSize is NOT side-effect-free — it reserves
        // layout space (advances the cursor) the way built-in widgets do. ItemSize also
        // carries an inline ImRect overload; ImRect is now aliased to float4 (added to
        // aot_alias above + glue in cb_dasIMGUI.h), so BOTH overloads bind cleanly.
        // One-shot calls, raw on the ALLOWED_IMGUI allow-list (no scope guard).
        internal_allow_func <- {
            "ImGui::RenderFrame", "ImGui::RenderFrameBorder",
            "ImGui::RenderText", "ImGui::RenderTextWrapped",
            "ImGui::RenderColorRectWithAlphaCheckerboard", "ImGui::RenderMouseCursor",
            "ImGui::RenderArrow", "ImGui::RenderBullet", "ImGui::RenderCheckMark",
            "ImGui::RenderArrowPointingAt", "ImGui::RenderArrowDockMenu",
            "ImGui::ShadeVertsLinearColorGradientKeepAlpha", "ImGui::ShadeVertsLinearUV",
            "ImGui::ShadeVertsTransformPos",
            "ImGui::PushItemFlag", "ImGui::PopItemFlag",
            "ImGui::CalcItemSize", "ImGui::CalcWrapWidthForPos",
            "ImGui::ItemSize",
            // Family 1-complete — Render* helpers that were dropped from family 1
            // because they take an ImRect; now that ImRect is aliased to float4
            // (#166) they bind directly. RenderNavCursor also pulls the
            // ImGuiNavRenderCursorFlags_ enum (bound as int arg, like PushItemFlag).
            "ImGui::RenderDragDropTargetRectForItem", "ImGui::RenderNavCursor",
            "ImGui::RenderRectFilledWithHole",
            // ID-stack seed / override helpers. Pure ImGuiID(uint)/const char*/int
            // in, ImGuiID/void out — no internal struct, enum, or nullable pointer.
            // GetIDWithSeed's two-pointer overload mirrors the public GetID(begin,end):
            // both halves are required, so it is NOT a text_end wrapper case. FOOTGUN:
            // begin/end must point into the SAME buffer — two distinct das strings
            // compute a garbage end-begin (OOB read in ImGui's hash loop), exactly
            // like public GetID(begin,end) (bound for parity, never called in-tree).
            // For a seeded string ID from das, use with_override_id(seed){GetID(str)}.
            // One allow-list entry covers both GetIDWithSeed overloads (gate keys on
            // the bare spelling). GetActiveID is deliberately NOT here — it is already
            // bound via the das::GetActiveID manual wrapper (Phase 2.1, predates this
            // allow-list); re-adding it would double-bind "GetActiveID".
            "ImGui::GetFocusID", "ImGui::GetHoveredID",
            "ImGui::KeepAliveID", "ImGui::MarkItemEdited", "ImGui::PushOverrideID",
            "ImGui::GetIDWithSeed", "ImGui::TreePushOverrideID",
            // Focus-scope + activation stack. Pure ImGuiID(uint)/int64/void — no
            // internal struct/enum/pointer. PushFocusScope/PopFocusScope are a
            // stack (v2 guard with_focus_scope, imgui_scope_builtin.das);
            // GetCurrentFocusScope is inline (binds via the makeExtern
            // address-taking path, like GetFocusID). SetNextItemSelectionUserData
            // takes ImGuiSelectionUserData (typedef ImS64 -> int64 workhorse). The
            // SetActiveID/SetFocusID/FocusTopMostWindowUnderOne neighbors take
            // ImGuiWindow* and are correctly out of scope.
            "ImGui::PushFocusScope", "ImGui::PopFocusScope", "ImGui::GetCurrentFocusScope",
            "ImGui::FocusItem", "ImGui::ActivateItemByID", "ImGui::SetNextItemSelectionUserData",
            // Internal popup / tooltip / menu *Ex entry points. All host-agnostic:
            // ImGuiID(uint), ImGuiPopupFlags / ImGuiWindowFlags (public enums),
            // int/bool/const char*, plus ImGuiTooltipFlags_ (internal enum, added to
            // internal_allow_enum below — one real flag, OverridePrevious). Bound raw,
            // like the Render* batch — the public popup/menu/tooltip *containers*
            // already wrap the public Begin/End; these are the lower-level escape
            // hatches (raw-id popups pairing with #169's with_override_id/GetID,
            // icon-column menus, override-previous tooltips, popup-stack management).
            // IsPopupOpen is an overload of the public const char* version; the
            // makeExtern explicit-signature template arg disambiguates the
            // address-taking (same as the two GetIDWithSeed overloads). The Begin*Ex
            // pair with the already-public End* (EndPopup/EndTooltip/EndMenu). The
            // ClosePopupsOverWindow / GetTopMost*PopupModal / FindBlockingModal /
            // FindBestWindowPosForPopup* / GetPopupAllowedExtentRect neighbors take or
            // return ImGuiWindow* and are correctly out of scope.
            "ImGui::OpenPopupEx", "ImGui::ClosePopupToLevel", "ImGui::ClosePopupsExceptModals",
            "ImGui::IsPopupOpen", "ImGui::BeginPopupEx",
            "ImGui::BeginTooltipEx", "ImGui::BeginTooltipHidden",
            "ImGui::BeginMenuEx", "ImGui::MenuItemEx",
            // Internal item hit-test / rounding geometry helpers — the building
            // blocks a custom-widget author needs below the public widget API.
            // All take ImRect (aliased to float4) + ImGuiID(uint); ItemHoverable
            // also takes ImGuiItemFlags (internal enum, binds as int like
            // PushItemFlag); CalcRoundingFlagsForRectInRect returns ImDrawFlags
            // (public enum). No nullable pointers / ImGuiWindow* — they read the
            // current window implicitly. ItemAdd is deliberately NOT here: its
            // nav_bb is a nullable ImRect* (no-lost-functionality DISCUSS — pinning
            // it to null would drop the separate-nav-rect feature).
            "ImGui::ItemHoverable", "ImGui::IsClippedEx",
            "ImGui::CalcRoundingFlagsForRectInRect",
            // Widget *Ex buttons with behavior flags. ButtonEx/ArrowButtonEx take an
            // ImGuiButtonFlags arg whose USEFUL values (Repeat, PressedOnClick, …) live
            // in the internal ImGuiButtonFlagsPrivate_ enum (added to internal_allow_enum
            // below); a cross-enum `|` rail in imgui_enums.das combines the public +
            // private flags into one ImGuiButtonFlags the *Ex funcs accept. ButtonBehavior
            // (bool* out-params) and ItemAdd (nullable nav_bb -> C++ wrapper) are the
            // custom-widget primitives, deferred to the next family. ImageButtonEx is
            // texture-centric (ImTextureID = void*), deferred to an image family.
            "ImGui::ButtonEx", "ImGui::ArrowButtonEx",
            // Axis-aligned interactive layout primitives. Scrollbar(axis) draws the
            // current window's scrollbar on the given axis — binds raw (one ImGuiAxis
            // arg). Its lower-level sibling ScrollbarEx (ImS64* scroll out-param) and
            // SplitterBehavior (float* size1/size2 out-params) are manual C++ wrappers
            // (dasIMGUI.main.cpp) so the out-params come back through int64& / float&.
            "ImGui::Scrollbar",
            // Tree-node state machine. TreeNodeSetOpen imperatively sets a node's
            // open state by ID; TreeNodeUpdateNextOpen consumes a pending
            // SetNextItemOpen() and returns the resolved open state. Both pure
            // ImGuiID(uint)/bool/ImGuiTreeNodeFlags(public). The lower-level
            // TreeNodeBehavior (nullable label_end -> C++ wrapper) is in
            // dasIMGUI.main.cpp; pairs with the already-bound TreePushOverrideID.
            "ImGui::TreeNodeSetOpen", "ImGui::TreeNodeUpdateNextOpen",
            // SeparatorEx — a separator with explicit axis/span flags + thickness.
            // ImGuiSeparatorFlags is internal -> the flags arg binds as int. Its
            // siblings TextEx / SeparatorTextEx carry a nullable text_end / label_end
            // and are manual C++ wrappers (dasIMGUI.main.cpp).
            "ImGui::SeparatorEx",
            // Combo internals — the low-level combo dropdown the public combo is
            // built on. BeginComboPopup(popup_id, bb, flags) opens the dropdown for a
            // self-managed box (closes with EndCombo, NOT EndPopup — it bumps
            // g.BeginComboDepth and only EndCombo unwinds it); BeginComboPreview / EndComboPreview
            // render a custom preview for a BeginCombo opened with CustomPreview. All
            // public types. Surface = with_combo_popup / with_custom_preview_combo /
            // with_combo_preview scope guards (imgui_scope_builtin.das).
            "ImGui::BeginComboPopup", "ImGui::BeginComboPreview", "ImGui::EndComboPreview",
            // Logging/capture — the text-capture primitives the public Log* API is
            // built on. LogBegin(type, auto_open_depth) starts a capture to the chosen
            // target (closed by the public LogFinish; surface = the with_log guard in
            // imgui_scope_builtin.das). LogSetNextTextDecoration(prefix, suffix) wraps
            // the next captured text. LogRenderedText (nullable ref_pos / text_end) is a
            // manual C++ wrapper (dasIMGUI.main.cpp).
            "ImGui::LogBegin", "ImGui::LogSetNextTextDecoration"
        }
        internal_allow_enum <- {
            "ImGuiItemFlags_", "ImGuiItemFlagsPrivate_",
            "ImGuiNavRenderCursorFlags_", "ImGuiTooltipFlags_",
            "ImGuiButtonFlagsPrivate_",
            // Axis selector (None=-1 / X=0 / Y=1) for the scrollbar / splitter
            // behavior primitives. Plain enum, tag has no trailing underscore.
            "ImGuiAxis",
            // Separator / low-level text flags. SeparatorEx takes ImGuiSeparatorFlags
            // (Horizontal / Vertical / SpanAllColumns); TextEx takes ImGuiTextFlags
            // (NoWidthForLargeClippedText). Both internal -> args bind as int (cast
            // int(ImGuiSeparatorFlags.Vertical) at the call site, like ItemHoverable).
            "ImGuiSeparatorFlags_", "ImGuiTextFlags_",
            // ImGuiComboFlagsPrivate_ = the single internal combo flag CustomPreview
            // (enables BeginComboPreview). Combined into the public ImGuiComboFlags
            // inside the with_custom_preview_combo guard (imgui_scope_builtin.das).
            "ImGuiComboFlagsPrivate_",
            // ImGuiLogFlags_ (Output{TTY,File,Buffer,Clipboard}) selects LogBegin's
            // capture target.
            "ImGuiLogFlags_"
        }
    }
    def is_internal : bool {
        return prevFileName |> ends_with("imgui_internal.h")
    }
    def override skip_file(fname : string) : bool {
        // accept imgui.h AND imgui_internal.h; internal symbols are then gated down
        // to the curated allow-lists by skip_struct / skip_enum / skip_anyFunction
        return !(fname |> ends_with("imgui.h") || fname |> ends_with("imgui_internal.h"))
    }
    def override parse_Struct(var c : CXCursor; struct_name : string) {
        // Gate the struct *declaration* here, not via skip_struct — skip_struct is
        // also consulted by skip_type for referenced arg types, where a public
        // type (ImVec2, ImDrawList) used by an internal function must stay bound.
        // parse_Struct only runs at decl time, so prevFileName is the struct's
        // own file. Internal-only structs are no-op'd, so the opaque types that
        // imgui.h forward-declares keep their dummies.
        if (is_internal() && !key_exists(internal_allow_struct, struct_name)) return
        CppGenBind`parse_Struct(self, c, struct_name)
    }
    def override skip_enum(ns_en, en : string) {
        if (is_internal()) return !key_exists(internal_allow_enum, ns_en)
        return AnyGenBind`skip_enum(self, ns_en, en)
    }
    def override skip_anyFunction(var c : CXCursor; isMethod : bool) {
        if (AnyGenBind`skip_anyFunction(self, c, isMethod)) return true
        let function_name = namespace_name(string(clang_getCursorSpelling(c)))
        if (imgui_skip_func |> key_exists(function_name)) {
            to_log(LOG_INFO, "skipping IMGUI blocked function {function_name} at {clang_getCursorLocationDescription(c)}")
            return true
        }
        return is_internal() && !key_exists(internal_allow_func, function_name)
    }
    def override keep_c_arg_type(funcName, argName : string; argIndex : int) : bool {
        // DockSpace/DockSpaceOverViewport ship a public `flags : int` signature that predates
        // enum-typed flag args; keep them int so existing callers (and the docking wrapper) don't break.
        return (funcName == "DockSpace" || funcName == "DockSpaceOverViewport") && argName == "flags"
    }
    def override isArgByValue(name : string) {
        return true
    }
    def override generateModuleHPrefix {
        module_h_file |> fwrite("#include \"cb_dasIMGUI.h\"\n")
    }
}

[export]
def main {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "bind_imgui")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "bind_imgui")
        return
    }
    var cgb = new ImguiGen("dasIMGUI", "imgui", "imgui_internal.h", cfg)
    defer() {
        unsafe {
            delete cgb
        }
    }
    cgb->generate()
    cgb->genCMakeDecl("DAS_IMGUI_BIND")
}
