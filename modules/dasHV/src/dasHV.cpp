#include "daScript/misc/platform.h"

#include <future>

#include "../../../src/builtin/module_builtin_rtti.h"

#include "dasHV.h"

IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketClient,hv::WebSocketClient)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketServer,hv::WebSocketServer)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketChannel,hv::WebSocketChannel)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpMessage,HttpMessage)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpRequest,HttpRequest)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpResponse,HttpResponse)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpContext,hv::HttpContext)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpResponseWriter,hv::HttpResponseWriter)

namespace das {

class Enumeration_ws_opcode : public das::Enumeration {
public:
    Enumeration_ws_opcode() : das::Enumeration("ws_opcode") {
        external = true;
        cppName = "ws_opcode";
        baseType = (das::Type) das::ToBasicType< das::underlying_type< ws_opcode >::type >::type;
		addIEx("WS_OPCODE_CONTINUE",    "WS_OPCODE_CONTINUE",   int64_t(ws_opcode::WS_OPCODE_CONTINUE), das::LineInfo());
		addIEx("WS_OPCODE_TEXT",        "WS_OPCODE_TEXT",       int64_t(ws_opcode::WS_OPCODE_TEXT), das::LineInfo());
		addIEx("WS_OPCODE_BINARY",      "WS_OPCODE_BINARY",     int64_t(ws_opcode::WS_OPCODE_BINARY), das::LineInfo());
		addIEx("WS_OPCODE_CLOSE",       "WS_OPCODE_CLOSE",      int64_t(ws_opcode::WS_OPCODE_CLOSE), das::LineInfo());
		addIEx("WS_OPCODE_PING",        "WS_OPCODE_PING",       int64_t(ws_opcode::WS_OPCODE_PING), das::LineInfo());
		addIEx("WS_OPCODE_PONG",        "WS_OPCODE_PONG",       int64_t(ws_opcode::WS_OPCODE_PONG), das::LineInfo());
	}
};

class Enumeration_ws_session_type : public das::Enumeration {
public:
    Enumeration_ws_session_type() : das::Enumeration("ws_session_type") {
        external = true;
        cppName = "ws_session_type";
        baseType = (das::Type) das::ToBasicType< das::underlying_type< ws_session_type >::type >::type;
		addIEx("WS_CLIENT", "WS_CLIENT",    int64_t(ws_session_type::WS_CLIENT), das::LineInfo());
		addIEx("WS_SERVER", "WS_SERVER",    int64_t(ws_session_type::WS_SERVER), das::LineInfo());
	}
};

class Enumeration_http_method : public das::Enumeration {
public:
    Enumeration_http_method() : das::Enumeration("http_method") {
        external = true;
        cppName = "http_method";
        baseType = (das::Type) das::ToBasicType< das::underlying_type< http_method >::type >::type;
        addIEx("DELETE",        "HTTP_DELETE",       int64_t(http_method::HTTP_DELETE),      das::LineInfo());
        addIEx("GET",           "HTTP_GET",          int64_t(http_method::HTTP_GET),         das::LineInfo());
        addIEx("HEAD",          "HTTP_HEAD",         int64_t(http_method::HTTP_HEAD),        das::LineInfo());
        addIEx("POST",          "HTTP_POST",         int64_t(http_method::HTTP_POST),        das::LineInfo());
        addIEx("PUT",           "HTTP_PUT",          int64_t(http_method::HTTP_PUT),         das::LineInfo());
        addIEx("CONNECT",       "HTTP_CONNECT",      int64_t(http_method::HTTP_CONNECT),     das::LineInfo());
        addIEx("OPTIONS",       "HTTP_OPTIONS",      int64_t(http_method::HTTP_OPTIONS),     das::LineInfo());
        addIEx("TRACE",         "HTTP_TRACE",        int64_t(http_method::HTTP_TRACE),       das::LineInfo());
        addIEx("COPY",          "HTTP_COPY",         int64_t(http_method::HTTP_COPY),        das::LineInfo());
        addIEx("LOCK",          "HTTP_LOCK",         int64_t(http_method::HTTP_LOCK),        das::LineInfo());
        addIEx("MKCOL",         "HTTP_MKCOL",        int64_t(http_method::HTTP_MKCOL),       das::LineInfo());
        addIEx("MOVE",          "HTTP_MOVE",         int64_t(http_method::HTTP_MOVE),        das::LineInfo());
        addIEx("PROPFIND",      "HTTP_PROPFIND",     int64_t(http_method::HTTP_PROPFIND),    das::LineInfo());
        addIEx("PROPPATCH",     "HTTP_PROPPATCH",    int64_t(http_method::HTTP_PROPPATCH),   das::LineInfo());
        addIEx("SEARCH",        "HTTP_SEARCH",       int64_t(http_method::HTTP_SEARCH),      das::LineInfo());
        addIEx("UNLOCK",        "HTTP_UNLOCK",       int64_t(http_method::HTTP_UNLOCK),      das::LineInfo());
        addIEx("BIND",          "HTTP_BIND",         int64_t(http_method::HTTP_BIND),        das::LineInfo());
        addIEx("REBIND",        "HTTP_REBIND",       int64_t(http_method::HTTP_REBIND),      das::LineInfo());
        addIEx("UNBIND",        "HTTP_UNBIND",       int64_t(http_method::HTTP_UNBIND),      das::LineInfo());
        addIEx("ACL",           "HTTP_ACL",          int64_t(http_method::HTTP_ACL),         das::LineInfo());
        addIEx("REPORT",        "HTTP_REPORT",       int64_t(http_method::HTTP_REPORT),      das::LineInfo());
        addIEx("MKACTIVITY",    "HTTP_MKACTIVITY",   int64_t(http_method::HTTP_MKACTIVITY),  das::LineInfo());
        addIEx("CHECKOUT",      "HTTP_CHECKOUT",     int64_t(http_method::HTTP_CHECKOUT),    das::LineInfo());
        addIEx("MERGE",         "HTTP_MERGE",        int64_t(http_method::HTTP_MERGE),       das::LineInfo());
        addIEx("MSEARCH",       "HTTP_MSEARCH",      int64_t(http_method::HTTP_SEARCH),      das::LineInfo());
        addIEx("NOTIFY",        "HTTP_NOTIFY",       int64_t(http_method::HTTP_NOTIFY),      das::LineInfo());
        addIEx("SUBSCRIBE",     "HTTP_SUBSCRIBE",    int64_t(http_method::HTTP_SUBSCRIBE),   das::LineInfo());
        addIEx("UNSUBSCRIBE",   "HTTP_UNSUBSCRIBE",  int64_t(http_method::HTTP_UNSUBSCRIBE), das::LineInfo());
        addIEx("PATCH",         "HTTP_PATCH",        int64_t(http_method::HTTP_PATCH),       das::LineInfo());
        addIEx("PURGE",         "HTTP_PURGE",        int64_t(http_method::HTTP_PURGE),       das::LineInfo());
        addIEx("MKCALENDAR",    "HTTP_MKCALENDAR",   int64_t(http_method::HTTP_MKCALENDAR),  das::LineInfo());
        addIEx("LINK",          "HTTP_LINK",         int64_t(http_method::HTTP_LINK),        das::LineInfo());
        addIEx("UNLINK",        "HTTP_UNLINK",       int64_t(http_method::HTTP_UNLINK),      das::LineInfo());
        addIEx("SOURCE",        "HTTP_SOURCE",       int64_t(http_method::HTTP_SOURCE),      das::LineInfo());

	}
};

class Enumeration_http_status : public das::Enumeration {
public:
    Enumeration_http_status() : das::Enumeration("http_status") {
        external = true;
        cppName = "http_status";
        baseType = (das::Type) das::ToBasicType< das::underlying_type< http_status >::type >::type;
        addIEx("CONTINUE",                          "HTTP_STATUS_CONTINUE",                         int64_t(http_status::HTTP_STATUS_CONTINUE), das::LineInfo());
        addIEx("SWITCHING_PROTOCOLS",               "HTTP_STATUS_SWITCHING_PROTOCOLS",              int64_t(http_status::HTTP_STATUS_SWITCHING_PROTOCOLS),  das::LineInfo());
        addIEx("PROCESSING",                        "HTTP_STATUS_PROCESSING",                       int64_t(http_status::HTTP_STATUS_PROCESSING),   das::LineInfo());
        addIEx("OK",                                "HTTP_STATUS_OK",                               int64_t(http_status::HTTP_STATUS_OK),   das::LineInfo());
        addIEx("CREATED",                           "HTTP_STATUS_CREATED",                          int64_t(http_status::HTTP_STATUS_CREATED),  das::LineInfo());
        addIEx("ACCEPTED",                          "HTTP_STATUS_ACCEPTED",                         int64_t(http_status::HTTP_STATUS_ACCEPTED), das::LineInfo());
        addIEx("NON_AUTHORITATIVE_INFORMATION",     "HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION",    int64_t(http_status::HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION),    das::LineInfo());
        addIEx("NO_CONTENT",                        "HTTP_STATUS_NO_CONTENT",                       int64_t(http_status::HTTP_STATUS_NO_CONTENT),   das::LineInfo());
        addIEx("RESET_CONTENT",                     "HTTP_STATUS_RESET_CONTENT",                    int64_t(http_status::HTTP_STATUS_RESET_CONTENT),    das::LineInfo());
        addIEx("PARTIAL_CONTENT",                   "HTTP_STATUS_PARTIAL_CONTENT",                  int64_t(http_status::HTTP_STATUS_PARTIAL_CONTENT),  das::LineInfo());
        addIEx("MULTI_STATUS",                      "HTTP_STATUS_MULTI_STATUS",                     int64_t(http_status::HTTP_STATUS_MULTI_STATUS), das::LineInfo());
        addIEx("ALREADY_REPORTED",                  "HTTP_STATUS_ALREADY_REPORTED",                 int64_t(http_status::HTTP_STATUS_ALREADY_REPORTED), das::LineInfo());
        addIEx("IM_USED",                           "HTTP_STATUS_IM_USED",                          int64_t(http_status::HTTP_STATUS_IM_USED),  das::LineInfo());
        addIEx("MULTIPLE_CHOICES",                  "HTTP_STATUS_MULTIPLE_CHOICES",                 int64_t(http_status::HTTP_STATUS_MULTIPLE_CHOICES), das::LineInfo());
        addIEx("MOVED_PERMANENTLY",                 "HTTP_STATUS_MOVED_PERMANENTLY",                int64_t(http_status::HTTP_STATUS_MOVED_PERMANENTLY),    das::LineInfo());
        addIEx("FOUND",                             "HTTP_STATUS_FOUND",                            int64_t(http_status::HTTP_STATUS_FOUND),    das::LineInfo());
        addIEx("SEE_OTHER",                         "HTTP_STATUS_SEE_OTHER",                        int64_t(http_status::HTTP_STATUS_SEE_OTHER),    das::LineInfo());
        addIEx("NOT_MODIFIED",                      "HTTP_STATUS_NOT_MODIFIED",                     int64_t(http_status::HTTP_STATUS_NOT_MODIFIED), das::LineInfo());
        addIEx("USE_PROXY",                         "HTTP_STATUS_USE_PROXY",                        int64_t(http_status::HTTP_STATUS_USE_PROXY),    das::LineInfo());
        addIEx("TEMPORARY_REDIRECT",                "HTTP_STATUS_TEMPORARY_REDIRECT",               int64_t(http_status::HTTP_STATUS_TEMPORARY_REDIRECT),   das::LineInfo());
        addIEx("PERMANENT_REDIRECT",                "HTTP_STATUS_PERMANENT_REDIRECT",               int64_t(http_status::HTTP_STATUS_PERMANENT_REDIRECT),   das::LineInfo());
        addIEx("BAD_REQUEST",                       "HTTP_STATUS_BAD_REQUEST",                      int64_t(http_status::HTTP_STATUS_BAD_REQUEST),  das::LineInfo());
        addIEx("UNAUTHORIZED",                      "HTTP_STATUS_UNAUTHORIZED",                     int64_t(http_status::HTTP_STATUS_UNAUTHORIZED), das::LineInfo());
        addIEx("PAYMENT_REQUIRED",                  "HTTP_STATUS_PAYMENT_REQUIRED",                 int64_t(http_status::HTTP_STATUS_PAYMENT_REQUIRED), das::LineInfo());
        addIEx("FORBIDDEN",                         "HTTP_STATUS_FORBIDDEN",                        int64_t(http_status::HTTP_STATUS_FORBIDDEN),    das::LineInfo());
        addIEx("NOT_FOUND",                         "HTTP_STATUS_NOT_FOUND",                        int64_t(http_status::HTTP_STATUS_NOT_FOUND),    das::LineInfo());
        addIEx("METHOD_NOT_ALLOWED",                "HTTP_STATUS_METHOD_NOT_ALLOWED",               int64_t(http_status::HTTP_STATUS_METHOD_NOT_ALLOWED),   das::LineInfo());
        addIEx("NOT_ACCEPTABLE",                    "HTTP_STATUS_NOT_ACCEPTABLE",                   int64_t(http_status::HTTP_STATUS_NOT_ACCEPTABLE),   das::LineInfo());
        addIEx("PROXY_AUTHENTICATION_REQUIRED",     "HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED",    int64_t(http_status::HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED),    das::LineInfo());
        addIEx("REQUEST_TIMEOUT",                   "HTTP_STATUS_REQUEST_TIMEOUT",                  int64_t(http_status::HTTP_STATUS_REQUEST_TIMEOUT),  das::LineInfo());
        addIEx("CONFLICT",                          "HTTP_STATUS_CONFLICT",                         int64_t(http_status::HTTP_STATUS_CONFLICT), das::LineInfo());
        addIEx("GONE",                              "HTTP_STATUS_GONE",                             int64_t(http_status::HTTP_STATUS_GONE), das::LineInfo());
        addIEx("LENGTH_REQUIRED",                   "HTTP_STATUS_LENGTH_REQUIRED",                  int64_t(http_status::HTTP_STATUS_LENGTH_REQUIRED),  das::LineInfo());
        addIEx("PRECONDITION_FAILED",               "HTTP_STATUS_PRECONDITION_FAILED",              int64_t(http_status::HTTP_STATUS_PRECONDITION_FAILED),  das::LineInfo());
        addIEx("PAYLOAD_TOO_LARGE",                 "HTTP_STATUS_PAYLOAD_TOO_LARGE",                int64_t(http_status::HTTP_STATUS_PAYLOAD_TOO_LARGE),    das::LineInfo());
        addIEx("URI_TOO_LONG",                      "HTTP_STATUS_URI_TOO_LONG",                     int64_t(http_status::HTTP_STATUS_URI_TOO_LONG), das::LineInfo());
        addIEx("UNSUPPORTED_MEDIA_TYPE",            "HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE",           int64_t(http_status::HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE),   das::LineInfo());
        addIEx("RANGE_NOT_SATISFIABLE",             "HTTP_STATUS_RANGE_NOT_SATISFIABLE",            int64_t(http_status::HTTP_STATUS_RANGE_NOT_SATISFIABLE),    das::LineInfo());
        addIEx("EXPECTATION_FAILED",                "HTTP_STATUS_EXPECTATION_FAILED",               int64_t(http_status::HTTP_STATUS_EXPECTATION_FAILED),   das::LineInfo());
        addIEx("MISDIRECTED_REQUEST",               "HTTP_STATUS_MISDIRECTED_REQUEST",              int64_t(http_status::HTTP_STATUS_MISDIRECTED_REQUEST),  das::LineInfo());
        addIEx("UNPROCESSABLE_ENTITY",              "HTTP_STATUS_UNPROCESSABLE_ENTITY",             int64_t(http_status::HTTP_STATUS_UNPROCESSABLE_ENTITY), das::LineInfo());
        addIEx("LOCKED",                            "HTTP_STATUS_LOCKED",                           int64_t(http_status::HTTP_STATUS_LOCKED),   das::LineInfo());
        addIEx("FAILED_DEPENDENCY",                 "HTTP_STATUS_FAILED_DEPENDENCY",                int64_t(http_status::HTTP_STATUS_FAILED_DEPENDENCY),    das::LineInfo());
        addIEx("UPGRADE_REQUIRED",                  "HTTP_STATUS_UPGRADE_REQUIRED",                 int64_t(http_status::HTTP_STATUS_UPGRADE_REQUIRED), das::LineInfo());
        addIEx("PRECONDITION_REQUIRED",             "HTTP_STATUS_PRECONDITION_REQUIRED",            int64_t(http_status::HTTP_STATUS_PRECONDITION_REQUIRED),    das::LineInfo());
        addIEx("TOO_MANY_REQUESTS",                 "HTTP_STATUS_TOO_MANY_REQUESTS",                int64_t(http_status::HTTP_STATUS_TOO_MANY_REQUESTS),    das::LineInfo());
        addIEx("REQUEST_HEADER_FIELDS_TOO_LARGE",   "HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE",  int64_t(http_status::HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE),  das::LineInfo());
        addIEx("UNAVAILABLE_FOR_LEGAL_REASONS",     "HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS",    int64_t(http_status::HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS),    das::LineInfo());
        addIEx("INTERNAL_SERVER_ERROR",             "HTTP_STATUS_INTERNAL_SERVER_ERROR",            int64_t(http_status::HTTP_STATUS_INTERNAL_SERVER_ERROR),    das::LineInfo());
        addIEx("NOT_IMPLEMENTED",                   "HTTP_STATUS_NOT_IMPLEMENTED",                  int64_t(http_status::HTTP_STATUS_NOT_IMPLEMENTED),  das::LineInfo());
        addIEx("BAD_GATEWAY",                       "HTTP_STATUS_BAD_GATEWAY",                      int64_t(http_status::HTTP_STATUS_BAD_GATEWAY),  das::LineInfo());
        addIEx("SERVICE_UNAVAILABLE",               "HTTP_STATUS_SERVICE_UNAVAILABLE",              int64_t(http_status::HTTP_STATUS_SERVICE_UNAVAILABLE),  das::LineInfo());
        addIEx("GATEWAY_TIMEOUT",                   "HTTP_STATUS_GATEWAY_TIMEOUT",                  int64_t(http_status::HTTP_STATUS_GATEWAY_TIMEOUT),  das::LineInfo());
        addIEx("HTTP_VERSION_NOT_SUPPORTED",        "HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED",       int64_t(http_status::HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED),   das::LineInfo());
        addIEx("VARIANT_ALSO_NEGOTIATES",           "HTTP_STATUS_VARIANT_ALSO_NEGOTIATES",          int64_t(http_status::HTTP_STATUS_VARIANT_ALSO_NEGOTIATES),  das::LineInfo());
        addIEx("INSUFFICIENT_STORAGE",              "HTTP_STATUS_INSUFFICIENT_STORAGE",             int64_t(http_status::HTTP_STATUS_INSUFFICIENT_STORAGE), das::LineInfo());
        addIEx("LOOP_DETECTED",                     "HTTP_STATUS_LOOP_DETECTED",                    int64_t(http_status::HTTP_STATUS_LOOP_DETECTED),    das::LineInfo());
        addIEx("NOT_EXTENDED",                      "HTTP_STATUS_NOT_EXTENDED",                     int64_t(http_status::HTTP_STATUS_NOT_EXTENDED), das::LineInfo());
        addIEx("NETWORK_AUTHENTICATION_REQUIRED",   "HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED",  int64_t(http_status::HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED),  das::LineInfo());
	}
};

#include "dashv_gen.inc"

class WebSocketClient_Adapter : public hv::WebSocketClient, public HvWebSocketClient_Adapter {
public:
    WebSocketClient_Adapter ( char * pClass, const StructInfo * info, Context * ctx )
        : HvWebSocketClient_Adapter(info), classPtr(pClass), context(ctx) {
        onopen = [=]() {
            lock_guard<mutex> guard(lock);
            que.emplace_back([=](){
                onOpen();
            });
        };
        onclose = [=]() {
            lock_guard<mutex> guard(lock);
            que.emplace_back([=](){
                onClose();
            });
        };
        onmessage = [=]( const string & msg ) {
            lock_guard<mutex> guard(lock);
            que.emplace_back([=](){
                onMessage(msg);
            });
        };
    }
    void onOpen() {
        if ( auto fnOnOpen = get_onOpen(classPtr) ) {
            invoke_onOpen(context,fnOnOpen,classPtr);
        }
    }
    void onClose() {
        if ( auto fnOnClose = get_onClose(classPtr) ) {
            invoke_onClose(context,fnOnClose,classPtr);
        }
    }
    void onMessage ( const string & msg ) {
        if ( auto fnOnMessage = get_onMessage(classPtr) ) {
            invoke_onMessage(context,fnOnMessage,classPtr,(char *)msg.c_str());
        }
    }
    void tick() {
        vector<function<void()>> q;
        {
            lock_guard<mutex> guard(lock);
            swap(q, que);
        }
        for ( auto & ev : q ) ev();
    }
protected:
    void *      classPtr;
    Context *   context;
    mutex       lock;
    vector<function<void()>>    que;
};

Handle<hv::WebSocketClient> makeWebSocketClient ( const void * pClass, const StructInfo * info, Context * context ) {
    auto adapter = new WebSocketClient_Adapter((char *)pClass,info,context);
    shared_ptr<hv::WebSocketClient> sp(adapter);
    return HandleRegistry<hv::WebSocketClient>::instance().acquire(sp);
}

int das_wsc_open ( Handle<hv::WebSocketClient> h, const char* url ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return -1;
    return p->open(url ? url : "");
}

int das_wsc_send ( Handle<hv::WebSocketClient> h, const char* msg ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return -1;
    return p->send(msg ? msg : "");
}

int das_wsc_send_buf ( Handle<hv::WebSocketClient> h, const char* msg, int32_t len, ws_opcode opcode ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return -1;
    if ( len < 0 ) return -1;
    if ( !msg && len != 0 ) return -1;
    return p->send(msg ? msg : "", len, opcode);
}

int das_wsc_close ( Handle<hv::WebSocketClient> h ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return -1;
    return p->close();
}

bool das_wsc_is_connected ( Handle<hv::WebSocketClient> h ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return false;
    return p->isConnected();
}

void das_wsc_tick ( Handle<hv::WebSocketClient> h ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return;
    ((WebSocketClient_Adapter *) p.get())->tick();
}


struct HttpMessageAnnotation : ManagedStructureAnnotation<HttpMessage> {
    HttpMessageAnnotation(ModuleLibrary & ml)
        : ManagedStructureAnnotation ("HttpMessage", ml, "HttpMessage") {
    }
};

struct HttpRequestAnnotation : ManagedStructureAnnotation<HttpRequest> {
    HttpRequestAnnotation(ModuleLibrary & ml)
        : ManagedStructureAnnotation ("HttpRequest", ml, "HttpRequest") {
        addField<DAS_BIND_MANAGED_FIELD(method)>("method");
        addField<DAS_BIND_MANAGED_FIELD(url)>("url");
        addField<DAS_BIND_MANAGED_FIELD(scheme)>("scheme");
        addField<DAS_BIND_MANAGED_FIELD(host)>("host");
        addField<DAS_BIND_MANAGED_FIELD(port)>("port");
        addField<DAS_BIND_MANAGED_FIELD(path)>("path");
        addField<DAS_BIND_MANAGED_FIELD(timeout)>("timeout");
        addField<DAS_BIND_MANAGED_FIELD(connect_timeout)>("connect_timeout");
        addField<DAS_BIND_MANAGED_FIELD(body)>("body");
        from("HttpMessage");
    }
};

struct HttpResponseAnnotation : ManagedStructureAnnotation<HttpResponse> {
    HttpResponseAnnotation(ModuleLibrary & ml)
        : ManagedStructureAnnotation ("HttpResponse", ml, "HttpResponse") {
        addField<DAS_BIND_MANAGED_FIELD(body)>("body");
        addField<DAS_BIND_MANAGED_FIELD(status_code)>("status_code");
        addField<DAS_BIND_MANAGED_FIELD(content)>("content");
        from("HttpMessage");
    }
};

struct HttpContextAnnotation : ManagedStructureAnnotation<hv::HttpContext> {
    HttpContextAnnotation(ModuleLibrary & ml)
        : ManagedStructureAnnotation ("HttpContext", ml, "hv::HttpContext") {
    }
};

struct HttpResponseWriterAnnotation : ManagedStructureAnnotation<hv::HttpResponseWriter> {
    HttpResponseWriterAnnotation(ModuleLibrary & ml)
        : ManagedStructureAnnotation ("HttpResponseWriter", ml, "hv::HttpResponseWriter") {
    }
};

class WebServer_Adapter : public hv::WebSocketServer, public HvWebServer_Adapter {
public:
    WebServer_Adapter ( char * pClass, const StructInfo * info, Context * ctx )
        : HvWebServer_Adapter(info), classPtr(pClass), context(ctx) {
        registerWebSocketService(&service);
        registerHttpService(&router);
        service.onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& url) {
            Handle<hv::WebSocketChannel> h;
            {
                lock_guard<mutex> cguard(channel_lock);
                h = HandleRegistry<hv::WebSocketChannel>::instance().acquire(channel);
                channel_handles[channel.get()] = h;
            }
            std::string urlStr = url ? url->url : std::string();
            lock_guard<mutex> guard(lock);
            que.emplace_back([this, h, urlStr](){
                onWsOpen(h, urlStr);
            });
        };
        service.onclose = [this](const WebSocketChannelPtr& channel) {
            // Keep the channel→handle mapping here until the queued close
            // actually runs: if the server is torn down before `tick()` drains
            // `que`, the adapter destructor's cleanup loop below is our only
            // chance to release the handle. Eagerly erasing the map entry
            // would leak it in that race.
            hv::WebSocketChannel * raw = channel.get();
            lock_guard<mutex> guard(lock);
            que.emplace_back([this, raw](){
                Handle<hv::WebSocketChannel> h;
                {
                    lock_guard<mutex> cguard(channel_lock);
                    auto it = channel_handles.find(raw);
                    if ( it != channel_handles.end() ) {
                        h = it->second;
                        channel_handles.erase(it);
                    }
                }
                if ( !h ) return;
                onWsClose(h);
                HandleRegistry<hv::WebSocketChannel>::instance().release(h);
            });
        };
        service.onmessage = [this](const WebSocketChannelPtr& channel, const std::string& msg) {
            Handle<hv::WebSocketChannel> h;
            {
                lock_guard<mutex> cguard(channel_lock);
                auto it = channel_handles.find(channel.get());
                if ( it != channel_handles.end() ) h = it->second;
            }
            if ( !h ) return;
            lock_guard<mutex> guard(lock);
            que.emplace_back([this, h, msg](){
                onWsMessage(h, msg);
            });
        };
    }
    ~WebServer_Adapter() {
        lock_guard<mutex> cguard(channel_lock);
        for ( auto & kv : channel_handles ) {
            HandleRegistry<hv::WebSocketChannel>::instance().release(kv.second);
        }
        channel_handles.clear();
    }
    void onWsOpen ( Handle<hv::WebSocketChannel> h, const std::string & url ) {
        lock_guard<mutex> guard(lock);
        if ( auto fnOnOpen = get_onWsOpen(classPtr) ) {
            invoke_onWsOpen(context,fnOnOpen,classPtr,h,(char *)url.c_str());
        }
    }
    void onWsClose ( Handle<hv::WebSocketChannel> h ) {
        lock_guard<mutex> guard(lock);
        if ( auto fnOnClose = get_onWsClose(classPtr) ) {
            invoke_onWsClose(context,fnOnClose,classPtr,h);
        }
    }
    void onWsMessage ( Handle<hv::WebSocketChannel> h, const std::string & msg ) {
        lock_guard<mutex> guard(lock);
        if ( auto fnOnMessage = get_onWsMessage(classPtr) ) {
            invoke_onWsMessage(context,fnOnMessage,classPtr,h,(char *)msg.c_str());
        }
    }
    void tick() {
        vector<function<void()>> q;
        {
            lock_guard<mutex> guard(lock);
            swap(q, que);
        }
        for ( auto & ev : q ) ev();
        lock_guard<mutex> guard(lock);
        if ( auto fnOnTick = get_onTick(classPtr) ) {
            invoke_onTick(context,fnOnTick,classPtr);
        }
    }
    void GET ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.GET(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void POST ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.POST(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void PUT ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.PUT(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void DEL ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Delete(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void PATCH ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.PATCH(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void HEAD ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.HEAD(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void ANY ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Any(relative_path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void STATIC ( const char * path, const char * dir ) {
        lock_guard<mutex> guard(lock);
        router.Static(path, dir);
    }
    void ALLOW_CORS ( ) {
        lock_guard<mutex> guard(lock);
        router.AllowCORS();
    }
    void SET_DOCUMENT_ROOT ( const char * dir ) {
        lock_guard<mutex> guard(lock);
        router.document_root = dir ? dir : ".";
    }
    void SET_HOME_PAGE ( const char * filename ) {
        lock_guard<mutex> guard(lock);
        router.home_page = filename ? filename : "index.html";
    }
    void SET_INDEX_OF ( const char * dir ) {
        lock_guard<mutex> guard(lock);
        router.index_of = dir ? dir : "";
    }
    void SET_ERROR_PAGE ( const char * filename ) {
        lock_guard<mutex> guard(lock);
        router.error_page = filename ? filename : "";
    }
    void SSE ( const char * path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Any(path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            {
                lock_guard<mutex> guard(lock);
                que.emplace_back([&](){
                    p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
                });
            }
            return f.get();
        });
    }
    void release_writer ( hv::HttpResponseWriter * w ) {
        lock_guard<mutex> guard(writer_lock);
        active_writers.erase(w);
    }
protected:
    HttpService router;
    WebSocketService service;
    void *      classPtr;
    Context *   context;
    mutex       lock;
    vector<function<void()>>    que;
    mutex       writer_lock;
    map<hv::HttpResponseWriter*, HttpResponseWriterPtr>  active_writers;
    mutex       channel_lock;
    map<hv::WebSocketChannel*, Handle<hv::WebSocketChannel>>  channel_handles;
};

string getDasRoot ( void );

static WebServer_Adapter * lookup_server ( Handle<hv::WebSocketServer> h ) {
    auto p = HandleRegistry<hv::WebSocketServer>::instance().lookup(h);
    return (WebServer_Adapter *) p.get();
}

Handle<hv::WebSocketServer> makeWebSocketServer ( int port, int httpsPort, const char * pathToCert, const void * pClass, const StructInfo * info, Context * context, LineInfoArg * at ) {
    auto adapter = new WebServer_Adapter((char *)pClass,info,context);
    adapter->port = port;
    adapter->https_port = httpsPort;
    if ( httpsPort ) {
        hssl_ctx_init_param_t param;
        memset(&param, 0, sizeof(param));
        string crt_root = pathToCert ? pathToCert : getDasRoot() + "/modules/dasHV/cert";
        auto crt_file = crt_root + "/server.crt";
        auto key_file = crt_root + "/server.key";
        param.crt_file = crt_file.c_str();
        param.key_file = key_file.c_str();
        param.endpoint = HSSL_SERVER;
        if (hssl_ctx_init(&param) == NULL) {
            context->throw_error_at(at, "libHV: hssl_ctx_init failed! Please check the certificate files `%s` and `%s`.", crt_file.c_str(), key_file.c_str());
        }
    }
    shared_ptr<hv::WebSocketServer> sp(adapter);
    return HandleRegistry<hv::WebSocketServer>::instance().acquire(sp);
}

int das_wss_send ( Handle<hv::WebSocketChannel> h, const char * msg, ws_opcode opcode, bool fin ) {
    auto p = HandleRegistry<hv::WebSocketChannel>::instance().lookup(h);
    if ( !p ) return -1;
    return p->send(string(msg ? msg : ""), opcode, fin);
}

int das_wss_send_buf ( Handle<hv::WebSocketChannel> h, const char * buf, int32_t len, ws_opcode opcode, bool fin ) {
    auto p = HandleRegistry<hv::WebSocketChannel>::instance().lookup(h);
    if ( !p ) return -1;
    if ( len < 0 ) return -1;
    if ( !buf && len != 0 ) return -1;
    return p->send(buf, len, opcode, fin);
}

int das_wss_send_fragment ( Handle<hv::WebSocketChannel> h, const char * buf, int32_t len, int32_t fragment, ws_opcode opcode ) {
    auto p = HandleRegistry<hv::WebSocketChannel>::instance().lookup(h);
    if ( !p ) return -1;
    if ( len < 0 || fragment < 0 ) return -1;
    if ( !buf && len != 0 ) return -1;
    return p->send(buf, len, fragment, opcode);
}

int das_wss_start ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return -1;
    return adapter->start();
}

void das_wss_tick ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return;
    adapter->tick();
}

int das_wss_stop ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return -1;
    return adapter->stop();
}

void das_wss_get ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->GET(url, lmb, context, at);
}

void das_wss_post ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->POST(url, lmb, context, at);
}

void das_wss_put ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->PUT(url, lmb, context, at);
}

