require network

require rtti

class Server
    _server : smart_ptr<NetworkServer>
    [unsafe] def Server
        let classInfo = class_info(self)
        if !make_server(addr(self),classInfo)
            panic("can't make server")
    def init ( port : int ) : bool
        return server_init(_server,port)
    def is_open : bool
        return server_is_open(_server)
    def is_connected : bool
        return server_is_connected(_server)
    def tick : void
        server_tick(_server)
    def send ( data : uint8?; size:int ) : bool
        return server_send(_server, data, size)
    def abstract onConnect : void
    def abstract onDisconnect : void
    def abstract onData ( buf : uint8?; size : int ) : void
    def abstract onError ( msg : string; code : int ) : void
    def abstract onLog ( msg : string ) : void

