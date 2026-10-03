#include "daScript/misc/platform.h"

#include <future>
#include <atomic>
#include <cstdlib>
#include <chrono>

#include "../../../src/builtin/module_builtin_rtti.h"

#include "dasHV.h"
#include "http_request_snapshot.h"
#include "bounded_file_pump.h"
#include "daScript/misc/sysos.h"
#include <sys/stat.h>

#include <hv/hlog.h>
#include <hv/hasync.h>
#include <hv/hsocket.h>
#include <hv/herr.h>
#include <hv/hurl.h>

IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketClient,hv::WebSocketClient)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketServer,hv::WebSocketServer)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketChannel,hv::WebSocketChannel)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpMessage,HttpMessage)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpRequest,HttpRequest)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpResponse,HttpResponse)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpContext,hv::HttpContext)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(HttpResponseWriter,hv::HttpResponseWriter)
IMPLEMENT_EXTERNAL_TYPE_FACTORY(WebSocketAdmission,das::WebSocketAdmission)

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
            connected.store(true);
            lock_guard<mutex> guard(lock);
            que.emplace_back([=](){
                onOpen();
            });
        };
        onclose = [=]() {
            connected.store(false);
            lock_guard<mutex> guard(lock);
            que.emplace_back([=](){
                onClose();
            });
        };
        onmessage = [=]( const string & msg ) {
            const auto message_opcode = opcode();
            lock_guard<mutex> guard(lock);
            que.emplace_back([=]() {
                onMessageFrame(msg, message_opcode);
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
    void onMessageFrame ( const string & msg, ws_opcode opcode ) {
        if ( auto fnOnMessageFrame = get_onMessageFrame(classPtr) ) {
            invoke_onMessageFrame(context, fnOnMessageFrame, classPtr,
                (char *)msg.data(), static_cast<int32_t>(msg.size()), opcode);
        }
    }
    ~WebSocketClient_Adapter() {
        // Join the loop thread while our members are still alive. Base
        // destructors run after the derived members are destroyed, so relying
        // on ~TcpClientTmpl's stop() lets a late onclose/onmessage lambda
        // push into a freed `que` from the loop thread (observed as an access
        // violation in hio_close under reload churn).
        stop(true);
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
    atomic<bool>    connected{false};
public:
    bool isConnected() { return connected.load(); }
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
    const auto & loop = p->loop();
    if ( !loop || !loop->isRunning() ) return p->close();
    auto client = p.get();
    loop->runInLoop([client](){ client->close(); });
    return 0;
}

void das_hv_set_log_file ( const char * path ) {
    if ( path && *path ) hlog_set_file(path);
}

bool das_wsc_is_connected ( Handle<hv::WebSocketClient> h ) {
    auto p = HandleRegistry<hv::WebSocketClient>::instance().lookup(h);
    if ( !p ) return false;
    return ((WebSocketClient_Adapter *) p.get())->isConnected();
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

struct WebSocketAdmission {
    HttpRequestPtr request;
    HttpResponseWriterPtr writer;
    Handle<hv::WebSocketServer> owner;
    Handle<WebSocketAdmission> handle;
    atomic<bool> open{true};
    atomic<bool> decided{false};
    std::chrono::steady_clock::time_point deadline;
};

struct ConnectionPending {
    size_t messages = 0, bytes = 0;
};

struct WriterRegistration {
    HttpResponseWriterPtr writer;
    shared_ptr<atomic<bool>> open;
    bool head_only = false;
};

class WebServer_Adapter : public hv::WebSocketServer, public HvWebServer_Adapter {
public:
    // modules/dasHV/ARCHITECTURE.md#websocket-admission-capacity
    WebServer_Adapter ( char * pClass, const StructInfo * info, Context * ctx )
        : HvWebServer_Adapter(info), classPtr(pClass), context(ctx) {
        registerWebSocketService(&service);
        registerHttpService(&router);
        service.onopen = [this](const WebSocketChannelPtr& channel, const HttpRequestPtr& url) {
            Handle<hv::WebSocketChannel> h;
            {
                lock_guard<mutex> cguard(channel_lock);
                auto admission = admissions.find(url.get());
                if (admission != admissions.end()) {
                    admission->second->open.store(false);
                    HandleRegistry<WebSocketAdmission>::instance().release(admission->second->handle);
                    admissions.erase(admission);
                }
                if (!max_pending_events || channel_handles.size() + admissions.size() < max_pending_events) {
                    h = HandleRegistry<hv::WebSocketChannel>::instance().acquire(channel);
                    channel_handles[channel.get()] = h;
                }
            }
            if (!h) { channel->close(); return; }
            if (max_connection_write_bytes) channel->setMaxWriteBufsize(uint32_t(max_connection_write_bytes));
            auto request = url ? snapshot_received_http_request(*url) : make_shared<HttpRequest>();
            if (!enqueue([this, h, request](){ onWsOpen(h, request.get()); }, request_size(*request))) {
                {
                    lock_guard<mutex> cguard(channel_lock);
                    channel_handles.erase(channel.get());
                }
                HandleRegistry<hv::WebSocketChannel>::instance().release(h);
                channel->close();
            }
        };
        service.onclose = [this](const WebSocketChannelPtr& channel) {
            // Keep the channel→handle mapping here until the queued close
            // actually runs: if the server is torn down before `tick()` drains
            // `que`, the adapter destructor's cleanup loop below is our only
            // chance to release the handle. Eagerly erasing the map entry
            // would leak it in that race.
            hv::WebSocketChannel * raw = channel.get();
            {
                lock_guard<mutex> cguard(channel_lock);
                if (channel_handles.find(raw) == channel_handles.end()) return;
            }
            enqueue([this, raw](){
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
            }, 0, true);
        };
        service.onmessage = [this](const WebSocketChannelPtr& channel, const std::string& msg) {
            Handle<hv::WebSocketChannel> h;
            {
                lock_guard<mutex> cguard(channel_lock);
                auto it = channel_handles.find(channel.get());
                if ( it != channel_handles.end() ) h = it->second;
            }
            if ( !h ) return;
            const auto opcode = channel->opcode;
            if (!enqueue([this, h, msg, opcode](){ onWsMessageFrame(h, msg, opcode); }, msg.size(), false, channel.get())) {
                channel->close();
            }
        };
    }
    ~WebServer_Adapter() {
        alive.reset();
        // Same destruction-order guard as the client adapter: stop the server
        // threads while `que`/locks are still alive, or a late service
        // callback races our member destruction.
        stop();
        lock_guard<mutex> cguard(channel_lock);
        for ( auto & kv : channel_handles ) {
            HandleRegistry<hv::WebSocketChannel>::instance().release(kv.second);
        }
        channel_handles.clear();
        for (auto & admission : admissions) {
            admission.second->open.store(false);
            HandleRegistry<WebSocketAdmission>::instance().release(admission.second->handle);
        }
        admissions.clear();
    }
    void onWsOpen ( Handle<hv::WebSocketChannel> h, HttpRequest * request ) {
        if ( auto fn = get_onWsOpenRequest(classPtr) ) {
            invoke_onWsOpenRequest(context,fn,classPtr,h,request);
        } else if ( auto fn = get_onWsOpen(classPtr) ) {
            invoke_onWsOpen(context,fn,classPtr,h,(char *)request->url.c_str());
        }
    }
    void onWsClose ( Handle<hv::WebSocketChannel> h ) {
        if ( auto fnOnClose = get_onWsClose(classPtr) ) {
            invoke_onWsClose(context,fnOnClose,classPtr,h);
        }
    }
    void onWsMessageFrame(Handle<hv::WebSocketChannel> h, const std::string & msg, ws_opcode opcode) {
        if (auto fn = get_onWsMessageFrame(classPtr)) {
            invoke_onWsMessageFrame(context, fn, classPtr, h, (char *)msg.data(), int32_t(msg.size()), opcode);
        }
    }
    void tick() {
        vector<PendingEvent> q;
        {
            lock_guard<mutex> guard(lock);
            swap(q, que);
        }
        for (auto & event : q) {
            event.invoke();
            event.invoke = nullptr;
            if (!event.cleanup) {
                lock_guard<mutex> guard(lock);
                --pending_events;
                pending_bytes -= event.bytes;
                if (event.channel) {
                    auto found = connection_pending.find(event.channel);
                    if (found != connection_pending.end()) {
                        --found->second.messages;
                        found->second.bytes -= event.bytes;
                        if (!found->second.messages) connection_pending.erase(found);
                    }
                }
            }
        }
        // onTick runs user das code on the main thread and touches none of the
        // lock-protected state (`que`); holding `lock` here would block the libhv
        // worker thread from enqueueing requests for the whole onTick duration.
        if ( auto fnOnTick = get_onTick(classPtr) ) {
            invoke_onTick(context,fnOnTick,classPtr);
        }
    }
    // Non-blocking request handling. The lone libhv worker thread must never
    // block: enqueue the das invocation to `que` (run on the main/tick thread,
    // where the das context is valid) and return HTTP_STATUS_UNFINISHED so the
    // worker returns to its loop immediately. The das handler fills the response
    // on the tick thread, but the actual send is posted back to the connection's
    // own event loop (runInLoop) — sending from the tick thread would touch the
    // response/connection concurrently with libhv's teardown (HttpHandler dtor)
    // on the loop thread, a data race. ctx is captured by value, keeping
    // req/resp + connection alive until the send; no [&]-to-stack capture, so no
    // deadlock on stop() and no UAF.
    http_ctx_handler makeCtxHandler ( Lambda lmb, Context * context, LineInfoArg * at ) {
        return [this,context,at,lmb](const HttpContextPtr & ctx) -> int {
            // The server's worker event loop (persistent, owned by the server).
            // worker_threads defaults to 1, so this is the loop the connection
            // lives on — the send must run here, not on the tick thread.
            auto connLoop = this->loop();
            auto resp = make_shared<HttpResponse>(*ctx->response);
            auto req = snapshot_received_http_request(*ctx->request);
            if (!enqueue([context,at,lmb,ctx,connLoop,resp,req](){
                int st = das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(
                    context, at, lmb, req.get(), resp.get());
                resp->status_code = (http_status) st;
                if ( connLoop ) {
                    connLoop->runInLoop([ctx,resp](){ *ctx->response = *resp; ctx->send(); });
                } else {
                    *ctx->response = *resp;
                    ctx->send();
                }
            }, request_size(*req))) {
                return HTTP_STATUS_SERVICE_UNAVAILABLE;
            }
            return HTTP_STATUS_UNFINISHED;
        };
    }
    void GET ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.GET(relative_path, makeCtxHandler(lmb, context, at));
    }
    void POST ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.POST(relative_path, makeCtxHandler(lmb, context, at));
    }
    void PUT ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.PUT(relative_path, makeCtxHandler(lmb, context, at));
    }
    void DEL ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Delete(relative_path, makeCtxHandler(lmb, context, at));
    }
    void PATCH ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.PATCH(relative_path, makeCtxHandler(lmb, context, at));
    }
    void HEAD ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.HEAD(relative_path, makeCtxHandler(lmb, context, at));
    }
    void ANY ( const char * relative_path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Any(relative_path, makeCtxHandler(lmb, context, at));
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
    // SSE intentionally keeps the synchronous (blocking) form: its das handler
    // streams events for the duration of one call, which the buffered ctx->send()
    // async path does not model. Caveat: it therefore still parks the worker in
    // f.get(), so an in-flight SSE request can still deadlock stop()/teardown —
    // the deadlock fix above does NOT cover SSE. Not used by daslang-live.
    void SSE ( const char * path, Lambda lmb, Context * context, LineInfoArg * at ) {
        lock_guard<mutex> guard(lock);
        router.Any(path,[this,context,at,lmb](HttpRequest * req,HttpResponse * resp) -> int {
            promise<int> p;
            auto f = p.get_future();
            if (!enqueue([&](){
                p.set_value(das_invoke_lambda<int>::invoke<HttpRequest*,HttpResponse*>(context,at,lmb,req,resp));
            }, request_size(*req))) return HTTP_STATUS_SERVICE_UNAVAILABLE;
            return f.get();
        });
    }
    void release_writer ( hv::HttpResponseWriter * w ) {
        lock_guard<mutex> guard(writer_lock);
        auto it = active_writers.find(w);
        if (it != active_writers.end()) {
            it->second.open->store(false);
            active_writers.erase(it);
        }
    }
    WriterRegistration writer_registration(hv::HttpResponseWriter * w) {
        lock_guard<mutex> guard(writer_lock);
        auto it = active_writers.find(w);
        return it != active_writers.end() ? it->second : WriterRegistration();
    }
    void release_writer(hv::HttpResponseWriter * w, const shared_ptr<atomic<bool>> & token) {
        lock_guard<mutex> guard(writer_lock);
        auto it = active_writers.find(w);
        if (it != active_writers.end() && it->second.open == token) {
            token->store(false);
            active_writers.erase(it);
        }
    }
    bool is_writer_open ( hv::HttpResponseWriter * w ) {
        lock_guard<mutex> guard(writer_lock);
        auto it = active_writers.find(w);
        return it != active_writers.end() && it->second.open->load();
    }
    HttpResponseWriterPtr find_writer ( hv::HttpResponseWriter * w ) {
        lock_guard<mutex> guard(writer_lock);
        auto it = active_writers.find(w);
        return it != active_writers.end() ? it->second.writer : HttpResponseWriterPtr();
    }
    http_ctx_handler makeDeferredHandler(Lambda lmb, Context * context, LineInfoArg * at) {
        return [this,context,at,lmb](const HttpContextPtr & ctx) -> int {
            auto req = snapshot_received_http_request(*ctx->request);
            auto writer = ctx->writer;
            auto open = make_shared<atomic<bool>>(true);
            writer->onclose = [this, open, pointer = writer.get()](){ open->store(false); release_writer(pointer, open); };
            {
                lock_guard<mutex> wguard(writer_lock);
                if (max_pending_events && active_writers.size() >= max_pending_events) return HTTP_STATUS_SERVICE_UNAVAILABLE;
                active_writers[writer.get()] = {writer, open, req->method == HTTP_HEAD};
            }
            if (!enqueue([context,at,lmb,req,writer](){
                das_invoke_lambda<void>::invoke<HttpRequest*,hv::HttpResponseWriter*>(
                    context, at, lmb, req.get(), writer.get());
            }, request_size(*req))) {
                release_writer(writer.get());
                return HTTP_STATUS_SERVICE_UNAVAILABLE;
            }
            return HTTP_STATUS_UNFINISHED;
        };
    }
    void STREAM(const char * path, Lambda lmb, Context * context, LineInfoArg * at) {
        lock_guard<mutex> guard(lock);
        router.Any(path, makeDeferredHandler(lmb, context, at));
    }
    bool UPGRADE(int timeout_ms, Lambda lmb, Context * context, LineInfoArg * at) {
        if (started || timeout_ms < 1 || timeout_ms > 60000) return false;
        lock_guard<mutex> guard(lock);
        service.upgrade_timeout_ms = timeout_ms;
        service.onupgrade = [this,timeout_ms,lmb,context,at](const HttpContextPtr & ctx) -> int {
            auto admission = make_shared<WebSocketAdmission>();
            admission->request = ctx->request;
            admission->writer = ctx->writer;
            admission->owner = self_handle;
            admission->deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
            {
                lock_guard<mutex> guard(channel_lock);
                if (max_pending_events && channel_handles.size() + admissions.size() >= max_pending_events) return HTTP_STATUS_SERVICE_UNAVAILABLE;
                admission->handle = HandleRegistry<WebSocketAdmission>::instance().acquire(admission);
                admissions[ctx->request.get()] = admission;
            }
            const auto handle = admission->handle;
            auto key = ctx->request.get();
            ctx->writer->onclose = [this,key,handle](){ release_admission(key, handle); };
            auto request = snapshot_received_http_request(*ctx->request);
            if (!enqueue([handle,request,lmb,context,at](){
                if (HandleRegistry<WebSocketAdmission>::instance().is_alive(handle)) {
                    das_invoke_lambda<void>::invoke<HttpRequest*,Handle<WebSocketAdmission>>(
                        context, at, lmb, request.get(), handle);
                }
            }, request_size(*request))) {
                release_admission(key, handle);
                return HTTP_STATUS_SERVICE_UNAVAILABLE;
            }
            return HTTP_STATUS_UNFINISHED;
        };
        return true;
    }
    void release_admission(HttpRequest * key, Handle<WebSocketAdmission> handle) {
        lock_guard<mutex> guard(channel_lock);
        auto found = admissions.find(key);
        if (found != admissions.end() && found->second->handle == handle) {
            found->second->open.store(false);
            admissions.erase(found);
        }
        HandleRegistry<WebSocketAdmission>::instance().release(handle);
    }
    weak_ptr<int> lifetime() const { return alive; }
    Handle<hv::WebSocketServer> self_handle;
    bool set_access_log(bool enabled) {
        if (started) return false;
        router.enable_access_log = enabled;
        return true;
    }
    bool set_limits(int http_bytes, int ws_bytes, int event_count, int queue_bytes) {
        if (started || http_bytes <= 0 || ws_bytes <= 0 || event_count <= 0 || queue_bytes <= 0) return false;
        router.max_request_body_size = size_t(http_bytes);
        service.max_message_size = size_t(ws_bytes);
        max_pending_events = size_t(event_count);
        max_pending_bytes = size_t(queue_bytes);
        return true;
    }
    bool set_connection_limits(int messages, int bytes, int write_bytes) {
        if (started || messages <= 0 || bytes <= 0 || write_bytes <= 0) return false;
        max_connection_messages = size_t(messages);
        max_connection_bytes = size_t(bytes);
        max_connection_write_bytes = size_t(write_bytes);
        return true;
    }
    bool started = false;

protected:
    HttpService router;
    WebSocketService service;
    void *      classPtr;
    Context *   context;
    mutex       lock;
    struct PendingEvent {
        function<void()> invoke;
        size_t bytes;
        bool cleanup;
        hv::WebSocketChannel * channel;
    };
    vector<PendingEvent> que;
    size_t max_pending_events = 0, max_pending_bytes = 0;
    size_t pending_events = 0, pending_bytes = 0;
    size_t max_connection_messages = 0, max_connection_bytes = 0, max_connection_write_bytes = 0;
    map<hv::WebSocketChannel *, ConnectionPending> connection_pending;
    static size_t request_size(const HttpRequest & req) {
        size_t bytes = 256 + req.body.size() + req.url.size() * 2;
        for (const auto & header : req.headers) bytes += 128 + header.first.size() + header.second.size();
        for (const auto & cookie : req.cookies) bytes += 128 + cookie.name.size() + cookie.value.size();
        return bytes;
    }
    bool enqueue(function<void()> invoke, size_t bytes, bool cleanup = false, hv::WebSocketChannel * channel = nullptr) {
        lock_guard<mutex> guard(lock);
        if (!cleanup) {
            if ((max_pending_events && pending_events >= max_pending_events) ||
                (max_pending_bytes && (bytes > max_pending_bytes || pending_bytes > max_pending_bytes - bytes))) return false;
            if (channel) {
                const auto found = connection_pending.find(channel);
                const size_t count = found == connection_pending.end() ? 0 : found->second.messages;
                const size_t used = found == connection_pending.end() ? 0 : found->second.bytes;
                if ((max_connection_messages && count >= max_connection_messages) ||
                    (max_connection_bytes && (bytes > max_connection_bytes || used > max_connection_bytes - bytes))) return false;
                connection_pending[channel] = {count + 1, used + bytes};
            }
            ++pending_events;
            pending_bytes += bytes;
        }
        que.push_back(PendingEvent{std::move(invoke), bytes, cleanup, channel});
        return true;
    }
    mutex       writer_lock;
    map<hv::HttpResponseWriter*, WriterRegistration> active_writers;
    mutex       channel_lock;
    map<hv::WebSocketChannel*, Handle<hv::WebSocketChannel>>  channel_handles;
    map<HttpRequest*,shared_ptr<WebSocketAdmission>> admissions;
    shared_ptr<int> alive = make_shared<int>(0);
};

