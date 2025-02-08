#!/bin/bash
set -e

APP=../project/main
INPUT_DIR=inputs
OUTPUT_DIR=outputs
TEMP_OUT=temp.out

fail=0

for infile in "$INPUT_DIR"/*.in.txt; do
    testname=$(basename "$infile" .in.txt)
    expected="$OUTPUT_DIR/${testname}.out.txt"
    echo "Running test: $testname"
    
    $APP "$infile" > "$TEMP_OUT"
    
    if diff -u "$expected" "$TEMP_OUT" > /dev/null; then
        echo "Test $testname passed."
    else
        echo "Test $testname failed. Differences:"
        diff -u "$expected" "$TEMP_OUT"
        fail=1
    fi
done

rm -f "$TEMP_OUT"
exit $fail
