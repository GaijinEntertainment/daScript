require RapidJson

[export]
def readJson ( src:string; subexpr:block<(root:JsValue?;err:int):void> )
	_builtin_parse_json(src,subexpr)
