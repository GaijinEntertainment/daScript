options gen2
options _dasllama_internal = true

module _model_once shared public

require dastest/testing_boost public
require daslib/fio   // path_join
require _model_tier public   // model_available, models_dir
require _model_form public   // model_once_at: the block's loop behind its gate; own_load: a load the block cannot hold
require _kernel_census   // kernel_census_write: a form's census file

//! The run area's one load of a model file that is no chat model (a
//! TTS voice, a front-end pack, an ASR model, a tower): `file` under
//! `root`, a form at a time. `load`, the family's form loader, builds
//! the model of each form of `forms` from the file's path - the knobs
//! a form pins are the load's to set and put back - and `cells` runs
//! that form's cells on it before it drops, so no two forms of the
//! file stand in memory together and each loads once. A file the box
//! lacks skips loudly, once, and the call answers false. A compare of
//! two forms is made after the call, on what the cells collected.
//! `area` and `file` are string literals on the call's first line:
//! `REVIEW.das` reads them to hold one call an area and file across
//! the folder. After each form's cells the process's census files are
//! written (`kernel_census_write`).
def with_model_once_at(t : T?; area, file : string; forms : array<string>; root : string;
                       load : function<(path : string; form : string) : auto(MODEL)>;
                       cells : block<(var m : MODEL; form : string) : void>) : bool {
    let path = path_join(root, file)
    return model_once_at(t, area, file, path, forms, @@model_available,
                         load) $(var m : MODEL; form) {
        invoke(cells, m, form)
        kernel_census_write()
    }
}

//! `with_model_once_at` for a file under the models directory
def with_model_once(t : T?; area, file : string; forms : array<string>;
                    load : function<(path : string; form : string) : auto(MODEL)>;
                    cells : block<(var m : MODEL; form : string) : void>) : bool =>
    with_model_once_at(t, area, file, forms, models_dir(), load, cells)

//! `with_model_once_at` for a file several of whose forms are minted from one staged read (`staged_once_at`): `key` names
//! the staged read a form is minted from, "" for a form that loads on its own; `stage` reads the file once a key; `load`
//! builds a form's model from its own copy of that staging, which it may cut and poison
def with_model_once_at(t : T?; area, file : string; forms : array<string>; root : string;
                       key : function<(form : string) : string>;
                       stage : function<(path : string; key : string) : auto(STG)>;
                       load : function<(path : string; form : string; var stg : STG) : auto(MODEL)>;
                       cells : block<(var m : MODEL; form : string) : void>) : bool {
    let path = path_join(root, file)
    return staged_once_at(t, area, file, path, forms, @@model_available, key, stage,
                          load) $(var m : MODEL; form) {
        invoke(cells, m, form)
        kernel_census_write()
    }
}

//! the staged `with_model_once_at` for a file under the models directory
def with_model_once(t : T?; area, file : string; forms : array<string>;
                    key : function<(form : string) : string>;
                    stage : function<(path : string; key : string) : auto(STG)>;
                    load : function<(path : string; form : string; var stg : STG) : auto(MODEL)>;
                    cells : block<(var m : MODEL; form : string) : void>) : bool =>
    with_model_once_at(t, area, file, forms, models_dir(), key, stage, load, cells)
