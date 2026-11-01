options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tower_image shared public

require dasllama/dasllama_lint public
require daslib/apply
require dasllama/dasllama_plane
require dasllama/dasllama_tower
require dasllama/dasllama_image
require dasllama/dasllama_env      // `load_through_image` instances here: its body's names resolve in this module
require dasllama/dasllama_common
require strings
require daslib/strings_boost
require daslib/fio

//! The GPU-flavored image of the CPU-form model `cpu` (`#tower-dev-block`): the driver's `bake` over it into the device block,
//! then `gst` - a fresh staging of the family - carrying `cpu`'s meta under `dev`, every plane the bake did not convert
//! (`consumed`) copied by name, and the block; written through `img` ("" builds it in memory) and mapped into `out`. True only
//! where the FILE was written. `cpu` and `gst` are spent.
[cold_path, arch(at = "../ARCHITECTURE_IMAGE_GPU_PLANES.md#tower-dev-block")]
def tower_dev_image(var cpu : auto(T); var gst : auto(S); var out : T; img, tag : string; dev : TowerDev;
                    bake : function<(m : T; var b : TowerDevBuild) : bool>; consumed : array<string>; who : string) : bool {
    var b : TowerDevBuild
    if (!invoke(bake, cpu, b)) {
        panic("{who}: the {dev} driver could not bake the model - its chain declined a model the load handed it")
    }
    tower_set_meta(gst, cpu, dev)
    tower_copy_planes(cpu, gst, consumed)
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
        panic("{who}: the {stage} declined on the {m.dev_backend} device - a {m.dev_backend}-flavored image never runs on the CPU")
    }
}

//! The device flavor a load on the `q8` lane takes under a family's flavor record `f` (its `backend` and `serves`): the
//! registered driver's where its chain serves the lane, else `none` - the CPU image
def tower_dev_flavor(f; q8 : bool) : TowerDev {
    return TowerDev.none if (f.backend == TowerDev.none || !invoke(f.serves, q8))
    return f.backend
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

//! Register a lane's GPU-flavored image tags, one a driver, config-free: no CPU backend's property shapes their bytes
def register_tower_dev_tags(lane : string) {
    register_image_family_tag(tower_dev_tag(lane, TowerDev.metal), true)
    register_image_family_tag(tower_dev_tag(lane, TowerDev.vulkan), true)
}

//! `load_staged_image` taking a GPU flavor (`#tower-dev-block`): `dev` `none` loads the lane's CPU image; a driver's flavor
//! stages row-major (`with_tower_dev_staging`), mints the CPU form in memory and writes the driver's image under its tag
[unused_argument(carrier)]
def load_staged_image_dev(path, lane, who, source : string; carrier : auto(T); dev : TowerDev;
                          bake : function<(m : T; var b : TowerDevBuild) : bool>; consumed : array<string>; stage : block<() : auto(S)>) : T -const {
    return <- load_staged_image(path, lane, who, source, type<T>, stage) if (dev == TowerDev.none)
    let tag = tower_dev_tag(lane, dev)
    return <- load_through_image(path, tag, who, source, type<T>, $(img : string; var out : T) {
        var st = S()
        with_tower_dev_staging(dev) {
            st <- invoke(stage)
        }
        var cpu = T()
        cache_via_image_staged(st, cpu, "", lane)
        delete st
        var gst = S()
        return tower_dev_image(cpu, gst, out, img, tag, dev, bake, consumed, who)
    }, $ [unused_argument(t)] (t : T) => true, $ [unused_argument(t, src)] (t : T; src : string) {})
}
