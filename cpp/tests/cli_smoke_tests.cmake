if(NOT DEFINED PROGRAM)
    message(FATAL_ERROR "PROGRAM must point to the subnet calculator executable.")
endif()

function(run_cli_case name expected_exit expected_text)
    execute_process(
        COMMAND "${PROGRAM}" ${ARGN}
        RESULT_VARIABLE actual_exit
        OUTPUT_VARIABLE standard_output
        ERROR_VARIABLE standard_error
    )

    if(NOT "${actual_exit}" STREQUAL "${expected_exit}")
        message(FATAL_ERROR
            "${name}: expected exit ${expected_exit}, got ${actual_exit}.\n"
            "stdout:\n${standard_output}\n"
            "stderr:\n${standard_error}")
    endif()

    string(CONCAT combined_output "${standard_output}" "${standard_error}")
    string(FIND "${combined_output}" "${expected_text}" match_position)

    if(match_position EQUAL -1)
        message(FATAL_ERROR
            "${name}: expected output fragment not found: ${expected_text}\n"
            "stdout:\n${standard_output}\n"
            "stderr:\n${standard_error}")
    endif()
endfunction()

run_cli_case(
    valid_direct
    0
    "Network address:   192.168.10.0"
    "192.168.10.42/24"
)

run_cli_case(
    invalid_direct
    1
    "Error: IPv4 octet out of range (0-255): 300"
    "192.168.1.300/24"
)

run_cli_case(
    help
    0
    "Usage:"
    "--help"
)

run_cli_case(
    incorrect_usage
    1
    "Error: Expected zero or one argument."
    "192.168.10.42/24"
    "extra"
)

message(STATUS "C++ CLI smoke tests passed: 4")
