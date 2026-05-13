#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasIMGUI.h"
#include "need_dasIMGUI.h"
#include "aot_dasIMGUI.h"

// Cherry-picked from imgui_internal.h per master plan §3 — the public-API
// stance allows internal symbols when "a real use case bumps into them."
// Phase 2: io.active_widget needs ImGui::GetActiveID(); no other internal
// symbols added without re-discussion.
#include "imgui_internal.h"

namespace das {

    ImU32 GetActiveID() {
        return ImGui::GetActiveID();
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

    void AddText2( ImDrawList & drawList, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col,
        const char* text_begin, float wrap_width, const ImVec4* cpu_fine_clip_rect) {
        drawList.AddText(font,font_size,pos,col,text_begin,nullptr,wrap_width,cpu_fine_clip_rect);
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

    ImGuiSortDirection_ SortDirection ( const ImGuiTableColumnSortSpecs & specs ) {
        return ImGuiSortDirection_(specs.SortDirection);
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
        addExtern<DAS_BIND_FUN(das::AddText2), SimNode_ExtFuncCall, imguiTempFn>(*this, lib, "AddText",
            SideEffects::worstDefault, "das::AddText2")
                ->args({"drawList","font","font_size","pos","col","text","wrap_width","cpu_fine_clip_rect"})
                    ->arg_init(6,new ExprConstFloat(0.0f))
                    ->arg_init(7,new ExprConstPtr());
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
        // ImGuiTableColumnSortSpecs
        addExtern<DAS_BIND_FUN(das::SortDirection)>(*this,lib,"SortDirection",
            SideEffects::none,"das::SortDirection");
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
                if ( arg->type->constant && arg->type->ref && arg->type->dim.size()==0 ) {
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
