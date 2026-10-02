# Keep request values out of diagnostic paths outside opt-in access logging.

das_hv_patch_begin("http/server/HttpHandler.cpp")
das_hv_hunk([=[
                hloge("[%s:%d] Illegal crlf path: %s", ip, port, pReq->path.c_str());
]=] [=[
                hloge("[%s:%d] Illegal request path", ip, port);
]=])
das_hv_hunk([=[
    hlogi("[%s:%d] Upgrade: %s", ip, port, upgrade_protocol);
]=] [=[
    hlogi("[%s:%d] Protocol upgrade requested", ip, port);
]=])
das_hv_hunk([=[
    hloge("[%s:%d] unsupported Upgrade: %s", ip, port, upgrade_protocol);
]=] [=[
    hloge("[%s:%d] unsupported protocol upgrade", ip, port);
]=])
das_hv_hunk([=[
            hlogw("[%s:%d] %s: %s => just select first protocol %s", ip, port, SEC_WEBSOCKET_PROTOCOL, iter_protocol->second.c_str(), subprotocols[0].c_str());
]=] [=[
            hlogw("[%s:%d] selecting first offered WebSocket protocol", ip, port);
]=])
das_hv_hunk([=[
        hlogw("[%s:%d] Forbidden to forward proxy %s", ip, port, req->url.c_str());
]=] [=[
        hlogw("[%s:%d] Forward proxy forbidden", ip, port);
]=])
das_hv_hunk([=[
    hlogi("[%s:%d] proxy_pass %s", ip, port, strUrl.c_str());
]=] [=[
    hlogi("[%s:%d] Proxy request", ip, port);
]=])
das_hv_hunk([=[
        hlogw("[%s:%d] Forbidden to proxy %s", ip, port, url.host.c_str());
]=] [=[
        hlogw("[%s:%d] Proxy destination forbidden", ip, port);
]=])
das_hv_patch_end()

das_hv_patch_begin("http/Http1Parser.cpp")
das_hv_hunk([=[
    printd("on_url:%.*s\n", (int)length, at);
]=] [=[
    printd("on_url:%d bytes\n", (int)length);
]=])
das_hv_hunk([=[
    printd("on_status:%d %.*s\n", (int)parser->status_code, (int)length, at);
]=] [=[
    printd("on_status:%d\n", (int)parser->status_code);
]=])
das_hv_hunk([=[
    printd("on_header_field:%.*s\n", (int)length, at);
]=] [=[
    printd("on_header_field:%d bytes\n", (int)length);
]=])
das_hv_hunk([=[
    printd("on_header_value:%.*s\n", (int)length, at);
]=] [=[
    printd("on_header_value:%d bytes\n", (int)length);
]=])
das_hv_patch_end()
