options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module curl shared private

require strings
require fio

let public HTTP_CURL_FAILED = -100500

let private LOG_REQUEST = false
let private LOG_RESPONSE = false

struct public HttpResponse
    //! HTTP request response
    status_code : int   //! HTTP status code
    text : string       //! HTTP response body
    error : string      //! misc error message

def public HTTP_REQ ( method:string; uri:string; timeout:float; header:string; var data:string; blk:block<(resp:HttpResponse?):void> )
    //! HTTP request
    if ! data |> empty
        data = data |> replace("\n", "")    // todo: we need multi-replace?
        data = data |> replace("\t", "")
        data = data |> sanitize_command_line
        data = data |> escape
    var cmd = build_string <| $ ( writer )
        writer |> write("curl -s -i -X {method} {uri}")
        if timeout != 0.0
            writer |> write(" --connect-timeout ") |> format("%.1f", timeout) |> write(" --max-time ") |> format("%.1f", timeout)
        if !header|> empty
            writer |> write(" {header}")
        if !data |> empty
            writer |> write(" -d \"{data}\"")
    static_if LOG_REQUEST
        print("request {cmd}\n")
    var resp : HttpResponse
    var inscope lines : array<string>
    let exit_code = unsafe(popen(cmd)) <| $ ( f )
        if f!=null
            while !f |> feof
                lines |> push <| fgets(f)
        else
            resp.status_code = HTTP_CURL_FAILED
            resp.error = "curl failed to start"
    static_if LOG_RESPONSE
        for l in lines
            print("{l}\n")
    if exit_code != 0
        resp.status_code = HTTP_CURL_FAILED
        if exit_code==28
            resp.error = "curl timeout (max {timeout} seconds)"
        elif exit_code==26
            resp.error = "file not found"
        else
            resp.error = "curl failed with exit code {exit_code}"
    if resp.status_code!=HTTP_CURL_FAILED
        if !lines |> empty
            var first_line = 0
            let linesl = lines |> length
            for i in first_line..linesl-1
                if lines[first_line] |> starts_with("HTTP/1.1 100 Continue")
                    first_line += 2
                elif lines[first_line] |> starts_with("HTTP/2 100 Continue")
                    first_line += 2
                else
                    break
            if lines[first_line] |> starts_with("HTTP/1.1 ")
                resp.status_code = lines[first_line] |> slice(9) |> to_int
                first_line ++
            elif lines[first_line] |> starts_with("HTTP/2 ")
                resp.status_code = lines[first_line] |> slice(7) |> to_int
                first_line ++
            for i in first_line..linesl
                let llength = lines[i] |> length
                if llength==1 || llength==2
                    first_line = i + 1
                    break
            resp.text = build_string <| $ ( writer )
                for i in first_line..lines |> length
                    writer |> write(lines[i])
    blk |> invoke(unsafe(addr(resp)))

def private header_str ( header:array<tuple<string;string>> ) : string
    return build_string <| $ ( writer )
        for kv in header
            writer |> write("-H \"") |> write(kv._0) |> write(": ") |> write(kv._1) |> write("\" ")

def private fragment_str ( fragment:array<tuple<string;string>> ) : string
    return build_string <| $ ( writer )
        for kv in fragment
            writer |> write("-F ") |> write(kv._0) |> write("=") |> write(kv._1) |> write(" ")

def public GET ( uri:string; timeout:float; blk:block<(resp:HttpResponse?):void> )
    //! HTTP GET request
    HTTP_REQ("GET", uri, timeout, "", "", blk)

def public GET ( uri:string; timeout:float; header:array<tuple<string;string>>; blk:block<(resp:HttpResponse?):void> )
    //! HTTP GET request with header
    HTTP_REQ("GET", uri, timeout, header_str(header), "", blk)

def public POST ( uri:string; timeout:float; data:string; blk:block<(resp:HttpResponse?):void> )
    //! HTTP POST request
    HTTP_REQ("POST", uri, timeout, "", data, blk)

def public POST ( uri:string; timeout:float; data:string; header:array<tuple<string;string>>; blk:block<(resp:HttpResponse?):void> )
    //! HTTP POST request with header
    HTTP_REQ("POST", uri, timeout, header_str(header), data, blk)

def public POST ( uri:string; timeout:float; data:string; header:array<tuple<string;string>>; fragment:array<tuple<string;string>>; blk:block<(resp:HttpResponse?):void> )
    //! HTTP POST request with header
    HTTP_REQ("POST", uri, timeout, header_str(header)+fragment_str(fragment), data, blk)



