// this file is generated via Daslang automatic binder
// all user modifications will be lost after this file is re-generated

// from imgui.h:1945:8
struct ImGuiTableSortSpecs_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTableSortSpecs> {
	ImGuiTableSortSpecs_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiTableSortSpecs", ml, "ImGuiTableSortSpecs") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Specs)>("Specs","Specs");
		addField<DAS_BIND_MANAGED_FIELD(SpecsCount)>("SpecsCount","SpecsCount");
		addField<DAS_BIND_MANAGED_FIELD(SpecsDirty)>("SpecsDirty","SpecsDirty");
	}
};

// from imgui.h:1955:8
struct ImGuiTableColumnSortSpecs_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTableColumnSortSpecs> {
	virtual bool isLocal() const override { return true; }
	virtual bool canBePlacedInContainer() const override { return true; }
	ImGuiTableColumnSortSpecs_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiTableColumnSortSpecs", ml, "ImGuiTableColumnSortSpecs") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ColumnUserID)>("ColumnUserID","ColumnUserID");
		addField<DAS_BIND_MANAGED_FIELD(ColumnIndex)>("ColumnIndex","ColumnIndex");
		addField<DAS_BIND_MANAGED_FIELD(SortOrder)>("SortOrder","SortOrder");
	}
};

// from imgui.h:1975:8
struct ImNewWrapper_GeneratedAnnotation : ManagedStructureAnnotation<ImNewWrapper> {
	ImNewWrapper_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImNewWrapper", ml, "ImNewWrapper") {
	}
	void init () {
	}
};

