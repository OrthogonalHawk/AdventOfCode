package main

import (
	_ "embed"
	"testing"
)

//go:embed input_test.txt
var input_test string

//go:embed input_test_extended.txt
var input_test_extended string

func TestPart1Only(t *testing.T) {
	result := Part1(string(input_test))
	expected := 0
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart1Extended(t *testing.T) {
	result := Part1(string(input_test_extended))
	expected := 13140
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart2Only(t *testing.T) {
	result := Part2(string(input_test))
	expected := 0
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart2Extended(t *testing.T) {
	result := Part2(string(input_test_extended))
	expected := 0
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
	}
}

func TestPart1Input(t *testing.T) {
	result := Part1(string(input_day))
	expected := 14360
	if result != expected {
		t.Errorf("Result is incorrect, got: %d, want: %d.", result, expected)
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
