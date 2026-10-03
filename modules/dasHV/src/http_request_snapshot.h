#pragma once

#include <hv/HttpMessage.h>
#include <memory>

namespace das {
inline std::shared_ptr<HttpRequest> snapshot_received_http_request(const HttpRequest & source) {
    auto snapshot = std::make_shared<HttpRequest>(source);
    snapshot->http_cb = nullptr;
    snapshot->content = nullptr;
    snapshot->content_length = snapshot->body.size();
    return snapshot;
}
}