// from imgui.h:2071:8
struct ImGuiStyle_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiStyle> {
	ImGuiStyle_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiStyle", ml, "ImGuiStyle") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Alpha)>("Alpha","Alpha");
		addField<DAS_BIND_MANAGED_FIELD(DisabledAlpha)>("DisabledAlpha","DisabledAlpha");
		addField<DAS_BIND_MANAGED_FIELD(WindowPadding)>("WindowPadding","WindowPadding");
		addField<DAS_BIND_MANAGED_FIELD(WindowRounding)>("WindowRounding","WindowRounding");
		addField<DAS_BIND_MANAGED_FIELD(WindowBorderSize)>("WindowBorderSize","WindowBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(WindowMinSize)>("WindowMinSize","WindowMinSize");
		addField<DAS_BIND_MANAGED_FIELD(WindowTitleAlign)>("WindowTitleAlign","WindowTitleAlign");
		addField<ImGuiDir_,offsetof(ManagedType,WindowMenuButtonPosition)>("WindowMenuButtonPosition","WindowMenuButtonPosition")
			.adjustAot("das_reinterpret<ImGuiDir_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(ChildRounding)>("ChildRounding","ChildRounding");
		addField<DAS_BIND_MANAGED_FIELD(ChildBorderSize)>("ChildBorderSize","ChildBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(PopupRounding)>("PopupRounding","PopupRounding");
		addField<DAS_BIND_MANAGED_FIELD(PopupBorderSize)>("PopupBorderSize","PopupBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(FramePadding)>("FramePadding","FramePadding");
		addField<DAS_BIND_MANAGED_FIELD(FrameRounding)>("FrameRounding","FrameRounding");
		addField<DAS_BIND_MANAGED_FIELD(FrameBorderSize)>("FrameBorderSize","FrameBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(ItemSpacing)>("ItemSpacing","ItemSpacing");
		addField<DAS_BIND_MANAGED_FIELD(ItemInnerSpacing)>("ItemInnerSpacing","ItemInnerSpacing");
		addField<DAS_BIND_MANAGED_FIELD(CellPadding)>("CellPadding","CellPadding");
		addField<DAS_BIND_MANAGED_FIELD(TouchExtraPadding)>("TouchExtraPadding","TouchExtraPadding");
		addField<DAS_BIND_MANAGED_FIELD(IndentSpacing)>("IndentSpacing","IndentSpacing");
		addField<DAS_BIND_MANAGED_FIELD(ColumnsMinSpacing)>("ColumnsMinSpacing","ColumnsMinSpacing");
		addField<DAS_BIND_MANAGED_FIELD(ScrollbarSize)>("ScrollbarSize","ScrollbarSize");
		addField<DAS_BIND_MANAGED_FIELD(ScrollbarRounding)>("ScrollbarRounding","ScrollbarRounding");
		addField<DAS_BIND_MANAGED_FIELD(GrabMinSize)>("GrabMinSize","GrabMinSize");
		addField<DAS_BIND_MANAGED_FIELD(GrabRounding)>("GrabRounding","GrabRounding");
		addField<DAS_BIND_MANAGED_FIELD(LogSliderDeadzone)>("LogSliderDeadzone","LogSliderDeadzone");
		addField<DAS_BIND_MANAGED_FIELD(TabRounding)>("TabRounding","TabRounding");
		addField<DAS_BIND_MANAGED_FIELD(TabBorderSize)>("TabBorderSize","TabBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(TabMinWidthForCloseButton)>("TabMinWidthForCloseButton","TabMinWidthForCloseButton");
		addField<DAS_BIND_MANAGED_FIELD(TabBarBorderSize)>("TabBarBorderSize","TabBarBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(TableAngledHeadersAngle)>("TableAngledHeadersAngle","TableAngledHeadersAngle");
		addField<DAS_BIND_MANAGED_FIELD(TableAngledHeadersTextAlign)>("TableAngledHeadersTextAlign","TableAngledHeadersTextAlign");
		addField<ImGuiDir_,offsetof(ManagedType,ColorButtonPosition)>("ColorButtonPosition","ColorButtonPosition")
			.adjustAot("das_reinterpret<ImGuiDir_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(ButtonTextAlign)>("ButtonTextAlign","ButtonTextAlign");
		addField<DAS_BIND_MANAGED_FIELD(SelectableTextAlign)>("SelectableTextAlign","SelectableTextAlign");
		addField<DAS_BIND_MANAGED_FIELD(SeparatorTextBorderSize)>("SeparatorTextBorderSize","SeparatorTextBorderSize");
		addField<DAS_BIND_MANAGED_FIELD(SeparatorTextAlign)>("SeparatorTextAlign","SeparatorTextAlign");
		addField<DAS_BIND_MANAGED_FIELD(SeparatorTextPadding)>("SeparatorTextPadding","SeparatorTextPadding");
		addField<DAS_BIND_MANAGED_FIELD(DisplayWindowPadding)>("DisplayWindowPadding","DisplayWindowPadding");
		addField<DAS_BIND_MANAGED_FIELD(DisplaySafeAreaPadding)>("DisplaySafeAreaPadding","DisplaySafeAreaPadding");
		addField<DAS_BIND_MANAGED_FIELD(DockingSeparatorSize)>("DockingSeparatorSize","DockingSeparatorSize");
		addField<DAS_BIND_MANAGED_FIELD(MouseCursorScale)>("MouseCursorScale","MouseCursorScale");
		addField<DAS_BIND_MANAGED_FIELD(AntiAliasedLines)>("AntiAliasedLines","AntiAliasedLines");
		addField<DAS_BIND_MANAGED_FIELD(AntiAliasedLinesUseTex)>("AntiAliasedLinesUseTex","AntiAliasedLinesUseTex");
		addField<DAS_BIND_MANAGED_FIELD(AntiAliasedFill)>("AntiAliasedFill","AntiAliasedFill");
		addField<DAS_BIND_MANAGED_FIELD(CurveTessellationTol)>("CurveTessellationTol","CurveTessellationTol");
		addField<DAS_BIND_MANAGED_FIELD(CircleTessellationMaxError)>("CircleTessellationMaxError","CircleTessellationMaxError");
		addField<DAS_BIND_MANAGED_FIELD(Colors)>("Colors","Colors");
		addField<DAS_BIND_MANAGED_FIELD(HoverStationaryDelay)>("HoverStationaryDelay","HoverStationaryDelay");
		addField<DAS_BIND_MANAGED_FIELD(HoverDelayShort)>("HoverDelayShort","HoverDelayShort");
		addField<DAS_BIND_MANAGED_FIELD(HoverDelayNormal)>("HoverDelayNormal","HoverDelayNormal");
		addField<ImGuiHoveredFlags_,offsetof(ManagedType,HoverFlagsForTooltipMouse)>("HoverFlagsForTooltipMouse","HoverFlagsForTooltipMouse")
			.adjustAot("das_reinterpret<ImGuiHoveredFlags_>::pass(",")");
		addField<ImGuiHoveredFlags_,offsetof(ManagedType,HoverFlagsForTooltipNav)>("HoverFlagsForTooltipNav","HoverFlagsForTooltipNav")
			.adjustAot("das_reinterpret<ImGuiHoveredFlags_>::pass(",")");
	}
};

// from imgui.h:2146:8
struct ImGuiKeyData_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiKeyData> {
	ImGuiKeyData_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiKeyData", ml, "ImGuiKeyData") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Down)>("Down","Down");
		addField<DAS_BIND_MANAGED_FIELD(DownDuration)>("DownDuration","DownDuration");
		addField<DAS_BIND_MANAGED_FIELD(DownDurationPrev)>("DownDurationPrev","DownDurationPrev");
		addField<DAS_BIND_MANAGED_FIELD(AnalogValue)>("AnalogValue","AnalogValue");
	}
};

// from imgui.h:2154:8
struct ImGuiIO_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiIO> {
	ImGuiIO_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiIO", ml, "ImGuiIO") {
	}
	void init () {
		addField<ImGuiConfigFlags_,offsetof(ManagedType,ConfigFlags)>("ConfigFlags","ConfigFlags")
			.adjustAot("das_reinterpret<ImGuiConfigFlags_>::pass(",")");
		addField<ImGuiBackendFlags_,offsetof(ManagedType,BackendFlags)>("BackendFlags","BackendFlags")
			.adjustAot("das_reinterpret<ImGuiBackendFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(DisplaySize)>("DisplaySize","DisplaySize");
		addField<DAS_BIND_MANAGED_FIELD(DeltaTime)>("DeltaTime","DeltaTime");
		addField<DAS_BIND_MANAGED_FIELD(IniSavingRate)>("IniSavingRate","IniSavingRate");
		addField<DAS_BIND_MANAGED_FIELD(IniFilename)>("IniFilename","IniFilename");
		addField<DAS_BIND_MANAGED_FIELD(LogFilename)>("LogFilename","LogFilename");
		addField<DAS_BIND_MANAGED_FIELD(UserData)>("UserData","UserData");
		addField<DAS_BIND_MANAGED_FIELD(Fonts)>("Fonts","Fonts");
		addField<DAS_BIND_MANAGED_FIELD(FontGlobalScale)>("FontGlobalScale","FontGlobalScale");
		addField<DAS_BIND_MANAGED_FIELD(FontAllowUserScaling)>("FontAllowUserScaling","FontAllowUserScaling");
		addField<DAS_BIND_MANAGED_FIELD(FontDefault)>("FontDefault","FontDefault");
		addField<DAS_BIND_MANAGED_FIELD(DisplayFramebufferScale)>("DisplayFramebufferScale","DisplayFramebufferScale");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDockingNoSplit)>("ConfigDockingNoSplit","ConfigDockingNoSplit");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDockingWithShift)>("ConfigDockingWithShift","ConfigDockingWithShift");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDockingAlwaysTabBar)>("ConfigDockingAlwaysTabBar","ConfigDockingAlwaysTabBar");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDockingTransparentPayload)>("ConfigDockingTransparentPayload","ConfigDockingTransparentPayload");
		addField<DAS_BIND_MANAGED_FIELD(ConfigViewportsNoAutoMerge)>("ConfigViewportsNoAutoMerge","ConfigViewportsNoAutoMerge");
		addField<DAS_BIND_MANAGED_FIELD(ConfigViewportsNoTaskBarIcon)>("ConfigViewportsNoTaskBarIcon","ConfigViewportsNoTaskBarIcon");
		addField<DAS_BIND_MANAGED_FIELD(ConfigViewportsNoDecoration)>("ConfigViewportsNoDecoration","ConfigViewportsNoDecoration");
		addField<DAS_BIND_MANAGED_FIELD(ConfigViewportsNoDefaultParent)>("ConfigViewportsNoDefaultParent","ConfigViewportsNoDefaultParent");
		addField<DAS_BIND_MANAGED_FIELD(MouseDrawCursor)>("MouseDrawCursor","MouseDrawCursor");
		addField<DAS_BIND_MANAGED_FIELD(ConfigMacOSXBehaviors)>("ConfigMacOSXBehaviors","ConfigMacOSXBehaviors");
		addField<DAS_BIND_MANAGED_FIELD(ConfigInputTrickleEventQueue)>("ConfigInputTrickleEventQueue","ConfigInputTrickleEventQueue");
		addField<DAS_BIND_MANAGED_FIELD(ConfigInputTextCursorBlink)>("ConfigInputTextCursorBlink","ConfigInputTextCursorBlink");
		addField<DAS_BIND_MANAGED_FIELD(ConfigInputTextEnterKeepActive)>("ConfigInputTextEnterKeepActive","ConfigInputTextEnterKeepActive");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDragClickToInputText)>("ConfigDragClickToInputText","ConfigDragClickToInputText");
		addField<DAS_BIND_MANAGED_FIELD(ConfigWindowsResizeFromEdges)>("ConfigWindowsResizeFromEdges","ConfigWindowsResizeFromEdges");
		addField<DAS_BIND_MANAGED_FIELD(ConfigWindowsMoveFromTitleBarOnly)>("ConfigWindowsMoveFromTitleBarOnly","ConfigWindowsMoveFromTitleBarOnly");
		addField<DAS_BIND_MANAGED_FIELD(ConfigMemoryCompactTimer)>("ConfigMemoryCompactTimer","ConfigMemoryCompactTimer");
		addField<DAS_BIND_MANAGED_FIELD(MouseDoubleClickTime)>("MouseDoubleClickTime","MouseDoubleClickTime");
		addField<DAS_BIND_MANAGED_FIELD(MouseDoubleClickMaxDist)>("MouseDoubleClickMaxDist","MouseDoubleClickMaxDist");
		addField<DAS_BIND_MANAGED_FIELD(MouseDragThreshold)>("MouseDragThreshold","MouseDragThreshold");
		addField<DAS_BIND_MANAGED_FIELD(KeyRepeatDelay)>("KeyRepeatDelay","KeyRepeatDelay");
		addField<DAS_BIND_MANAGED_FIELD(KeyRepeatRate)>("KeyRepeatRate","KeyRepeatRate");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDebugIsDebuggerPresent)>("ConfigDebugIsDebuggerPresent","ConfigDebugIsDebuggerPresent");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDebugBeginReturnValueOnce)>("ConfigDebugBeginReturnValueOnce","ConfigDebugBeginReturnValueOnce");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDebugBeginReturnValueLoop)>("ConfigDebugBeginReturnValueLoop","ConfigDebugBeginReturnValueLoop");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDebugIgnoreFocusLoss)>("ConfigDebugIgnoreFocusLoss","ConfigDebugIgnoreFocusLoss");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDebugIniSettings)>("ConfigDebugIniSettings","ConfigDebugIniSettings");
		addField<DAS_BIND_MANAGED_FIELD(BackendPlatformName)>("BackendPlatformName","BackendPlatformName");
		addField<DAS_BIND_MANAGED_FIELD(BackendRendererName)>("BackendRendererName","BackendRendererName");
		addField<DAS_BIND_MANAGED_FIELD(BackendPlatformUserData)>("BackendPlatformUserData","BackendPlatformUserData");
		addField<DAS_BIND_MANAGED_FIELD(BackendRendererUserData)>("BackendRendererUserData","BackendRendererUserData");
		addField<DAS_BIND_MANAGED_FIELD(BackendLanguageUserData)>("BackendLanguageUserData","BackendLanguageUserData");
		addField<DAS_BIND_MANAGED_FIELD(ClipboardUserData)>("ClipboardUserData","ClipboardUserData");
		addField<DAS_BIND_MANAGED_FIELD(PlatformLocaleDecimalPoint)>("PlatformLocaleDecimalPoint","PlatformLocaleDecimalPoint");
		addField<DAS_BIND_MANAGED_FIELD(WantCaptureMouse)>("WantCaptureMouse","WantCaptureMouse");
		addField<DAS_BIND_MANAGED_FIELD(WantCaptureKeyboard)>("WantCaptureKeyboard","WantCaptureKeyboard");
		addField<DAS_BIND_MANAGED_FIELD(WantTextInput)>("WantTextInput","WantTextInput");
		addField<DAS_BIND_MANAGED_FIELD(WantSetMousePos)>("WantSetMousePos","WantSetMousePos");
		addField<DAS_BIND_MANAGED_FIELD(WantSaveIniSettings)>("WantSaveIniSettings","WantSaveIniSettings");
		addField<DAS_BIND_MANAGED_FIELD(NavActive)>("NavActive","NavActive");
		addField<DAS_BIND_MANAGED_FIELD(NavVisible)>("NavVisible","NavVisible");
		addField<DAS_BIND_MANAGED_FIELD(Framerate)>("Framerate","Framerate");
		addField<DAS_BIND_MANAGED_FIELD(MetricsRenderVertices)>("MetricsRenderVertices","MetricsRenderVertices");
		addField<DAS_BIND_MANAGED_FIELD(MetricsRenderIndices)>("MetricsRenderIndices","MetricsRenderIndices");
		addField<DAS_BIND_MANAGED_FIELD(MetricsRenderWindows)>("MetricsRenderWindows","MetricsRenderWindows");
		addField<DAS_BIND_MANAGED_FIELD(MetricsActiveWindows)>("MetricsActiveWindows","MetricsActiveWindows");
		addField<DAS_BIND_MANAGED_FIELD(MouseDelta)>("MouseDelta","MouseDelta");
		addField<DAS_BIND_MANAGED_FIELD(Ctx)>("Ctx","Ctx");
		addField<DAS_BIND_MANAGED_FIELD(MousePos)>("MousePos","MousePos");
		addField<DAS_BIND_MANAGED_FIELD(MouseDown)>("MouseDown","MouseDown");
		addField<DAS_BIND_MANAGED_FIELD(MouseWheel)>("MouseWheel","MouseWheel");
		addField<DAS_BIND_MANAGED_FIELD(MouseWheelH)>("MouseWheelH","MouseWheelH");
		addField<DAS_BIND_MANAGED_FIELD(MouseSource)>("MouseSource","MouseSource");
		addField<DAS_BIND_MANAGED_FIELD(MouseHoveredViewport)>("MouseHoveredViewport","MouseHoveredViewport");
		addField<DAS_BIND_MANAGED_FIELD(KeyCtrl)>("KeyCtrl","KeyCtrl");
		addField<DAS_BIND_MANAGED_FIELD(KeyShift)>("KeyShift","KeyShift");
		addField<DAS_BIND_MANAGED_FIELD(KeyAlt)>("KeyAlt","KeyAlt");
		addField<DAS_BIND_MANAGED_FIELD(KeySuper)>("KeySuper","KeySuper");
		addField<DAS_BIND_MANAGED_FIELD(KeyMods)>("KeyMods","KeyMods");
		addField<DAS_BIND_MANAGED_FIELD(KeysData)>("KeysData","KeysData");
		addField<DAS_BIND_MANAGED_FIELD(WantCaptureMouseUnlessPopupClose)>("WantCaptureMouseUnlessPopupClose","WantCaptureMouseUnlessPopupClose");
		addField<DAS_BIND_MANAGED_FIELD(MousePosPrev)>("MousePosPrev","MousePosPrev");
		addField<DAS_BIND_MANAGED_FIELD(MouseClickedPos)>("MouseClickedPos","MouseClickedPos");
		addField<DAS_BIND_MANAGED_FIELD(MouseClickedTime)>("MouseClickedTime","MouseClickedTime");
		addField<DAS_BIND_MANAGED_FIELD(MouseClicked)>("MouseClicked","MouseClicked");
		addField<DAS_BIND_MANAGED_FIELD(MouseDoubleClicked)>("MouseDoubleClicked","MouseDoubleClicked");
		addField<DAS_BIND_MANAGED_FIELD(MouseClickedCount)>("MouseClickedCount","MouseClickedCount");
		addField<DAS_BIND_MANAGED_FIELD(MouseClickedLastCount)>("MouseClickedLastCount","MouseClickedLastCount");
		addField<DAS_BIND_MANAGED_FIELD(MouseReleased)>("MouseReleased","MouseReleased");
		addField<DAS_BIND_MANAGED_FIELD(MouseDownOwned)>("MouseDownOwned","MouseDownOwned");
		addField<DAS_BIND_MANAGED_FIELD(MouseDownOwnedUnlessPopupClose)>("MouseDownOwnedUnlessPopupClose","MouseDownOwnedUnlessPopupClose");
		addField<DAS_BIND_MANAGED_FIELD(MouseWheelRequestAxisSwap)>("MouseWheelRequestAxisSwap","MouseWheelRequestAxisSwap");
		addField<DAS_BIND_MANAGED_FIELD(MouseDownDuration)>("MouseDownDuration","MouseDownDuration");
		addField<DAS_BIND_MANAGED_FIELD(MouseDownDurationPrev)>("MouseDownDurationPrev","MouseDownDurationPrev");
		addField<DAS_BIND_MANAGED_FIELD(MouseDragMaxDistanceAbs)>("MouseDragMaxDistanceAbs","MouseDragMaxDistanceAbs");
		addField<DAS_BIND_MANAGED_FIELD(MouseDragMaxDistanceSqr)>("MouseDragMaxDistanceSqr","MouseDragMaxDistanceSqr");
		addField<DAS_BIND_MANAGED_FIELD(PenPressure)>("PenPressure","PenPressure");
		addField<DAS_BIND_MANAGED_FIELD(AppFocusLost)>("AppFocusLost","AppFocusLost");
		addField<DAS_BIND_MANAGED_FIELD(AppAcceptingEvents)>("AppAcceptingEvents","AppAcceptingEvents");
		addField<DAS_BIND_MANAGED_FIELD(BackendUsingLegacyKeyArrays)>("BackendUsingLegacyKeyArrays","BackendUsingLegacyKeyArrays");
		addField<DAS_BIND_MANAGED_FIELD(BackendUsingLegacyNavInputArray)>("BackendUsingLegacyNavInputArray","BackendUsingLegacyNavInputArray");
		addField<DAS_BIND_MANAGED_FIELD(InputQueueSurrogate)>("InputQueueSurrogate","InputQueueSurrogate");
		addField<DAS_BIND_MANAGED_FIELD(InputQueueCharacters)>("InputQueueCharacters","InputQueueCharacters");
	}
};

