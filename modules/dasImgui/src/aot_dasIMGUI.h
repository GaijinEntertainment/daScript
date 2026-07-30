#pragma once

namespace das {

    template <> struct das_alias<ImVec2>  : das_alias_vec<ImVec2,float2> {};
    template <> struct das_alias<ImVec4>  : das_alias_vec<ImVec4,float4> {};
    template <> struct das_alias<ImColor> : das_alias_vec<ImColor,float4> {};
    template <> struct das_alias<ImRect>  : das_alias_vec<ImRect,float4> {};

    template <typename TT>
    struct das_index<ImVector<TT>> : das_default_vector_index<ImVector<TT>, TT> {};
    template <typename TT>
    struct das_index<ImVector<TT> const> : das_default_vector_index<ImVector<TT>, TT> {};

    template <typename TT>
    struct das_default_vector_size<ImVector<TT>> {
        static __forceinline uint32_t size( const ImVector<TT> & value ) {
            return uint32_t(value.size());
        }
    };

    DAS_MOD_API void Text ( const char * txt );
    DAS_MOD_API void LabelText ( const char * lab, const char * txt );
    DAS_MOD_API void TextWrapped ( const char * txt );
    DAS_MOD_API void TextDisabled ( const char * txt );
    DAS_MOD_API void TextColored ( const ImVec4 & col, const char * txt );
    DAS_MOD_API void LogText ( const char * txt );
    DAS_MOD_API bool TreeNode ( const char * id, const char * txt );
    DAS_MOD_API bool TreeNodeEx ( const char * id, ImGuiTreeNodeFlags_ flags, const char * txt );
    DAS_MOD_API bool TreeNodeEx2 ( const void * id, ImGuiTreeNodeFlags_ flags, const char * txt );
    DAS_MOD_API void TextUnformatted ( const char * txt );
    DAS_MOD_API void BulletText ( const char * txt );
    DAS_MOD_API void SetTooltip ( const char * txt );
    DAS_MOD_API bool InputTextMultiline(vec4f vdiit, const char* label, const ImVec2& size, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context );
    DAS_MOD_API bool InputText(vec4f vdiit, const char * label, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context );
    DAS_MOD_API bool InputTextWithHint(vec4f vdiit, const char * label, const char * hint, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context );
    DAS_MOD_API bool PassFilter ( ImGuiTextFilter & filter, const char* text );
    using MarkdownEventBlock = TBlock<void,
        int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t,
        const char *, const char *>;
    DAS_MOD_API int32_t MarkdownParseEvents(const char * source, int32_t source_size,
        int32_t flags, const MarkdownEventBlock & block, Context * context, LineInfoArg * at);
    DAS_MOD_API char* text_range_string(ImGuiTextFilter::ImGuiTextRange &r, das::Context *context, das::LineInfoArg *at);
    DAS_MOD_API void AddText( ImDrawList & drawList, const ImVec2& pos, ImU32 col, const char* text );
    DAS_MOD_API void InstallCoreClipboardBackend();
    DAS_MOD_API ImVec2 CalcTextSizeForFont(ImFont * font, float font_size, const char * text);
    DAS_MOD_API ImVec2 CalcTextSizeForFontRange(ImFont * font, float font_size, const char * text,
        int32_t text_size, int32_t start_byte, int32_t end_byte);
    DAS_MOD_API void AddTextRange(ImDrawList & drawList, ImFont* font, float font_size,
        const ImVec2& pos, ImU32 col, const char* text, int32_t text_size,
        int32_t start_byte, int32_t end_byte);
    DAS_MOD_API void AddText2( ImDrawList & drawList, ImFont* font, float font_size, const ImVec2& pos, ImU32 col,
        const char* text_begin, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = nullptr);
    // image API — const ImTextureRef& wrappers (const-arg binding).
    DAS_MOD_API void Image_das( const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1 );
    DAS_MOD_API void ImageWithBg_das( const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1, const ImVec4 & bg_col, const ImVec4 & tint_col );
    DAS_MOD_API bool ImageButton_das( const char* str_id, const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1, const ImVec4 & bg_col, const ImVec4 & tint_col );
    // imgui_internal.h Render*Clipped / Ellipsis text helpers — text_end is forced to
    // nullptr (a daslang string can't express the NULL these need for "auto-strlen";
    // a non-null "" over-reads past the buffer → crash). text_size_if_known is forced
    // to nullptr (let ImGui compute it — identical result, it is only a perf hint).
    DAS_MOD_API void RenderTextClippedW( const ImVec2& pos_min, const ImVec2& pos_max, const char* text,
        const ImVec2& align, const ImRect* clip_rect );
    DAS_MOD_API void RenderTextClippedExW( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max,
        const char* text, const ImVec2& align, const ImRect* clip_rect );
    DAS_MOD_API void RenderTextEllipsisW( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max,
        float ellipsis_max_x, const char* text );
    // imgui_internal.h custom-widget primitives — ItemAdd (two overloads) + ButtonBehavior.
    DAS_MOD_API bool ItemAddW( const ImRect& bb, ImGuiID id, ImGuiItemFlags extra_flags = 0 );
    DAS_MOD_API bool ItemAddNavW( const ImRect& bb, ImGuiID id, const ImRect& nav_bb, ImGuiItemFlags extra_flags = 0 );
    DAS_MOD_API bool ButtonBehaviorW( const ImRect& bb, ImGuiID id, bool& out_hovered, bool& out_held, ImGuiButtonFlags_ flags );
    DAS_MOD_API bool ScrollbarExW( const ImRect& bb, ImGuiID id, ImGuiAxis axis, ImS64& scroll_v,
        ImS64 avail_v, ImS64 contents_v, ImDrawFlags_ flags );
    DAS_MOD_API bool SplitterBehaviorW( const ImRect& bb, ImGuiID id, ImGuiAxis axis, float& size1, float& size2,
        float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f, ImU32 bg_col = 0 );
    DAS_MOD_API bool TreeNodeBehaviorW( ImGuiID id, ImGuiTreeNodeFlags_ flags, const char* label );
    DAS_MOD_API void TextExW( const char* text, ImGuiTextFlags flags = 0 );
    DAS_MOD_API void SeparatorTextExW( ImGuiID id, const char* label, float extra_width = 0.0f );
    DAS_MOD_API void LogRenderedTextW( const char* text );
    DAS_MOD_API ImColor HSV(float h, float s, float v, float a = 1.0f);
    DAS_MOD_API void ImGTB_Append ( ImGuiTextBuffer & buf, const char * txt );
    DAS_MOD_API int ImGTB_At ( ImGuiTextBuffer & buf, int32_t index );
    DAS_MOD_API void ImGTB_SetAt ( ImGuiTextBuffer & buf, int32_t index, int32_t value );
    DAS_MOD_API char * ImGTB_Slice ( ImGuiTextBuffer & buf, int32_t head, int32_t tail, Context * context, LineInfoArg * at );
    DAS_MOD_API void InsertChars(ImGuiInputTextCallbackData & data, int pos, const char* text );
    DAS_MOD_API void SetNextWindowSizeConstraints(vec4f snwscc, const ImVec2& size_min, const ImVec2& size_max, Context * context, LineInfoArg * at );
    DAS_MOD_API void SetNextWindowSizeConstraintsNoCallback(const ImVec2& size_min, const ImVec2& size_max);
    DAS_MOD_API ImGuiSortDirection GetColumnSortDirection ( const ImGuiTableColumnSortSpecs * specs );
    DAS_MOD_API const ImGuiTableColumnSortSpecs * GetSortSpec ( ImGuiTableSortSpecs * specs, int idx );
    DAS_MOD_API ImVec2 CalcTextSize(const char* text,bool hide_text_after_double_hash, float wrap_width);
    DAS_MOD_API bool Combo ( vec4f cg, const char * label, int * current_item, int items_count, int popup_max_height_in_items, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void PlotLines ( vec4f igpg, const char* label, int values_count, int values_offset, const char* overlay_text,
        float scale_min, float scale_max, ImVec2 graph_size, Context * ctx, LineInfoArg * at );
    DAS_MOD_API void PlotHistogram ( vec4f igpg, const char* label, int values_count, int values_offset, const char* overlay_text,
        float scale_min, float scale_max, ImVec2 graph_size, Context * ctx, LineInfoArg * at );
}
