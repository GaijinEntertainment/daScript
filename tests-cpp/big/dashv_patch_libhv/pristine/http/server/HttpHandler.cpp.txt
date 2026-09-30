// Excerpts from pinned libhv 303f50c7, used to test patch anchors.
    parser->InitRequest(req.get());

        nfeed = parser->FeedRecvData(data, len);

    ws_parser  = std::make_shared<WebSocketParser>();

#include "wsdef.h"

void HttpHandler::Close() {
    if (writer) {

        if (this->state == WANT_CLOSE) return;
        switch (state) {

            handleUpgrade(iter_upgrade->second.c_str());
            status_code = resp->status_code;

int HttpHandler::FeedRecvData(const char* data, size_t len) {
    if (protocol == HttpHandler::UNKNOWN) {

int HttpHandler::upgradeWebSocket() {

        writer->status = hv::SocketChannel::CONNECTED;

    // access log
    if (service && service->enable_access_log) {

        // printf("FeedRecvData %d=>%d\n", (int)len, nfeed);
        if (nfeed != len) {

                hloge("[%s:%d] Illegal crlf path: %s", ip, port, pReq->path.c_str());

    hlogi("[%s:%d] Upgrade: %s", ip, port, upgrade_protocol);

    hloge("[%s:%d] unsupported Upgrade: %s", ip, port, upgrade_protocol);

            hlogw("[%s:%d] %s: %s => just select first protocol %s", ip, port, SEC_WEBSOCKET_PROTOCOL, iter_protocol->second.c_str(), subprotocols[0].c_str());

        hlogw("[%s:%d] Forbidden to forward proxy %s", ip, port, req->url.c_str());

    hlogi("[%s:%d] proxy_pass %s", ip, port, strUrl.c_str());

        hlogw("[%s:%d] Forbidden to proxy %s", ip, port, url.host.c_str());