// from imgui.h:2374:8
struct ImGuiInputTextCallbackData_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiInputTextCallbackData> {
	ImGuiInputTextCallbackData_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiInputTextCallbackData", ml, "ImGuiInputTextCallbackData") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Ctx)>("Ctx","Ctx");
		addField<ImGuiInputTextFlags_,offsetof(ManagedType,EventFlag)>("EventFlag","EventFlag")
			.adjustAot("das_reinterpret<ImGuiInputTextFlags_>::pass(",")");
		addField<ImGuiInputTextFlags_,offsetof(ManagedType,Flags)>("Flags","Flags")
			.adjustAot("das_reinterpret<ImGuiInputTextFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(UserData)>("UserData","UserData");
		addField<DAS_BIND_MANAGED_FIELD(EventChar)>("EventChar","EventChar");
		addField<DAS_BIND_MANAGED_FIELD(EventKey)>("EventKey","EventKey");
		addField<DAS_BIND_MANAGED_FIELD(Buf)>("Buf","Buf");
		addField<DAS_BIND_MANAGED_FIELD(BufTextLen)>("BufTextLen","BufTextLen");
		addField<DAS_BIND_MANAGED_FIELD(BufSize)>("BufSize","BufSize");
		addField<DAS_BIND_MANAGED_FIELD(BufDirty)>("BufDirty","BufDirty");
		addField<DAS_BIND_MANAGED_FIELD(CursorPos)>("CursorPos","CursorPos");
		addField<DAS_BIND_MANAGED_FIELD(SelectionStart)>("SelectionStart","SelectionStart");
		addField<DAS_BIND_MANAGED_FIELD(SelectionEnd)>("SelectionEnd","SelectionEnd");
	}
};

