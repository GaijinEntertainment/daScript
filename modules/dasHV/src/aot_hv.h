#pragma once

#ifndef HV_STATICLIB
#define HV_STATICLIB
#endif

#ifdef _MSC_VER
#pragma warning(disable:4458)
#endif

#include <hv/requests.h>
#include <hv/HttpServer.h>
#include <hv/WebSocketClient.h>
#include <hv/WebSocketServer.h>

#include "daScript/simulate/simulate.h"

namespace das {

    // websocket client
    hv::WebSocketClient * makeWebSocketClient ( const void * pClass, const StructInfo * info, Context * context );
    int das_wsc_open ( hv::WebSocketClient & client, const char* url );
    int das_wsc_send ( hv::WebSocketClient & client, const char* msg );
    int das_wsc_send_buf ( hv::WebSocketClient & client, const char* msg, int32_t len, ws_opcode opcode );
    bool das_wsc_is_connected ( hv::WebSocketClient & client );
    void das_wsc_tick ( hv::WebSocketClient & client );

    // websocket server
    hv::WebSocketServer * makeWebSocketServer ( int port, int httpsPort, const char * pathToCert, const void * pClass, const StructInfo * info, Context * context, LineInfoArg * at );
    int das_wss_send ( hv::WebSocketChannel * channel, const char * msg, ws_opcode opcode, bool fin );
    int das_wss_send_buf ( hv::WebSocketChannel * channel, const char * buf, int32_t len, ws_opcode opcode, bool fin );
    int das_wss_send_fragment ( hv::WebSocketChannel * channel, const char * buf, int32_t len, int32_t fragment, ws_opcode opcode );
    int das_wss_start ( hv::WebSocketServer * server );
    void das_wss_tick ( hv::WebSocketServer * server );
    int das_wss_stop ( hv::WebSocketServer * server );

    // server routes
    void das_wss_get ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_post ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_put ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_del ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_patch ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_head ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_any ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_sse ( hv::WebSocketServer * server, const char * url, Lambda lmb, Context * context, LineInfoArg * at );
    void das_wss_static ( hv::WebSocketServer * server, const char * path, const char * dir );
    void das_wss_allow_cors ( hv::WebSocketServer * server );
    void das_wss_set_document_root ( hv::WebSocketServer * server, const char * dir );
    void das_wss_set_home_page ( hv::WebSocketServer * server, const char * filename );
    void das_wss_set_index_of ( hv::WebSocketServer * server, const char * dir );
    void das_wss_set_error_page ( hv::WebSocketServer * server, const char * filename );

    // response writer (SSE/chunked)
    int das_writer_end_headers ( hv::HttpResponseWriter * w, const char * key, const char * value );
    int das_writer_sse_event ( hv::HttpResponseWriter * w, const char * data, const char * event );
    int das_writer_write_chunked ( hv::HttpResponseWriter * w, const char * data, int32_t len );
    int das_writer_end ( hv::HttpResponseWriter * w );
    int das_writer_close ( hv::HttpResponseWriter * w );
    void das_writer_release ( hv::WebSocketServer * server, hv::HttpResponseWriter * w );

    // response helpers
    http_status das_resp_string ( HttpResponse * resp, const char * msg, http_status status );
    http_status das_resp_json ( HttpResponse * resp, const char * json_str, http_status status );
    http_status das_resp_redirect ( HttpResponse * resp, const char * location, http_status status );
    http_status das_resp_file ( HttpResponse * resp, const char * filepath );
    http_status das_resp_data ( HttpResponse * resp, const char * data, int32_t len, http_status status );
    void das_resp_set_header ( HttpResponse * resp, const char * key, const char * value );
    void das_resp_set_content_type ( HttpResponse * resp, const char * ct );
    int das_http_response_content_length ( const HttpResponse & resp );

    // request parameter access
    void das_httpr_each_param ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );
    void das_httpr_set_header ( HttpRequest * req, const char * key, const char * value );
    char * das_httpm_get_header ( HttpMessage * msg, const char * key, Context * context, LineInfoArg * at );
    void das_httpm_each_header ( HttpMessage * msg, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );
    char * das_httpr_status_message ( HttpResponse * resp, Context * context, LineInfoArg * at );
    char * das_httpr_get_param ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at );

    // request configuration
    void das_httpr_set_basic_auth ( HttpRequest * req, const char * username, const char * password );
    void das_httpr_set_bearer_token_auth ( HttpRequest * req, const char * token );
    void das_httpr_set_timeout ( HttpRequest * req, int sec );
    void das_httpr_set_connect_timeout ( HttpRequest * req, int sec );
    void das_httpr_allow_redirect ( HttpRequest * req, bool on );
    void das_httpr_set_param ( HttpRequest * req, const char * key, const char * value );
    void das_httpr_set_content_type ( HttpRequest * req, const char * ct );

    // HTTP verbs
    void das_req_GET ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_GET_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_POST ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_POST_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_POST_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PUT ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PUT_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PUT_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PATCH ( const char * url, const char * text, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PATCH_H ( const char * url, const char * text, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_PATCH_HF ( const char * url, const char * text, const TTable<char *,char *> & tab, const TTable<char *,char *> & from, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_DELETE ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_DELETE_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_HEAD ( const char * url, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_HEAD_H ( const char * url, const TTable<char *,char *> & tab, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_REQUEST ( HttpRequest * req, const TBlock<void,HttpResponse*> & block, Context * context, LineInfoArg * at );
    void das_req_REQUEST_CB ( HttpRequest * req, const TBlock<void,const uint8_t*,int32_t> & on_body, const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at );
    void das_req_REQUEST_CB_S ( HttpRequest * req, const TBlock<void,const char*> & on_body, const TBlock<void,HttpResponse*> & on_complete, Context * context, LineInfoArg * at );

    // cookies
    void das_httpreq_add_cookie ( HttpRequest * req, const char * name, const char * value );
    void das_httpreq_add_cookie_ex ( HttpRequest * req, const char * name, const char * value, const char * domain, const char * path, int max_age, bool secure, bool httponly );
    char * das_httpreq_get_cookie ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at );
    void das_httpreq_each_cookie ( HttpRequest * req, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );
    void das_httpresp_add_cookie ( HttpResponse * resp, const char * name, const char * value );
    void das_httpresp_add_cookie_ex ( HttpResponse * resp, const char * name, const char * value, const char * domain, const char * path, int max_age, bool secure, bool httponly );
    char * das_httpresp_get_cookie ( HttpResponse * resp, const char * name, Context * context, LineInfoArg * at );
    void das_httpresp_each_cookie ( HttpResponse * resp, const TBlock<void,const char *,const char *> & block, Context * context, LineInfoArg * at );

    // form data
    void das_httpr_set_form_data ( HttpRequest * req, const char * name, const char * value );
    void das_httpr_set_form_file ( HttpRequest * req, const char * name, const char * filepath );
    char * das_httpreq_get_form_data ( HttpRequest * req, const char * name, Context * context, LineInfoArg * at );
    int das_httpreq_save_form_file ( HttpRequest * req, const char * name, const char * path );
    void das_httpreq_each_form_field ( HttpRequest * req, const TBlock<void,const char *,const char *,const char *> & block, Context * context, LineInfoArg * at );

    // url-encoded
    void das_httpr_set_url_encoded ( HttpRequest * req, const char * key, const char * value );
    char * das_httpreq_get_url_encoded ( HttpRequest * req, const char * key, Context * context, LineInfoArg * at );
}
