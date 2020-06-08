require daslib/json
require daslib/json_enh

[export]
def test
    var error = ""
    var jsv = %_esc~1234%_esc
    print("jsv=`{jsv}`\n")
    return true
