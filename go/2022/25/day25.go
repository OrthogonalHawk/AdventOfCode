package main

import (
	_ "embed"
	"fmt"
	"math"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

var SnafuToDecimal = map[string]int{
	"1": 1,
	"2": 2,
	"0": 0,
	"-": -1,
	"=": -2,
}

var SnafuToRune = map[string]rune{
	"1": '1',
	"2": '2',
	"0": '0',
	"-": '-',
	"=": '=',
}

func MyPow(x, y int) int {
	flt := math.Pow(float64(x), float64(y))
	return int(flt)
}

func ConvertSnafuToDecimal(snafu string) int {
	ret := 0

	if len(snafu) == 0 {
		return -1000
	}

	snafuLen := len(snafu)
	for ii, val := range snafu {
		valStr := string(val)
		valInt := SnafuToDecimal[valStr]
		valPos := snafuLen - ii - 1
		valVal := valInt * MyPow(5, valPos)

		// fmt.Printf("Snafu: %s, val='%s' valInt=%d valPos=%d valVal=%d\n", snafu, valStr, valInt, valPos, valVal)

		ret += valVal
	}

	return ret
}

func CreateSnafuPermutations(reqLen int) []string {
	var ret []string = []string{"1", "2"}

	if reqLen == 1 {
		return []string{"1", "2", "0", "-", "="}
	}

	complete := false
	for !complete {
		extStrs := make([]string, 0)

		for _, curStr := range ret {
			if len(curStr) < reqLen {
				for m, _ := range SnafuToDecimal {
					extStrs = append(extStrs, curStr+m)
				}
			} else {
				complete = true
			}
		}

		if !complete {
			ret = extStrs
		}
	}

	return ret
}

func replaceAtIndex(in string, r rune, i int) string {
	out := []rune(in)
	out[i] = r
	return string(out)
}

func ConvertDecimalToSnafu(dec int) string {
	ret := ""
	fmt.Printf("Convert %d to SNAFU\n", dec)

	maxRet := FindSnafuLargerThanDecimal(dec)
	ret = maxRet
	for charIdx := 0; charIdx < len(maxRet); charIdx++ {
		fmt.Printf("Evaluating '%s'\n", ret)
		curVal := ConvertSnafuToDecimal(ret)
		if curVal == dec {

		} else if curVal > dec {
			valsToTry := []rune{'1', '0', '-', '='}
			for runeIdx := range valsToTry {
				nextStr := replaceAtIndex(ret, valsToTry[runeIdx], charIdx)
				nextVal := ConvertSnafuToDecimal(nextStr)
				if nextVal < dec {
					break
				} else {
					ret = nextStr
				}
			}
		} else if curVal < dec {
			fmt.Printf("ERROR : Went too small!!!\n")
			return "ERROR"
		}
	}

	return ret
}

func FindSnafuLargerThanDecimal(dec int) string {
	// find the minimum 1xxxx value that is greater than dec
	ret := "2"

	for ConvertSnafuToDecimal(ret) < dec {
		ret += "2"
	}

	return ret
}

func Part1(input string) string {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	fuelSum := 0
	for _, line := range lines {
		fuelSum += ConvertSnafuToDecimal(line)
	}

	fmt.Printf("Found sum: %d\n", fuelSum)

	return ""
}

func Part2(input string) int {
	// input = strings.TrimSuffix(input, "\n")
	// lines := strings.Split(input, "\n")
	return 0
}

func main() {
	fmt.Println("--{2022} day {25} solution--")
	start := time.Now()
	fmt.Println("part1: ", Part1(input_day))
	fmt.Println(time.Since(start))

	start = time.Now()
	fmt.Println("part2: ", Part2(input_day))
	fmt.Println(time.Since(start))
}
