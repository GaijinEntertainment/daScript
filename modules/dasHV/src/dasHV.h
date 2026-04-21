#pragma once

#include "aot_hv.h"

#include "daScript/simulate/bind_enum.h"

MAKE_EXTERNAL_TYPE_FACTORY(WebSocketClient,hv::WebSocketClient)
MAKE_EXTERNAL_TYPE_FACTORY(WebSocketServer,hv::WebSocketServer)
MAKE_EXTERNAL_TYPE_FACTORY(WebSocketChannel,hv::WebSocketChannel)
MAKE_EXTERNAL_TYPE_FACTORY(HttpMessage,HttpMessage)
MAKE_EXTERNAL_TYPE_FACTORY(HttpRequest,HttpRequest)
MAKE_EXTERNAL_TYPE_FACTORY(HttpResponse,HttpResponse)
MAKE_EXTERNAL_TYPE_FACTORY(HttpContext,hv::HttpContext)
MAKE_EXTERNAL_TYPE_FACTORY(HttpResponseWriter,hv::HttpResponseWriter)

DAS_BIND_ENUM_CAST(ws_opcode)
DAS_BASE_BIND_ENUM_GEN(ws_opcode,ws_opcode)

DAS_BIND_ENUM_CAST(ws_session_type)
DAS_BASE_BIND_ENUM_GEN(ws_session_type,ws_session_type)

DAS_BIND_ENUM_CAST(http_method)
DAS_BASE_BIND_ENUM_GEN(http_method,http_method)

DAS_BIND_ENUM_CAST(http_status)
DAS_BASE_BIND_ENUM_GEN(http_status,http_status)

