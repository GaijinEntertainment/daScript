//! the HttpRequest handed to a WebSocket upgrade authorizer is
//! borrowed and cannot escape into a global (expect 30187)
options gen2
expect 30187
require dashv/dashv_boost
var escaped : HttpRequest?
class UpgradeBorrowProbe : HvWebServer {
    def override onInit {
        WEBSOCKET_UPGRADE(1000) <| @(var request : HttpRequest?; _ticket : WebSocketAdmission) {
            escaped = request
        }
    }
}
[export]
def main {
    var server = new UpgradeBorrowProbe()
    server->init(0)
    server->cleanup()
    unsafe { delete server }
}
