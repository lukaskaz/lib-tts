#!/bin/bash

EXPECTED_COVERAGE=80
MINIMUM_COVERAGE=$((EXPECTED_COVERAGE-20))
REDFONT="\033[0;31m"
GREENFONT="\033[0;32m"
YELLOWFONT="\033[0;33m"
NOCOLORFONT="\033[0m"

output=$(make -C build coverage 2>/dev/null | sed -n "/Overall/,/functions/p")
lines=$(echo "$output" | grep lines | sed -r "s/^.*lines.*: ([0-9.]+)%.*$/\1/g") 
funcs=$(echo "$output" | grep functions | sed -r "s/^.*functions.*: ([0-9.]+)%.*$/\1/g")
lines=${lines%%.*}
funcs=${funcs%%.*}

echo "Coverage LINES/funcs: ${lines}%/${funcs}%"

if [ $lines -ge $EXPECTED_COVERAGE ]; then echo -e "[${GREENFONT}GOOD${NOCOLORFONT}] Met expected tests coverage ${EXPECTED_COVERAGE}%"; exit 0
elif [ $lines -ge $MINIMUM_COVERAGE ]; then echo -e "[${YELLOWFONT}POOR${NOCOLORFONT}] Met minimum tests coverage ${MINIMUM_COVERAGE}%"; exit 0
else echo -e "[${REDFONT}TOOLOW${NOCOLORFONT}] Not met minimum test coverage of ${MINIMUM_COVERAGE}%"; exit 1; fi

