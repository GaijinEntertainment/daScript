options indenting = 2
// array

def resize(var Arr:array<auto(numT)>;newSize:int)
  __builtin_array_resize(Arr,newSize,typeinfo(sizeof type numT))

def reserve(var Arr:array<auto(numT)>;newSize:int)
  __builtin_array_reserve(Arr,newSize,typeinfo(sizeof type numT))

def push(var Arr:array<auto(numT)>;value:numT;at:int=-1)
  Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] = value

def emplace(var Arr:array<auto(numT)>;value:numT;at:int=-1)
  Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] <- value

def erase(var Arr:array<auto(numT)>;at:int)
  __builtin_array_erase(Arr,at,typeinfo(sizeof type numT))

// table

def find(var Tab:table<auto(keyT),auto(valT)>;at:keyT;blk:block<(var p:valT?):void>)
  __builtin_table_lock(Tab)
  var val = __builtin_table_find(Tab,at)
  invoke(blk,val)
  __builtin_table_unlock(Tab)

def erase(var Tab:table<auto(keyT),auto(valT)>;at:keyT)
  return __builtin_table_erase(Tab,at)

def key_exists(var Tab:table<auto(keyT),auto(valT)>;at:keyT):bool
  return __builtin_table_key_exists(Tab,at)

options removeUnusedSymbols=false

[generic]
def peek ( src:das_string; subexpr:block<(root:string):void> ) 
  _builtin_peek(src,subexpr)

def binary_save(obj; subexpr:block<(data:string):void>)
  static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
  _builtin_binary_save(obj,subexpr)

def binary_load(var obj; data:string)
  static_assert(typeinfo(is_ref_type obj),"can only serialize ref types")
  _builtin_binary_load(obj,data)

def clone(var a:array<auto(TT)>;b:array<TT>)
  resize(a,length(b))
  for aV,bV in a,b
    aV := bV

def clone(var a:table<auto(KT),auto(VT)>;b:table<KT,VT>)
  clear(a)
  for k,v in keys(b),values(b)
    a[k] := v

def clone(var a:das_string;b:string)
  set(a,b)

def keys(a:table<auto(keyT),auto(valT)>) : iterator<keyT & const>
  return __builtin_table_keys(a,typeinfo(sizeof type keyT))

def values(a:table<auto(keyT),auto(valT)>) : iterator<valT &>
  return __builtin_table_values(a,typeinfo(sizeof type valT))

// string writer

[generic]
def build_string(blk:block<(var writer:StringBuilderWriter)>)
  return _builtin_build_string(blk)

options indenting = 0

