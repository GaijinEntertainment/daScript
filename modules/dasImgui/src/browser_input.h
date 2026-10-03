#pragma once
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <string>
#include "imgui_backend_state.h"
EM_JS_DEPS(das_browser_clipboard_helpers, "$UTF8ToString,$stringToNewUTF8,free");

EM_JS(int, das_browser_is_macos, (), {
    return /Mac/.test(navigator.platform || "") ? 1 : 0;
});
// modules/dasImgui/ARCHITECTURE.md#browser-composition-ordering
EM_JS(void, das_browser_clipboard_install, (int pointer_is_64bit), {
    if (Module['dasClipboardBridge']) return;
    const state = {active:false, enabled:true, bytes:null, error:false, composing:false, visible:false, lastCommit:null, disposed:false, editor:0};
    state.cancel=()=>{state.composing=false;state.lastCommit=null;if(state.host)state.host.value="";};
    state.focus=()=>{
        if (!state.active || !state.enabled || !state.visible || !document.hasFocus()) return;
        const active=document.activeElement;
        if (active===state.host) return;
        if (active && active!==document.body && active!==Module['canvas']) return;
        state.host.focus({preventScroll:true});
    };
    state.commit=(text,host)=>{
        if (state.disposed || state.host!==host || !state.active || !state.enabled || !text || document.activeElement!==host) return;
        if (text.length>1048576 || text.includes('\0')) {state.error=true;return;}
        const bytes=new TextEncoder().encode(text);
        if (bytes.length>1048576) {state.error=true;return;}
        const pointer=stringToNewUTF8(new TextDecoder().decode(bytes));
        if (!pointer) {state.error=true;return;}
        try {_das_imgui_browser_input_text(pointer_is_64bit ? BigInt(pointer) : pointer);}
        finally {_free(pointer);}
    };
    state.replaceHost=()=>{
        if(state.disposed)return;
        const previous=state.host;
        const style=previous ? previous.style.cssText : 'position:fixed;width:1px;height:16px;opacity:0;pointer-events:none;padding:0;border:0;resize:none';
        state.cancel();state.bytes=null;
        const host=document.createElement('textarea');state.host=host;
        host.tabIndex=-1;host.setAttribute('aria-label','Text input');
        host.autocomplete='off';host.spellcheck=false;host.autocapitalize='off';host.style.cssText=style;
        host.addEventListener('compositionstart',()=>{
            if(state.host!==host)return;state.composing=true;state.lastCommit=null;
        });
        host.addEventListener('compositionend',event=>{
            if(state.host!==host || !state.composing)return;
            state.composing=false;state.commit(event.data,host);state.lastCommit=event.data || null;host.value="";
        });
        host.addEventListener('keydown',event=>{
            if(state.host!==host)return;
            if(!state.composing && !event.isComposing && event.keyCode!==229)state.lastCommit=null;
        });
        host.addEventListener('beforeinput',event=>{
            if(event.inputType==='insertLineBreak' || event.inputType==='insertParagraph')event.preventDefault();
        });
        host.addEventListener('input',event=>{
            if(state.host!==host || state.composing || event.isComposing)return;
            if(event.inputType==='insertFromComposition' || event.inputType==='insertCompositionText' || (state.lastCommit && event.data===state.lastCommit)){
                state.lastCommit=null;host.value="";return;
            }
            if(event.inputType!=='insertLineBreak' && event.inputType!=='insertParagraph')state.commit(host.value,host);
            host.value="";
        });
        host.addEventListener('blur',()=>{if(state.host===host)state.replaceHost();});
        document.body.appendChild(host);
        if(previous)previous.remove();
    };
    state.pointerDown=()=>{
        if(!state.composing || !state.enabled || !state.active)return;
        const host=state.host;state.composing=false;state.commit(host.value,host);host.value="";
    };
    state.replaceHost();
    window.addEventListener('pointerdown',state.pointerDown,true);
    state.paste = event => {
        if (!state.enabled || !state.active || !event.clipboardData) return;
        if (event.target!==state.host && event.target && event.target.closest && event.target.closest('input,textarea,[contenteditable]')) return;
        event.preventDefault();
        state.bytes=null;
        const text = event.clipboardData.getData('text/plain');
        if (text.length > 1048576 || text.includes('\0')) { state.error=true; return; }
        const bytes = new TextEncoder().encode(text);
        if (bytes.length > 1048576) { state.error=true; return; }
        state.bytes = bytes;
    };
    document.addEventListener('paste', state.paste, true);
    Module['dasClipboardBridge'] = state;
});
EM_JS(void, das_browser_clipboard_state, (int active, int enabled, int editor), {
    const state=Module['dasClipboardBridge'];
    if (!state) return;
    state.active=!!active; state.enabled=!!enabled;
    if(state.editor!==editor){state.editor=editor;state.replaceHost();}
    if (!active || !enabled) {
        state.bytes=null;state.cancel();
        if (document.activeElement===state.host) state.host.blur();
    } else state.focus();
});
EM_JS(int, das_browser_clipboard_size, (), {
    const state=Module['dasClipboardBridge'];
    return state && state.active && state.enabled && state.bytes ? state.bytes.length+1 : 0;
});
EM_JS(void, das_browser_clipboard_read, (char* output, int capacity), {
    const state=Module['dasClipboardBridge'];
    const address=Number(output);
    if (state && state.bytes && state.bytes.length < capacity) {
        HEAPU8.set(state.bytes,address); HEAPU8[address+state.bytes.length]=0;
    } else { HEAPU8[address]=0; }
    if (state) state.bytes=null;
});
EM_JS(void, das_browser_clipboard_write, (const char* text), {
    const state=Module['dasClipboardBridge'];
    if (!state || !state.enabled) return;
    if (!navigator.clipboard) {state.error=true;return;}
    navigator.clipboard.writeText(UTF8ToString(Number(text))).catch(()=>{state.error=true;});
});
EM_JS(void, das_browser_clipboard_remove, (), {
    const state=Module['dasClipboardBridge'];
    if (!state) return;
    document.removeEventListener('paste',state.paste,true);
    window.removeEventListener('pointerdown',state.pointerDown,true);
    state.disposed=true;state.active=false;state.enabled=false;state.bytes=null;state.cancel();state.host.remove(); delete Module['dasClipboardBridge'];
});

