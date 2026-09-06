# The watchdog exe supervising a daslang child that crashes once and then exits cleanly:
# supervision must end with 0, the log must carry one crash and two starts.
# Inputs: WATCHDOG, DASLANG, DAS_ROOT, WORK_DIR.
file(REMOVE_RECURSE "${WORK_DIR}")
file(MAKE_DIRECTORY "${WORK_DIR}/state")
execute_process(
    COMMAND "${WATCHDOG}"
        --program "${DASLANG}" --name smoke --cwd "${WORK_DIR}" --no-health
        --stable-seconds 0.1 --max-restart-delay 0.5
        -- -dasroot "${DAS_ROOT}" "${DAS_ROOT}/tests/watchdog/_fixture_watchdog_child.das"
        -- "${WORK_DIR}/state" crash-then-ok
    RESULT_VARIABLE rc
    OUTPUT_VARIABLE out
    ERROR_VARIABLE err
    TIMEOUT 90)
if(NOT rc EQUAL 0)
    message(FATAL_ERROR "watchdog exited ${rc}\n--- stdout ---\n${out}\n--- stderr ---\n${err}")
endif()
file(READ "${WORK_DIR}/logs/smoke-watchdog.log" log)
string(REGEX MATCHALL "\"event\": \"child_started\"" starts "${log}")
string(REGEX MATCHALL "\"event\": \"crash\"" crashes "${log}")
list(LENGTH starts n_starts)
list(LENGTH crashes n_crashes)
if(NOT n_starts EQUAL 2 OR NOT n_crashes EQUAL 1)
    message(FATAL_ERROR "expected 2 child_started and 1 crash, got ${n_starts} and ${n_crashes}\n${log}")
endif()
string(FIND "${log}" "\"event\": \"intentional_shutdown\"" shutdown_at)
if(shutdown_at EQUAL -1)
    message(FATAL_ERROR "no intentional_shutdown in the log\n${log}")
endif()
