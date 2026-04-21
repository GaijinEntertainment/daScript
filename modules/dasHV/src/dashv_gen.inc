class HvWebSocketClient_Adapter {
protected:
  enum {
    __fn_onOpen = 0,
    __fn_onClose = 1,
    __fn_onMessage = 2,
  };
protected:
  int _das_class_method_offset[3];
public:
  HvWebSocketClient_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_onOpen] = info->fields[5]->offset;
      _das_class_method_offset[__fn_onClose] = info->fields[6]->offset;
      _das_class_method_offset[__fn_onMessage] = info->fields[7]->offset;
  }
  __forceinline Func get_onOpen ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onOpen]);
  }
  __forceinline void invoke_onOpen ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_onClose ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onClose]);
  }
  __forceinline void invoke_onClose ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
  __forceinline Func get_onMessage ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onMessage]);
  }
  __forceinline void invoke_onMessage ( Context * __context__, Func __funcCall__, void * self, char * const  msg ) const {
    das_invoke_function<void>::invoke
      <void *,char * const >
        (__context__,nullptr,__funcCall__,
          self,msg);
  }
};

class HvWebServer_Adapter {
protected:
  enum {
    __fn_onWsOpen = 0,
    __fn_onWsClose = 1,
    __fn_onWsMessage = 2,
    __fn_onTick = 3,
  };
protected:
  int _das_class_method_offset[4];
public:
  HvWebServer_Adapter ( const StructInfo * info ) {
      _das_class_method_offset[__fn_onWsOpen] = info->fields[9]->offset;
      _das_class_method_offset[__fn_onWsClose] = info->fields[10]->offset;
      _das_class_method_offset[__fn_onWsMessage] = info->fields[11]->offset;
      _das_class_method_offset[__fn_onTick] = info->fields[12]->offset;
  }
  __forceinline Func get_onWsOpen ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onWsOpen]);
  }
  __forceinline void invoke_onWsOpen ( Context * __context__, Func __funcCall__, void * self, das::Handle<hv::WebSocketChannel> const  channel, char * const  url ) const {
    das_invoke_function<void>::invoke
      <void *,das::Handle<hv::WebSocketChannel> const ,char * const >
        (__context__,nullptr,__funcCall__,
          self,channel,url);
  }
  __forceinline Func get_onWsClose ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onWsClose]);
  }
  __forceinline void invoke_onWsClose ( Context * __context__, Func __funcCall__, void * self, das::Handle<hv::WebSocketChannel> const  channel ) const {
    das_invoke_function<void>::invoke
      <void *,das::Handle<hv::WebSocketChannel> const >
        (__context__,nullptr,__funcCall__,
          self,channel);
  }
  __forceinline Func get_onWsMessage ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onWsMessage]);
  }
  __forceinline void invoke_onWsMessage ( Context * __context__, Func __funcCall__, void * self, das::Handle<hv::WebSocketChannel> const  channel, char * const  msg ) const {
    das_invoke_function<void>::invoke
      <void *,das::Handle<hv::WebSocketChannel> const ,char * const >
        (__context__,nullptr,__funcCall__,
          self,channel,msg);
  }
  __forceinline Func get_onTick ( void * self ) const {
    return getDasClassMethod(self,_das_class_method_offset[__fn_onTick]);
  }
  __forceinline void invoke_onTick ( Context * __context__, Func __funcCall__, void * self ) const {
    das_invoke_function<void>::invoke
      <void *>
        (__context__,nullptr,__funcCall__,
          self);
  }
};

