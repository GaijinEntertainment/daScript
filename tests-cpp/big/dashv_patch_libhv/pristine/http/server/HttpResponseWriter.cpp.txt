// Excerpts from pinned libhv 303f50c7, used to test patch anchors.

    if (state != SEND_BEGIN) return -1;

int HttpResponseWriter::WriteChunked(const char* buf, int len /* = -1 */) {
    int ret = 0;

int HttpResponseWriter::WriteBody(const char* buf, int len /* = -1 */) {
    if (response->IsChunked()) {

    int ret = 0;
    bool keepAlive = response->IsKeepAlive();

    if (end == SEND_END) return 0;
    end = SEND_END;

    if (!keepAlive) {
        close(true);
    }
