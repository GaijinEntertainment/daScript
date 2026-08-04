options gen2
options indenting = 4

module curated_import public

require samples_store public
require daslib/json_boost
require daslib/fio
require daslib/logger
require daslib/strings_boost
require strings

//! The curated importer: reads the deployed playground manifest
//! (`<samples_dir>/data.json`, the hand-authored schema the UI ships) and
//! feeds every entry through the store — single-file entries as raw source,
//! multi-file entries as the same {files, active} JSON bundle user shares
//! use. Idempotent: unchanged files are dedup hits; changed files mint new
//! hashes and the listing repoints; vanished entries get demoted (rows and
//! permalinks always survive). Filesystem reads live here, not in the store.

def private is_contained_relative_path(rel : string) : bool {
    //! Manifest paths must stay under the samples dir. path_join is
    //! std::filesystem's operator/=, which DISCARDS the base when the right
    //! side is absolute, so "/etc/passwd" would read that file verbatim and
    //! publish it as a permanent public sample; "../" would climb out the same
    //! way. The manifest ships from the site deploy — a different trust domain
    //! from this service — so containment is checked, not assumed.
    // ':' rejects a drive-qualified path ("C:/..."), which is absolute too
    if (empty(rel) || rel |> starts_with("/") || rel |> starts_with("\\") || find(rel, ":") >= 0) {
        return false
    }
    for (part in split(replace(rel, "\\", "/"), "/")) {
        if (part == "..") {
            return false
        }
    }
    return true
}

def private entry_body(samples_dir : string; entry : JsonValue?; var primary_path : string&) : string {
    //! "" = a file was missing, unreadable, or escaped the samples dir (entry
    //! skipped by the caller).
    let fv = entry?["files"]
    if (fv == null || !(fv.value is _array)) {
        return ""
    }
    var rels <- [for (fe in fv.value as _array); fe ?? ""]
    if (empty(rels) || empty(rels[0])) {
        return ""
    }
    primary_path = rels[0]
    var texts : array<string>
    texts |> reserve(length(rels))
    for (rel in rels) {
        if (!is_contained_relative_path(rel)) {
            logger_error("playground.import", "refusing path outside {samples_dir}: {rel}")
            return ""
        }
        let text = fread(path_join(samples_dir, rel))
        if (empty(text)) {
            logger_error("playground.import", "cannot read {rel} under {samples_dir}")
            return ""
        }
        texts |> push(text)
    }
    if (length(texts) == 1) {
        return texts[0]
    }
    var files_obj <- {for (rel, text in rels, texts); base_name(rel) => JV(text)}
    return write_json(JV((files = JV(files_obj), active = base_name(rels[0]))))
}

def import_curated(db : SqlRunner; samples_dir : string; now : int64) : int {
    //! Returns the listed-entry count, or -1 when the manifest is missing or
    //! malformed (existing listings are left untouched in that case).
    let manifest_path = path_join(samples_dir, "data.json")
    let body = fread(manifest_path)
    if (empty(body)) {
        logger_error("playground.import", "cannot read manifest {manifest_path}")
        return -1
    }
    var jerr = ""
    var root = read_json(body, jerr)
    if (root == null || !(root.value is _object)) {
        logger_error("playground.import", "bad manifest {manifest_path}: {jerr}")
        return -1
    }
    var keep : table<string>
    var listed = 0
    var skipped = 0
    var position = 0
    for (category in keys(root.value as _object)) {
        let entries = root?[category]
        if (entries == null || !(entries.value is _array)) {
            continue
        }
        for (entry in entries.value as _array) {
            let title : string = entry?["name"] ?? ""
            if (empty(title)) {
                skipped ++
                continue
            }
            var primary_path = ""
            let src = entry_body(samples_dir, entry, primary_path)
            if (empty(src)) {
                logger_error("playground.import", "skipping '{title}': unreadable files")
                skipped ++
                continue
            }
            let hash = upsert_curated_source(db, src, now)
            list_curated_sample(db, hash, title, category, primary_path, entry?["slug"] ?? "", position)
            keep |> insert(hash)
            listed ++
            position ++
        }
    }
    if (listed == 0) {
        // Nothing was readable, yet the manifest parsed: a half-written deploy
        // tree, a `current` symlink swapped mid-import, or curated_dir pointing
        // one directory off. Demoting against an empty keep set would unlist
        // the whole dropdown, so treat it the same as an unreadable manifest —
        // prior listings stand.
        logger_error("playground.import", "manifest parsed but no entry was readable; keeping prior listings",
            JV((skipped = skipped, dir = samples_dir)))
        return -1
    }
    let demoted = demote_stale_curated(db, keep)
    logger_info("playground.import", "curated import: {listed} listed, {skipped} skipped, {demoted} demoted",
        JV((listed = listed, skipped = skipped, demoted = demoted, dir = samples_dir)))
    return listed
}