string getDasRoot ( void );

static WebServer_Adapter * lookup_server ( Handle<hv::WebSocketServer> h ) {
    auto p = HandleRegistry<hv::WebSocketServer>::instance().lookup(h);
    return (WebServer_Adapter *) p.get();
}

Handle<hv::WebSocketServer> makeWebSocketServer ( int port, int httpsPort, const char * pathToCert, const void * pClass, const StructInfo * info, Context * context, LineInfoArg * at ) {
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
    auto adapter = new WebServer_Adapter((char *)pClass,info,context);
    // libHV serves nothing for port 0 - it listens only above zero - so a server asked for any free
    // port learns one from a socket of its own and releases it. Resolved here rather than at start,
    // so bound_port answers before the event loop runs and a route still registers single-threaded.
    if ( port == 0 ) {
        int fd = Listen(0, adapter->host);
        if ( fd >= 0 ) {
            sockaddr_u addr;
            memset(&addr, 0, sizeof(addr));
            socklen_t len = sizeof(addr);
            if ( getsockname(fd, &addr.sa, &len) == 0 ) port = (int) sockaddr_port(&addr);
            closesocket(fd);
        }
    }
    adapter->port = port;
    adapter->https_port = httpsPort;
    shared_ptr<hv::WebSocketServer> sp(adapter);
    adapter->self_handle = HandleRegistry<hv::WebSocketServer>::instance().acquire(sp);
    return adapter->self_handle;
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

int das_wss_close_channel ( Handle<hv::WebSocketChannel> h ) {
    auto p = HandleRegistry<hv::WebSocketChannel>::instance().lookup(h);
    if ( !p ) return -1;
    return p->close();
}

bool das_wss_set_bind_host ( Handle<hv::WebSocketServer> h, const char * host ) {
    auto adapter = lookup_server(h);
    if ( !adapter || !host ) return false;
    const size_t len = strlen(host);
    if ( len == 0 || len >= sizeof(adapter->host) ) return false;
    memcpy(adapter->host, host, len + 1);
    return true;
}

bool das_wss_set_access_log(Handle<hv::WebSocketServer> h, bool enabled) {
    auto adapter = lookup_server(h);
    return adapter && adapter->set_access_log(enabled);
}

bool das_wss_set_limits(Handle<hv::WebSocketServer> h, int http_bytes, int ws_bytes, int events, int queue_bytes) {
    auto adapter = lookup_server(h);
    return adapter && adapter->set_limits(http_bytes, ws_bytes, events, queue_bytes);
}
bool das_wss_set_connection_limits(Handle<hv::WebSocketServer> h, int messages, int bytes, int write_bytes) {
    auto adapter = lookup_server(h);
    return adapter && adapter->set_connection_limits(messages, bytes, write_bytes);
}

int das_wss_start ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return -1;
    const int result = adapter->start();
    if (result == 0) adapter->started = true;
    return result;
}

