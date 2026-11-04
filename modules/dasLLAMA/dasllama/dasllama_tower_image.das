options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tower_image shared public

require dasllama/dasllama_lint public
require daslib/apply
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_image
require dasllama/dasllama_env      // nolint:STYLE030 - `load_through_image` instances here: its body's names resolve in this module
require dasllama/dasllama_common
require dasllama/dasllama_kqformat   // kq_elems: the q8 block
require dasllama/dasllama_layout   // q8_gather_rows: the discrete homes' row-major gather
require dasllama/dasllama_math   // active_q8_repack_layout: the gather's plain layout
require strings                    // nolint:STYLE030 - ... `ends_with` among them
require daslib/strings_boost       // nolint:STYLE030 - ... the instances' string helpers
require daslib/fio                 // nolint:STYLE030 - ... `stat` / `remove` on the image path

//! The GPU-flavored image of the CPU-form model `cpu` (`#tower-dev-block`): the driver's `bake` over it into the device block,
//! then `gst` - a fresh staging of the family - carrying `cpu`'s meta under `dev`, every plane the bake did not convert
//! (`consumed`, less the bake's `kept`) copied by name, and the block; written through `img` ("" builds it in memory) and mapped into `out`. True only
//! where the FILE was written. `cpu` and `gst` are spent.
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_dev_image(var cpu : auto(T); var gst : auto(S); var out : T; img, tag : string; dev : TowerDev;
                    bake : function<(m : T; var b : TowerDevBuild) : bool>; consumed : array<string>; who : string) : bool {
    var inscope b : TowerDevBuild
    if (!invoke(bake, cpu, b)) {
        panic("{who}: the {dev} driver could not bake the model - its chain declined a model the load handed it")
    }
    tower_set_meta(gst, cpu, dev)
    var inscope dropped <- [for (c in consumed); c; where !has_value(b.kept, c)]
    tower_copy_planes(cpu, gst, dropped)
    tower_place_block(gst, b)
    delete cpu
    let wrote = cache_via_image_staged(gst, out, img, tag)
    delete gst
    return wrote
}

//! the staging's meta field - the one carrying an image mapping - a copy of `cpu` under the flavor `dev`, mapping nothing
def private tower_set_meta(var gst; cpu : auto(T); dev : TowerDev) {
    apply(gst) $ [unused_argument(name)] (name : string; var field) {
        static_if (typeinfo safe_has_field < image_map > (field)) {
            field := cpu
            field.image_map = null
            field.image_bytes = 0ul
            tower_mark_dev(field, dev)
        }
    }
}

//! A GPU-flavored model reached a CPU path at `stage`: the device seat declined what the load handed it, and the CPU never runs
//! that image (`#tower-dev-block`)
def tower_cpu_refuse(m; who, stage : string) {
    if (m.dev_backend != TowerDev.none) {
        panic("{who}: the {stage} declined on the {m.dev_backend} device - a {m.dev_backend}-flavored image never runs on the CPU (where this box's {m.dev_backend} driver is off or absent, load the source model: it maps the box's own image)")
    }
}

var private g_cpu_flavor_pin = false

//! The loads inside `blk` take every tower family's CPU image whatever the box's drivers serve - the offline planar bake
def with_cpu_flavor(blk : block<() : void>) {
    let was = g_cpu_flavor_pin
    g_cpu_flavor_pin = true
    invoke(blk)
    g_cpu_flavor_pin = was
}

//! a load stands under `with_cpu_flavor`: its flavor pick answers the CPU image
def cpu_flavor_pinned() : bool => g_cpu_flavor_pin

//! The device flavor a load takes under a family's flavor record `f` (its `backend` and `serves`), `lane` what its `serves`
//! reads (the q8 flag, or the family's lane meta): the registered driver's where its chain serves the lane, else `none` -
//! the CPU image
def tower_dev_flavor(f; lane) : TowerDev {
    return TowerDev.none if (cpu_flavor_pinned() || f.backend == TowerDev.none || !invoke(f.serves, lane))
    return f.backend
}

//! panics where `dev` is not the flavor `f` registered: a mint asked for an image no driver of the family serves
def tower_flavor_registered(f; dev : TowerDev; who : string) {
    if (f.backend != dev) {
        panic("{who}: a {dev}-flavored mint asked, the family's registered flavor is {f.backend}")
    }
}

//! the model's flavor set where it carries the device block: on the model, else on the nested tower that does
def private tower_mark_dev(var e; dev : TowerDev) {
    static_if (typeinfo safe_has_field < dev_backend > (e)) {
        e.dev_backend = dev
    }
    static_if (!typeinfo safe_has_field < dev_backend > (e)) {
        apply(e) $ [unused_argument(name)] (name : string; var field) {
            static_if (typeinfo safe_has_field < dev_backend > (field)) {
                field.dev_backend = dev
            }
        }
    }
}

//! the device block into the staging's own arrays, else into its nested tower's byte group
def private tower_place_block(var gst; var b : TowerDevBuild) {
    static_if (typeinfo safe_has_field < devplan > (gst)) {
        gst.dev <- b.blob
        gst.devplan <- b.plan
    }
    static_if (!typeinfo safe_has_field < devplan > (gst)) {
        apply(gst) $ [unused_argument(name)] (name : string; var field) {
            static_if (typeinfo safe_has_field < image_planes > (field)) {
                static_if (typeinfo safe_has_field < devplan > (field)) {
                    field.dev <- b.blob
                    field.devplan <- b.plan
                }
            }
        }
    }
}

