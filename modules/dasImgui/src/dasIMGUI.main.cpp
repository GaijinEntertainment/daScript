#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "aot_builtin_clipboard.h"
#include "dasIMGUI.h"
#include "need_dasIMGUI.h"
#include "aot_dasIMGUI.h"

// Cherry-picked from imgui_internal.h per master plan §3 — the public-API
// stance allows internal symbols when "a real use case bumps into them."
// Phase 2: io.active_widget needs ImGui::GetActiveID().
// Phase 7: DockBuilder API needed for programmatic initial dock layouts in
// the boost dockspace/dock_window wrappers (widgets/imgui_docking_builtin.das).
// No other internal symbols added without re-discussion.
#include "imgui_internal.h"

static_assert(sizeof(ImWchar) == 4, "dasImgui requires 32-bit ImWchar for full Unicode support");

namespace das {

    static const char * CoreClipboardGetText(ImGuiContext *) {
        int32_t status = int32_t(ClipboardStatus::failed);
        return builtin_clipboard_get_text_temporary(status);
    }

    static void CoreClipboardSetText(ImGuiContext *, const char * text) {
        const char * value = text ? text : "";
        builtin_clipboard_set_text(value, int32_t(strlen(value)));
    }

    void InstallCoreClipboardBackend() {
        ImGuiPlatformIO & platformIO = ImGui::GetPlatformIO();
        platformIO.Platform_GetClipboardTextFn = CoreClipboardGetText;
        platformIO.Platform_SetClipboardTextFn = CoreClipboardSetText;
        platformIO.Platform_ClipboardUserData = nullptr;
    }

    ImU32 GetActiveID() {
        return ImGui::GetActiveID();
    }

    // Phase 7 docking — DockBuilder is internal API; bind a thin C++ surface so
    // boost wrappers in widgets/imgui_docking_builtin.das can seed default
    // layouts. ImGui::DockBuilderSplitNode takes ImGuiID* out params; daslang
    // expresses these as ImGuiID& references for natural call sites:
    //   var left, right : uint
    //   DockBuilderSplitNode(parent, ImGuiDir.Left, 0.25f, left, right)
    ImGuiID DockBuilderAddNode_das(ImGuiID node_id, ImGuiDockNodeFlags_ flags) {
        return ImGui::DockBuilderAddNode(node_id, (ImGuiDockNodeFlags)flags);
    }
    void DockBuilderRemoveNode_das(ImGuiID node_id) {
        ImGui::DockBuilderRemoveNode(node_id);
    }
    void DockBuilderRemoveNodeDockedWindows_das(ImGuiID node_id, bool clear_settings_refs) {
        ImGui::DockBuilderRemoveNodeDockedWindows(node_id, clear_settings_refs);
    }
    void DockBuilderRemoveNodeChildNodes_das(ImGuiID node_id) {
        ImGui::DockBuilderRemoveNodeChildNodes(node_id);
    }
    void DockBuilderSetNodeSize_das(ImGuiID node_id, const ImVec2 & size) {
        ImGui::DockBuilderSetNodeSize(node_id, size);
    }
    ImGuiID DockBuilderSplitNode_das(ImGuiID parent_id, ImGuiDir split_dir, float ratio,
                                     ImGuiID & out_at_dir, ImGuiID & out_at_opp) {
        ImGuiID a = 0, b = 0;
        ImGuiID r = ImGui::DockBuilderSplitNode(parent_id, split_dir, ratio, &a, &b);
        out_at_dir = a;
        out_at_opp = b;
        return r;
    }
    void DockBuilderDockWindow_das(const char * window_name, ImGuiID node_id) {
        ImGui::DockBuilderDockWindow(window_name ? window_name : "", node_id);
    }
    void DockBuilderFinish_das(ImGuiID node_id) {
        ImGui::DockBuilderFinish(node_id);
    }
    // Existence-check shim — full ImGuiDockNode* handle binding deferred to v2.
    bool DockBuilderHasNode(ImGuiID node_id) {
        return ImGui::DockBuilderGetNode(node_id) != nullptr;
    }
    // ImGuiDockNodeFlags_DockSpace is internal-only; required when seeding a
    // dockspace root via DockBuilder. Exposed as a dedicated forwarder so
    // daslang code doesn't have to hard-code the bit value.
    ImGuiID DockBuilderAddDockSpaceNode(ImGuiID node_id, ImGuiDockNodeFlags_ flags) {
        return ImGui::DockBuilderAddNode(node_id, (ImGuiDockNodeFlags)flags | ImGuiDockNodeFlags_DockSpace);
    }

    void Text ( const char * txt ) {
        ImGui::Text("%s",txt);
    }
    void LabelText ( const char * lab, const char * txt ) {
        ImGui::LabelText(lab,"%s",txt ? txt : "");
    }
    void TextWrapped ( const char * txt ) {
        ImGui::TextWrapped("%s",txt ? txt : "");
    }
    void TextDisabled ( const char * txt ) {
        ImGui::TextDisabled("%s",txt ? txt : "");
    }
    void TextColored ( const ImVec4 & col, const char * txt ) {
        ImGui::TextColored(col,"%s",txt ? txt : "");
    }
    void LogText ( const char * txt ) {
        ImGui::LogText("%s",txt ? txt : "");
    }
    bool TreeNode ( const char * id, const char * txt ) {
        return ImGui::TreeNode(id,"%s",txt ? txt : "");
    }
    bool TreeNodeEx ( const char * id, ImGuiTreeNodeFlags_ flags, const char * txt ) {
        return ImGui::TreeNodeEx(id,flags,"%s",txt ? txt : "");
    }
    bool TreeNodeEx2 ( const void * id, ImGuiTreeNodeFlags_ flags, const char * txt ) {
        return ImGui::TreeNodeEx(id,flags,"%s",txt ? txt : "");
    }
    void TextUnformatted ( const char * txt ) {
        ImGui::TextUnformatted(txt ? txt : "", nullptr);
    }
    void BulletText ( const char * txt ) {
        ImGui::BulletText("%s",txt ? txt : "");
    }
    void SetTooltip ( const char * txt ) {
        ImGui::SetTooltip("%s",txt ? txt : "");
    }

    struct DasImguiInputText {
        Context *  context;
        TLambda<void,DasImguiInputText *,ImGuiInputTextCallbackData *>    callback;
        TArray<uint8_t> buffer;
        LineInfo *      at;
    };

    int InputTextCallback (ImGuiInputTextCallbackData* data) {
        auto diit = (DasImguiInputText *) data->UserData;
        DAS_VERIFY(diit->context && "context is always specified");
        if ( !diit->callback.capture ) {
            diit->context->throw_error("ImguiTextCallback: missing capture");
        }
        return das_invoke_lambda<int>::invoke<DasImguiInputText *,ImGuiInputTextCallbackData *>(diit->context, diit->at, diit->callback, diit, data);
    }