int das_wss_bound_port ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return -1;
    return adapter->port;
}

void das_wss_tick ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return;
    adapter->tick();
}

int das_wss_stop ( Handle<hv::WebSocketServer> h ) {
    auto adapter = lookup_server(h);
    if ( !adapter ) return -1;
    const int result = adapter->stop();
    adapter->started = false;
    return result;
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

void das_wss_stream ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at ) {
    if ( auto adapter = lookup_server(h) ) adapter->STREAM(url, lmb, context, at);
}

// HttpResponseWriter operations. Each marshals the real socket write onto the connection's event loop
// via runInLoop (the writer is a SocketChannel with loop affinity; the das handler runs on the tick
// thread) and captures the writer's shared_ptr so it outlives the async post.

bool das_wss_upgrade(Handle<hv::WebSocketServer> h, int timeout_ms, Lambda lmb, Context * context, LineInfoArg * at) {
    auto adapter = lookup_server(h);
    return adapter && adapter->UPGRADE(timeout_ms, lmb, context, at);
}

static void post_writer_op ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w,
        std::function<void(hv::HttpResponseWriter*)> fn ) {
    // Hold the server's shared_ptr for the whole async op. respond/close capture the adapter to call
    // release_writer inside fn, and the posted op may run on the connection loop after the handler has
    // returned — capturing ssp keeps the adapter alive until the write executes (no use-after-free).
    auto ssp = HandleRegistry<hv::WebSocketServer>::instance().lookup(h);
    auto adapter = (WebServer_Adapter *) ssp.get();
    if ( !adapter || !w ) return;
    auto sp = adapter->find_writer(w);
    if ( !sp ) return;
    // loop(0), not loop(): the default idx=-1 resolves via currentThreadEventLoop — always null on
    // the tick thread, so the fallback ran every write there, racing the loop thread (glibc gmtime).
    auto loop = adapter->loop(0);
    if ( loop ) {
        loop->runInLoop([ssp,sp,fn](){ fn(sp.get()); });
    } else {
        fn(sp.get());
    }
}

