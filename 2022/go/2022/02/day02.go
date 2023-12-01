package main

import (
	_ "embed"
	"fmt"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

const (
	OPP_ROCK     = "A"
	OPP_PAPER    = "B"
	OPP_SCISSORS = "C"

	MY_ROCK     = "X"
	MY_PAPER    = "Y"
	MY_SCISSORS = "Z"

	INSTRUCTIONS_WIN  = "Z"
	INSTRUCTIONS_LOSE = "X"
	INSTRUCTIONS_DRAW = "Y"

	ROUND_WINNER_SCORE = 6
	ROUND_DRAW_SCORE   = 3
)

func getRoundScore(oppMove string, myMove string) int {
	score := 0

	switch oppMove {
	case OPP_ROCK:
		if myMove == MY_PAPER {
			score += ROUND_WINNER_SCORE
		} else if myMove == MY_ROCK {
			score += ROUND_DRAW_SCORE
		}

	case OPP_PAPER:
		if myMove == MY_SCISSORS {
			score += ROUND_WINNER_SCORE
		} else if myMove == MY_PAPER {
			score += ROUND_DRAW_SCORE
		}

	case OPP_SCISSORS:
		if myMove == MY_ROCK {
			score += ROUND_WINNER_SCORE
		} else if myMove == MY_SCISSORS {
			score += ROUND_DRAW_SCORE
		}

	default:
		fmt.Printf("Unrecognized opponent move '%s'\n", oppMove)
	}

	switch myMove {
	case MY_ROCK:
		score += 1
	case MY_PAPER:
		score += 2
	case MY_SCISSORS:
		score += 3
	default:
		fmt.Printf("Unrecognized own move '%s'\n", myMove)
	}

	return score
}

func chooseNextMove(oppMove string, instructions string) string {
	myMove := MY_ROCK

	switch instructions {
	case INSTRUCTIONS_WIN:
		switch oppMove {
		case OPP_ROCK:
			myMove = MY_PAPER
		case OPP_PAPER:
			myMove = MY_SCISSORS
		case OPP_SCISSORS:
			myMove = MY_ROCK
		}
	case INSTRUCTIONS_LOSE:
		switch oppMove {
		case OPP_ROCK:
			myMove = MY_SCISSORS
		case OPP_PAPER:
			myMove = MY_ROCK
		case OPP_SCISSORS:
			myMove = MY_PAPER
		}
	case INSTRUCTIONS_DRAW:
		switch oppMove {
		case OPP_ROCK:
			myMove = MY_ROCK
		case OPP_PAPER:
			myMove = MY_PAPER
		case OPP_SCISSORS:
			myMove = MY_SCISSORS
		}
	default:
		fmt.Printf("Unknown instruction '%s'\n", instructions)
	}

	return myMove
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	cumulativeScore := 0
	for _, line := range lines {
		moves := strings.Fields(line)
		if len(moves) != 2 {
			fmt.Printf("Failed to parse correctly; found %d fields when 2 were expected\n", len(moves))
			return -1
		}

		oppMove := moves[0]
		myMove := moves[1]
		cumulativeScore += getRoundScore(oppMove, myMove)
	}

	return cumulativeScore
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	cumulativeScore := 0
	for _, line := range lines {
		moves := strings.Fields(line)
		if len(moves) != 2 {
			fmt.Printf("Failed to parse correctly; found %d fields when 2 were expected\n", len(moves))
			return -1
		}

		oppMove := moves[0]
		myInstructions := moves[1]
		myMove := chooseNextMove(oppMove, myInstructions)
		cumulativeScore += getRoundScore(oppMove, myMove)
	}

	return cumulativeScore
}

func main() {
	fmt.Println("--{2022} day {02} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
