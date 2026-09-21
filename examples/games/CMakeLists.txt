if(DAS_TOOLS_DISABLED)
    return()
endif()

set(_games_root ${PROJECT_SOURCE_DIR})
set(_games_daslang $<TARGET_FILE:daslang>)
set(_games_dastest ${_games_root}/dastest/dastest.das)

add_custom_target(check_example_games
    COMMAND ${_games_daslang} examples/games/REVIEW.das
    DEPENDS daslang
    WORKING_DIRECTORY ${_games_root}
    COMMENT "Checking the example games stay wired to the site"
)

add_das_target(run_tests_boulder_dash
    COMMAND ${_games_daslang} ${_games_dastest} -- --color --failures-only --test ${_games_root}/examples/games/boulder-dash
    COMMAND ${_games_daslang} ${_games_dastest} -- --color --failures-only --test ${_games_root}/web/examples/ui/samples/examples/boulder-dash
    DEPENDS daslang
    WORKING_DIRECTORY ${_games_root}
    COMMENT "Testing the boulder-dash samples (both copies)"
)

set(_games_modules)
foreach(_m dasModuleGlfw dasModuleLiveHost dasModuleHV dasModuleAudio dasModulePUGIXML dasModuleStbImage watchdog)
    if(TARGET ${_m})
        list(APPEND _games_modules ${_m})
    endif()
endforeach()
if(WIN32)
    set(_games_sequence_cmds COMMAND pwsh ${CMAKE_CURRENT_SOURCE_DIR}/sequence/ci_smoke_test.ps1 ${_games_root})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(_games_sequence_cmds COMMAND xvfb-run -a bash ${CMAKE_CURRENT_SOURCE_DIR}/sequence/ci_smoke_test.sh ${_games_root})
    foreach(_g arcanoid pacman boulder-dash river_run)
        list(APPEND _games_sequence_cmds
            COMMAND timeout 120 xvfb-run -a ${_games_daslang} examples/games/${_g}/main.das -- --max-frames 60)
    endforeach()
else()
    set(_games_sequence_cmds COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/sequence/ci_smoke_test.sh ${_games_root})
endif()
add_das_target(run_sequence_smoke
    ${_games_sequence_cmds}
    DEPENDS daslang ${_games_modules}
    WORKING_DIRECTORY ${_games_root}
    COMMENT "Sequence release smoke test (daspkg install -> release -> launch)"
)