// libhv closes any connection whose queued unsent bytes exceed the channel's max write
// bufsize (1<<24 by default): a buffered body larger than the socket drains in one write()
// gets the connection dropped MID-BODY — a truncated response after a clean 200. Raise the
// cap to cover the body; never lower it, so later responses on a kept-alive connection
// inherit at least the default.
static void cover_body_write_bufsize ( hv::HttpResponseWriter * wr, size_t body_size ) {
    size_t want = body_size + (1u << 20);
    if ( want < (1u << 24) ) want = (1u << 24);
    if ( want > 0xFFFFFFFFu ) want = 0xFFFFFFFFu;
    wr->setMaxWriteBufsize((uint32_t)want);
}

// Whole-body response through the writer (the non-streaming path): status + content-type + body, then
// end + release. Lets one async (writer) route serve both streamed and buffered responses.
int das_writer_respond ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w, int32_t status,
        const char * content_type, const char * body ) {
    auto adapter = lookup_server(h);
    std::string ct = content_type ? content_type : "application/json";
    std::string b = body ? body : "";
    int st = status;
    post_writer_op(h, w, [adapter,w,st,ct,b](hv::HttpResponseWriter* wr){
        wr->WriteStatus((http_status)st);
        wr->WriteHeader("Content-Type", ct.c_str());
        cover_body_write_bufsize(wr, b.size());
        int rc = wr->End(b);
        if ( rc < 0 ) {
            hloge("dasHV: writer respond failed rc=%d body=%zu", rc, b.size());
        }
        if ( adapter ) adapter->release_writer(w);
    });
    return 0;
}

