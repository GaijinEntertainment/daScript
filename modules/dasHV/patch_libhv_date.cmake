# Each server owns a timer, but the Date cache was process-global. Keep cached
# writes and response serialization on their calling thread; other threads use
# HttpResponse::Dump's existing reentrant date-formatting fallback.
das_hv_patch_begin("http/HttpMessage.h")
das_hv_hunk([=[
    static char         s_date[32];
]=] [=[
    static char* date_cache();
]=])
das_hv_patch_end()

das_hv_patch_begin("http/HttpMessage.cpp")
das_hv_hunk([=[
char HttpMessage::s_date[32] = {0};
]=] [=[
char* HttpMessage::date_cache() {
    static thread_local char value[32] = {0};
    return value;
}
]=])
# Keep TLS implementation-private: Windows cannot export thread-local data on a
# class with a DLL interface, but can export the accessor normally.
das_hv_hunk([=[
        if (*s_date) {
            headers["Date"] = s_date;
]=] [=[
        if (*date_cache()) {
            headers["Date"] = date_cache();
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpServer.cpp")
das_hv_hunk([=[
            gmtime_fmt(hloop_now(hevent_loop(timer)), HttpMessage::s_date);
]=] [=[
            gmtime_fmt(hloop_now(hevent_loop(timer)), HttpMessage::date_cache());
]=])
das_hv_patch_end()
