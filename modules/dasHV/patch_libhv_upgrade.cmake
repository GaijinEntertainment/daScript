# Deferred WebSocket admission.

das_hv_patch_begin("http/server/WebSocketServer.h")
das_hv_hunk([=[
    int ping_interval;
]=] [=[
    http_ctx_handler onupgrade;
    int upgrade_timeout_ms = 5000;
    int ping_interval;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpResponseWriter.h")
das_hv_hunk([=[
    HttpResponsePtr response;
]=] [=[
    HttpResponsePtr response;
    bool awaiting_upgrade = false;
    std::function<int()> onupgrade;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpResponseWriter.cpp")
das_hv_hunk([=[
    if (state != SEND_BEGIN) return -1;
]=] [=[
    if (awaiting_upgrade || state != SEND_BEGIN) return -1;
]=])
das_hv_hunk([=[
int HttpResponseWriter::WriteChunked(const char* buf, int len /* = -1 */) {
    int ret = 0;
]=] [=[
int HttpResponseWriter::WriteChunked(const char* buf, int len /* = -1 */) {
    if (awaiting_upgrade) return -1;
    int ret = 0;
]=])
das_hv_hunk([=[
int HttpResponseWriter::WriteBody(const char* buf, int len /* = -1 */) {
    if (response->IsChunked()) {
]=] [=[
int HttpResponseWriter::WriteBody(const char* buf, int len /* = -1 */) {
    if (awaiting_upgrade) return -1;
    if (response->IsChunked()) {
]=])
das_hv_hunk([=[
    int ret = 0;
    bool keepAlive = response->IsKeepAlive();
]=] [=[
    if (awaiting_upgrade) {
        if (buf) response->body.assign(buf, len < 0 ? strlen(buf) : size_t(len));
        auto complete = std::move(onupgrade);
        return complete ? complete() : -1;
    }
    int ret = 0;
    bool keepAlive = response->IsKeepAlive();
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpHandler.h")
das_hv_hunk([=[
    uint64_t                last_recv_pong_time;
]=] [=[
    uint64_t                last_recv_pong_time;
    bool                    upgrade_pending = false;
    uint64_t                upgrade_timer = uint64_t(-1);
    std::shared_ptr<int>     connection_alive = std::make_shared<int>(0);
]=])
das_hv_hunk([=[
    int upgradeWebSocket();
]=] [=[
    int upgradeWebSocket();
    int finishWebSocketUpgrade();
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpHandler.cpp")
das_hv_hunk([=[
#include "wsdef.h"
]=] [=[
#include "wsdef.h"
#include "base64.h"
]=])
das_hv_hunk([=[
void HttpHandler::Close() {
    if (writer) {
]=] [=[
void HttpHandler::Close() {
    connection_alive.reset();
    upgrade_pending = false;
    if (upgrade_timer != INVALID_TIMER_ID) {
        killTimer(upgrade_timer);
        upgrade_timer = INVALID_TIMER_ID;
    }
    if (writer) {
]=])
das_hv_hunk([=[
        if (this->state == WANT_CLOSE) return;
        switch (state) {
]=] [=[
        if (this->state == WANT_CLOSE) return;
        if (this->upgrade_pending) { SetError(ERR_REQUEST); return; }
        switch (state) {
]=])
das_hv_hunk([=[
            handleUpgrade(iter_upgrade->second.c_str());
            status_code = resp->status_code;
]=] [=[
            handleUpgrade(iter_upgrade->second.c_str());
            if (upgrade_pending) return;
            status_code = resp->status_code;
]=])
das_hv_hunk([=[
int HttpHandler::FeedRecvData(const char* data, size_t len) {
    if (protocol == HttpHandler::UNKNOWN) {
]=] [=[
int HttpHandler::FeedRecvData(const char* data, size_t len) {
    if (upgrade_pending) return -1; // wait for the pending response
    if (protocol == HttpHandler::UNKNOWN) {
]=])
das_hv_hunk([=[
int HttpHandler::upgradeWebSocket() {
]=] [=[
static bool websocket_protocol_offered(const HttpRequestPtr& request, const std::string& selected) {
    if (selected.empty()) return true;
    if (selected.size() > 128) return false;
    for (unsigned char ch : selected) {
        if (ch <= 32 || ch >= 127 || strchr("()<>@,;:\"/[]?={}", ch)) return false;
    }
    auto offered = request->GetHeader(SEC_WEBSOCKET_PROTOCOL);
    for (auto token : hv::split(offered, ',')) {
        const auto first = token.find_first_not_of(" \t");
        const auto last = token.find_last_not_of(" \t");
        if (first != std::string::npos && token.substr(first, last - first + 1) == selected) return true;
    }
    return false;
}

int HttpHandler::upgradeWebSocket() {
    if (!ws_service || !ws_service->onupgrade) return finishWebSocketUpgrade();
    auto event_loop = currentThreadEventLoop;
    if (!writer || !event_loop) return SetError(ERR_INVALID_PROTOCOL);
    upgrade_pending = true;
    writer->awaiting_upgrade = true;
    std::weak_ptr<int> alive = connection_alive;
    writer->onupgrade = [this, alive, event_loop]() -> int {
        if (alive.expired()) return -1;
        auto reply = writer;
        auto decision = std::make_shared<HttpResponse>(*reply->response);
        // Always defer past the parser callback. The first End snapshots the decision;
        // repeated/late writes cannot change a timeout into an acceptance.
        event_loop->queueInLoop([this, alive, reply, decision]() {
            if (alive.expired() || !upgrade_pending || !reply->isConnected()) return;
            upgrade_pending = false;
            if (upgrade_timer != INVALID_TIMER_ID) {
                killTimer(upgrade_timer);
                upgrade_timer = INVALID_TIMER_ID;
            }
            *reply->response = *decision;
            reply->awaiting_upgrade = false;
            reply->end = HttpResponseWriter::SEND_BEGIN;
            if (decision->status_code == HTTP_STATUS_SWITCHING_PROTOCOLS &&
                websocket_protocol_offered(req, decision->GetHeader(SEC_WEBSOCKET_PROTOCOL))) {
                char accept[32] = {0};
                ws_encode_key(req->GetHeader(SEC_WEBSOCKET_KEY).c_str(), accept);
                reply->response->body.clear();
                reply->response->headers["Connection"] = "Upgrade";
                reply->response->headers["Upgrade"] = "websocket";
                reply->response->headers[SEC_WEBSOCKET_ACCEPT] = accept;
                const int sent = reply->End();
                if (alive.expired()) return;
                if (sent < 0 || !SwitchWebSocket()) { hio_close_async(io); return; }
                WebSocketOnOpen();
            } else {
                if (decision->status_code == HTTP_STATUS_SWITCHING_PROTOCOLS) {
                    reply->response->status_code = HTTP_STATUS_BAD_REQUEST;
                }
                reply->response->headers["Connection"] = "close";
                reply->End();
            }
        });
        return 0;
    };
    upgrade_timer = setTimeout(ws_service->upgrade_timeout_ms, [this, alive](TimerID) {
        if (alive.expired() || !upgrade_pending) return;
        upgrade_timer = INVALID_TIMER_ID;
        writer->response->status_code = HTTP_STATUS_REQUEST_TIMEOUT;
        writer->End();
    });
    unsigned char decoded[18];
    const auto key = req->GetHeader(SEC_WEBSOCKET_KEY);
    if (req->method != HTTP_GET || req->GetHeader(SEC_WEBSOCKET_VERSION) != "13" ||
        key.size() != 24 || hv_base64_decode(key.data(), unsigned(key.size()), decoded) != 16) {
        writer->response->status_code = HTTP_STATUS_BAD_REQUEST;
        writer->End();
        return 0;
    }
    const int result = ws_service->onupgrade(context());
    if (result != HTTP_STATUS_UNFINISHED) {
        writer->response->status_code = (http_status)result;
        writer->End();
    }
    return 0;
}

int HttpHandler::finishWebSocketUpgrade() {
]=])
das_hv_patch_end()