// from imgui.h:2408:8
struct ImGuiSizeCallbackData_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiSizeCallbackData> {
	ImGuiSizeCallbackData_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiSizeCallbackData", ml, "ImGuiSizeCallbackData") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(UserData)>("UserData","UserData");
		addField<DAS_BIND_MANAGED_FIELD(Pos)>("Pos","Pos");
		addField<DAS_BIND_MANAGED_FIELD(CurrentSize)>("CurrentSize","CurrentSize");
		addField<DAS_BIND_MANAGED_FIELD(DesiredSize)>("DesiredSize","DesiredSize");
	}
};

// from imgui.h:2423:8
struct ImGuiWindowClass_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiWindowClass> {
	ImGuiWindowClass_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiWindowClass", ml, "ImGuiWindowClass") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ClassId)>("ClassId","ClassId");
		addField<DAS_BIND_MANAGED_FIELD(ParentViewportId)>("ParentViewportId","ParentViewportId");
		addField<DAS_BIND_MANAGED_FIELD(FocusRouteParentWindowId)>("FocusRouteParentWindowId","FocusRouteParentWindowId");
		addField<ImGuiViewportFlags_,offsetof(ManagedType,ViewportFlagsOverrideSet)>("ViewportFlagsOverrideSet","ViewportFlagsOverrideSet")
			.adjustAot("das_reinterpret<ImGuiViewportFlags_>::pass(",")");
		addField<ImGuiViewportFlags_,offsetof(ManagedType,ViewportFlagsOverrideClear)>("ViewportFlagsOverrideClear","ViewportFlagsOverrideClear")
			.adjustAot("das_reinterpret<ImGuiViewportFlags_>::pass(",")");
		addField<ImGuiTabItemFlags_,offsetof(ManagedType,TabItemFlagsOverrideSet)>("TabItemFlagsOverrideSet","TabItemFlagsOverrideSet")
			.adjustAot("das_reinterpret<ImGuiTabItemFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(DockNodeFlagsOverrideSet)>("DockNodeFlagsOverrideSet","DockNodeFlagsOverrideSet");
		addField<DAS_BIND_MANAGED_FIELD(DockingAlwaysTabBar)>("DockingAlwaysTabBar","DockingAlwaysTabBar");
		addField<DAS_BIND_MANAGED_FIELD(DockingAllowUnclassed)>("DockingAllowUnclassed","DockingAllowUnclassed");
	}
};

