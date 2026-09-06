#include "daScript/misc/platform.h"

#include "dasStdDlgTray.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"

namespace das {

    static TrayBackend * g_tray = nullptr;
    static vector<TrayMenuEntry> g_pendingMenu;

    bool TrayAvailable() {
        return TrayPlatformAvailable();
    }

    bool TrayCreate(const char * tooltip) {
        if ( g_tray ) return true;
        TrayBackend * backend = TrayPlatformCreate();
        if ( !backend ) return false;
        if ( !backend->create(tooltip ? tooltip : "") ) {
            delete backend;
            return false;
        }
        g_tray = backend;
        return true;
    }

    void TraySetIcon(const TArray<uint8_t> & rgba8, int32_t width, int32_t height, Context * context, LineInfoArg * at) {
        if ( width <= 0 || height <= 0 ) {
            context->throw_error_at(at, "tray_set_icon: icon size %dx%d", width, height);
        }
        if ( int64_t(rgba8.size) != int64_t(width) * int64_t(height) * 4 ) {
            context->throw_error_at(at, "tray_set_icon: %d bytes for a %dx%d RGBA8 icon (need %d)",
                int(rgba8.size), width, height, width * height * 4);
        }
        if ( g_tray ) g_tray->setIcon((const uint8_t *) rgba8.data, width, height);
    }

    void TraySetTooltip(const char * text) {
        if ( g_tray ) g_tray->setTooltip(text ? text : "");
    }

    void TrayMenuClear() {
        g_pendingMenu.clear();
    }

    void TrayMenuAdd(int32_t id, const char * label, bool enabled, bool checked) {
        g_pendingMenu.push_back(TrayMenuEntry{id, label ? label : "", enabled, checked, false});
    }

    void TrayMenuAddSeparator() {
        g_pendingMenu.push_back(TrayMenuEntry{0, "", false, false, true});
    }

    void TrayMenuCommit() {
        if ( g_tray ) g_tray->setMenu(g_pendingMenu);
    }

    void TrayPoll(const TBlock<void, const TrayEvent &> & blk, Context * context, LineInfoArg * at) {
        if ( !g_tray ) return;
        vector<TrayEvent> events;
        g_tray->poll(events);
        for ( auto & ev : events ) {
            das_invoke<void>::invoke<const TrayEvent &>(context, at, blk, ev);
        }
    }

    bool TrayNotify(const char * title, const char * body) {
        if ( !g_tray ) return false;
        return g_tray->notify(title ? title : "", body ? body : "");
    }

    void TrayDestroy() {
        delete g_tray;
        g_tray = nullptr;
        g_pendingMenu.clear();
    }
}
