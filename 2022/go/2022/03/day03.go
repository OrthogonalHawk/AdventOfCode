package main

import (
	_ "embed"
	"fmt"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

func findCommonItem(sideA string, sideB string) string {

	for idx := range sideA {
		if strings.Contains(sideB, string(sideA[idx])) {
			return string(sideA[idx])
		}
	}

	return ""
}

func findCommonItems(sideA string, sideB string) string {
	ret := ""

	for idx := range sideA {
		nextStr := string(sideA[idx])
		if strings.Contains(sideB, nextStr) && !strings.Contains(ret, nextStr) {
			ret = ret + string(sideA[idx])
		}
	}

	return ret
}

func getItemPriority(item string) int {
	priority := 0

	if len(item) != 1 {
		fmt.Printf("ERROR: Cannot calculate item priority\n")
		return 0
	}

	itemInt := int(item[0])
	if itemInt >= int(byte('a')) && itemInt <= int(byte('z')) {
		return itemInt - int(byte('a')) + 1
	}

	if itemInt >= int(byte('A')) && itemInt <= int(byte('Z')) {
		return itemInt - int(byte('A')) + 27
	}

	return priority
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	commonItems := make([]string, 0)
	for _, line := range lines {
		if len(line)%2 != 0 {
			fmt.Printf("ERROR: Found a line with an odd number of items\n")
			return -1
		}
		sideAStr := line[0:(len(line) / 2)]
		sideBStr := line[len(line)/2:]

		commonItem := findCommonItem(sideAStr, sideBStr)
		if commonItem != "" {
			commonItems = append(commonItems, commonItem)
		}
	}

	prioritySum := 0
	for _, item := range commonItems {
		prioritySum += getItemPriority(item)
	}

	return prioritySum
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	prioritySum := 0
	groupItems := make([]string, 0)
	for _, line := range lines {

		groupItems = append(groupItems, line)

		if len(groupItems) == 3 {
			if len(groupItems) == 3 {
				sharedBtwnFirstAndSecond := findCommonItems(groupItems[0], groupItems[1])
				sharedWithThird := findCommonItems(sharedBtwnFirstAndSecond, groupItems[2])

				if len(sharedWithThird) != 1 {
					fmt.Printf("ERROR: Failed to find expected match in group; shared='%v'\n", sharedWithThird)
					return -1
				}

				prioritySum += getItemPriority(sharedWithThird)
			}

			groupItems = make([]string, 0)
		}
	}

	return prioritySum

}

func main() {
	fmt.Println("--{2022} day {03} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
