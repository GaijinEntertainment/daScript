// Excerpts from pinned libhv 303f50c7, used to test patch anchors.
    Http2Parser* hp = (Http2Parser*)userdata;

    if (hp->parsed->ContentType() == APPLICATION_GRPC) {

int Http2Parser::InitRequest(HttpRequest* req) {
    req->Reset();
