IF ((NOT DAS_SMT_INCLUDED) AND ((NOT ${DAS_SMT_DISABLED}) OR (NOT DEFINED DAS_SMT_DISABLED)))
    SET(DAS_SMT_INCLUDED TRUE)
    MESSAGE(STATUS "dasSMT module included.")

    SET(DAS_SMT_DIR ${PROJECT_SOURCE_DIR}/modules/dasSMT)

    SET(DAS_SMT_LIB_OUTPUT "${PROJECT_SOURCE_DIR}/lib")
    # Fixed name on every platform — must match the library="libz3.dll" in
    # generated bindings/z3_func.das [extern] annotations.
    SET(DAS_SMT_DLL_NAME libz3.dll)

    SET(DAS_Z3_TARGET_VERSION "4.15.4")
    # Sentinel carries a -r<N> revision on top of the Z3 version: the prebuilt
    # can change without a Z3 version bump.
    SET(DAS_Z3_SENTINEL_VERSION "${DAS_Z3_TARGET_VERSION}-r1")
    SET(DAS_SMT_REFETCH FALSE)
    IF (NOT EXISTS "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}")
        SET(DAS_SMT_REFETCH TRUE)
    ELSEIF (NOT EXISTS "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}.version")
        SET(DAS_SMT_REFETCH TRUE)
    ELSE()
        file(READ "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}.version" _das_z3_cached_ver)
        STRING(STRIP "${_das_z3_cached_ver}" _das_z3_cached_ver)
        IF (NOT _das_z3_cached_ver STREQUAL DAS_Z3_SENTINEL_VERSION)
            MESSAGE(STATUS "dasSMT: cached Z3 ${_das_z3_cached_ver} != target ${DAS_Z3_SENTINEL_VERSION}, refetching")
            SET(DAS_SMT_REFETCH TRUE)
        ENDIF()
    ENDIF()

    IF (DAS_SMT_REFETCH)
        SET(DAS_Z3_HAS_PREBUILT TRUE)
        IF(WIN32)
            IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ARM64|aarch64")
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-arm64-win")
                SET(DAS_Z3_FILE_HASH "318e108a5383b19a0a47133970c5d3e14c2ca08581539e2d283d526450893cb9")
            ELSE()
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-x64-win")
                SET(DAS_Z3_FILE_HASH "842ddadfd5a0612da8834da559502257895af88f103cd2c18ff76f3a4743e661")
            ENDIF()
            SET(DAS_Z3_SO_NAME "bin/libz3.dll")
        ELSEIF(APPLE)
            IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-arm64-osx-13.7.6")
                SET(DAS_Z3_FILE_HASH "8bb3439772cafd75240d61abf255e89122850bab93563d1283b048359ab4e88f")
            ELSE()
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-x64-osx-13.7.6")
                SET(DAS_Z3_FILE_HASH "2e36c8128781f3222099ef629a7aeb694dcc9b49531d4195128f610bd2d135c9")
            ENDIF()
            SET(DAS_Z3_SO_NAME "bin/libz3.dylib")
        ELSEIF(UNIX)
            IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm|aarch64")
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-arm64-glibc-2.34")
                SET(DAS_Z3_FILE_HASH "9e832578e28d9ed51a79b97948728a874854a3c38ee49e7aae05e7d6e0e93508")
            ELSE()
                SET(DAS_Z3_FILE "z3-${DAS_Z3_TARGET_VERSION}-x64-glibc-2.39")
                SET(DAS_Z3_FILE_HASH "a41b690e89c343931471506cdc6d957b6044a200fd2d240cc017432afdff7d3e")
            ENDIF()
            SET(DAS_Z3_SO_NAME "bin/libz3.so")
        ELSE()
            SET(DAS_Z3_HAS_PREBUILT FALSE)
        ENDIF()
        IF (NOT DAS_Z3_HAS_PREBUILT)
            MESSAGE(FATAL_ERROR
                "dasSMT: no prebuilt libz3 for this platform "
                "(${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}) at the "
                "z3-${DAS_Z3_TARGET_VERSION} release. To disable the module "
                "set -DDAS_SMT_DISABLED=ON.")
        ENDIF()
        SET(DAS_Z3_LINK https://github.com/Z3Prover/z3/releases/download/z3-${DAS_Z3_TARGET_VERSION}/${DAS_Z3_FILE}.zip)
        MESSAGE(STATUS "Download Z3 at: ${DAS_Z3_LINK}")
        include(FetchContent)

        FetchContent_Declare(
            das_z3_shared_lib
            URL_HASH               SHA256=${DAS_Z3_FILE_HASH}
            DOWNLOAD_EXTRACT_TIMESTAMP 1
            INACTIVITY_TIMEOUT 60
            DOWNLOAD_RETRIES 5
            URL ${DAS_Z3_LINK}
        )
        FetchContent_MakeAvailable(das_z3_shared_lib)

        configure_file("${das_z3_shared_lib_SOURCE_DIR}/${DAS_Z3_SO_NAME}"
            "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}"
            COPYONLY
        )
        file(WRITE "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}.version" "${DAS_Z3_SENTINEL_VERSION}")
        SET(DAS_Z3_INCLUDE_DIR "${das_z3_shared_lib_SOURCE_DIR}/include" CACHE PATH "Z3 C headers (bind_z3.das input)")
    ELSE()
        MESSAGE(STATUS "Z3 lib exists at: ${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}")
    ENDIF()

    IF(UNIX AND NOT APPLE)
        # DIRTY HACK — remove once fixed upstream: upstream libz3 ships without
        # -Bsymbolic (unlike libLLVM), so its internal Z3_* self-calls can be
        # preempted by a same-name system libz3 (e.g. apt libLLVM pulls libz3.so.4)
        # -> memory corruption. We post-patch DT_SYMBOLIC onto the binary. Proper
        # fix belongs upstream (Z3 released linked with -Bsymbolic), or in the
        # daslang loader (isolate dasbind [extern] libs from the global namespace).
        find_package(Python3 COMPONENTS Interpreter QUIET)
        IF(Python3_Interpreter_FOUND)
            # Insert a DT_SYMBOLIC tag into a spare DT_NULL slot of libz3's
            # .dynamic (idempotent; ELF64). No stdlib-only dep beyond python3.
            SET(_z3_symbolic_py "
import struct,sys
d=bytearray(open(sys.argv[1],'rb').read())
sho,she,shn=struct.unpack_from('<Q',d,0x28)[0],struct.unpack_from('<H',d,0x3a)[0],struct.unpack_from('<H',d,0x3c)[0]
o,n=next(struct.unpack_from('<QQ',d,sho+i*she+24) for i in range(shn) if struct.unpack_from('<I',d,sho+i*she+4)[0]==6)
e=[struct.unpack_from('<qQ',d,o+i*16) for i in range(n//16)]
sys.exit(0) if any(t==16 for t,_ in e) else None
z=[i for i,(t,_) in enumerate(e) if t==0]
sys.exit('no spare DT_NULL for DT_SYMBOLIC') if len(z)<2 else None
struct.pack_into('<qQ',d,o+z[0]*16,16,0); open(sys.argv[1],'wb').write(d)
")
            execute_process(
                COMMAND ${Python3_EXECUTABLE} -c "${_z3_symbolic_py}" "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}"
                RESULT_VARIABLE _z3_sym_rc OUTPUT_VARIABLE _z3_sym_out ERROR_VARIABLE _z3_sym_out)
            IF(_z3_sym_rc)
                MESSAGE(FATAL_ERROR "dasSMT: DT_SYMBOLIC patch failed: ${_z3_sym_out}")
            ENDIF()
        ELSE()
            MESSAGE(WARNING "dasSMT: python3 not found — libz3 left without DT_SYMBOLIC; "
                "coexistence with a system libz3 (e.g. via apt libLLVM) will corrupt memory")
        ENDIF()
    ENDIF()

    # Always put to single place, path and name should match with z3_func.das definitions
    install(FILES "${DAS_SMT_LIB_OUTPUT}/${DAS_SMT_DLL_NAME}" DESTINATION ${DAS_INSTALL_LIBDIR})
    install(FILES ${DAS_SMT_DIR}/LICENSE.TXT DESTINATION ${DAS_INSTALL_DOCDIR} RENAME Z3.LICENSE)

    file(GLOB DASSMT_SOURCES ${DAS_SMT_DIR}/daslib/*.das)
    install(FILES ${DASSMT_SOURCES} DESTINATION ${DAS_INSTALL_MODULESDIR}/dasSMT/daslib)
    file(GLOB DASSMT_BINDINGS ${DAS_SMT_DIR}/bindings/*.das)
    install(FILES ${DASSMT_BINDINGS} DESTINATION ${DAS_INSTALL_MODULESDIR}/dasSMT/bindings)
    install(FILES ${DAS_SMT_DIR}/.das_module DESTINATION ${DAS_INSTALL_MODULESDIR}/dasSMT/)

    ADD_EXAMPLE_RUN(modules/dasSMT/examples/hello_z3.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/hello_smtlib.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/hello_api.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/atlas_packing.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/entity_placement.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/theorems.das)
    ADD_EXAMPLE_RUN(modules/dasSMT/examples/smt_fn_errors.das FALSE)
    ADD_EXAMPLE_RUN(modules/dasSMT/tests/smt_lint_check.das FALSE)

ENDIF()
