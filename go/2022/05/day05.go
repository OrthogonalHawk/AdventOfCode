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

func Part1(input string) string {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	stackLayoutLines := make([]string, 0)
	moveInstructionLines := make([]string, 0)

	foundBlankLine := false
	for _, line := range lines {
		if line == "" {
			foundBlankLine = true
		}

		if !foundBlankLine {
			stackLayoutLines = append(stackLayoutLines, line)
		} else {
			moveInstructionLines = append(moveInstructionLines, line)
		}
	}

	// get the number of stacks
	stackNumStrs := strings.Split(stackLayoutLines[len(stackLayoutLines)-1], " ")
	numStacks, _ := strconv.Atoi(stackNumStrs[len(stackNumStrs)-1])
	stacks := make([][]string, numStacks)

	// populate the stacks with the initial contents
	for lineIdx := len(stackLayoutLines) - 2; lineIdx >= 0; lineIdx-- {
		line := stackLayoutLines[lineIdx]
		stackIdx := 0
		for idx := range line {
			if idx%4 == 1 {
				nextStackStr := string(line[idx])
				if nextStackStr != " " {
					stacks[stackIdx] = append(stacks[stackIdx], nextStackStr)
				}
				stackIdx += 1
			}
		}
	}

	// parse the movement instructions and at each step perform the action
	for _, line := range moveInstructionLines {

		if line == "" {
			continue
		}

		elems := strings.Split(line, " ")

		if len(elems) != 6 {
			fmt.Printf("ERROR: only found %d elements in move instruction (%s)", len(elems), line)
			return "FAILURE"
		}

		repeatCount, _ := strconv.Atoi(elems[1])
		srcStackIdx, _ := strconv.Atoi(elems[3])
		srcStackIdx -= 1
		dstStackIdx, _ := strconv.Atoi(elems[5])
		dstStackIdx -= 1

		for rpt := 0; rpt < repeatCount; rpt++ {
			stacks[dstStackIdx] = append(stacks[dstStackIdx], stacks[srcStackIdx][len(stacks[srcStackIdx])-1])
			stacks[srcStackIdx] = stacks[srcStackIdx][:len(stacks[srcStackIdx])-1]
		}
	}

	resStr := ""
	for stackIdx := range stacks {
		resStr += stacks[stackIdx][len(stacks[stackIdx])-1]
	}

	return resStr
}

func Part2(input string) string {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	stackLayoutLines := make([]string, 0)
	moveInstructionLines := make([]string, 0)

	foundBlankLine := false
	for _, line := range lines {
		if line == "" {
			foundBlankLine = true
		}

		if !foundBlankLine {
			stackLayoutLines = append(stackLayoutLines, line)
		} else {
			moveInstructionLines = append(moveInstructionLines, line)
		}
	}

	// get the number of stacks
	stackNumStrs := strings.Split(stackLayoutLines[len(stackLayoutLines)-1], " ")
	numStacks, _ := strconv.Atoi(stackNumStrs[len(stackNumStrs)-1])
	stacks := make([][]string, numStacks)

	// populate the stacks with the initial contents
	for lineIdx := len(stackLayoutLines) - 2; lineIdx >= 0; lineIdx-- {
		line := stackLayoutLines[lineIdx]
		stackIdx := 0
		for idx := range line {
			if idx%4 == 1 {
				nextStackStr := string(line[idx])
				if nextStackStr != " " {
					stacks[stackIdx] = append(stacks[stackIdx], nextStackStr)
				}
				stackIdx += 1
			}
		}
	}

	// parse the movement instructions and at each step perform the action
	for _, line := range moveInstructionLines {

		if line == "" {
			continue
		}

		elems := strings.Split(line, " ")

		if len(elems) != 6 {
			fmt.Printf("ERROR: only found %d elements in move instruction (%s)", len(elems), line)
			return "FAILURE"
		}

		repeatCount, _ := strconv.Atoi(elems[1])
		srcStackIdx, _ := strconv.Atoi(elems[3])
		srcStackIdx -= 1
		dstStackIdx, _ := strconv.Atoi(elems[5])
		dstStackIdx -= 1

		stacks[dstStackIdx] = append(stacks[dstStackIdx], stacks[srcStackIdx][len(stacks[srcStackIdx])-repeatCount:]...)
		for rpt := 0; rpt < repeatCount; rpt++ {
			stacks[srcStackIdx] = stacks[srcStackIdx][:len(stacks[srcStackIdx])-1]
		}
	}

	resStr := ""
	for stackIdx := range stacks {
		resStr += stacks[stackIdx][len(stacks[stackIdx])-1]
	}

	return resStr
}

func main() {
	fmt.Println("--{2022} day {05} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