//! every array plane of the staging `gst` - its nested towers' byte groups too, named `<group>.<plane>` - from the same-named
//! borrowed plane of `cpu`, but the device block's and the `consumed`
def private tower_copy_planes(cpu : auto(T); var gst : auto(S); consumed : array<string>) {
    apply(gst) $(sname : string; var sfield) {
        static_if (typeinfo is_array(sfield)) {
            static_if (!typeinfo is_string(sfield[0])) {
                if (sname != "dev" && sname != "devplan" && !has_value(consumed, sname)) {
                    tower_copy_named(cpu, sname, sfield)
                }
            }
        } static_elif (typeinfo safe_has_field < image_planes > (sfield)) {
            apply(sfield) $(iname : string; var ifield) {
                static_if (typeinfo is_array(ifield)) {
                    if (iname != "dev" && iname != "devplan" && !has_value(consumed, "{sname}.{iname}")) {
                        apply(cpu) $(mname : string; mfield) {
                            static_if (typeinfo safe_has_field < image_map > (mfield)) {
                                if (mname == sname) {
                                    tower_copy_named(mfield, iname, ifield)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//! the borrowed plane `name` of `m` copied into `dst` where its elements are `dst`'s
def private tower_copy_named(m; name : string; var dst : array<auto(ET)>) {
    apply(m) $(mname : string; mfield) {
        static_if (typeinfo safe_has_field < borrowed_plane > (mfield)) {
            if (mname == name && typeinfo sizeof(*mfield.p) == typeinfo sizeof(dst[0])) {
                plane_copy(mfield, dst)
            }
        }
    }
}

//! the q8 planes' regions gathered into the discrete homes' device layout (`wq` the quants, `ws` the halfword scales)
[cold_path]
def tower_gather_q8_regions(qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>; lay : Q8RepackLayout; var wq, ws : array<uint8>) {
    let q8e = kq_elems(KqFmt.q8)
    for (reg in regions) {
        q8_gather_rows(plane_at(qblob, reg.woff), null, plane_at(qscales, reg.woff / q8e), reg.n, reg.rows, q8_gather_slice(reg.rows, lay.mr), lay.mr, lay.kgroup, lay.wbias,
            wq, ws, reg.woff, reg.woff / q8e)
    }
}

//! the regions gathered at the row-major layout into the pieces `<prefix>.wq` / `<prefix>.ws`
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_bake_q8(var b : TowerDevBuild; prefix : string; qblob : PlaneI8; qscales : PlaneF; regions : array<Q8Region>) {
    var @exact_size wq : array<uint8>
    var @exact_size ws : array<uint8>
    wq |> reserve_resize(qblob.n)
    ws |> reserve_resize(qscales.n * 2l)
    tower_gather_q8_regions(qblob, qscales, regions, active_q8_repack_layout(false), wq, ws)
    tower_dev_put(b, "{prefix}.wq", wq)
    tower_dev_put(b, "{prefix}.ws", ws)
    delete wq
    delete ws
}

//! Register a lane's GPU-flavored image tags, one a driver, config-free: no CPU backend's property shapes their bytes
def register_tower_dev_tags(lane : string) {
    register_image_flavor_tag(tower_dev_tag(lane, TowerDev.metal), lane)
    register_image_flavor_tag(tower_dev_tag(lane, TowerDev.vulkan), lane)
    register_image_flavor_tag(tower_dev_tag(lane, TowerDev.cuda), lane)
}

//! `load_staged_image` taking a GPU flavor (`#tower-dev-block`): `dev` `none` loads the lane's CPU image; a driver's flavor
//! stages row-major (`with_tower_dev_staging`), mints the CPU form in memory and writes the driver's image under its tag
[unused_argument(carrier)]
def load_staged_image_dev(path, lane, who, source : string; carrier : auto(T); dev : TowerDev;
                          bake : function<(m : T; var b : TowerDevBuild) : bool>; consumed : array<string>; stage : block<() : auto(S)>) : T -const {
    return <- load_staged_image(path, lane, who, source, type<T>, stage) if (dev == TowerDev.none)
    let tag = tower_dev_tag(lane, dev)
    return <- load_through_image(path, tag, who, source, type<T>, $(img : string; var out : T) {
        var st <- tower_dev_staged(dev, stage)
        var gst = S()
        return tower_mint_dev(st, gst, out, img, lane, who, dev, bake, consumed)
    }, $ [unused_argument(t)] (t : T) => true, $ [unused_argument(t, src)] (t : T; src : string) {})
}

//! `st` (staged under `dev`) minted into `out` as the lane's `dev`-flavored image (`#tower-dev-block`): the CPU form, then the
//! driver's `bake` into `gst`, a fresh staging, through `img` ("" = in memory); true where the FILE was written, `st`/`gst` spent
[cold_path]
def tower_mint_dev(var st : auto(S); var gst : S; var out : auto(T); img, lane, who : string; dev : TowerDev;
                   bake : function<(m : T; var b : TowerDevBuild) : bool>; consumed : array<string>) : bool {
    var cpu = T()
    cache_via_image_staged(st, cpu, "", lane)
    delete st
    return tower_dev_image(cpu, gst, out, img, tower_dev_tag(lane, dev), dev, bake, consumed, who)
}

//! A suite's off-rail mint of `st` into `out` on the lane `lane` (`#tower-dev-block`): the CPU image where `dev` is `none`, else
//! the flavor `f` registered - a `dev` no driver registered for the family panics, the suite asked for an image nothing serves
def tower_mint_flavor(var st : auto(S); var out : auto(T); lane, who : string; dev : TowerDev; f; consumed : array<string>) {
    if (dev == TowerDev.none) {
        cache_via_image_staged(st, out, "", lane)
        return
    }
    tower_flavor_registered(f, dev, who)
    var gst = S()
    tower_mint_dev(st, gst, out, "", lane, who, dev, f.bake, consumed)
}
