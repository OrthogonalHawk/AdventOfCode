package main

import (
	_ "embed"
	"fmt"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")
	inputStr := lines[0]

	seqBuffer := make([]string, 0)
	for idx := range inputStr {
		seqBuffer = append(seqBuffer, string(inputStr[idx]))

		if len(seqBuffer) >= 4 {
			strMap := make(map[string]int, 0)
			for _, b := range seqBuffer {
				strMap[b]++
			}

			foundDup := false
			for _, f := range strMap {
				if f > 1 {
					foundDup = true
				}
			}

			if !foundDup {
				return idx + 1
			}

			seqBuffer = seqBuffer[1:]
		}
	}

	return -1
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")
	inputStr := lines[0]

	seqBuffer := make([]string, 0)
	for idx := range inputStr {
		seqBuffer = append(seqBuffer, string(inputStr[idx]))

		if len(seqBuffer) >= 14 {
			strMap := make(map[string]int, 0)
			for _, b := range seqBuffer {
				strMap[b]++
			}

			foundDup := false
			for _, f := range strMap {
				if f > 1 {
					foundDup = true
				}
			}

			if !foundDup {
				return idx + 1
			}

			seqBuffer = seqBuffer[1:]
		}
	}

	return -1
}

func main() {
	fmt.Println("--{year} day {day} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
