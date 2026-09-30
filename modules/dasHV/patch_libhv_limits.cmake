# Bounded parser storage for HTTP bodies and fragmented WebSocket messages.

das_hv_patch_begin("http/HttpParser.h")
das_hv_hunk([=[
    http_session_type   type;
]=] [=[
    http_session_type   type;
    size_t max_body_size = 0;
    size_t body_received = 0;
    bool body_limit_exceeded = false;

    bool AcceptBody(size_t size) {
        if (max_body_size && (size > max_body_size || body_received > max_body_size - size)) {
            body_limit_exceeded = true;
            return false;
        }
        body_received += size;
        return true;
    }
]=])
das_hv_patch_end()

das_hv_patch_begin("http/Http1Parser.cpp")
das_hv_hunk([=[
    hp->state = HP_BODY;
    if (hp->invokeHttpCb(at, length) != 0) {
]=] [=[
    if (!hp->AcceptBody(length)) return -1;
    hp->state = HP_BODY;
    if (hp->invokeHttpCb(at, length) != 0) {
]=])
das_hv_hunk([=[
    hp->state = HP_MESSAGE_BEGIN;
    hp->invokeHttpCb();
]=] [=[
    hp->body_received = 0;
    hp->body_limit_exceeded = false;
    hp->state = HP_MESSAGE_BEGIN;
    hp->invokeHttpCb();
]=])
das_hv_hunk([=[
        size_t content_length = atoll(iter->second.c_str());
        hp->parsed->content_length = content_length;
        size_t reserve_length = MIN(content_length + 1, MAX_CONTENT_LENGTH);
]=] [=[
        if (hp->max_body_size && parser->content_length > hp->max_body_size) {
            hp->body_limit_exceeded = true;
            return -1;
        }
        size_t content_length = (size_t)parser->content_length;
        hp->parsed->content_length = content_length;
        size_t reserve_length = (size_t)MIN(parser->content_length, MAX_CONTENT_LENGTH - 1) + 1;
]=])
das_hv_hunk([=[
    int chunk_size = parser->content_length;
    int reserve_size = MIN(chunk_size + 1, MAX_CONTENT_LENGTH);
]=] [=[
    if (hp->max_body_size && (parser->content_length > hp->max_body_size ||
        hp->body_received > hp->max_body_size - parser->content_length)) {
        hp->body_limit_exceeded = true;
        return -1;
    }
    uint64_t chunk_size = parser->content_length;
    size_t reserve_size = (size_t)MIN(chunk_size, MAX_CONTENT_LENGTH - 1) + 1;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/Http2Parser.cpp")
das_hv_hunk([=[
    Http2Parser* hp = (Http2Parser*)userdata;

    if (hp->parsed->ContentType() == APPLICATION_GRPC) {
]=] [=[
    Http2Parser* hp = (Http2Parser*)userdata;
    if (!hp->AcceptBody(len)) return NGHTTP2_ERR_CALLBACK_FAILURE;

    if (hp->parsed->ContentType() == APPLICATION_GRPC) {
]=])
das_hv_hunk([=[
int Http2Parser::InitRequest(HttpRequest* req) {
    req->Reset();
]=] [=[
int Http2Parser::InitRequest(HttpRequest* req) {
    body_received = 0;
    body_limit_exceeded = false;
    req->Reset();
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpService.h")
das_hv_hunk([=[
    int keepalive_timeout;
]=] [=[
    size_t max_request_body_size = 0;
    int keepalive_timeout;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/HttpHandler.cpp")
das_hv_hunk([=[
    parser->InitRequest(req.get());
]=] [=[
    parser->max_body_size = service ? service->max_request_body_size : 0;
    parser->InitRequest(req.get());
]=])
das_hv_hunk([=[
        nfeed = parser->FeedRecvData(data, len);
]=] [=[
        nfeed = parser->FeedRecvData(data, len);
        if (parser->body_limit_exceeded) {
            // Parser callbacks have returned. Do not route a rejected request, and do
            // not access this after End: a completed close may destroy the handler.
            auto reply = writer;
            if (!reply) return -1;
            state = WANT_CLOSE;
            reply->response->status_code = HTTP_STATUS_PAYLOAD_TOO_LARGE;
            reply->response->headers["Connection"] = "close";
            reply->End();
            return (int)len;
        }

]=])
das_hv_hunk([=[
    ws_parser  = std::make_shared<WebSocketParser>();
]=] [=[
    ws_parser  = std::make_shared<WebSocketParser>();
    ws_parser->max_message_size = ws_service ? ws_service->max_message_size : 0;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/server/WebSocketServer.h")
das_hv_hunk([=[
    int ping_interval;
]=] [=[
    size_t max_message_size = 0;
    int ping_interval;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/WebSocketParser.h")
das_hv_hunk([=[
    std::string                         message;
]=] [=[
    std::string                         message;
    std::string                         control_message;
    size_t                              max_message_size = 0;
    bool                                fragmented = false;
    bool                                control_frame = false;
]=])
das_hv_patch_end()

das_hv_patch_begin("http/WebSocketParser.cpp")
das_hv_hunk([=[
static int on_frame_header(websocket_parser* parser) {
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    int opcode = parser->flags & WS_OP_MASK;
    // printf("on_frame_header opcode=%d\n", opcode);
    if (opcode != WS_OP_CONTINUE) {
        wp->opcode = opcode;
    }
    int length = parser->length;
    int reserve_length = MIN(length + 1, MAX_PAYLOAD_LENGTH);
    if (reserve_length > wp->message.capacity()) {
        wp->message.reserve(reserve_length);
    }
    if (wp->state == WS_FRAME_BEGIN ||
        wp->state == WS_FRAME_FIN) {
        wp->message.clear();
    }
    wp->state = WS_FRAME_HEADER;
    return 0;
}

static int on_frame_body(websocket_parser* parser, const char * at, size_t length) {
    // printf("on_frame_body length=%d\n", (int)length);
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    wp->state = WS_FRAME_BODY;
    if (wp->parser->flags & WS_HAS_MASK) {
        websocket_parser_decode((char*)at, at, length, wp->parser);
    }
    wp->message.append(at, length);
    return 0;
}

static int on_frame_end(websocket_parser* parser) {
    // printf("on_frame_end\n");
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    wp->state = WS_FRAME_END;
    if (wp->parser->flags & WS_FIN) {
        wp->state = WS_FRAME_FIN;
        if (wp->onMessage) {
            wp->onMessage(wp->opcode, wp->message);
        }
    }
    return 0;
}
]=] [=[
static int on_frame_header(websocket_parser* parser) {
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    int frame_opcode = parser->flags & WS_OP_MASK;
    wp->control_frame = frame_opcode >= WS_OP_CLOSE;
    if (wp->control_frame) {
        if (!(parser->flags & WS_FIN) || parser->length > 125 || frame_opcode > WS_OP_PONG) return -1;
        wp->control_message.clear();
    } else {
        if (frame_opcode == WS_OP_CONTINUE) {
            if (!wp->fragmented) return -1;
        } else {
            if (wp->fragmented || (frame_opcode != WS_OP_TEXT && frame_opcode != WS_OP_BINARY)) return -1;
            wp->opcode = frame_opcode;
            wp->message.clear();
        }
        if (wp->max_message_size && (parser->length > wp->max_message_size ||
            wp->message.size() > wp->max_message_size - parser->length)) return -1;
        wp->fragmented = !(parser->flags & WS_FIN);
    }
    auto& payload = wp->control_frame ? wp->control_message : wp->message;
    size_t reserve_length = payload.size() + (size_t)MIN(parser->length, MAX_PAYLOAD_LENGTH);
    if (reserve_length > payload.capacity()) payload.reserve(reserve_length);
    wp->state = WS_FRAME_HEADER;
    return 0;
}

static int on_frame_body(websocket_parser* parser, const char * at, size_t length) {
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    auto& payload = wp->control_frame ? wp->control_message : wp->message;
    size_t limit = wp->control_frame ? 125 : wp->max_message_size;
    if (limit && (length > limit || payload.size() > limit - length)) return -1;
    wp->state = WS_FRAME_BODY;
    if (wp->parser->flags & WS_HAS_MASK) {
        websocket_parser_decode((char*)at, at, length, wp->parser);
    }
    payload.append(at, length);
    return 0;
}

static int on_frame_end(websocket_parser* parser) {
    WebSocketParser* wp = (WebSocketParser*)parser->data;
    wp->state = WS_FRAME_END;
    if (parser->flags & WS_FIN) {
        wp->state = WS_FRAME_FIN;
        if (wp->onMessage) {
            wp->onMessage(wp->control_frame ? (parser->flags & WS_OP_MASK) : wp->opcode,
                wp->control_frame ? wp->control_message : wp->message);
        }
    }
    return 0;
}


]=])
das_hv_patch_end()
