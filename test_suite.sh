#!/bin/bash
shopt -s globstar

FOLDER="./ROMs"

all_success=true

for FILE in "$FOLDER"/**/*; do
    if [[ -f "$FILE" ]]; then
	if ./chip8 "$FILE"; then
	    echo "Success: '$FILE' returned 0."
	else
	    echo "Failure: '$FILE' did not return 0."
	fi
    fi
done

if $all_success; then
    echo "All tests passed."
    exit 0

else
    echo "One or more tests failed."
    exit 1
fi
