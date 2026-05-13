options gen2
options indenting = 4

module imgui_demo

require imgui/imgui_boost
require daslib/safe_addr
require daslib/static_let
require math
require strings

def DasHelpMarker(desc : string) {
    TextDisabled("(?)");
    if (IsItemHovered()) {
        BeginTooltip();
        PushTextWrapPos(GetFontSize() * 35.0f);
        TextUnformatted(desc);
        PopTextWrapPos();
        EndTooltip();
    }
}

def DasShowUserGuide() {
    let io & = unsafe(GetIO());
    BulletText("Double-click on title bar to collapse window.");
    BulletText(
        "Click and drag on lower corner to resize window\n" +
        "(double-click to auto fit window to its contents).");
    BulletText("CTRL+Click on a slider or drag box to input value as text.");
    BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
    if (io.FontAllowUserScaling) {
        BulletText("CTRL+Mouse Wheel to zoom window contents.");
    }
    BulletText("While inputing text:\n");
    Indent();
    BulletText("CTRL+Left/Right to word jump.");
    BulletText("CTRL+A or double-click to select all.");
    BulletText("CTRL+X/C/V to use clipboard cut/copy/paste.");
    BulletText("CTRL+Z,CTRL+Y to undo/redo.");
    BulletText("ESCAPE to revert.");
    BulletText("You can apply arithmetic operators +,*,/ on numerical values.\nUse +- to subtract.");
    Unindent();
    BulletText("With keyboard navigation enabled:");
    Indent();
    BulletText("Arrow keys to navigate.");
    BulletText("Space to activate a widget.");
    BulletText("Return to input text into a widget.");
    BulletText("Escape to deactivate a widget, close popup, exit child window.");
    BulletText("Alt to jump to the menu layer of a window.");
    BulletText("CTRL+Tab to select a window.");
    Unindent();
}

def DasShowExampleAppMainMenuBar() {
    if (BeginMainMenuBar()) {
        if (BeginMenu("File")) {
            ShowExampleMenuFile();
            EndMenu();
        }
        if (BeginMenu("Edit")) {
            if (MenuItem("Undo", "CTRL+Z")) {
                pass;
            }
            if (MenuItem("Redo", "CTRL+Y", false, false)) {
                pass;
            }
            Separator();
            if (MenuItem("Cut", "CTRL+X")) {
                pass;
            }
            if (MenuItem("Copy", "CTRL+C")) {
                pass;
            }
            if (MenuItem("Paste", "CTRL+V")) {
                pass;
            }
            EndMenu();
        }
        EndMainMenuBar();
    }
}

def DasShowAboutWindow(p_open : bool? implicit) {
    if (!Begin("About Dear ImGui", p_open, ImGuiWindowFlags.AlwaysAutoResize)) {
        End();
        return ;
    }
    Text("Dear ImGui {GetVersion()}");
    Separator();
    Text("By Omar Cornut and all Dear ImGui contributors.");
    Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
    static_let <| $() {
        var show_config_info = false;
    }
    Checkbox("Config/Build Information", safe_addr(show_config_info));
    if (show_config_info) {
        var io & = unsafe(GetIO());
        var style & = unsafe(GetStyle());
        let copy_to_clipboard = Button("Copy to clipboard");
        var child_size = float2(0., GetTextLineHeightWithSpacing() * 18.);
        BeginChild(GetID("cfg_infos"), child_size, ImGuiChildFlags.FrameStyle, ImGuiWindowFlags.NoMove);
        if (copy_to_clipboard) {
            LogToClipboard();
            LogText("```\n");
        }
        Text("Dear ImGui {IMGUI_VERSION} ({IMGUI_VERSION_NUM})");
        Separator();
        // Text("sizeof(ImDrawIdx): {typeinfo(sizeof type<ImDrawIdx>)}, sizeof(ImDrawVert): {typeinfo(sizeof type<ImDrawVert>)}")
        // Separator()
        Text("io.BackendPlatformName: {io.BackendPlatformName}");
        Text("io.BackendRendererName: {io.BackendRendererName}");
        Text("io.ConfigFlags: {io.ConfigFlags}");
        var icf : ImGuiConfigFlags;
        unsafe {
            icf = reinterpret<ImGuiConfigFlags> io.ConfigFlags;
        }
        if (icf && ImGuiConfigFlags.NavEnableKeyboard) {
            Text(" NavEnableKeyboard");
        }
        if (icf && ImGuiConfigFlags.NavEnableGamepad) {
            Text(" NavEnableGamepad");
        }
        if (icf && ImGuiConfigFlags.NavEnableSetMousePos) {
            Text(" NavEnableSetMousePos");
        }
        if (icf && ImGuiConfigFlags.NavNoCaptureKeyboard) {
            Text(" NavNoCaptureKeyboard");
        }
        if (icf && ImGuiConfigFlags.NoMouse) {
            Text(" NoMouse");
        }
        if (icf && ImGuiConfigFlags.NoMouseCursorChange) {
            Text(" NoMouseCursorChange");
        }
        if (io.MouseDrawCursor) {
            Text("io.MouseDrawCursor");
        }
        if (io.ConfigMacOSXBehaviors) {
            Text("io.ConfigMacOSXBehaviors");
        }
        if (io.ConfigInputTextCursorBlink) {
            Text("io.ConfigInputTextCursorBlink");
        }
        if (io.ConfigWindowsResizeFromEdges) {
            Text("io.ConfigWindowsResizeFromEdges");
        }
        if (io.ConfigWindowsMoveFromTitleBarOnly) {
            Text("io.ConfigWindowsMoveFromTitleBarOnly");
        }
        if (io.ConfigMemoryCompactTimer >= 0.0f) {
            Text("io.ConfigMemoryCompactTimer = {io.ConfigMemoryCompactTimer}");
        }
        Text("io.BackendFlags: {io.BackendFlags}");
        var ibf : ImGuiBackendFlags;
        unsafe {
            ibf = reinterpret<ImGuiBackendFlags> io.BackendFlags;
        }
        if (ibf && ImGuiBackendFlags.HasGamepad) {
            Text(" HasGamepad");
        }
        if (ibf && ImGuiBackendFlags.HasMouseCursors) {
            Text(" HasMouseCursors");
        }
        if (ibf && ImGuiBackendFlags.HasSetMousePos) {
            Text(" HasSetMousePos");
        }
        if (ibf && ImGuiBackendFlags.RendererHasVtxOffset) {
            Text(" RendererHasVtxOffset");
        }
        Separator();
        Text("io.Fonts: {io.Fonts.Fonts} fonts, Flags: {io.Fonts.Flags}, TexSize: {io.Fonts.TexWidth},{io.Fonts.TexHeight}");
        Text("io.DisplaySize: {io.DisplaySize.x},{io.DisplaySize.y}");
        Text("io.DisplayFramebufferScale: {io.DisplayFramebufferScale.x},{io.DisplayFramebufferScale.y}");
        Separator();
        Text("style.WindowPadding: {style.WindowPadding.x},{style.WindowPadding.y}");
        Text("style.WindowBorderSize: {style.WindowBorderSize}");
        Text("style.FramePadding: {style.FramePadding.x},{style.FramePadding.y}");
        Text("style.FrameRounding: {style.FrameRounding}");
        Text("style.FrameBorderSize: {style.FrameBorderSize}");
        Text("style.ItemSpacing: {style.ItemSpacing.x},{style.ItemSpacing.y}");
        Text("style.ItemInnerSpacing: {style.ItemInnerSpacing.x},{style.ItemInnerSpacing.y}");
        if (copy_to_clipboard) {
            LogText("\n```\n");
            LogFinish();
        }
        EndChild();
    }
    End();
}

[export]
def DasShowDemoWindow(var _p_open : bool?) {
    var p_open = _p_open;
    verify(GetCurrentContext() != null, "Missing dear imgui context. Refer to examples app!");

    static_let <| $() {
        var show_app_main_menu_bar = false;
        var show_app_documents = false;
        var show_app_console = false;
        var show_app_log = false;
        var show_app_layout = false;
        var show_app_property_editor = false;
        var show_app_long_text = false;
        var show_app_auto_resize = false;
        var show_app_constrained_resize = false;
        var show_app_simple_overlay = false;
        var show_app_window_titles = false;
        var show_app_custom_rendering = false;
    }
    if (show_app_main_menu_bar) {
        DasShowExampleAppMainMenuBar();
    }
    if (show_app_documents) {
        DasShowExampleAppDocuments(safe_addr(show_app_documents));
    }
    if (show_app_console) {
        DasShowExampleAppConsole(safe_addr(show_app_console));
    }
    if (show_app_log) {
        DasShowExampleAppLog(safe_addr(show_app_log));
    }
    if (show_app_layout) {
        DasShowExampleAppLayout(safe_addr(show_app_layout));
    }
    if (show_app_property_editor) {
        DasShowExampleAppPropertyEditor(safe_addr(show_app_property_editor));
    }
    if (show_app_long_text) {
        DasShowExampleAppLongText(safe_addr(show_app_long_text));
    }
    if (show_app_auto_resize) {
        DasShowExampleAppAutoResize(safe_addr(show_app_auto_resize));
    }
    if (show_app_constrained_resize) {
        DasShowExampleAppConstrainedResize(safe_addr(show_app_constrained_resize));
    }
    if (show_app_simple_overlay) {
        DasShowExampleAppSimpleOverlay(safe_addr(show_app_simple_overlay));
    }
    if (show_app_window_titles) {
        DasShowExampleAppWindowTitles(safe_addr(show_app_window_titles));
    }
    if (show_app_custom_rendering) {
        DasShowExampleAppCustomRendering(safe_addr(show_app_custom_rendering));
    }

    static_let <| $() {
        var show_app_metrics = false;
        var show_app_style_editor = false;
        var show_app_about = false;
    }
    if (show_app_metrics) {
        ShowMetricsWindow(safe_addr(show_app_metrics));  // NOTE: this is in imgui.cpp ???
    }
    if (show_app_about) {
        DasShowAboutWindow(safe_addr(show_app_about));
    }
    if (show_app_style_editor) {
        Begin("Dear ImGui Style Editor", safe_addr(show_app_style_editor));
        ShowStyleEditor();
        End();
    }

    static_let <| $() {
        var no_titlebar = false;
        var no_scrollbar = false;
        var no_menu = false;
        var no_move = false;
        var no_resize = false;
        var no_collapse = false;
        var no_close = false;
        var no_nav = false;
        var no_background = false;
        var no_bring_to_front = false;
    }
    var window_flags : ImGuiWindowFlags;
    if (no_titlebar) {
        window_flags |= ImGuiWindowFlags.NoTitleBar;
    }
    if (no_scrollbar) {
        window_flags |= ImGuiWindowFlags.NoScrollbar;
    }
    if (!no_menu) {
        window_flags |= ImGuiWindowFlags.MenuBar;
    }
    if (no_move) {
        window_flags |= ImGuiWindowFlags.NoMove;
    }
    if (no_resize) {
        window_flags |= ImGuiWindowFlags.NoResize;
    }
    if (no_collapse) {
        window_flags |= ImGuiWindowFlags.NoCollapse;
    }
    if (no_nav) {
        window_flags |= ImGuiWindowFlags.NoNav;
    }
    if (no_background) {
        window_flags |= ImGuiWindowFlags.NoBackground;
    }
    if (no_bring_to_front) {
        window_flags |= ImGuiWindowFlags.NoBringToFrontOnFocus;
    }
    if (no_close) {
        p_open = null;
    }
    SetNextWindowPos(float2(650., 20.), ImGuiCond.FirstUseEver);
    SetNextWindowSize(float2(550., 680.), ImGuiCond.FirstUseEver);
    if (!Begin("Dear ImGui Demo", p_open, window_flags)) {
        End();
    }
    PushItemWidth(float(GetFontSize()) * -12.);

    if (BeginMenuBar()) {
        if (BeginMenu("Menu")) {
            ShowExampleMenuFile();
            EndMenu();
        }
        if (BeginMenu("Examples")) {
            MenuItem("Main menu bar", "", safe_addr(show_app_main_menu_bar));
            MenuItem("Console", "", safe_addr(show_app_console));
            MenuItem("Log", "", safe_addr(show_app_log));
            MenuItem("Simple layout", "", safe_addr(show_app_layout));
            MenuItem("Property editor", "", safe_addr(show_app_property_editor));
            MenuItem("Long text display", "", safe_addr(show_app_long_text));
            MenuItem("Auto-resizing window", "", safe_addr(show_app_auto_resize));
            MenuItem("Constrained-resizing window", "", safe_addr(show_app_constrained_resize));
            MenuItem("Simple overlay", "", safe_addr(show_app_simple_overlay));
            MenuItem("Manipulating window titles", "", safe_addr(show_app_window_titles));
            MenuItem("Custom rendering", "", safe_addr(show_app_custom_rendering));
            MenuItem("Documents", "", safe_addr(show_app_documents));
            EndMenu();
        }
        if (BeginMenu("Tools")) {
            MenuItem("Metrics/Debugger", "", safe_addr(show_app_metrics));
            MenuItem("Style Editor", "", safe_addr(show_app_style_editor));
            MenuItem("About Dear ImGui", "", safe_addr(show_app_about));
            EndMenu();
        }
        EndMenuBar();
    }

    Text("dear imgui says hello. {IMGUI_VERSION}");
    Spacing();

    if (CollapsingHeader("Help")) {
        Text("ABOUT THIS DEMO:");
        BulletText("Sections below are demonstrating many aspects of the library.");
        BulletText("The \"Examples\" menu above leads to more demo contents.");
        BulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n" +
                          "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");
        Separator();
        Text("PROGRAMMER GUIDE:");
        BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        BulletText("See comments in imgui.cpp.");
        BulletText("See example applications in the examples/ folder.");
        BulletText("Read the FAQ at http://www.dearimgui.org/faq/");
        BulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        BulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        Separator();
        Text("USER GUIDE:");
        DasShowUserGuide();
    }

    if (CollapsingHeader("Configuration")) {
        var io & = unsafe(GetIO());
        if (TreeNode("Configuration##2")) {
            unsafe {
                CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", addr(io.ConfigFlags), ImGuiConfigFlags.NavEnableKeyboard);
            }
            SameLine();
            DasHelpMarker("Enable keyboard controls.");
            unsafe {
                CheckboxFlags("io.ConfigFlags: NavEnableGamepad", addr(io.ConfigFlags), ImGuiConfigFlags.NavEnableGamepad);
            }
            SameLine();
            DasHelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= ImGuiBackendFlags HasGamepad.\n\nRead instructions in imgui.cpp for details.");
            unsafe {
                CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", addr(io.ConfigFlags), ImGuiConfigFlags.NavEnableSetMousePos);
            }
            SameLine();
            DasHelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlagsNavEnableSetMousePos.");
            unsafe {
                CheckboxFlags("io.ConfigFlags: NoMouse", addr(io.ConfigFlags), ImGuiConfigFlags.NoMouse);
            }
            if (io.ConfigFlags && ImGuiConfigFlags.NoMouse) {
                if ((float(GetTime()) % 0.40f) < 0.20f) {
                    SameLine();
                    Text("<<PRESS SPACE TO DISABLE>>");
                }
                if (IsKeyPressed(ImGuiKey.Space)) {
                    io.ConfigFlags &= ~ImGuiConfigFlags.NoMouse;
                }
            }
            unsafe {
                CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", addr(io.ConfigFlags), ImGuiConfigFlags.NoMouseCursorChange);
            }
            SameLine();
            DasHelpMarker("Instruct backend to not alter mouse cursor shape and visibility.");
            unsafe {
                Checkbox("io.ConfigInputTextCursorBlink", addr(io.ConfigInputTextCursorBlink));
            }
            SameLine();
            DasHelpMarker("Enable blinking cursor (optional as some users consider it to be distracting)");
            unsafe {
                Checkbox("io.ConfigDragClickToInputText", addr(io.ConfigDragClickToInputText));
            }
            SameLine();
            DasHelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving).");
            unsafe {
                Checkbox("io.ConfigWindowsResizeFromEdges", addr(io.ConfigWindowsResizeFromEdges));
            }
            SameLine();
            DasHelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags HasMouseCursors) because it needs mouse cursor feedback.");
            unsafe {
                Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", addr(io.ConfigWindowsMoveFromTitleBarOnly));
            }
            unsafe {
                Checkbox("io.MouseDrawCursor", addr(io.MouseDrawCursor));
            }
            SameLine();
            DasHelpMarker("Instruct Dear ImGui to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");
            Text("Also see Style->Rendering for rendering options.");
            TreePop();
            Separator();
        }
        if (TreeNode("Backend Flags")) {
            DasHelpMarker(
                "Those flags are set by the backends (imgui_impl_xxx files) to specify their capabilities.\n" +
                "Here we expose then as read-only fields to avoid breaking interactions with your backend.");
            var backend_flags = io.BackendFlags;
            CheckboxFlags("io.BackendFlags: HasGamepad", safe_addr(backend_flags), ImGuiBackendFlags.HasGamepad);
            CheckboxFlags("io.BackendFlags: HasMouseCursors", safe_addr(backend_flags), ImGuiBackendFlags.HasMouseCursors);
            CheckboxFlags("io.BackendFlags: HasSetMousePos", safe_addr(backend_flags), ImGuiBackendFlags.HasSetMousePos);
            CheckboxFlags("io.BackendFlags: RendererHasVtxOffset", safe_addr(backend_flags), ImGuiBackendFlags.RendererHasVtxOffset);
            TreePop();
            Separator();
        }
        if (TreeNode("Style")) {
            DasHelpMarker("The same contents can be accessed in 'Tools->Style Editor' or by calling the ShowStyleEditor() function.");
            ShowStyleEditor();
            TreePop();
            Separator();
        }
        if (TreeNode("Capture/Logging")) {
            DasHelpMarker(
                "The logging API redirects all text output so you can easily capture the content of " +
                "a window or a block. Tree nodes can be automatically expanded.\n" +
                "Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
            LogButtons();
            DasHelpMarker("You can also call LogText() to output directly to the log without a visual output.");
            if (Button("Copy \"Hello, world!\" to clipboard")) {
                LogToClipboard();
                LogText("Hello, world!");
                LogFinish();
            }
            TreePop();
        }
    }

    if (CollapsingHeader("Window options")) {
        if (BeginTable("split", 3)) {
            TableNextColumn();
            Checkbox("No titlebar", safe_addr(no_titlebar));
            TableNextColumn();
            Checkbox("No scrollbar", safe_addr(no_scrollbar));
            TableNextColumn();
            Checkbox("No menu", safe_addr(no_menu));
            TableNextColumn();
            Checkbox("No move", safe_addr(no_move));
            TableNextColumn();
            Checkbox("No resize", safe_addr(no_resize));
            TableNextColumn();
            Checkbox("No collapse", safe_addr(no_collapse));
            TableNextColumn();
            Checkbox("No close", safe_addr(no_close));
            TableNextColumn();
            Checkbox("No nav", safe_addr(no_nav));
            TableNextColumn();
            Checkbox("No background", safe_addr(no_background));
            TableNextColumn();
            Checkbox("No bring to front", safe_addr(no_bring_to_front));
            EndTable();
        }
    }

    DasShowDemoWindowWidgets();
    DasShowDemoWindowLayout();
    DasShowDemoWindowPopups();
    DasShowDemoWindowTables();
    DasShowDemoWindowMisc();
    PopItemWidth();
    End();
}

def DasShowDemoWindowPopups() {
    if (!CollapsingHeader("Popups & Modal windows")) return ;

    if (TreeNode("Popups")) {
        TextWrapped(
            "When a popup is active, it inhibits interacting with windows that are behind the popup. " +
            "Clicking outside the popup closes it.");

        static_let <| $() {
            var selected_fish = -1;
            var toggles <- [ true, false, false, false, false];
            let names <- [ "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish"];
        }

        if (Button("Select..")) {
            OpenPopup("my_select_popup");
        }
        SameLine();
        TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (BeginPopup("my_select_popup")) {
            Text("Aquarium");
            Separator();
            for (name, i in names, count()) {
                if (Selectable(name)) {
                    selected_fish = i;
                }
            }
            EndPopup();
        }
        if (Button("Toggle..")) {
            OpenPopup("my_toggle_popup");
        }
        if (BeginPopup("my_toggle_popup")) {
            for (name, toggle in names, toggles) {
                MenuItem(name, "", toggle);
            }
            if (BeginMenu("Sub-menu")) {
                MenuItem("Click me");
                EndMenu();
            }
            Separator();
            Text("Tooltip here");
            if (IsItemHovered()) {
                SetTooltip("I am a tooltip over a popup");
            }
            if (Button("Stacked Popup")) {
                OpenPopup("another popup");
            }
            if (BeginPopup("another popup")) {
                for (name, toggle in names, toggles) {
                    MenuItem(name, "", toggle);
                }
                if (BeginMenu("Sub-menu")) {
                    MenuItem("Click me");
                    if (Button("Stacked Popup")) {
                        OpenPopup("another popup");
                    }
                    if (BeginPopup("another popup")) {
                        Text("I am the last one here.");
                        EndPopup();
                    }
                    EndMenu();
                }
                EndPopup();
            }
            EndPopup();
        }

        if (Button("File Menu..")) {
            OpenPopup("my_file_popup");
        }
        if (BeginPopup("my_file_popup")) {
            ShowExampleMenuFile();
            EndPopup();
        }
        TreePop();
    }

    if (TreeNode("Context menus")) {
        static_let <| $() {
            var value = 0.5f;
        }
        Text("Value = {value} (<-- right-click here)");
        if (BeginPopupContextItem("item context menu")) {
            if (Selectable("Set to zero")) {
                value = 0.0f;
            }
            if (Selectable("Set to PI")) {
                value = 3.1415f;
            }
            SetNextItemWidth(-1.);
            DragFloat("##Value", safe_addr(value), 0.1f, 0.0f, 0.0f);
            EndPopup();
        }
        Text("(You can also right-click me to open the same popup as above.)");
        OpenPopupOnItemClick("item context menu", ImGuiPopupFlags.MouseButtonRight);
        static_let <| $() {
            var name <- ImGuiInputTextBuffer("Label1", 32);
        }
        var buf = "Button: {name |> to_string}";
        Button(buf);
        if (BeginPopupContextItem()) {
            Text("Edit name:");
            InputText("##edit", name);
            if (Button("Close")) {
                CloseCurrentPopup();
            }
            EndPopup();
        }
        SameLine();
        Text("(<-- right-click here)");
        TreePop();
    }

    if (TreeNode("Modals")) {
        TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside.");
        if (Button("Delete..")) {
            OpenPopup("Delete?");
        }
        var center = float2(GetIO().DisplaySize.x * 0.5f, GetIO().DisplaySize.y * 0.5f);
        SetNextWindowPos(center, ImGuiCond.Appearing, float2(0.5f, 0.5f));
        if (BeginPopupModal("Delete?", null, ImGuiWindowFlags.AlwaysAutoResize)) {
            Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            Separator();
            static_let <| $() {
                var unused_i = 0;
            }
            Combo("Combo", safe_addr(unused_i), fixed_array("Delete", "Delete harder"));
            static_let <| $() {
                var dont_ask_me_next_time = false;
            }
            PushStyleVar(ImGuiStyleVar.FramePadding, float2(0.0f, 0.0f));
            Checkbox("Don't ask me next time", safe_addr(dont_ask_me_next_time));
            PopStyleVar();
            if (Button("OK", float2(120., 0.))) {
                CloseCurrentPopup();
            }
            SetItemDefaultFocus();
            SameLine();
            if (Button("Cancel", float2(120., 0.))) {
                CloseCurrentPopup();
            }
            EndPopup();
        }
        if (Button("Stacked modals..")) {
            OpenPopup("Stacked 1");
        }
        if (BeginPopupModal("Stacked 1", null, ImGuiWindowFlags.MenuBar)) {
            if (BeginMenuBar()) {
                if (BeginMenu("File")) {
                    if (MenuItem("Some menu item")) {
                        pass;
                    }
                    EndMenu();
                }
                EndMenuBar();
            }
            Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");
            static_let <| $() {
                var item = 1;
                var color = fixed_array<float>(0.4f, 0.7f, 0.0f, 0.5f);
            }
            Combo("Combo", safe_addr(item), fixed_array("aaaa", "bbbb", "cccc", "dddd", "eeee"));
            ColorEdit4("color", safe_addr(color[0]));
            if (Button("Add another modal..")) {
                OpenPopup("Stacked 2");
            }
            static_let <| $() {
                var unused_open = true;
            }
            if (BeginPopupModal("Stacked 2", safe_addr(unused_open))) {
                Text("Hello from Stacked The Second!");
                if (Button("Close")) {
                    CloseCurrentPopup();
                }
                EndPopup();
            }
            if (Button("Close")) {
                CloseCurrentPopup();
            }
            EndPopup();
        }
        TreePop();
    }

    if (TreeNode("Menus inside a regular window")) {
        TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
        Separator();
        PushID("foo");
        MenuItem("Menu item", "CTRL+M");
        if (BeginMenu("Menu inside a regular window")) {
            ShowExampleMenuFile();
            EndMenu();
        }
        PopID();
        Separator();
        TreePop();
    }
}

def ShowExampleMenuFile() {
    MenuItem("(demo menu)", "", false, false);
    if (MenuItem("New")) {
        pass;
    }
    if (MenuItem("Open", "Ctrl+O")) {
        pass;
    }
    if (BeginMenu("Open Recent")) {
        MenuItem("fish_hat.c");
        MenuItem("fish_hat.inl");
        MenuItem("fish_hat.h");
        if (BeginMenu("More..")) {
            MenuItem("Hello");
            MenuItem("Sailor");
            if (BeginMenu("Recurse..")) {
                ShowExampleMenuFile();
                EndMenu();
            }
            EndMenu();
        }
        EndMenu();
    }
    if (MenuItem("Save", "Ctrl+S")) {
        pass;
    }
    if (MenuItem("Save As..")) {
        pass;
    }

    Separator();
    if (BeginMenu("Options")) {
        static_let <| $() {
            var enabled = true;
        }
        MenuItem("Enabled", "", enabled);
        BeginChild("child", float2(0., 60.), ImGuiChildFlags.Border);
        for (i in range(1)) {
            Text("Scrolling Text {i}");
        }
        EndChild();
        static_let <| $() {
            var f = 0.5f;
            var n = 0;
        }
        SliderFloat("Value", safe_addr(f), 0.0f, 1.0f);
        InputFloat("Input", safe_addr(f), 0.1f);
        Combo("Combo", safe_addr(n), fixed_array("Yes", "No", "Maybe"));
        EndMenu();
    }

    if (BeginMenu("Colors")) {
        let sz = GetTextLineHeight();
        for (col in each_enum(type<ImGuiCol>)) {
            if (col == ImGuiCol.COUNT) continue;
            let name = GetStyleColorName(col);
            let p = GetCursorScreenPos();
            *GetWindowDrawList() |> AddRectFilled(p, float2(p.x + sz, p.y + sz), GetColorU32(col));
            Dummy(float2(sz, sz));
            SameLine();
            MenuItem(name);
        }
        EndMenu();
    }

    if (BeginMenu("Options")) {
        static_let <| $() {
            var b = true;
        }
        Checkbox("SomeOption", safe_addr(b));
        EndMenu();
    }

    if (BeginMenu("Disabled", false)) {
        assert(false);
    }
    if (MenuItem("Checked", "", true)) {
        pass;
    }
    if (MenuItem("Quit", "Alt+F4")) {
        pass;
    }
}

def DasShowDemoWindowMisc() {
    if (CollapsingHeader("Filtering")) {
        static_let <| $() {
            var filter = new ImGuiTextFilter;
        }
        Text("Filter usage:\n" +
                    "  \"\"         display all lines\n" +
                    "  \"xxx\"      display lines containing \"xxx\"\n" +
                    "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n" +
                    "  \"-xxx\"     hide lines containing \"xxx\"");

        *filter |> Draw();
        static_let <| $() {
            var lines = fixed_array("aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world");
        }
        for (ln in lines) {
            if (*filter |> PassFilter(ln)) {
                BulletText("{ln}");
            }
        }
    }

    if (CollapsingHeader("Inputs, Navigation & Focus")) {
        var io & = unsafe(GetIO());

        Text("WantCaptureMouse: {io.WantCaptureMouse}");
        Text("WantCaptureKeyboard: {io.WantCaptureKeyboard}");
        Text("WantTextInput: {io.WantTextInput}");
        Text("WantSetMousePos: {io.WantSetMousePos}");
        Text("NavActive: {io.NavActive}, NavVisible: {io.NavVisible}");

        if (TreeNode("Keyboard, Mouse & Navigation State")) {
            if (IsMousePosValid()) {
                Text("Mouse pos: ({io.MousePos.x}, {io.MousePos.y})");
            } else {
                Text("Mouse pos: <INVALID>");
            }
            Text("Mouse delta: ({io.MouseDelta.x}, {io.MouseDelta.y})");
            Text("Mouse down:");
            for (i in range(length(io.MouseDown))) {
                if (io.MouseDownDuration[i] >= 0.0f) {
                    SameLine();
                    Text("b{i} ({io.MouseDownDuration[i]} secs)");
                }
            }
            Text("Mouse clicked:");
            for (i in each_enum(type<ImGuiMouseButton>)) {
                if (i != ImGuiMouseButton.COUNT && IsMouseClicked(i)) {
                    SameLine();
                    Text("{i}");
                }
            }
            Text("Mouse dblclick:");
            for (i in each_enum(type<ImGuiMouseButton>)) {
                if (i != ImGuiMouseButton.COUNT && IsMouseDoubleClicked(i)) {
                    SameLine();
                    Text("b{i}");
                }
            }
            Text("Mouse released:");
            for (i in each_enum(type<ImGuiMouseButton>)) {
                if (i != ImGuiMouseButton.COUNT && IsMouseReleased(i)) {
                    SameLine();
                    Text("b{i}");
                }
            }
            Text("Mouse wheel: {io.MouseWheel}");
            Text("Keys down:");
            for (i in range(int(ImGuiKey.NamedKey_BEGIN), int(ImGuiKey.NamedKey_END))) {
                let key = unsafe(reinterpret<ImGuiKey> i);
                if (IsKeyDown(key)) {
                    SameLine();
                    Text("\"{GetKeyName(key)}\" {i}");
                }
            }
            Text("Keys mods: {io.KeyCtrl ? "CTRL " : ""}{io.KeyShift ? "SHIFT " : ""}{io.KeyAlt ? "ALT " : ""}{io.KeySuper ? "SUPER " : ""}");
            Text("Chars queue:");
            for (i in range(io.InputQueueCharacters |> length)) {
                let c = int(io.InputQueueCharacters[i]);
                SameLine();
                Text("'{(c > ' ' && c <= 255) ? to_char(c) : "?"}' (0x{uint(c)})");
            }
            TreePop();
        }

        if (TreeNode("Tabbing")) {
            static_let <| $() {
                var buft <- ImGuiInputTextBuffer("hello", 32);
            }
            Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
            InputText("1", buft);
            InputText("2", buft);
            InputText("3", buft);
            PushTabStop(false);
            InputText("4 (tab skip)", buft);
            PopTabStop();
            InputText("5", buft);
            TreePop();
        }

        if (TreeNode("Focus from code")) {
            var focus_1 = Button("Focus on 1");
            SameLine();
            var focus_2 = Button("Focus on 2");
            SameLine();
            var focus_3 = Button("Focus on 3");
            var has_focus = 0;
            static_let <| $() {
                var buf <- ImGuiInputTextBuffer("click on a button to set focus", 128);
            }
            if (focus_1) {
                SetKeyboardFocusHere();
            }
            InputText("1", buf);
            if (IsItemActive()) {
                has_focus = 1;
            }
            if (focus_2) {
                SetKeyboardFocusHere();
            }
            InputText("2", buf);
            if (IsItemActive()) {
                has_focus = 2;
            }
            PushTabStop(false);
            if (focus_3) {
                SetKeyboardFocusHere();
            }
            InputText("3 (tab skip)", buf);
            if (IsItemActive()) {
                has_focus = 3;
            }
            PopTabStop();
            if (has_focus != 0) {
                Text("Item with focus: {has_focus}");
            } else {
                Text("Item with focus: <none>");
            }
            static_let <| $() {
                var f3 = fixed_array<float>(0.0f, 0.0f, 0.0f);
            }
            var focus_ahead = -1;
            if (Button("Focus on X")) {
                focus_ahead = 0;
            }
            SameLine();
            if (Button("Focus on Y")) {
                focus_ahead = 1;
            }
            SameLine();
            if (Button("Focus on Z")) {
                focus_ahead = 2;
            }
            if (focus_ahead != -1) {
                SetKeyboardFocusHere(focus_ahead);
            }
            SliderFloat3("Float3", safe_addr(f3[0]), 0.0f, 1.0f);
            TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            TreePop();
        }

        if (TreeNode("Dragging")) {
            TextWrapped("You can use GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (button in each_enum(type<ImGuiMouseButton>)) {
                if (button != ImGuiMouseButton.COUNT) {
                    Text("IsMouseDragging({button}):");
                    Text("  w/ default threshold: {IsMouseDragging(button)},");
                    Text("  w/ zero threshold: {IsMouseDragging(button, 0.0f)},");
                    Text("  w/ large threshold: {IsMouseDragging(button, 20.0f)},");
                }
            }
            Button("Drag Me");
            if (IsItemActive()) {
                *GetForegroundDrawList() |> AddLine(
                    io.MouseClickedPos[0],
                    io.MousePos,
                    GetColorU32(ImGuiCol.Button),
                    4.0f);
            }
            let value_raw = GetMouseDragDelta(ImGuiMouseButton.Left, 0.0f);
            let value_with_lock_threshold = GetMouseDragDelta(ImGuiMouseButton.Left);
            let mouse_delta = io.MouseDelta;
            Text("GetMouseDragDelta(0):");
            Text("  w/ default threshold: ({value_with_lock_threshold.x}, {value_with_lock_threshold.y})");
            Text("  w/ zero threshold: ({value_raw.x}, {value_raw.y})");
            Text("io.MouseDelta: ({mouse_delta.x}, {mouse_delta.y})");
            TreePop();
        }

        if (TreeNode("Mouse cursors")) {
            static_let <| $() {
                var mouse_cursors_names = fixed_array("Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed");
            }
            let current = GetMouseCursor();
            Text("Current mouse cursor = {current}: {mouse_cursors_names[int(current)]}");
            Text("Hover to see mouse cursors:");
            SameLine();
            DasHelpMarker(
                "Your application can render a different mouse cursor based on what GetMouseCursor() returns. " +
                "If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, " +
                "otherwise your backend needs to handle it.");
            for (mcn, i in mouse_cursors_names, count()) {
                let lab = "Mouse cursor {i}: {mcn}";
                Bullet();
                Selectable(lab, false);
                if (IsItemHovered()) {
                    unsafe {
                        SetMouseCursor(reinterpret<ImGuiMouseCursor> i);
                    }
                }
            }
            TreePop();
        }
    }
}