void das_wss_del ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->DEL(url, lmb, context, at);
}

void das_wss_patch ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->PATCH(url, lmb, context, at);
}

void das_wss_head ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->HEAD(url, lmb, context, at);
}

void das_wss_any ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->ANY(url, lmb, context, at);
}

void das_wss_static ( Handle<hv::WebSocketServer> h, const char * path, const char * dir ) {
    if ( auto adapter = lookup_server(h) ) adapter->STATIC(path ? path : "/", dir ? dir : ".");
}

void das_wss_allow_cors ( Handle<hv::WebSocketServer> h ) {
    if ( auto adapter = lookup_server(h) ) adapter->ALLOW_CORS();
}

void das_wss_sse ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->SSE(url, lmb, context, at);
}

// HttpResponseWriter operations

int das_writer_end_headers ( hv::HttpResponseWriter * w, const char * key, const char * value ) {
    if ( !w ) return -1;
    return w->EndHeaders(key ? key : "", value ? value : "");
}

int das_writer_sse_event ( hv::HttpResponseWriter * w, const char * data, const char * event ) {
    if ( !w ) return -1;
    return w->SSEvent(data ? data : "", event ? event : "message");
}

int das_writer_write_chunked ( hv::HttpResponseWriter * w, const char * data, int32_t len ) {
    if ( !w ) return -1;
    return w->WriteChunked(data ? data : "", len);
}

