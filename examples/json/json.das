options indenting = 2
require RapidJson

def readJson ( src:string; subexpr:block<(var root:JsValue?;err:int):void> )
  _builtin_parse_json(src,subexpr)

def writeToJson ( src:auto; subexpr:block<(root:JsValue?):void> )
  _builtin_save_json(src,subexpr)