def DasShowDemoWindowLayout {
    if (!CollapsingHeader("Layout & Scrolling")) return ;

    if (TreeNode("Child windows")) {
        DasHelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
        static_let <| $() {
            var disable_mouse_wheel = false;
            var disable_menu = false;
        }
        Checkbox("Disable Mouse Wheel", safe_addr(disable_mouse_wheel));
        Checkbox("Disable Menu", safe_addr(disable_menu));
        {
            var window_flags = ImGuiWindowFlags.HorizontalScrollbar;
            if (disable_mouse_wheel) {
                window_flags |= ImGuiWindowFlags.NoScrollWithMouse;
            }
            BeginChild("ChildL", float2((GetWindowContentRegionMax().x - GetWindowContentRegionMin().x) * 0.5f, 260.), ImGuiChildFlags.None, window_flags);
            for (i in range(100)) {
                Text("{i}: scrollable region");
            }
            EndChild();
        }
        SameLine();
        {
            var window_flags = ImGuiWindowFlags.None;
            if (disable_mouse_wheel) {
                window_flags |= ImGuiWindowFlags.NoScrollWithMouse;
            }
            if (!disable_menu) {
                window_flags |= ImGuiWindowFlags.MenuBar;
            }
            PushStyleVar(ImGuiStyleVar.ChildRounding, 5.0f);
            BeginChild("ChildR", float2(0., 260.), ImGuiChildFlags.Border, window_flags);
            if (!disable_menu && BeginMenuBar()) {
                if (BeginMenu("Menu")) {
                    ShowExampleMenuFile();
                    EndMenu();
                }
                EndMenuBar();
            }
            if (BeginTable("split", 2, ImGuiTableFlags.Resizable | ImGuiTableFlags.NoSavedSettings)) {
                for (i in range(100)) {
                    TableNextColumn();
                    Button("{i}", float2(-FLT_MIN, 0.0f));
                }
                EndTable();
            }
            EndChild();
            PopStyleVar();
        }
        Separator();
        {
            static_let <| $() {
                var offset_x = 0;
            }
            SetNextItemWidth(100.);
            DragInt("Offset X", safe_addr(offset_x), 1.0f, -1000, 1000);
            SetCursorPosX(GetCursorPosX() + float(offset_x));
            PushStyleColor(ImGuiCol.ChildBg, IM_COL32(255, 0, 0, 100));
            BeginChild("Red", float2(200., 100.), ImGuiChildFlags.Border, ImGuiWindowFlags.None);
            for (i in range(50)) {
                Text("Some test {i}");
            }
            EndChild();
            let child_is_hovered = IsItemHovered();
            let child_rect_min = GetItemRectMin();
            let child_rect_max = GetItemRectMax();
            PopStyleColor();
            Text("Hovered: {child_is_hovered}");
            Text("Rect of child window is: ({child_rect_min.x},{child_rect_min.y}) ({child_rect_max.x}, {child_rect_max.y})");
        }
        TreePop();
    }

    if (TreeNode("Widgets Width")) {
        static_let <| $() {
            var f = 0.0f;
            var show_indented_items = true;
        }
        Checkbox("Show indented items", safe_addr(show_indented_items));
        Text("SetNextItemWidth/PushItemWidth(100)");
        SameLine();
        DasHelpMarker("Fixed width.");
        PushItemWidth(100.);
        DragFloat("float##1b", safe_addr(f));
        if (show_indented_items) {
            Indent();
            DragFloat("float (indented)##1b", safe_addr(f));
            Unindent();
        }
        PopItemWidth();
        Text("SetNextItemWidth/PushItemWidth(-100)");
        SameLine();
        DasHelpMarker("Align to right edge minus 100");
        PushItemWidth(-100.);
        DragFloat("float##2a", safe_addr(f));
        if (show_indented_items) {
            Indent();
            DragFloat("float (indented)##2b", safe_addr(f));
            Unindent();
        }
        PopItemWidth();
        Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
        SameLine();
        DasHelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
        PushItemWidth(GetContentRegionAvail().x * 0.5f);
        DragFloat("float##3a", safe_addr(f));
        if (show_indented_items) {
            Indent();
            DragFloat("float (indented)##3b", safe_addr(f));
            Unindent();
        }
        PopItemWidth();
        Text("SetNextItemWidth/PushItemWidth(-GetContentRegionAvail().x * 0.5f)");
        SameLine();
        DasHelpMarker("Align to right edge minus half");
        PushItemWidth(-GetContentRegionAvail().x * 0.5f);
        DragFloat("float##4a", safe_addr(f));
        if (show_indented_items) {
            Indent();
            DragFloat("float (indented)##4b", safe_addr(f));
            Unindent();
        }
        PopItemWidth();
        Text("SetNextItemWidth/PushItemWidth(-FLT_MIN)");
        SameLine();
        DasHelpMarker("Align to right edge");
        PushItemWidth(-FLT_MIN);
        DragFloat("##float5a", safe_addr(f));
        if (show_indented_items) {
            Indent();
            DragFloat("float (indented)##5b", safe_addr(f));
            Unindent();
        }
        PopItemWidth();
        TreePop();
    }

    if (TreeNode("Basic Horizontal Layout")) {
        TextWrapped("(Use SameLine() to keep adding items to the right of the preceding item)");
        Text("Two items: Hello");
        SameLine();
        TextColored(float4(1., 1., 0., 1.), "Sailor");
        Text("More spacing: Hello");
        SameLine(0., 20.);
        TextColored(float4(1., 1., 0., 1.), "Sailor");
        AlignTextToFramePadding();
        Text("Normal buttons");
        SameLine();
        Button("Banana");
        SameLine();
        Button("Apple");
        SameLine();
        Button("Corniflower");
        Text("Small buttons");
        SameLine();
        SmallButton("Like this one");
        SameLine();
        Text("can fit within a text block.");
        Text("Aligned");
        SameLine(150.);
        Text("x=150");
        SameLine(300.);
        Text("x=300");
        Text("Aligned");
        SameLine(150.);
        SmallButton("x=150");
        SameLine(300.);
        SmallButton("x=300");
        static_let <| $() {
            var c1 = false;
            var c2 = false;
            var c3 = false;
            var c4 = false;
        }
        Checkbox("My", safe_addr(c1));
        SameLine();
        Checkbox("Tailor", safe_addr(c2));
        SameLine();
        Checkbox("Is", safe_addr(c3));
        SameLine();
        Checkbox("Rich", safe_addr(c4));
        static_let <| $() {
            var f0 = 1.0f;
            var f1 = 2.0f;
            var f2 = 3.0f;
        }
        PushItemWidth(80.);
        static_let <| $() {
            let items = fixed_array("AAAA", "BBBB", "CCCC", "DDDD");
            var item = -1;
        }
        Combo("Combo", safe_addr(item), items);
        SameLine();
        SliderFloat("X", safe_addr(f0), 0.0f, 5.0f);
        SameLine();
        SliderFloat("Y", safe_addr(f1), 0.0f, 5.0f);
        SameLine();
        SliderFloat("Z", safe_addr(f2), 0.0f, 5.0f);
        PopItemWidth();
        PushItemWidth(80.);
        Text("Lists:");
        static_let <| $() {
            var selection = fixed_array(0, 1, 2, 3);
        }
        for (i in range(4)) {
            if (i > 0) {
                SameLine();
            }
            PushID(i);
            ListBox("", safe_addr(selection[i]), items);
            PopID();
        }
        PopItemWidth();
        let button_sz = float2(40., 40.);
        Button("A", button_sz);
        SameLine();
        Dummy(button_sz);
        SameLine();
        Button("B", button_sz);
        Text("Manually wrapping:");
        let style & = unsafe(GetStyle());
        let buttons_count = 20;
        let window_visible_x2 = GetWindowPos().x + GetWindowContentRegionMax().x;
        for (n in range(buttons_count)) {
            PushID(n);
            Button("Box", button_sz);
            let last_button_x2 = GetItemRectMax().x;
            let next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x;
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) {
                SameLine();
            }
            PopID();
        }
        TreePop();
    }

    if (TreeNode("Groups")) {
        DasHelpMarker(
            "BeginGroup() basically locks the horizontal position for new line. " +
            "EndGroup() bundles the whole group so that you can use \"item\" functions such as " +
            "IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
        BeginGroup();
        BeginGroup();
        Button("AAA");
        SameLine();
        Button("BBB");
        SameLine();
        BeginGroup();
        Button("CCC");
        Button("DDD");
        EndGroup();
        SameLine();
        Button("EEE");
        EndGroup();
        if (IsItemHovered()) {
            SetTooltip("First group hovered");
        }
        var size = GetItemRectSize();
        static_let <| $() {
            let values = fixed_array(0.5f, 0.20f, 0.80f, 0.60f, 0.25f);
        }
        PlotHistogram("##values", values, 0, "", 0.0f, 1.0f, size);
        Button("ACTION", float2((size.x - GetStyle().ItemSpacing.x) * 0.5f, size.y));
        SameLine();
        Button("REACTION", float2((size.x - GetStyle().ItemSpacing.x) * 0.5f, size.y));
        EndGroup();
        SameLine();
        Button("LEVERAGE\nBUZZWORD", size);
        SameLine();
        /*
        if ListBoxHeader("List", size)
            Selectable("Selected", true)
            Selectable("Not Selected", false)
            ListBoxFooter()
        */
        TreePop();
    }

    if (TreeNode("Text Baseline Alignment")) {
        BulletText("Text baseline:");
        SameLine();
        DasHelpMarker(
            "This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. " +
            "Lines only composed of text or \"small\" widgets use less vertical space than lines with framed widgets.");
        Indent();
        Text("KO Blahblah");
        SameLine();
        Button("Some framed item");
        SameLine();
        DasHelpMarker("Baseline of button will look misaligned with text..");
        AlignTextToFramePadding();
        Text("OK Blahblah");
        SameLine();
        Button("Some framed item");
        SameLine();
        DasHelpMarker("We call AlignTextToFramePadding() to vertically align the text baseline by +FramePadding.y");
        Button("TEST##1");
        SameLine();
        Text("TEST");
        SameLine();
        SmallButton("TEST##2");
        AlignTextToFramePadding();
        Text("Text aligned to framed item");
        SameLine();
        Button("Item##1");
        SameLine();
        Text("Item");
        SameLine();
        SmallButton("Item##2");
        SameLine();
        Button("Item##3");
        Unindent();
        Spacing();
        BulletText("Multi-line text:");
        Indent();
        Text("One\nTwo\nThree");
        SameLine();
        Text("Hello\nWorld");
        SameLine();
        Text("Banana");
        Text("Banana");
        SameLine();
        Text("Hello\nWorld");
        SameLine();
        Text("One\nTwo\nThree");
        Button("HOP##1");
        SameLine();
        Text("Banana");
        SameLine();
        Text("Hello\nWorld");
        SameLine();
        Text("Banana");
        Button("HOP##2");
        SameLine();
        Text("Hello\nWorld");
        SameLine();
        Text("Banana");
        Unindent();
        Spacing();
        BulletText("Misc items:");
        Indent();
        Button("80x80", float2(80., 80.));
        SameLine();
        Button("50x50", float2(50., 50.));
        SameLine();
        Button("Button()");
        SameLine();
        SmallButton("SmallButton()");
        let spacing = GetStyle().ItemInnerSpacing.x;
        Button("Button##1");
        SameLine(0.0f, spacing);
        if (TreeNode("Node##1")) {
            for (i in range(6)) {
                BulletText("Item {i}..");
            }
            TreePop();
        }
        AlignTextToFramePadding();
        let node_open = TreeNode("Node##2");
        SameLine(0.0f, spacing);
        Button("Button##2");
        if (node_open) {
            for (i in range(6)) {
                BulletText("Item {i}..");
            }
            TreePop();
        }
        Button("Button##3");
        SameLine(0.0f, spacing);
        BulletText("Bullet text");
        AlignTextToFramePadding();
        BulletText("Node");
        SameLine(0.0f, spacing);
        Button("Button##4");
        Unindent();
        TreePop();
    }

    if (TreeNode("Scrolling")) {
        DasHelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position.");
        static_let <| $() {
            var track_item = 50;
            var enable_track = true;
            var enable_extra_decorations = false;
            var scroll_to_off_px = 0.0f;
            var scroll_to_pos_px = 200.0f;
        }
        Checkbox("Decoration", safe_addr(enable_extra_decorations));
        Checkbox("Track", safe_addr(enable_track));
        PushItemWidth(100.);
        SameLine(140.);
        enable_track = DragInt("##item", safe_addr(track_item), 0.25f, 0, 99, "Item = %d") || enable_track;
        var scroll_to_off = Button("Scroll Offset");
        SameLine(140.);
        scroll_to_off = DragFloat("##off", safe_addr(scroll_to_off_px), 1.00f, 0., FLT_MAX, "+%.0f px") || scroll_to_off;
        var scroll_to_pos = Button("Scroll To Pos");
        SameLine(140.);
        scroll_to_pos = DragFloat("##pos", safe_addr(scroll_to_pos_px), 1.00f, -10., FLT_MAX, "X/Y = %.0f px") || scroll_to_pos;
        PopItemWidth();
        if (scroll_to_off || scroll_to_pos) {
            enable_track = false;
        }
        var style & = unsafe(GetStyle());
        var child_w = (GetContentRegionAvail().x - 4. * style.ItemSpacing.x) / 5.;
        if (child_w < 1.0f) {
            child_w = 1.0f;
        }
        PushID("##VerticalScrolling");
        for (i in range(5)) {
            if (i > 0) {
                SameLine();
            }
            BeginGroup();
            static_let <| $() {
                let names_v = fixed_array("Top", "25%", "Center", "75%", "Bottom");
            }
            TextUnformatted(names_v[i]);
            let child_flags = enable_extra_decorations ? ImGuiWindowFlags.MenuBar : ImGuiWindowFlags.None;
            var child_id : uint;
            unsafe {
                child_id = GetID(reinterpret<void?> i);
            }
            let child_is_visible = BeginChild(child_id, float2(child_w, 200.0f), ImGuiChildFlags.Border, child_flags);
            if (BeginMenuBar()) {
                TextUnformatted("abc");
                EndMenuBar();
            }
            if (scroll_to_off) {
                SetScrollY(scroll_to_off_px);
            }
            if (scroll_to_pos) {
                SetScrollFromPosY(GetCursorStartPos().y + scroll_to_pos_px, float(i) * 0.25f);
            }
            if (child_is_visible) {
                for (item in range(100)) {
                    if (enable_track && item == track_item) {
                        TextColored(float4(1., 1., 0., 1.), "Item {item}");
                        SetScrollHereY(float(i) * 0.25f);
                    } else {
                        Text("Item {item}");
                    }
                }
            }
            let scroll_y = GetScrollY();
            let scroll_max_y = GetScrollMaxY();
            EndChild();
            Text("{scroll_y}/{scroll_max_y}");
            EndGroup();
        }
        PopID();
        Spacing();
        DasHelpMarker(
            "Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\n" +
            "Because the clipping rectangle of most window hides half worth of WindowPadding on the " +
            "left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the " +
            "equivalent SetScrollFromPosY(+1) wouldn't.");
        PushID("##HorizontalScrolling");
        for (i in range(5)) {
            let child_height = GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
            let child_flags = ImGuiWindowFlags.HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags.AlwaysVerticalScrollbar : ImGuiWindowFlags.None);
            var child_id : uint;
            unsafe {
                child_id = GetID(reinterpret<void?> i);
            }
            let child_is_visible = BeginChild(child_id, float2(-100., child_height), ImGuiChildFlags.Border, child_flags);
            if (scroll_to_off) {
                SetScrollX(scroll_to_off_px);
            }
            if (scroll_to_pos) {
                SetScrollFromPosX(GetCursorStartPos().x + scroll_to_pos_px, float(i) * 0.25f);
            }
            if (child_is_visible) {
                for (item in range(100)) {
                    if (enable_track && item == track_item) {
                        TextColored(float4(1., 1., 0., 1.), "Item {item}");
                        SetScrollHereX(float(i) * 0.25f);
                    } else {
                        Text("Item {item}");
                    }
                    SameLine();
                }
            }
            let scroll_x = GetScrollX();
            let scroll_max_x = GetScrollMaxX();
            EndChild();
            SameLine();
            static_let <| $() {
                let names_h = fixed_array("Left", "25%", "Center", "75%", "Right");
            }
            Text("{names_h[i]}\n{scroll_x}/{scroll_max_x}");
            Spacing();
        }
        PopID();
        DasHelpMarker(
            "Horizontal scrolling for a window is enabled via the ImGuiWindowFlags HorizontalScrollbar flag.\n\n" +
            "You may want to also explicitly specify content width by using SetNextWindowContentWidth() before Begin().");
        static_let <| $() {
            var lines = 7;
        }
        SliderInt("Lines", safe_addr(lines), 1, 15);
        PushStyleVar(ImGuiStyleVar.FrameRounding, 3.0f);
        PushStyleVar(ImGuiStyleVar.FramePadding, float2(2.0f, 1.0f));
        let scrolling_child_size = float2(0., GetFrameHeightWithSpacing() * 7.0 + 30.0);
        BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags.Border, ImGuiWindowFlags.HorizontalScrollbar);
        for (line in range(lines)) {
            let num_buttons = 10 + ((line & 1) != 0 ? line * 9 : line * 3);
            for (n in range(num_buttons)) {
                if (n > 0) {
                    SameLine();
                }
                PushID(n + line * 1000);
                let num_buf = "{n}";
                let lab = ((n % 15) == 0) ? "FizzBuzz" : ((n % 3) == 0) ? "Fizz" : ((n % 5) == 0) ? "Buzz" : num_buf;
                let hue = float(n) * 0.05f;
                PushStyleColor(ImGuiCol.Button, HSV(hue, 0.6f, 0.6f));
                PushStyleColor(ImGuiCol.ButtonHovered,  HSV(hue, 0.7f, 0.7f));
                PushStyleColor(ImGuiCol.ButtonActive, HSV(hue, 0.8f, 0.8f));
                Button(lab, float2(40.0f + sin(float(line + n)) * 20.0f, 0.0f));
                PopStyleColor(3);
                PopID();
            }
        }
        let scroll_x = GetScrollX();
        let scroll_max_x = GetScrollMaxX();
        EndChild();
        PopStyleVar(2);
        var scroll_x_delta = 0.0f;
        SmallButton("<<");
        if (IsItemActive()) {
            scroll_x_delta = -GetIO().DeltaTime * 1000.0f;
        }
        SameLine();
        Text("Scroll from code");
        SameLine();
        SmallButton(">>");
        if (IsItemActive()) {
            scroll_x_delta = +GetIO().DeltaTime * 1000.0f;
        }
        SameLine();
        Text("{scroll_x}/{scroll_max_x}");
        if (scroll_x_delta != 0.0f) {
            BeginChild("scrolling", float2(0., 0.));
            SetScrollX(GetScrollX() + scroll_x_delta);
            EndChild();
        }
        Spacing();
        static_let <| $() {
            var show_horizontal_contents_size_demo_window = false;
        }
        Checkbox("Show Horizontal contents size demo window", safe_addr(show_horizontal_contents_size_demo_window));
        if (show_horizontal_contents_size_demo_window) {
            static_let <| $() {
                var show_h_scrollbar = true;
                var show_button = true;
                var show_tree_nodes = true;
                var show_text_wrapped = false;
                var show_columns = true;
                var show_tab_bar = true;
                var show_child = false;
                var explicit_content_size = false;
                var contents_size_x = 300.0f;
            }
            if (explicit_content_size) {
                SetNextWindowContentSize(float2(contents_size_x, 0.0f));
            }
            Begin("Horizontal contents size demo window", safe_addr(show_horizontal_contents_size_demo_window),
                show_h_scrollbar ? ImGuiWindowFlags.HorizontalScrollbar : ImGuiWindowFlags.None);
            PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(2., 0.));
            PushStyleVar(ImGuiStyleVar.FramePadding, float2(2., 0.));
            DasHelpMarker("Test of different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\nUse 'Metrics->Tools->Show windows rectangles' to visualize rectangles.");
            Checkbox("H-scrollbar", safe_addr(show_h_scrollbar));
            Checkbox("Button", safe_addr(show_button));
            Checkbox("Tree nodes", safe_addr(show_tree_nodes));
            Checkbox("Text wrapped", safe_addr(show_text_wrapped));
            Checkbox("Columns", safe_addr(show_columns));
            Checkbox("Tab bar", safe_addr(show_tab_bar));
            Checkbox("Child", safe_addr(show_child));
            Checkbox("Explicit content size", safe_addr(explicit_content_size));
            Text("Scroll {GetScrollX()}/{GetScrollMaxX()} {GetScrollY()}/{GetScrollMaxY()}");
            if (explicit_content_size) {
                SameLine();
                SetNextItemWidth(100.);
                DragFloat("##csx", safe_addr(contents_size_x));
                var p = GetCursorScreenPos();
                *GetWindowDrawList() |> AddRectFilled(p, float2(p.x + 10., p.y + 10.), IM_COL32_WHITE);
                *GetWindowDrawList() |> AddRectFilled(float2(p.x + contents_size_x - 10., p.y), float2(p.x + contents_size_x, p.y + 10.), IM_COL32_WHITE);
                Dummy(float2(0., 10.));
            }
            PopStyleVar(2);
            Separator();
            if (show_button) {
                Button("this is a 300-wide button", float2(300., 0.));
            }
            if (show_tree_nodes) {
                var open = true;
                if (TreeNode("this is a tree node")) {
                    if (TreeNode("another one of those tree node...")) {
                        Text("Some tree contents");
                        TreePop();
                    }
                    TreePop();
                }
                CollapsingHeader("CollapsingHeader", safe_addr(open));
            }
            if (show_text_wrapped) {
                TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
            }
            if (show_columns) {
                Text("Tables:");
                if (BeginTable("table", 4, ImGuiTableFlags.Borders)) {
                    for (n in range(4)) {
                        TableNextColumn();
                        Text("Width {GetContentRegionAvail().x}");
                    }
                    EndTable();
                }
                Text("Columns:");
                Columns(4);
                for (n in range(4)) {
                    Text("Width {GetColumnWidth()}");
                    NextColumn();
                }
                Columns(1);
            }
            if (show_tab_bar && BeginTabBar("Hello")) {
                if (BeginTabItem("OneOneOne")) {
                    EndTabItem();
                }
                if (BeginTabItem("TwoTwoTwo")) {
                    EndTabItem();
                }
                if (BeginTabItem("ThreeThreeThree")) {
                    EndTabItem();
                }
                if (BeginTabItem("FourFourFour")) {
                    EndTabItem();
                }
                EndTabBar();
            }
            if (show_child) {
                BeginChild("child", float2(0., 0.), ImGuiChildFlags.Border);
                EndChild();
            }
            End();
        }
        TreePop();
    }

    if (TreeNode("Clipping")) {
        static_let <| $() {
            var size = float2(100.0f, 100.0f);
            var offset = float2(30.0f, 30.0f);
        }
        DragFloat2("size", safe_addr(size.x), 0.5f, 1.0f, 200.0f, "%.0f");
        TextWrapped("(Click and drag to scroll)");
        for (n in range(3)) {
            if (n > 0) {
                SameLine();
            }
            PushID(n);
            BeginGroup();
            InvisibleButton("##empty", size);
            if (IsItemActive() && IsMouseDragging(ImGuiMouseButton.Left)) {
                offset.x += GetIO().MouseDelta.x;
                offset.y += GetIO().MouseDelta.y;
            }
            var p0 = GetItemRectMin();
            var p1 = GetItemRectMax();
            let text_str = "Line 1 hello\nLine 2 clip me!";
            let text_pos = float2(p0.x + offset.x, p0.y + offset.y);
            var draw_list = GetWindowDrawList();
            if (n == 0) {
                DasHelpMarker(
                    "Using PushClipRect():\n" +
                    "Will alter ImGui hit-testing logic + ImDrawList rendering.\n" +
                    "(use this if you want your clipping rectangle to affect interactions)");
                PushClipRect(p0, p1, true);
                *draw_list |> AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                *draw_list |> AddText(text_pos, IM_COL32_WHITE, text_str);
                PopClipRect();
            } elif (n == 1) {
                DasHelpMarker(
                    "Using ImDrawList::PushClipRect():\n" +
                    "Will alter ImDrawList rendering only.\n" +
                    "(use this as a shortcut if you are only using ImDrawList calls)");
                *draw_list |> PushClipRect(p0, p1, true);
                *draw_list |> AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                *draw_list |> AddText(text_pos, IM_COL32_WHITE, text_str);
                *draw_list |> PopClipRect();
            } elif (n == 2) {
                DasHelpMarker(
                    "Using ImDrawList::AddText() with a fine ClipRect:\n" +
                    "Will alter only this specific ImDrawList::AddText() rendering.\n" +
                    "(this is often used internally to avoid altering the clipping rectangle and minimize draw calls)");
                var clip_rect = float4(p0.x, p0.y, p1.x, p1.y);
                *draw_list |> AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
                *draw_list |> AddText(GetFont(), GetFontSize(), text_pos, IM_COL32_WHITE, text_str, 0.0f, safe_addr(clip_rect));
            }
            EndGroup();
            PopID();
        }
        TreePop();
    }
}

class ExampleAppLog {
    Buf : ImGuiTextBuffer? = new ImGuiTextBuffer;
    Filter : ImGuiTextFilter? = new ImGuiTextFilter;
    LineOffsets : array<int>;
    AutoScroll : bool = true;
    def ExampleAppLog() {
        self->Clear();
    }
    def finalize {
        unsafe {
            delete Buf;
            delete Filter;
        }
    }
    def Clear() : void {
        *Buf |> clear();
        LineOffsets |> clear();
        LineOffsets |> push(0);
    }
    def AddLog(txt : string) : void {
        var old_size = *Buf |> size();
        *Buf |> append(txt);
        var new_size = *Buf |> size();
        while (old_size < new_size) {
            if (*Buf |> at(old_size) == '\n') {
                LineOffsets |> push(old_size + 1);
            }
            old_size ++;
        }
    }
    def Draw(title : string; p_open : bool? implicit) : void {
        if (!Begin(title, p_open)) {
            End();
            return ;
        }
        if (BeginPopup("Options")) {
            Checkbox("Auto-scroll", unsafe(addr(AutoScroll)));
            EndPopup();
        }
        if (Button("Options")) {
            OpenPopup("Options");
        }
        SameLine();
        let clear = Button("Clear");
        SameLine();
        let copy = Button("Copy");
        SameLine();
        *Filter |> Draw("Filter", -100.0f);
        Separator();
        BeginChild("scrolling", float2(0., 0.), ImGuiChildFlags.None, ImGuiWindowFlags.HorizontalScrollbar);
        if (clear) {
            self->Clear();
        }
        if (copy) {
            LogToClipboard();
        }
        PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(0., 0.));
        if (*Filter |> IsActive()) {
            var line_no = 0;
            while (line_no < length(LineOffsets)) {
                var line_start = LineOffsets[line_no];
                var line_end = (line_no + 1 < length(LineOffsets)) ? (LineOffsets[line_no + 1] - 1) : (*Buf |> size());
                var ltext = *Buf |> slice(line_start, line_end);
                if (*Filter |> PassFilter(ltext)) {
                    TextUnformatted(ltext);
                }
                line_no++;
            }
        } else {
            using() <| $(var clipper : ImGuiListClipper) {
                clipper |> Begin(length(LineOffsets));
                while (clipper |> Step()) {
                    for (line_no in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                        var line_start = LineOffsets[line_no];
                        var line_end = (line_no + 1 < length(LineOffsets)) ? (LineOffsets[line_no + 1] - 1) : (*Buf |> size());
                        var ltext = *Buf |> slice(line_start, line_end);
                        TextUnformatted(ltext);
                    }
                }
                clipper |> End();
            }
        }
        PopStyleVar();
        if (AutoScroll && GetScrollY() >= GetScrollMaxY()) {
            SetScrollHereY(1.0f);
        }
        EndChild();
        End();
    }
}

def DasShowExampleAppLog(p_open : bool? implicit) {
    static_let <| $() {
        var log = new ExampleAppLog();
    }
    SetNextWindowSize(float2(500., 400.), ImGuiCond.FirstUseEver);
    Begin("Example: Log", p_open);
    if (SmallButton("[Debug] Add 5 entries")) {
        static_let <| $() {
            var counter = 0;
            let categories = fixed_array("info", "warn", "error");
            let words = fixed_array("Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent");
        }
        for (n in range(5)) {
            let category = categories[counter % length(categories)];
            let word = words[counter % length(words)];
            log->AddLog("[{GetFrameCount()}] [{category}] Hello, current time is {GetTime()}, here's a word: '{word}'\n");
            counter++;
        }
    }
    End();
    log->Draw("Example: Log", p_open);
}

