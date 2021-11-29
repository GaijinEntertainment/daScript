require uriparser

/*
    URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
    authority = [userinfo "@"] host [":" port]
*/

[export]
def main
    using("scheme://userinfo@host:1234/one/two/three?quone=one&qtow=2#someframgnet") <| $ ( var uri:Uri# )
        print("uri = {string(uri)}\n")

    unsafe
        var uriA <- Uri("blah")
        var uriB <- Uri("http://www.google.com/")
        var uriC <- add_base_uri(uriB, uriA)
        print("uriA = {string(uriA)}\n")
        print("uriB = {string(uriB)}\n")
        print("uriC = {string(uriC)} {uriC.status}\n")