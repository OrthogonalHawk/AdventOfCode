package main

import (
	"bufio"
	_ "embed"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

type knotPosition struct {
	X int
	Y int
}

func absInt(x int) int {
	return absDiffInt(x, 0)
}

func absDiffInt(x, y int) int {
	if x < y {
		return y - x
	}
	return x - y
}

func getNextTailPos(curStep int, hKnotPos knotPosition, direction string, tKnotPos knotPosition) knotPosition {

	// if the 'head' didn't move then the tail won't either
	if direction == "N" {
		return tKnotPos
	}

	// evaluate tail positions
	if absInt(hKnotPos.X-tKnotPos.X) <= 1 && absInt(hKnotPos.Y-tKnotPos.Y) <= 1 {
		return tKnotPos
	} else {
		moved := false

		// same row, but too far away
		if hKnotPos.Y == tKnotPos.Y && absInt(hKnotPos.X-tKnotPos.X) > 1 {
			if hKnotPos.X > tKnotPos.X {
				tKnotPos.X = hKnotPos.X - 1
			} else {
				tKnotPos.X = hKnotPos.X + 1
			}
			moved = true
		}

		// same column, but too far away
		if !moved && (hKnotPos.X == tKnotPos.X && absInt(hKnotPos.Y-tKnotPos.Y) > 1) {
			if hKnotPos.Y > tKnotPos.Y {
				tKnotPos.Y = hKnotPos.Y - 1
			} else {
				tKnotPos.Y = hKnotPos.Y + 1
			}
			moved = true
		}

		if !moved && hKnotPos.Y > tKnotPos.Y {
			if direction == "L" { // case 1
				tKnotPos.X = hKnotPos.X + 1
				tKnotPos.Y = hKnotPos.Y
				moved = true
			} else if direction == "R" { // case 4
				tKnotPos.X = hKnotPos.X - 1
				tKnotPos.Y = hKnotPos.Y
				moved = true
			} else if direction == "U" { // cases 2 and 3
				tKnotPos.X = hKnotPos.X
				tKnotPos.Y = hKnotPos.Y - 1
				moved = true
			} else if direction == "UL" { // case 9
				tKnotPos.X--
				tKnotPos.Y++
				moved = true
			} else if direction == "UR" { // case 10
				tKnotPos.X++
				tKnotPos.Y++
				moved = true
			}
		}

		if !moved && hKnotPos.Y < tKnotPos.Y {
			if direction == "L" { // case 8
				tKnotPos.X = hKnotPos.X + 1
				tKnotPos.Y = hKnotPos.Y
				moved = true
			} else if direction == "R" { // case 5
				tKnotPos.X = hKnotPos.X - 1
				tKnotPos.Y = hKnotPos.Y
				moved = true
			} else if direction == "D" { // cases 2 and 3
				tKnotPos.X = hKnotPos.X
				tKnotPos.Y = hKnotPos.Y + 1
				moved = true
			} else if direction == "DL" { // case 12
				tKnotPos.X--
				tKnotPos.Y--
				moved = true
			} else if direction == "DR" { // case 11
				tKnotPos.X++
				tKnotPos.Y--
				moved = true
			}
		}

		if !moved {
			fmt.Printf("[%d] Failed to move when distance is too great (dir='%s')", curStep, direction)
			os.Exit(-1)
		}
		return tKnotPos
	}
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	hKnotPos := knotPosition{X: 0, Y: 0}
	tKnotPos := knotPosition{X: 0, Y: 0}

	curStep := 0
	movMap := map[int]string{}
	hKnotMap := map[int]knotPosition{}
	tKnotMap := map[int]knotPosition{}

	movMap[curStep] = "S"
	hKnotMap[curStep] = hKnotPos
	tKnotMap[curStep] = tKnotPos
	curStep++

	for _, line := range lines {
		elems := strings.Split(line, " ")
		direction := elems[0]
		numSteps, _ := strconv.Atoi(elems[1])

		for step := 0; step < numSteps; step++ {
			switch direction {
			case "R":
				hKnotPos.X++
			case "L":
				hKnotPos.X--
			case "U":
				hKnotPos.Y++
			case "D":
				hKnotPos.Y--
			default:
				fmt.Printf("ERROR : Unknown direction\n")
				return -1
			}
			movMap[curStep] = direction
			hKnotMap[curStep] = hKnotPos

			newTKnotPos := getNextTailPos(curStep, hKnotPos, direction, tKnotPos)
			tKnotMap[curStep] = newTKnotPos
			tKnotPos = newTKnotPos
			curStep++
		}
	}

	tVisitMap := map[string]int{}
	for _, tPos := range tKnotMap {
		posStr := fmt.Sprintf("%d,%d", tPos.X, tPos.Y)
		curEntry, ok := tVisitMap[posStr]
		if ok {
			tVisitMap[posStr] = curEntry + 1
		} else {
			tVisitMap[posStr] = 1
		}
	}

	return len(tVisitMap)
}

func getMovementDirection(newPos knotPosition, oldPos knotPosition) string {
	if newPos.X == oldPos.X && newPos.Y == oldPos.Y {
		return "N"
	}

	if newPos.X < oldPos.X && newPos.Y == oldPos.Y {
		return "L"
	}

	if newPos.X > oldPos.X && newPos.Y == oldPos.Y {
		return "R"
	}

	if newPos.X > oldPos.X && newPos.Y > oldPos.Y {
		return "UR"
	}

	if newPos.X < oldPos.X && newPos.Y > oldPos.Y {
		return "UL"
	}

	if newPos.X < oldPos.X && newPos.Y < oldPos.Y {
		return "DL"
	}

	if newPos.X > oldPos.X && newPos.Y < oldPos.Y {
		return "DR"
	}

	if newPos.X == oldPos.X && newPos.Y > oldPos.Y {
		return "U"
	}

	if newPos.X == oldPos.X && newPos.Y < oldPos.Y {
		return "D"
	}

	fmt.Printf("Unable to calculate new direction from\n old=%v\nnew=%v\n", oldPos, newPos)
	os.Exit(-1)
	return ""
}

func waitForUserInput() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
}

