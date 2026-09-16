options gen2
options gc
options persistent_heap
require gltf/gltf_processed
require daslib/fio
require daslib/clargs
require strings

def private convert_inputs(inputs : array<string>; root, output : string; binary, force : bool; profile : ProcessingProfile; var catalog : ProcessedCatalog) : int2 {
    var converted = 0; var skipped = 0
    for (path in inputs){
        var error : string; let rel = empty(root) ? base_name(path) : relative(path, root, error)
        if (!empty(error) || starts_with(rel, "../")){panic("Input outside --root: {path}")}
        let extension = binary ? ".das_glb" : ".das_gltf"
        let destination = "{output}/{slice(rel,0,length(rel)-(ends_with(rel,".gltf")?5:4))}{extension}"
        var asset <- load_and_process_gltf(path, profile)
        var unchanged = false
        if (!force){
            fopen(destination, "rb") $(f){if (f != null){
                try{var previous <- load_processed(destination); unchanged = previous.key == asset.key; delete previous}recover{unchanged = false}
            }}
        }
        if (unchanged){skipped++}
        else{
            if (!save_processed(asset, destination, output)){panic("Could not write {destination}")}
            converted++
        }
        print("{converted+skipped}/{length(inputs)} {unchanged?"cached":"converted"}: {rel}; vertices {asset.source_vertices}->{asset.processed_vertices}; textures {length(asset.images)} ({asset.texture_hits} cached)\n")
        let asset_path = relative(destination, output, error)
        var found = false
        for (entry in catalog.assets){if (entry.path == asset_path){entry.key = asset.key; found = true; break}}
        if (!found){catalog.assets |> push(ProcessedCatalogEntry(path = asset_path, key = asset.key))}
        delete asset
    }
    return int2(converted, skipped)
}
[export]
def main(){
    var args <- get_user_args(); var inputs : array<string>
    var root = ""; var output = ""; var cache = ".jitted_scripts/assets/textures"
    var binary = false; var force = false; var i = 0
    while (i < length(args)){
        let arg = args[i++]
        if (arg == "--root" || arg == "--out" || arg == "--cache"){
            if (i >= length(args)){panic("Missing value after {arg}")}
            let value = args[i++]
            if (arg == "--root"){root = clone(value)}elif (arg == "--out"){output = clone(value)}else{cache = clone(value)}
        }elif (arg == "--self-contained"){binary = true}
        elif (arg == "--force"){force = true}
        elif (arg == "--help"){
            print("convert.das -- --root SOURCE --out DEST [--cache DIR] [--force] [--self-contained] [FILES...]\n")
            return
        }else{inputs |> push(clone(arg))}
    }
    if (empty(output)){panic("Specify --out; original assets are never overwritten")}
    if (empty(inputs)){
        if (empty(root)){panic("Specify --root or input files")}
        dir_rec(root) $(file, is_dir){if (!is_dir && (ends_with(file, ".gltf") || ends_with(file, ".glb"))){inputs |> push("{root}/{file}")}}
    }
    if (empty(inputs)){panic("No glTF or GLB files found") }
    sort(inputs); mkdir_rec(output)
    var profile = ProcessingProfile(texture_cache = cache)
    var catalog : ProcessedCatalog
    sscan_json(fread("{output}/asset-build.json"), catalog); catalog.version = DAS_GLTF_VERSION
    let totals = convert_inputs(inputs, root, output, binary, force, profile, catalog)
    sort(catalog.assets) $(a, b) => a.path < b.path
    if (!fwrite("{output}/asset-build.json", sprint_json(catalog, false))){panic("Could not write asset build manifest")}
    delete catalog
    print("Done: {totals.x} converted, {totals.y} unchanged. Output: {output}\n")
    delete args; delete inputs; delete profile
}