int das_writer_end ( hv::HttpResponseWriter * w ) {
    if ( !w ) return -1;
    return w->End();
}

int das_writer_close ( hv::HttpResponseWriter * w ) {
    if ( !w ) return -1;
    return w->close(true);
}

void das_writer_release ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w ) {
    if ( !w ) return;
    if ( auto adapter = lookup_server(h) ) adapter->release_writer(w);
}

void das_wss_set_document_root ( Handle<hv::WebSocketServer> h, const char * dir ) {
    if ( auto adapter = lookup_server(h) ) adapter->SET_DOCUMENT_ROOT(dir);
}

void das_wss_set_home_page ( Handle<hv::WebSocketServer> h, const char * filename ) {
    if ( auto adapter = lookup_server(h) ) adapter->SET_HOME_PAGE(filename);
}

void das_wss_set_index_of ( Handle<hv::WebSocketServer> h, const char * dir ) {
    if ( auto adapter = lookup_server(h) ) adapter->SET_INDEX_OF(dir);
}

void das_wss_set_error_page ( Handle<hv::WebSocketServer> h, const char * filename ) {
    if ( auto adapter = lookup_server(h) ) adapter->SET_ERROR_PAGE(filename);
}

http_status das_resp_string ( HttpResponse * resp, const char * msg, http_status status ) {
    resp->content_type = TEXT_PLAIN;
    resp->body = msg ? msg : "";
    return status;
}

