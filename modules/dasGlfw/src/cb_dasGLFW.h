namespace das {
    struct GlswCB {
        Lambda      lambda;
        Context *   context = nullptr;
    };

    struct GlswCallbacks {
        GlswCB  keyCB;
        GlswCB  charCB;
        GlswCB  cursorCB;
        GlswCB  cursorEnterCB;
        GlswCB  mouseCB;
        GlswCB  scrollCB;
    };
}