// from imgui.h:2439:8
struct ImGuiPayload_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiPayload> {
	ImGuiPayload_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiPayload", ml, "ImGuiPayload") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Data)>("Data","Data");
		addField<DAS_BIND_MANAGED_FIELD(DataSize)>("DataSize","DataSize");
		addField<DAS_BIND_MANAGED_FIELD(SourceId)>("SourceId","SourceId");
		addField<DAS_BIND_MANAGED_FIELD(SourceParentId)>("SourceParentId","SourceParentId");
		addField<DAS_BIND_MANAGED_FIELD(DataFrameCount)>("DataFrameCount","DataFrameCount");
		addField<DAS_BIND_MANAGED_FIELD(DataType)>("DataType","DataType");
		addField<DAS_BIND_MANAGED_FIELD(Preview)>("Preview","Preview");
		addField<DAS_BIND_MANAGED_FIELD(Delivery)>("Delivery","Delivery");
	}
};

// from imgui.h:2474:8
struct ImGuiOnceUponAFrame_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiOnceUponAFrame> {
	ImGuiOnceUponAFrame_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiOnceUponAFrame", ml, "ImGuiOnceUponAFrame") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(RefFrame)>("RefFrame","RefFrame");
	}
};

// from imgui.h:2492:12
struct ImGuiTextFilter_ImGuiTextRange_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTextFilter::ImGuiTextRange> {
	ImGuiTextFilter_ImGuiTextRange_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiTextRange", ml, "ImGuiTextFilter::ImGuiTextRange") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(b)>("b","b");
		addField<DAS_BIND_MANAGED_FIELD(e)>("e","e");
	}
};

// from imgui.h:2482:8
struct ImGuiTextFilter_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTextFilter> {
	ImGuiTextFilter_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiTextFilter", ml, "ImGuiTextFilter") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(InputBuf)>("InputBuf","InputBuf");
		addField<DAS_BIND_MANAGED_FIELD(Filters)>("Filters","Filters");
		addField<DAS_BIND_MANAGED_FIELD(CountGrep)>("CountGrep","CountGrep");
	}
};

// from imgui.h:2509:8
struct ImGuiTextBuffer_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTextBuffer> {
	ImGuiTextBuffer_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiTextBuffer", ml, "ImGuiTextBuffer") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Buf)>("Buf","Buf");
	}
};

// from imgui.h:2539:12
struct ImGuiStorage_ImGuiStoragePair_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiStorage::ImGuiStoragePair> {
	ImGuiStorage_ImGuiStoragePair_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiStoragePair", ml, "ImGuiStorage::ImGuiStoragePair") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(key)>("key","key");
	}
};

// from imgui.h:2536:8
struct ImGuiStorage_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiStorage> {
	ImGuiStorage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiStorage", ml, "ImGuiStorage") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Data)>("Data","Data");
	}
};

// from imgui.h:2598:8
struct ImGuiListClipper_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiListClipper> {
	ImGuiListClipper_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiListClipper", ml, "ImGuiListClipper") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Ctx)>("Ctx","Ctx");
		addField<DAS_BIND_MANAGED_FIELD(DisplayStart)>("DisplayStart","DisplayStart");
		addField<DAS_BIND_MANAGED_FIELD(DisplayEnd)>("DisplayEnd","DisplayEnd");
		addField<DAS_BIND_MANAGED_FIELD(ItemsCount)>("ItemsCount","ItemsCount");
		addField<DAS_BIND_MANAGED_FIELD(ItemsHeight)>("ItemsHeight","ItemsHeight");
		addField<DAS_BIND_MANAGED_FIELD(StartPosY)>("StartPosY","StartPosY");
		addField<DAS_BIND_MANAGED_FIELD(TempData)>("TempData","TempData");
	}
};

// from imgui.h:2733:8
struct ImDrawCmd_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawCmd> {
	ImDrawCmd_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawCmd", ml, "ImDrawCmd") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ClipRect)>("ClipRect","ClipRect");
		addField<DAS_BIND_MANAGED_FIELD(TextureId)>("TextureId","TextureId");
		addField<DAS_BIND_MANAGED_FIELD(VtxOffset)>("VtxOffset","VtxOffset");
		addField<DAS_BIND_MANAGED_FIELD(IdxOffset)>("IdxOffset","IdxOffset");
		addField<DAS_BIND_MANAGED_FIELD(ElemCount)>("ElemCount","ElemCount");
		addField<DAS_BIND_MANAGED_FIELD(UserCallbackData)>("UserCallbackData","UserCallbackData");
	}
};

