// Excerpts from pinned libhv 303f50c7, used to test patch anchors.
    hp->state = HP_BODY;
    if (hp->invokeHttpCb(at, length) != 0) {

    hp->state = HP_MESSAGE_BEGIN;
    hp->invokeHttpCb();

        size_t content_length = atoll(iter->second.c_str());
        hp->parsed->content_length = content_length;
        size_t reserve_length = MIN(content_length + 1, MAX_CONTENT_LENGTH);

    int chunk_size = parser->content_length;
    int reserve_size = MIN(chunk_size + 1, MAX_CONTENT_LENGTH);

    printd("on_url:%.*s\n", (int)length, at);

    printd("on_status:%d %.*s\n", (int)parser->status_code, (int)length, at);

    printd("on_header_field:%.*s\n", (int)length, at);

    printd("on_header_value:%.*s\n", (int)length, at);
