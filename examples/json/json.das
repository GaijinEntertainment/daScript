options indenting = 2
require RapidJson

def readJson ( src:string; subexpr:block<(var root:JsValue const?;err:int):void> )
  _builtin_parse_json(src,subexpr)

def writeToJson ( src:auto; subexpr:block<(root:JsValue const?):void> )
  _builtin_save_json(src,subexpr)


