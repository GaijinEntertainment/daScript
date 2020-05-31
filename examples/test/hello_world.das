// options log = true

require network

class TelnetServer : Server
    current_string : string
    done : bool = false
    def TelnetServer
        Server`Server(cast<Server> self)
    def override onError(msg:string; code:int)
        print("server error:{code} - {msg}\n")
    def override onConnect
        print("connected\n")
    def override onDisconnect
        if !empty(current_string)
            print(current_string)
            current_string = ""
        print("disconnected\n")
    [unsafe] def override onData(msg:uint8?; size:int)
        for i in range(0,size)
            let ch = int(msg[i])
            if ch=='\r'
                continue
            elif ch=='\n'
                print("`{current_string}` length={length(current_string)}\n")
                var new_msg = "length = {length(current_string)}\n"
                self->send(reinterpret<uint8?> new_msg, length(new_msg))
                if current_string=="quit"
                    done = true
                delete current_string
                delete new_msg
            else
                current_string = append(current_string,ch)

[export, unsafe]
def test
    let port = 9000
    print("sever at port {port}\n")
    var telnet = new TelnetServer()
    if !telnet->init(9000)
        return false
    while !telnet.done
        telnet->tick()
    delete telnet                       // don't forget this, or we will leak the Server
    return true