def DasShowExampleAppLayout(var p_open : bool? implicit) {
    SetNextWindowSize(float2(500.0, 440.0), ImGuiCond.FirstUseEver);
    if (Begin("Example: Simple layout", p_open, ImGuiWindowFlags.MenuBar)) {
        if (BeginMenuBar()) {
            if (BeginMenu("File")) {
                if (MenuItem("Close")) {
                    *p_open = false;
                }
                EndMenu();
            }
            EndMenuBar();
        }
        static_let <| $() {
            var selected = 0;
        }
        BeginChild("left pane", float2(150., 0.), ImGuiChildFlags.Border);
        for (i in range(100)) {
            if (Selectable("MyObject {i}", selected == i)) {
                selected = i;
            }
        }
        EndChild();
        SameLine();
        BeginGroup();
        BeginChild("item view", float2(0., -GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        Text("MyObject: {selected}");
        Separator();
        if (BeginTabBar("##Tabs", ImGuiTabBarFlags.None)) {
            if (BeginTabItem("Description")) {
                TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                EndTabItem();
            }
            if (BeginTabItem("Details")) {
                Text("ID: 0123456789");
                EndTabItem();
            }
            EndTabBar();
        }
        EndChild();
        if (Button("Revert")) {
            pass;
        }
        SameLine();
        if (Button("Save")) {
            pass;
        }
        EndGroup();
    }
    End();
}

class ExampleAppConsole {
    InputBuf : ImGuiInputTextBuffer <- ImGuiInputTextBuffer("", 256);
    Items : array<string>;
    Commands : array<string>;
    History : array<string>;
    HistoryPos : int = -1;
    Filter : ImGuiTextFilter? = new ImGuiTextFilter;
    AutoScroll : bool = true;
    ScrollToBottom : bool = false;
    def ExampleAppConsole() {
        self->ClearLog();
        Commands |> push("HELP");
        Commands |> push("HISTORY");
        Commands |> push("CLEAR");
        Commands |> push("CLASSIFY");
        self->AddLog("Welcome to Dear ImGui!");
        unsafe {
            var console  = addr(self);
            InputBuf.callback <- @(var it : ImGuiInputTextBuffer; var itc : ImGuiInputTextCallbackData) : int {
                (*console)->TextEditCallback(itc);
                return 0;
            }
        }
    }

    def finalize {
        self->ClearLog();
        unsafe {
            delete Filter;
        }
    }
    def ClearLog() {
        delete Items;
    }
    def AddLog(txt : string) {
        Items |> push(txt);
    }
    def Draw(title : string; var p_open : bool? implicit) {
        SetNextWindowSize(float2(520., 600.), ImGuiCond.FirstUseEver);
        if (!Begin(title, p_open)) {
            End();
            return ;
        }
        if (BeginPopupContextItem()) {
            if (MenuItem("Close Console")) {
                *p_open = false;
            }
            EndPopup();
        }
        TextWrapped(
            "This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate " +
            "implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        TextWrapped("Enter 'HELP' for help.");
        if (SmallButton("Add Debug Text")) {
            self->AddLog("{length(Items)} some text");
            self->AddLog("some more text");
            self->AddLog("display very important message here!");
        }
        SameLine();
        if (SmallButton("Add Debug Error")) {
            self->AddLog("[error] something went wrong");
        }
        SameLine();
        if (SmallButton("Clear")) {
            self->ClearLog();
        }
        SameLine();
        let copy_to_clipboard = SmallButton("Copy");
        Separator();
        if (BeginPopup("Options")) {
            Checkbox("Auto-scroll", unsafe(addr(AutoScroll)));
            EndPopup();
        }
        if (Button("Options")) {
            OpenPopup("Options");
        }
        SameLine();
        *Filter |> Draw("Filter (\"incl,-excl\") (\"error\")", 180.);
        Separator();
        let footer_height_to_reserve = GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();
        BeginChild("ScrollingRegion", float2(0., -footer_height_to_reserve), ImGuiChildFlags.None, ImGuiWindowFlags.HorizontalScrollbar);
        if (BeginPopupContextWindow()) {
            if (Selectable("Clear")) {
                self->ClearLog();
            }
            EndPopup();
        }
        PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(4., 1.));
        if (copy_to_clipboard) {
            LogToClipboard();
        }
        for (item in Items) {
            if (!(*Filter |> PassFilter(item))) continue;
            var color = float4();
            var has_color = false;
            if (find(item, "[error]") != -1) {
                color = float4(1.0f, 0.4f, 0.4f, 1.0f);
                has_color = true;
            } elif (starts_with(item, "# ")) {
                color = float4(1.0f, 0.8f, 0.6f, 1.0f);
                has_color = true;
            }
            if (has_color) {
                PushStyleColor(ImGuiCol.Text, color);
            }
            TextUnformatted(item);
            if (has_color) {
                PopStyleColor();
            }
        }
        if (copy_to_clipboard) {
            LogFinish();
        }
        if (ScrollToBottom || (AutoScroll && GetScrollY() >= GetScrollMaxY())) {
            SetScrollHereY(1.0f);
        }
        ScrollToBottom = false;
        PopStyleVar();
        EndChild();
        Separator();
        var reclaim_focus = false;
        let input_text_flags = ImGuiInputTextFlags.EnterReturnsTrue | ImGuiInputTextFlags.CallbackCompletion | ImGuiInputTextFlags.CallbackHistory;
        if (InputText("Input", InputBuf, input_text_flags)) {
            let s = rtrim(InputBuf |> to_string);
            if (!empty(s)) {
                self->ExecCommand(s);
            }
            InputBuf |> clear();
            reclaim_focus = true;
        }
        SetItemDefaultFocus();
        if (reclaim_focus) {
            SetKeyboardFocusHere(-1);
        }
        End();
    }
    def ExecCommand(command_line : string) {
        self->AddLog("# {command_line}\n");
        HistoryPos = -1;
        let index = History |> find_index(command_line);
        if (index != -1) {
            History |> erase(index);
        }
        History |> push(command_line);
        if (command_line == "CLEAR") {
            self->ClearLog();
        } elif (command_line == "HELP") {
            self->AddLog("Commands:\n");
            for (cmd in Commands) {
                self->AddLog("- {cmd}\n");
            }
        } elif (command_line == "HISTORY") {
            let first = length(History) - 10;
            for (i in range(max(0, first), length(History))) {
                self->AddLog("{i}: {History[i]}\n");
            }
        } else {
            self->AddLog("Unknown command: '{command_line}'\n");
        }
        ScrollToBottom = true;
    }
    def TextEditCallback(var data : ImGuiInputTextCallbackData) {
        if (data.EventFlag == ImGuiInputTextFlags.CallbackCompletion) {
            let word_end = data.CursorPos;
            var word_start = word_end;
            while (word_start > 0) {
                var c : int;
                unsafe {
                    c = character_uat(data.Buf, word_start - 1);
                }
                if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
                word_start--;
            }
            let word = data.Buf |> slice(word_start, word_end);
            var match_len = word_end - word_start;
            var candidates : array<string>;
            for (cmd in Commands) {
                if (cmd |> starts_with(word)) {
                    candidates |> push(cmd);
                }
            }
            if (candidates |> empty) {
                self->AddLog("No match for \"{word}\"!\n");
            } elif (length(candidates) == 1) {
                data |> DeleteChars(word_start, match_len);
                data |> InsertChars(data.CursorPos, candidates[0]);
                data |> InsertChars(data.CursorPos, " ");
            } else {
                while (true) {
                    var c = 0;
                    var all_candidates_matches = true;
                    var ucandidates <- [for (cn in candidates); to_upper(cn)];
                    for (cnn, i in ucandidates, count()) {
                        var tc : int;
                        unsafe {
                            tc = cnn |> character_uat(match_len);
                        }
                        if (i == 0) {
                            c = tc;
                        } elif (c == 0 || c != tc) {
                            all_candidates_matches = false;
                            break;
                        }
                    }
                    if (!all_candidates_matches) break;
                    match_len++;
                }
                if (match_len > 0) {
                    data |> DeleteChars(word_start, word_end - word_start);
                    data |> InsertChars(data.CursorPos, candidates[0] |> slice(0, match_len));
                }
                self->AddLog("Possible matches:\n");
                for (cn in candidates) {
                    self->AddLog("- {cn}\n");
                }
            }
        } elif (data.EventFlag == ImGuiInputTextFlags.CallbackHistory) {
            let prev_history_pos = HistoryPos;
            if (data.EventKey == ImGuiKey.UpArrow) {
                if (HistoryPos == -1) {
                    HistoryPos = length(History) - 1;
                } elif (HistoryPos > 0) {
                    HistoryPos--;
                }
            } elif (data.EventKey == ImGuiKey.DownArrow) {
                if (HistoryPos != -1) {
                    if (++HistoryPos >= length(History)) {
                        HistoryPos = -1;
                    }
                }
            }
            if (prev_history_pos != HistoryPos) {
                let history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                data |> DeleteChars(0, data.BufTextLen);
                data |> InsertChars(0, history_str);
            }
        }
    }
}

def DasShowExampleAppConsole(var p_open : bool? implicit) {
    static_let <| $() {
        var console = new ExampleAppConsole();
    }
    console->Draw("Example: Console", p_open);
}

[safe_when_uninitialized]
struct MyDocument {
    Name : string;
    Open : bool;
    OpenPrev : bool;
    Dirty : bool = false;
    WantClose : bool = false;
    Color : float4 = float4();
}

def MyDocument(name : string; open : bool = true; color : float4 = float4(1.0f, 1.0f, 1.0f, 1.0f)) {
    var self : MyDocument;
    self.Name = name;
    self.Open = open;
    self.OpenPrev = open;
    self.Color = color;
    return self;
}

def DoOpen(var self : MyDocument) {
    self.Open = true;
}

def DoQueueClose(var self : MyDocument) {
    self.WantClose = true;
}

def DoForceClose(var self : MyDocument) {
    self.Open = false;
    self.Dirty = false;
}

def DoSave(var self : MyDocument) {
    self.Dirty = false;
}

def DisplayContents(var doc : MyDocument) {
    PushID(unsafe(addr(doc)));
    Text("Document \"{doc.Name}\"");
    PushStyleColor(ImGuiCol.Text, doc.Color);
    TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    PopStyleColor();
    if (Button("Modify", float2(100., 0.))) {
        doc.Dirty = true;
    }
    SameLine();
    if (Button("Save", float2(100., 0.))) {
        doc |> DoSave();
    }
    ColorEdit3("color", unsafe(addr(doc.Color.x)));
    PopID();
}

def DisplayContextMenu(var doc : MyDocument) {
    if (!BeginPopupContextItem()) return ;
    if (MenuItem("Save {doc.Name}", "CTRL+S", false, doc.Open)) {
        doc |> DoSave();
    }
    if (MenuItem("Close", "CTRL+W", false, doc.Open)) {
        doc |> DoQueueClose();
    }
    EndPopup();
}

struct ExampleAppDocuments {
    Documents : array<MyDocument>;
}

def ExampleAppDocuments() {
    var self : ExampleAppDocuments;
    self.Documents |> push(MyDocument("Lettuce",             true,  float4(0.4f, 0.8f, 0.4f, 1.0f)));
    self.Documents |> push(MyDocument("Eggplant",            true,  float4(0.8f, 0.5f, 1.0f, 1.0f)));
    self.Documents |> push(MyDocument("Carrot",              true,  float4(1.0f, 0.8f, 0.5f, 1.0f)));
    self.Documents |> push(MyDocument("Tomato",              false, float4(1.0f, 0.3f, 0.4f, 1.0f)));
    self.Documents |> push(MyDocument("A Rather Long Title", false));
    self.Documents |> push(MyDocument("Some Document",       false));
    return <- self;
}

def NotifyOfDocumentsClosedElsewhere(var app : ExampleAppDocuments) {
    for (doc in app.Documents) {
        if (!doc.Open && doc.OpenPrev) {
            SetTabItemClosed(doc.Name);
        }
        doc.OpenPrev = doc.Open;
    }
}

def DasShowExampleAppDocuments(p_open : bool? implicit) {
    static_let <| $() {
        var app <- ExampleAppDocuments();
    }
    static_let <| $() {
        var opt_reorderable = true;
        var opt_fitting_flags = ImGuiTabBarFlags.FittingPolicyDefault_;
    }
    let window_contents_visible = Begin("Example: Documents", p_open, ImGuiWindowFlags.MenuBar);
    if (!window_contents_visible) {
        End();
        return ;
    }
    if (BeginMenuBar()) {
        if (BeginMenu("File")) {
            var open_count = 0;
            for (doc in app.Documents) {
                open_count += doc.Open ? 1 : 0;
            }
            if (BeginMenu("Open", open_count < length(app.Documents))) {
                for (doc in app.Documents) {
                    if (!doc.Open) {
                        if (MenuItem(doc.Name)) {
                            doc |> DoOpen();
                        }
                    }
                }
                EndMenu();
            }
            if (MenuItem("Close All Documents", "", false, open_count > 0)) {
                for (doc in app.Documents) {
                    doc |> DoQueueClose();
                }
            }
            if (MenuItem("Exit", "Alt+F4")) {
                pass;
            }
            EndMenu();
        }
        EndMenuBar();
    }
    for (doc, doc_n in app.Documents, count()) {
        if (doc_n > 0) {
            SameLine();
        }
        PushID(safe_addr(doc));
        if (Checkbox(doc.Name, safe_addr(doc.Open))) {
            if (!doc.Open) {
                doc |> DoForceClose();
            }
        }
        PopID();
    }
    Separator();
    let tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags.Reorderable : ImGuiTabBarFlags.None);
    if (BeginTabBar("##tabs", tab_bar_flags)) {
        if (opt_reorderable) {
            NotifyOfDocumentsClosedElsewhere(app);
        }
        for (doc in app.Documents) {
            if (!doc.Open) continue;
            let tab_flags = (doc.Dirty ? ImGuiTabItemFlags.UnsavedDocument : ImGuiTabItemFlags.None);
            let visible = BeginTabItem(doc.Name, safe_addr(doc.Open), tab_flags);
            if (!doc.Open && doc.Dirty) {
                doc.Open = true;
                doc |> DoQueueClose();
            }
            DisplayContextMenu(doc);
            if (visible) {
                DisplayContents(doc);
                EndTabItem();
            }
        }
        EndTabBar();
    }
    static_let <| $() {
        var close_queue : array<MyDocument?>;
    }
    if (close_queue |> empty()) {
        for (doc in app.Documents) {
            if (doc.WantClose) {
                doc.WantClose = false;
                unsafe {
                    close_queue |> push(addr(doc));
                }
            }
        }
    }
    if (!(close_queue |> empty())) {
        var close_queue_unsaved_documents = 0;
        for (qq in close_queue) {
            if (qq.Dirty) {
                close_queue_unsaved_documents++;
            }
        }
        if (close_queue_unsaved_documents == 0) {
            for (qq in close_queue) {
                (*qq) |> DoForceClose();
            }
            close_queue |> clear();
        } else {
            if (!IsPopupOpen("Save?")) {
                OpenPopup("Save?");
            }
            if (BeginPopupModal("Save?", null, ImGuiWindowFlags.AlwaysAutoResize)) {
                Text("Save change to the following items?");
                let item_height = GetTextLineHeightWithSpacing();
                if (BeginChild(GetID("frame"), float2(-FLT_MIN, 6.25f * item_height), ImGuiChildFlags.FrameStyle)) {
                    for (qq in close_queue) {
                        if (qq.Dirty) {
                            Text("{qq.Name}");
                        }
                    }
                }
                EndChild();
                var button_size = float2(GetFontSize() * 7.0f, 0.0f);
                if (Button("Yes", button_size)) {
                    for (qq in close_queue) {
                        if (qq.Dirty) {
                            (*qq) |> DoSave();
                        }
                        (*qq) |> DoForceClose();
                    }
                    close_queue |> clear();
                    CloseCurrentPopup();
                }
                SameLine();
                if (Button("No", button_size)) {
                    for (qq in close_queue) {
                        (*qq) |> DoForceClose();
                    }
                    close_queue |> clear();
                    CloseCurrentPopup();
                }
                SameLine();
                if (Button("Cancel", button_size)) {
                    close_queue |> clear();
                    CloseCurrentPopup();
                }
                EndPopup();
            }
        }
    }
    End();
}

def ShowPlaceholderObject(prefix : string; uid : int) {
    PushID(uid);
    TableNextRow();
    TableSetColumnIndex(0);
    AlignTextToFramePadding();
    let node_open = TreeNode("Object", "{prefix}_{uid}");
    TableSetColumnIndex(1);
    Text("my sailor is rich");
    if (node_open) {
        static_let <| $() {
            var placeholder_members = fixed_array<float>(0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f, 0.0f, 0.0f);
        }
        for (i in range(8)) {
            PushID(i);
            if (i < 2) {
                ShowPlaceholderObject("Child", 424242);
            } else {
                TableNextRow();
                TableSetColumnIndex(0);
                AlignTextToFramePadding();
                let flags = ImGuiTreeNodeFlags.Leaf | ImGuiTreeNodeFlags.NoTreePushOnOpen | ImGuiTreeNodeFlags.Bullet;
                TreeNodeEx("Field", flags, "Field_{i}");
                TableSetColumnIndex(1);
                SetNextItemWidth(-FLT_MIN);
                if (i >= 5) {
                    InputFloat("##value", safe_addr(placeholder_members[i]), 1.0f);
                } else {
                    DragFloat("##value", safe_addr(placeholder_members[i]), 0.01f);
                }
                NextColumn();
            }
            PopID();
        }
        TreePop();
    }
    PopID();
}

def DasShowExampleAppPropertyEditor(p_open : bool? implicit) {
    SetNextWindowSize(float2(430., 450.), ImGuiCond.FirstUseEver);
    if (!Begin("Example: Property editor", p_open)) {
        End();
        return ;
    }
    DasHelpMarker(
        "This example shows how you may implement a property editor using two columns.\n" +
        "All objects/fields data are dummies here.\n" +
        "Remember that in many simple cases, you can use SameLine(xxx) to position\n" +
        "your cursor horizontally instead of using the Columns() API.");
    PushStyleVar(ImGuiStyleVar.FramePadding, float2(2., 2.));
    if (BeginTable("split", 2, ImGuiTableFlags.BordersOuter | ImGuiTableFlags.Resizable)) {
        for (obj_i in range(4)) {
            ShowPlaceholderObject("Object", obj_i);
        }
        EndTable();
    }
    PopStyleVar();
    End();
}

def DasShowExampleAppLongText(p_open : bool? implicit) {
    SetNextWindowSize(float2(520., 600.), ImGuiCond.FirstUseEver);
    if (!Begin("Example: Long text display", p_open)) {
        End();
        return ;
    }
    static_let <| $() {
        var test_type = 0;
        var log = new ImGuiTextBuffer;
        var lines = 0;
    }
    Text("Printing unusually long amount of text.");
    Combo("Test type", safe_addr(test_type),
        fixed_array("Single call to TextUnformatted()",
        "Multiple calls to Text(), clipped",
        "Multiple calls to Text(), not clipped (slow)"));
    Text("Buffer contents: {lines} lines, {*log |> size} bytes");
    if (Button("Clear")) {
        *log |> clear();
        lines = 0;
    }
    SameLine();
    if (Button("Add 1000 lines")) {
        for (i in range(1000)) {
            *log |> append("{lines+i} The quick brown fox jumps over the lazy dog\n");
        }
        lines += 1000;
    }
    BeginChild("Log", float2());
    if (test_type == 0) {
        TextUnformatted(*log |> c_str());
    } elif (test_type == 1) {
        PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(0., 0.));
        using() <| $(var clipper : ImGuiListClipper) {
            clipper |> Begin(lines);
            while (clipper |> Step()) {
                for (i in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                    Text("{i} The quick brown fox jumps over the lazy dog");
                }
            }
            clipper |> End();
            PopStyleVar();
        }
    } elif (test_type == 2) {
        PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(0., 0.));
        for (i in range(lines)) {
            Text("{i} The quick brown fox jumps over the lazy dog");
        }
        PopStyleVar();
    }
    EndChild();
    End();
}

def DasShowExampleAppAutoResize(p_open : bool? implicit) {
    if (!Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags.AlwaysAutoResize)) {
        End();
        return ;
    }
    static_let <| $() {
        var lines = 10;
    }
    TextUnformatted(
        "Window will resize every-frame to the size of its content.\n" +
        "Note that you probably don't want to query the window size to\n" +
        "output your content because that would create a feedback loop.");
    SliderInt("Number of lines", safe_addr(lines), 1, 20);
    for (i in range(lines)) {
        Text("This is line {i}");
    }
    End();
}

def DasShowExampleAppConstrainedResize(p_open : bool? implicit) {
    static_let <| $() {
        let test_desc = fixed_array(
            "Resize vertical only",
            "Resize horizontal only",
            "Width > 100, Height > 100",
            "Width 400-500",
            "Height 400-500",
            "Custom: Always Square",
            "Custom: Fixed Steps (100)"
        );
        var auto_resize = false;
        var con_type = 0;
        var display_lines = 10;
        var ccs_square <- ImGuiSizeConstraints() <| @(var data : ImGuiSizeCallbackData) {
            let t = max(data.DesiredSize.x, data.DesiredSize.y);
            data.DesiredSize.x = t;
            data.DesiredSize.y = t;
        }
        var ccs_step <- ImGuiSizeConstraints() <| @(var data : ImGuiSizeCallbackData) {
            let step = 100.f;
            data.DesiredSize = float2(
                floor(data.DesiredSize.x / step + 0.5f) * step,
                floor(data.DesiredSize.y / step + 0.5f) * step
            );
        }
    }
    if (con_type == 0) {
        SetNextWindowSizeConstraints(float2(-1., 0.),    float2(-1., FLT_MAX));
    } elif (con_type == 1) {
        SetNextWindowSizeConstraints(float2(0., -1.),    float2(FLT_MAX, -1.));
    } elif (con_type == 2) {
        SetNextWindowSizeConstraints(float2(100., 100.), float2(FLT_MAX, FLT_MAX));
    } elif (con_type == 3) {
        SetNextWindowSizeConstraints(float2(400., -1.),  float2(500., -1.));
    } elif (con_type == 4) {
        SetNextWindowSizeConstraints(float2(-1., 400.),  float2(-1., 500.));
    } elif (con_type == 5) {
        SetNextWindowSizeConstraints(float2(0., 0.),     float2(FLT_MAX, FLT_MAX), ccs_square);
    } elif (con_type == 6) {
        SetNextWindowSizeConstraints(float2(0., 0.),     float2(FLT_MAX, FLT_MAX), ccs_step);
    }
    let flags = auto_resize ? ImGuiWindowFlags.AlwaysAutoResize : ImGuiWindowFlags.None;
    if (Begin("Example: Constrained Resize", p_open, flags)) {
        if (Button("200x200")) {
            SetWindowSize(float2(200., 200.));
        }
        SameLine();
        if (Button("500x500")) {
            SetWindowSize(float2(500., 500.));
        }
        SameLine();
        if (Button("800x200")) {
            SetWindowSize(float2(800., 200.));
        }
        SetNextItemWidth(200.);
        Combo("Constraint", safe_addr(con_type), test_desc);
        SetNextItemWidth(200.);
        DragInt("Lines", safe_addr(display_lines), 0.2f, 1, 100);
        Checkbox("Auto-resize", safe_addr(auto_resize));
        for (i in range(display_lines)) {
            Text("{i} Hello, sailor! Making this line long enough for the example.");
        }
    }
    End();
}

def DasShowExampleAppSimpleOverlay(var p_open : bool? implicit) {
    let PAD = 10.0f;
    static_let <| $() {
        var corner = 0;
    }
    var io & = unsafe(GetIO());
    var window_flags = ImGuiWindowFlags.NoDecoration | ImGuiWindowFlags.AlwaysAutoResize | ImGuiWindowFlags.NoSavedSettings | ImGuiWindowFlags.NoFocusOnAppearing | ImGuiWindowFlags.NoNav;
    if (corner != -1) {
        let viewport = GetMainViewport();
        var work_pos = viewport.WorkPos;
        var work_size = viewport.WorkSize;
        var window_pos = float2();
        var window_pos_pivot = float2();
        window_pos.x = (corner & 1) != 0 ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (corner & 2) != 0 ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (corner & 1) != 0 ? 1.0f : 0.0f;
        window_pos_pivot.y = (corner & 2) != 0 ? 1.0f : 0.0f;
        SetNextWindowPos(window_pos, ImGuiCond.Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags.NoMove;
    }
    SetNextWindowBgAlpha(0.35f);
    if (Begin("Example: Simple overlay", p_open, window_flags)) {
        Text("Simple overlay\nin the corner of the screen.\n(right-click to change position)");
        Separator();
        if (IsMousePosValid()) {
            Text("Mouse Position: ({io.MousePos.x},{io.MousePos.y})");
        } else {
            Text("Mouse Position: <invalid>");
        }
        if (BeginPopupContextWindow()) {
            if (MenuItem("Custom",       "", corner == -1)) {
                corner = -1;
            }
            if (MenuItem("Top-left",     "", corner == 0)) {
                corner = 0;
            }
            if (MenuItem("Top-right",    "", corner == 1)) {
                corner = 1;
            }
            if (MenuItem("Bottom-left",  "", corner == 2)) {
                corner = 2;
            }
            if (MenuItem("Bottom-right", "", corner == 3)) {
                corner = 3;
            }
            if (p_open != null && MenuItem("Close")) {
                *p_open = false;
            }
            EndPopup();
        }
    }
    End();
}

def DasShowExampleAppWindowTitles(p_open : bool? implicit) {
    let viewport = GetMainViewport();
    let base_pos = viewport.Pos;
    SetNextWindowPos(float2(base_pos.x + 100., base_pos.y + 100.), ImGuiCond.FirstUseEver);
    Begin("Same title as another window##1");
    Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    End();
    SetNextWindowPos(float2(base_pos.x + 100., base_pos.y + 200.), ImGuiCond.FirstUseEver);
    Begin("Same title as another window##2");
    Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
    End();
    SetNextWindowPos(float2(base_pos.x + 100., base_pos.y + 300.), ImGuiCond.FirstUseEver);
    static_let <| $() {
        let crot = fixed_array<int>('|', '/', '-', '\\');
    }
    let ch = crot[ int(GetTime() / 0.25lf) & 3 ];
    Begin("Animated title {to_char(ch)} {GetFrameCount()}###AnimatedTitle");
    Text("This window has a changing title.");
    End();
}

def DasShowExampleAppCustomRendering(p_open : bool? implicit) {
    if (!Begin("Example: Custom rendering", p_open)) {
        End();
        return ;
    }

    if (BeginTabBar("##TabBar")) {
        if (BeginTabItem("Primitives")) {
            PushItemWidth(-GetFontSize() * 15.);
            var draw_list = GetWindowDrawList();
            Text("Gradients");
            var gradient_size = float2(CalcItemWidth(), GetFrameHeight());
            {
                let p0 = GetCursorScreenPos();
                let p1 = float2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                let col_a = GetColorU32(IM_COL32(0, 0, 0, 255));
                let col_b = GetColorU32(IM_COL32(255, 255, 255, 255));
                *draw_list |> AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                InvisibleButton("##gradient1", gradient_size);
            }
            {
                let p0 = GetCursorScreenPos();
                let p1 = float2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                let col_a = GetColorU32(IM_COL32(0, 255, 0, 255));
                let col_b = GetColorU32(IM_COL32(255, 0, 0, 255));
                *draw_list |> AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                InvisibleButton("##gradient2", gradient_size);
            }

            Text("All primitives");
            static_let <| $() {
                var sz = 36.0f;
                var thickness = 3.0f;
                var ngon_sides = 6;
                var circle_segments_override = false;
                var circle_segments_override_v = 12;
                var curve_segments_override = false;
                var curve_segments_override_v = 8;
                var colf = float4(1.0f, 1.0f, 0.4f, 1.0f);
            }
            DragFloat("Size", safe_addr(sz), 0.2f, 2.0f, 100.0f, "%.0f");
            DragFloat("Thickness", safe_addr(thickness), 0.05f, 1.0f, 8.0f, "%.02f");
            SliderInt("N-gon sides", safe_addr(ngon_sides), 3, 12);
            Checkbox("##circlesegmentoverride", safe_addr(circle_segments_override));
            SameLine(0.0f, GetStyle().ItemInnerSpacing.x);
            circle_segments_override = SliderInt("Circle segments override", safe_addr(circle_segments_override_v), 3, 40) || circle_segments_override;
            Checkbox("##curvessegmentoverride", safe_addr(curve_segments_override));
            SameLine(0.0f, GetStyle().ItemInnerSpacing.x);
            curve_segments_override = SliderInt("Curves segments override", safe_addr(curve_segments_override_v), 3, 40) || curve_segments_override;
            ColorEdit4("Color", safe_addr(colf.x));

            let p = GetCursorScreenPos();
            let col = ColorConvertFloat4ToU32(colf);
            let spacing = 10.0f;
            let corners_none = ImDrawFlags.None;
            let corners_all = ImDrawFlags.RoundCornersAll;
            let corners_tl_br = ImDrawFlags.RoundCornersTopLeft | ImDrawFlags.RoundCornersBottomRight;
            let rounding = sz / 5.0f;
            let circle_segments = circle_segments_override ? circle_segments_override_v : 0;
            let curve_segments = curve_segments_override ? curve_segments_override_v : 0;
            var x = p.x + 4.0f;
            var y = p.y + 4.0f;
            for (n in range(2)) {
                let th = (n == 0) ? 1.0f : thickness;
                *draw_list |> AddNgon(float2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides, th);
                x += sz + spacing;
                *draw_list |> AddCircle(float2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments, th);
                x += sz + spacing;
                *draw_list |> AddRect(float2(x, y), float2(x + sz, y + sz), col, 0.0f,  corners_none, th);
                x += sz + spacing;
                *draw_list |> AddRect(float2(x, y), float2(x + sz, y + sz), col, rounding, corners_all, th);
                x += sz + spacing;
                *draw_list |> AddRect(float2(x, y), float2(x + sz, y + sz), col, rounding, corners_tl_br, th);
                x += sz + spacing;
                *draw_list |> AddTriangle(float2(x + sz * 0.5f, y), float2(x + sz, y + sz - 0.5f), float2(x, y + sz - 0.5f), col, th);
                x += sz + spacing;
                *draw_list |> AddLine(float2(x, y), float2(x + sz, y), col, th);
                x += sz + spacing;
                *draw_list |> AddLine(float2(x, y), float2(x, y + sz), col, th);
                x += spacing;
                *draw_list |> AddLine(float2(x, y), float2(x + sz, y + sz), col, th);
                x += sz + spacing;
                var cp3 = fixed_array<float2>(float2(x, y + sz * 0.6f), float2(x + sz * 0.5f, y - sz * 0.4f), float2(x + sz, y + sz));
                *draw_list |> AddBezierQuadratic(cp3[0], cp3[1], cp3[2], col, th, curve_segments);
                x += sz + spacing;
                var cp4 = fixed_array<float2>(float2(x, y), float2(x + sz * 1.3f, y + sz * 0.3f), float2(x + sz - sz * 1.3f, y + sz - sz * 0.3f), float2(x + sz, y + sz));
                *draw_list |> AddBezierCubic(cp4[0], cp4[1], cp4[2], cp4[3], col, th, curve_segments);
                x = p.x + 4.;
                y += sz + spacing;
            }

            *draw_list |> AddNgonFilled(float2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, ngon_sides);
            x += sz + spacing;
            *draw_list |> AddCircleFilled(float2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, col, circle_segments);
            x += sz + spacing;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + sz, y + sz), col);
            x += sz + spacing;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + sz, y + sz), col, 10.0f);
            x += sz + spacing;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + sz, y + sz), col, 10.0f, corners_tl_br);
            x += sz + spacing;
            *draw_list |> AddTriangleFilled(float2(x + sz * 0.5f, y), float2(x + sz, y + sz - 0.5f), float2(x, y + sz - 0.5f), col);
            x += sz + spacing;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + sz, y + thickness), col);
            x += sz + spacing;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + thickness, y + sz), col);
            x += spacing * 2.0f;
            *draw_list |> AddRectFilled(float2(x, y), float2(x + 1., y + 1.), col);
            x += sz;
            *draw_list |> AddRectFilledMultiColor(float2(x, y), float2(x + sz, y + sz), IM_COL32(0, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255));

            Dummy(float2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
            PopItemWidth();
            EndTabItem();
        }

        if (BeginTabItem("Canvas")) {
            static_let <| $() {
                var points : array<float2>;
                var scrolling = float2(0.0f, 0.0f);
                var opt_enable_grid = true;
                var opt_enable_context_menu = true;
                var adding_line = false;
            }
            Checkbox("Enable grid", safe_addr(opt_enable_grid));
            Checkbox("Enable context menu", safe_addr(opt_enable_context_menu));
            Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

            var canvas_p0 = GetCursorScreenPos();
            var canvas_sz = GetContentRegionAvail();
            if (canvas_sz.x < 50.0f) {
                canvas_sz.x = 50.0f;
            }
            if (canvas_sz.y < 50.0f) {
                canvas_sz.y = 50.0f;
            }
            var canvas_p1 = float2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            var io & = unsafe(GetIO());
            var draw_list = GetWindowDrawList();
            *draw_list |> AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            *draw_list |> AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

            InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags.MouseButtonLeft | ImGuiButtonFlags.MouseButtonRight);
            let is_hovered = IsItemHovered();
            let is_active = IsItemActive();
            let origin = float2(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);
            let mouse_pos_in_canvas = float2(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            if (is_hovered && !adding_line && IsMouseClicked(ImGuiMouseButton.Left)) {
                points |> push(mouse_pos_in_canvas);
                points |> push(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (adding_line) {
                points [ points |> length() - 1 ] = mouse_pos_in_canvas;
                if (!IsMouseDown(ImGuiMouseButton.Left)) {
                    adding_line = false;
                }
            }

            let mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && IsMouseDragging(ImGuiMouseButton.Right, mouse_threshold_for_pan)) {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            let drag_delta = GetMouseDragDelta(ImGuiMouseButton.Right);
            if (opt_enable_context_menu && IsMouseReleased(ImGuiMouseButton.Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f) {
                OpenPopupOnItemClick("context");
            }
            if (BeginPopup("context")) {
                if (adding_line) {
                    points |> resize(points |> length() - 2);
                }
                adding_line = false;
                if (MenuItem("Remove one", "", false, points |> !empty(points))) {
                    points |> resize(points |> length() - 2);
                }
                if (MenuItem("Remove all", "", false, points |> !empty(points))) {
                    points |> clear();
                }
                EndPopup();
            }

            *draw_list |> PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid) {
                let GRID_STEP = 64.0f;
                var x = scrolling.x % GRID_STEP;
                while (x < canvas_sz.x) {
                    *draw_list |> AddLine(float2(canvas_p0.x + x, canvas_p0.y), float2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                    x += GRID_STEP;
                }
                var y = scrolling.y % GRID_STEP;
                while (y < canvas_sz.y) {
                    *draw_list |> AddLine(float2(canvas_p0.x, canvas_p0.y + y), float2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
                    y += GRID_STEP;
                }
            }
            var n = 0;
            while (n < points |> length()) {
                *draw_list |> AddLine(float2(origin.x + points[n].x, origin.y + points[n].y), float2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
                n += 2;
            }
            *draw_list |> PopClipRect();
            EndTabItem();
        }

        if (BeginTabItem("BG/FG draw lists")) {
            static_let <| $() {
                var draw_bg = true;
                var draw_fg = true;
            }
            Checkbox("Draw in Background draw list", safe_addr(draw_bg));
            SameLine();
            DasHelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
            Checkbox("Draw in Foreground draw list", safe_addr(draw_fg));
            SameLine();
            DasHelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
            let window_pos = GetWindowPos();
            let window_size = GetWindowSize();
            let window_center = float2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg) {
                *GetBackgroundDrawList() |> AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10. + 4.);
            }
            if (draw_fg) {
                *GetForegroundDrawList() |> AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10.);
            }
            EndTabItem();
        }
        EndTabBar();
    }
    End();
}