static int decide_websocket(Handle<WebSocketAdmission> handle, int status, const string & protocol) {
    auto admission = HandleRegistry<WebSocketAdmission>::instance().lookup(handle);
    if (!admission || !admission->open.load() || std::chrono::steady_clock::now() >= admission->deadline) return -1;
    auto owner = HandleRegistry<hv::WebSocketServer>::instance().lookup(admission->owner);
    auto adapter = (WebServer_Adapter *)owner.get();
    if (!adapter || !adapter->started) return -1;
    auto loop = adapter->loop(0);
    if (!loop || admission->decided.exchange(true)) return -1;
    HandleRegistry<WebSocketAdmission>::instance().release(handle);
    auto alive = adapter->lifetime();
    loop->queueInLoop([alive,admission,status,protocol](){
        if (alive.expired() || !admission->open.load() || !admission->writer->isConnected()) return;
        auto & writer = admission->writer;
        if (!writer->awaiting_upgrade) return;
        writer->WriteStatus((http_status)status);
        if (!protocol.empty()) writer->WriteHeader(SEC_WEBSOCKET_PROTOCOL, protocol.c_str());
        writer->End();
    });
    return 0;
}

int das_accept_websocket(Handle<WebSocketAdmission> admission, const char * protocol) {
    const string selected = protocol ? protocol : "";
    if (selected.size() > 128 || selected.find_first_of("\r\n") != string::npos) return -1;
    return decide_websocket(admission, HTTP_STATUS_SWITCHING_PROTOCOLS, selected);
}

int das_reject_websocket(Handle<WebSocketAdmission> admission, int status) {
    if (status < 400 || status > 599) return -1;
    return decide_websocket(admission, status, "");
}

// Set a header on a writer-rail response. Order matters only relative to the terminal op:
// headers land on the writer's response object and serialize when `respond` / SERVE_FILE
// writes it out, so every set_header must be issued before that call.
int das_writer_set_header ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w,
        const char * key, const char * value ) {
    std::string k = key ? key : "";
    std::string v = value ? value : "";
    if ( k.empty() ) return -1;
    post_writer_op(h, w, [k,v](hv::HttpResponseWriter* wr){
        wr->WriteHeader(k.c_str(), v.c_str());
    });
    return 0;
}

// Whole-file response through the writer — the writer-rail twin of SERVE_FILE. The file is
// read on the connection loop, so the bytes never pass through a das string (whose
// `const char*` marshalling truncates binary at the first NUL). Content type follows the
// file name; headers already set through the writer ride along; a missing file is a 404
// with an empty body.
int das_writer_serve_file ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w, const char * filepath ) {
    auto adapter = lookup_server(h);
    std::string path = filepath ? filepath : "";
    post_writer_op(h, w, [adapter,w,path](hv::HttpResponseWriter* wr){
        int st = wr->response->File(path.c_str());
        if ( st != 200 ) wr->response->body.clear();
        wr->response->status_code = (http_status)st;
        cover_body_write_bufsize(wr, wr->response->body.size());
        int rc = wr->WriteResponse(wr->response.get());
        if ( rc < 0 ) {
            hloge("dasHV: writer SERVE_FILE failed rc=%d file=%s body=%zu",
                rc, path.c_str(), wr->response->body.size());
        }
        wr->End();
        if ( adapter ) adapter->release_writer(w);
    });
    return 0;
}

class WriterFileStream : public std::enable_shared_from_this<WriterFileStream> {
public:
    // modules/dasHV/ARCHITECTURE.md#bounded-file-transfer-lifecycle
    WriterFileStream(const shared_ptr<hv::WebSocketServer> & server, const HttpResponseWriterPtr & writer,
                     shared_ptr<atomic<bool>> token, FILE * file, uint64_t bytes)
        : server_(server), writer_(writer), token_(std::move(token)), file_(file, fclose), pump_(bytes) {}

    // modules/dasHV/ARCHITECTURE.md#bounded-file-transfer-lifecycle
    void start(uint64_t bytes, bool head_only) {
        auto writer = writer_.lock();
        if (!writer) return;
        auto self = shared_from_this();
        previous_close_ = writer->onclose;
        writer->onclose = [self]() {
            auto keep = self;
            keep->finish(false);
            if (keep->previous_close_) keep->previous_close_();
        };
        writer->onwrite = [self](hv::Buffer *) { self->schedule(); };
        writer->response->body.clear();
        writer->response->headers.erase("Transfer-Encoding");
        writer->response->content_length = int64_t(bytes);
        writer->response->SetHeader("Content-Length", std::to_string(bytes));
        if (!bytes) {
            writer->WriteResponse(writer->response.get());
            finish(true);
            return;
        }
        if (writer->EndHeaders() < 0) { finish(false); return; }
        if (head_only) { finish(true); return; }
        schedule();
    }

private:
    // modules/dasHV/ARCHITECTURE.md#bounded-file-transfer-lifecycle
    void schedule() {
        if (finished_ || scheduled_) return;
        auto server = server_.lock();
        auto adapter = static_cast<WebServer_Adapter *>(server.get());
        auto loop = adapter ? adapter->loop(0) : nullptr;
        if (!loop) { finish(false); return; }
        scheduled_ = true;
        auto self = shared_from_this();
        loop->queueInLoop([self]() { self->scheduled_ = false; self->step(); });
    }
    void step() {
        if (finished_) return;
        auto writer = writer_.lock();
        if (!writer || !writer->isConnected() || !token_->load()) { finish(false); return; }
        const auto result = pump_.step(writer->isWriteComplete(),
            [this](char * bytes, size_t count) { return fread(bytes, 1, count, file_.get()); },
            [&writer](const char * bytes, size_t count) { return writer->WriteBody(bytes, int(count)) >= 0; });
        if (result == FilePumpResult::failed) { finish(false); }
        else if (result == FilePumpResult::complete) { finish(true); }
        else if (result == FilePumpResult::progress && writer->isWriteComplete()) { schedule(); }
    }
    // modules/dasHV/ARCHITECTURE.md#bounded-file-transfer-lifecycle
    void finish(bool success) {
        if (finished_) return;
        finished_ = true;
        pump_.cancel();
        file_.reset();
        auto server = server_.lock();
        auto writer = writer_.lock();
        auto adapter = static_cast<WebServer_Adapter *>(server.get());
        if (!writer || !adapter) return;
        auto registration = adapter->writer_registration(writer.get());
        if (registration.open != token_) return;
        writer->onwrite = nullptr;
        writer->onclose = previous_close_;
        adapter->release_writer(writer.get(), token_);
        if (success) writer->End();
        else if (writer->isConnected()) writer->close(true);
    }
    std::weak_ptr<hv::WebSocketServer> server_;
    std::weak_ptr<hv::HttpResponseWriter> writer_;
    shared_ptr<atomic<bool>> token_;
    std::unique_ptr<FILE, decltype(&fclose)> file_;
    BoundedFilePump pump_;
    std::function<void()> previous_close_;
    bool scheduled_ = false, finished_ = false;
};

