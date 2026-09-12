# Proves the generated header's layout guard fires. The packed host must fail to compile, and it
# must fail ON a layout assert - any other error would let a broken guard pass this test.
#
# cmake -DCC=<c compiler> -DSRC=<host.c> -DINC=<generated dir> -DOUT=<object path> -P this

foreach(_var CC SRC INC OUT)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "expect_layout_assert.cmake: -D${_var} is required")
    endif()
endforeach()

execute_process(
    COMMAND ${CC} -std=c11 -I${INC} -c ${SRC} -o ${OUT}
    RESULT_VARIABLE _rc
    OUTPUT_VARIABLE _log
    ERROR_VARIABLE _log
)

if(_rc EQUAL 0)
    message(FATAL_ERROR
        "the packed host compiled: the generated header's layout asserts did not fire")
endif()

string(FIND "${_log}" "differs from the daslang layout" _found)
if(_found EQUAL -1)
    message(FATAL_ERROR "the packed host failed, but not on a layout assert:\n${_log}")
endif()

message(STATUS "the layout assert fired, as it must")