// from imgui.h:2751:8
struct ImDrawVert_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawVert> {
	ImDrawVert_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawVert", ml, "ImDrawVert") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pos)>("pos","pos");
		addField<DAS_BIND_MANAGED_FIELD(uv)>("uv","uv");
		addField<DAS_BIND_MANAGED_FIELD(col)>("col","col");
	}
};

// from imgui.h:2766:8
struct ImDrawCmdHeader_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawCmdHeader> {
	ImDrawCmdHeader_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawCmdHeader", ml, "ImDrawCmdHeader") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ClipRect)>("ClipRect","ClipRect");
		addField<DAS_BIND_MANAGED_FIELD(TextureId)>("TextureId","TextureId");
		addField<DAS_BIND_MANAGED_FIELD(VtxOffset)>("VtxOffset","VtxOffset");
	}
};

// from imgui.h:2774:8
struct ImDrawChannel_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawChannel> {
	ImDrawChannel_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawChannel", ml, "ImDrawChannel") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(_CmdBuffer)>("_CmdBuffer","_CmdBuffer");
		addField<DAS_BIND_MANAGED_FIELD(_IdxBuffer)>("_IdxBuffer","_IdxBuffer");
	}
};

// from imgui.h:2783:8
struct ImDrawListSplitter_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawListSplitter> {
	ImDrawListSplitter_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawListSplitter", ml, "ImDrawListSplitter") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(_Current)>("_Current","_Current");
		addField<DAS_BIND_MANAGED_FIELD(_Count)>("_Count","_Count");
		addField<DAS_BIND_MANAGED_FIELD(_Channels)>("_Channels","_Channels");
	}
};

// from imgui.h:2838:8
struct ImDrawList_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawList> {
	ImDrawList_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawList", ml, "ImDrawList") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(CmdBuffer)>("CmdBuffer","CmdBuffer");
		addField<DAS_BIND_MANAGED_FIELD(IdxBuffer)>("IdxBuffer","IdxBuffer");
		addField<DAS_BIND_MANAGED_FIELD(VtxBuffer)>("VtxBuffer","VtxBuffer");
		addField<ImDrawListFlags_,offsetof(ManagedType,Flags)>("Flags","Flags")
			.adjustAot("das_reinterpret<ImDrawListFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(_VtxCurrentIdx)>("_VtxCurrentIdx","_VtxCurrentIdx");
		addField<DAS_BIND_MANAGED_FIELD(_Data)>("_Data","_Data");
		addField<DAS_BIND_MANAGED_FIELD(_VtxWritePtr)>("_VtxWritePtr","_VtxWritePtr");
		addField<DAS_BIND_MANAGED_FIELD(_IdxWritePtr)>("_IdxWritePtr","_IdxWritePtr");
		addField<DAS_BIND_MANAGED_FIELD(_Path)>("_Path","_Path");
		addField<DAS_BIND_MANAGED_FIELD(_CmdHeader)>("_CmdHeader","_CmdHeader");
		addField<DAS_BIND_MANAGED_FIELD(_Splitter)>("_Splitter","_Splitter");
		addField<DAS_BIND_MANAGED_FIELD(_ClipRectStack)>("_ClipRectStack","_ClipRectStack");
		addField<DAS_BIND_MANAGED_FIELD(_TextureIdStack)>("_TextureIdStack","_TextureIdStack");
		addField<DAS_BIND_MANAGED_FIELD(_FringeScale)>("_FringeScale","_FringeScale");
		addField<DAS_BIND_MANAGED_FIELD(_OwnerName)>("_OwnerName","_OwnerName");
	}
};

// from imgui.h:2978:8
struct ImDrawData_GeneratedAnnotation : ManagedStructureAnnotation<ImDrawData> {
	ImDrawData_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImDrawData", ml, "ImDrawData") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Valid)>("Valid","Valid");
		addField<DAS_BIND_MANAGED_FIELD(CmdListsCount)>("CmdListsCount","CmdListsCount");
		addField<DAS_BIND_MANAGED_FIELD(TotalIdxCount)>("TotalIdxCount","TotalIdxCount");
		addField<DAS_BIND_MANAGED_FIELD(TotalVtxCount)>("TotalVtxCount","TotalVtxCount");
		addField<DAS_BIND_MANAGED_FIELD(CmdLists)>("CmdLists","CmdLists");
		addField<DAS_BIND_MANAGED_FIELD(DisplayPos)>("DisplayPos","DisplayPos");
		addField<DAS_BIND_MANAGED_FIELD(DisplaySize)>("DisplaySize","DisplaySize");
		addField<DAS_BIND_MANAGED_FIELD(FramebufferScale)>("FramebufferScale","FramebufferScale");
		addField<DAS_BIND_MANAGED_FIELD(OwnerViewport)>("OwnerViewport","OwnerViewport");
	}
};

// from imgui.h:3002:8
struct ImFontConfig_GeneratedAnnotation : ManagedStructureAnnotation<ImFontConfig> {
	ImFontConfig_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFontConfig", ml, "ImFontConfig") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(FontData)>("FontData","FontData");
		addField<DAS_BIND_MANAGED_FIELD(FontDataSize)>("FontDataSize","FontDataSize");
		addField<DAS_BIND_MANAGED_FIELD(FontDataOwnedByAtlas)>("FontDataOwnedByAtlas","FontDataOwnedByAtlas");
		addField<DAS_BIND_MANAGED_FIELD(FontNo)>("FontNo","FontNo");
		addField<DAS_BIND_MANAGED_FIELD(SizePixels)>("SizePixels","SizePixels");
		addField<DAS_BIND_MANAGED_FIELD(OversampleH)>("OversampleH","OversampleH");
		addField<DAS_BIND_MANAGED_FIELD(OversampleV)>("OversampleV","OversampleV");
		addField<DAS_BIND_MANAGED_FIELD(PixelSnapH)>("PixelSnapH","PixelSnapH");
		addField<DAS_BIND_MANAGED_FIELD(GlyphExtraSpacing)>("GlyphExtraSpacing","GlyphExtraSpacing");
		addField<DAS_BIND_MANAGED_FIELD(GlyphOffset)>("GlyphOffset","GlyphOffset");
		addField<DAS_BIND_MANAGED_FIELD(GlyphRanges)>("GlyphRanges","GlyphRanges");
		addField<DAS_BIND_MANAGED_FIELD(GlyphMinAdvanceX)>("GlyphMinAdvanceX","GlyphMinAdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(GlyphMaxAdvanceX)>("GlyphMaxAdvanceX","GlyphMaxAdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(MergeMode)>("MergeMode","MergeMode");
		addField<DAS_BIND_MANAGED_FIELD(FontBuilderFlags)>("FontBuilderFlags","FontBuilderFlags");
		addField<DAS_BIND_MANAGED_FIELD(RasterizerMultiply)>("RasterizerMultiply","RasterizerMultiply");
		addField<DAS_BIND_MANAGED_FIELD(RasterizerDensity)>("RasterizerDensity","RasterizerDensity");
		addField<DAS_BIND_MANAGED_FIELD(EllipsisChar)>("EllipsisChar","EllipsisChar");
		addField<DAS_BIND_MANAGED_FIELD(Name)>("Name","Name");
		addField<DAS_BIND_MANAGED_FIELD(DstFont)>("DstFont","DstFont");
	}
};