    bool InputTextMultiline(vec4f vdiit, const char* label, const ImVec2& size, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context ) {
        auto diit = cast<DasImguiInputText *>::to(vdiit);
        if ( diit->buffer.size==0 ) {
            builtin_array_resize(diit->buffer, 256, 1, context, at);
        }
        if ( diit->callback.capture ) {
            diit->context = context;
            diit->at = at;
            return ImGui::InputTextMultiline(
                label,
                diit->buffer.data,
                diit->buffer.size,
                size,
                flags,
                &InputTextCallback,
                diit
            );
        } else {
            return ImGui::InputTextMultiline(label, diit->buffer.data, diit->buffer.size, size, flags);
        }
    }

    bool InputText(vec4f vdiit, const char * label, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context ) {
        auto diit = cast<DasImguiInputText *>::to(vdiit);
        if ( diit->buffer.size==0 ) {
            builtin_array_resize(diit->buffer, 256, 1, context, at);
        }
        if ( diit->callback.capture ) {
            diit->context = context;
            diit->at = at;
            return ImGui::InputText(
                label,
                diit->buffer.data,
                diit->buffer.size,
                flags,
                &InputTextCallback,
                diit
            );
        } else {
            return ImGui::InputText(label, diit->buffer.data, diit->buffer.size, flags);
        }
    }

    bool InputTextWithHint(vec4f vdiit, const char * label, const char * hint, ImGuiInputTextFlags_ flags, LineInfoArg * at, Context * context ) {
        auto diit = cast<DasImguiInputText *>::to(vdiit);
        if ( diit->buffer.size==0 ) {
            builtin_array_resize(diit->buffer, 256, 1, context, at);
        }
        if ( diit->callback.capture ) {
            diit->context = context;
            diit->at = at;
            return ImGui::InputTextWithHint(
                label,
                hint,
                diit->buffer.data,
                diit->buffer.size,
                flags,
                &InputTextCallback,
                diit
            );
        } else {
            return ImGui::InputTextWithHint(label, hint, diit->buffer.data, diit->buffer.size, flags);
        }
    }

    // ===== Phase 0b.4 — buffer-as-pointer InputText path =====
    // The legacy DasImguiInputText path above stays as the daslib/imgui_boost.das
    // v1 surface. The widgets in imgui_widgets_builtin.das instead own
    // `state.buffer : array<uint8>` and pass `addr(buffer[0])` directly. The
    // _basic forwarders are 1-line overload pickers; the _cb forwarders
    // stack-allocate a thunk holding (Context*, LineInfo*, Lambda) for the
    // duration of the ImGui call. ImGui's InputText/Combo callbacks fire
    // synchronously inside that call, so the thunk's lifetime is one C frame —
    // no ABI pinning, no struct field, no per-widget mirror struct.

    bool InputTextBasic ( uint8_t * buf, int buf_size, const char * label, ImGuiInputTextFlags_ flags ) {
        return ImGui::InputText(label, (char *)buf, buf_size, flags);
    }
    bool InputTextWithHintBasic ( uint8_t * buf, int buf_size, const char * label, const char * hint, ImGuiInputTextFlags_ flags ) {
        return ImGui::InputTextWithHint(label, hint, (char *)buf, buf_size, flags);
    }
    bool InputTextMultilineBasic ( uint8_t * buf, int buf_size, const char * label, const ImVec2 & size, ImGuiInputTextFlags_ flags ) {
        return ImGui::InputTextMultiline(label, (char *)buf, buf_size, size, flags);
    }

    struct InputTextLambdaThunk {
        Context *  context;
        LineInfo * at;
        Lambda     lambda;
    };

    int InputTextLambdaTrampoline ( ImGuiInputTextCallbackData * data ) {
        auto t = (InputTextLambdaThunk *) data->UserData;
        if ( !t->lambda.capture ) {
            t->context->throw_error_at(t->at, "InputText callback: lambda has no capture");
        }
        return das_invoke_lambda<int>::invoke<ImGuiInputTextCallbackData *>(
            t->context, t->at, t->lambda, data);
    }

    bool InputTextCb ( uint8_t * buf, int buf_size, const char * label, ImGuiInputTextFlags_ flags,
                       Lambda cb, Context * context, LineInfoArg * at ) {
        InputTextLambdaThunk thunk { context, at, cb };
        return ImGui::InputText(label, (char *)buf, buf_size, flags,
                                &InputTextLambdaTrampoline, &thunk);
    }
    bool InputTextWithHintCb ( uint8_t * buf, int buf_size, const char * label, const char * hint, ImGuiInputTextFlags_ flags,
                               Lambda cb, Context * context, LineInfoArg * at ) {
        InputTextLambdaThunk thunk { context, at, cb };
        return ImGui::InputTextWithHint(label, hint, (char *)buf, buf_size, flags,
                                        &InputTextLambdaTrampoline, &thunk);
    }
    bool InputTextMultilineCb ( uint8_t * buf, int buf_size, const char * label, const ImVec2 & size, ImGuiInputTextFlags_ flags,
                                Lambda cb, Context * context, LineInfoArg * at ) {
        InputTextLambdaThunk thunk { context, at, cb };
        return ImGui::InputTextMultiline(label, (char *)buf, buf_size, size, flags,
                                         &InputTextLambdaTrampoline, &thunk);
    }

    struct ComboGetterLambdaThunk {
        Context *  context;
        LineInfo * at;
        Lambda     lambda;
    };

    const char * ComboGetterLambdaTrampoline ( void * data, int idx ) {
        auto t = (ComboGetterLambdaThunk *) data;
        if ( !t->lambda.capture ) {
            t->context->throw_error_at(t->at, "Combo getter: lambda has no capture");
        }
        char * out = nullptr;
        das_invoke_lambda<bool>::invoke<int, char **>(
            t->context, t->at, t->lambda, idx, (char **)&out);
        return out ? out : "";
    }

    bool ComboCb ( int * current_item, const char * label, int items_count, int popup_max_height_in_items,
                   Lambda cb, Context * context, LineInfoArg * at ) {
        ComboGetterLambdaThunk thunk { context, at, cb };
        return ImGui::Combo(label, current_item, &ComboGetterLambdaTrampoline, &thunk,
                            items_count, popup_max_height_in_items);
    }

    // Plot getters — same single-call thunk pattern as Combo. Lambda is
    // invoked synchronously by PlotLines/PlotHistogram during the C call,
    // so the thunk lives on the stack frame of the forwarder. No long-
    // lived registration; lambda lifetime is bounded by the C call.
    struct PlotGetterLambdaThunk {
        Context *  context;
        LineInfo * at;
        Lambda     lambda;
    };