EM_JS(void, das_browser_ime_position, (int visible, float x, float y, float height, float width, float display_height, int editor), {
    const state=Module['dasClipboardBridge'];if (!state) return;
    if(state.editor!==editor){state.editor=editor;state.replaceHost();}
    state.visible=!!visible;
    const canvas=Module['canvas'];const rect=canvas.getBoundingClientRect();
    state.host.style.left=(rect.left+x*rect.width/Math.max(1,width))+'px';
    state.host.style.top=(rect.top+y*rect.height/Math.max(1,display_height))+'px';
    state.host.style.height=Math.max(1,height*rect.height/Math.max(1,display_height))+'px';
    if (visible) state.focus();
    else {state.cancel();if(document.activeElement===state.host){state.host.blur();canvas.focus({preventScroll:true});}}
});
EM_JS(int, das_browser_block_key, (), {
    const state=Module['dasClipboardBridge'];if (!state || !state.enabled) return 0;
    const active=document.activeElement;
    if(active===state.host) return state.composing ? 1 : 0;
    return active && active!==Module['canvas'] && active!==document.body ? 1 : 0;
});
EM_JS(int, das_browser_block_character, (), {
    const state=Module['dasClipboardBridge'];if (!state || !state.enabled) return 0;
    const active=document.activeElement;
    return active && active!==Module['canvas'] && active!==document.body ? 1 : 0;
});

static ImGuiContext* browser_clipboard_owner = nullptr;
static std::string browser_clipboard_text;
static bool browser_clipboard_enabled = true;
extern "C" EMSCRIPTEN_KEEPALIVE void das_imgui_browser_input_text(const char* text) {
    if (text && browser_clipboard_enabled && ImGui::GetCurrentContext()==browser_clipboard_owner && ImGui::GetIO().WantTextInput)
        ImGui::GetIO().AddInputCharactersUTF8(text);
}
static void browser_ime_position(ImGuiContext* context, ImGuiViewport* viewport, ImGuiPlatformImeData* data) {
    if (context!=browser_clipboard_owner) return;
    const auto& io=ImGui::GetIO();
    das_browser_ime_position(data->WantTextInput, data->InputPos.x-viewport->Pos.x,
        data->InputPos.y-viewport->Pos.y, data->InputLineHeight, io.DisplaySize.x, io.DisplaySize.y, das::GetActiveID());
}
static const char* browser_clipboard_get(ImGuiContext* context) {
    if (context != browser_clipboard_owner || !browser_clipboard_enabled) return nullptr;
    const int size = das_browser_clipboard_size();
    if (size <= 0 || size > 1048577) return nullptr;
    browser_clipboard_text.resize(size);
    das_browser_clipboard_read(browser_clipboard_text.data(), size);
    return browser_clipboard_text.c_str();
}
static void browser_clipboard_set(ImGuiContext* context, const char* text) {
    if (context == browser_clipboard_owner && browser_clipboard_enabled)
        das_browser_clipboard_write(text ? text : "");
}
static void browser_clipboard_install() {
    const bool new_owner = browser_clipboard_owner != ImGui::GetCurrentContext();
    if (new_owner) {
        das_browser_clipboard_remove();
        browser_clipboard_text.clear();
        browser_clipboard_enabled = true;
    }
    browser_clipboard_owner = ImGui::GetCurrentContext();
    das_browser_clipboard_install(sizeof(void*) == 8);
    if (new_owner) ImGui::GetIO().ConfigMacOSXBehaviors = das_browser_is_macos() != 0;
    auto& platform = ImGui::GetPlatformIO();
    platform.Platform_GetClipboardTextFn = browser_clipboard_get;
    platform.Platform_SetClipboardTextFn = browser_clipboard_set;
    platform.Platform_SetImeDataFn = browser_ime_position;
}
static void browser_clipboard_tick() {
    if (ImGui::GetCurrentContext() == browser_clipboard_owner)
        das_browser_clipboard_state(ImGui::GetIO().WantTextInput, browser_clipboard_enabled, das::GetActiveID());
}
static void browser_clipboard_shutdown() {
    if (ImGui::GetCurrentContext() != browser_clipboard_owner) return;
    das_browser_clipboard_remove();
    browser_clipboard_text.clear(); browser_clipboard_owner = nullptr;
}
#endif
