options gen2
options _dasllama_internal = true

module _model_once shared public

require dastest/testing_boost public
require daslib/fio   // path_join
require _model_tier public   // model_available, models_dir
require _model_form public   // model_once_at: the block's loop behind its gate; own_load: a load the block cannot hold

//! The run area's one load of a model file that is no chat model (a TTS voice, a front-end pack, an ASR model, a
//! tower): `file` under `root`, a form at a time. `load`, the family's form loader, builds the model of each form
//! of `forms` from the file's path - the knobs a form pins are the load's to set and put back - and `cells` runs
//! that form's cells on it before it drops, so no two forms of the file stand in memory together and each loads
//! once. A file the box lacks skips loudly, once, and the call answers false. A compare of two forms is made after
//! the call, on what the cells collected. `area` and `file` are string literals on the call's first line:
//! `REVIEW.das` reads them to hold one call an area and file across the folder.
def with_model_once_at(t : T?; area, file : string; forms : array<string>; root : string;
                       load : function<(path : string; form : string) : auto(MODEL)>;
                       cells : block<(var m : MODEL; form : string) : void>) : bool =>
    model_once_at(t, area, file, path_join(root, file), forms, @@model_available, load, cells)

//! `with_model_once_at` for a file under the models directory
def with_model_once(t : T?; area, file : string; forms : array<string>;
                    load : function<(path : string; form : string) : auto(MODEL)>;
                    cells : block<(var m : MODEL; form : string) : void>) : bool =>
    with_model_once_at(t, area, file, forms, models_dir(), load, cells)
