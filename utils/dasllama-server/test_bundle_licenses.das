options gen2
options indenting = 4
require dastest/testing_boost public
require REVIEW

[test]
def component_metadata_does_not_become_a_bundled_license(t : T?) {
    for (suffix in ["COMPONENT UnicodeData RENAME UNICODE.LICENSE", "RENAME UNICODE.LICENSE COMPONENT LICENSE_COMPONENT"]) {
        let source = "install(FILES $\{PROJECT_SOURCE_DIR\}/3rdparty/unicode/LICENSE.txt DESTINATION $\{DAS_INSTALL_DOCDIR\} {suffix})"
        var inscope rows <- doc_root_licenses("CMakeLists.txt", source)
        t |> equal(length(rows), 1, "install metadata is not a source file")
        if (length(rows) != 1) return
        t |> equal(rows[0].source, "3rdparty/unicode/LICENSE.txt")
        t |> equal(rows[0].name, "UNICODE.LICENSE")
    }
}

[test]
def uppercase_notice_files_remain_in_the_bundle_inventory(t : T?) {
    let rows <- doc_root_licenses("modules/Example/CMakeLists.txt", "install(FILES LICENSE COPYING DESTINATION $\{DAS_INSTALL_DOCDIR\} COMPONENT Notices)")
    t |> equal(length(rows), 2)
    if (length(rows) != 2) return
    t |> equal(rows[0].source, "modules/Example/LICENSE")
    t |> equal(rows[0].name, "LICENSE")
    t |> equal(rows[1].source, "modules/Example/COPYING")
    t |> equal(rows[1].name, "COPYING")
}
