package main

import (
	_ "embed"
	"fmt"
	"strconv"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

func getRangeStartStop(input string) (int, int) {
	strElems := strings.Split(input, "-")
	startInt, _ := strconv.Atoi(strElems[0])
	stopInt, _ := strconv.Atoi(strElems[1])
	return startInt, stopInt
}

func rangeFullyIncludedByPartner(aStart int, aStop int, bStart int, bStop int) bool {
	if aStart >= bStart && aStop <= bStop {
		return true
	} else if bStart >= aStart && bStop <= aStop {
		return true
	} else {
		return false
	}
}

func rangePartiallyOverlapsWithPartner(aStart int, aStop int, bStart int, bStop int) bool {
	if aStart >= bStart && aStart <= bStop {
		return true
	} else if aStop >= bStart && aStop <= bStop {
		return true
	} else {
		return false
	}
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	includedPairs := 0
	for _, line := range lines {
		pairElems := strings.Split(line, ",")
		firstStart, firstStop := getRangeStartStop(pairElems[0])
		secondStart, secondStop := getRangeStartStop(pairElems[1])
		if rangeFullyIncludedByPartner(firstStart, firstStop, secondStart, secondStop) {
			includedPairs += 1
		}
	}

	return includedPairs
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	includedPairs := 0
	for _, line := range lines {
		pairElems := strings.Split(line, ",")
		firstStart, firstStop := getRangeStartStop(pairElems[0])
		secondStart, secondStop := getRangeStartStop(pairElems[1])
		if rangeFullyIncludedByPartner(firstStart, firstStop, secondStart, secondStop) ||
			rangePartiallyOverlapsWithPartner(firstStart, firstStop, secondStart, secondStop) {
			includedPairs += 1
		}
	}

	return includedPairs
}

func main() {
	fmt.Println("--{2022} day {04} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
