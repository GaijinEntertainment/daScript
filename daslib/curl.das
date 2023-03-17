options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module curl shared private

require strings
require fio

struct public HttpResponse
    //! HTTP request response
    status_code : int   //! HTTP status code
    text : string       //! HTTP response body

def public HTTP_REQ ( method:string; uri:string; header:string; var data:string; blk:block<(resp:HttpResponse?):void> )
    //! HTTP request
    if ! data |> empty
        data = data |> replace("\n", "")    // todo: we need multi-replace?
        data = data |> replace("\t", "")
        data = data |> replace("^", "^^")
        data = data |> replace("|", "^|")
        data = data |> replace("<", "^<")
        data = data |> replace(">", "^>")
        data = data |> escape
    var cmd = build_string <| $ ( writer )
        writer |> write("curl -s -i -X {method} {uri}")
        if !header|> empty
            writer |> write(" {header}")
        if !data |> empty
            writer |> write(" -d \"{data}\"")
    // print("request {cmd}\n")
    var resp : HttpResponse
    var inscope lines : array<string>
    let exit_code = unsafe(popen(cmd)) <| $ ( f )
        if f!=null
            while !f |> feof
                lines |> push <| fgets(f)
        else
            resp.status_code = -100500
    if exit_code != 0
        panic("curl failed with exit code {exit_code}")
    if resp.status_code==-100500
        blk |> invoke(null)
    else
        if !lines |> empty
            var first_line = 0
            if lines[0] |> starts_with("HTTP/1.1 ")
                resp.status_code = lines[0] |> slice(9) |> to_int
                first_line ++
            for i in first_line..lines |> length
                if length(lines[i]) == 1
                    first_line = i + 1
                    break
            resp.text = build_string <| $ ( writer )
                for i in first_line..lines |> length
                    writer |> write(lines[i])
        blk |> invoke(unsafe(addr(resp)))

def private header_str ( header:table<string;string> ) : string
    return build_string <| $ ( writer )
        for k,v in keys(header),values(header)
            writer |> write("-H \"")
            writer |> write(k)
            writer |> write(": ")
            writer |> write(v)
            writer |> write("\" ")

def public GET ( uri:string; blk:block<(resp:HttpResponse?):void> )
    //! HTTP GET request
    HTTP_REQ("GET", uri, "", "", blk)

def public GET ( uri:string; header:table<string;string>; blk:block<(resp:HttpResponse?):void> )
    //! HTTP GET request with header
    HTTP_REQ("GET", uri, header_str(header), "", blk)

def public POST ( uri:string; data:string; blk:block<(resp:HttpResponse?):void> )
    //! HTTP POST request
    HTTP_REQ("POST", uri, "", data, blk)

def public POST ( uri:string; data:string; header:table<string;string>; blk:block<(resp:HttpResponse?):void> )
    //! HTTP POST request with header
    HTTP_REQ("POST", uri, header_str(header), data, blk)
