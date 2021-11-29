require uriparser

/*
    URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
    authority = [userinfo "@"] host [":" port]
*/

[export]
def main
    using("scheme://userinfo@host:1234/one/two/three?quone=one&qtow=2#someframgnet") <| $ ( var uri:Uri# )
        debug(uri,"URI=")