enum MyItemColumnID {
    ID
    Name
    Action
    Quantity
    Description
}

struct MyItem {
    ID : int;
    Name : string;
    Quantity : int;
}

def SortMyItems(var items : array<MyItem>; var sorts_specs : ImGuiTableSortSpecs?) {
    sort(items) <| $(a, b) {
        for (n in range(sorts_specs.SpecsCount)) {
            unsafe {
                let sort_spec & = sorts_specs.Specs[n];
                var delta = 0;
                if (sort_spec.ColumnUserID == uint(MyItemColumnID.ID)) {
                    delta = a.ID - b.ID;
                } elif (sort_spec.ColumnUserID == uint(MyItemColumnID.Name) || sort_spec.ColumnUserID == uint(MyItemColumnID.Description)) {
                    if (a.Name < b.Name) {
                        delta = +1;
                    } elif (a.Name > b.Name) {
                        delta = -1;
                    }
                } elif (sort_spec.ColumnUserID == uint(MyItemColumnID.Quantity)) {
                    delta = a.Quantity - b.Quantity;
                }
                if (delta > 0) {
                    return sort_spec |> SortDirection == ImGuiSortDirection.Ascending;
                } elif (delta < 0) return !(sort_spec |> SortDirection == ImGuiSortDirection.Ascending);
            }
        }
        return a.ID < b.ID;
    }
    sorts_specs.SpecsDirty = false;
}

def PushStyleCompact {
    var style & = unsafe(GetStyle());
    PushStyleVar(ImGuiStyleVar.FramePadding, float2(style.FramePadding.x, floor(style.FramePadding.y * 0.60f)));
    PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(style.ItemSpacing.x, floor(style.ItemSpacing.y * 0.60f)));
}

def PopStyleCompact {
    PopStyleVar(2);
}

struct EnumDesc {
    Value : ImGuiTableFlags;
    Name : string;
    Tooltip : string;
}

def EditTableSizingFlags(var p_flags : ImGuiTableFlags? implicit) {
    static_let <| $() {
        var policies = [EnumDesc(
            Value=ImGuiTableFlags.None,                 Name="Default",
                Tooltip="Use default sizing policy:\n- ImGuiTableFlags SizingFixedFit if ScrollX is on or if host window has ImGuiWindowFlags AlwaysAutoResize.\n" +
                    "- ImGuiTableFlags SizingStretchSame otherwise."), EnumDesc(
            Value=ImGuiTableFlags.SizingFixedFit,       Name="ImGuiTableFlags SizingFixedFit",
                Tooltip="Columns default to _WidthFixed (if resizable) or _WidthAuto (if not resizable), matching contents width."), EnumDesc(
            Value=ImGuiTableFlags.SizingFixedSame,      Name="ImGuiTableFlags SizingFixedSame",
                Tooltip="Columns are all the same width, matching the maximum contents width.\nImplicitly disable ImGuiTableFlags Resizable and enable ImGuiTableFlags NoKeepColumnsVisible."), EnumDesc(
            Value=ImGuiTableFlags.SizingStretchProp,    Name="ImGuiTableFlags SizingStretchProp",
                Tooltip="Columns default to _WidthStretch with weights proportional to their widths."), EnumDesc(
            Value=ImGuiTableFlags.SizingStretchSame,    Name="ImGuiTableFlags SizingStretchSame",
                Tooltip="Columns default to _WidthStretch with same weights."
        )];
    }
    var idx = 0;
    for (pol in policies) {
        if (pol.Value == (*p_flags & ImGuiTableFlags.SizingMask_)) break;
        idx ++;
    }
    var preview_text = "";
    if (idx < length(policies)) {
        if (idx == 0) {
            preview_text = policies[idx].Name;
        } else {
            preview_text = policies[idx].Name |> slice(length("ImGuiTableFlags"));
        }
    }
    if (BeginCombo("Sizing Policy", preview_text)) {
        for (pol, n in policies, count()) {
            if (Selectable(pol.Name, idx == n)) {
                *p_flags = (*p_flags & ~ImGuiTableFlags.SizingMask_) | pol.Value; // TODO: add & and ~ to flags
            }
        }
        EndCombo();
    }
    SameLine();
    TextDisabled("(?)");
    if (IsItemHovered()) {
        BeginTooltip();
        PushTextWrapPos(GetFontSize() * 50.0f);
        for (pol in policies) {
            Separator();
            Text("{pol.Name}:");
            Separator();
            SetCursorPosX(GetCursorPosX() + GetStyle().IndentSpacing * 0.5f);
            TextUnformatted(pol.Tooltip);
        }
        PopTextWrapPos();
        EndTooltip();
    }
}

def EditTableColumnsFlags(var p_flags : ImGuiTableColumnFlags? implicit) {
    CheckboxFlags("_DefaultHide", p_flags, ImGuiTableColumnFlags.DefaultHide);
    CheckboxFlags("_DefaultSort", p_flags, ImGuiTableColumnFlags.DefaultSort);
    if (CheckboxFlags("_WidthStretch", p_flags, ImGuiTableColumnFlags.WidthStretch)) {
        *p_flags &= ~(ImGuiTableColumnFlags.WidthMask_ ^ ImGuiTableColumnFlags.WidthStretch);
    }
    if (CheckboxFlags("_WidthFixed", p_flags, ImGuiTableColumnFlags.WidthFixed)) {
        *p_flags &= ~(ImGuiTableColumnFlags.WidthMask_ ^ ImGuiTableColumnFlags.WidthFixed);
    }
    CheckboxFlags("_NoResize", p_flags, ImGuiTableColumnFlags.NoResize);
    CheckboxFlags("_NoReorder", p_flags, ImGuiTableColumnFlags.NoReorder);
    CheckboxFlags("_NoHide", p_flags, ImGuiTableColumnFlags.NoHide);
    CheckboxFlags("_NoClip", p_flags, ImGuiTableColumnFlags.NoClip);
    CheckboxFlags("_NoSort", p_flags, ImGuiTableColumnFlags.NoSort);
    CheckboxFlags("_NoSortAscending", p_flags, ImGuiTableColumnFlags.NoSortAscending);
    CheckboxFlags("_NoSortDescending", p_flags, ImGuiTableColumnFlags.NoSortDescending);
    CheckboxFlags("_NoHeaderWidth", p_flags, ImGuiTableColumnFlags.NoHeaderWidth);
    CheckboxFlags("_PreferSortAscending", p_flags, ImGuiTableColumnFlags.PreferSortAscending);
    CheckboxFlags("_PreferSortDescending", p_flags, ImGuiTableColumnFlags.PreferSortDescending);
    CheckboxFlags("_IndentEnable", p_flags, ImGuiTableColumnFlags.IndentEnable);
    SameLine();
    DasHelpMarker("Default for column 0");
    CheckboxFlags("_IndentDisable", p_flags, ImGuiTableColumnFlags.IndentDisable);
    SameLine();
    DasHelpMarker("Default for column >0");
}

def ShowTableColumnsStatusFlags(var flags : ImGuiTableColumnFlags) {
    CheckboxFlags("_IsEnabled", unsafe(addr(flags)), ImGuiTableColumnFlags.IsEnabled);
    CheckboxFlags("_IsVisible", unsafe(addr(flags)), ImGuiTableColumnFlags.IsVisible);
    CheckboxFlags("_IsSorted",  unsafe(addr(flags)), ImGuiTableColumnFlags.IsSorted);
    CheckboxFlags("_IsHovered", unsafe(addr(flags)), ImGuiTableColumnFlags.IsHovered);
}

enum ContentsType {
    CT_ShowWidth
    CT_ShortText
    CT_LongText
    CT_Button
    CT_FillButton
    CT_InputText
}

enum ContentsTypeEx {
    CT_ShortText
    CT_Button
    CT_SmallButton
    CT_FillButton
    CT_Selectable
    CT_SelectableSpanRow
}

enum Mode {
    Copy
    Move
    Swap
}

