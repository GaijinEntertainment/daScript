// options log=true

require libCurl

[export,unsafe]
def test
    var curl = new CURL
    curl_easy_setop_url(curl,"www.google.com")
    let callback = lambda() <| $ (data:string;sz,nmemb:uint)
        print("data={data}\n")
    curl_easy_setopt_writedata(curl, callback)
    curl_perform(curl)
    delete curl
    return true

