// The weight-format census both homes' cells run: each site's tag on a loaded model against the file's own tensor type, one
// tally a site, so a cell asserts every site native and lists the sites still read through a converted copy.

options gen2
options _dasllama_internal = true

module _format_census shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_common   // Model, fmt_at
require dasllama/dasllama_ple   // ple_plane
require dasllama/dasllama_gguf   // GGUFMeta, parse_gguf_meta, gguf_tensor_type, ggml_type_name
require dasllama/dasllama_kqformat   // kq_ggml_type, GGML_TYPE_F32, GGML_TYPE_MXFP4
require daslib/fio

//! one site's tally: how many of its planes the file carries and how many the load serves in another form
struct SiteCensus {
    site : string
    planes : int
    copies : int
    first : string
}

//! one plane `name` of the file against the tag `tag` the load gave it; `f32_native`: an F32 tensor on disk is served as the
//! file's f32 rows (the beta/alpha planes on the f32 arm, which carry no tag); `mx4_native`: an MXFP4 tensor on disk is served as
//! the native-MXFP4 stacks (`experts_mx4`, a stack property, no tag)
def census_plane(var c : SiteCensus; m : GGUFMeta; name : string; tag : KqFmt; f32_native : bool = false; mx4_native : bool = false) {
    return if (gguf_find_tensor(m, name) < 0)
    c.planes++
    let disk = gguf_tensor_type(m, name)
    return if ((f32_native && disk == GGML_TYPE_F32) || (mx4_native && disk == GGML_TYPE_MXFP4))
    if (kq_ggml_type(tag) != disk) {
        if (c.copies == 0) {
            c.first = "{name}: {ggml_type_name(disk)} on disk, served as {tag}"
        }
        c.copies++
    }
}

//! every site's tally over the loaded model `tr` against the file's tensor types; a tied classifier is the token table's plane, the
//! shared expert the plane its sites read (`shexp_plane`)
def format_census(tr : Model; m : GGUFMeta) : array<SiteCensus> {
    var attn = SiteCensus(site = "attention q/k/v/o")
    var exps = SiteCensus(site = "routed experts")
    var shexp = SiteCensus(site = "shared expert")
    var dn = SiteCensus(site = "deltanet qkv/gate/out")
    var ba = SiteCensus(site = "deltanet beta/alpha")
    var emb = SiteCensus(site = "token table")
    var cls = SiteCensus(site = "classifier")
    var eh = SiteCensus(site = "NextN eh_proj")
    var ple = SiteCensus(site = "per-layer embeddings")
    let c = tr.config
    if (has_ple(c)) {   //! the gate and projection planes a layer (an F32 kind served as the file's f32 rows) and the token table
        for (l in range64(c.n_layers)) {
            census_plane(ple, m, "blk.{l}.inp_gate.weight", ple_plane(tr, l, false).f, tr.ple_gate_f32)
            census_plane(ple, m, "blk.{l}.proj.weight", ple_plane(tr, l, true).f, tr.ple_proj_f32)
        }
        census_plane(ple, m, "per_layer_token_embd.weight", tr.ple_emb_fmt)
    }
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        census_plane(attn, m, "blk.{l}.attn_q.weight", fmt_at(tr.wq_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_k.weight", fmt_at(tr.wk_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_v.weight", fmt_at(tr.wv_fmt, l))
        census_plane(attn, m, "blk.{l}.attn_output.weight", fmt_at(tr.wo_fmt, l))
        census_plane(exps, m, "blk.{l}.ffn_gate_exps.weight", fmt_at(tr.we1_fmt, l), false, tr.experts_mx4)
        census_plane(exps, m, "blk.{l}.ffn_up_exps.weight", fmt_at(tr.we3_fmt, l), false, tr.experts_mx4)
        census_plane(exps, m, "blk.{l}.ffn_down_exps.weight", fmt_at(tr.we2_fmt, l), false, tr.experts_mx4)
        census_plane(exps, m, "blk.{l}.ffn_gate_up_exps.weight", fmt_at(tr.we1_fmt, l), false, tr.experts_mx4)   //! the fused stack (gemma-4): both halves' tags
        census_plane(exps, m, "blk.{l}.ffn_gate_up_exps.weight", fmt_at(tr.we3_fmt, l), false, tr.experts_mx4)
        if (c.n_ff_shexp > 0l) {
            census_plane(shexp, m, "blk.{l}.ffn_gate_shexp.weight", shexp_plane(tr, l, 1).f)
            census_plane(shexp, m, "blk.{l}.ffn_up_shexp.weight", shexp_plane(tr, l, 3).f)
            census_plane(shexp, m, "blk.{l}.ffn_down_shexp.weight", shexp_plane(tr, l, 2).f)
        }
        census_plane(dn, m, "blk.{l}.attn_qkv.weight", fmt_at(tr.dnqkv_fmt, l))
        census_plane(dn, m, "blk.{l}.attn_gate.weight", fmt_at(tr.dngate_fmt, l))
        census_plane(dn, m, "blk.{l}.ssm_out.weight", fmt_at(tr.dnout_fmt, l))
        census_plane(ba, m, "blk.{l}.ssm_beta.weight", fmt_at(tr.dnbeta_fmt, l), tr.dn_ba_f32)
        census_plane(ba, m, "blk.{l}.ssm_alpha.weight", fmt_at(tr.dnalpha_fmt, l), tr.dn_ba_f32)
    }
    census_plane(emb, m, "token_embd.weight", tr.emb_fmt)
    census_plane(cls, m, c.shared_weights ? "token_embd.weight" : "output.weight", cls_fmt_of(tr))
    census_plane(eh, m, "blk.{c.n_layers}.nextn.eh_proj.weight", tr.mtp_ehproj_fmt)
    return <- [attn, exps, shexp, dn, ba, emb, cls, eh, ple]
}