func printBoard(ll knotPosition, ur knotPosition, curStep int, hKnotMap map[int]knotPosition, tKnotMapList []map[int]knotPosition) {
	hKnotPos, _ := hKnotMap[curStep]
	tKnotPos := make([]knotPosition, len(tKnotMapList))
	for knot := 0; knot < len(tKnotMapList); knot++ {
		tKnotPos[knot], _ = tKnotMapList[knot][curStep]
	}

	for row := ur.Y; row >= ll.Y; row-- {
		for col := ll.X; col <= ur.X; col++ {
			if hKnotPos.X == col && hKnotPos.Y == row {
				fmt.Printf("H")
				continue
			}

			foundKnot := false
			for knot := 0; knot < len(tKnotMapList) && !foundKnot; knot++ {
				curKnotPos := tKnotPos[knot]
				if curKnotPos.X == col && curKnotPos.Y == row {
					fmt.Printf("%d", knot+1)
					foundKnot = true
				}
			}
			if foundKnot {
				continue
			}

			if col == 0 && row == 0 {
				fmt.Printf(".")
			} else {
				fmt.Printf(".")
			}
		}
		fmt.Printf("\n")
	}
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	numKnots := 9
	hKnotPos := knotPosition{X: 0, Y: 0}
	tKnotPos := make([]knotPosition, numKnots)

	curStep := 0
	movMap := map[int]string{}
	hKnotMap := map[int]knotPosition{}
	tKnotMapList := make([]map[int]knotPosition, numKnots)
	for ii := 0; ii < numKnots; ii++ {
		tKnotMapList[ii] = map[int]knotPosition{}
	}

	movMap[curStep] = "S"
	hKnotMap[curStep] = hKnotPos
	for ii := 0; ii < numKnots; ii++ {
		tKnotMapList[ii][curStep] = tKnotPos[ii]
	}
	curStep++

	for _, line := range lines {
		elems := strings.Split(line, " ")
		direction := elems[0]
		numSteps, _ := strconv.Atoi(elems[1])

		for step := 0; step < numSteps; step++ {
			switch direction {
			case "R":
				hKnotPos.X++
			case "L":
				hKnotPos.X--
			case "U":
				hKnotPos.Y++
			case "D":
				hKnotPos.Y--
			default:
				fmt.Printf("ERROR : Unknown direction\n")
				return -1
			}
			movMap[curStep] = direction
			hKnotMap[curStep] = hKnotPos

			for ii := 0; ii < numKnots; ii++ {
				if ii == 0 {
					newTKnotPos := getNextTailPos(curStep, hKnotPos, direction, tKnotPos[ii])
					tKnotMapList[ii][curStep] = newTKnotPos
					tKnotPos[ii] = newTKnotPos
				} else {
					prevKnotDir := getMovementDirection(tKnotMapList[ii-1][curStep], tKnotMapList[ii-1][curStep-1])
					newTKnotPos := getNextTailPos(curStep, tKnotPos[ii-1], prevKnotDir, tKnotPos[ii])
					tKnotMapList[ii][curStep] = newTKnotPos
					tKnotPos[ii] = newTKnotPos
				}
			}

			curStep++
		}
	}

	hMinPos := knotPosition{X: 0, Y: 0}
	hMaxPos := knotPosition{X: 0, Y: 0}
	for _, tPos := range hKnotMap {
		if tPos.X > hMaxPos.X {
			hMaxPos.X = tPos.X
		}
		if tPos.Y > hMaxPos.Y {
			hMaxPos.Y = tPos.Y
		}
		if tPos.X < hMinPos.X {
			hMinPos.X = tPos.X
		}
		if tPos.Y < hMinPos.Y {
			hMinPos.Y = tPos.Y
		}
	}

	// for step := 0; step < curStep; step++ {
	// 	printBoard(hMinPos, hMaxPos, step, hKnotMap, tKnotMapList)
	// 	waitForUserInput()
	// }

	tVisitMap := map[string]int{}
	for _, tPos := range tKnotMapList[numKnots-1] {
		posStr := fmt.Sprintf("%d,%d", tPos.X, tPos.Y)
		curEntry, ok := tVisitMap[posStr]
		if ok {
			tVisitMap[posStr] = curEntry + 1
		} else {
			tVisitMap[posStr] = 1
		}
	}

	return len(tVisitMap)
}

func main() {
	fmt.Println("--{2022} day {09} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