// from imgui.h:3032:8
struct ImFontGlyph_GeneratedAnnotation : ManagedStructureAnnotation<ImFontGlyph> {
	ImFontGlyph_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFontGlyph", ml, "ImFontGlyph") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(AdvanceX)>("AdvanceX","AdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(X0)>("X0","X0");
		addField<DAS_BIND_MANAGED_FIELD(Y0)>("Y0","Y0");
		addField<DAS_BIND_MANAGED_FIELD(X1)>("X1","X1");
		addField<DAS_BIND_MANAGED_FIELD(Y1)>("Y1","Y1");
		addField<DAS_BIND_MANAGED_FIELD(U0)>("U0","U0");
		addField<DAS_BIND_MANAGED_FIELD(V0)>("V0","V0");
		addField<DAS_BIND_MANAGED_FIELD(U1)>("U1","U1");
		addField<DAS_BIND_MANAGED_FIELD(V1)>("V1","V1");
	}
};

// from imgui.h:3044:8
struct ImFontGlyphRangesBuilder_GeneratedAnnotation : ManagedStructureAnnotation<ImFontGlyphRangesBuilder> {
	ImFontGlyphRangesBuilder_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFontGlyphRangesBuilder", ml, "ImFontGlyphRangesBuilder") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(UsedChars)>("UsedChars","UsedChars");
	}
};

// from imgui.h:3059:8
struct ImFontAtlasCustomRect_GeneratedAnnotation : ManagedStructureAnnotation<ImFontAtlasCustomRect> {
	ImFontAtlasCustomRect_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFontAtlasCustomRect", ml, "ImFontAtlasCustomRect") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Width)>("Width","Width");
		addField<DAS_BIND_MANAGED_FIELD(Height)>("Height","Height");
		addField<DAS_BIND_MANAGED_FIELD(X)>("X","X");
		addField<DAS_BIND_MANAGED_FIELD(Y)>("Y","Y");
		addField<DAS_BIND_MANAGED_FIELD(GlyphID)>("GlyphID","GlyphID");
		addField<DAS_BIND_MANAGED_FIELD(GlyphAdvanceX)>("GlyphAdvanceX","GlyphAdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(GlyphOffset)>("GlyphOffset","GlyphOffset");
		addField<DAS_BIND_MANAGED_FIELD(Font)>("Font","Font");
	}
};

// from imgui.h:3097:8
struct ImFontAtlas_GeneratedAnnotation : ManagedStructureAnnotation<ImFontAtlas> {
	ImFontAtlas_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFontAtlas", ml, "ImFontAtlas") {
	}
	void init () {
		addField<ImFontAtlasFlags_,offsetof(ManagedType,Flags)>("Flags","Flags")
			.adjustAot("das_reinterpret<ImFontAtlasFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(TexID)>("TexID","TexID");
		addField<DAS_BIND_MANAGED_FIELD(TexDesiredWidth)>("TexDesiredWidth","TexDesiredWidth");
		addField<DAS_BIND_MANAGED_FIELD(TexGlyphPadding)>("TexGlyphPadding","TexGlyphPadding");
		addField<DAS_BIND_MANAGED_FIELD(Locked)>("Locked","Locked");
		addField<DAS_BIND_MANAGED_FIELD(UserData)>("UserData","UserData");
		addField<DAS_BIND_MANAGED_FIELD(TexReady)>("TexReady","TexReady");
		addField<DAS_BIND_MANAGED_FIELD(TexPixelsUseColors)>("TexPixelsUseColors","TexPixelsUseColors");
		addField<DAS_BIND_MANAGED_FIELD(TexPixelsAlpha8)>("TexPixelsAlpha8","TexPixelsAlpha8");
		addField<DAS_BIND_MANAGED_FIELD(TexPixelsRGBA32)>("TexPixelsRGBA32","TexPixelsRGBA32");
		addField<DAS_BIND_MANAGED_FIELD(TexWidth)>("TexWidth","TexWidth");
		addField<DAS_BIND_MANAGED_FIELD(TexHeight)>("TexHeight","TexHeight");
		addField<DAS_BIND_MANAGED_FIELD(TexUvScale)>("TexUvScale","TexUvScale");
		addField<DAS_BIND_MANAGED_FIELD(TexUvWhitePixel)>("TexUvWhitePixel","TexUvWhitePixel");
		addField<DAS_BIND_MANAGED_FIELD(Fonts)>("Fonts","Fonts");
		addField<DAS_BIND_MANAGED_FIELD(CustomRects)>("CustomRects","CustomRects");
		addField<DAS_BIND_MANAGED_FIELD(ConfigData)>("ConfigData","ConfigData");
		addField<DAS_BIND_MANAGED_FIELD(TexUvLines)>("TexUvLines","TexUvLines");
		addField<DAS_BIND_MANAGED_FIELD(FontBuilderIO)>("FontBuilderIO","FontBuilderIO");
		addField<DAS_BIND_MANAGED_FIELD(FontBuilderFlags)>("FontBuilderFlags","FontBuilderFlags");
		addField<DAS_BIND_MANAGED_FIELD(PackIdMouseCursors)>("PackIdMouseCursors","PackIdMouseCursors");
		addField<DAS_BIND_MANAGED_FIELD(PackIdLines)>("PackIdLines","PackIdLines");
	}
};

