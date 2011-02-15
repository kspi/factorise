#!/bin/bash

fail=0

t() {
    local program="./factorise"
    local input="$1"
    shift
    local expected_output="$@"
    
    local output="$($program $input 2>&1)"
    local ret="$?"
    
    if [ $ret -eq 0 ] && ( echo "$output" | grep -q "$expected_output" )
    then
        printf "pass: %10s → %s\n" "$input" "$output"
    else
        printf "FAIL: %10s → %s\n" "$input" "$expected_output"
        echo   "    Actual output: $output"
        fail=1
    fi
}

t -1 'Number out of range'
t asd 'Invalid number'
t 0 0
t 1 1
t 2 2
t 17191 17191
t '-v 17191' Factoring
t 10 2 5
t 1010 2 5 101
t 10101 3 7 13 37
t 3000 2 2 2 3 5 5 5

if [ $fail -eq 1 ]
then
    echo "SOME TESTS FAILED."
    exit 1
else
    echo "All tests passed."
fi
