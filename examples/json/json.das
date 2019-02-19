options indenting = 2
require RapidJson

[export]
def readJson ( src:string; subexpr:block<(root:JsValue?;err:int):void> )
  _builtin_parse_json(src,subexpr)

[export]
def writeToJson ( src:auto; subexpr:block<(root:JsValue?):void> )
  _builtin_save_json(src,subexpr)


