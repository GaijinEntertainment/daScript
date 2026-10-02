options gen2
require dastest/testing_boost public
require daslib/logger
require daslib/json_boost
require daslib/fio

[test]
def checked_logger_reports_output_and_recoverable_open_failure(t : T?) {
    var error : string
    let root = create_temp_directory("logger-checked-", error)
    t |> equal(error, "")
    logger_close(); logger_set_stderr_fallback(false); logger_clear_category_levels()
    logger_set_min_level(LOG_INFO)
    logger_set_path(root)
    t |> success(!logger_try_log(LOG_INFO, "test", "cannot append to a directory"))
    logger_close()
    let path = path_join(root, "events.jsonl")
    logger_set_path(path)
    t |> success(!logger_try_log(LOG_DEBUG, "test", "filtered"))
    t |> success(!fexist(path), "filtered records do not open the file")
    var fields = JV((code = 7))
    t |> success(logger_try_log(LOG_INFO, "test", "Привет", fields)) // nolint:STYLE039 - UTF-8 log round-trip fixture
    delete_json(fields)
    logger_close()
    var value = read_json(fread(path), error)
    t |> equal(error, "")
    t |> success(value?["msg"].value is _string)
    t |> equal(value?["msg"].value as _string, "Привет") // nolint:STYLE039 - UTF-8 log round-trip fixture
    delete_json(value)
    logger_set_path(""); remove(path); rmdir(root)
}

[test]
def checked_flush_reports_null_and_prior_write_error(t : T?) {
    t |> success(!try_fflush(null))
    var error : string
    let root = create_temp_directory("flush-checked-", error)
    t |> equal(error, "")
    let path = path_join(root, "data")
    t |> success(fwrite(path, "original"))
    fopen(path, "rb") $(file) {
        t |> success(try_fflush(file))
        let bytes = [1u8]
        t |> equal(fwrite(file, bytes), 0, "writing a read-only stream fails")
        t |> success(!try_fflush(file), "stream errors are not reported as a successful flush")
    }
    t |> equal(fread(path), "original")
    remove(path); rmdir(root)
}
