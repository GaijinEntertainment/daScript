// The stubs the cells' fake overrides register, with no fixture root beside them: a model-free cell requires this to seat an
// override that declines.

options gen2
options _dasllama_internal = true

module _override_probes shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common

//! a decode override's step that declines every call
[unused_argument(t, s, token, pos)]
def decode_declines(t : Model; var s : Session; token, pos : int64) : bool => false