int das_writer_serve_file_stream(Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w,
                                const char * filepath, int64_t max_bytes) {
    if (!w || !filepath || !*filepath || max_bytes <= 0) return -1;
    auto server = HandleRegistry<hv::WebSocketServer>::instance().lookup(h);
    auto adapter = static_cast<WebServer_Adapter *>(server.get());
    if (!adapter) return -1;
    auto registration = adapter->writer_registration(w);
    auto loop = adapter->loop(0);
    if (!registration.writer || !registration.open->load() || !loop) return -1;
    const std::string path(filepath);
    const std::weak_ptr<hv::WebSocketServer> weak_server = server;
    loop->queueInLoop([weak_server, registration, path, max_bytes]() {
        auto server = weak_server.lock();
        if (!server) return;
        auto adapter = static_cast<WebServer_Adapter *>(server.get());
        auto writer = registration.writer;
        if (!registration.open->load() || !writer->isConnected() || writer->state != hv::HttpResponseWriter::SEND_BEGIN) return;
        writer->response->headers.erase("Transfer-Encoding");
        if (writer->response->GetHeader("Content-Type").empty() && writer->response->content_type == CONTENT_TYPE_NONE) {
            writer->response->SetHeader("Content-Type", "application/octet-stream");
        }
        uint64_t bytes = 0;
        std::unique_ptr<FILE, decltype(&fclose)> file(das_fopen_regular_read_utf8(path.c_str(), bytes), fclose);
        const int status = !file ? 404 : bytes > uint64_t(max_bytes) ? 413 : 200;
        if (status != 200) {
            file.reset();
            writer->response->body.clear();
            writer->response->status_code = http_status(status);
            writer->response->content_length = 0;
            writer->response->SetHeader("Content-Length", "0");
            adapter->release_writer(writer.get(), registration.open);
            writer->WriteResponse(writer->response.get());
            writer->End();
            return;
        }
        auto transfer = std::make_shared<WriterFileStream>(server, writer, registration.open, file.release(), bytes);
        transfer->start(bytes, registration.head_only);
    });
    return 0;
}

// One SSE event, chunk-framed: `data: <data>\n\n` (an `event:` line only when named) — the OpenAI
// streaming wire format. The first event sends text/event-stream + Transfer-Encoding: chunked
// headers; chunked framing is what makes close_writer's End() a VISIBLE terminator (the 0-chunk).
// libhv's raw SSEvent() writes unframed bytes whose only end-of-stream is a connection close that
// keep-alive never sends — buffered clients would wait out their whole timeout on every stream.
int das_writer_sse_event ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w, const char * data, const char * event ) {
    std::string d = data ? data : "";
    std::string e = event ? event : "";
    post_writer_op(h, w, [d,e](hv::HttpResponseWriter* wr){
        if ( wr->state == hv::HttpResponseWriter::SEND_BEGIN ) {
            wr->WriteHeader("Content-Type", "text/event-stream");
        }
        std::string msg;
        if ( !e.empty() ) { msg = "event: "; msg += e; msg += "\n"; }
        msg += "data: "; msg += d; msg += "\n\n";
        wr->WriteChunked(msg);   // first call emits the headers via EndHeaders("Transfer-Encoding", "chunked")
    });
    return 0;
}

int das_writer_write_chunked ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w, const char * data ) {
    std::string d = data ? data : "";
    post_writer_op(h, w, [d](hv::HttpResponseWriter* wr){ wr->WriteChunked(d.c_str(), (int)d.size()); });
    return 0;
}

int das_writer_end_headers ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w, const char * key, const char * value ) {
    std::string k = key ? key : "";
    std::string v = value ? value : "";
    post_writer_op(h, w, [k,v](hv::HttpResponseWriter* wr){
        wr->EndHeaders(k.empty() ? nullptr : k.c_str(), v.empty() ? nullptr : v.c_str());
    });
    return 0;
}

// Change the idle keepalive timeout for a retained writer. STREAM handlers that defer their first
// write for long-running work otherwise inherit libhv's 75-second HTTP keepalive timeout.
void das_writer_set_keepalive_timeout ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w,
        int32_t timeout_ms ) {
    int timeout = timeout_ms;
    post_writer_op(h, w, [timeout](hv::HttpResponseWriter* wr){
        wr->setKeepaliveTimeout(timeout);
    });
}

// Terminal for a streamed response: end the stream and release the writer.
void das_writer_close ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w ) {
    auto adapter = lookup_server(h);
    post_writer_op(h, w, [adapter,w](hv::HttpResponseWriter* wr){
        wr->End();
        if ( adapter ) adapter->release_writer(w);
    });
}

void das_writer_release ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w ) {
    if ( !w ) return;
    if ( auto adapter = lookup_server(h) ) adapter->release_writer(w);
}