http_status das_resp_json ( HttpResponse * resp, const char * json_str, http_status status ) {
    resp->content_type = APPLICATION_JSON;
    resp->body = json_str ? json_str : "{}";
    return status;
}

http_status das_resp_redirect ( HttpResponse * resp, const char * location, http_status status ) {
    return (http_status)resp->Redirect(location ? location : "/", status);
}

http_status das_resp_file ( HttpResponse * resp, const char * filepath ) {
    return (http_status)resp->File(filepath ? filepath : "");
}

http_status das_resp_data ( HttpResponse * resp, const char * data, int32_t len, http_status status ) {
    resp->content_type = APPLICATION_OCTET_STREAM;
    resp->body.assign(data, len);
    return status;
}

void das_resp_set_header ( HttpResponse * resp, const char * key, const char * value ) {
    resp->SetHeader(key ? key : "", value ? value : "");
}

void das_resp_set_content_type ( HttpResponse * resp, const char * ct ) {
    resp->SetHeader("Content-Type", ct ? ct : "text/plain");
}

void das_httpr_each_param ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    for ( auto & kv : req->query_params ) {
        vec4f args[2];
        args[0] = cast<const char *>::from(kv.first.c_str());
        args[1] = cast<const char *>::from(kv.second.c_str());
        context->invoke(block, args, nullptr, at);
    }
}

