# Each server owns a timer, but the Date cache was process-global. Keep cached
# writes and response serialization on their calling thread; other threads use
# HttpResponse::Dump's existing reentrant date-formatting fallback.
das_hv_patch_begin("http/HttpMessage.h")
das_hv_hunk([=[
    static char         s_date[32];
]=] [=[
    static thread_local char s_date[32];
]=])
das_hv_patch_end()

das_hv_patch_begin("http/HttpMessage.cpp")
das_hv_hunk([=[
char HttpMessage::s_date[32] = {0};
]=] [=[
thread_local char HttpMessage::s_date[32] = {0};
]=])
das_hv_patch_end()