// from imgui.h:3201:8
struct ImFont_GeneratedAnnotation : ManagedStructureAnnotation<ImFont> {
	ImFont_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImFont", ml, "ImFont") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(IndexAdvanceX)>("IndexAdvanceX","IndexAdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(FallbackAdvanceX)>("FallbackAdvanceX","FallbackAdvanceX");
		addField<DAS_BIND_MANAGED_FIELD(FontSize)>("FontSize","FontSize");
		addField<DAS_BIND_MANAGED_FIELD(IndexLookup)>("IndexLookup","IndexLookup");
		addField<DAS_BIND_MANAGED_FIELD(Glyphs)>("Glyphs","Glyphs");
		addField<DAS_BIND_MANAGED_FIELD(FallbackGlyph)>("FallbackGlyph","FallbackGlyph");
		addField<DAS_BIND_MANAGED_FIELD(ContainerAtlas)>("ContainerAtlas","ContainerAtlas");
		addField<DAS_BIND_MANAGED_FIELD(ConfigData)>("ConfigData","ConfigData");
		addField<DAS_BIND_MANAGED_FIELD(ConfigDataCount)>("ConfigDataCount","ConfigDataCount");
		addField<DAS_BIND_MANAGED_FIELD(FallbackChar)>("FallbackChar","FallbackChar");
		addField<DAS_BIND_MANAGED_FIELD(EllipsisChar)>("EllipsisChar","EllipsisChar");
		addField<DAS_BIND_MANAGED_FIELD(EllipsisCharCount)>("EllipsisCharCount","EllipsisCharCount");
		addField<DAS_BIND_MANAGED_FIELD(EllipsisWidth)>("EllipsisWidth","EllipsisWidth");
		addField<DAS_BIND_MANAGED_FIELD(EllipsisCharStep)>("EllipsisCharStep","EllipsisCharStep");
		addField<DAS_BIND_MANAGED_FIELD(DirtyLookupTables)>("DirtyLookupTables","DirtyLookupTables");
		addField<DAS_BIND_MANAGED_FIELD(Scale)>("Scale","Scale");
		addField<DAS_BIND_MANAGED_FIELD(Ascent)>("Ascent","Ascent");
		addField<DAS_BIND_MANAGED_FIELD(Descent)>("Descent","Descent");
		addField<DAS_BIND_MANAGED_FIELD(MetricsTotalSurface)>("MetricsTotalSurface","MetricsTotalSurface");
		addField<DAS_BIND_MANAGED_FIELD(Used4kPagesMap)>("Used4kPagesMap","Used4kPagesMap");
	}
};

// from imgui.h:3287:8
struct ImGuiViewport_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiViewport> {
	ImGuiViewport_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiViewport", ml, "ImGuiViewport") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ID)>("ID","ID");
		addField<ImGuiViewportFlags_,offsetof(ManagedType,Flags)>("Flags","Flags")
			.adjustAot("das_reinterpret<ImGuiViewportFlags_>::pass(",")");
		addField<DAS_BIND_MANAGED_FIELD(Pos)>("Pos","Pos");
		addField<DAS_BIND_MANAGED_FIELD(Size)>("Size","Size");
		addField<DAS_BIND_MANAGED_FIELD(WorkPos)>("WorkPos","WorkPos");
		addField<DAS_BIND_MANAGED_FIELD(WorkSize)>("WorkSize","WorkSize");
		addField<DAS_BIND_MANAGED_FIELD(DpiScale)>("DpiScale","DpiScale");
		addField<DAS_BIND_MANAGED_FIELD(ParentViewportId)>("ParentViewportId","ParentViewportId");
		addField<DAS_BIND_MANAGED_FIELD(DrawData)>("DrawData","DrawData");
		addField<DAS_BIND_MANAGED_FIELD(RendererUserData)>("RendererUserData","RendererUserData");
		addField<DAS_BIND_MANAGED_FIELD(PlatformUserData)>("PlatformUserData","PlatformUserData");
		addField<DAS_BIND_MANAGED_FIELD(PlatformHandle)>("PlatformHandle","PlatformHandle");
		addField<DAS_BIND_MANAGED_FIELD(PlatformHandleRaw)>("PlatformHandleRaw","PlatformHandleRaw");
		addField<DAS_BIND_MANAGED_FIELD(PlatformWindowCreated)>("PlatformWindowCreated","PlatformWindowCreated");
		addField<DAS_BIND_MANAGED_FIELD(PlatformRequestMove)>("PlatformRequestMove","PlatformRequestMove");
		addField<DAS_BIND_MANAGED_FIELD(PlatformRequestResize)>("PlatformRequestResize","PlatformRequestResize");
		addField<DAS_BIND_MANAGED_FIELD(PlatformRequestClose)>("PlatformRequestClose","PlatformRequestClose");
	}
};

// from imgui.h:3371:8
struct ImGuiPlatformIO_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiPlatformIO> {
	ImGuiPlatformIO_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiPlatformIO", ml, "ImGuiPlatformIO") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Monitors)>("Monitors","Monitors");
		addField<DAS_BIND_MANAGED_FIELD(Viewports)>("Viewports","Viewports");
	}
};

// from imgui.h:3434:8
struct ImGuiPlatformMonitor_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiPlatformMonitor> {
	ImGuiPlatformMonitor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiPlatformMonitor", ml, "ImGuiPlatformMonitor") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(MainPos)>("MainPos","MainPos");
		addField<DAS_BIND_MANAGED_FIELD(MainSize)>("MainSize","MainSize");
		addField<DAS_BIND_MANAGED_FIELD(WorkPos)>("WorkPos","WorkPos");
		addField<DAS_BIND_MANAGED_FIELD(WorkSize)>("WorkSize","WorkSize");
		addField<DAS_BIND_MANAGED_FIELD(DpiScale)>("DpiScale","DpiScale");
		addField<DAS_BIND_MANAGED_FIELD(PlatformHandle)>("PlatformHandle","PlatformHandle");
	}
};

// from imgui.h:3444:8
struct ImGuiPlatformImeData_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiPlatformImeData> {
	ImGuiPlatformImeData_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("ImGuiPlatformImeData", ml, "ImGuiPlatformImeData") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(WantVisible)>("WantVisible","WantVisible");
		addField<DAS_BIND_MANAGED_FIELD(InputPos)>("InputPos","InputPos");
		addField<DAS_BIND_MANAGED_FIELD(InputLineHeight)>("InputLineHeight","InputLineHeight");
	}
};

