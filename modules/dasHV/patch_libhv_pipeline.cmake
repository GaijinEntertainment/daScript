# Serialize asynchronous HTTP/1 responses while retaining a bounded read remainder.

das_hv_patch_begin("http/HttpParser.h")
das_hv_hunk([=[
    virtual int GetError() = 0;
]=] [=[
    virtual void Pause() {}
    virtual bool IsPaused() { return false; }
    virtual int GetError() = 0;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/Http1Parser.h")
das_hv_hunk([=[
    virtual int GetError() {
]=] [=[
    virtual void Pause() { http_parser_pause(&parser, 1); }
    virtual bool IsPaused() { return parser.http_errno == HPE_PAUSED; }
    virtual int GetError() {
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpResponseWriter.h")
das_hv_hunk([=[
    HttpResponsePtr response;
]=] [=[
    HttpResponsePtr response;
    std::function<void()> onend;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpResponseWriter.cpp")
das_hv_hunk([=[
    if (end == SEND_END) return 0;
    end = SEND_END;
]=] [=[
    if (end == SEND_END) return 0;
    auto completion = onend;
    end = SEND_END;
]=])
das_hv_hunk([=[
    if (!keepAlive) {
        close(true);
    }
]=] [=[
    if (ret >= 0 && completion) completion();
    if (!keepAlive) {
        close(true);
    }
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpHandler.h")
das_hv_hunk([=[
    HttpContextPtr          ctx;
]=] [=[
    HttpContextPtr          ctx;
    bool                    http_pending = false;
    std::string             pipeline_buffer;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpHandler.cpp")
das_hv_hunk([=[
    if (upgrade_pending) return -1; // wait for the pending response
]=] [=[
    if (upgrade_pending) return -1; // wait for the pending response
    if (http_pending) {
        if (len > 65536 - pipeline_buffer.size()) return -1;
        pipeline_buffer.append(data, len);
        return (int)len;
    }
]=])
das_hv_hunk([=[
        writer->status = hv::SocketChannel::CONNECTED;
]=] [=[
        writer->status = hv::SocketChannel::CONNECTED;
        auto event_loop = currentThreadEventLoop;
        std::weak_ptr<int> alive = connection_alive;
        writer->onend = [this, alive, event_loop]() {
            if (!event_loop || alive.expired()) return;
            event_loop->queueInLoop([this, alive]() {
                if (alive.expired() || !http_pending || protocol != HTTP_V1) return;
                http_pending = false;
                std::string pending;
                pending.swap(pipeline_buffer);
                Reset();
                if (!pending.empty()) {
                    const int consumed = FeedRecvData(pending.data(), pending.size());
                    if (alive.expired()) return;
                    if (consumed != (int)pending.size()) { hio_close_async(io); return; }
                }
            });
        };
]=])
das_hv_hunk([=[
    // access log
    if (service && service->enable_access_log) {
]=] [=[
    if (status_code == HTTP_STATUS_UNFINISHED && protocol == HTTP_V1 && !upgrade_pending) {
        http_pending = true;
        parser->Pause();
    }

    // access log
    if (service && service->enable_access_log) {
]=])
das_hv_hunk([=[
        // printf("FeedRecvData %d=>%d\n", (int)len, nfeed);
        if (nfeed != len) {
]=] [=[
        if (parser->IsPaused()) {
            if (nfeed < 0 || size_t(nfeed) > len || len - size_t(nfeed) > 65536) return -1;
            pipeline_buffer.assign(data + nfeed, len - size_t(nfeed));
            return (int)len;
        }
        // printf("FeedRecvData %d=>%d\n", (int)len, nfeed);
        if (nfeed != len) {
]=])
das_hv_patch_end()
