#! /bin/bash

set -e

USAGE=$(cat <<EOF
usage: $(basename "$0") [options]

Go utility script to handle Go things...

Add flags to tell the script to do more things; default behavior is to do nothing.

OPTIONS

    --build             Build the Go files in the current directory
    --filter=FILTER     sets a Go -run test filter
    --format            Runs the Go format utility
    --test              Run any unit tests defined in the current directory

  Unit Test Options
    --no-cache          disables the Go unit test cache; forces tests to run
    -|--verbose         enable verbose output
EOF
)

OPTS=()
LONGOPTS=(
    'build'
    'filter:'
    'format'
    'test'
    'no-cache'
)

opts="$(printf '%s' "${OPTS[@]}")vh"
longopts="$(printf '%s,' "${LONGOPTS[@]}")verbose,help"
if ! TEMP="$(getopt -o "${opts}" -l "${longopts}" -n "$0" -- "$@")"; then
    echo "${USAGE}" >&2
    exit 2
fi
eval set -- "${TEMP}"

BUILD=0
FORMAT=0
TEST=0
NO_CACHE=0
TEST_FILTER=""
VERBOSE=0


while true; do
    case "$1" in
    --build)
        BUILD=1
        shift 1
        ;;
    --format)
        FORMAT=1
        shift 1
        ;;
    --filter)
        TEST_FILTER=$2
        shift 2
        ;;
    --test)
        TEST=1
        shift 1
        ;;
    --no-cache)
        NO_CACHE=1
        shift 1
        ;;
    -v|--verbose) VERBOSE=1; shift 1;;
    -h|--help) echo "${USAGE}"; exit 0;;
    --) shift; break;;
    *) echo 'The script is broken: unhandled option' >&2; exit 2;;
    esac
done

if [[ "${FORMAT}" -ne 0 ]]; then
    gofmt -w ./
fi

if [[ "${BUILD}" -ne 0 ]]; then
    go build -o dist/advent-of-code day04.go
fi

VERBOSE_OPTION=""
if [[ "${VERBOSE}" -ne 0 ]]; then
    VERBOSE_OPTION=" -v"
fi

CACHE_CMD_TO_USE=""
if [[ "${NO_CACHE}" -ne 0 ]]; then
    CACHE_CMD_TO_USE=" -count=1 "
fi

TEST_FILTER_TO_USE=""
if [[ -n "${TEST_FILTER}" ]]; then
    TEST_FILTER_TO_USE=" -run ${TEST_FILTER} "
fi


if [[ "${TEST}" -ne 0 ]]; then
    echo "Unit Test Cmd: 'go test ${VERBOSE_OPTION}${TEST_FILTER_TO_USE}${CACHE_CMD_TO_USE} ./'"
    go test ${VERBOSE_OPTION}${TEST_FILTER_TO_USE}${CACHE_CMD_TO_USE}
fi
