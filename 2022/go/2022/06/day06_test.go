package main

import (
	_ "embed"
	"testing"
)

//go:embed input_test.txt
var input_test string

func TestPart1Only(t *testing.T) {
	result := Part1("mjqjpqmgbljsphdztnvjfqwrcgsmlb")
	expected := 7
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}

	result = Part1("bvwbjplbgvbhsrlpgdmjqwftvncz")
	expected = 5
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}

	result = Part1("nppdvjthqldpwncqszvftbrmjlhg")
	expected = 6
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}

	result = Part1("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg")
	expected = 10
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}

	result = Part1("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw")
	expected = 11
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart2(t *testing.T) {
	result := Part2("mjqjpqmgbljsphdztnvjfqwrcgsmlb")
	expected := 19
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart1Input(t *testing.T) {
	result := Part1(string(input_day))
	expected := 1287
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart2Input(t *testing.T) {
	result := Part2(string(input_day))
	expected := 3716
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
