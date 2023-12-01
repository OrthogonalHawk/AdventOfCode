package main

import (
	_ "embed"
	"fmt"
	"testing"
)

//go:embed input_test.txt
var input_test string

func TestPart1Only(t *testing.T) {
	result := Part1(string(input_test))
	expected := "2=-1=0"
	if result != expected {
		t.Errorf("Result is incorrect, got: %s, want: %s.", result, expected)
	}
}

func TestSnafuToDecimal(t *testing.T) {
	expectedRes := map[string]int{
		"1":             1,
		"2":             2,
		"0":             0,
		"-":             -1,
		"=":             -2,
		"1=":            3,
		"1-":            4,
		"10":            5,
		"11":            6,
		"12":            7,
		"2=":            8,
		"2-":            9,
		"20":            10,
		"1=11-2":        2022,
		"1-0---0":       12345,
		"1121-1110-1=0": 314159265,
	}

	for m, v := range expectedRes {
		recvd := ConvertSnafuToDecimal(m)
		if recvd != v {
			t.Fatalf("Result is incorrect for '%s'. Got: %d, Want: %d", m, recvd, v)
		}
	}
}

func TestDecimalToSnafu(t *testing.T) {
	expectedRes := map[string]int{
		// "1":             1,
		// "2":             2,
		// "0":             0,
		// "-":             -1,
		// "=":             -2,
		// "1=":            3,
		// "1-":            4,
		// "10":            5,
		// "11":            6,
		// "12":            7,
		// "2=":            8,
		// "2-":            9,
		// "20":            10,
		"1=11-2":               2022,
		"1-0---0":              12345,
		"1121-1110-1=0":        314159265,
		"2-=12=2-2-2-=0012==2": 33010101016442,
	}

	for m, v := range expectedRes {
		recvd := ConvertDecimalToSnafu(v)
		if recvd != m {
			t.Fatalf("Result is incorrect for '%s'. Got: '%s', Want: %s", m, recvd, m)
		}
	}
}

func TestSnafuPermutations(t *testing.T) {
	snafuLen := 1
	expectedNumPermutations := MyPow(5, snafuLen)
	result := CreateSnafuPermutations(snafuLen)
	fmt.Printf("%v\n", result)
	if len(result) != expectedNumPermutations {
		t.Errorf("Result length is incorrect, got: %d, want: %d.", len(result), expectedNumPermutations)
	}
}

func TestPart2Only(t *testing.T) {
	result := Part2(string(input_test))
	expected := 0
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart1Input(t *testing.T) {
	result := Part1(string(input_day))
	expected := ""
	if result != expected {
		t.Errorf("Result is incorrect, got: %s, want: %s.", result, expected)
	}
}

func TestPart2Input(t *testing.T) {
	result := Part2(string(input_day))
	expected := 0
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func BenchmarkPart1(b *testing.B) {
	for i := 0; i < b.N; i++ {
		Part1(input_day)
	}
}
func BenchmarkPart2(b *testing.B) {
	for i := 0; i < b.N; i++ {
		Part2(input_day)
	}
}