bool das_writer_is_connected ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w ) {
    auto adapter = lookup_server(h);
    if ( !adapter || !w ) return false;
    return adapter->is_writer_open(w);
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

// The buffered rail cannot raise the connection's write-buf cap — the write happens after
// the handler returns, with no channel in reach — so a body over the libhv default gets the
// connection closed MID-SEND: a truncated response after a clean 200, logged only in libhv's
// side log. Refuse loudly instead; a big body belongs on the writer rail, which covers it.
static const size_t DAS_HV_BUFFERED_BODY_MAX = (1u << 24) - (1u << 20);

static http_status das_resp_refuse_oversize ( HttpResponse * resp, const char * what, size_t size ) {
    hloge("dasHV: buffered %s refused: %zu bytes exceeds the %zu write-buf budget; "
          "serve it through a STREAM route's SERVE_FILE/respond", what, size, DAS_HV_BUFFERED_BODY_MAX);
    resp->content_type = TEXT_PLAIN;
    resp->body = "response too large for the buffered rail; serve it through a STREAM route";
    return (http_status)HTTP_STATUS_INTERNAL_SERVER_ERROR;
}

http_status das_resp_file ( HttpResponse * resp, const char * filepath ) {
    std::string path = filepath ? filepath : "";
    size_t fs = hv_filesize(path.c_str());
    if ( fs > DAS_HV_BUFFERED_BODY_MAX ) {
        return das_resp_refuse_oversize(resp, "SERVE_FILE", fs);
    }
    return (http_status)resp->File(path.c_str());
}

http_status das_resp_data ( HttpResponse * resp, const char * data, int32_t len, http_status status ) {
    if ( !data || len < 0 ) len = 0;
    if ( (size_t)len > DAS_HV_BUFFERED_BODY_MAX ) {
        return das_resp_refuse_oversize(resp, "DATA", (size_t)len);
    }
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
    table_for_each<char *, char *>(tab, [&]( char * key, char * value ) {
        headers[key] = value;
    });
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
    table_for_each<char *, char *>(from, [&]( char * key, char * value ) {
        hv::FormData data;
        if ( value != nullptr ) {
            if (*value == '@') {
                data.filename = value+1;
            } else {
                data.content = value;
            }
        }
        req->form[key ? key : ""] = data;
    });
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
    table_for_each<char *, char *>(from, [&]( char * key, char * value ) {
        hv::FormData data;
        if ( value != nullptr ) {
            if (*value == '@') {
                data.filename = value+1;
            } else {
                data.content = value;
            }
        }
        req->form[key ? key : ""] = data;
    });
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
    table_for_each<char *, char *>(from, [&]( char * key, char * value ) {
        hv::FormData data;
        if ( value != nullptr ) {
            if (*value == '@') {
                data.filename = value+1;
            } else {
                data.content = value;
            }
        }
        req->form[key ? key : ""] = data;
    });
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

int das_req_REQUEST_CHECKED(HttpRequest * req, const char * ca_file, int max_response_bytes,
        const TBlock<void,TTemporary<HttpResponse*>> & block, Context * context, LineInfoArg * at) {
    if (!req || max_response_bytes < 1 || max_response_bytes > 16 * 1024 * 1024 ||
        req->url.size() > 8192 || req->url.find('\0') != string::npos || req->timeout == 0 || req->timeout > 120) return ERR_INVALID_PARAM;
    for (unsigned char byte : req->url) {
        if (byte <= 32 || byte == 127) return ERR_INVALID_PARAM;
    }
    HUrl url;
    if (!url.parse(req->url) || url.host.empty() || !url.username.empty() || !url.password.empty()) return ERR_INVALID_PARAM;
    if (stricmp(url.scheme.c_str(), "https") != 0) return ERR_INVALID_PROTOCOL;
    url.scheme = "https";
    url.fragment.clear();
    HttpRequest request = *req;
    request.url = url.dump();
    request.proxy = 0;
    request.redirect = 0;
    request.retry_count = 0;
    request.http_cb = nullptr;
    request.response_body_limit = size_t(max_response_bytes);
    std::unique_ptr<http_client_t,decltype(&http_client_del)> client(http_client_new(), http_client_del);
    if (!client) return ERR_NULL_POINTER;
    hssl_ctx_opt_t tls{};
    tls.endpoint = HSSL_CLIENT;
    tls.verify_peer = 1;
    tls.ca_file = ca_file && *ca_file ? ca_file : nullptr;
    const int configured = http_client_new_ssl_ctx(client.get(), &tls);
    if (configured != 0) return configured;
    HttpResponse response;
    const int result = http_client_send(client.get(), &request, &response);
    if (result == 0) das_invoke<void>::invoke<HttpResponse*>(context, at, block, &response);
    return result;
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

// The peer address of the accepted connection. Unlike any header, a client
// cannot set this — a service that must distinguish loopback callers from
// proxied ones has to ask the transport, not X-Forwarded-For.
char * das_httpr_client_ip ( HttpRequest * req, Context * context, LineInfoArg * at ) {
    if ( !req ) return nullptr;
    if ( req->client_addr.ip.empty() ) return nullptr;
    return context->allocateString(req->client_addr.ip, at);
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
    // Parsed HTTP/1 requests retain the raw Cookie field. Other messages
    // store cookies separately; emit those as DumpHeaders() does.
    if (msg->type == HTTP_REQUEST && msg->headers.find("Cookie") != msg->headers.end()) return;
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
#ifdef _WIN32
        // libhv's client path initializes Winsock lazily; the server start()
        // path does not, so a server started before any client call fails
        // with WSANOTINITIALISED (-10093) — a race every threaded test
        // harness sits on. Initialize once here, before any context runs.
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        // libhv defaults to file_logger (bin/libhv.YYYYMMDD.log). Invisible
        // under popen + CI runners. Two env-gated opt-outs:
        //   DASLIVE_HV_LOG=stderr (or =1)  → redirect libhv to stderr
        //   DASLIVE_HV_LOG=stdout          → redirect libhv to stdout
        //   DASLIVE_HV_LOG=silent          → disable libhv logging entirely
        // DASLIVE_HV_LOG_LEVEL=DEBUG/INFO/WARN/ERROR overrides the level.
        // Default unset → file_logger (unchanged).
        if (const char * route = std::getenv("DASLIVE_HV_LOG")) {
            if (!strcmp(route, "stderr") || !strcmp(route, "1")) {
                hlog_set_handler(stderr_logger);
            } else if (!strcmp(route, "stdout")) {
                hlog_set_handler(stdout_logger);
            } else if (!strcmp(route, "silent")) {
                hlog_disable();
            }
        }
        if (const char * lvl = std::getenv("DASLIVE_HV_LOG_LEVEL")) {
            hlog_set_level_by_str(lvl);
        }

        ModuleLibrary lib;
        lib.addModule(this);
        lib.addBuiltInModule();
        lib.addModule(Module::require("rtti_core"));
        addEnumeration(new Enumeration_ws_opcode());
        addEnumeration(new Enumeration_ws_session_type());
        addEnumeration(new Enumeration_http_method());
        addEnumeration(new Enumeration_http_status());
        addExtern<DAS_BIND_FUN(das_hv_set_log_file)>(*this, lib, "hv_set_log_file",
            SideEffects::modifyExternal, "das_hv_set_log_file")
                ->args({"path"});
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
        addAnnotation(new HttpResponseWriterAnnotation(lib));
        addHandleAnnotation<WebSocketAdmission>(this, lib, "WebSocketAdmission", "",
            "das::Handle<das::WebSocketAdmission>");
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
        addExtern<DAS_BIND_FUN(das_wss_close_channel)> (*this, lib, "close",
            SideEffects::worstDefault, "das_wss_close_channel")
                ->args({"channel"});
        addExtern<DAS_BIND_FUN(das_wss_upgrade)>(*this, lib, "WEBSOCKET_UPGRADE",
            SideEffects::worstDefault, "das_wss_upgrade")
            ->args({"server", "timeout_ms", "handler", "context", "at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_accept_websocket)>(*this, lib, "accept_websocket",
            SideEffects::worstDefault, "das_accept_websocket")->args({"admission", "protocol"});
        addExtern<DAS_BIND_FUN(das_reject_websocket)>(*this, lib, "reject_websocket",
            SideEffects::worstDefault, "das_reject_websocket")->args({"admission", "status"});
        addExtern<DAS_BIND_FUN(das_wss_set_access_log)>(*this, lib, "set_access_log",
            SideEffects::worstDefault, "das_wss_set_access_log")->args({"server", "enabled"});
        addExtern<DAS_BIND_FUN(das_wss_set_limits)>(*this, lib, "set_limits",
            SideEffects::worstDefault, "das_wss_set_limits")
            ->args({"server", "http_body_bytes", "websocket_message_bytes", "pending_events", "pending_bytes"});
        addExtern<DAS_BIND_FUN(das_wss_set_connection_limits)>(*this, lib, "set_connection_limits",
            SideEffects::worstDefault, "das_wss_set_connection_limits")
            ->args({"server", "pending_messages", "pending_bytes", "write_buffer_bytes"});
        addExtern<DAS_BIND_FUN(das_wss_set_bind_host)> (*this, lib, "set_bind_host",
            SideEffects::worstDefault, "das_wss_set_bind_host")
                ->args({"server","host"});
        addExtern<DAS_BIND_FUN(das_wss_start)> (*this, lib, "start",
            SideEffects::worstDefault, "das_wss_start")
                ->args({"server"});
        addExtern<DAS_BIND_FUN(das_wss_bound_port)> (*this, lib, "bound_port",
            SideEffects::worstDefault, "das_wss_bound_port")
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
        addExtern<DAS_BIND_FUN(das_req_REQUEST_CHECKED)>(*this, lib, "request_checked",
            SideEffects::worstDefault, "das_req_REQUEST_CHECKED")
            ->args({"request", "ca_file", "max_response_bytes", "block", "context", "at"});
        addExtern<DAS_BIND_FUN(das_req_REQUEST)> (*this, lib, "request",
            SideEffects::worstDefault, "das_req_REQUEST")
                ->args({"request","block","context","at"});
        // Transport-level peer address (unforgeable, unlike any header)
        addExtern<DAS_BIND_FUN(das_httpr_client_ip)> (*this, lib, "client_ip",
            SideEffects::worstDefault, "das_httpr_client_ip")
                ->args({"request","context","at"})->setTempStringResult();
        // Response/message header access
        addExtern<DAS_BIND_FUN(das_httpm_get_header)> (*this, lib, "get_header",
            SideEffects::worstDefault, "das_httpm_get_header")
                ->args({"message","key","context","at"})->setTempStringResult();
        addExtern<DAS_BIND_FUN(das_httpm_each_header)> (*this, lib, "each_header",
            SideEffects::worstDefault, "das_httpm_each_header")
                ->args({"message","block","context","at"});
        // Response status message
        addExtern<DAS_BIND_FUN(das_httpr_status_message)> (*this, lib, "status_message",
            SideEffects::worstDefault, "das_httpr_status_message")
                ->args({"response","context","at"})->setTempStringResult();
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
                ->args({"request","key","context","at"})->setTempStringResult();
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
                ->args({"request","name","context","at"})->setTempStringResult();
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
                ->args({"response","name","context","at"})->setTempStringResult();
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
                ->args({"request","name","context","at"})->setTempStringResult();
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
                ->args({"request","key","context","at"})->setTempStringResult();
        // Streaming request (client-side)
        addExtern<DAS_BIND_FUN(das_req_REQUEST_CB)> (*this, lib, "request_cb",
            SideEffects::worstDefault, "das_req_REQUEST_CB")
                ->args({"request","on_body","on_complete","context","at"});
        addExtern<DAS_BIND_FUN(das_req_REQUEST_CB_S)> (*this, lib, "request_cb",
            SideEffects::worstDefault, "das_req_REQUEST_CB_S")
                ->args({"request","on_body","on_complete","context","at"});
        // Server-side SSE
        addExtern<DAS_BIND_FUN(das_wss_sse)> (*this, lib, "SSE",
            SideEffects::worstDefault, "das_wss_sse")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        addExtern<DAS_BIND_FUN(das_wss_stream)> (*this, lib, "STREAM",
            SideEffects::worstDefault, "das_wss_stream")
                ->args({"server","url","lambda","context","at"})->unsafeOperation = true;
        // HttpResponseWriter operations (handle-first: they marshal the write onto the server loop)
        addExtern<DAS_BIND_FUN(das_writer_respond)> (*this, lib, "respond",
            SideEffects::worstDefault, "das_writer_respond")
                ->args({"server","writer","status","content_type","body"});
        addExtern<DAS_BIND_FUN(das_writer_end_headers)> (*this, lib, "end_headers",
            SideEffects::worstDefault, "das_writer_end_headers")
                ->args({"server","writer","key","value"});
        addExtern<DAS_BIND_FUN(das_writer_sse_event)> (*this, lib, "sse_event",
            SideEffects::worstDefault, "das_writer_sse_event")
                ->args({"server","writer","data","event"});
        addExtern<DAS_BIND_FUN(das_writer_write_chunked)> (*this, lib, "write_chunked",
            SideEffects::worstDefault, "das_writer_write_chunked")
                ->args({"server","writer","data"});
        addExtern<DAS_BIND_FUN(das_writer_set_keepalive_timeout)> (*this, lib, "set_writer_keepalive_timeout",
            SideEffects::worstDefault, "das_writer_set_keepalive_timeout")
                ->args({"server","writer","timeout_ms"});
        addExtern<DAS_BIND_FUN(das_writer_set_header)> (*this, lib, "set_header",
            SideEffects::worstDefault, "das_writer_set_header")
                ->args({"server","writer","key","value"});
        addExtern<DAS_BIND_FUN(das_writer_serve_file)> (*this, lib, "SERVE_FILE",
            SideEffects::worstDefault, "das_writer_serve_file")
                ->args({"server","writer","filepath"});
        addExtern<DAS_BIND_FUN(das_writer_serve_file_stream)> (*this, lib, "SERVE_FILE_STREAM",
            SideEffects::worstDefault, "das_writer_serve_file_stream")
                ->args({"server","writer","filepath","max_bytes"});
        addExtern<DAS_BIND_FUN(das_writer_close)> (*this, lib, "close_writer",
            SideEffects::worstDefault, "das_writer_close")
                ->args({"server","writer"});
        addExtern<DAS_BIND_FUN(das_writer_release)> (*this, lib, "release_writer",
            SideEffects::worstDefault, "das_writer_release")
                ->args({"server","writer"});
        addExtern<DAS_BIND_FUN(das_writer_is_connected)> (*this, lib, "is_writer_connected",
            SideEffects::worstDefault, "das_writer_is_connected")
                ->args({"server","writer"});

    }
    ~Module_HV() {
        hv::async::cleanup();
    }
    virtual ModuleAotType aotRequire ( TextWriter & tw ) const override {
        tw << "#include \"../modules/dasHV/src/aot_hv.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_HV,Module_HV);
}

REGISTER_MODULE_IN_NAMESPACE(Module_HV,das);