    float PlotGetterLambdaTrampoline ( void * data, int idx ) {
        auto t = (PlotGetterLambdaThunk *) data;
        if ( !t->lambda.capture ) {
            t->context->throw_error_at(t->at, "Plot getter: lambda has no capture");
        }
        return das_invoke_lambda<float>::invoke<int>(t->context, t->at, t->lambda, idx);
    }

    void PlotLinesCb ( const char * label, int values_count, int values_offset,
                       const char * overlay_text, float scale_min, float scale_max,
                       ImVec2 graph_size, Lambda cb, Context * context, LineInfoArg * at ) {
        PlotGetterLambdaThunk thunk { context, at, cb };
        ImGui::PlotLines(label, &PlotGetterLambdaTrampoline, &thunk,
                         values_count, values_offset,
                         overlay_text, scale_min, scale_max, graph_size);
    }

    void PlotHistogramCb ( const char * label, int values_count, int values_offset,
                           const char * overlay_text, float scale_min, float scale_max,
                           ImVec2 graph_size, Lambda cb, Context * context, LineInfoArg * at ) {
        PlotGetterLambdaThunk thunk { context, at, cb };
        ImGui::PlotHistogram(label, &PlotGetterLambdaTrampoline, &thunk,
                             values_count, values_offset,
                             overlay_text, scale_min, scale_max, graph_size);
    }

    // ImGui::ImGuiTextFilter::PassFilter

    bool PassFilter ( ImGuiTextFilter & filter, const char* text ) {
        return filter.PassFilter(text, nullptr);
    }

    char * text_range_string( ImGuiTextFilter::ImGuiTextRange & r, das::Context *context, das::LineInfoArg * at ) {
        return context->allocateString(r.b, r.e - r.b, at);
    }

    void AddText( ImDrawList & drawList, const ImVec2& pos, ImU32 col, const char* text ) {
        drawList.AddText(pos, col, text);
    }