def DasShowDemoWindowTables() {
    if (!CollapsingHeader("Tables & Columns")) return ;
    let TEXT_BASE_WIDTH = CalcTextSize("A").x;
    let TEXT_BASE_HEIGHT = GetTextLineHeightWithSpacing();
    PushID("Tables");

    var open_action = -1;
    if (Button("Open all")) {
        open_action = 1;
    }
    SameLine();
    if (Button("Close all")) {
        open_action = 0;
    }
    SameLine();

    static_let <| $() {
        var disable_indent = false;
    }
    Checkbox("Disable tree indentation", safe_addr(disable_indent));
    SameLine();
    DasHelpMarker("Disable the indenting of tree nodes so demo tables can use the full window width.");
    Separator();
    if (disable_indent) {
        PushStyleVar(ImGuiStyleVar.IndentSpacing, 0.0f);
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Basic")) {
        DasHelpMarker("Using TableNextRow() + calling TableSetColumnIndex() _before_ each cell, in a loop.");
        if (BeginTable("table1", 3)) {
            for (row in range(4)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("Row {row} Column {column}");
                }
            }
            EndTable();
        }
        DasHelpMarker("Using TableNextRow() + calling TableNextColumn() _before_ each cell, manually.");
        if (BeginTable("table2", 3)) {
            for (row in range(4)) {
                TableNextRow();
                TableNextColumn();
                Text("Row {row}");
                TableNextColumn();
                Text("Some contents");
                TableNextColumn();
                Text("123.456");
            }
            EndTable();
        }
        DasHelpMarker(
            "Only using TableNextColumn(), which tends to be convenient for tables where every cells contains the same type of contents.\n" +
            "This is also more similar to the old NextColumn() function of the Columns API, and provided to facilitate the Columns->Tables API transition.");
        if (BeginTable("table3", 3)) {
            for (item in range(14)) {
                TableNextColumn();
                Text("Item {item}");
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Borders, background")) {
        static_let <| $() {
            var flags = ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg;
            var display_headers = false;
            var contents_type = ContentsType.CT_ShortText;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags RowBg", safe_addr(flags), ImGuiTableFlags.RowBg);
        CheckboxFlags("ImGuiTableFlags Borders", safe_addr(flags), ImGuiTableFlags.Borders);
        SameLine();
        DasHelpMarker("ImGuiTableFlags Borders\n = ImGuiTableFlags BordersInnerV\n | ImGuiTableFlags BordersOuterV\n | ImGuiTableFlags BordersInnerV\n | ImGuiTableFlags BordersOuterH");
        Indent();
        CheckboxFlags("ImGuiTableFlags BordersH", safe_addr(flags), ImGuiTableFlags.BordersH);
        Indent();
        CheckboxFlags("ImGuiTableFlags BordersOuterH", safe_addr(flags), ImGuiTableFlags.BordersOuterH);
        CheckboxFlags("ImGuiTableFlags BordersInnerH", safe_addr(flags), ImGuiTableFlags.BordersInnerH);
        Unindent();
        CheckboxFlags("ImGuiTableFlags BordersV", safe_addr(flags), ImGuiTableFlags.BordersV);
        Indent();
        CheckboxFlags("ImGuiTableFlags BordersOuterV", safe_addr(flags), ImGuiTableFlags.BordersOuterV);
        CheckboxFlags("ImGuiTableFlags BordersInnerV", safe_addr(flags), ImGuiTableFlags.BordersInnerV);
        Unindent();
        CheckboxFlags("ImGuiTableFlags BordersOuter", safe_addr(flags), ImGuiTableFlags.BordersOuter);
        CheckboxFlags("ImGuiTableFlags BordersInner", safe_addr(flags), ImGuiTableFlags.BordersInner);
        Unindent();
        AlignTextToFramePadding();
        Text("Cell contents:");
        SameLine();
        RadioButton("Text", safe_addr(contents_type), ContentsType.CT_ShortText);
        SameLine();
        RadioButton("FillButton", safe_addr(contents_type), ContentsType.CT_FillButton);
        Checkbox("Display headers", safe_addr(display_headers));
        CheckboxFlags("ImGuiTableFlags NoBordersInBody", safe_addr(flags), ImGuiTableFlags.NoBordersInBody);
        SameLine();
        DasHelpMarker("Disable vertical borders in columns Body (borders will always appears in Headers");
        PopStyleCompact();
        if (BeginTable("table1", 3, flags)) {
            if (display_headers) {
                TableSetupColumn("One");
                TableSetupColumn("Two");
                TableSetupColumn("Three");
                TableHeadersRow();
            }
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    let buf = "Hello {column},{row}";
                    if (contents_type == ContentsType.CT_ShortText) {
                        TextUnformatted(buf);
                    } elif (contents_type == ContentsType.CT_FillButton) {
                        Button(buf, float2(-FLT_MIN, 0.0f));
                    }
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Resizable, stretch")) {
        static_let <| $() {
            var flags = ImGuiTableFlags.SizingStretchSame | ImGuiTableFlags.Resizable | ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV | ImGuiTableFlags.ContextMenuInBody;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags BordersV", safe_addr(flags), ImGuiTableFlags.BordersV);
        SameLine();
        DasHelpMarker("Using the _Resizable flag automatically enables the _BordersInnerV flag as well, this is why the resize borders are still showing when unchecking this.");
        PopStyleCompact();
        if (BeginTable("table1", 3, flags)) {
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("Hello {column},{row}");
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Resizable, fixed")) {
        DasHelpMarker(
            "Using _Resizable + _SizingFixedFit flags.\n" +
            "Fixed-width columns generally makes more sense if you want to use horizontal scrolling.\n\n" +
            "Double-click a column border to auto-fit the column to its contents.");
        PushStyleCompact();
        static_let <| $() {
            var flags = ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.Resizable | ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV | ImGuiTableFlags.ContextMenuInBody;
        }
        CheckboxFlags("ImGuiTableFlags NoHostExtendX", safe_addr(flags), ImGuiTableFlags.NoHostExtendX);
        PopStyleCompact();
        if (BeginTable("table1", 3, flags)) {
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("Hello {column},{row}");
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Resizable, mixed")) {
        DasHelpMarker(
            "Using TableSetupColumn() to alter resizing policy on a per-column basis.\n\n" +
            "When combining Fixed and Stretch columns, generally you only want one, maybe two trailing columns to use _WidthStretch.");
        static_let <| $() {
            var flags = ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.RowBg | ImGuiTableFlags.Borders | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable;
        }
        if (BeginTable("table1", 3, flags)) {
            TableSetupColumn("AAA", ImGuiTableColumnFlags.WidthFixed);
            TableSetupColumn("BBB", ImGuiTableColumnFlags.WidthFixed);
            TableSetupColumn("CCC", ImGuiTableColumnFlags.WidthStretch);
            TableHeadersRow();
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("{(column == 2) ? "Stretch" : "Fixed"} {column},{row}");
                }
            }
            EndTable();
        }
        if (BeginTable("table2", 6, flags)) {
            TableSetupColumn("AAA", ImGuiTableColumnFlags.WidthFixed);
            TableSetupColumn("BBB", ImGuiTableColumnFlags.WidthFixed);
            TableSetupColumn("CCC", ImGuiTableColumnFlags.WidthFixed | ImGuiTableColumnFlags.DefaultHide);
            TableSetupColumn("DDD", ImGuiTableColumnFlags.WidthStretch);
            TableSetupColumn("EEE", ImGuiTableColumnFlags.WidthStretch);
            TableSetupColumn("FFF", ImGuiTableColumnFlags.WidthStretch | ImGuiTableColumnFlags.DefaultHide);
            TableHeadersRow();
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("{(column == 2) ? "Stretch" : "Fixed"} {column},{row}");
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Reorderable, hideable, with headers")) {
        DasHelpMarker(
            "Click and drag column headers to reorder columns.\n\n" +
            "Right-click on a header to open a context menu.");
        static_let <| $() {
            var flags = ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable | ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags Reorderable", safe_addr(flags), ImGuiTableFlags.Reorderable);
        CheckboxFlags("ImGuiTableFlags Hideable", safe_addr(flags), ImGuiTableFlags.Hideable);
        CheckboxFlags("ImGuiTableFlags NoBordersInBody", safe_addr(flags), ImGuiTableFlags.NoBordersInBody);
        CheckboxFlags("ImGuiTableFlags NoBordersInBodyUntilResize", safe_addr(flags), ImGuiTableFlags.NoBordersInBodyUntilResize);
        SameLine();
        DasHelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appears in Headers)");
        PopStyleCompact();
        if (BeginTable("table1", 3, flags)) {
            TableSetupColumn("One");
            TableSetupColumn("Two");
            TableSetupColumn("Three");
            TableHeadersRow();
            for (row in range(6)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("Hello {column},{row}");
                }
            }
            EndTable();
        }
        if (BeginTable("table2", 3, flags | ImGuiTableFlags.SizingFixedFit, float2(0.0f, 0.0f))) {
            TableSetupColumn("One");
            TableSetupColumn("Two");
            TableSetupColumn("Three");
            TableHeadersRow();
            for (row in range(6)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Text("Fixed {column},{row}");
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Padding")) {
        DasHelpMarker(
            "We often want outer padding activated when any using features which makes the edges of a column visible:\n" +
            "e.g.:\n" +
            "- BorderOuterV\n" +
            "- any form of row selection\n" +
            "Because of this, activating BorderOuterV sets the default to PadOuterX. Using PadOuterX or NoPadOuterX you can override the default.\n\n" +
            "Actual padding values are using style.CellPadding.\n\n" +
            "In this demo we don't show horizontal borders to emphasis how they don't affect default horizontal padding.");
        static_let <| $() {
            var flags1 = ImGuiTableFlags.BordersV;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags PadOuterX", safe_addr(flags1), ImGuiTableFlags.PadOuterX);
        SameLine();
        DasHelpMarker("Enable outer-most padding (default if ImGuiTableFlags BordersOuterV is set)");
        CheckboxFlags("ImGuiTableFlags NoPadOuterX", safe_addr(flags1), ImGuiTableFlags.NoPadOuterX);
        SameLine();
        DasHelpMarker("Disable outer-most padding (default if ImGuiTableFlags BordersOuterV is not set)");
        CheckboxFlags("ImGuiTableFlags NoPadInnerX", safe_addr(flags1), ImGuiTableFlags.NoPadInnerX);
        SameLine();
        DasHelpMarker("Disable inner padding between columns (double inner padding if BordersOuterV is on, single inner padding if BordersOuterV is off)");
        CheckboxFlags("ImGuiTableFlags BordersOuterV", safe_addr(flags1), ImGuiTableFlags.BordersOuterV);
        CheckboxFlags("ImGuiTableFlags BordersInnerV", safe_addr(flags1), ImGuiTableFlags.BordersInnerV);
        static_let <| $() {
            var show_headers = false;
        }
        Checkbox("show_headers", safe_addr(show_headers));
        PopStyleCompact();
        if (BeginTable("table_padding", 3, flags1)) {
            if (show_headers) {
                TableSetupColumn("One");
                TableSetupColumn("Two");
                TableSetupColumn("Three");
                TableHeadersRow();
            }
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    if (row == 0) {
                        Text("Avail {GetContentRegionAvail().x}");
                    } else {
                        Button("Hello {column},{row}", float2(-FLT_MIN, 0.0f));
                    }
                }
            }
            EndTable();
        }
        DasHelpMarker("Setting style.CellPadding to (0,0) or a custom value.");
        static_let <| $() {
            var flags2 = ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg;
            var cell_padding =  float2(0.0f, 0.0f);
            var show_widget_frame_bg = true;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Borders", safe_addr(flags2), ImGuiTableFlags.Borders);
        CheckboxFlags("ImGuiTableFlags BordersH", safe_addr(flags2), ImGuiTableFlags.BordersH);
        CheckboxFlags("ImGuiTableFlags BordersV", safe_addr(flags2), ImGuiTableFlags.BordersV);
        CheckboxFlags("ImGuiTableFlags BordersInner", safe_addr(flags2), ImGuiTableFlags.BordersInner);
        CheckboxFlags("ImGuiTableFlags BordersOuter", safe_addr(flags2), ImGuiTableFlags.BordersOuter);
        CheckboxFlags("ImGuiTableFlags RowBg", safe_addr(flags2), ImGuiTableFlags.RowBg);
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags2), ImGuiTableFlags.Resizable);
        Checkbox("show_widget_frame_bg", safe_addr(show_widget_frame_bg));
        SliderFloat2("CellPadding", safe_addr(cell_padding.x), 0.0f, 10.0f, "%.0f");
        PopStyleCompact();
        PushStyleVar(ImGuiStyleVar.CellPadding, cell_padding);
        if (BeginTable("table_padding_2", 3, flags2)) {
            static_let <| $() {
                var text_bufs : array<ImGuiInputTextBuffer?>;
                var init = true;
            }
            if (!show_widget_frame_bg) {
                PushStyleColor(ImGuiCol.FrameBg, 0u);
            }
            for (cell in range(3 * 5)) {
                TableNextColumn();
                if (init) {
                    text_bufs |> push(new ImGuiInputTextBuffer("edit me", 16));
                }
                SetNextItemWidth(-FLT_MIN);
                PushID(cell);
                InputText("##cell", *(text_bufs[cell]));
                PopID();
            }
            if (!show_widget_frame_bg) {
                PopStyleColor();
            }
            init = false;
            EndTable();
        }
        PopStyleVar();
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Sizing policies")) {
        static_let <| $() {
            var flags1 = ImGuiTableFlags.BordersV | ImGuiTableFlags.BordersOuterH | ImGuiTableFlags.RowBg | ImGuiTableFlags.ContextMenuInBody;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags1), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags NoHostExtendX", safe_addr(flags1), ImGuiTableFlags.NoHostExtendX);
        PopStyleCompact();
        static_let <| $() {
            var sizing_policy_flags = fixed_array<ImGuiTableFlags>(ImGuiTableFlags.SizingFixedFit, ImGuiTableFlags.SizingFixedSame, ImGuiTableFlags.SizingStretchProp, ImGuiTableFlags.SizingStretchSame);
        }
        for (table_n in range(4)) {
            PushID(table_n);
            SetNextItemWidth(TEXT_BASE_WIDTH * 30.0);
            EditTableSizingFlags(safe_addr(sizing_policy_flags[table_n]));
            if (BeginTable("table1", 3, sizing_policy_flags[table_n] | flags1)) {
                for (row in range(3)) {
                    TableNextRow();
                    TableNextColumn();
                    Text("Oh dear");
                    TableNextColumn();
                    Text("Oh dear");
                    TableNextColumn();
                    Text("Oh dear");
                }
                EndTable();
            }
            if (BeginTable("table2", 3, sizing_policy_flags[table_n] | flags1)) {
                for (row in range(3)) {
                    TableNextRow();
                    TableNextColumn();
                    Text("AAAA");
                    TableNextColumn();
                    Text("BBBBBBBB");
                    TableNextColumn();
                    Text("CCCCCCCCCCCC");
                }
                EndTable();
            }
            PopID();
        }
        Spacing();
        TextUnformatted("Advanced");
        SameLine();
        DasHelpMarker("This section allows you to interact and see the effect of various sizing policies depending on whether Scroll is enabled and the contents of your columns.");
        static_let <| $() {
            var flags = ImGuiTableFlags.ScrollY | ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg | ImGuiTableFlags.Resizable;
            var contents_type = ContentsType.CT_ShowWidth;
            var column_count = 3;
        }
        PushStyleCompact();
        PushID("Advanced");
        PushItemWidth(TEXT_BASE_WIDTH * 30.0);
        EditTableSizingFlags(safe_addr(flags));
        unsafe {
            Combo("Contents", reinterpret<int?>(addr(contents_type)), fixed_array("Show width", "Short Text", "Long Text", "Button", "Fill Button", "InputText"));
        }
        if (contents_type == ContentsType.CT_FillButton) {
            SameLine();
            DasHelpMarker("Be mindful that using right-alignment (e.g. size.x = -FLT_MIN) creates a feedback loop where contents width can feed into auto-column width can feed into contents width.");
        }
        DragInt("Columns", safe_addr(column_count), 0.1f, 1, 64, "%d", ImGuiSliderFlags.AlwaysClamp);
        CheckboxFlags("ImGuiTableFlagsResizable", safe_addr(flags), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags PreciseWidths", safe_addr(flags), ImGuiTableFlags.PreciseWidths);
        SameLine();
        DasHelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: Without this flag: 33,33,34. With this flag: 33,33,33)." +
            " With larger number of columns, resizing will appear to be less smooth.");
        CheckboxFlags("ImGuiTableFlags ScrollX", safe_addr(flags), ImGuiTableFlags.ScrollX);
        CheckboxFlags("ImGuiTableFlags ScrollY", safe_addr(flags), ImGuiTableFlags.ScrollY);
        CheckboxFlags("ImGuiTableFlags NoClip", safe_addr(flags), ImGuiTableFlags.NoClip);
        PopItemWidth();
        PopID();
        PopStyleCompact();
        if (BeginTable("table2", column_count, flags, float2(0.0f, TEXT_BASE_HEIGHT * 7.))) {
            for (cell in range(10 * column_count)) {
                TableNextColumn();
                let column = TableGetColumnIndex();
                let row = TableGetRowIndex();
                PushID(cell);
                let lab = "Hello {column},{row}";
                if (contents_type == ContentsType.CT_ShortText) {
                    TextUnformatted(lab);
                } elif (contents_type == ContentsType.CT_LongText) {
                    Text("Some {column == 0 ? "long" : "longeeer"} text {column},{row}\nOver two lines..");
                } elif (contents_type == ContentsType.CT_ShowWidth) {
                    Text("W: {GetContentRegionAvail().x}");
                } elif (contents_type == ContentsType.CT_Button) {
                    Button(lab);
                } elif (contents_type == ContentsType.CT_FillButton) {
                    Button(lab, float2(-FLT_MIN, 0.0f));
                } elif (contents_type == ContentsType.CT_InputText) {
                    SetNextItemWidth(-FLT_MIN);
                    static_let <| $() {
                        var text_buf <- ImGuiInputTextBuffer("", 32);
                    }
                    InputText("##", text_buf);
                }
                PopID();
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Vertical scrolling, with clipping")) {
        DasHelpMarker("Here we activate ScrollY, which will create a child window container to allow hosting scrollable contents.\n" +
            "\nWe also demonstrate using ImGuiListClipper to virtualize the submission of many items.");
        static_let <| $() {
            var flags = (ImGuiTableFlags.ScrollY | ImGuiTableFlags.RowBg | ImGuiTableFlags.BordersOuter |
            ImGuiTableFlags.BordersV | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable);
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags ScrollY", safe_addr(flags), ImGuiTableFlags.ScrollY);
        PopStyleCompact();
        var outer_size = float2(0.0f, TEXT_BASE_HEIGHT * 8.);
        if (BeginTable("table_scrolly", 3, flags, outer_size)) {
            TableSetupScrollFreeze(0, 1);
            TableSetupColumn("One", ImGuiTableColumnFlags.None);
            TableSetupColumn("Two", ImGuiTableColumnFlags.None);
            TableSetupColumn("Three", ImGuiTableColumnFlags.None);
            TableHeadersRow();
            using() <| $(var clipper : ImGuiListClipper) {
                clipper |> Begin(1000);
                while (clipper |> Step()) {
                    for (row in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                        TableNextRow();
                        for (column in range(3)) {
                            TableSetColumnIndex(column);
                            Text("Hello {column},{row}");
                        }
                    }
                }
                clipper |> End();
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Horizontal scrolling")) {
        DasHelpMarker(
            "When ScrollX is enabled, the default sizing policy becomes ImGuiTableFlags SizingFixedFit, " +
            "as automatically stretching columns doesn't make much sense with horizontal scrolling.\n\n" +
            "Also note that as of the current version, you will almost always want to enable ScrollY along with ScrollX," +
            "because the container window won't automatically extend vertically to fix contents (this may be improved in future versions).");
        static_let  <| $() {
            var flags = (ImGuiTableFlags.ScrollX | ImGuiTableFlags.ScrollY | ImGuiTableFlags.RowBg | ImGuiTableFlags.BordersOuter |
                ImGuiTableFlags.BordersV | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable);
            var freeze_cols = 1;
            var freeze_rows = 1;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags ScrollX", safe_addr(flags), ImGuiTableFlags.ScrollX);
        CheckboxFlags("ImGuiTableFlags ScrollY", safe_addr(flags), ImGuiTableFlags.ScrollY);
        SetNextItemWidth(GetFrameHeight());
        DragInt("freeze_cols", safe_addr(freeze_cols), 0.2f, 0, 9, "", ImGuiSliderFlags.NoInput);
        SetNextItemWidth(GetFrameHeight());
        DragInt("freeze_rows", safe_addr(freeze_rows), 0.2f, 0, 9, "", ImGuiSliderFlags.NoInput);
        PopStyleCompact();
        var outer_size = float2(0.0f, TEXT_BASE_HEIGHT * 8.);
        if (BeginTable("table_scrollx", 7, flags, outer_size)) {
            TableSetupScrollFreeze(freeze_cols, freeze_rows);
            TableSetupColumn("Line #", ImGuiTableColumnFlags.NoHide);
            TableSetupColumn("One");
            TableSetupColumn("Two");
            TableSetupColumn("Three");
            TableSetupColumn("Four");
            TableSetupColumn("Five");
            TableSetupColumn("Six");
            TableHeadersRow();
            for (row in range(20)) {
                TableNextRow();
                for (column in range(7)) {
                    if (!TableSetColumnIndex(column) && column > 0) continue;
                    if (column == 0) {
                        Text("Line {row}");
                    } else {
                        Text("Hello world {column},{row}");
                    }
                }
            }
            EndTable();
        }
        Spacing();
        TextUnformatted("Stretch + ScrollX");
        SameLine();
        DasHelpMarker(
            "Showcase using Stretch columns + ScrollX together: " +
            "this is rather unusual and only makes sense when specifying an 'inner_width' for the table!\n" +
            "Without an explicit value, inner_width is == outer_size.x and therefore using Stretch columns + ScrollX together doesn't make sense.");
        static_let <| $() {
            var flags2 = (ImGuiTableFlags.SizingStretchSame | ImGuiTableFlags.ScrollX | ImGuiTableFlags.ScrollY | ImGuiTableFlags.BordersOuter
                | ImGuiTableFlags.RowBg | ImGuiTableFlags.ContextMenuInBody);
            var inner_width = 1000.0f;
        }
        PushStyleCompact();
        PushID("flags3");
        PushItemWidth(TEXT_BASE_WIDTH * 30.);
        CheckboxFlags("ImGuiTableFlags ScrollX", safe_addr(flags2), ImGuiTableFlags.ScrollX);
        DragFloat("inner_width", safe_addr(inner_width), 1.0f, 0.0f, FLT_MAX, "%.1f");
        PopItemWidth();
        PopID();
        PopStyleCompact();
        if (BeginTable("table2", 7, flags2, outer_size, inner_width)) {
            for (cell in range(20 * 7)) {
                TableNextColumn();
                Text("Hello world {TableGetColumnIndex()},{TableGetRowIndex()}");
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Columns flags")) {
        let column_count = 3;
        let column_names = fixed_array<string>("One", "Two", "Three");
        static_let <| $() {
            var column_flags = fixed_array<ImGuiTableColumnFlags>(ImGuiTableColumnFlags.DefaultSort, ImGuiTableColumnFlags.None, ImGuiTableColumnFlags.DefaultHide);
            var column_FlagsOut = fixed_array<ImGuiTableColumnFlags>(ImGuiTableColumnFlags.None, ImGuiTableColumnFlags.None, ImGuiTableColumnFlags.None);
        }
        if (BeginTable("table_columns_Flags checkboxes", column_count, ImGuiTableFlags.None)) {
            PushStyleCompact();
            for (column in range(column_count)) {
                TableNextColumn();
                PushID(column);
                AlignTextToFramePadding();
                Text("'{column_names[column]}'");
                Spacing();
                Text("Input flags:");
                EditTableColumnsFlags(safe_addr(column_flags[column]));
                Spacing();
                Text("Output flags:");
                ShowTableColumnsStatusFlags(column_FlagsOut[column]);
                PopID();
            }
            PopStyleCompact();
            EndTable();
        }
        let flags = (
              ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.ScrollX | ImGuiTableFlags.ScrollY
            | ImGuiTableFlags.RowBg | ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV
            | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable | ImGuiTableFlags.Sortable);
        var outer_size = float2(0.0f, TEXT_BASE_HEIGHT * 9.);
        if (BeginTable("table_columns_flags", column_count, flags, outer_size)) {
            for (column in range(column_count)) {
                TableSetupColumn(column_names[column], column_flags[column]);
            }
            TableHeadersRow();
            for (column in range(column_count)) {
                unsafe {
                    column_FlagsOut[column] = TableGetColumnFlags(column);
                }
            }
            let indent_step = floor(TEXT_BASE_WIDTH / 2.);
            for (row in range(8)) {
                Indent(indent_step);
                TableNextRow();
                for (column in range(column_count)) {
                    TableSetColumnIndex(column);
                    Text("{(column == 0) ? "Indented" : "Hello"} {TableGetColumnName(column)}");
                }
            }
            Unindent(indent_step * 8.0f);
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Columns widths")) {
        DasHelpMarker("Using TableSetupColumn() to setup default width.");
        static_let <| $() {
            var flags1 = ImGuiTableFlags.Borders | ImGuiTableFlags.NoBordersInBodyUntilResize;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags1), ImGuiTableFlags.Resizable);
        CheckboxFlags("ImGuiTableFlags NoBordersInBodyUntilResize", safe_addr(flags1), ImGuiTableFlags.NoBordersInBodyUntilResize);
        PopStyleCompact();
        if (BeginTable("table1", 3, flags1)) {
            TableSetupColumn("one", ImGuiTableColumnFlags.WidthFixed, 100.0f);
            TableSetupColumn("two", ImGuiTableColumnFlags.WidthFixed, 200.0f);
            TableSetupColumn("three", ImGuiTableColumnFlags.WidthFixed);
            TableHeadersRow();
            for (row in range(4)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    if (row == 0) {
                        Text("(w: {GetContentRegionAvail().x})");
                    } else {
                        Text("Hello {column},{row}");
                    }
                }
            }
            EndTable();
        }
        DasHelpMarker("Using TableSetupColumn() to setup explicit width.\n\nUnless _NoKeepColumnsVisible is set, fixed columns with set width may still be shrunk down if there's not enough space in the host.");
        static_let <| $() {
            var flags2 = ImGuiTableFlags.None;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags NoKeepColumnsVisible", safe_addr(flags2), ImGuiTableFlags.NoKeepColumnsVisible);
        CheckboxFlags("ImGuiTableFlags BordersInnerV", safe_addr(flags2), ImGuiTableFlags.BordersInnerV);
        CheckboxFlags("ImGuiTableFlags BordersOuterV", safe_addr(flags2), ImGuiTableFlags.BordersOuterV);
        PopStyleCompact();
        if (BeginTable("table2", 4, flags2)) {
            TableSetupColumn("", ImGuiTableColumnFlags.WidthFixed, 100.0f);
            TableSetupColumn("", ImGuiTableColumnFlags.WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            TableSetupColumn("", ImGuiTableColumnFlags.WidthFixed, TEXT_BASE_WIDTH * 30.0f);
            TableSetupColumn("", ImGuiTableColumnFlags.WidthFixed, TEXT_BASE_WIDTH * 15.0f);
            for (row in range(5)) {
                TableNextRow();
                for (column in range(4)) {
                    TableSetColumnIndex(column);
                    if (row == 0) {
                        Text("(w: GetContentRegionAvail().x)");
                    } else {
                        Text("Hello {column},{row}");
                    }
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Nested tables")) {
        DasHelpMarker("This demonstrate embedding a table into another table cell.");
        if (BeginTable("table_nested1", 2, ImGuiTableFlags.Borders | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable)) {
            TableSetupColumn("A0");
            TableSetupColumn("A1");
            TableHeadersRow();
            TableNextColumn();
            Text("A0 Row 0");
            let rows_height = TEXT_BASE_HEIGHT * 2.0;
            if (BeginTable("table_nested2", 2, ImGuiTableFlags.Borders | ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable)) {
                TableSetupColumn("B0");
                TableSetupColumn("B1");
                TableHeadersRow();
                TableNextRow(ImGuiTableRowFlags.None, rows_height);
                TableNextColumn();
                Text("B0 Row 0");
                TableNextColumn();
                Text("B1 Row 0");
                TableNextRow(ImGuiTableRowFlags.None, rows_height);
                TableNextColumn();
                Text("B0 Row 1");
                TableNextColumn();
                Text("B1 Row 1");
                EndTable();
            }
            TableNextColumn();
            Text("A1 Row 0");
            TableNextColumn();
            Text("A0 Row 1");
            TableNextColumn();
            Text("A1 Row 1");
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Row height")) {
        DasHelpMarker("You can pass a 'min_row_height' to TableNextRow().\n\n" +
            "Rows are padded with 'style.CellPadding.y' on top and bottom, so effectively the minimum row height will always be >= " +
            "'style.CellPadding.y * 2.0f'.\n\nWe cannot honor a _maximum_ row height as that would requires a unique clipping rectangle per row.");
        if (BeginTable("table_row_height", 1, ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersInnerV)) {
            for (row in range(10)) {
                let min_row_height = TEXT_BASE_HEIGHT * 0.30f * float(row);
                TableNextRow(ImGuiTableRowFlags.None, min_row_height);
                TableNextColumn();
                Text("min_row_height = {min_row_height}");
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Outer size")) {
        Text("Using NoHostExtendX and NoHostExtendY:");
        PushStyleCompact();
        static_let <| $() {
            var flags = (ImGuiTableFlags.Borders | ImGuiTableFlags.Resizable | ImGuiTableFlags.ContextMenuInBody
                | ImGuiTableFlags.RowBg | ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.NoHostExtendX);
        }
        CheckboxFlags("ImGuiTableFlags NoHostExtendX", safe_addr(flags), ImGuiTableFlags.NoHostExtendX);
        SameLine();
        DasHelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n\n" +
            "Only available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
        CheckboxFlags("ImGuiTableFlags NoHostExtendY", safe_addr(flags), ImGuiTableFlags.NoHostExtendY);
        SameLine();
        DasHelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n\n" +
            "Only available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
        PopStyleCompact();
        var outer_size = float2(0.0f, TEXT_BASE_HEIGHT * 5.5f);
        if (BeginTable("table1", 3, flags, outer_size)) {
            for (row in range(10)) {
                TableNextRow();
                for (column in range(3)) {
                    TableNextColumn();
                    Text("Cell {column},{row}");
                }
            }
            EndTable();
        }
        SameLine();
        Text("Hello!");
        Spacing();
        Text("Using explicit size:");
        if (BeginTable("table2", 3, ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg, float2(TEXT_BASE_WIDTH * 30., 0.0f))) {
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableNextColumn();
                    Text("Cell {column},{row}");
                }
            }
            EndTable();
        }
        SameLine();
        if (BeginTable("table3", 3, ImGuiTableFlags.Borders | ImGuiTableFlags.RowBg, float2(TEXT_BASE_WIDTH * 30., 0.0f))) {
            for (row in range(3)) {
                TableNextRow(ImGuiTableRowFlags.None, TEXT_BASE_HEIGHT * 1.5f);
                for (column in range(3)) {
                    TableNextColumn();
                    Text("Cell {column},{row}");
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Background color")) {
        static_let <| $() {
            var flags = ImGuiTableFlags.RowBg;
            var row_bg_type = 1;
            var row_bg_target = 1;
            var cell_bg_type = 1;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags Borders", safe_addr(flags), ImGuiTableFlags.Borders);
        CheckboxFlags("ImGuiTableFlags RowBg", safe_addr(flags), ImGuiTableFlags.RowBg);
        SameLine();
        DasHelpMarker("ImGuiTableFlags RowBg automatically sets RowBg0 to alternative colors pulled from the Style.");
        Combo("row bg type", safe_addr(row_bg_type), fixed_array("None", "Red", "Gradient"));
        Combo("row bg target", safe_addr(row_bg_target), fixed_array("RowBg0", "RowBg1"));
        SameLine();
        DasHelpMarker("Target RowBg0 to override the alternating odd/even colors,\nTarget RowBg1 to blend with them.");
        Combo("cell bg type", safe_addr(cell_bg_type), fixed_array("None", "Blue"));
        SameLine();
        DasHelpMarker("We are colorizing cells to B1->C2 here.");
        assert(row_bg_type >= 0 && row_bg_type <= 2);
        assert(row_bg_target >= 0 && row_bg_target <= 1);
        assert(cell_bg_type >= 0 && cell_bg_type <= 1);
        PopStyleCompact();
        if (BeginTable("table1", 5, flags)) {
            for (row in range(6)) {
                TableNextRow();
                if (row_bg_type != 0) {
                    let row_bg_color = GetColorU32(row_bg_type == 1 ?
                        float4(0.7f, 0.3f, 0.3f, 0.65f) :
                        float4(0.2f + float(row) * 0.1f, 0.2f, 0.2f, 0.65f));
                    unsafe {
                        TableSetBgColor(reinterpret<ImGuiTableBgTarget>(int(ImGuiTableBgTarget.RowBg0) + row_bg_target), row_bg_color);
                    }
                }
                for (column in range(5)) {
                    TableSetColumnIndex(column);
                    Text("{to_char('A' + row)}{to_char('0' + column)}");
                    if (row >= 1 && row <= 2 && column >= 1 && column <= 2 && cell_bg_type == 1) {
                        let cell_bg_color = GetColorU32(float4(0.3f, 0.3f, 0.7f, 0.65f));
                        TableSetBgColor(ImGuiTableBgTarget.CellBg, cell_bg_color);
                    }
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Tree view")) {
        static_let <| $() {
            var flags = ImGuiTableFlags.BordersV | ImGuiTableFlags.BordersOuterH | ImGuiTableFlags.Resizable | ImGuiTableFlags.RowBg | ImGuiTableFlags.NoBordersInBody;
        }
        if (BeginTable("3ways", 3, flags)) {
            TableSetupColumn("Name", ImGuiTableColumnFlags.NoHide);
            TableSetupColumn("Size", ImGuiTableColumnFlags.WidthFixed, TEXT_BASE_WIDTH * 12.0f);
            TableSetupColumn("Type", ImGuiTableColumnFlags.WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            TableHeadersRow();
            static_let <| $() {
                var nodes = [MyTreeNode(
                    Name="Root",                          Type="Folder",       Size=-1,      ChildIdx= 1, ChildCount= 3), MyTreeNode(
                    Name="Music",                         Type="Folder",       Size=-1,      ChildIdx= 4, ChildCount= 2), MyTreeNode(
                    Name="Textures",                      Type="Folder",       Size=-1,      ChildIdx= 6, ChildCount= 3), MyTreeNode(
                    Name="desktop.ini",                   Type="System file",  Size=1024,    ChildIdx=-1, ChildCount=-1), MyTreeNode(
                    Name="File1_a.wav",                   Type="Audio file",   Size=123000,  ChildIdx=-1, ChildCount=-1), MyTreeNode(
                    Name="File1_b.wav",                   Type="Audio file",   Size=456000,  ChildIdx=-1, ChildCount=-1), MyTreeNode(
                    Name="Image001.png",                  Type="Image file",   Size=203128,  ChildIdx=-1, ChildCount=-1), MyTreeNode(
                    Name="Copy of Image001.png",          Type="Image file",   Size=203256,  ChildIdx=-1, ChildCount=-1), MyTreeNode(
                    Name="Copy of Image001 (Final2).png", Type="Image file",   Size=203512,  ChildIdx=-1, ChildCount=-1
                )];
            }
            unsafe {
                DisplayNode(addr(nodes[0]), addr(nodes[0]));
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Item width")) {
        DasHelpMarker(
            "Showcase using PushItemWidth() and how it is preserved on a per-column basis.\n\n" +
            "Note that on auto-resizing non-resizable fixed columns, querying the content width for e.g. right-alignment doesn't make sense.");
        if (BeginTable("table_item_width", 3, ImGuiTableFlags.Borders)) {
            TableSetupColumn("small");
            TableSetupColumn("half");
            TableSetupColumn("right-align");
            TableHeadersRow();
            for (row in range(3)) {
                TableNextRow();
                if (row == 0) {
                    TableSetColumnIndex(0);
                    PushItemWidth(TEXT_BASE_WIDTH * 3.0f);
                    TableSetColumnIndex(1);
                    PushItemWidth(-GetContentRegionAvail().x * 0.5f);
                    TableSetColumnIndex(2);
                    PushItemWidth(-FLT_MIN);
                }
                static_let <| $() {
                    var dummy_f = 0.0f;
                }
                PushID(row);
                TableSetColumnIndex(0);
                SliderFloat("float0", safe_addr(dummy_f), 0.0f, 1.0f);
                TableSetColumnIndex(1);
                SliderFloat("float1", safe_addr(dummy_f), 0.0f, 1.0f);
                TableSetColumnIndex(2);
                SliderFloat("float2", safe_addr(dummy_f), 0.0f, 1.0f);
                PopID();
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Custom headers")) {
        let COLUMNS_COUNT = 3;
        if (BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags.Borders | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable)) {
            TableSetupColumn("Apricot");
            TableSetupColumn("Banana");
            TableSetupColumn("Cherry");
            static_let <| $() {
                var column_selected : bool[3];
            }
            TableNextRow(ImGuiTableRowFlags.Headers);
            for (column in range(COLUMNS_COUNT)) {
                TableSetColumnIndex(column);
                let column_name = TableGetColumnName(column);
                PushID(column);
                PushStyleVar(ImGuiStyleVar.FramePadding, float2(0., 0.));
                Checkbox("##checkall", safe_addr(column_selected[column]));
                PopStyleVar();
                SameLine(0.0f, GetStyle().ItemInnerSpacing.x);
                TableHeader(column_name);
                PopID();
            }
            for (row in range(5)) {
                TableNextRow();
                for (column in range(3)) {
                    TableSetColumnIndex(column);
                    Selectable("Cell {column},{row}", column_selected[column]);
                }
            }
            EndTable();
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Context menus")) {
        DasHelpMarker("By default, right-clicking over a TableHeadersRow()/TableHeader() line will open the default context-menu.\n" +
            "Using ImGuiTableFlags ContextMenuInBody we also allow right-clicking over columns body.");
        static_let <| $() {
            var flags1 = ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable | ImGuiTableFlags.Borders | ImGuiTableFlags.ContextMenuInBody;
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags ContextMenuInBody", safe_addr(flags1), ImGuiTableFlags.ContextMenuInBody);
        PopStyleCompact();
        let COLUMNS_COUNT = 3;
        if (BeginTable("table_context_menu", COLUMNS_COUNT, flags1)) {
            TableSetupColumn("One");
            TableSetupColumn("Two");
            TableSetupColumn("Three");
            TableHeadersRow();
            for (row in range(4)) {
                TableNextRow();
                for (column in range(COLUMNS_COUNT)) {
                    TableSetColumnIndex(column);
                    Text("Cell {column},{row}");
                }
            }
            EndTable();
        }
        DasHelpMarker("Demonstrate mixing table context menu (over header), item context button (over button) and custom per-colum context menu (over column body).");
        let flags2 = ImGuiTableFlags.Resizable | ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable | ImGuiTableFlags.Borders;
        if (BeginTable("table_context_menu_2", COLUMNS_COUNT, flags2)) {
            TableSetupColumn("One");
            TableSetupColumn("Two");
            TableSetupColumn("Three");
            TableHeadersRow();
            for (row in range(4)) {
                TableNextRow();
                for (column in range(COLUMNS_COUNT)) {
                    TableSetColumnIndex(column);
                    Text("Cell {column},{row}");
                    SameLine();
                    PushID(row * COLUMNS_COUNT + column);
                    SmallButton("..");
                    if (BeginPopupContextItem()) {
                        Text("This is the popup for Button(\"..\") in Cell {column},{row}");
                        if (Button("Close")) {
                            CloseCurrentPopup();
                        }
                        EndPopup();
                    }
                    PopID();
                }
            }
            var hovered_column = -1;
            for (column in range(COLUMNS_COUNT + 1)) {
                PushID(column);
                if (TableGetColumnFlags(column) && ImGuiTableColumnFlags.IsHovered) {
                    hovered_column = column;
                }
                if (hovered_column == column && !IsAnyItemHovered() && IsMouseReleased(ImGuiMouseButton.Right)) {
                    OpenPopup("MyPopup");
                }
                if (BeginPopup("MyPopup")) {
                    if (column == COLUMNS_COUNT) {
                        Text("This is a custom popup for unused space after the last column.");
                    } else {
                        Text("This is a custom popup for Column {column}");
                    }
                    if (Button("Close")) {
                        CloseCurrentPopup();
                    }
                    EndPopup();
                }
                PopID();
            }
            EndTable();
            Text("Hovered column: {hovered_column}");
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Synced instances")) {
        DasHelpMarker("Multiple tables with the same identifier will share their settings, width, visibility, order etc.");
        for (n in range(3)) {
            let open = CollapsingHeader("Synced Table {n}", ImGuiTreeNodeFlags.DefaultOpen);
            if (open && BeginTable("Table", 3, ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable |
                ImGuiTableFlags.Borders | ImGuiTableFlags.SizingFixedFit | ImGuiTableFlags.NoSavedSettings)) {
                TableSetupColumn("One");
                TableSetupColumn("Two");
                TableSetupColumn("Three");
                TableHeadersRow();
                for (cell in range(9)) {
                    TableNextColumn();
                    Text("this cell {cell}");
                }
                EndTable();
            }
        }
        TreePop();
    }

    static_let <| $() {
        let template_items_names = fixed_array<string>(
            "Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
            "Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
        );
    }
    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Sorting")) {
        static_let <| $() {
            var items : array<MyItem>;
        }
        if (items |> empty()) {
            items |> resize(50);
            for (n in range(length(items))) {
                let template_n = n % length(template_items_names);
                var item & = unsafe(items[n]);
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (n * n - n) % 20;
            }
        }
        static_let <| $() {
            var flags = (
                ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable | ImGuiTableFlags.Sortable | ImGuiTableFlags.SortMulti
                | ImGuiTableFlags.RowBg | ImGuiTableFlags.BordersOuter | ImGuiTableFlags.BordersV | ImGuiTableFlags.NoBordersInBody
                | ImGuiTableFlags.ScrollY);
        }
        PushStyleCompact();
        CheckboxFlags("ImGuiTableFlags SortMulti", safe_addr(flags), ImGuiTableFlags.SortMulti);
        SameLine();
        DasHelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
        CheckboxFlags("ImGuiTableFlags SortTristate", safe_addr(flags), ImGuiTableFlags.SortTristate);
        SameLine();
        DasHelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
        PopStyleCompact();
        if (BeginTable("table_sorting", 4, flags, float2(0.0f, TEXT_BASE_HEIGHT * 15.), 0.0f)) {
            TableSetupColumn("ID",       ImGuiTableColumnFlags.DefaultSort          | ImGuiTableColumnFlags.WidthFixed,   0.0f, uint(MyItemColumnID.ID));
            TableSetupColumn("Name",                                                  ImGuiTableColumnFlags.WidthFixed,   0.0f, uint(MyItemColumnID.Name));
            TableSetupColumn("Action",   ImGuiTableColumnFlags.NoSort               | ImGuiTableColumnFlags.WidthFixed,   0.0f, uint(MyItemColumnID.Action));
            TableSetupColumn("Quantity", ImGuiTableColumnFlags.PreferSortDescending | ImGuiTableColumnFlags.WidthStretch, 0.0f, uint(MyItemColumnID.Quantity));
            TableSetupScrollFreeze(0, 1);
            TableHeadersRow();
            var sorts_specs = TableGetSortSpecs();
            if (sorts_specs != null && sorts_specs.SpecsDirty) {
                SortMyItems(items, TableGetSortSpecs());
            }
            using() <| $(var clipper : ImGuiListClipper) {
                clipper |> Begin(length(items));
                while (clipper |> Step()) {
                    for (row_n in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                        var item & = unsafe(items[row_n]);
                        PushID(item.ID);
                        TableNextRow();
                        TableNextColumn();
                        Text("{item.ID}");
                        TableNextColumn();
                        TextUnformatted(item.Name);
                        TableNextColumn();
                        SmallButton("None");
                        TableNextColumn();
                        Text("{item.Quantity}");
                        PopID();
                    }
                }
                clipper |> End();
                EndTable();
            }
        }
        TreePop();
    }

    if (open_action != -1) {
        SetNextItemOpen(open_action != 0);
    }
    if (TreeNode("Advanced")) {
        static_let <| $() {
            var flags = (
                  ImGuiTableFlags.Resizable | ImGuiTableFlags.Reorderable | ImGuiTableFlags.Hideable
                | ImGuiTableFlags.Sortable  | ImGuiTableFlags.SortMulti
                | ImGuiTableFlags.RowBg     | ImGuiTableFlags.Borders     | ImGuiTableFlags.NoBordersInBody
                | ImGuiTableFlags.ScrollX   | ImGuiTableFlags.ScrollY
                | ImGuiTableFlags.SizingFixedFit);
            var contents_type = ContentsTypeEx.CT_SelectableSpanRow;
            let contents_type_names = fixed_array<string>("Text", "Button", "SmallButton", "FillButton", "Selectable", "Selectable (span row)");
            var freeze_cols = 1;
            var freeze_rows = 1;
            var items_count = length(template_items_names) * 2;
            var outer_size_value = float2(0.0f, 0.0f);
            var row_min_height = 0.0f;
            var inner_width_with_scroll = 0.0f;
            var outer_size_enabled = true;
            var show_headers = true;
            var show_wrapped_text = false;
        }
        if (outer_size_value.y == 0.) {
            outer_size_value.y = TEXT_BASE_HEIGHT * 12.;
        }
        if (TreeNode("Options")) {
            PushStyleCompact();
            PushItemWidth(TEXT_BASE_WIDTH * 28.0f);
            if (TreeNodeEx("Features:", ImGuiTreeNodeFlags.DefaultOpen)) {
                CheckboxFlags("ImGuiTableFlags Resizable", safe_addr(flags), ImGuiTableFlags.Resizable);
                CheckboxFlags("ImGuiTableFlags Reorderable", safe_addr(flags), ImGuiTableFlags.Reorderable);
                CheckboxFlags("ImGuiTableFlags Hideable", safe_addr(flags), ImGuiTableFlags.Hideable);
                CheckboxFlags("ImGuiTableFlags Sortable", safe_addr(flags), ImGuiTableFlags.Sortable);
                CheckboxFlags("ImGuiTableFlags NoSavedSettings", safe_addr(flags), ImGuiTableFlags.NoSavedSettings);
                CheckboxFlags("ImGuiTableFlags ContextMenuInBody", safe_addr(flags), ImGuiTableFlags.ContextMenuInBody);
                TreePop();
            }
            if (TreeNodeEx("Decorations:", ImGuiTreeNodeFlags.DefaultOpen)) {
                CheckboxFlags("ImGuiTableFlags RowBg", safe_addr(flags), ImGuiTableFlags.RowBg);
                CheckboxFlags("ImGuiTableFlags BordersV", safe_addr(flags), ImGuiTableFlags.BordersV);
                CheckboxFlags("ImGuiTableFlags BordersOuterV", safe_addr(flags), ImGuiTableFlags.BordersOuterV);
                CheckboxFlags("ImGuiTableFlags BordersInnerV", safe_addr(flags), ImGuiTableFlags.BordersInnerV);
                CheckboxFlags("ImGuiTableFlags BordersH", safe_addr(flags), ImGuiTableFlags.BordersH);
                CheckboxFlags("ImGuiTableFlags BordersOuterH", safe_addr(flags), ImGuiTableFlags.BordersOuterH);
                CheckboxFlags("ImGuiTableFlags BordersInnerH", safe_addr(flags), ImGuiTableFlags.BordersInnerH);
                CheckboxFlags("ImGuiTableFlags NoBordersInBody", safe_addr(flags), ImGuiTableFlags.NoBordersInBody);
                SameLine();
                DasHelpMarker("Disable vertical borders in columns Body (borders will always appears in Headers");
                CheckboxFlags("ImGuiTableFlags NoBordersInBodyUntilResize", safe_addr(flags), ImGuiTableFlags.NoBordersInBodyUntilResize);
                SameLine();
                DasHelpMarker("Disable vertical borders in columns Body until hovered for resize (borders will always appears in Headers)");
                TreePop();
            }
            if (TreeNodeEx("Sizing:", ImGuiTreeNodeFlags.DefaultOpen)) {
                EditTableSizingFlags(safe_addr(flags));
                SameLine();
                DasHelpMarker("In the Advanced demo we override the policy of each column so those table-wide settings have less effect that typical.");
                CheckboxFlags("ImGuiTableFlags NoHostExtendX", safe_addr(flags), ImGuiTableFlags.NoHostExtendX);
                SameLine();
                DasHelpMarker("Make outer width auto-fit to columns, overriding outer_size.x value.\n" +
                    "\nOnly available when ScrollX/ScrollY are disabled and Stretch columns are not used.");
                CheckboxFlags("ImGuiTableFlags NoHostExtendY", safe_addr(flags), ImGuiTableFlags.NoHostExtendY);
                SameLine();
                DasHelpMarker("Make outer height stop exactly at outer_size.y (prevent auto-extending table past the limit).\n" +
                    "\nOnly available when ScrollX/ScrollY are disabled. Data below the limit will be clipped and not visible.");
                CheckboxFlags("ImGuiTableFlags NoKeepColumnsVisible", safe_addr(flags), ImGuiTableFlags.NoKeepColumnsVisible);
                SameLine();
                DasHelpMarker("Only available if ScrollX is disabled.");
                CheckboxFlags("ImGuiTableFlags PreciseWidths", safe_addr(flags), ImGuiTableFlags.PreciseWidths);
                SameLine();
                DasHelpMarker("Disable distributing remainder width to stretched columns (width allocation on a 100-wide table with 3 columns: " +
                    "Without this flag: 33,33,34. With this flag: 33,33,33). With larger number of columns, resizing will appear to be less smooth.");
                CheckboxFlags("ImGuiTableFlags NoClip", safe_addr(flags), ImGuiTableFlags.NoClip);
                SameLine();
                DasHelpMarker("Disable clipping rectangle for every individual columns (reduce draw command count, " +
                    " items will be able to overflow into other columns). Generally incompatible with ScrollFreeze options.");
                TreePop();
            }
            if (TreeNodeEx("Padding:", ImGuiTreeNodeFlags.DefaultOpen)) {
                CheckboxFlags("ImGuiTableFlags PadOuterX", safe_addr(flags), ImGuiTableFlags.PadOuterX);
                CheckboxFlags("ImGuiTableFlags NoPadOuterX", safe_addr(flags), ImGuiTableFlags.NoPadOuterX);
                CheckboxFlags("ImGuiTableFlags NoPadInnerX", safe_addr(flags), ImGuiTableFlags.NoPadInnerX);
                TreePop();
            }
            if (TreeNodeEx("Scrolling:", ImGuiTreeNodeFlags.DefaultOpen)) {
                CheckboxFlags("ImGuiTableFlags ScrollX", safe_addr(flags), ImGuiTableFlags.ScrollX);
                SameLine();
                SetNextItemWidth(GetFrameHeight());
                DragInt("freeze_cols", safe_addr(freeze_cols), 0.2f, 0, 9, "", ImGuiSliderFlags.NoInput);
                CheckboxFlags("ImGuiTableFlags ScrollY", safe_addr(flags), ImGuiTableFlags.ScrollY);
                SameLine();
                SetNextItemWidth(GetFrameHeight());
                DragInt("freeze_rows", safe_addr(freeze_rows), 0.2f, 0, 9, "", ImGuiSliderFlags.NoInput);
                TreePop();
            }
            if (TreeNodeEx("Sorting:", ImGuiTreeNodeFlags.DefaultOpen)) {
                CheckboxFlags("ImGuiTableFlags SortMulti", safe_addr(flags), ImGuiTableFlags.SortMulti);
                SameLine();
                DasHelpMarker("When sorting is enabled: hold shift when clicking headers to sort on multiple column. TableGetSortSpecs() may return specs where (SpecsCount > 1).");
                CheckboxFlags("ImGuiTableFlags SortTristate", safe_addr(flags), ImGuiTableFlags.SortTristate);
                SameLine();
                DasHelpMarker("When sorting is enabled: allow no sorting, disable default sorting. TableGetSortSpecs() may return specs where (SpecsCount == 0).");
                TreePop();
            }
            if (TreeNodeEx("Other:", ImGuiTreeNodeFlags.DefaultOpen)) {
                Checkbox("show_headers", safe_addr(show_headers));
                Checkbox("show_wrapped_text", safe_addr(show_wrapped_text));
                DragFloat2("##OuterSize", safe_addr(outer_size_value.x));
                SameLine(0.0f, GetStyle().ItemInnerSpacing.x);
                Checkbox("outer_size", safe_addr(outer_size_enabled));
                SameLine();
                DasHelpMarker("If scrolling is disabled (ScrollX and ScrollY not set):\n" +
                    "- The table is output directly in the parent window.\n" +
                    "- OuterSize.x < 0.0f will right-align the table.\n" +
                    "- OuterSize.x = 0.0f will narrow fit the table unless there are any Stretch column.\n" +
                    "- OuterSize.y then becomes the minimum size for the table, which will extend vertically if there are more rows (unless NoHostExtendY is set).");
                DragFloat("inner_width (when ScrollX active)", safe_addr(inner_width_with_scroll), 1.0f, 0.0f, FLT_MAX);
                DragFloat("row_min_height", safe_addr(row_min_height), 1.0f, 0.0f, FLT_MAX);
                SameLine();
                DasHelpMarker("Specify height of the Selectable item.");
                DragInt("items_count", safe_addr(items_count), 0.1f, 0, 9999);
                unsafe {
                    Combo("items_type (first column)", reinterpret<int?> addr(contents_type), contents_type_names);
                }
                TreePop();
            }
            PopItemWidth();
            PopStyleCompact();
            Spacing();
            TreePop();
        }
        static_let <| $() {
            var items : array<MyItem>;
            var selection : array<int>;
            var items_need_sort = false;
        }
        if (length(items) != items_count) {
            items |> resize(items_count);
            for (n in range(items_count)) {
                let template_n = n % length(template_items_names);
                var item & = unsafe(items[n]);
                item.ID = n;
                item.Name = template_items_names[template_n];
                item.Quantity = (template_n == 3) ? 10 : (template_n == 4) ? 20 : 0;
            }
        }
        var parent_draw_list = GetWindowDrawList();
        let parent_draw_list_draw_cmd_count = parent_draw_list.CmdBuffer |> length;
        var table_scroll_cur = float2();
        var table_scroll_max = float2();
        var table_draw_list : ImDrawList?;
        let inner_width_to_use = (flags && ImGuiTableFlags.ScrollX) ? inner_width_with_scroll : 0.0f;
        if (BeginTable("table_advanced", 6, flags, outer_size_enabled ? outer_size_value : float2(0., 0.), inner_width_to_use)) {
            TableSetupColumn("ID",           ImGuiTableColumnFlags.DefaultSort | ImGuiTableColumnFlags.WidthFixed | ImGuiTableColumnFlags.NoHide, 0.0f, uint(MyItemColumnID.ID));
            TableSetupColumn("Name",         ImGuiTableColumnFlags.WidthFixed, 0.0f, uint(MyItemColumnID.Name));
            TableSetupColumn("Action",       ImGuiTableColumnFlags.NoSort | ImGuiTableColumnFlags.WidthFixed, 0.0f, uint(MyItemColumnID.Action));
            TableSetupColumn("Quantity",     ImGuiTableColumnFlags.PreferSortDescending, 0.0f, uint(MyItemColumnID.Quantity));
            TableSetupColumn("Description",  (flags && ImGuiTableFlags.NoHostExtendX) ? ImGuiTableColumnFlags.None : ImGuiTableColumnFlags.WidthStretch, 0.0f, uint(MyItemColumnID.Description));
            TableSetupColumn("Hidden",       ImGuiTableColumnFlags.DefaultHide | ImGuiTableColumnFlags.NoSort);
            TableSetupScrollFreeze(freeze_cols, freeze_rows);
            var sorts_specs = TableGetSortSpecs();
            if (sorts_specs != null && sorts_specs.SpecsDirty) {
                items_need_sort = true;
            }
            if (items_need_sort) {
                SortMyItems(items, TableGetSortSpecs());
                items_need_sort = false;
            }
            let sorts_specs_using_quantity = TableGetColumnFlags(3) && ImGuiTableColumnFlags.IsSorted;
            if (show_headers) {
                TableHeadersRow();
            }
            PushButtonRepeat(true);
            using() <| $(var clipper : ImGuiListClipper) {
                clipper |> Begin(length(items));
                while (clipper |> Step()) {
                    for (row_n in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                        var item & = unsafe(items[row_n]);
                        let item_is_selected = selection |> find_index(item.ID) != -1;
                        PushID(item.ID);
                        TableNextRow(ImGuiTableRowFlags.None, row_min_height);
                        TableNextColumn();
                        let lab = "{item.ID}";
                        if (contents_type == ContentsTypeEx.CT_ShortText) {
                            TextUnformatted(lab);
                        } elif (contents_type == ContentsTypeEx.CT_Button) {
                            Button(lab);
                        } elif (contents_type == ContentsTypeEx.CT_SmallButton) {
                            SmallButton(lab);
                        } elif (contents_type == ContentsTypeEx.CT_FillButton) {
                            Button(lab, float2(-FLT_MIN, 0.0f));
                        } elif (contents_type == ContentsTypeEx.CT_Selectable || contents_type == ContentsTypeEx.CT_SelectableSpanRow) {
                            let selectable_flags = ((contents_type == ContentsTypeEx.CT_SelectableSpanRow) ?
                                ImGuiSelectableFlags.SpanAllColumns | ImGuiSelectableFlags.AllowOverlap :
                                    ImGuiSelectableFlags.None);
                            if (Selectable(lab, item_is_selected, selectable_flags, float2(0., row_min_height))) {
                                if (GetIO().KeyCtrl) {
                                    if (item_is_selected) {
                                        let idx = selection |> find_index(item.ID);
                                        if (idx != -1) {
                                            selection |> erase(idx);
                                        }
                                    } else {
                                        selection |> push(item.ID);
                                    }
                                } else {
                                    selection |> clear();
                                    selection |> push(item.ID);
                                }
                            }
                        }
                        if (TableNextColumn()) {
                            TextUnformatted(item.Name);
                        }
                        if (TableNextColumn()) {
                            if (SmallButton("Chop")) {
                                item.Quantity++;
                            }
                            if (sorts_specs_using_quantity && IsItemDeactivated()) {
                                items_need_sort = true;
                            }
                            SameLine();
                            if (SmallButton("Eat")) {
                                item.Quantity--;
                            }
                            if (sorts_specs_using_quantity && IsItemDeactivated()) {
                                items_need_sort = true;
                            }
                        }
                        if (TableNextColumn()) {
                            Text("{item.Quantity}");
                        }
                        TableNextColumn();
                        if (show_wrapped_text) {
                            TextWrapped("Lorem ipsum dolor sit amet");
                        } else {
                            Text("Lorem ipsum dolor sit amet");
                        }
                        if (TableNextColumn()) {
                            Text("1234");
                        }
                        PopID();
                    }
                }
                clipper |> End();
            }
            PopButtonRepeat();
            table_scroll_cur = float2(GetScrollX(), GetScrollY());
            table_scroll_max = float2(GetScrollMaxX(), GetScrollMaxY());
            table_draw_list = GetWindowDrawList();
            EndTable();
        }
        static_let <| $() {
            var show_debug_details = false;
        }
        Checkbox("Debug details", safe_addr(show_debug_details));
        if (show_debug_details && table_draw_list != null) {
            SameLine(0.0f, 0.0f);
            let table_draw_list_draw_cmd_count = table_draw_list.CmdBuffer |> length;
            if (table_draw_list == parent_draw_list) {
                Text(": DrawCmd: +{table_draw_list_draw_cmd_count - parent_draw_list_draw_cmd_count} (in same window)");
            } else {
                Text(": DrawCmd: +{table_draw_list_draw_cmd_count - 1} (in child window), Scroll: ({table_scroll_cur.x}/{table_scroll_max.x}) ({table_scroll_cur.y}/{table_scroll_max.y})");
            }
        }
        TreePop();
    }
    PopID();
    DasShowDemoWindowColumns();
    if (disable_indent) {
        PopStyleVar();
    }
}

struct MyTreeNode {
    Name : string;
    Type : string;
    Size : int;
    ChildIdx : int;
    ChildCount : int;
}

def DisplayNode(node : MyTreeNode const?; all_nodes : MyTreeNode const?) {
    TableNextRow();
    TableNextColumn();
    let is_folder = node.ChildCount > 0;
    if (is_folder) {
        let open = TreeNodeEx(node.Name, ImGuiTreeNodeFlags.SpanFullWidth);
        TableNextColumn();
        TextDisabled("--");
        TableNextColumn();
        TextUnformatted(node.Type);
        if (open) {
            var child_n = 0;
            while (child_n < node.ChildCount) {
                unsafe {
                    DisplayNode(addr(all_nodes[node.ChildIdx + child_n]), all_nodes);
                }
                child_n++;
            }
            TreePop();
        }
    } else {
        TreeNodeEx(node.Name, ImGuiTreeNodeFlags.Leaf | ImGuiTreeNodeFlags.Bullet | ImGuiTreeNodeFlags.NoTreePushOnOpen | ImGuiTreeNodeFlags.SpanFullWidth);
        TableNextColumn();
        Text("{node.Size}");
        TableNextColumn();
        TextUnformatted(node.Type);
    }
}

def DasShowDemoWindowColumns() {
    var open = TreeNode("Legacy Columns API");
    SameLine();
    DasHelpMarker("Columns() is an old API! Prefer using the more flexible and powerful BeginTable() API!");
    if (!open) return ;
    if (TreeNode("Basic")) {
        Text("Without border:");
        Columns(3, "mycolumns3", false);
        Separator();
        for (n in range(14)) {

            if (Selectable("Item {n}")) {
                pass;
            }
            NextColumn();
        }
        Columns(1);
        Separator();
        Text("With border:");
        Columns(4, "mycolumns");
        Separator();
        Text("ID");
        NextColumn();
        Text("Name");
        NextColumn();
        Text("Path");
        NextColumn();
        Text("Hovered");
        NextColumn();
        Separator();
        let names = fixed_array<string>("One", "Two", "Three");
        let paths = fixed_array<string>("/path/one", "/path/two", "/path/three");
        static_let <| $() {
            var selected = -1;
        }
        for (i in range(3)) {
            if (Selectable("{i}", selected == i, ImGuiSelectableFlags.SpanAllColumns)) {
                selected = i;
            }
            let hovered = IsItemHovered();
            NextColumn();
            Text(names[i]);
            NextColumn();
            Text(paths[i]);
            NextColumn();
            Text("{hovered}");
            NextColumn();
        }
        Columns(1);
        Separator();
        TreePop();
    }
    if (TreeNode("Borders")) {
        static_let <| $() {
            var h_borders = true;
            var v_borders = true;
            var columns_count = 4;
        }
        let lines_count = 3;
        SetNextItemWidth(GetFontSize() * 8.);
        DragInt("##columns_count", safe_addr(columns_count), 0.1f, 2, 10, "%d columns");
        if (columns_count < 2) {
            columns_count = 2;
        }
        SameLine();
        Checkbox("horizontal", safe_addr(h_borders));
        SameLine();
        Checkbox("vertical", safe_addr(v_borders));
        Columns(columns_count, "", v_borders);
        for (i in range(columns_count * lines_count)) {
            if (h_borders && GetColumnIndex() == 0) {
                Separator();
            }
            Text("{to_char('a' + i)}{to_char('a' + i)}{to_char('a' + i)}");
            Text("Width {GetColumnWidth()}");
            Text("Avail {GetContentRegionAvail().x}");
            Text("Offset {GetColumnOffset()}");
            Text("Long text that is likely to clip");
            Button("Button", float2(-FLT_MIN, 0.0f));
            NextColumn();
        }
        Columns(1);
        if (h_borders) {
            Separator();
        }
        TreePop();
    }
    if (TreeNode("Mixed items")) {
        Columns(3, "mixed");
        Separator();
        Text("Hello");
        Button("Banana");
        NextColumn();
        Text("ImGui");
        Button("Apple");
        static_let <| $() {
            var foo = 1.0f;
        }
        InputFloat("red", safe_addr(foo), 0.05f, 0., "%.3f");
        Text("An extra line here.");
        NextColumn();
        Text("Sailor");
        Button("Corniflower");
        static_let <| $() {
            var bar = 1.0f;
        }
        InputFloat("blue", safe_addr(bar), 0.05f, 0., "%.3f");
        NextColumn();
        if (CollapsingHeader("Category A")) {
            Text("Blah blah blah");
            NextColumn();
        }
        if (CollapsingHeader("Category B")) {
            Text("Blah blah blah");
            NextColumn();
        }
        if (CollapsingHeader("Category C")) {
            Text("Blah blah blah");
            NextColumn();
        }
        Columns(1);
        Separator();
        TreePop();
    }
    if (TreeNode("Word-wrapping")) {
        Columns(2, "word-wrapping");
        Separator();
        TextWrapped("The quick brown fox jumps over the lazy dog.");
        TextWrapped("Hello Left");
        NextColumn();
        TextWrapped("The quick brown fox jumps over the lazy dog.");
        TextWrapped("Hello Right");
        Columns(1);
        Separator();
        TreePop();
    }
    if (TreeNode("Horizontal Scrolling")) {
        SetNextWindowContentSize(float2(1500.0f, 0.0f));
        var child_size = float2(0., GetFontSize() * 20.0f);
        BeginChild("##ScrollingRegion", child_size, ImGuiChildFlags.None, ImGuiWindowFlags.HorizontalScrollbar);
        Columns(10);
        let ITEMS_COUNT = 2000;
        using() <| $(var clipper : ImGuiListClipper) {
            clipper |> Begin(ITEMS_COUNT);
            while (clipper |> Step()) {
                for (i in range(clipper.DisplayStart, clipper.DisplayEnd)) {
                    for (j in range(10)) {
                        Text("Line {i} Column {j}...");
                        NextColumn();
                    }
                }
            }
            clipper |> End();
        }
        Columns(1);
        EndChild();
        TreePop();
    }
    if (TreeNode("Tree")) {
        Columns(2, "tree", true);
        for (x in range(3)) {
            let open1 = TreeNode("{x}", "Node{x}");
            NextColumn();
            Text("Node contents");
            NextColumn();
            if (open1) {
                for (y in range(3)) {
                    let open2 = TreeNode("{y}", "Node{x}.{y}");
                    NextColumn();
                    Text("Node contents");
                    if (open2) {
                        Text("Even more contents");
                        if (TreeNode("Tree in column")) {
                            Text("The quick brown fox jumps over the lazy dog");
                            TreePop();
                        }
                    }
                    NextColumn();
                    if (open2) {
                        TreePop();
                    }
                }
                TreePop();
            }
        }
        Columns(1);
        TreePop();
    }
    TreePop();
}

enum Element {
    Fire
    Earth
    Air
    Water
}

def DasShowDemoWindowWidgets() {
    if (!CollapsingHeader("Widgets")) return ;
    if (TreeNode("Basic")) {
        static_let <| $() {
            var clicked = 0;
        }
        if (Button("Button")) {
            clicked++;
        }
        if ((clicked & 1) != 0) {
            SameLine();
            Text("Thanks for clicking me!");
        }
        static_let <| $() {
            var check = true;
        }
        Checkbox("checkbox", safe_addr(check));
        static_let <| $() {
            var e = 0;
        }
        RadioButton("radio a", safe_addr(e), 0);
        SameLine();
        RadioButton("radio b", safe_addr(e), 1);
        SameLine();
        RadioButton("radio c", safe_addr(e), 2);
        for (i in range(7)) {
            if (i > 0) {
                SameLine();
            }
            PushID(i);
            PushStyleColor(ImGuiCol.Button, HSV(float(i) / 7.0f, 0.6f, 0.6f));
            PushStyleColor(ImGuiCol.ButtonHovered, HSV(float(i) / 7.0f, 0.7f, 0.7f));
            PushStyleColor(ImGuiCol.ButtonActive, HSV(float(i) / 7.0f, 0.8f, 0.8f));
            Button("Click");
            PopStyleColor(3);
            PopID();
        }
        AlignTextToFramePadding();
        Text("Hold to repeat:");
        SameLine();
        static_let <| $() {
            var counter = 0;
        }
        let spacing = GetStyle().ItemInnerSpacing.x;
        PushButtonRepeat(true);
        if (ArrowButton("##left", ImGuiDir.Left)) {
            counter--;
        }
        SameLine(0.0f, spacing);
        if (ArrowButton("##right", ImGuiDir.Right)) {
            counter++;
        }
        PopButtonRepeat();
        SameLine();
        Text("{counter}");
        Text("Hover over me");
        if (IsItemHovered()) {
            SetTooltip("I am a tooltip");
        }
        SameLine();
        Text("- or me");
        if (IsItemHovered()) {
            BeginTooltip();
            Text("I am a fancy tooltip");
            static_let <| $() {
                var arr = fixed_array<float>(0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f);
            }
            PlotLines("Curve", arr);
            EndTooltip();
        }
        Separator();
        LabelText("label", "Value");
        {
            var items = fixed_array<string>("AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK");
            static_let <| $() {
                var item_current = 0;
            }
            Combo("combo", safe_addr(item_current), items);
            SameLine();
            DasHelpMarker("Using the simplified one-liner Combo API here.\nRefer to the \"Combo\" section below for" +
                " an explanation of how to use the more flexible and general BeginCombo/EndCombo API.");
        }
        {
            static_let <| $() {
                var str0 <- ImGuiInputTextBuffer("Hello, world!", 128);
            }
            InputText("input text", str0);
            SameLine();
            DasHelpMarker(
                "USER:\n" +
                "Hold SHIFT or use mouse to select text.\n" +
                "CTRL+Left/Right to word jump.\n" +
                "CTRL+A or double-click to select all.\n" +
                "CTRL+X,CTRL+C,CTRL+V clipboard.\n" +
                "CTRL+Z,CTRL+Y undo/redo.\n" +
                "ESCAPE to revert.\n\n" +
                "PROGRAMMER:\n" +
                "You can use the ImGuiInputTextFlags CallbackResize facility if you need to wire InputText() " +
                "to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated " +
                "in imgui_demo.cpp).");
            static_let <| $() {
                var str1 <- ImGuiInputTextBuffer("", 128);
            }
            InputTextWithHint("input text (w/ hint)", "enter text here", str1);
            static_let <| $() {
                var i0 = 123;
            }
            InputInt("input int", safe_addr(i0));
            SameLine();
            DasHelpMarker(
                "You can apply arithmetic operators +,*,/ on numerical values.\n" +
                "  e.g. [ 100 ], input '*2', result becomes [ 200 ]\n" +
                "Use +- to subtract.");
            static_let <| $() {
                var f0 = 0.001f;
            }
            InputFloat("input float", safe_addr(f0), 0.01f, 1.0f, "%.3f");
            static_let <| $() {
                var d0 = 999999.00000001lf;
            }
            InputDouble("input double", safe_addr(d0), 0.01lf, 1.0lf, "%.8f");
            static_let <| $() {
                var f1 = 1.e10f;
            }
            InputFloat("input scientific", safe_addr(f1), 0.0f, 0.0f, "%e");
            SameLine();
            DasHelpMarker(
                "You can input value using the scientific notation,\n" +
                "  e.g. \"1e+8\" becomes \"100000000\".");
            static_let <| $() {
                var vec4a = fixed_array<float>(0.10f, 0.20f, 0.30f, 0.44f);
            }
            InputFloat3("input float3", safe_addr(vec4a[0]));
        }
        {
            static_let <| $() {
                var i1 = 50;
                var i2 = 42;
            }
            DragInt("drag int", safe_addr(i1), 1.);
            SameLine();
            DasHelpMarker(
                "Click and drag to edit value.\n" +
                "Hold SHIFT/ALT for faster/slower edit.\n" +
                "Double-click or CTRL+click to input value.");
            DragInt("drag int 0..100", safe_addr(i2), 1., 0, 100, "%d%%", ImGuiSliderFlags.AlwaysClamp);
            static_let <| $() {
                var f1 = 1.00f;
                var f2 = 0.0067f;
            }
            DragFloat("drag float", safe_addr(f1), 0.005f);
            DragFloat("drag small float", safe_addr(f2), 0.0001f, 0.0f, 0.0f, "%.06f ns");
        }
        {
            static_let <| $() {
                var i1 = 0;
            }
            SliderInt("slider int", safe_addr(i1), -1, 3);
            SameLine();
            DasHelpMarker("CTRL+click to input value.");
            static_let <| $() {
                var f1 = 0.123f;
                var f2 = 0.0f;
            }
            SliderFloat("slider float", safe_addr(f1), 0.0f, 1.0f, "ratio = %.3f");
            SliderFloat("slider float (log)", safe_addr(f2), -10.0f, 10.0f, "%.4f", ImGuiSliderFlags.Logarithmic);
            static_let <| $() {
                var angle = 0.0f;
            }
            SliderAngle("slider angle", safe_addr(angle));
            static_let <| $() {
                var elem = Element.Fire;
                let elems_names = fixed_array<string>("Fire", "Earth", "Air", "Water");
            }
            let elem_name = (int(elem) >= 0 && int(elem) <= int(Element.Water)) ? elems_names[int(elem)] : "Unknown";
            unsafe {
                SliderInt("slider enum", reinterpret<int?> addr(elem), 0, int(Element.Water), elem_name);
            }
            SameLine();
            DasHelpMarker("Using the format string parameter to display a name instead of the underlying integer.");
        }
        {
            static_let <| $() {
                var col1 = fixed_array<float>(1.0f, 0.0f, 0.2f);
                var col2 = fixed_array<float>(0.4f, 0.7f, 0.0f, 0.5f);
            }
            ColorEdit3("color 1", safe_addr(col1[0]));
            SameLine();
            DasHelpMarker(
                "Click on the color square to open a color picker.\n" +
                "Click and hold to use drag and drop.\n" +
                "Right-click on the color square to show options.\n" +
                "CTRL+click on individual component to input value.\n");
            ColorEdit4("color 2", safe_addr(col2[0]));
        }
        {
            static_let <| $() {
                let items = fixed_array<string>("Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon");
            }
            static_let <| $() {
                var item_current = 1;
            }
            ListBox("listbox", safe_addr(item_current), items, 4);
            SameLine();
            DasHelpMarker(
                "Using the simplified one-liner ListBox API here.\nRefer to the \"List boxes\" section below" +
                    " for an explanation of how to use the more flexible and general BeginListBox/EndListBox API.");
        }
        TreePop();
    }

    if (TreeNode("Trees")) {
        if (TreeNode("Basic trees")) {
            for (i in range(5)) {
                if (i == 0) {
                    SetNextItemOpen(true, ImGuiCond.Once);
                }
                if (TreeNode("{i}", "Child {i}")) {
                    Text("blah blah");
                    SameLine();
                    if (SmallButton("button")) {
                        pass;
                    }
                    TreePop();
                }
            }
            TreePop();
        }
        if (TreeNode("Advanced, with Selectable nodes")) {
            DasHelpMarker(
                "This is a more typical looking tree with selectable nodes.\n" +
                "Click to select, CTRL+Click to toggle, click on arrows or double-click to open.");
            static_let <| $() {
                var base_flags = ImGuiTreeNodeFlags.OpenOnArrow | ImGuiTreeNodeFlags.OpenOnDoubleClick | ImGuiTreeNodeFlags.SpanAvailWidth;
                var align_label_with_current_x_position = false;
                var test_drag_and_drop = false;
            }
            CheckboxFlags("ImGuiTreeNodeFlags OpenOnArrow",       safe_addr(base_flags), ImGuiTreeNodeFlags.OpenOnArrow);
            CheckboxFlags("ImGuiTreeNodeFlags OpenOnDoubleClick", safe_addr(base_flags), ImGuiTreeNodeFlags.OpenOnDoubleClick);
            CheckboxFlags("ImGuiTreeNodeFlags SpanAvailWidth",    safe_addr(base_flags), ImGuiTreeNodeFlags.SpanAvailWidth);
            SameLine();
            DasHelpMarker("Extend hit area to all available width instead of allowing more items to be laid out after the node.");
            CheckboxFlags("ImGuiTreeNodeFlags SpanFullWidth",     safe_addr(base_flags), ImGuiTreeNodeFlags.SpanFullWidth);
            Checkbox("Align label with current X position", safe_addr(align_label_with_current_x_position));
            Checkbox("Test tree node as drag source", safe_addr(test_drag_and_drop));
            Text("Hello!");
            if (align_label_with_current_x_position) {
                Unindent(GetTreeNodeToLabelSpacing());
            }
            static_let <| $() {
                var selection_mask = 1 << 2;
            }
            var node_clicked = -1;
            for (i in range(6)) {
                var node_flags = base_flags;
                let is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected) {
                    node_flags |= ImGuiTreeNodeFlags.Selected;
                }
                if (i < 3) {
                    let node_open = TreeNodeEx("{i}", node_flags, "Selectable Node {i}");
                    if (IsItemClicked()) {
                        node_clicked = i;
                    }
                    if (test_drag_and_drop && BeginDragDropSource()) {
                        SetDragDropPayload("_TREENODE", null, 0ul);
                        Text("This is a drag and drop source");
                        EndDragDropSource();
                    }
                    if (node_open) {
                        BulletText("Blah blah\nBlah Blah");
                        TreePop();
                    }
                } else {
                    node_flags |= ImGuiTreeNodeFlags.Leaf | ImGuiTreeNodeFlags.NoTreePushOnOpen;
                    TreeNodeEx("{i}", node_flags, "Selectable Leaf {i}");
                    if (IsItemClicked()) {
                        node_clicked = i;
                    }
                    if (test_drag_and_drop && BeginDragDropSource()) {
                        SetDragDropPayload("_TREENODE", null, 0ul);
                        Text("This is a drag and drop source");
                        EndDragDropSource();
                    }
                }
            }
            if (node_clicked != -1) {
                if (GetIO().KeyCtrl) {
                    selection_mask ^= (1 << node_clicked);
                } else {
                    selection_mask = (1 << node_clicked);
                }
            }
            if (align_label_with_current_x_position) {
                Indent(GetTreeNodeToLabelSpacing());
            }
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Collapsing Headers")) {
        static_let <| $() {
            var closable_group = true;
        }
        Checkbox("Show 2nd header", safe_addr(closable_group));
        if (CollapsingHeader("Header", ImGuiTreeNodeFlags.None)) {
            Text("IsItemHovered: {IsItemHovered()}");
            for (i in range(5)) {
                Text("Some content {i}");
            }
        }
        if (CollapsingHeader("Header with a close button", safe_addr(closable_group))) {
            Text("IsItemHovered: {IsItemHovered()}");
            for (i in range(5)) {
                Text("More content {i}");
            }
        }
        TreePop();
    }

    if (TreeNode("Bullets")) {
        BulletText("Bullet point 1");
        BulletText("Bullet point 2\nOn multiple lines");
        if (TreeNode("Tree node")) {
            BulletText("Another bullet point");
            TreePop();
        }
        Bullet();
        Text("Bullet point 3 (two calls)");
        Bullet();
        SmallButton("Button");
        TreePop();
    }

    if (TreeNode("Text")) {
        if (TreeNode("Colorful Text")) {
            TextColored(float4(1.0f, 0.0f, 1.0f, 1.0f), "Pink");
            TextColored(float4(1.0f, 1.0f, 0.0f, 1.0f), "Yellow");
            TextDisabled("Disabled");
            SameLine();
            DasHelpMarker("The TextDisabled color is stored in ImGuiStyle.");
            TreePop();
        }
        if (TreeNode("Word Wrapping")) {
            TextWrapped(
                "This text should automatically wrap on the edge of the window. The current implementation " +
                "for text wrapping follows simple rules suitable for English and possibly other languages.");
            Spacing();
            static_let <| $() {
                var wrap_width = 200.0f;
            }
            SliderFloat("Wrap width", safe_addr(wrap_width), -20., 600., "%.0f");
            var draw_list = GetWindowDrawList();
            for (n in range(2)) {
                Text("Test paragraph {n}:");
                var pos = GetCursorScreenPos();
                var marker_min = float2(pos.x + wrap_width, pos.y);
                var marker_max = float2(pos.x + wrap_width + 10., pos.y + GetTextLineHeight());
                PushTextWrapPos(GetCursorPos().x + wrap_width);
                if (n == 0) {
                    Text("The lazy dog is a good dog. This paragraph should fit within {wrap_width} pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.");
                } else {
                    Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");
                }
                *draw_list |> AddRect(GetItemRectMin(), GetItemRectMax(), IM_COL32(255, 255, 0, 255));
                *draw_list |> AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
                PopTextWrapPos();
            }
            TreePop();
        }

        if (TreeNode("UTF-8 Text")) {
            TextWrapped(
                "CJK text will only appears if the font was loaded with the appropriate CJK character ranges. " +
                "Call io.Font->AddFontFromFileTTF() manually to load extra character ranges. " +
                "Read docs/FONTS.md for details.");
            Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)");
            Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static_let <| $() {
                var buf <- ImGuiInputTextBuffer("\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e", 32);
            }
            InputText("UTF-8 input", buf);
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Images")) {
        var io & = unsafe(GetIO());
        TextWrapped(
            "Below we are displaying the font texture (which is the only texture we have access to in this demo). " +
            "Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. " +
            "Hover the texture for a zoomed view!");
        var my_tex_id = io.Fonts.TexID;
        let my_tex_w = float(io.Fonts.TexWidth);
        let my_tex_h = float(io.Fonts.TexHeight);
        {
            Text("{my_tex_w}x{my_tex_h}");
            var pos = GetCursorScreenPos();
            var uv_min = float2(0.0f, 0.0f);
            var uv_max = float2(1.0f, 1.0f);
            var tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f);
            var border_col = float4(1.0f, 1.0f, 1.0f, 0.5f);
            Image(my_tex_id, float2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            if (IsItemHovered()) {
                BeginTooltip();
                var region_sz = 32.0f;
                var region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                var region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                var zoom = 4.0f;
                if (region_x < 0.0f) {
                    region_x = 0.0f;
                } elif (region_x > my_tex_w - region_sz) {
                    region_x = my_tex_w - region_sz;
                }
                if (region_y < 0.0f) {
                    region_y = 0.0f;
                } elif (region_y > my_tex_h - region_sz) {
                    region_y = my_tex_h - region_sz;
                }
                Text("Min: ({region_x}, {region_y})");
                Text("Max: ({region_x + region_sz}, region_y + region_sz)");
                var uv0 = float2((region_x) / my_tex_w, (region_y) / my_tex_h);
                var uv1 = float2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                Image(my_tex_id, float2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                EndTooltip();
            }
        }
        TextWrapped("And now some textured buttons..");
        static_let <| $() {
            var pressed_count = 0;
        }
        for (i in range(8)) {
            PushID(i);
            var frame_padding = -1 + i;
            var size = float2(32.0f, 32.0f);
            var uv0 = float2(0.0f, 0.0f);
            var uv1 = float2(32.0f / my_tex_w, 32.0f / my_tex_h);
            var bg_col = float4(0.0f, 0.0f, 0.0f, 1.0f);
            var tint_col = float4(1.0f, 1.0f, 1.0f, 1.0f);
            if (i > 0) {
                PushStyleVar(ImGuiStyleVar.FramePadding, ImVec2(float(i) - 1.0f, float(i) - 1.0f));
            }
            if (ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col)) {
                pressed_count++;
            }
            if (i > 0) {
                PopStyleVar();
            }
            PopID();
            SameLine();
        }
        NewLine();
        Text("Pressed {pressed_count} times.");
        TreePop();
    }

    if (TreeNode("Combo")) {
        static_let <| $() {
            var flags = ImGuiComboFlags.None;
        }
        CheckboxFlags("ImGuiComboFlags PopupAlignLeft", safe_addr(flags), ImGuiComboFlags.PopupAlignLeft);
        SameLine();
        DasHelpMarker("Only makes a difference if the popup is larger than the combo");
        if (CheckboxFlags("ImGuiComboFlags NoArrowButton", safe_addr(flags), ImGuiComboFlags.NoArrowButton)) {
            flags &= ~ImGuiComboFlags.NoPreview;
        }
        if (CheckboxFlags("ImGuiComboFlags NoPreview", safe_addr(flags), ImGuiComboFlags.NoPreview)) {
            flags &= ~ImGuiComboFlags.NoArrowButton;
        }
        static_let <| $() {
            let items = fixed_array<string>("AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO");
            var item_current_idx = 0;
        }
        let combo_label = items[item_current_idx];
        if (BeginCombo("combo 1", combo_label, flags)) {
            for (n in range(length(items))) {
                let is_selected = item_current_idx == n;
                if (Selectable(items[n], is_selected)) {
                    item_current_idx = n;
                }
                if (is_selected) {
                    SetItemDefaultFocus();
                }
            }
            EndCombo();
        }
        static_let <| $() {
            var item_current_2 = 0;
        }
        Combo("combo 2 (one-liner)", safe_addr(item_current_2), fixed_array<string>("aaaa", "bbbb", "cccc", "dddd", "eeee"));
        static_let <| $() {
            var item_current_3 = -1;
        }
        Combo("combo 3 (array)", safe_addr(item_current_3), items);
        static_let <| $() {
            var func_getter <- ImGuiComboGetter() <| @(index : int; var output : string&) {
                output = "fun[{index}] = {items[index]}";
                return true;
            }
            var item_current_4 = 0;
        }
        Combo("combo 4 (function)", safe_addr(item_current_4), func_getter, length(items));
        TreePop();
    }

    if (TreeNode("List boxes")) {
        static_let <| $() {
            let items = fixed_array<string>("AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO");
            var item_current_idx = 0;
        }
        if (BeginListBox("listbox 1")) {
            for (n in range(length(items))) {
                let is_selected = (item_current_idx == n);
                if (Selectable(items[n], is_selected)) {
                    item_current_idx = n;
                }
                if (is_selected) {
                    SetItemDefaultFocus();
                }
            }
            EndListBox();
        }
        Text("Full-width:");
        if (BeginListBox("##listbox 2", float2(-FLT_MIN, 5. * GetTextLineHeightWithSpacing()))) {
            for (n in range(length(items))) {
                let is_selected = (item_current_idx == n);
                if (Selectable(items[n], is_selected)) {
                    item_current_idx = n;
                }
                if (is_selected) {
                    SetItemDefaultFocus();
                }
            }
            EndListBox();
        }
        TreePop();
    }

    if (TreeNode("Selectables")) {
        if (TreeNode("Basic")) {
            static_let <| $() {
                var selection = fixed_array<bool>(false, true, false, false, false);
            }
            Selectable("1. I am selectable", safe_addr(selection[0]));
            Selectable("2. I am selectable", safe_addr(selection[1]));
            Text("3. I am not selectable");
            Selectable("4. I am selectable", safe_addr(selection[3]));
            if (Selectable("5. I am double clickable", selection[4], ImGuiSelectableFlags.AllowDoubleClick)) {
                if (IsMouseDoubleClicked(ImGuiMouseButton.Left)) {
                    selection[4] = !selection[4];
                }
            }
            TreePop();
        }
        if (TreeNode("Selection State: Single Selection")) {
            static_let <| $() {
                var selected = -1;
            }
            for (n in range(5)) {
                if (Selectable("Object {n}", selected == n)) {
                    selected = n;
                }
            }
            TreePop();
        }
        if (TreeNode("Selection State: Multiple Selection")) {
            DasHelpMarker("Hold CTRL and click to select multiple items.");
            static_let <| $() {
                var selection : bool[5];
            }
            for (n in range(5)) {
                if (Selectable("Object {n}", selection[n])) {
                    if (!GetIO().KeyCtrl) {
                        memzero(selection);
                    }
                    selection[n] ^^= true;
                }
            }
            TreePop();
        }
        if (TreeNode("Rendering more text into the same line")) {
            static_let <| $() {
                var selected : bool[3];
            }
            Selectable("main.c",    safe_addr(selected[0]));
            SameLine(300.);
            Text(" 2,345 bytes");
            Selectable("Hello.cpp", safe_addr(selected[1]));
            SameLine(300.);
            Text("12,345 bytes");
            Selectable("Hello.h",   safe_addr(selected[2]));
            SameLine(300.);
            Text(" 2,345 bytes");
            TreePop();
        }
        if (TreeNode("In columns")) {
            static_let <| $() {
                var selected : bool[10];
            }
            if (BeginTable("split1", 3, ImGuiTableFlags.Resizable | ImGuiTableFlags.NoSavedSettings)) {
                for (i in range(10)) {
                    Selectable("Item {i}", safe_addr(selected[i]));
                }
                EndTable();
            }
            Separator();
            if (BeginTable("split2", 3, ImGuiTableFlags.Resizable | ImGuiTableFlags.NoSavedSettings)) {
                for (i in range(10)) {
                    TableNextRow();
                    TableNextColumn();
                    Selectable("Item {i}", safe_addr(selected[i]), ImGuiSelectableFlags.SpanAllColumns);
                    TableNextColumn();
                    Text("Some other contents");
                    TableNextColumn();
                    Text("123456");
                }
                EndTable();
            }
            TreePop();
        }
        if (TreeNode("Grid")) {
            static_let <| $() {
                var selected = fixed_array<int>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            }
            let time = float(GetTime());
            let winning_state = find_index(selected, 0) == 0;
            if (winning_state) {
                PushStyleVar(ImGuiStyleVar.SelectableTextAlign, float2(0.5f + 0.5f * cos(time * 2.0f), 0.5f + 0.5f * sin(time * 3.0f)));
            }
            for (y in range(4)) {
                for (x in range(4)) {
                    if (x > 0) {
                        SameLine();
                    }
                    PushID(y * 4 + x);
                    if (Selectable("Sailor", selected[y * 4 + x] != 0, ImGuiSelectableFlags.None, float2(50., 50.))) {
                        selected[y * 4 + x] ^= 1;
                        if (x > 0) {
                            selected[y * 4 + x - 1] ^= 1;
                        }
                        if (x < 3) {
                            selected[y * 4 + x + 1] ^= 1;
                        }
                        if (y > 0) {
                            selected[(y - 1) * 4 + x] ^= 1;
                        }
                        if (y < 3) {
                            selected[(y + 1) * 4 + x] ^= 1;
                        }
                    }
                    PopID();
                }
            }
            if (winning_state) {
                PopStyleVar();
            }
            TreePop();
        }
        if (TreeNode("Alignment")) {
            DasHelpMarker(
                "By default, Selectables uses style.SelectableTextAlign but it can be overridden on a per-item " +
                "basis using PushStyleVar(). You'll probably want to always keep your default situation to " +
                "left-align otherwise it becomes difficult to layout multiple items on a same line");
            static_let <| $() {
                var selected = fixed_array<bool>(true, false, true, false, true, false, true, false, true);
            }
            for (y in range(3)) {
                for (x in range(3)) {
                    var alignment = float2(float(x) / 2.0f, float(y) / 2.0f);
                    if (x > 0) {
                        SameLine();
                    }
                    PushStyleVar(ImGuiStyleVar.SelectableTextAlign, alignment);
                    Selectable("({alignment.x},{alignment.y})", safe_addr(selected[3 * y + x]), ImGuiSelectableFlags.None, float2(80., 80.));
                    PopStyleVar();
                }
            }
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Text Input")) {
        if (TreeNode("Multi-line Text Input")) {
            static_let <| $() {
                var text <- ImGuiInputTextBuffer(
                    "///\n" +
                    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n" +
                    " the hexadecimal encoding of one offending instruction,\n" +
                    " more formally, the invalid operand with locked CMPXCHG8B\n" +
                    " instruction bug, is a design flaw in the majority of\n" +
                    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n" +
                    " processors (all in the P5 microarchitecture).\n" +
                    "\n\n" +
                    "label:\n" +
                    "\tlock cmpxchg8b eax\n",
                    1024 * 16);
                var flags = ImGuiInputTextFlags.AllowTabInput;
            }
            DasHelpMarker("You can use the ImGuiInputTextFlags CallbackResize facility if you need to wire InputTextMultiline()" +
                " to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. " +
                "(This is not demonstrated in imgui_demo.cpp because we don't want to include <string> in here)");
            CheckboxFlags("ImGuiInputTextFlags ReadOnly", safe_addr(flags), ImGuiInputTextFlags.ReadOnly);
            CheckboxFlags("ImGuiInputTextFlags AllowTabInput", safe_addr(flags), ImGuiInputTextFlags.AllowTabInput);
            CheckboxFlags("ImGuiInputTextFlags CtrlEnterForNewLine", safe_addr(flags), ImGuiInputTextFlags.CtrlEnterForNewLine);
            InputTextMultiline("##source", text, float2(-FLT_MIN, GetTextLineHeight() * 16.), flags);
            TreePop();
        }
        if (TreeNode("Filtered Text Input")) {
            static_let <| $() {
                var buf1 <- ImGuiInputTextBuffer("", 64);
            }
            InputText("default",     buf1);
            static_let <| $() {
                var buf2 <- ImGuiInputTextBuffer("", 64);
            }
            InputText("decimal",     buf2, ImGuiInputTextFlags.CharsDecimal);
            static_let <| $() {
                var buf3 <- ImGuiInputTextBuffer("", 64);
            }
            InputText("hexadecimal", buf3, ImGuiInputTextFlags.CharsHexadecimal | ImGuiInputTextFlags.CharsUppercase);
            static_let <| $() {
                var buf4 <- ImGuiInputTextBuffer("", 64);
            }
            InputText("uppercase",   buf4, ImGuiInputTextFlags.CharsUppercase);
            static_let <| $() {
                var buf5 <- ImGuiInputTextBuffer("", 64);
            }
            InputText("no blank",    buf5, ImGuiInputTextFlags.CharsNoBlank);
            static_let <| $() {
                var buf6 <- ImGuiInputTextBuffer("", 64) <| @(it, data) {
                    let ch = int(data.EventChar);
                    if (ch == 'i' || ch == 'm' || ch == 'g' || ch == 'u') return 0;
                    return 1;
                }
            }
            InputText("\"imgui\" letters", buf6, ImGuiInputTextFlags.CallbackCharFilter);
            TreePop();
        }
        if (TreeNode("Password Input")) {
            static_let <| $() {
                var password <- ImGuiInputTextBuffer("password123", 64);
            }
            InputText("password", password, ImGuiInputTextFlags.Password);
            SameLine();
            DasHelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
            InputTextWithHint("password (w/ hint)", "<password>", password, ImGuiInputTextFlags.Password);
            InputText("password (clear)", password);
            TreePop();
        }

        if (TreeNode("Completion, History, Edit Callbacks")) {
            static_let <| $() {
                var buf1 <- ImGuiInputTextBuffer("", 64) <| MakeMyCallback();
            }
            InputText("Completion", buf1, ImGuiInputTextFlags.CallbackCompletion);
            SameLine();
            DasHelpMarker("Here we append \"..\" each time Tab is pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback.");
            static_let <| $() {
                var buf2 <- ImGuiInputTextBuffer("", 64) <| MakeMyCallback();
            }
            InputText("History", buf2, ImGuiInputTextFlags.CallbackHistory);
            SameLine();
            DasHelpMarker("Here we replace and select text each time Up/Down are pressed. See 'Examples>Console' for a more meaningful demonstration of using this callback.");
            static_let <| $() {
                var buf3 <- ImGuiInputTextBuffer("", 64) <| MakeMyCallback();
            }
            InputText("Edit", buf3, ImGuiInputTextFlags.CallbackEdit);
            SameLine();
            DasHelpMarker("Here we toggle the casing of the first character on every edits + count edits.");
            SameLine();
            Text("({g_edit_count})");
            TreePop();
        }

        if (TreeNode("Resize Callback")) {
            DasHelpMarker(
                "Using ImGuiInputTextFlags CallbackResize to wire your custom string type to InputText().\n\n" +
                "See misc/cpp/imgui_stdlib.h for an implementation of this for std::string.");
            static_let <| $() {
                var m_str <- ImGuiInputTextBuffer("", 64) <| @(it, data) {
                    if (data.EventFlag == ImGuiInputTextFlags.CallbackResize) {
                        it.buffer |> resize(data.BufSize);
                        unsafe {
                            data.Buf = reinterpret<string> addr(it.buffer[0]);
                        }
                    }
                    return 0;
                }
            }
            InputTextMultiline("##MyStr", m_str, float2(-FLT_MIN, GetTextLineHeight() * 16.), ImGuiInputTextFlags.CallbackResize);
            unsafe {
                let pbuf = intptr(addr(m_str.buffer[0]));
                Text("Data: {pbuf}\nSize: {length(m_str.buffer)}\nCapacity: {capacity(m_str.buffer)}");
            }
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Tabs")) {
        if (TreeNode("Basic")) {
            var tab_bar_flags = ImGuiTabBarFlags.None;
            if (BeginTabBar("MyTabBar", tab_bar_flags)) {
                if (BeginTabItem("Avocado")) {
                    Text("This is the Avocado tab!\nblah blah blah blah blah");
                    EndTabItem();
                }
                if (BeginTabItem("Broccoli")) {
                    Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    EndTabItem();
                }
                if (BeginTabItem("Cucumber")) {
                    Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    EndTabItem();
                }
                EndTabBar();
            }
            Separator();
            TreePop();
        }
        if (TreeNode("Advanced & Close Button")) {
            static_let <| $() {
                var tab_bar_flags = ImGuiTabBarFlags.Reorderable;
            }
            CheckboxFlags("ImGuiTabBarFlags Reorderable", safe_addr(tab_bar_flags), ImGuiTabBarFlags.Reorderable);
            CheckboxFlags("ImGuiTabBarFlags AutoSelectNewTabs", safe_addr(tab_bar_flags), ImGuiTabBarFlags.AutoSelectNewTabs);
            CheckboxFlags("ImGuiTabBarFlags TabListPopupButton", safe_addr(tab_bar_flags), ImGuiTabBarFlags.TabListPopupButton);
            CheckboxFlags("ImGuiTabBarFlags NoCloseWithMiddleMouseButton", safe_addr(tab_bar_flags), ImGuiTabBarFlags.NoCloseWithMiddleMouseButton);
            if (!(tab_bar_flags && ImGuiTabBarFlags.FittingPolicyMask_)) {
                tab_bar_flags |= ImGuiTabBarFlags.FittingPolicyDefault_;
            }
            if (CheckboxFlags("ImGuiTabBarFlags FittingPolicyResizeDown", safe_addr(tab_bar_flags), ImGuiTabBarFlags.FittingPolicyResizeDown)) {
                tab_bar_flags &= ~(ImGuiTabBarFlags.FittingPolicyMask_ ^ ImGuiTabBarFlags.FittingPolicyResizeDown);
            }
            if (CheckboxFlags("ImGuiTabBarFlags FittingPolicyScroll", safe_addr(tab_bar_flags), ImGuiTabBarFlags.FittingPolicyScroll)) {
                tab_bar_flags &= ~(ImGuiTabBarFlags.FittingPolicyMask_ ^ ImGuiTabBarFlags.FittingPolicyScroll);
            }
            static_let <| $() {
                let names = fixed_array<string>("Artichoke", "Beetroot", "Celery", "Daikon");
                var opened = fixed_array<bool>(true, true, true, true);
            }
            for (n in range(length(opened))) {
                if (n > 0) {
                    SameLine();
                }
                Checkbox(names[n], safe_addr(opened[n]));
            }
            if (BeginTabBar("MyTabBar", tab_bar_flags)) {
                for (n in range(length(opened))) {
                    if (opened[n] && BeginTabItem(names[n], safe_addr(opened[n]), ImGuiTabItemFlags.None)) {
                        Text("This is the {names[n]} tab!");
                        if ((n & 1) != 0) {
                            Text("I am an odd tab.");
                        }
                        EndTabItem();
                    }
                }
                EndTabBar();
            }
            Separator();
            TreePop();
        }
        if (TreeNode("TabItemButton & Leading/Trailing flags")) {
            static_let <| $() {
                var active_tabs : array<int>;
                var next_tab_id = 0;
            }
            if (next_tab_id == 0) {
                for (i in range(3)) {
                    active_tabs |> push(next_tab_id++);
                }
            }
            static_let <| $() {
                var show_leading_button = true;
                var show_trailing_button = true;
            }
            Checkbox("Show Leading TabItemButton()", safe_addr(show_leading_button));
            Checkbox("Show Trailing TabItemButton()", safe_addr(show_trailing_button));
            static_let <| $() {
                var tab_bar_flags = ImGuiTabBarFlags.AutoSelectNewTabs | ImGuiTabBarFlags.Reorderable | ImGuiTabBarFlags.FittingPolicyResizeDown;
            }
            CheckboxFlags("ImGuiTabBarFlags TabListPopupButton", safe_addr(tab_bar_flags), ImGuiTabBarFlags.TabListPopupButton);
            if (CheckboxFlags("ImGuiTabBarFlags FittingPolicyResizeDown", safe_addr(tab_bar_flags), ImGuiTabBarFlags.FittingPolicyResizeDown)) {
                tab_bar_flags &= ~(ImGuiTabBarFlags.FittingPolicyMask_ ^ ImGuiTabBarFlags.FittingPolicyResizeDown);
            }
            if (CheckboxFlags("ImGuiTabBarFlags FittingPolicyScroll", safe_addr(tab_bar_flags), ImGuiTabBarFlags.FittingPolicyScroll)) {
                tab_bar_flags &= ~(ImGuiTabBarFlags.FittingPolicyMask_ ^ ImGuiTabBarFlags.FittingPolicyScroll);
            }
            if (BeginTabBar("MyTabBar", tab_bar_flags)) {
                if (show_leading_button) {
                    if (TabItemButton("?", ImGuiTabItemFlags.Leading | ImGuiTabItemFlags.NoTooltip)) {
                        OpenPopup("MyHelpMenu");
                    }
                }
                if (BeginPopup("MyHelpMenu")) {
                    Selectable("Hello!");
                    EndPopup();
                }
                if (show_trailing_button) {
                    if (TabItemButton("+", ImGuiTabItemFlags.Trailing | ImGuiTabItemFlags.NoTooltip)) {
                        active_tabs |> push(next_tab_id++);
                    }
                }
                var n = 0;
                while (n < length(active_tabs)) {
                    var open = true;
                    let name = "{active_tabs[n]}";
                    if (BeginTabItem(name, safe_addr(open), ImGuiTabItemFlags.None)) {
                        Text("This is the {name} tab!");
                        EndTabItem();
                    }
                    if (!open) {
                        active_tabs |> erase(n);
                    } else {
                        n++;
                    }
                }
                EndTabBar();
            }
            Separator();
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Plots Widgets")) {
        static_let <| $() {
            var animate = true;
        }
        Checkbox("Animate", safe_addr(animate));
        static_let <| $() {
            var arr = fixed_array<float>(0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f);
        }
        PlotLines("Frame Times", arr);
        static_let <| $() {
            var values : float[90];
            var values_offset = 0;
            var refresh_time = 0.0lf;
        }
        if (!animate || refresh_time == 0.0lf) {
            refresh_time = GetTime();
        }
        while (refresh_time < GetTime()) {
            static_let <| $() {
                var phase = 0.0f;
            }
            values[values_offset] = cos(phase);
            values_offset = (values_offset + 1) % length(values);
            phase += 0.10f * float(values_offset);
            refresh_time += 1.0lf / 60.0lf;
        }
        {
            var average = 0.0f;
            for (val in values) {
                average += val;
            }
            average /= float(length(values));
            PlotLines("Lines", values, values_offset, "avg {average}", -1.0f, 1.0f, float2(0., 80.0f));
        }
        PlotHistogram("Histogram", arr, 0, "", 0.0f, 1.0f, float2(0., 80.0f));
        static_let <| $() {
            var Sin <- ImGuiPlotGetter() <| @(i) {
                return sin(float(i) * 0.1);
            }
            var Saw <- ImGuiPlotGetter() <| @(i) {
                return (i & 1) != 0 ? 1.0f : -1.0f;
            }
            var func_type = 0;
            var display_count = 70;
        }
        Separator();
        SetNextItemWidth(100.);
        Combo("func", safe_addr(func_type), fixed_array<string>("Sin", "Saw"));
        SameLine();
        SliderInt("Sample count", safe_addr(display_count), 1, 400);
        PlotLines("Lines", (func_type == 0) ? Sin : Saw, display_count, 0, "", -1.0f, 1.0f, float2(0., 80.));
        PlotHistogram("Histogram", (func_type == 0) ? Sin : Saw, display_count, 0, "", -1.0f, 1.0f, float2(0., 80.));
        Separator();
        static_let <| $() {
            var progress = 0.0f;
            var progress_dir = 1.0f;
        }
        if (animate) {
            progress += progress_dir * 0.4f * GetIO().DeltaTime;
            if (progress >= +1.1f) {
                progress = +1.1f;
                progress_dir *= -1.0f;
            }
            if (progress <= -0.1f) {
                progress = -0.1f;
                progress_dir *= -1.0f;
            }
        }
        ProgressBar(progress, float2(0.0f, 0.0f));
        SameLine(0.0f, GetStyle().ItemInnerSpacing.x);
        Text("Progress Bar");
        var progress_saturated = clamp(progress, 0.0f, 1.0f);
        ProgressBar(progress, float2(0.f, 0.f), "{int(progress_saturated * 1753.)}{1753}");
        TreePop();
    }

    if (TreeNode("Color/Picker Widgets")) {
        static_let <| $() {
            var color = float4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
            var alpha_preview = true;
            var alpha_half_preview = false;
            var drag_and_drop = true;
            var options_menu = true;
            var hdr = false;
        }
        Checkbox("With Alpha Preview", safe_addr(alpha_preview));
        Checkbox("With Half Alpha Preview", safe_addr(alpha_half_preview));
        Checkbox("With Drag and Drop", safe_addr(drag_and_drop));
        Checkbox("With Options Menu", safe_addr(options_menu));
        SameLine();
        DasHelpMarker("Right-click on the individual color widget to show options.");
        Checkbox("With HDR", safe_addr(hdr));
        SameLine();
        DasHelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
        var misc_flags = (
                (hdr ? ImGuiColorEditFlags.HDR : ImGuiColorEditFlags.None) |
                (drag_and_drop ? ImGuiColorEditFlags.None : ImGuiColorEditFlags.NoDragDrop) |
                (alpha_half_preview ? ImGuiColorEditFlags.AlphaPreviewHalf :
                    (alpha_preview ? ImGuiColorEditFlags.AlphaPreview : ImGuiColorEditFlags.None)) |
                (options_menu ? ImGuiColorEditFlags.None : ImGuiColorEditFlags.NoOptions));
        Text("Color widget:");
        SameLine();
        DasHelpMarker(
            "Click on the color square to open a color picker.\n" +
            "CTRL+click on individual component to input value.\n");
        ColorEdit3("MyColor##1", safe_addr(color.x), misc_flags);
        Text("Color widget HSV with Alpha:");
        ColorEdit4("MyColor##2", safe_addr(color.x), ImGuiColorEditFlags.DisplayHSV | misc_flags);
        Text("Color widget with Float Display:");
        ColorEdit4("MyColor##2f", safe_addr(color.x), ImGuiColorEditFlags.Float | misc_flags);
        Text("Color button with Picker:");
        SameLine();
        DasHelpMarker(
            "With the ImGuiColorEditFlags NoInputs flag you can hide all the slider/text inputs.\n" +
            "With the ImGuiColorEditFlags NoLabel flag you can pass a non-empty label which will only " +
            "be used for the tooltip and picker popup.");
        ColorEdit4("MyColor##3", safe_addr(color.x), ImGuiColorEditFlags.NoInputs | ImGuiColorEditFlags.NoLabel | misc_flags);
        Text("Color button with Custom Picker Popup:");
        static_let <| $() {
            var saved_palette_init = true;
            var saved_palette : float4[32];
        }
        if (saved_palette_init) {
            for (n in range(length(saved_palette))) {
                ColorConvertHSVtoRGB(float(n) / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f;
            }
            saved_palette_init = false;
        }
        static_let <| $() {
            var backup_color = float4();
        }
        var open_popup = ColorButton("MyColor##3b", color, misc_flags);
        SameLine(0., GetStyle().ItemInnerSpacing.x);
        open_popup = Button("Palette") || open_popup;
        if (open_popup) {
            OpenPopup("mypicker");
            backup_color = color;
        }
        if (BeginPopup("mypicker")) {
            Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
            Separator();
            ColorPicker4("##picker", safe_addr(color.x), misc_flags | ImGuiColorEditFlags.NoSidePreview | ImGuiColorEditFlags.NoSmallPreview);
            SameLine();
            BeginGroup();
            Text("Current");
            ColorButton("##current", color, ImGuiColorEditFlags.NoPicker | ImGuiColorEditFlags.AlphaPreviewHalf, float2(60., 40.));
            Text("Previous");
            if (ColorButton("##previous", backup_color, ImGuiColorEditFlags.NoPicker | ImGuiColorEditFlags.AlphaPreviewHalf, float2(60., 40.))) {
                color = backup_color;
            }
            Separator();
            Text("Palette");
            for (n in range(length(saved_palette))) {
                PushID(n);
                if ((n % 8) != 0) {
                    SameLine(0.0f, GetStyle().ItemSpacing.y);
                }
                var palette_button_flags = ImGuiColorEditFlags.NoAlpha | ImGuiColorEditFlags.NoPicker | ImGuiColorEditFlags.NoTooltip;
                var cc = float4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, saved_palette[n].w);
                if (ColorButton("##palette", cc, palette_button_flags, float2(20., 20.))) {
                    color = float4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w);
                }
                if (BeginDragDropTarget()) {
                    {
                        var payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F);
                        if (payload != null) {
                            unsafe {
                                memcpy(addr(saved_palette[n]), payload.Data, 4 * 3);
                            }
                        }
                    }
                    {
                        var payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F);
                        if (payload != null) {
                            unsafe {
                                memcpy(addr(saved_palette[n]), payload.Data, 4 * 4);
                            }
                        }
                    }
                    EndDragDropTarget();
                }
                PopID();
            }
            EndGroup();
            EndPopup();
        }
        Text("Color button only:");
        static_let <| $() {
            var no_border = false;
        }
        Checkbox("ImGuiColorEditFlags NoBorder", safe_addr(no_border));
        ColorButton("MyColor##3c", color, misc_flags | (no_border ? ImGuiColorEditFlags.NoBorder : ImGuiColorEditFlags.None), float2(80., 80.));
        Text("Color picker:");
        static_let <| $() {
            var alpha = true;
            var alpha_bar = true;
            var side_preview = true;
            var ref_color = false;
            var ref_color_v = float4(1.0f, 0.0f, 1.0f, 0.5f);
            var display_mode = 0;
            var picker_mode = 0;
        }
        Checkbox("With Alpha", safe_addr(alpha));
        Checkbox("With Alpha Bar", safe_addr(alpha_bar));
        Checkbox("With Side Preview", safe_addr(side_preview));
        if (side_preview) {
            SameLine();
            Checkbox("With Ref Color", safe_addr(ref_color));
            if (ref_color) {
                SameLine();
                ColorEdit4("##RefColor", safe_addr(ref_color_v.x), ImGuiColorEditFlags.NoInputs | misc_flags);
            }
        }
        Combo("Display Mode", safe_addr(display_mode), fixed_array<string>("Auto/Current", "None", "RGB Only", "HSV Only", "Hex Only"));
        SameLine();
        DasHelpMarker(
            "ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, " +
            "but the user can change it with a right-click.\n\nColorPicker defaults to displaying RGB+HSV+Hex " +
            "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");
        Combo("Picker Mode", safe_addr(picker_mode), fixed_array<string>("Auto/Current", "Hue bar + SV rect", "Hue wheel + SV triangle"));
        SameLine();
        DasHelpMarker("User can right-click the picker to change mode.");
        var flags = misc_flags;
        if (!alpha) {
            flags |= ImGuiColorEditFlags.NoAlpha;
        }
        if (alpha_bar) {
            flags |= ImGuiColorEditFlags.AlphaBar;
        }
        if (!side_preview) {
            flags |= ImGuiColorEditFlags.NoSidePreview;
        }
        if (picker_mode == 1) {
            flags |= ImGuiColorEditFlags.PickerHueBar;
        }
        if (picker_mode == 2) {
            flags |= ImGuiColorEditFlags.PickerHueWheel;
        }
        if (display_mode == 1) {
            flags |= ImGuiColorEditFlags.NoInputs;
        }
        if (display_mode == 2) {
            flags |= ImGuiColorEditFlags.DisplayRGB;
        }
        if (display_mode == 3) {
            flags |= ImGuiColorEditFlags.DisplayHSV;
        }
        if (display_mode == 4) {
            flags |= ImGuiColorEditFlags.DisplayHex;
        }
        ColorPicker4("MyColor##4", safe_addr(color.x), flags, ref_color ? safe_addr(ref_color_v.x) : null);
        Text("Set defaults in code:");
        SameLine();
        DasHelpMarker(
            "SetColorEditOptions() is designed to allow you to set boot-time default.\n" +
            "We don't have Push/Pop functions because you can force options on a per-widget basis if needed," +
            "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid" +
            "encouraging you to persistently save values that aren't forward-compatible.");
        if (Button("Default: Uint8 + HSV + Hue Bar")) {
            SetColorEditOptions(ImGuiColorEditFlags.Uint8 | ImGuiColorEditFlags.DisplayHSV | ImGuiColorEditFlags.PickerHueBar);
        }
        if (Button("Default: Float + HDR + Hue Wheel")) {
            SetColorEditOptions(ImGuiColorEditFlags.Float | ImGuiColorEditFlags.HDR | ImGuiColorEditFlags.PickerHueWheel);
        }
        static_let <| $() {
            var color_hsv = float4(0.23f, 1.0f, 1.0f, 1.0f);
        }
        Spacing();
        Text("HSV encoded colors");
        SameLine();
        DasHelpMarker(
            "By default, colors are given to ColorEdit and ColorPicker in RGB, but ImGuiColorEditFlags InputHSV" +
            "allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the" +
            "added benefit that you can manipulate hue values with the picker even when saturation or value are zero.");
        Text("Color widget with InputHSV:");
        ColorEdit4("HSV shown as RGB##1", safe_addr(color_hsv.x), ImGuiColorEditFlags.DisplayRGB | ImGuiColorEditFlags.InputHSV | ImGuiColorEditFlags.Float);
        ColorEdit4("HSV shown as HSV##1", safe_addr(color_hsv.x), ImGuiColorEditFlags.DisplayHSV | ImGuiColorEditFlags.InputHSV | ImGuiColorEditFlags.Float);
        DragFloat4("Raw HSV values", safe_addr(color_hsv.x), 0.01f, 0.0f, 1.0f);
        TreePop();
    }

    if (TreeNode("Drag/Slider Flags")) {
        static_let <| $() {
            var flags = ImGuiSliderFlags.None;
        }
        CheckboxFlags("ImGuiSliderFlags AlwaysClamp", safe_addr(flags), ImGuiSliderFlags.AlwaysClamp);
        SameLine();
        DasHelpMarker("Always clamp value to min/max bounds (if any) when input manually with CTRL+Click.");
        CheckboxFlags("ImGuiSliderFlags Logarithmic", safe_addr(flags), ImGuiSliderFlags.Logarithmic);
        SameLine();
        DasHelpMarker("Enable logarithmic editing (more precision for small values).");
        CheckboxFlags("ImGuiSliderFlags NoRoundToFormat",  safe_addr(flags), ImGuiSliderFlags.NoRoundToFormat);
        SameLine();
        DasHelpMarker("Disable rounding underlying value to match precision of the format string (e.g. %.3f values are rounded to those 3 digits).");
        CheckboxFlags("ImGuiSliderFlags NoInput", safe_addr(flags), ImGuiSliderFlags.NoInput);
        SameLine();
        DasHelpMarker("Disable CTRL+Click or Enter key allowing to input text directly into the widget.");
        static_let <| $() {
            var drag_f = 0.5f;
            var drag_i = 50;
        }
        Text("Underlying float value: {drag_f}");
        DragFloat("DragFloat (0 -> 1)",  safe_addr(drag_f), 0.005f, 0.0f, 1.0f, "%.3f", flags);
        DragFloat("DragFloat (0 -> +inf)",  safe_addr(drag_f), 0.005f, 0.0f, FLT_MAX, "%.3f", flags);
        DragFloat("DragFloat (-inf -> 1)",  safe_addr(drag_f), 0.005f, -FLT_MAX, 1.0f, "%.3f", flags);
        DragFloat("DragFloat (-inf -> +inf)",  safe_addr(drag_f), 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
        DragInt("DragInt (0 -> 100)",  safe_addr(drag_i), 0.5f, 0, 100, "%d", flags);
        static_let <| $() {
            var slider_f = 0.5f;
            var slider_i = 50;
        }
        Text("Underlying float value: {slider_f}");
        SliderFloat("SliderFloat (0 -> 1)",  safe_addr(slider_f), 0.0f, 1.0f, "%.3f", flags);
        SliderInt("SliderInt (0 -> 100)",  safe_addr(slider_i), 0, 100, "%d", flags);
        TreePop();
    }

    if (TreeNode("Range Widgets")) {
        static_let <| $() {
            var begin = 10.;
            var end = 90.;
            var begin_i = 100;
            var end_i = 1000;
        }
        DragFloatRange2("range float", safe_addr(begin), safe_addr(end), 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%", ImGuiSliderFlags.AlwaysClamp);
        DragIntRange2("range int", safe_addr(begin_i), safe_addr(end_i), 5., 0, 1000, "Min: %d units", "Max: %d units");
        DragIntRange2("range int (no bounds)", safe_addr(begin_i), safe_addr(end_i), 5., 0, 0, "Min: %d units", "Max: %d units");
        TreePop();
    }

    if (TreeNode("Data Types")) {
        let s8_zero = int8(0);
        let s8_one  = int8(1);
        let s8_fifty = int8(50);
        let s8_min = int8(-128);
        let s8_max = int8(127);
        let u8_zero = uint8(0);
        let u8_one  = uint8(1);
        let u8_fifty = uint8(50);
        let u8_min = uint8(0);
        let u8_max = uint8(255);
        let s16_zero = int16(0);
        let s16_one = int16(1);
        let s16_fifty = int16(50);
        let s16_min = int16(-32768);
        let s16_max = int16(32767);
        let u16_zero = uint16(0);
        let u16_one = uint16(1);
        let u16_fifty = uint16(50);
        let u16_min = uint16(0);
        let u16_max = uint16(65535);
        let s32_zero = 0;
        let s32_one = 1;
        let s32_fifty = 50;
        let s32_min = INT_MIN / 2;
        let s32_max = INT_MAX / 2;
        let s32_hi_a = INT_MAX / 2 - 100;
        let s32_hi_b = INT_MAX / 2;
        let u32_zero = 0u;
        let u32_one = 1u;
        let u32_fifty = 50u;
        let u32_min = 0u;
        let u32_max = UINT_MAX / 2u;
        let u32_hi_a = UINT_MAX / 2u - 100u;
        let u32_hi_b = UINT_MAX / 2u;
        let s64_zero = 0l;
        let s64_one = 1l;
        let s64_fifty = 50l;
        let s64_min = LONG_MIN / 2l;
        let s64_max = LONG_MAX / 2l;
        let s64_hi_a = LONG_MAX / 2l - 100l;
        let s64_hi_b = LONG_MAX / 2l;
        let u64_zero = 0ul;
        let u64_one = 1ul;
        let u64_fifty = 50ul;
        let u64_min = 0ul;
        let u64_max = ULONG_MAX / 2ul;
        let u64_hi_a = ULONG_MAX / 2ul - 100ul;
        let u64_hi_b = ULONG_MAX / 2ul;
        let f32_zero = 0.f;
        let f32_one = 1.f;
        let f32_lo_a = -10000000000.0f;
        let f32_hi_a = +10000000000.0f;
        let f64_zero = 0.lf;
        let f64_one = 1.lf;
        let f64_lo_a = -1000000000000000.0lf;
        let f64_hi_a = +1000000000000000.0lf;
        static_let <| $() {
            var s8_v  = int8(127);
            var u8_v  = uint8(255);
            var s16_v = int16(32767);
            var u16_v = uint16(65535);
            var s32_v = -1;
            var u32_v = -1u;
            var s64_v = -1l;
            var u64_v = -1ul;
            var f32_v = 0.123f;
            var f64_v = 90000.01234567890123456789lf;
        }
        let drag_speed = 0.2f;
        static_let <| $() {
            var drag_clamp = false;
        }
        Text("Drags:");
        Checkbox("Clamp integers to 0..50", safe_addr(drag_clamp));
        SameLine();
        DasHelpMarker(
            "As with every widgets in dear imgui, we never modify values unless there is a user interaction.\n" +
            "You can override the clamping limits by using CTRL+Click to input a value.");
        DragScalar("drag s8",        ImGuiDataType.S8,     safe_addr(s8_v),  drag_speed, drag_clamp ? safe_addr(s8_zero)  : null, drag_clamp ? safe_addr(s8_fifty)  : null);
        DragScalar("drag u8",        ImGuiDataType.U8,     safe_addr(u8_v),  drag_speed, drag_clamp ? safe_addr(u8_zero)  : null, drag_clamp ? safe_addr(u8_fifty)  : null, "%u ms");
        DragScalar("drag s16",       ImGuiDataType.S16,    safe_addr(s16_v), drag_speed, drag_clamp ? safe_addr(s16_zero) : null, drag_clamp ? safe_addr(s16_fifty) : null);
        DragScalar("drag u16",       ImGuiDataType.U16,    safe_addr(u16_v), drag_speed, drag_clamp ? safe_addr(u16_zero) : null, drag_clamp ? safe_addr(u16_fifty) : null, "%u ms");
        DragScalar("drag s32",       ImGuiDataType.S32,    safe_addr(s32_v), drag_speed, drag_clamp ? safe_addr(s32_zero) : null, drag_clamp ? safe_addr(s32_fifty) : null);
        DragScalar("drag u32",       ImGuiDataType.U32,    safe_addr(u32_v), drag_speed, drag_clamp ? safe_addr(u32_zero) : null, drag_clamp ? safe_addr(u32_fifty) : null, "%u ms");
        DragScalar("drag s64",       ImGuiDataType.S64,    safe_addr(s64_v), drag_speed, drag_clamp ? safe_addr(s64_zero) : null, drag_clamp ? safe_addr(s64_fifty) : null);
        DragScalar("drag u64",       ImGuiDataType.U64,    safe_addr(u64_v), drag_speed, drag_clamp ? safe_addr(u64_zero) : null, drag_clamp ? safe_addr(u64_fifty) : null);
        DragScalar("drag float",     ImGuiDataType.Float,  safe_addr(f32_v), 0.005f,  safe_addr(f32_zero), safe_addr(f32_one), "%f");
        DragScalar("drag float log", ImGuiDataType.Float,  safe_addr(f32_v), 0.005f,  safe_addr(f32_zero), safe_addr(f32_one), "%f", ImGuiSliderFlags.Logarithmic);
        DragScalar("drag double",    ImGuiDataType.Double, safe_addr(f64_v), 0.0005f, safe_addr(f64_zero), null,     "%.10f grams");
        DragScalar("drag double log", ImGuiDataType.Double, safe_addr(f64_v), 0.0005f, safe_addr(f64_zero), safe_addr(f64_one), "0 < %.10f < 1", ImGuiSliderFlags.Logarithmic);
        Text("Sliders");
        SliderScalar("slider s8 full",       ImGuiDataType.S8,     safe_addr(s8_v),  safe_addr(s8_min),   safe_addr(s8_max),   "%d");
        SliderScalar("slider u8 full",       ImGuiDataType.U8,     safe_addr(u8_v),  safe_addr(u8_min),   safe_addr(u8_max),   "%u");
        SliderScalar("slider s16 full",      ImGuiDataType.S16,    safe_addr(s16_v), safe_addr(s16_min),  safe_addr(s16_max),  "%d");
        SliderScalar("slider u16 full",      ImGuiDataType.U16,    safe_addr(u16_v), safe_addr(u16_min),  safe_addr(u16_max),  "%u");
        SliderScalar("slider s32 low",       ImGuiDataType.S32,    safe_addr(s32_v), safe_addr(s32_zero), safe_addr(s32_fifty), "%d");
        SliderScalar("slider s32 high",      ImGuiDataType.S32,    safe_addr(s32_v), safe_addr(s32_hi_a), safe_addr(s32_hi_b), "%d");
        SliderScalar("slider s32 full",      ImGuiDataType.S32,    safe_addr(s32_v), safe_addr(s32_min),  safe_addr(s32_max),  "%d");
        SliderScalar("slider u32 low",       ImGuiDataType.U32,    safe_addr(u32_v), safe_addr(u32_zero), safe_addr(u32_fifty), "%u");
        SliderScalar("slider u32 high",      ImGuiDataType.U32,    safe_addr(u32_v), safe_addr(u32_hi_a), safe_addr(u32_hi_b), "%u");
        SliderScalar("slider u32 full",      ImGuiDataType.U32,    safe_addr(u32_v), safe_addr(u32_min),  safe_addr(u32_max),  "%u");
        SliderScalar("slider s64 low",       ImGuiDataType.S64,    safe_addr(s64_v), safe_addr(s64_zero), safe_addr(s64_fifty), "%I64d");
        SliderScalar("slider s64 high",      ImGuiDataType.S64,    safe_addr(s64_v), safe_addr(s64_hi_a), safe_addr(s64_hi_b), "%I64d");
        SliderScalar("slider s64 full",      ImGuiDataType.S64,    safe_addr(s64_v), safe_addr(s64_min),  safe_addr(s64_max),  "%I64d");
        SliderScalar("slider u64 low",       ImGuiDataType.U64,    safe_addr(u64_v), safe_addr(u64_zero), safe_addr(u64_fifty), "%I64u ms");
        SliderScalar("slider u64 high",      ImGuiDataType.U64,    safe_addr(u64_v), safe_addr(u64_hi_a), safe_addr(u64_hi_b), "%I64u ms");
        SliderScalar("slider u64 full",      ImGuiDataType.U64,    safe_addr(u64_v), safe_addr(u64_min),  safe_addr(u64_max),  "%I64u ms");
        SliderScalar("slider float low",     ImGuiDataType.Float,  safe_addr(f32_v), safe_addr(f32_zero), safe_addr(f32_one));
        SliderScalar("slider float low log", ImGuiDataType.Float,  safe_addr(f32_v), safe_addr(f32_zero), safe_addr(f32_one),  "%.10f", ImGuiSliderFlags.Logarithmic);
        SliderScalar("slider float high",    ImGuiDataType.Float,  safe_addr(f32_v), safe_addr(f32_lo_a), safe_addr(f32_hi_a), "%e");
        SliderScalar("slider double low",    ImGuiDataType.Double, safe_addr(f64_v), safe_addr(f64_zero), safe_addr(f64_one),  "%.10f grams");
        SliderScalar("slider double low log", ImGuiDataType.Double, safe_addr(f64_v), safe_addr(f64_zero), safe_addr(f64_one),  "%.10f", ImGuiSliderFlags.Logarithmic);
        SliderScalar("slider double high",   ImGuiDataType.Double, safe_addr(f64_v), safe_addr(f64_lo_a), safe_addr(f64_hi_a), "%e grams");
        Text("Sliders (reverse)");
        SliderScalar("slider s8 reverse",    ImGuiDataType.S8,   safe_addr(s8_v),  safe_addr(s8_max),    safe_addr(s8_min), "%d");
        SliderScalar("slider u8 reverse",    ImGuiDataType.U8,   safe_addr(u8_v),  safe_addr(u8_max),    safe_addr(u8_min), "%u");
        SliderScalar("slider s32 reverse",   ImGuiDataType.S32,  safe_addr(s32_v), safe_addr(s32_fifty), safe_addr(s32_zero), "%d");
        SliderScalar("slider u32 reverse",   ImGuiDataType.U32,  safe_addr(u32_v), safe_addr(u32_fifty), safe_addr(u32_zero), "%u");
        SliderScalar("slider s64 reverse",   ImGuiDataType.S64,  safe_addr(s64_v), safe_addr(s64_fifty), safe_addr(s64_zero), "%I64d");
        SliderScalar("slider u64 reverse",   ImGuiDataType.U64,  safe_addr(u64_v), safe_addr(u64_fifty), safe_addr(u64_zero), "%I64u ms");
        static_let <| $() {
            var inputs_step = true;
        }
        Text("Inputs");
        Checkbox("Show step buttons", safe_addr(inputs_step));
        InputScalar("input s8",      ImGuiDataType.S8,     safe_addr(s8_v),  inputs_step ? safe_addr(s8_one)  : null, null, "%d");
        InputScalar("input u8",      ImGuiDataType.U8,     safe_addr(u8_v),  inputs_step ? safe_addr(u8_one)  : null, null, "%u");
        InputScalar("input s16",     ImGuiDataType.S16,    safe_addr(s16_v), inputs_step ? safe_addr(s16_one) : null, null, "%d");
        InputScalar("input u16",     ImGuiDataType.U16,    safe_addr(u16_v), inputs_step ? safe_addr(u16_one) : null, null, "%u");
        InputScalar("input s32",     ImGuiDataType.S32,    safe_addr(s32_v), inputs_step ? safe_addr(s32_one) : null, null, "%d");
        InputScalar("input s32 hex", ImGuiDataType.S32,    safe_addr(s32_v), inputs_step ? safe_addr(s32_one) : null, null, "%08X", ImGuiInputTextFlags.CharsHexadecimal);
        InputScalar("input u32",     ImGuiDataType.U32,    safe_addr(u32_v), inputs_step ? safe_addr(u32_one) : null, null, "%u");
        InputScalar("input u32 hex", ImGuiDataType.U32,    safe_addr(u32_v), inputs_step ? safe_addr(u32_one) : null, null, "%08X", ImGuiInputTextFlags.CharsHexadecimal);
        InputScalar("input s64",     ImGuiDataType.S64,    safe_addr(s64_v), inputs_step ? safe_addr(s64_one) : null);
        InputScalar("input u64",     ImGuiDataType.U64,    safe_addr(u64_v), inputs_step ? safe_addr(u64_one) : null);
        InputScalar("input float",   ImGuiDataType.Float,  safe_addr(f32_v), inputs_step ? safe_addr(f32_one) : null);
        InputScalar("input double",  ImGuiDataType.Double, safe_addr(f64_v), inputs_step ? safe_addr(f64_one) : null);
        TreePop();
    }

    if (TreeNode("Multi-component Widgets")) {
        static_let <| $() {
            var vec4f = fixed_array<float>(0.10f, 0.20f, 0.30f, 0.44f);
            var vec4i = fixed_array<int>(1, 5, 100, 255);
        }
        InputFloat2("input float2", safe_addr(vec4f));
        DragFloat2("drag float2", safe_addr(vec4f), 0.01f, 0.0f, 1.0f);
        SliderFloat2("slider float2", safe_addr(vec4f), 0.0f, 1.0f);
        InputInt2("input int2", safe_addr(vec4i));
        DragInt2("drag int2", safe_addr(vec4i), 1., 0, 255);
        SliderInt2("slider int2", safe_addr(vec4i), 0, 255);
        Spacing();
        InputFloat3("input float3", safe_addr(vec4f));
        DragFloat3("drag float3", safe_addr(vec4f), 0.01f, 0.0f, 1.0f);
        SliderFloat3("slider float3", safe_addr(vec4f), 0.0f, 1.0f);
        InputInt3("input int3", safe_addr(vec4i));
        DragInt3("drag int3", safe_addr(vec4i), 1., 0, 255);
        SliderInt3("slider int3", safe_addr(vec4i), 0, 255);
        Spacing();
        InputFloat4("input float4", safe_addr(vec4f));
        DragFloat4("drag float4", safe_addr(vec4f), 0.01f, 0.0f, 1.0f);
        SliderFloat4("slider float4", safe_addr(vec4f), 0.0f, 1.0f);
        InputInt4("input int4", safe_addr(vec4i));
        DragInt4("drag int4", safe_addr(vec4i), 1., 0, 255);
        SliderInt4("slider int4", safe_addr(vec4i), 0, 255);
        TreePop();
    }

    if (TreeNode("Vertical Sliders")) {
        let spacing = 4.;
        PushStyleVar(ImGuiStyleVar.ItemSpacing, float2(spacing, spacing));
        static_let <| $() {
            var int_value = 0;
        }
        VSliderInt("##int", float2(18., 160.), safe_addr(int_value), 0, 5);
        SameLine();
        static_let <| $() {
            var values = fixed_array<float>(0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f);
        }
        PushID("set1");
        for (i in range(7)) {
            if (i > 0) {
                SameLine();
            }
            PushID(i);
            PushStyleColor(ImGuiCol.FrameBg, HSV(float(i) / 7.0f, 0.5f, 0.5f));
            PushStyleColor(ImGuiCol.FrameBgHovered, HSV(float(i) / 7.0f, 0.6f, 0.5f));
            PushStyleColor(ImGuiCol.FrameBgActive, HSV(float(i) / 7.0f, 0.7f, 0.5f));
            PushStyleColor(ImGuiCol.SliderGrab, HSV(float(i) / 7.0f, 0.9f, 0.9f));
            VSliderFloat("##v", float2(18., 160.), safe_addr(values[i]), 0.0f, 1.0f, "");
            if (IsItemActive() || IsItemHovered()) {
                SetTooltip("{values[i]}");
            }
            PopStyleColor(4);
            PopID();
        }
        PopID();
        SameLine();
        PushID("set2");
        static_let <| $() {
            var values2 = fixed_array<float>(0.20f, 0.80f, 0.40f, 0.25f);
        }
        let rows = 3;
        let small_slider_size = float2(18., floor((160.0f - float(rows - 1) * spacing) / float(rows)));
        for (nx in range(4)) {
            if (nx > 0) {
                SameLine();
            }
            BeginGroup();
            for (ny in range(rows)) {
                PushID(nx * rows + ny);
                VSliderFloat("##v", small_slider_size, safe_addr(values2[nx]), 0.0f, 1.0f, "");
                if (IsItemActive() || IsItemHovered()) {
                    SetTooltip("{values2[nx]}");
                }
                PopID();
            }
            EndGroup();
        }
        PopID();
        SameLine();
        PushID("set3");
        for (i in range(4)) {
            if (i > 0) {
                SameLine();
            }
            PushID(i);
            PushStyleVar(ImGuiStyleVar.GrabMinSize, 40.);
            VSliderFloat("##v", float2(40., 160.), safe_addr(values[i]), 0.0f, 1.0f, "%.2f\nsec");
            PopStyleVar();
            PopID();
        }
        PopID();
        PopStyleVar();
        TreePop();
    }

    if (TreeNode("Drag and Drop")) {
        if (TreeNode("Drag and drop in standard widgets")) {
            DasHelpMarker("You can drag from the color squares.");
            static_let <| $() {
                var col1 = fixed_array<float>(1.0f, 0.0f, 0.2f);
                var col2 = fixed_array<float>(0.4f, 0.7f, 0.0f, 0.5f);
            }
            ColorEdit3("color 1", safe_addr(col1));
            ColorEdit4("color 2", safe_addr(col2));
            TreePop();
        }
        if (TreeNode("Drag and drop to copy/swap items")) {
            static_let <| $() {
                var mode = Mode.Copy;
            }
            if (RadioButton("Copy", mode == Mode.Copy)) {
                mode = Mode.Copy;
            }
            SameLine();
            if (RadioButton("Move", mode == Mode.Move)) {
                mode = Mode.Move;
                SameLine();
            }
            if (RadioButton("Swap", mode == Mode.Swap)) {
                mode = Mode.Swap;
            }
            static_let <| $() {
                var names = fixed_array<string>(
                    "Bobby", "Beatrice", "Betty",
                    "Brianna", "Barry", "Bernard",
                    "Bibi", "Blaine", "Bryn"
                );
            }
            for (n in range(length(names))) {
                PushID(n);
                if ((n % 3) != 0) {
                    SameLine();
                }
                Button(names[n], float2(60., 60.));
                if (BeginDragDropSource(ImGuiDragDropFlags.None)) {
                    SetDragDropPayload("DND_DEMO_CELL", safe_addr(n), 4ul);
                    if (mode == Mode.Copy) {
                        Text("Copy {names[n]}");
                    } elif (mode == Mode.Move) {
                        Text("Move {names[n]}");
                    } elif (mode == Mode.Swap) {
                        Text("Swap {names[n]}");
                    }
                    EndDragDropSource();
                }
                if (BeginDragDropTarget()) {
                    let payload = AcceptDragDropPayload("DND_DEMO_CELL");
                    if (payload != null) {
                        assert(payload.DataSize == 4);
                        var payload_n : int;
                        unsafe {
                            payload_n = *reinterpret<int?>(payload.Data);
                        }
                        if (mode == Mode.Copy) {
                            names[n] = names[payload_n];
                        } elif (mode == Mode.Move) {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        } elif (mode == Mode.Swap) {
                            let tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    EndDragDropTarget();
                }
                PopID();
            }
            TreePop();
        }
        if (TreeNode("Drag to reorder items (simple)")) {
            DasHelpMarker(
                "We don't use the drag and drop api at all here! " +
                "Instead we query when the item is held but not hovered, and order items accordingly.");
            static_let <| $() {
                var item_names = fixed_array<string>("Item One", "Item Two", "Item Three", "Item Four", "Item Five");
            }
            for (n in range(length(item_names))) {
                let item = item_names[n];
                Selectable(item);
                if (IsItemActive() && !IsItemHovered()) {
                    let n_next = n + (GetMouseDragDelta(ImGuiMouseButton.Left).y < 0.f ? -1 : 1);
                    if (n_next >= 0 && n_next < length(item_names)) {
                        item_names[n] = item_names[n_next];
                        item_names[n_next] = item;
                        ResetMouseDragDelta();
                    }
                }
            }
            TreePop();
        }
        TreePop();
    }

    if (TreeNode("Querying Status (Edited/Active/Focused/Hovered etc.)")) {
        static_let <| $() {
            let item_names = fixed_array<string>(
                    "Text", "Button", "Button (w/ repeat)", "Checkbox", "SliderFloat", "InputText", "InputFloat",
                    "InputFloat3", "ColorEdit4", "MenuItem", "TreeNode", "TreeNode (w/ double-click)", "Combo", "ListBox"
                );
            var item_type = 1;
        }
        Combo("Item Type", safe_addr(item_type), item_names);
        SameLine();
        DasHelpMarker("Testing how various types of items are interacting with the IsItemXXX functions. Note that " +
            "the bool return value of most ImGui function is generally equivalent to calling IsItemHovered().");
        var ret = false;
        static_let <| $() {
            var b = false;
            var col4f = fixed_array<float>(1.0f, 0.5, 0.0f, 1.0f);
            var str <- ImGuiInputTextBuffer("", 32);
        }
        if (item_type == 0) {
            Text("ITEM: Text");
        } elif (item_type == 1) {
            ret = Button("ITEM: Button");
        } elif (item_type == 2) {
            PushButtonRepeat(true);
            ret = Button("ITEM: Button");
            PopButtonRepeat();
        } elif (item_type == 3) {
            ret = Checkbox("ITEM: Checkbox", safe_addr(b));
        } elif (item_type == 4) {
            ret = SliderFloat("ITEM: SliderFloat", safe_addr(col4f), 0.0f, 1.0f);
        } elif (item_type == 5) {
            ret = InputText("ITEM: InputText", str);
        } elif (item_type == 6) {
            ret = InputFloat("ITEM: InputFloat", safe_addr(col4f), 1.0f);
        } elif (item_type == 7) {
            ret = InputFloat3("ITEM: InputFloat3", safe_addr(col4f));
        } elif (item_type == 8) {
            ret = ColorEdit4("ITEM: ColorEdit4", safe_addr(col4f));
        } elif (item_type == 9) {
            ret = MenuItem("ITEM: MenuItem");
        } elif (item_type == 10) {
            ret = TreeNode("ITEM: TreeNode");
            if (ret) {
                TreePop();
            }
        } elif (item_type == 11) {
            ret = TreeNodeEx("ITEM: TreeNode w/ ImGuiTreeNodeFlags OpenOnDoubleClick", ImGuiTreeNodeFlags.OpenOnDoubleClick | ImGuiTreeNodeFlags.NoTreePushOnOpen);
        } elif (item_type == 12) {
            static_let <| $() {
                let items = fixed_array<string>("Apple", "Banana", "Cherry", "Kiwi");
                var current = 1;
            }
            ret = Combo("ITEM: Combo", safe_addr(current), items);
        } elif (item_type == 13) {
            static_let <| $() {
                let items = fixed_array<string>("Apple", "Banana", "Cherry", "Kiwi");
                var current = 1;
            }
            ret = ListBox("ITEM: ListBox", safe_addr(current), items);
        }
        BulletText(
            "Return value = {ret}\n" +
            "IsItemFocused() = {IsItemFocused()}\n" +
            "IsItemHovered() = {IsItemHovered()}\n" +
            "IsItemHovered(AllowWhenBlockedByPopup) = {IsItemHovered(ImGuiHoveredFlags.AllowWhenBlockedByPopup)}\n" +
            "IsItemHovered(AllowWhenBlockedByActiveItem) = {IsItemHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)}\n" +
            "IsItemHovered(AllowWhenOverlapped) = {IsItemHovered(ImGuiHoveredFlags.AllowWhenOverlapped)}\n" +
            "IsItemHovered(RectOnly) = {IsItemHovered(ImGuiHoveredFlags.RectOnly)}\n" +
            "IsItemActive() = {IsItemActive()}\n" +
            "IsItemEdited() = {IsItemEdited()}\n" +
            "IsItemActivated() = {IsItemActivated()}\n" +
            "IsItemDeactivated() = {IsItemDeactivated()}\n" +
            "IsItemDeactivatedAfterEdit() = {IsItemDeactivatedAfterEdit()}\n" +
            "IsItemVisible() = {IsItemVisible()}\n" +
            "IsItemClicked() = {IsItemClicked()}\n" +
            "IsItemToggledOpen() = {IsItemToggledOpen()}\n" +
            "GetItemRectMin() = ({GetItemRectMin().x}, {GetItemRectMin().y})\n" +
            "GetItemRectMax() = ({GetItemRectMax().x}, {GetItemRectMax().y})\n" +
            "GetItemRectSize() = ({GetItemRectSize().x}, {GetItemRectSize().y})");
        static_let <| $() {
            var embed_all_inside_a_child_window = false;
        }
        Checkbox("Embed everything inside a child window (for additional testing)", safe_addr(embed_all_inside_a_child_window));
        if (embed_all_inside_a_child_window) {
            BeginChild("outer_child", float2(0., GetFontSize() * 20.0f), ImGuiChildFlags.Border);
        }
        BulletText(
            "IsWindowFocused() = {IsWindowFocused()}\n" +
            "IsWindowFocused(_ChildWindows) = {IsWindowFocused(ImGuiFocusedFlags.ChildWindows)}\n" +
            "IsWindowFocused(_ChildWindows|_RootWindow) = {IsWindowFocused(ImGuiFocusedFlags.ChildWindows | ImGuiFocusedFlags.RootWindow)}\n" +
            "IsWindowFocused(_RootWindow) = {IsWindowFocused(ImGuiFocusedFlags.RootWindow)}\n" +
            "IsWindowFocused(_AnyWindow) = {IsWindowFocused(ImGuiFocusedFlags.AnyWindow)}\n");
        BulletText(
            "IsWindowHovered() = {IsWindowHovered()}\n" +
            "IsWindowHovered(_AllowWhenBlockedByPopup) = {IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByPopup)}\n" +
            "IsWindowHovered(_AllowWhenBlockedByActiveItem) = {IsWindowHovered(ImGuiHoveredFlags.AllowWhenBlockedByActiveItem)}\n" +
            "IsWindowHovered(_ChildWindows) = {IsWindowHovered(ImGuiHoveredFlags.ChildWindows)}\n" +
            "IsWindowHovered(_ChildWindows|_RootWindow) = {IsWindowHovered(ImGuiHoveredFlags.ChildWindows | ImGuiHoveredFlags.RootWindow)}\n" +
            "IsWindowHovered(_ChildWindows|_AllowWhenBlockedByPopup) = {IsWindowHovered(ImGuiHoveredFlags.ChildWindows | ImGuiHoveredFlags.AllowWhenBlockedByPopup)}\n" +
            "IsWindowHovered(_RootWindow) = {IsWindowHovered(ImGuiHoveredFlags.RootWindow)}\n" +
            "IsWindowHovered(_AnyWindow) = {IsWindowHovered(ImGuiHoveredFlags.AnyWindow)}\n");

        BeginChild("child", float2(0., 50.), ImGuiChildFlags.Border);
        Text("This is another child window for testing the _ChildWindows flag.");
        EndChild();
        if (embed_all_inside_a_child_window) {
            EndChild();
        }
        static_let <| $() {
            var unused_str <- ImGuiInputTextBuffer("This widget is only here to be able to tab-out of the widgets above.", 128);
        }
        InputText("unused", unused_str, ImGuiInputTextFlags.ReadOnly);
        static_let <| $() {
            var test_window = false;
        }
        Checkbox("Hovered/Active tests after Begin() for title bar testing", safe_addr(test_window));
        if (test_window) {
            Begin("Title bar Hovered/Active tests", safe_addr(test_window));
            if (BeginPopupContextItem()) {
                if (MenuItem("Close")) {
                    test_window = false;
                }
                EndPopup();
            }
            Text("IsItemHovered() after begin = {IsItemHovered()} (== is title bar hovered)\n" +
                 "IsItemActive() after begin = {IsItemActive()} (== is window being clicked/moved)\n");
            End();
        }
        TreePop();
    }
}

var g_edit_count = 0

def MakeMyCallback() {
    return @(var it : ImGuiInputTextBuffer; var data : ImGuiInputTextCallbackData) : int {
        if (data.EventFlag == ImGuiInputTextFlags.CallbackCompletion) {
            data |> InsertChars(data.CursorPos, "..");
        } elif (data.EventFlag == ImGuiInputTextFlags.CallbackHistory) {
            if (data.EventKey == ImGuiKey.UpArrow) {
                data |> DeleteChars(0, data.BufTextLen);
                data |> InsertChars(0, "Pressed Up!");
                data |> SelectAll();
            } elif (data.EventKey == ImGuiKey.DownArrow) {
                data |> DeleteChars(0, data.BufTextLen);
                data |> InsertChars(0, "Pressed Down!");
                data |> SelectAll();
            }
        } elif (data.EventFlag == ImGuiInputTextFlags.CallbackEdit) {
            // note - this is mutable imgui string
            var pc : uint8?;
            unsafe {
                pc = reinterpret<uint8?> data.Buf;
            }
            let c = int(*pc);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                *pc = uint8(c ^ 32);
            }
            data.BufDirty = true;
            g_edit_count ++;
        }
        return 0;
    }
}