// options log = true

require network
require fio

var
    isContDev = false

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
    [unsafe] def restart
        if _server != null
            var session : smart_ptr<NetworkServer>
            self->save(session)
            gc0_save_smart_ptr("telnet-session", session)
    [unsafe] def override onData(msg:uint8?; size:int)
        for i in range(size)
            let ch = int(msg[i])
            if ch=='\r'
                continue
            elif ch=='\n'
                print("`{current_string}` length={length(current_string)}\n")
                var new_msg = "length = {length(current_string)}\n"
                self->send(reinterpret<uint8?> new_msg, length(new_msg))
                if current_string=="quit"
                    done = true
                elif current_string=="restart"
                    if isContDev
                        print("restarting via telnet command")
                        done = true
                        self->restart()
                    else
                        print("restart request ignored, not running from dasContDev\n")
                delete_string(current_string)
                delete_string(new_msg)
            else
                current_string = append(current_string,ch)

[export,unsafe]
def main (fn:string)
    isContDev = true
    let main_time = stat(fn).mtime
    print("server at {fn} {main_time}\n")
    let port = 9000
    var telnet = new TelnetServer()
    var session : smart_ptr<NetworkServer> <- gc0_restore_smart_ptr("telnet-session")
    if session != null
        print("restoring server after restart\n")
        telnet->restore(session)
    elif telnet->init(9000)
        print("sever at port {port}\n")
    else
        print("server failed to initialize\n")
        return false
    while !telnet.done
        telnet->tick()
        let new_time = stat(fn).mtime
        if new_time != main_time
            print("restarting via source change {new_time}\n")
            telnet->restart()
            break
    delete telnet
    return true

[export, unsafe]
def test
    let port = 9000
    print("sever at port {port}\n")
    var telnet = new TelnetServer()
    if !telnet->init(9000)
        return false
    while !telnet.done
        telnet->tick()
    delete telnet
    return true
