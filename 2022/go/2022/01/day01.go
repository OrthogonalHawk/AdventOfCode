package main

import (
	_ "embed"
	"fmt"
	"sort"
	"strconv"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	curCount := 0
	maxCount := 0
	for _, line := range lines {
		if line != "" {
			nextVal, err := strconv.Atoi(line)
			if err != nil {
				fmt.Printf("Failed to convert %s to integer\n", line)
				return -1
			}

			curCount += nextVal
		} else {

			// end of a segment
			if curCount > maxCount {
				maxCount = curCount
			}
			curCount = 0
		}
	}

	if curCount > maxCount {
		maxCount = curCount
	}

	return maxCount
}

func newMaxArr(curArr []int, newVal int, maxArrVals int) []int {
	res := curArr
	res = append(res, newVal)

	if len(res) > maxArrVals {
		sort.Ints(res)
		return res[len(res)-maxArrVals:]
	} else {
		return res
	}
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	curCount := 0
	maxArr := make([]int, 0)
	for _, line := range lines {
		if line != "" {
			nextVal, err := strconv.Atoi(line)
			if err != nil {
				fmt.Printf("Failed to convert %s to integer\n", line)
				return -1
			}

			curCount += nextVal
		} else {

			maxArr = newMaxArr(maxArr, curCount, 3)
			curCount = 0
		}
	}

	maxArr = newMaxArr(maxArr, curCount, 3)

	maxArrSum := 0
	for _, elem := range maxArr {
		maxArrSum += elem
	}

	return maxArrSum

}

func main() {
	fmt.Println("--{2022} day {01} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