void das_httpr_set_header ( HttpRequest * req, const char * key, const char * value ) {
    if ( !req ) return;
    req->SetHeader(key ? key : "", value ? value : value);
}

http_headers das_req_table_to_headers ( const TTable<char *,char *> & tab) {
    http_headers headers;
    char ** keys = (char **)tab.keys;
    char ** values = (char **)tab.data;
    auto * hashes = (TableHashKey *)tab.hashes;
    for ( uint32_t i=0; i!=tab.capacity; ++i ) {
        if ( hashes[i]!=HASH_EMPTY64 && hashes[i]!=HASH_KILLED64 ) {
            headers[keys[i]] = values[i];
        }
    }
    return headers;
}

void das_req_GET ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::get(url ? url : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_GET_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::get(url ? url : "", headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_POST ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::post(url ? url : "", text ? text : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_POST_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::post(url ? url : "", text ? text : "",headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_POST_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from,
        const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    using namespace requests;
    Request req(new HttpRequest);
    req->method = HTTP_POST;
    req->url = url ? url : "";
    req->headers =das_req_table_to_headers(tab);
    req->body = text ? text : "";
    char ** keys = (char **)from.keys;
    char ** values = (char **)from.data;
    auto * hashes = (TableHashKey *)from.hashes;
    for ( uint32_t i=0; i!=from.capacity; ++i ) {
        if ( hashes[i]!=HASH_EMPTY64 && hashes[i]!=HASH_KILLED64 ) {
            hv::FormData data;
            auto value = values[i];
            if ( value != nullptr ) {
                if (*value == '@') {
                    data.filename = value+1;
                } else {
                    data.content = value;
                }
            }
            req->form[keys[i] ? keys[i] : ""] = data;
        }
    }
    auto resp = request(req);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// PUT
void das_req_PUT ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::put(url ? url : "", text ? text : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_PUT_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::put(url ? url : "", text ? text : "",headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_PUT_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from,
        const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    using namespace requests;
    Request req(new HttpRequest);
    req->method = HTTP_PUT;
    req->url = url ? url : "";
    req->headers = das_req_table_to_headers(tab);
    req->body = text ? text : "";
    char ** keys = (char **)from.keys;
    char ** values = (char **)from.data;
    auto * hashes = (TableHashKey *)from.hashes;
    for ( uint32_t i=0; i!=from.capacity; ++i ) {
        if ( hashes[i]!=HASH_EMPTY64 && hashes[i]!=HASH_KILLED64 ) {
            hv::FormData data;
            auto value = values[i];
            if ( value != nullptr ) {
                if (*value == '@') {
                    data.filename = value+1;
                } else {
                    data.content = value;
                }
            }
            req->form[keys[i] ? keys[i] : ""] = data;
        }
    }
    auto resp = request(req);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// PATCH
void das_req_PATCH ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::patch(url ? url : "", text ? text : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_PATCH_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::patch(url ? url : "", text ? text : "",headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_PATCH_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from,
        const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    using namespace requests;
    Request req(new HttpRequest);
    req->method = HTTP_PATCH;
    req->url = url ? url : "";
    req->headers = das_req_table_to_headers(tab);
    req->body = text ? text : "";
    char ** keys = (char **)from.keys;
    char ** values = (char **)from.data;
    auto * hashes = (TableHashKey *)from.hashes;
    for ( uint32_t i=0; i!=from.capacity; ++i ) {
        if ( hashes[i]!=HASH_EMPTY64 && hashes[i]!=HASH_KILLED64 ) {
            hv::FormData data;
            auto value = values[i];
            if ( value != nullptr ) {
                if (*value == '@') {
                    data.filename = value+1;
                } else {
                    data.content = value;
                }
            }
            req->form[keys[i] ? keys[i] : ""] = data;
        }
    }
    auto resp = request(req);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// DELETE
void das_req_DELETE ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::Delete(url ? url : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_DELETE_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::Delete(url ? url : "", headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// HEAD
void das_req_HEAD ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto resp = requests::head(url ? url : "");
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

void das_req_HEAD_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    auto headers = das_req_table_to_headers(tab);
    auto resp = requests::head(url ? url : "", headers);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// Generic request
void das_req_REQUEST ( HttpRequest * req, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    auto preq = std::make_shared<HttpRequest>(*req);
    auto resp = requests::request(preq);
    das_invoke<void>::invoke<HttpResponse*>(context,at,block,resp.get());
}

// Streaming request — invokes on_body per chunk
void das_req_REQUEST_CB ( HttpRequest * req, const TBlock<void,const uint8_t*,int32_t> & on_body,
        const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    auto preq = std::make_shared<HttpRequest>(*req);
    preq->http_cb = [&on_body, context, at]
            (HttpMessage* , http_parser_state state, const char* data, size_t size) {
        if ( state == HP_BODY && data && size ) {
            das_invoke<void>::invoke<const uint8_t*,int32_t>(context, at, on_body,
                (const uint8_t*)data, int32_t(size));
        }
    };
    auto resp = requests::request(preq);
    das_invoke<void>::invoke<HttpResponse*>(context,at,on_complete,resp.get());
}

void das_req_REQUEST_CB_S ( HttpRequest * req, const TBlock<void,const char*> & on_body,
        const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    auto preq = std::make_shared<HttpRequest>(*req);
    preq->http_cb = [&on_body, context, at]
            (HttpMessage* , http_parser_state state, const char* data, size_t size) {
        if ( state == HP_BODY && data && size ) {
            string tmp(data, size);
            das_invoke<void>::invoke<const char*>(context, at, on_body, tmp.c_str());
        }
    };
    auto resp = requests::request(preq);
    das_invoke<void>::invoke<HttpResponse*>(context,at,on_complete,resp.get());
}

// Response/message header access
char * das_httpm_get_header ( HttpMessage * msg, const char * key, Context * context, LineInfoArg * at ) {
    if ( !msg ) return nullptr;
    auto val = msg->GetHeader(key ? key : "");
    if ( val.empty() ) return nullptr;
    return context->allocateString(val, at);
}

void das_httpm_each_header ( HttpMessage * msg, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at ) {
    if ( !msg ) return;
    for ( auto & kv : msg->headers ) {
        das_invoke<void>::invoke<const char *,const char *>(context,at,block,kv.first.c_str(),kv.second.c_str());
    }
    // libhv stores cookies separately from headers — emit them too,
    // matching DumpHeaders() which serialises Set-Cookie / Cookie lines.
    const char * cookie_field = msg->type == HTTP_RESPONSE ? "Set-Cookie" : "Cookie";
    for ( auto & cookie : msg->cookies ) {
        auto dumped = cookie.dump();
        das_invoke<void>::invoke<const char *,const char *>(context,at,block,cookie_field,dumped.c_str());
    }
}

// Response status message
char * das_httpr_status_message ( HttpResponse * resp, Context * context, LineInfoArg * at ) {
    if ( !resp ) return nullptr;
    auto msg = resp->status_message();
    if ( !msg ) return nullptr;
    return context->allocateString(msg, uint32_t(strlen(msg)), at);
}

// Request configuration
void das_httpr_set_basic_auth ( HttpRequest * req, const char * username, const char * password ) {
    if ( !req ) return;
    req->SetBasicAuth(username ? username : "", password ? password : "");
}

void das_httpr_set_bearer_token_auth ( HttpRequest * req, const char * token ) {
    if ( !req ) return;
    req->SetBearerTokenAuth(token ? token : "");
}

void das_httpr_set_timeout ( HttpRequest * req, int sec ) {
    if ( !req ) return;
    req->SetTimeout(sec);
}

void das_httpr_set_connect_timeout ( HttpRequest * req, int sec ) {
    if ( !req ) return;
    req->SetConnectTimeout(sec);
}

void das_httpr_allow_redirect ( HttpRequest * req, bool on ) {
    if ( !req ) return;
    req->AllowRedirect(on);
}

void das_httpr_set_param ( HttpRequest * req, const char * key, const char * value ) {
    if ( !req ) return;
    req->SetParam(key ? key : "", std::string(value ? value : ""));
}

char * das_httpr_get_param ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at ) {
    if ( !req ) return nullptr;
    auto val = req->GetParam(key ? key : "");
    if ( val.empty() ) return nullptr;
    return context->allocateString(val, at);
}

void das_httpr_set_content_type ( HttpRequest * req, const char * ct ) {
    if ( !req ) return;
    req->SetContentType(ct ? ct : "");
}

// Cookies — HttpRequest* overloads (client building + server reading via addr(req))
void das_httpreq_add_cookie ( HttpRequest * req, const char * name, const char * value ) {
    if ( !req ) return;
    HttpCookie cookie;
    cookie.name = name ? name : "";
    cookie.value = value ? value : "";
    req->AddCookie(cookie);
}

void das_httpreq_add_cookie_ex ( HttpRequest * req, const char * name, const char * value,
        const char * domain, const char * path, int max_age, bool secure, bool httponly ) {
    if ( !req ) return;
    HttpCookie cookie;
    cookie.name = name ? name : "";
    cookie.value = value ? value : "";
    if ( domain ) cookie.domain = domain;
    if ( path ) cookie.path = path;
    cookie.max_age = max_age;
    cookie.secure = secure;
    cookie.httponly = httponly;
    req->AddCookie(cookie);
}

char * das_httpreq_get_cookie ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at ) {
    if ( !req ) return nullptr;
    auto & cookie = req->GetCookie(name ? name : "");
    if ( cookie.name.empty() && cookie.value.empty() ) return nullptr;
    return context->allocateString(cookie.value, at);
}

void das_httpreq_each_cookie ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    for ( auto & cookie : req->cookies ) {
        vec4f args[2];
        args[0] = cast<const char *>::from(cookie.name.c_str());
        args[1] = cast<const char *>::from(cookie.value.c_str());
        context->invoke(block, args, nullptr, at);
    }
}

// Cookies — HttpResponse& overloads (server setting cookies on response)
void das_httpresp_add_cookie ( HttpResponse * resp, const char * name, const char * value ) {
    HttpCookie cookie;
    cookie.name = name ? name : "";
    cookie.value = value ? value : "";
    resp->AddCookie(cookie);
}

void das_httpresp_add_cookie_ex ( HttpResponse * resp, const char * name, const char * value,
        const char * domain, const char * path, int max_age, bool secure, bool httponly ) {
    HttpCookie cookie;
    cookie.name = name ? name : "";
    cookie.value = value ? value : "";
    if ( domain ) cookie.domain = domain;
    if ( path ) cookie.path = path;
    cookie.max_age = max_age;
    cookie.secure = secure;
    cookie.httponly = httponly;
    resp->AddCookie(cookie);
}

// Cookies — HttpResponse* overloads (client reading Set-Cookie from response)
char * das_httpresp_get_cookie ( HttpResponse * resp, const char * name, Context * context, LineInfoArg * at ) {
    if ( !resp ) return nullptr;
    auto & cookie = resp->GetCookie(name ? name : "");
    if ( cookie.name.empty() && cookie.value.empty() ) return nullptr;
    return context->allocateString(cookie.value, at);
}

void das_httpresp_each_cookie ( HttpResponse * resp, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at ) {
    if ( !resp ) return;
    for ( auto & cookie : resp->cookies ) {
        vec4f args[2];
        args[0] = cast<const char *>::from(cookie.name.c_str());
        args[1] = cast<const char *>::from(cookie.value.c_str());
        context->invoke(block, args, nullptr, at);
    }
}

int das_http_response_content_length ( const HttpResponse & resp ) {
    return int(resp.content_length);
}

// Form data - client side (request building)
void das_httpr_set_form_data ( HttpRequest * req, const char * name, const char * value ) {
    if ( !req ) return;
    req->SetFormData(name ? name : "", std::string(value ? value : ""));
}

void das_httpr_set_form_file ( HttpRequest * req, const char * name, const char * filepath ) {
    if ( !req ) return;
    req->SetFormFile(name ? name : "", filepath ? filepath : "");
}

// Form data - server side (reading from received request via HttpRequest*)
char * das_httpreq_get_form_data ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at ) {
    if ( !req ) return nullptr;
    auto val = req->GetFormData(name ? name : "");
    if ( val.empty() ) return nullptr;
    return context->allocateString(val, at);
}

int das_httpreq_save_form_file ( HttpRequest * req, const char * name, const char * path ) {
    if ( !req ) return 400;
    return req->SaveFormFile(name ? name : "", path ? path : "");
}

void das_httpreq_each_form_field ( HttpRequest * req, const TBlock<void,const char *,const char *,const char *> & block, Context * context, LineInfoArg * at ) {
    if ( !req ) return;
    auto & form = req->GetForm();
    for ( auto & kv : form ) {
        vec4f args[3];
        args[0] = cast<const char *>::from(kv.first.c_str());
        args[1] = cast<const char *>::from(kv.second.content.c_str());
        args[2] = cast<const char *>::from(kv.second.filename.c_str());
        context->invoke(block, args, nullptr, at);
    }
}

// URL-encoded form data
void das_httpr_set_url_encoded ( HttpRequest * req, const char * key, const char * value ) {
    if ( !req ) return;
    req->SetUrlEncoded(key ? key : "", std::string(value ? value : ""));
}

char * das_httpreq_get_url_encoded ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at ) {
    if ( !req ) return nullptr;
    auto val = req->GetUrlEncoded(key ? key : "");
    if ( val.empty() ) return nullptr;
    return context->allocateString(val, at);
}

class Module_HV : public Module {
public:
    Module_HV() : Module("dashv") {
        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        lib.addModule(Module::require("rtti_core"));
        addEnumeration(new Enumeration_ws_opcode());
        addEnumeration(new Enumeration_ws_session_type());
        addEnumeration(new Enumeration_http_method());
        addEnumeration(new Enumeration_http_status());
        // client — handle-backed
        addHandleAnnotation<hv::WebSocketClient>(this, lib, "WebSocketClient",
            "destroy_web_socket_client", "das::Handle<hv::WebSocketClient>");
        addExtern<DAS_BIND_FUN(makeWebSocketClient)> (*this, lib, "make_web_socket_client",
            SideEffects::worstDefault, "makeWebSocketClient")
                ->args({"class","info","context"});
        addExtern<DAS_BIND_FUN(das_wsc_open)> (*this, lib, "open",
            SideEffects::worstDefault, "das_wsc_open")
                ->args({"self","url"});
        addExtern<DAS_BIND_FUN(das_wsc_send)> (*this, lib, "send",
            SideEffects::worstDefault, "das_wsc_send")
                ->args({"self","msg"});
        addExtern<DAS_BIND_FUN(das_wsc_send_buf)> (*this, lib, "send",
            SideEffects::worstDefault, "das_wsc_send_buf")
                ->args({"self","msg","len","opcode"});
        addExtern<DAS_BIND_FUN(das_wsc_close)>(*this, lib, "close",
            SideEffects::worstDefault, "das_wsc_close")
                ->args({"self"});
        addExtern<DAS_BIND_FUN(das_wsc_is_connected)>(*this, lib, "is_connected",
            SideEffects::worstDefault,"das_wsc_is_connected")
	            ->args({"self"});
        addExtern<DAS_BIND_FUN(das_wsc_tick)>(*this, lib, "tick",
            SideEffects::worstDefault,"das_wsc_tick")
	            ->args({"self"});
        // server — handle-backed
        addHandleAnnotation<hv::WebSocketServer>(this, lib, "WebSocketServer",
            "destroy_web_socket_server", "das::Handle<hv::WebSocketServer>");
        addHandleAnnotation<hv::WebSocketChannel>(this, lib, "WebSocketChannel",
            "", "das::Handle<hv::WebSocketChannel>");
        addAnnotation(new HttpMessageAnnotation(lib));
        addAnnotation(new HttpRequestAnnotation(lib));
        addAnnotation(new HttpResponseAnnotation(lib));
        addExtern<DAS_BIND_FUN(das_http_response_content_length)>(*this, lib, ".`content_length",
            SideEffects::none, "das_http_response_content_length")
                ->args({"self"});
        addAnnotation(new HttpContextAnnotation(lib));
        addExtern<DAS_BIND_FUN(makeWebSocketServer)> (*this, lib, "make_web_socket_server",
            SideEffects::worstDefault, "makeWebSocketServer")
                ->args({"port","https_port","pathToCert","class","info","context","at"});
        addExtern<DAS_BIND_FUN(das_wss_send)> (*this, lib, "send",
            SideEffects::worstDefault, "das_wss_send")
                ->args({"channel","msg","opcode","fin"});
        addExtern<DAS_BIND_FUN(das_wss_send_buf)> (*this, lib, "send",
            SideEffects::worstDefault, "das_wss_send_buf")
                ->args({"channel","msg","len","opcode","fin"});
        addExtern<DAS_BIND_FUN(das_wss_send_fragment)> (*this, lib, "send",
            SideEffects::worstDefault, "das_wss_send_fragment")
                ->args({"channel","msg","len","fragment","opcode"});
        addExtern<DAS_BIND_FUN(das_wss_start)> (*this, lib, "start",
            SideEffects::worstDefault, "das_wss_start")
                ->args({"server"});
        addExtern<DAS_BIND_FUN(das_wss_tick)> (*this, lib, "tick",
            SideEffects::worstDefault, "das_wss_tick")
                ->args({"server"});
        addExtern<DAS_BIND_FUN(das_wss_stop)> (*this, lib, "stop",
            SideEffects::worstDefault, "das_wss_stop")
                ->args({"server"});
        addExtern<DAS_BIND_FUN(das_wss_get)> (*this, lib, "GET",
            SideEffects::worstDefault, "das_wss_get")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_post)> (*this, lib, "POST",
            SideEffects::worstDefault, "das_wss_post")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_put)> (*this, lib, "PUT",
            SideEffects::worstDefault, "das_wss_put")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_del)> (*this, lib, "DELETE",
            SideEffects::worstDefault, "das_wss_del")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_patch)> (*this, lib, "PATCH",
            SideEffects::worstDefault, "das_wss_patch")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_head)> (*this, lib, "HEAD",
            SideEffects::worstDefault, "das_wss_head")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_any)> (*this, lib, "ANY",
            SideEffects::worstDefault, "das_wss_any")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_static)> (*this, lib, "STATIC",
            SideEffects::worstDefault, "das_wss_static")
                ->args({"server","path","dir"});
        addExtern<DAS_BIND_FUN(das_wss_allow_cors)> (*this, lib, "allow_cors",
            SideEffects::worstDefault, "das_wss_allow_cors")
                ->args({"server"});
        addExtern<DAS_BIND_FUN(das_wss_set_document_root)> (*this, lib, "set_document_root",
            SideEffects::worstDefault, "das_wss_set_document_root")
                ->args({"server","dir"});
        addExtern<DAS_BIND_FUN(das_wss_set_home_page)> (*this, lib, "set_home_page",
            SideEffects::worstDefault, "das_wss_set_home_page")
                ->args({"server","filename"});
        addExtern<DAS_BIND_FUN(das_wss_set_index_of)> (*this, lib, "set_index_of",
            SideEffects::worstDefault, "das_wss_set_index_of")
                ->args({"server","dir"});
        addExtern<DAS_BIND_FUN(das_wss_set_error_page)> (*this, lib, "set_error_page",
            SideEffects::worstDefault, "das_wss_set_error_page")
                ->args({"server","filename"});
        // response
        auto http_status_enum = findEnum("http_status");
        addExtern<DAS_BIND_FUN(das_resp_string)> (*this, lib, "TEXT_PLAIN",
            SideEffects::worstDefault, "das_resp_string")
                ->args({"response","text","status"})
                ->arg_init(2,new ExprConstEnumeration(int(HTTP_STATUS_OK), http_status_enum->makeEnumType()));
        addExtern<DAS_BIND_FUN(das_resp_json)> (*this, lib, "JSON",
            SideEffects::worstDefault, "das_resp_json")
                ->args({"response","json_string","status"})
                ->arg_init(2,new ExprConstEnumeration(int(HTTP_STATUS_OK), http_status_enum->makeEnumType()));
        addExtern<DAS_BIND_FUN(das_resp_redirect)> (*this, lib, "REDIRECT",
            SideEffects::worstDefault, "das_resp_redirect")
                ->args({"response","location","status"});
        addExtern<DAS_BIND_FUN(das_resp_file)> (*this, lib, "SERVE_FILE",
            SideEffects::worstDefault, "das_resp_file")
                ->args({"response","filepath"});
        addExtern<DAS_BIND_FUN(das_resp_data)> (*this, lib, "DATA",
            SideEffects::worstDefault, "das_resp_data")
                ->args({"response","data","length","status"})
                ->arg_init(3,new ExprConstEnumeration(int(HTTP_STATUS_OK), http_status_enum->makeEnumType()));
        addExtern<DAS_BIND_FUN(das_resp_set_header)> (*this, lib, "set_header",
            SideEffects::worstDefault, "das_resp_set_header")
                ->args({"response","key","value"});
        addExtern<DAS_BIND_FUN(das_resp_set_content_type)> (*this, lib, "set_content_type",
            SideEffects::worstDefault, "das_resp_set_content_type")
                ->args({"response","content_type"});
        // request
        addExtern<DAS_BIND_FUN(das_httpr_set_header)> (*this, lib, "set_header",
            SideEffects::worstDefault, "das_httpr_set_header")
                ->args({"request","key","value"});
        // requests
        addExtern<DAS_BIND_FUN(das_req_GET)> (*this, lib, "GET",
            SideEffects::worstDefault, "das_req_GET")
                ->args({"url","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_GET_H)> (*this, lib, "GET",
            SideEffects::worstDefault, "das_req_GET_H")
                ->args({"url","headers","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_POST)> (*this, lib, "POST",
            SideEffects::worstDefault, "das_req_POST")
                ->args({"url","text","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_POST_H)> (*this, lib, "POST",
            SideEffects::worstDefault, "das_req_POST_H")
                ->args({"url","text","headers","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_POST_HF)> (*this, lib, "POST",
            SideEffects::worstDefault, "das_req_POST_HF")
                ->args({"url","text","headers","from","block","context","at"});
        // PUT
        addExtern<DAS_BIND_FUN(das_req_PUT)> (*this, lib, "PUT",
            SideEffects::worstDefault, "das_req_PUT")
                ->args({"url","text","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_PUT_H)> (*this, lib, "PUT",
            SideEffects::worstDefault, "das_req_PUT_H")
                ->args({"url","text","headers","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_PUT_HF)> (*this, lib, "PUT",
            SideEffects::worstDefault, "das_req_PUT_HF")
                ->args({"url","text","headers","from","block","context","at"});
        // PATCH
        addExtern<DAS_BIND_FUN(das_req_PATCH)> (*this, lib, "PATCH",
            SideEffects::worstDefault, "das_req_PATCH")
                ->args({"url","text","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_PATCH_H)> (*this, lib, "PATCH",
            SideEffects::worstDefault, "das_req_PATCH_H")
                ->args({"url","text","headers","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_PATCH_HF)> (*this, lib, "PATCH",
            SideEffects::worstDefault, "das_req_PATCH_HF")
                ->args({"url","text","headers","from","block","context","at"});
        // DELETE
        addExtern<DAS_BIND_FUN(das_req_DELETE)> (*this, lib, "DELETE",
            SideEffects::worstDefault, "das_req_DELETE")
                ->args({"url","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_DELETE_H)> (*this, lib, "DELETE",
            SideEffects::worstDefault, "das_req_DELETE_H")
                ->args({"url","headers","block","context","at"});
        // HEAD
        addExtern<DAS_BIND_FUN(das_req_HEAD)> (*this, lib, "HEAD",
            SideEffects::worstDefault, "das_req_HEAD")
                ->args({"url","block","context","at"});
        addExtern<DAS_BIND_FUN(das_req_HEAD_H)> (*this, lib, "HEAD",
            SideEffects::worstDefault, "das_req_HEAD_H")
                ->args({"url","headers","block","context","at"});
        // Generic request
        addExtern<DAS_BIND_FUN(das_req_REQUEST)> (*this, lib, "request",
            SideEffects::worstDefault, "das_req_REQUEST")
                ->args({"request","block","context","at"});
        // Response/message header access
        addExtern<DAS_BIND_FUN(das_httpm_get_header)> (*this, lib, "get_header",
            SideEffects::worstDefault, "das_httpm_get_header")
                ->args({"message","key","context","at"});
        addExtern<DAS_BIND_FUN(das_httpm_each_header)> (*this, lib, "each_header",
            SideEffects::worstDefault, "das_httpm_each_header")
                ->args({"message","block","context","at"});
        // Response status message
        addExtern<DAS_BIND_FUN(das_httpr_status_message)> (*this, lib, "status_message",
            SideEffects::worstDefault, "das_httpr_status_message")
                ->args({"response","context","at"});
        // Request configuration
        addExtern<DAS_BIND_FUN(das_httpr_set_basic_auth)> (*this, lib, "set_basic_auth",
            SideEffects::worstDefault, "das_httpr_set_basic_auth")
                ->args({"request","username","password"});
        addExtern<DAS_BIND_FUN(das_httpr_set_bearer_token_auth)> (*this, lib, "set_bearer_token_auth",
            SideEffects::worstDefault, "das_httpr_set_bearer_token_auth")
                ->args({"request","token"});
        addExtern<DAS_BIND_FUN(das_httpr_set_timeout)> (*this, lib, "set_timeout",
            SideEffects::worstDefault, "das_httpr_set_timeout")
                ->args({"request","seconds"});
        addExtern<DAS_BIND_FUN(das_httpr_set_connect_timeout)> (*this, lib, "set_connect_timeout",
            SideEffects::worstDefault, "das_httpr_set_connect_timeout")
                ->args({"request","seconds"});
        addExtern<DAS_BIND_FUN(das_httpr_allow_redirect)> (*this, lib, "allow_redirect",
            SideEffects::worstDefault, "das_httpr_allow_redirect")
                ->args({"request","on"});
        addExtern<DAS_BIND_FUN(das_httpr_set_param)> (*this, lib, "set_param",
            SideEffects::worstDefault, "das_httpr_set_param")
                ->args({"request","key","value"});
        addExtern<DAS_BIND_FUN(das_httpr_get_param)> (*this, lib, "get_param",
            SideEffects::worstDefault, "das_httpr_get_param")
                ->args({"request","key","context","at"});
        addExtern<DAS_BIND_FUN(das_httpr_each_param)> (*this, lib, "each_param",
            SideEffects::worstDefault, "das_httpr_each_param")
                ->args({"request","block","context","at"});
        addExtern<DAS_BIND_FUN(das_httpr_set_content_type)> (*this, lib, "set_content_type",
            SideEffects::worstDefault, "das_httpr_set_content_type")
                ->args({"request","content_type"});
        // Cookies — HttpRequest* overloads
        addExtern<DAS_BIND_FUN(das_httpreq_add_cookie)> (*this, lib, "add_cookie",
            SideEffects::worstDefault, "das_httpreq_add_cookie")
                ->args({"request","name","value"});
        addExtern<DAS_BIND_FUN(das_httpreq_add_cookie_ex)> (*this, lib, "add_cookie",
            SideEffects::worstDefault, "das_httpreq_add_cookie_ex")
                ->args({"request","name","value","domain","path","max_age","secure","httponly"});
        addExtern<DAS_BIND_FUN(das_httpreq_get_cookie)> (*this, lib, "get_cookie",
            SideEffects::worstDefault, "das_httpreq_get_cookie")
                ->args({"request","name","context","at"});
        addExtern<DAS_BIND_FUN(das_httpreq_each_cookie)> (*this, lib, "each_cookie",
            SideEffects::worstDefault, "das_httpreq_each_cookie")
                ->args({"request","block","context","at"});
        // Cookies — HttpResponse overloads
        addExtern<DAS_BIND_FUN(das_httpresp_add_cookie)> (*this, lib, "add_cookie",
            SideEffects::worstDefault, "das_httpresp_add_cookie")
                ->args({"response","name","value"});
        addExtern<DAS_BIND_FUN(das_httpresp_add_cookie_ex)> (*this, lib, "add_cookie",
            SideEffects::worstDefault, "das_httpresp_add_cookie_ex")
                ->args({"response","name","value","domain","path","max_age","secure","httponly"});
        addExtern<DAS_BIND_FUN(das_httpresp_get_cookie)> (*this, lib, "get_cookie",
            SideEffects::worstDefault, "das_httpresp_get_cookie")
                ->args({"response","name","context","at"});
        addExtern<DAS_BIND_FUN(das_httpresp_each_cookie)> (*this, lib, "each_cookie",
            SideEffects::worstDefault, "das_httpresp_each_cookie")
                ->args({"response","block","context","at"});
        // Form data - client side
        addExtern<DAS_BIND_FUN(das_httpr_set_form_data)> (*this, lib, "set_form_data",
            SideEffects::worstDefault, "das_httpr_set_form_data")
                ->args({"request","name","value"});
        addExtern<DAS_BIND_FUN(das_httpr_set_form_file)> (*this, lib, "set_form_file",
            SideEffects::worstDefault, "das_httpr_set_form_file")
                ->args({"request","name","filepath"});
        // Form data - server side (HttpRequest*)
        addExtern<DAS_BIND_FUN(das_httpreq_get_form_data)> (*this, lib, "get_form_data",
            SideEffects::worstDefault, "das_httpreq_get_form_data")
                ->args({"request","name","context","at"});
        addExtern<DAS_BIND_FUN(das_httpreq_save_form_file)> (*this, lib, "save_form_file",
            SideEffects::worstDefault, "das_httpreq_save_form_file")
                ->args({"request","name","path"});
        addExtern<DAS_BIND_FUN(das_httpreq_each_form_field)> (*this, lib, "each_form_field",
            SideEffects::worstDefault, "das_httpreq_each_form_field")
                ->args({"request","block","context","at"});
        // URL-encoded form data
        addExtern<DAS_BIND_FUN(das_httpr_set_url_encoded)> (*this, lib, "set_url_encoded",
            SideEffects::worstDefault, "das_httpr_set_url_encoded")
                ->args({"request","key","value"});
        addExtern<DAS_BIND_FUN(das_httpreq_get_url_encoded)> (*this, lib, "get_url_encoded",
            SideEffects::worstDefault, "das_httpreq_get_url_encoded")
                ->args({"request","key","context","at"});
        // Streaming request (client-side)
        addExtern<DAS_BIND_FUN(das_req_REQUEST_CB)> (*this, lib, "request_cb",
            SideEffects::worstDefault, "das_req_REQUEST_CB")
                ->args({"request","on_body","on_complete","context","at"});
        addExtern<DAS_BIND_FUN(das_req_REQUEST_CB_S)> (*this, lib, "request_cb",
            SideEffects::worstDefault, "das_req_REQUEST_CB_S")
                ->args({"request","on_body","on_complete","context","at"});
        // Server-side SSE
        addAnnotation(new HttpResponseWriterAnnotation(lib));
        addExtern<DAS_BIND_FUN(das_wss_sse)> (*this, lib, "SSE",
            SideEffects::worstDefault, "das_wss_sse")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        // HttpResponseWriter operations
        addExtern<DAS_BIND_FUN(das_writer_end_headers)> (*this, lib, "end_headers",
            SideEffects::worstDefault, "das_writer_end_headers")
                ->args({"writer","key","value"});
        addExtern<DAS_BIND_FUN(das_writer_sse_event)> (*this, lib, "sse_event",
            SideEffects::worstDefault, "das_writer_sse_event")
                ->args({"writer","data","event"});
        addExtern<DAS_BIND_FUN(das_writer_write_chunked)> (*this, lib, "write_chunked",
            SideEffects::worstDefault, "das_writer_write_chunked")
                ->args({"writer","data","length"});
        addExtern<DAS_BIND_FUN(das_writer_end)> (*this, lib, "end_response",
            SideEffects::worstDefault, "das_writer_end")
                ->args({"writer"});
        addExtern<DAS_BIND_FUN(das_writer_close)> (*this, lib, "close_writer",
            SideEffects::worstDefault, "das_writer_close")
                ->args({"writer"});
        addExtern<DAS_BIND_FUN(das_writer_release)> (*this, lib, "release_writer",
            SideEffects::worstDefault, "das_writer_release")
                ->args({"server","writer"});

    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasHV/src/aot_hv.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_HV,Module_HV);
}

REGISTER_MODULE_IN_NAMESPACE(Module_HV,das);

