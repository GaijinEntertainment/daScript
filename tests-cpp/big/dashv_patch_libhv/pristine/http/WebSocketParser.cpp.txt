// Excerpts from pinned libhv 303f50c7, used to test patch anchors.
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