//! every site the census tallies but the E-series' per-layer embeddings, in its order: what a cell on a file carrying them all wants
def census_sites_all() : array<string> {
    return <- ["attention q/k/v/o", "routed experts", "shared expert", "deltanet qkv/gate/out", "deltanet beta/alpha", "token table",
        "classifier", "NextN eh_proj"]
}

//! the census of `tr` asserted site by site under `tag` (the file at `path`) - every site the file carries read in its own
//! format, the sites `want` names carried at all - each check soft so the cell names every site still on a converted copy,
//! that list logged as one line
def census_cell(t : T?; tr : Model; path, tag : string; want : array<string>) {
    var inscope every : array<string>
    census_cell(t, tr, path, tag, want, every)
}

//! the same census binding the own-format duty to the sites `bound` names alone (every site when empty): a carrier whose
//! other sites the tier serves off a converted form by design - a BF16 plane as q8 - holds its own-bytes sites to the duty
def census_cell(t : T?; tr : Model; path, tag : string; want, bound : array<string>) {
    let f = fopen(path, "rb")
    if (f == null) {
        t |> failure("{tag}: cannot open {path}")
        return
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        var inscope sites <- format_census(tr, m)
        var copies = ""
        for (s in sites) {
            if (want |> has_value(s.site)) {
                t |> success(s.planes > 0, "{tag} {s.site}: the file carries the site")
            }
            continue if (s.planes == 0 || (!empty(bound) && !(bound |> has_value(s.site))))
            t |> success(s.copies == 0, "{tag} {s.site}: {s.copies} of {s.planes} planes read through a converted copy ({s.first})")
            if (s.copies > 0) {
                copies = "{copies} [{s.site}: {s.copies}/{s.planes}]"
            }
        }
        to_log(LOG_INFO, "{tag}: sites still on a converted copy:{empty(copies) ? " none" : copies}\n")
    }
    fclose(f)
}
