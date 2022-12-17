package main

import (
	_ "embed"
	"fmt"
	"math"
	"os"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

type MapPos struct {
	X int
	Y int
}

func (pos MapPos) String() string {
	return fmt.Sprintf("%d,%d", pos.X, pos.Y)
}

type AltitudeMap struct {
	RawMap   map[string]int
	StartPos MapPos
	EndPos   MapPos
	MaxX     int
	MaxY     int
}

func (altMap AltitudeMap) PrintMap() {
	for yIdx := altMap.MaxY - 1; yIdx >= 0; yIdx-- {
		for xIdx := 0; xIdx < altMap.MaxX; xIdx++ {
			curPos := MapPos{X: xIdx, Y: yIdx}
			fmt.Printf(" %02d", altMap.RawMap[fmt.Sprintf("%s", curPos)])
		}
		fmt.Printf("\n")
	}
}

func createAltitudeMap(lines []string) AltitudeMap {

	var altMap AltitudeMap = AltitudeMap{
		RawMap:   make(map[string]int),
		StartPos: MapPos{X: 0, Y: 0},
		EndPos:   MapPos{X: 0, Y: 0},
		MaxX:     len(lines[0]),
		MaxY:     len(lines),
	}

	for lineIdx, line := range lines {
		for charIdx := 0; charIdx < len(line); charIdx++ {
			curChar := string(line[charIdx])
			curElev := line[charIdx] - ([]byte("a"))[0]

			if curChar == "S" {
				altMap.StartPos = MapPos{X: charIdx, Y: len(lines) - lineIdx - 1}
				curElev = 0
			} else if curChar == "E" {
				altMap.EndPos = MapPos{X: charIdx, Y: len(lines) - lineIdx - 1}
				curElev = 26
			}

			curPos := MapPos{X: charIdx, Y: len(lines) - lineIdx - 1}
			altMap.RawMap[fmt.Sprintf("%s", curPos)] = int(curElev)
			fmt.Printf("%s : %d\n", curPos, int(curElev))
		}
	}

	return altMap
}

func GetCandidateStartPositions(lines []string) []MapPos {
	res := make([]MapPos, 0)

	for lineIdx, line := range lines {
		for charIdx := 0; charIdx < len(line); charIdx++ {
			curChar := string(line[charIdx])
			if curChar == "a" || curChar == "S" {
				res = append(res, MapPos{X: charIdx, Y: len(lines) - lineIdx - 1})
			}
		}
	}

	return res
}

func (altMap *AltitudeMap) GetValidDestinations(curPos MapPos) []MapPos {
	var res []MapPos = make([]MapPos, 0)

	if curPos.X >= 0 && curPos.X < altMap.MaxX && curPos.Y >= 0 && curPos.Y < altMap.MaxY {
		curElev, ok := altMap.RawMap[fmt.Sprintf("%s", curPos)]
		if !ok {
			fmt.Printf("ERROR : Unable to retrieve position for %s", curPos)
			os.Exit(-1)
		}

		// check to the left of the current position
		if curPos.X >= 1 {
			leftPos := MapPos{X: curPos.X - 1, Y: curPos.Y}
			leftElev, ok := altMap.RawMap[fmt.Sprintf("%s", leftPos)]
			if !ok {
				fmt.Printf("ERROR : Unable to retrieve 'left' position from %s", curPos)
				os.Exit(-1)
			}
			if leftElev <= (curElev + 1) {
				res = append(res, leftPos)
			}
		}

		// check to the right of the current position
		if curPos.X < (altMap.MaxX - 1) {
			rightPos := MapPos{X: curPos.X + 1, Y: curPos.Y}
			rightElev, ok := altMap.RawMap[fmt.Sprintf("%s", rightPos)]
			if !ok {
				fmt.Printf("ERROR : Unable to retrieve 'right' position from %s", curPos)
				os.Exit(-1)
			}
			if rightElev <= (curElev + 1) {
				res = append(res, rightPos)
			}
		}

		// check above the current position
		if curPos.Y < (altMap.MaxY - 1) {
			upPos := MapPos{X: curPos.X, Y: curPos.Y + 1}
			upElev, ok := altMap.RawMap[fmt.Sprintf("%s", upPos)]
			if !ok {
				fmt.Printf("ERROR : Unable to retrieve 'up' position from %s", curPos)
				os.Exit(-1)
			}
			if upElev <= (curElev + 1) {
				res = append(res, upPos)
			}
		}

		// check below the current position
		if curPos.Y >= 1 {
			downPos := MapPos{X: curPos.X, Y: curPos.Y - 1}
			downElev, ok := altMap.RawMap[fmt.Sprintf("%s", downPos)]
			if !ok {
				fmt.Printf("ERROR : Unable to retrieve 'down' position from %s", curPos)
				os.Exit(-1)
			}
			if downElev <= (curElev + 1) {
				res = append(res, downPos)
			}
		}
	} else {

	}

	return res
}

func FindStepsToDestination(altMap *AltitudeMap, visitMap *map[string]int, curPos MapPos, curNumSteps int) (bool, int) {
	possibleDestinations := altMap.GetValidDestinations(curPos)
	nextDestinations := make([]MapPos, 0)

	if curPos == altMap.EndPos {
		// fmt.Printf("Found destination %s after %d steps\n", curPos, curNumSteps)
		return true, curNumSteps
	}

	for _, dest := range possibleDestinations {
		if dest == altMap.EndPos {
			nextDestinations = make([]MapPos, 0)
			nextDestinations = append(nextDestinations, dest)
			break
		}

		_, visited := (*visitMap)[fmt.Sprintf("%s", dest)]
		if !visited {
			(*visitMap)[fmt.Sprintf("%s", dest)] = curNumSteps
			nextDestinations = append(nextDestinations, dest)
		} else {
			prevNumSteps, _ := (*visitMap)[fmt.Sprintf("%s", dest)]
			if prevNumSteps > curNumSteps {
				(*visitMap)[fmt.Sprintf("%s", dest)] = curNumSteps
				nextDestinations = append(nextDestinations, dest)
			}
		}
	}

	// curElev, _ := altMap.RawMap[fmt.Sprintf("%s", curPos)]
	// fmt.Printf("Visited %s ht:%d stp:%d and found %v next destinations\n", curPos, curElev, curNumSteps, nextDestinations)

	if len(nextDestinations) == 0 {
		return false, curNumSteps
	}

	foundDest := false
	numSteps := math.MaxInt
	for _, dest := range nextDestinations {
		iterFoundDest, iterNumSteps := FindStepsToDestination(altMap, visitMap, dest, curNumSteps+1)
		if iterFoundDest && iterNumSteps < numSteps {
			foundDest = true
			numSteps = iterNumSteps
		}
	}

	return foundDest, numSteps
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	altMap := createAltitudeMap(lines)

	visitMap := make(map[string]int)
	visitMap[fmt.Sprintf("%s", altMap.StartPos)] = 0
	found, numSteps := FindStepsToDestination(&altMap, &visitMap, altMap.StartPos, 0)
	if !found {
		fmt.Printf("ERROR : Unable to find destination\n")
	}

	return numSteps
}

func Part2(input string) int {

	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	altMap := createAltitudeMap(lines)
	startPositions := GetCandidateStartPositions(lines)

	fmt.Printf("Found %d starting positions\n", len(startPositions))

	minSteps := math.MaxInt
	for idx, startPos := range startPositions {

		fmt.Printf("Evaluating start position %d of %d\n", idx, len(startPositions)-1)

		visitMap := make(map[string]int)
		altMap.StartPos = startPos
		visitMap[fmt.Sprintf("%s", altMap.StartPos)] = 0
		found, numSteps := FindStepsToDestination(&altMap, &visitMap, altMap.StartPos, 0)
		if !found {
			fmt.Printf("ERROR : Unable to find destination\n")
		}
		if numSteps < minSteps {
			minSteps = numSteps
		}
	}

	return minSteps
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
