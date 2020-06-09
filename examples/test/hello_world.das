require daslib/json
require daslib/json_boost

var
	jsv = %json~
		{"menu": {
		"id": "file",
		"value": "File",
		"popup": {
			"menuitem": [
			{"value": "New", "onclick": "CreateNewDoc()"},
			{"value": "Open", "onclick": "OpenDoc()"},
			{"value": "Close", "onclick": "CloseDoc()"}
			]
		}
		}}
    %%

[export]
def test()
	print("jsv = {write_json(jsv)}\n")
    return true

