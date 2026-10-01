# Keep the request Cookie field available for strict duplicate/ambiguity checks.
das_hv_patch_begin("http/Http1Parser.h")
das_hv_hunk([=[
            if (stricmp(header_field.c_str(), "Set-CooKie") == 0 ||
]=] [=[
            if (parsed->type == HTTP_REQUEST && stricmp(header_field.c_str(), "Cookie") == 0) {
                auto& raw_cookie = parsed->headers["Cookie"];
                if (!raw_cookie.empty()) raw_cookie += "; ";
                raw_cookie += header_value;
                HttpCookie cookie;
                if (cookie.parse(header_value)) parsed->cookies.emplace_back(cookie);
                header_field.clear();
                header_value.clear();
                return;
            }
            if (stricmp(header_field.c_str(), "Set-CooKie") == 0 ||
]=])
das_hv_patch_end()

# Unknown cookie attributes may contain credentials; do not dump their values.
das_hv_patch_begin("http/HttpMessage.cpp")
das_hv_hunk([=[
                hlogi("Cookie Unrecognized key '%s'", key.c_str());
]=] [=[
                // Ignore unrecognized cookie attributes without logging their contents.
]=])
das_hv_patch_end()

# A preserved request header is authoritative when forwarding/serializing it.
das_hv_patch_begin("http/HttpMessage.cpp")
das_hv_hunk([=[
void HttpMessage::AddCookie(const HttpCookie& cookie) {
    cookies.push_back(cookie);
}
]=] [=[
void HttpMessage::AddCookie(const HttpCookie& cookie) {
    if (type == HTTP_REQUEST) {
        auto raw = headers.find("Cookie");
        if (raw != headers.end()) {
            if (!raw->second.empty()) raw->second += "; ";
            raw->second += cookie.dump();
        }
    }
    cookies.push_back(cookie);
}
]=])
das_hv_hunk([=[
    // cookies
    const char* cookie_field = "Cookie";
]=] [=[
    // Parsed request cookies are already represented by the preserved raw field.
    if (type == HTTP_REQUEST && headers.find("Cookie") != headers.end()) return;
    // cookies
    const char* cookie_field = "Cookie";
]=])
das_hv_patch_end()
