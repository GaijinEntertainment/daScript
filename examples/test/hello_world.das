options indenting = 2

options log_cpp

require daslib/json

struct JsonRpcReq

  js: JsonValue?
  id: double = 0lf
  reqType: JsonRpcReqType
  params: JsonValue?

class JsonRpcMatch
  requests: array<JsonRpcReq>

variant JsonRpcReqType
  none: void?
  method: string
  result: void?
  error: void?

def clean(var match:JsonRpcMatch)
  for it in match.requests
    it.params = null // it.js point to same json data
    delete it
  match.requests |> clear()

[export]
def main
  print("hello, world\n")