    ImVec2 CalcTextSizeForFont(ImFont * font, float font_size, const char * text) {
        return font ? font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text ? text : "")
                    : ImVec2();
    }

    static void ClampTextRange(const char * text, int32_t text_size,
                               int32_t & start_byte, int32_t & end_byte,
                               const char *& begin, const char *& end) {
        const char * value = text ? text : "";
        const int32_t size = text ? (text_size < 0 ? 0 : text_size) : 0;
        start_byte = start_byte < 0 ? 0 : (start_byte > size ? size : start_byte);
        end_byte = end_byte < start_byte ? start_byte : (end_byte > size ? size : end_byte);
        begin = value + start_byte;
        end = value + end_byte;
    }

    ImVec2 CalcTextSizeForFontRange(ImFont * font, float font_size, const char * text,
                                     int32_t text_size, int32_t start_byte, int32_t end_byte) {
        if (!font) return ImVec2();
        const char * begin = nullptr;
        const char * end = nullptr;
        ClampTextRange(text, text_size, start_byte, end_byte, begin, end);
        return font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, begin, end);
    }

    void AddTextRange(ImDrawList & drawList, ImFont* font, float font_size,
                      const ImVec2& pos, ImU32 col, const char* text,
                      int32_t text_size, int32_t start_byte, int32_t end_byte) {
        const char * begin = nullptr;
        const char * end = nullptr;
        ClampTextRange(text, text_size, start_byte, end_byte, begin, end);
        drawList.AddText(font, font_size, pos, col, begin, end);
    }

    void AddText2( ImDrawList & drawList, ImFont* font, float font_size, const ImVec2& pos, ImU32 col,
        const char* text_begin, float wrap_width, const ImVec4* cpu_fine_clip_rect) {
        drawList.AddText(font,font_size,pos,col,text_begin,nullptr,wrap_width,cpu_fine_clip_rect);
    }

    // Image/ImageButton/ImageWithBg take ImTextureRef BY VALUE. These wrappers
    // take `const ImTextureRef&` so the daslang arg binds as const (like ImVec2),
    // letting a const ImTextureRef (GetIO().Fonts.TexRef) pass through.
    void Image_das( const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1 ) {
        ImGui::Image(tex_ref, image_size, uv0, uv1);
    }
    void ImageWithBg_das( const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1, const ImVec4 & bg_col, const ImVec4 & tint_col ) {
        ImGui::ImageWithBg(tex_ref, image_size, uv0, uv1, bg_col, tint_col);
    }
    bool ImageButton_das( const char* str_id, const ImTextureRef & tex_ref, const ImVec2 & image_size, const ImVec2 & uv0, const ImVec2 & uv1, const ImVec4 & bg_col, const ImVec4 & tint_col ) {
        return ImGui::ImageButton(str_id, tex_ref, image_size, uv0, uv1, bg_col, tint_col);
    }

    // imgui_internal.h Render*Clipped / Ellipsis — see aot_dasIMGUI.h for why
    // text_end / text_size_if_known are pinned to nullptr.
    void RenderTextClippedW( const ImVec2& pos_min, const ImVec2& pos_max, const char* text,
        const ImVec2& align, const ImRect* clip_rect ) {
        ImGui::RenderTextClipped(pos_min, pos_max, text, nullptr, nullptr, align, clip_rect);
    }

    void RenderTextClippedExW( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max,
        const char* text, const ImVec2& align, const ImRect* clip_rect ) {
        ImGui::RenderTextClippedEx(draw_list, pos_min, pos_max, text, nullptr, nullptr, align, clip_rect);
    }

    void RenderTextEllipsisW( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max,
        float ellipsis_max_x, const char* text ) {
        // RenderTextEllipsis takes no separate clip_max_x arg.
        ImGui::RenderTextEllipsis(draw_list, pos_min, pos_max, ellipsis_max_x, text, nullptr, nullptr);
    }

    // imgui_internal.h ItemAdd — two overloads so the optional separate nav rect
    // stays a clean by-value arg instead of a nullable pointer at the call site.
    bool ItemAddW( const ImRect& bb, ImGuiID id, ImGuiItemFlags extra_flags ) {
        return ImGui::ItemAdd(bb, id, nullptr, extra_flags);
    }

    bool ItemAddNavW( const ImRect& bb, ImGuiID id, const ImRect& nav_bb, ImGuiItemFlags extra_flags ) {
        return ImGui::ItemAdd(bb, id, &nav_bb, extra_flags);
    }

    // imgui_internal.h ButtonBehavior — out_hovered / out_held exposed as bool&
    // (das passes plain vars; write-back works, registered SideEffects::worstDefault
    // below since the call also touches ImGui global hover / active-id state).
    bool ButtonBehaviorW( const ImRect& bb, ImGuiID id, bool& out_hovered, bool& out_held, ImGuiButtonFlags_ flags ) {
        return ImGui::ButtonBehavior(bb, id, &out_hovered, &out_held, flags);
    }

    // imgui_internal.h ScrollbarEx — low-level scrollbar over an explicit rect.
    // p_scroll_v is an in/out scroll position; exposed as ImS64& (das var passed in,
    // updated on drag). avail_v / contents_v are the visible / total content extents.
    bool ScrollbarExW( const ImRect& bb, ImGuiID id, ImGuiAxis axis, ImS64& scroll_v,
        ImS64 avail_v, ImS64 contents_v, ImDrawFlags_ flags ) {
        return ImGui::ScrollbarEx(bb, id, axis, &scroll_v, avail_v, contents_v, flags);
    }

    // imgui_internal.h SplitterBehavior — draggable splitter bar. size1 / size2 are
    // the two pane sizes (in/out floats, exposed as float&): on drag the bar moves
    // and both are updated, clamped to min_size1 / min_size2.
    bool SplitterBehaviorW( const ImRect& bb, ImGuiID id, ImGuiAxis axis, float& size1, float& size2,
        float min_size1, float min_size2, float hover_extend, float hover_visibility_delay, ImU32 bg_col ) {
        return ImGui::SplitterBehavior(bb, id, axis, &size1, &size2, min_size1, min_size2,
            hover_extend, hover_visibility_delay, bg_col);
    }

    // imgui_internal.h TreeNodeBehavior — the tree-node open/closed state machine.
    // label_end is pinned to nullptr ("whole string"); a daslang string can't pass
    // the NULL these need (same class as the RenderText*W text helpers above).
    bool TreeNodeBehaviorW( ImGuiID id, ImGuiTreeNodeFlags_ flags, const char* label ) {
        return ImGui::TreeNodeBehavior(id, flags, label, nullptr);
    }

    // imgui_internal.h TextEx / SeparatorTextEx — both carry a nullable text_end /
    // label_end (a daslang string can't express the NULL), pinned to nullptr here.
    // ImGuiTextFlags is internal -> binds as int (cast int(ImGuiTextFlags.X)).
    void TextExW( const char* text, ImGuiTextFlags flags ) {
        ImGui::TextEx(text, nullptr, flags);
    }

    void SeparatorTextExW( ImGuiID id, const char* label, float extra_width ) {
        ImGui::SeparatorTextEx(id, label, nullptr, extra_width);
    }

    // imgui_internal.h LogRenderedText — inject text into the active log (the primitive
    // widgets call to capture their own rendered text). ref_pos is pinned to nullptr:
    // its only effect is position-driven newline insertion keyed off a widget's screen
    // Y (the internal widget path); from das, embed '\n' in `text` for line breaks — the
    // splitter turns it into real newlines + tree indentation. text_end -> nullptr like
    // TextEx (a daslang string can't express the NULL).
    void LogRenderedTextW( const char* text ) {
        ImGui::LogRenderedText(nullptr, text, nullptr);
    }

    // ImColor

    ImColor HSV(float h, float s, float v, float a) {
        return ImColor::HSV(h,s,v,a);
    }

    // ImGuiTextBuffer

    void ImGTB_Append ( ImGuiTextBuffer & buf, const char * txt ) {
        buf.append(txt, nullptr);
    }

    int ImGTB_At ( ImGuiTextBuffer & buf, int32_t index ) {
        return buf[index];
    }

    void ImGTB_SetAt ( ImGuiTextBuffer & buf, int32_t index, int32_t value ) {
        buf.Buf[index] = (char) value;
    }

    char * ImGTB_Slice ( ImGuiTextBuffer & buf, int32_t head, int32_t tail, Context * context, LineInfoArg * at ) {
        if ( head>tail ) {
            context->throw_error_at(at, "can't get slice of ImGuiTextBuffer, head > tail");
        }
        int32_t len = tail - head;
        if ( len>buf.size() ) {
            context->throw_error_at(at, "can't get slice of ImGuiTextBuffer, slice too big");
        }
        return context->allocateString(buf.begin() + head,len+1,at);
    }

    // ImGuiInputTextCallbackData

    void InsertChars(ImGuiInputTextCallbackData & data, int pos, const char* text ) {
        data.InsertChars(pos, text);
    }

    // SetNextWindowSizeConstraints

    struct DasImGuiSizeConstraints {
        Context *   context;
        Lambda      lambda;
        LineInfo *  at;
    };

    void SetNextWindowSizeConstraintsCallback ( ImGuiSizeCallbackData* data ) {
        DasImGuiSizeConstraints * temp = (DasImGuiSizeConstraints *) data->UserData;
        if ( !temp->lambda.capture ) {
            temp->context->throw_error_at(temp->at, "expecting lambda");
        }
        das_invoke_lambda<void>::invoke<ImGuiSizeCallbackData*>(temp->context,temp->at,temp->lambda,data);
    }

    void SetNextWindowSizeConstraints ( vec4f snwscc, const ImVec2& size_min, const ImVec2& size_max, Context * context, LineInfoArg * at ) {
        DasImGuiSizeConstraints * temp = cast<DasImGuiSizeConstraints *>::to(snwscc);
        temp->context = context;
        temp->at = at;
        ImGui::SetNextWindowSizeConstraints(size_min, size_max, &SetNextWindowSizeConstraintsCallback, temp);
    }

    void SetNextWindowSizeConstraintsNoCallback ( const ImVec2& size_min, const ImVec2& size_max ) {
        ImGui::SetNextWindowSizeConstraints(size_min, size_max);
    }

    void DisableIniPersistence () {
        // Null IniFilename so ImGui skips loading/saving window geometry from
        // imgui.ini. Call after CreateContext and BEFORE the first NewFrame:
        // ImGui loads the ini on the first NewFrame, so a later call only stops
        // future saves — it won't undo an already-loaded layout. Tutorial/demo
        // apps call it in init() to start in their documented layout every run.
        // The bound IniFilename field is `const char*` (read-only from daslang),
        // so this C++ helper is the only way to clear it.
        ImGui::GetIO().IniFilename = nullptr;
    }

    ImGuiSortDirection GetColumnSortDirection ( const ImGuiTableColumnSortSpecs * specs ) {
        // Takes pointer (not reference) for daslang interop consistency with `GetSortSpec`.
        // The bitfield-stored `SortDirection` is exposed as this free helper because
        // `ImU8 SortDirection : 8` doesn't survive the generated struct-annotation binding.
        // Renamed from bare `SortDirection` to avoid `decltype(&das::SortDirection)` ambiguity
        // with the struct member `ImGuiTableColumnSortSpecs::SortDirection` under MSVC.
        return ImGuiSortDirection(specs->SortDirection);
    }

    const ImGuiTableColumnSortSpecs * GetSortSpec ( ImGuiTableSortSpecs * specs, int idx ) {
        // Indexed access to ``ImGuiTableSortSpecs::Specs`` (a `const ImGuiTableColumnSortSpecs *`
        // array) — daslang has no native C-pointer-arithmetic, so this small helper exposes
        // per-index access via pointer. Returns pointer (not reference) because daslang's interop
        // WrapType layer only handles `T*` cleanly for non-workhorse return types; `const T&`
        // returns trigger "missing WrapType implementation" at runtime.
        // Caller responsibility: specs != NULL && 0 <= idx < specs->SpecsCount.
        return &specs->Specs[idx];
    }

    ImVec2 CalcTextSize(const char* text,bool hide_text_after_double_hash, float wrap_width) {
        return ImGui::CalcTextSize(text,nullptr,hide_text_after_double_hash,wrap_width);
    }

    // Combo with accessor
    struct ImGuiComboGetter {
        Context *   context;
        Lambda      lambda;
        LineInfo *  at;
    };

    const char *ComboGetterCallback(void* data, int idx) {
        ImGuiComboGetter * getter = (ImGuiComboGetter *) data;
        if ( !getter->lambda.capture ) {
            getter->context->throw_error_at(getter->at, "expecting lambda");
        }
        const char *out_text = nullptr;
        das_invoke_lambda<bool>::invoke<int,char **>(getter->context,getter->at,getter->lambda,idx,(char **)&out_text);
        if ( out_text==nullptr ) out_text = "";
        return out_text;
    }

    bool Combo ( vec4f cg, const char * label, int * current_item, int items_count, int popup_max_height_in_items, Context * ctx, LineInfoArg * at ) {
        ImGuiComboGetter * getter = cast<ImGuiComboGetter *>::to(cg);
        getter->context = ctx;
        getter->at = at;
        return ImGui::Combo(label,current_item,&ComboGetterCallback,getter,items_count,popup_max_height_in_items);
    }

    // Plot lines or historgrams.
    //
    // ABI-paired struct: the daslang-side `ImGuiPlotGetter` in
    // `daslib/imgui_boost.das` matches this layout byte-for-byte
    // (Context* / Lambda / LineInfo*). Callers stack-allocate the
    // daslang struct and pass its address as `vec4f igpg`; the wrappers
    // below reinterpret_cast back. This is the older pattern, preserved
    // for v1 compatibility (daslib/imgui_boost.das + example/imgui_demo.das).
    //
    // The new boost (Phase 1+) plot widgets in
    // widgets/imgui_widgets_builtin.das use the array form
    // (ImGui::PlotLines direct binding via dasIMGUI.func_11.cpp) and
    // don't touch this struct. A lambda-thunk form mirroring 0b.4's
    // ComboCb (see ComboGetterLambdaThunk above) is deferred to Phase 2
    // alongside the public boost API for transports / playwright.

    struct ImGuiPlotGetter {
        Context *   context;
        Lambda      lambda;
        LineInfo *  at;
    };

    float PlotLinesCallback ( void* data, int idx ) {
        ImGuiPlotGetter * getter = (ImGuiPlotGetter *) data;
        if ( !getter->lambda.capture ) {
            getter->context->throw_error_at(getter->at, "expecting lambda");
        }
        return  das_invoke_lambda<float>::invoke<int>(getter->context,getter->at,getter->lambda,idx);
    }

    void PlotLines ( vec4f igpg, const char* label, int values_count, int values_offset, const char* overlay_text,
        float scale_min, float scale_max, ImVec2 graph_size, Context * ctx, LineInfoArg * at ) {
        ImGuiPlotGetter * getter = cast<ImGuiPlotGetter *>::to(igpg);
        getter->context = ctx;
        getter->at = at;
        return ImGui::PlotLines(label, &PlotLinesCallback, getter, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size );
    }

    void PlotHistogram ( vec4f igpg, const char* label, int values_count, int values_offset, const char* overlay_text,
        float scale_min, float scale_max, ImVec2 graph_size, Context * ctx, LineInfoArg * at ) {
        ImGuiPlotGetter * getter = cast<ImGuiPlotGetter *>::to(igpg);
        getter->context = ctx;
        getter->at = at;
        return ImGui::PlotHistogram(label, &PlotLinesCallback, getter, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size );
    }

    void Module_dasIMGUI::initAotAlias () {
        addAlias(typeFactory<ImVec2>::make(lib));
        addAlias(typeFactory<ImVec4>::make(lib));
        addAlias(typeFactory<ImColor>::make(lib));
    }

	void Module_dasIMGUI::initMain () {
        addConstant(*this,"IMGUI_VERSION", IMGUI_VERSION);
        // MD4C SAX bridge. The typed document arena lives in markdown/
        // daScript code; native code only forwards parser events and exact
        // source offsets for text callbacks.
        addExtern<DAS_BIND_FUN(das::MarkdownParseEvents)>(*this, lib, "_markdown_parse_events",
            SideEffects::invoke, "das::MarkdownParseEvents")
                ->args({"source","source_size","flags","block","context","at"});
        // imgui text filter
        addExtern<DAS_BIND_FUN(das::PassFilter)>(*this, lib, "PassFilter",
            SideEffects::worstDefault, "das::PassFilter");
        addExtern<DAS_BIND_FUN(das::text_range_string)>(*this, lib, "string",
            SideEffects::worstDefault, "das::text_range_string");
        // imcolor
        addExtern<DAS_BIND_FUN(das::HSV)>(*this, lib, "HSV",
            SideEffects::none, "das::HSV")
                ->args({"h","s","v","a"})
                    ->arg_init(3,new ExprConstFloat(1.0f));
        // imgui draw list
        addExtern<DAS_BIND_FUN(das::AddText), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "AddText",
            SideEffects::worstDefault, "das::AddText");
        addExtern<DAS_BIND_FUN(das::InstallCoreClipboardBackend)>(*this, lib,
            "InstallCoreClipboardBackend", SideEffects::modifyExternal,
            "das::InstallCoreClipboardBackend");
        addExtern<DAS_BIND_FUN(das::CalcTextSizeForFont)>(*this, lib, "CalcTextSizeForFont",
            SideEffects::none, "das::CalcTextSizeForFont")
                ->args({"font","font_size","text"});
        addExtern<DAS_BIND_FUN(das::CalcTextSizeForFontRange)>(*this, lib, "CalcTextSizeForFontRange",
            SideEffects::none, "das::CalcTextSizeForFontRange")
                ->args({"font","font_size","text","text_size","start_byte","end_byte"});
        addExtern<DAS_BIND_FUN(das::AddTextRange), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "AddTextRange",
            SideEffects::worstDefault, "das::AddTextRange")
                ->args({"drawList","font","font_size","pos","col","text","text_size","start_byte","end_byte"});
        addExtern<DAS_BIND_FUN(das::AddText2), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "AddText",
            SideEffects::worstDefault, "das::AddText2")
                ->args({"drawList","font","font_size","pos","col","text","wrap_width","cpu_fine_clip_rect"})
                    ->arg_init(6,new ExprConstFloat(0.0f))
                    ->arg_init(7,new ExprConstPtr());
        // image API — const ImTextureRef& wrappers (see das:: defs above).
        addExtern<DAS_BIND_FUN(das::Image_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "Image",
            SideEffects::worstDefault, "das::Image_das")
                ->args({"tex_ref","image_size","uv0","uv1"});
        addExtern<DAS_BIND_FUN(das::ImageWithBg_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ImageWithBg",
            SideEffects::worstDefault, "das::ImageWithBg_das")
                ->args({"tex_ref","image_size","uv0","uv1","bg_col","tint_col"});
        addExtern<DAS_BIND_FUN(das::ImageButton_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ImageButton",
            SideEffects::worstDefault, "das::ImageButton_das")
                ->args({"str_id","tex_ref","image_size","uv0","uv1","bg_col","tint_col"});
        // imgui_internal.h Render*Clipped / Ellipsis text helpers (wrappers pin text_end
        // + text_size_if_known to nullptr — see das:: defs above). align defaults to
        // ImVec2(0,0), clip_rect to null, so the common call is just (pos_min,pos_max,text).
        addExtern<DAS_BIND_FUN(das::RenderTextClippedW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "RenderTextClipped",
            SideEffects::worstDefault, "das::RenderTextClippedW")
                ->args({"pos_min","pos_max","text","align","clip_rect"})
                    ->arg_init(3,new ExprCall(LineInfo(),"ImVec2"))
                    ->arg_init(4,new ExprConstPtr());
        addExtern<DAS_BIND_FUN(das::RenderTextClippedExW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "RenderTextClippedEx",
            SideEffects::worstDefault, "das::RenderTextClippedExW")
                ->args({"draw_list","pos_min","pos_max","text","align","clip_rect"})
                    ->arg_init(4,new ExprCall(LineInfo(),"ImVec2"))
                    ->arg_init(5,new ExprConstPtr());
        addExtern<DAS_BIND_FUN(das::RenderTextEllipsisW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "RenderTextEllipsis",
            SideEffects::worstDefault, "das::RenderTextEllipsisW")
                ->args({"draw_list","pos_min","pos_max","ellipsis_max_x","text"});
        // imgui_internal.h custom-widget primitives. ItemAdd is two overloads —
        // without / with an explicit nav rect. extra_flags defaults, so the das
        // calls are ItemAdd(bb,id[,flags]) and ItemAdd(bb,id,nav_bb[,flags]); at
        // three args the nav overload is picked by float4 nav_bb vs int flags.
        // ButtonBehavior returns its two state outputs through bool& args.
        // extra_flags is the internal ImGuiItemFlags (binds as int); flags is the
        // public ImGuiButtonFlags (das enum, combines via the imgui_enums.das `|` rail).
        addExtern<DAS_BIND_FUN(das::ItemAddW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ItemAdd",
            SideEffects::worstDefault, "das::ItemAddW")
                ->args({"bb","id","extra_flags"})
                    ->arg_init(2,new ExprConstInt(0));
        addExtern<DAS_BIND_FUN(das::ItemAddNavW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ItemAdd",
            SideEffects::worstDefault, "das::ItemAddNavW")
                ->args({"bb","id","nav_bb","extra_flags"})
                    ->arg_init(3,new ExprConstInt(0));
        addExtern<DAS_BIND_FUN(das::ButtonBehaviorW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ButtonBehavior",
            SideEffects::worstDefault, "das::ButtonBehaviorW")
                ->args({"bb","id","out_hovered","out_held","flags"});
        // imgui_internal.h scrollbar / splitter behaviors. Both take an ImGuiAxis +
        // ImRect (float4) and return their in/out numeric state through references —
        // ScrollbarEx's scroll pos as ImS64&, SplitterBehavior's two pane sizes as
        // float&. SplitterBehavior's hover_extend / hover_visibility_delay / bg_col
        // default, so the common call is (bb, id, axis, size1, size2, min1, min2).
        addExtern<DAS_BIND_FUN(das::ScrollbarExW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "ScrollbarEx",
            SideEffects::worstDefault, "das::ScrollbarExW")
                ->args({"bb","id","axis","scroll_v","avail_v","contents_v","flags"});
        addExtern<DAS_BIND_FUN(das::SplitterBehaviorW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "SplitterBehavior",
            SideEffects::worstDefault, "das::SplitterBehaviorW")
                ->args({"bb","id","axis","size1","size2","min_size1","min_size2",
                        "hover_extend","hover_visibility_delay","bg_col"})
                    ->arg_init(7,new ExprConstFloat(0.0f))
                    ->arg_init(8,new ExprConstFloat(0.0f))
                    ->arg_init(9,new ExprConstUInt(0));
        // imgui_internal.h TreeNodeBehavior — wrapper pins label_end to nullptr (a das
        // string can't express the NULL it needs). flags default ImGuiTreeNodeFlags.None.
        addExtern<DAS_BIND_FUN(das::TreeNodeBehaviorW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "TreeNodeBehavior",
            SideEffects::worstDefault, "das::TreeNodeBehaviorW")
                ->args({"id","flags","label"})
                    ->arg_init(1,new ExprConstEnumeration(0,makeType<ImGuiTreeNodeFlags_>(lib)));
        // imgui_internal.h TextEx / SeparatorTextEx — wrappers pin text_end / label_end
        // to nullptr. TextEx's flags is the internal ImGuiTextFlags (int), default 0.
        addExtern<DAS_BIND_FUN(das::TextExW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "TextEx",
            SideEffects::worstDefault, "das::TextExW")
                ->args({"text","flags"})
                    ->arg_init(1,new ExprConstInt(0));
        addExtern<DAS_BIND_FUN(das::SeparatorTextExW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "SeparatorTextEx",
            SideEffects::worstDefault, "das::SeparatorTextExW")
                ->args({"id","label","extra_width"})
                    ->arg_init(2,new ExprConstFloat(0.0f));
        // imgui_internal.h LogRenderedText — manual log injection; ref_pos / text_end
        // pinned to nullptr (embed '\n' in text for line breaks).
        addExtern<DAS_BIND_FUN(das::LogRenderedTextW), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "LogRenderedText",
            SideEffects::worstDefault, "das::LogRenderedTextW")
                ->args({"text"});
        // variadic functions
        addExtern<DAS_BIND_FUN(das::Text)>(*this,lib,"Text",
            SideEffects::worstDefault,"das::Text");
        addExtern<DAS_BIND_FUN(das::TextWrapped)>(*this,lib,"TextWrapped",
            SideEffects::worstDefault,"das::TextWrapped");
        addExtern<DAS_BIND_FUN(das::TextDisabled)>(*this,lib,"TextDisabled",
            SideEffects::worstDefault,"das::TextDisabled");
        addExtern<DAS_BIND_FUN(das::TextColored), SimNode_ExtFuncCall, imguiTempFn>(*this,lib,"TextColored",
            SideEffects::worstDefault,"das::TextColored");
        addExtern<DAS_BIND_FUN(das::LabelText)>(*this,lib,"LabelText",
            SideEffects::worstDefault,"das::LabelText");
        addExtern<DAS_BIND_FUN(das::LogText)>(*this,lib,"LogText",
            SideEffects::worstDefault,"das::LogText");
        addExtern<DAS_BIND_FUN(das::TreeNode)>(*this,lib,"TreeNode",
            SideEffects::worstDefault,"das::TreeNode");
        addExtern<DAS_BIND_FUN(das::TreeNodeEx)>(*this,lib,"TreeNodeEx",
            SideEffects::worstDefault,"das::TreeNodeEx");
        addExtern<DAS_BIND_FUN(das::TreeNodeEx2)>(*this,lib,"TreeNodeEx",
            SideEffects::worstDefault,"das::TreeNodeEx2");
        addExtern<DAS_BIND_FUN(das::BulletText)>(*this,lib,"BulletText",
            SideEffects::worstDefault,"das::BulletText");
        addExtern<DAS_BIND_FUN(das::SetTooltip)>(*this,lib,"SetTooltip",
            SideEffects::worstDefault,"das::SetTooltip");
        // text unfromatted
        addExtern<DAS_BIND_FUN(das::TextUnformatted)>(*this, lib, "TextUnformatted",
            SideEffects::worstDefault, "das::TextUnformatted")
            ->arg("text");
        // input text
        addExtern<DAS_BIND_FUN(das::InputText)>(*this, lib, "_builtin_InputText",
            SideEffects::worstDefault, "das::InputText");
        addExtern<DAS_BIND_FUN(das::InputTextWithHint)>(*this, lib, "_builtin_InputTextWithHint",
            SideEffects::worstDefault, "das::InputTextWithHint");
        addExtern<DAS_BIND_FUN(das::InputTextMultiline), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "_builtin_InputTextMultiline",
            SideEffects::worstDefault, "das::InputTextMultiline");
        // Phase 0b.4 — buffer-as-pointer InputText path (no DasImguiInputText mirror).
        addExtern<DAS_BIND_FUN(das::InputTextBasic)>(*this, lib, "_builtin_InputText_basic",
            SideEffects::worstDefault, "das::InputTextBasic");
        addExtern<DAS_BIND_FUN(das::InputTextWithHintBasic)>(*this, lib, "_builtin_InputTextWithHint_basic",
            SideEffects::worstDefault, "das::InputTextWithHintBasic");
        addExtern<DAS_BIND_FUN(das::InputTextMultilineBasic), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "_builtin_InputTextMultiline_basic",
            SideEffects::worstDefault, "das::InputTextMultilineBasic");
        addExtern<DAS_BIND_FUN(das::InputTextCb)>(*this, lib, "_builtin_InputText_cb",
            SideEffects::worstDefault, "das::InputTextCb");
        addExtern<DAS_BIND_FUN(das::InputTextWithHintCb)>(*this, lib, "_builtin_InputTextWithHint_cb",
            SideEffects::worstDefault, "das::InputTextWithHintCb");
        addExtern<DAS_BIND_FUN(das::InputTextMultilineCb), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "_builtin_InputTextMultiline_cb",
            SideEffects::worstDefault, "das::InputTextMultilineCb");
        // imgui text buffer
        addExtern<DAS_BIND_FUN(das::ImGTB_Append)>(*this,lib,"append",
            SideEffects::worstDefault,"das::ImGTB_Append");
        addExtern<DAS_BIND_FUN(das::ImGTB_At)>(*this,lib,"at",          // TODO: do we need to learn to map operator []?
            SideEffects::worstDefault,"das::ImGTB_At");
        addExtern<DAS_BIND_FUN(das::ImGTB_SetAt)>(*this,lib,"set_at",   // TODO: do we need to learn to map operator []?
            SideEffects::worstDefault,"das::ImGTB_SetAt");
        addExtern<DAS_BIND_FUN(das::ImGTB_Slice)>(*this,lib,"slice",
            SideEffects::worstDefault,"das::ImGTB_Slice");
        // ImGuiInputTextCallbackData
        addExtern<DAS_BIND_FUN(das::InsertChars)>(*this,lib,"InsertChars",
            SideEffects::worstDefault,"das::InsertChars");
        // SetNextWindowSizeConstraints
        addExtern<DAS_BIND_FUN(das::SetNextWindowSizeConstraints), SimNode_ExtFuncCall, imguiTempFn>(*this,lib,"_builtin_SetNextWindowSizeConstraints",
            SideEffects::worstDefault,"das::SetNextWindowSizeConstraints");
        addExtern<DAS_BIND_FUN(das::SetNextWindowSizeConstraintsNoCallback), SimNode_ExtFuncCall, imguiTempFn>(*this,lib,"SetNextWindowSizeConstraints",
            SideEffects::worstDefault,"das::SetNextWindowSizeConstraintsNoCallback")
                ->args({"size_min","size_max"});
        // Disable imgui.ini persistence — tutorial/demo apps start fresh each run.
        addExtern<DAS_BIND_FUN(das::DisableIniPersistence)>(*this,lib,"DisableIniPersistence",
            SideEffects::worstDefault,"das::DisableIniPersistence");
        // ImGuiTableColumnSortSpecs / ImGuiTableSortSpecs
        addExtern<DAS_BIND_FUN(das::GetColumnSortDirection)>(*this,lib,"GetColumnSortDirection",
            SideEffects::none,"das::GetColumnSortDirection");
        addExtern<DAS_BIND_FUN(das::GetSortSpec)>(*this,lib,"GetSortSpec",
            SideEffects::none,"das::GetSortSpec");
        // CalcTextSize
        addExtern<DAS_BIND_FUN(das::CalcTextSize)>(*this, lib, "CalcTextSize",SideEffects::worstDefault, "das::CalcTextSize")
        ->args({"text","hide_text_after_double_hash","wrap_width"})
            ->arg_init(1,new ExprConstBool(false))
            ->arg_init(2,new ExprConstFloat(-1.0f));
        // combo
        addExtern<DAS_BIND_FUN(das::Combo)>(*this, lib, "_builtin_Combo",
            SideEffects::worstDefault, "das::Combo");
        // Phase 0b.4 — Combo with per-call lambda getter (no ImGuiComboGetter mirror).
        addExtern<DAS_BIND_FUN(das::ComboCb)>(*this, lib, "_builtin_Combo_cb",
            SideEffects::worstDefault, "das::ComboCb");
        // Phase 2.1 — GetActiveID cherry-picked from imgui_internal.h.
        addExtern<DAS_BIND_FUN(das::GetActiveID)>(*this, lib, "GetActiveID",
            SideEffects::worstDefault, "das::GetActiveID");
        // Phase 7 docking — DockBuilder forwarders cherry-picked from imgui_internal.h.
        addExtern<DAS_BIND_FUN(das::DockBuilderAddNode_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderAddNode",
            SideEffects::worstDefault, "das::DockBuilderAddNode_das")
                ->args({"node_id","flags"})
                    ->arg_init(0, new ExprConstUInt(uint32_t(0)))
                    ->arg_init(1, new ExprConstEnumeration("None", makeType<ImGuiDockNodeFlags_>(lib)));
        addExtern<DAS_BIND_FUN(das::DockBuilderRemoveNode_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderRemoveNode",
            SideEffects::worstDefault, "das::DockBuilderRemoveNode_das");
        addExtern<DAS_BIND_FUN(das::DockBuilderRemoveNodeDockedWindows_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderRemoveNodeDockedWindows",
            SideEffects::worstDefault, "das::DockBuilderRemoveNodeDockedWindows_das")
                ->args({"node_id","clear_settings_refs"})
                    ->arg_init(1, new ExprConstBool(true));
        addExtern<DAS_BIND_FUN(das::DockBuilderRemoveNodeChildNodes_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderRemoveNodeChildNodes",
            SideEffects::worstDefault, "das::DockBuilderRemoveNodeChildNodes_das");
        addExtern<DAS_BIND_FUN(das::DockBuilderSetNodeSize_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderSetNodeSize",
            SideEffects::worstDefault, "das::DockBuilderSetNodeSize_das");
        addExtern<DAS_BIND_FUN(das::DockBuilderSplitNode_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderSplitNode",
            SideEffects::worstDefault, "das::DockBuilderSplitNode_das")
                ->args({"node_id","split_dir","size_ratio_for_node_at_dir","out_id_at_dir","out_id_at_opposite_dir"});
        addExtern<DAS_BIND_FUN(das::DockBuilderDockWindow_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderDockWindow",
            SideEffects::worstDefault, "das::DockBuilderDockWindow_das");
        addExtern<DAS_BIND_FUN(das::DockBuilderFinish_das), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderFinish",
            SideEffects::worstDefault, "das::DockBuilderFinish_das");
        addExtern<DAS_BIND_FUN(das::DockBuilderHasNode), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderHasNode",
            SideEffects::worstDefault, "das::DockBuilderHasNode");
        addExtern<DAS_BIND_FUN(das::DockBuilderAddDockSpaceNode), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "DockBuilderAddDockSpaceNode",
            SideEffects::worstDefault, "das::DockBuilderAddDockSpaceNode")
                ->args({"node_id","flags"})
                    ->arg_init(0, new ExprConstUInt(uint32_t(0)))
                    ->arg_init(1, new ExprConstEnumeration("None", makeType<ImGuiDockNodeFlags_>(lib)));
        // plot lines and historgram
        addExtern<DAS_BIND_FUN(das::PlotLines)>(*this, lib, "_builtin_PlotLines",
            SideEffects::worstDefault, "das::PlotLines");
        addExtern<DAS_BIND_FUN(das::PlotHistogram)>(*this, lib, "_builtin_PlotHistogram",
            SideEffects::worstDefault, "das::PlotHistogram");
        // Phase 2.7 — Plot getters with per-call lambda (no ImGuiPlotGetter mirror).
        addExtern<DAS_BIND_FUN(das::PlotLinesCb)>(*this, lib, "_builtin_PlotLines_cb",
            SideEffects::worstDefault, "das::PlotLinesCb");
        addExtern<DAS_BIND_FUN(das::PlotHistogramCb)>(*this, lib, "_builtin_PlotHistogram_cb",
            SideEffects::worstDefault, "das::PlotHistogramCb");
        // additional default values
        findUniqueFunction("AddRect")
            ->arg_init(5, new ExprConstEnumeration("RoundCornersAll",makeType<ImDrawFlags_>(lib)));
        findUniqueFunction("AddRectFilled")
            ->arg_init(5, new ExprConstEnumeration("RoundCornersAll",makeType<ImDrawFlags_>(lib)));
        findUniqueFunction("BeginTable")
            ->arg_init(3, new ExprCall(LineInfo(), "ImVec2"));
        for ( auto & fn : functionsByName[hash64z("Selectable")] ) {
            fn->arg_init(3, new ExprCall(LineInfo(), "ImVec2"));
        }
        findUniqueFunction("SetNextWindowPos")
            ->arg_init(2, new ExprCall(LineInfo(), "ImVec2"));
        findUniqueFunction("Button")
            ->arg_init(1, new ExprCall(LineInfo(), "ImVec2"));
        for ( auto & fn : functionsByName[hash64z("PlotHistogram")] ) {
            if ( fn->arguments.size()==9 ) {
                fn->arg_init(7, new ExprCall(LineInfo(), "ImVec2"));
                fn->arg_init(8, new ExprConstInt(int32_t(sizeof(float))));
            }
        }
        findUniqueFunction("TableSetupColumn")
            ->arg_init(3, new ExprConstUInt(uint32_t(0)));
        findUniqueFunction("BeginListBox")
            ->arg_init(1, new ExprCall(LineInfo(), "ImVec2"));
        findUniqueFunction("ColorButton")
            ->arg_init(3, new ExprCall(LineInfo(), "ImVec2"));
        // time to fix-up const & ImVec2 and const & ImVec4
        for ( auto & pfn : this->functions.each() ) {
            bool anyString = false;
            for ( auto & arg : pfn->arguments ) {
                if ( arg->type->constant && arg->type->ref && !arg->type->isArray() ) {
                    if ( arg->type->baseType==Type::tFloat2 || arg->type->baseType==Type::tFloat4 ) {
                        arg->type->ref = false;
                    }
                }
                if ( arg->type->isString() && !arg->type->ref ) {
                    anyString = true;
                }
            }
            if ( anyString ) {
                pfn->needStringCast = true;
            }
        }
    }

	ModuleAotType Module_dasIMGUI::aotRequire ( TextWriter & tw ) const {
        // add your stuff here
        tw << "#include \"../modules/dasImgui/src/imgui_stub.h\"\n";
        tw << "#include \"../modules/dasImgui/src/aot_dasIMGUI.h\"\n";
        tw << "#include \"daScript/simulate/bind_enum.h\"\n";
        tw << "#include \"../modules/dasImgui/src/dasIMGUI.enum.decl.cast.inc\"\n";
        // specifying AOT type, in this case direct cpp mode (and not hybrid mode)
        return ModuleAotType::cpp;
    }

}