namespace das {

Handle<hv::WebSocketClient> makeWebSocketClient ( const void * pClass, const StructInfo * info, Context * context );
int das_wsc_open ( Handle<hv::WebSocketClient> h, const char* url );
int das_wsc_send ( Handle<hv::WebSocketClient> h, const char* msg );
int das_wsc_send_buf ( Handle<hv::WebSocketClient> h, const char* msg, int32_t len, ws_opcode opcode );
int das_wsc_close ( Handle<hv::WebSocketClient> h );
bool das_wsc_is_connected ( Handle<hv::WebSocketClient> h );
void das_wsc_tick ( Handle<hv::WebSocketClient> h );

Handle<hv::WebSocketServer> makeWebSocketServer ( int port, int httpsPort, const char * pathToCert, const void * pClass, const StructInfo * info, Context * context, LineInfoArg * at );
int das_wss_send ( Handle<hv::WebSocketChannel> h, const char * msg, ws_opcode opcode, bool fin );
int das_wss_send_buf ( Handle<hv::WebSocketChannel> h, const char * buf, int32_t len, ws_opcode opcode, bool fin );
int das_wss_send_fragment ( Handle<hv::WebSocketChannel> h, const char * buf, int32_t len, int32_t fragment, ws_opcode opcode );
int das_wss_start ( Handle<hv::WebSocketServer> h );
void das_wss_tick ( Handle<hv::WebSocketServer> h );
int das_wss_stop ( Handle<hv::WebSocketServer> h );
void das_wss_get ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_post ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_put ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_del ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_patch ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_head ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_any ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
void das_wss_static ( Handle<hv::WebSocketServer> h, const char * path, const char * dir );
void das_wss_allow_cors ( Handle<hv::WebSocketServer> h );
void das_wss_sse ( Handle<hv::WebSocketServer> h, const char * url, Lambda lmb, Context * context, LineInfoArg * at );

// HttpResponseWriter operations
int das_writer_end_headers ( hv::HttpResponseWriter * w, const char * key, const char * value );
int das_writer_sse_event ( hv::HttpResponseWriter * w, const char * data, const char * event );
int das_writer_write_chunked ( hv::HttpResponseWriter * w, const char * data, int32_t len );
int das_writer_end ( hv::HttpResponseWriter * w );
int das_writer_close ( hv::HttpResponseWriter * w );
void das_writer_release ( Handle<hv::WebSocketServer> h, hv::HttpResponseWriter * w );

http_status das_resp_string ( HttpResponse * resp, const char * msg, http_status status = HTTP_STATUS_OK );
http_status das_resp_json ( HttpResponse * resp, const char * json_str, http_status status = HTTP_STATUS_OK );
http_status das_resp_redirect ( HttpResponse * resp, const char * location, http_status status );
http_status das_resp_file ( HttpResponse * resp, const char * filepath );
http_status das_resp_data ( HttpResponse * resp, const char * data, int32_t len, http_status status = HTTP_STATUS_OK );
void das_resp_set_header ( HttpResponse * resp, const char * key, const char * value );
void das_resp_set_content_type ( HttpResponse * resp, const char * ct );

http_headers das_req_table_to_headers ( const TTable<char *,char *> & tab );
void das_req_GET ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_GET_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_POST ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_POST_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_POST_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// PUT
void das_req_PUT ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_PUT_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_PUT_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// PATCH
void das_req_PATCH ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_PATCH_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_PATCH_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// DELETE
void das_req_DELETE ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_DELETE_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// HEAD
void das_req_HEAD ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
void das_req_HEAD_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// Generic request
void das_req_REQUEST ( HttpRequest * req, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );

// Streaming request — invokes on_body per chunk, then on_complete when done
void das_req_REQUEST_CB ( HttpRequest * req, const TBlock<void,const uint8_t*,int32_t> & on_body,
    const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at );
void das_req_REQUEST_CB_S ( HttpRequest * req, const TBlock<void,const char*> & on_body,
    const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at );

// Response/message header access
char * das_httpm_get_header ( HttpMessage * msg, const char * key, Context * context, LineInfoArg * at );
void das_httpm_each_header ( HttpMessage * msg, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );

// Response status message
char * das_httpr_status_message ( HttpResponse * resp, Context * context, LineInfoArg * at );

// Request configuration
void das_httpr_set_header ( HttpRequest * req, const char * key, const char * value );
void das_httpr_set_basic_auth ( HttpRequest * req, const char * username, const char * password );
void das_httpr_set_bearer_token_auth ( HttpRequest * req, const char * token );
void das_httpr_set_timeout ( HttpRequest * req, int sec );
void das_httpr_set_connect_timeout ( HttpRequest * req, int sec );
void das_httpr_allow_redirect ( HttpRequest * req, bool on );
void das_httpr_set_param ( HttpRequest * req, const char * key, const char * value );
char * das_httpr_get_param ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at );
void das_httpr_each_param ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );
void das_httpr_set_content_type ( HttpRequest * req, const char * ct );

// Cookies — HttpRequest* overloads
void das_httpreq_add_cookie ( HttpRequest * req, const char * name, const char * value );
void das_httpreq_add_cookie_ex ( HttpRequest * req, const char * name, const char * value,
        const char * domain, const char * path, int max_age, bool secure, bool httponly );
char * das_httpreq_get_cookie ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at );
void das_httpreq_each_cookie ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );

// Cookies — HttpResponse overloads
void das_httpresp_add_cookie ( HttpResponse * resp, const char * name, const char * value );
void das_httpresp_add_cookie_ex ( HttpResponse * resp, const char * name, const char * value,
        const char * domain, const char * path, int max_age, bool secure, bool httponly );
char * das_httpresp_get_cookie ( HttpResponse * resp, const char * name, Context * context, LineInfoArg * at );
void das_httpresp_each_cookie ( HttpResponse * resp, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );

// Form data - client side
void das_httpr_set_form_data ( HttpRequest * req, const char * name, const char * value );
void das_httpr_set_form_file ( HttpRequest * req, const char * name, const char * filepath );

// Form data - server side (HttpRequest*)
char * das_httpreq_get_form_data ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at );
int das_httpreq_save_form_file ( HttpRequest * req, const char * name, const char * path );
void das_httpreq_each_form_field ( HttpRequest * req, const TBlock<void,const char *,const char *,const char *> & block, Context * context, LineInfoArg * at );

// URL-encoded form data
void das_httpr_set_url_encoded ( HttpRequest * req, const char * key, const char * value );
char * das_httpreq_get_url_encoded ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at );

}
