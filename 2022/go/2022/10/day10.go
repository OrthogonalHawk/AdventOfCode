package main

import (
	_ "embed"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
)

//go:embed input.txt
var input_day string

type processorModel struct {
	CycleCount     int
	CurInstruction string
	State          string
	XRegVal        int
	PendingXRegVal int
}

const (
	STATE_IDLE        = "IDLE"
	STATE_NO_OP       = "NOOP"
	STATE_ADD_STAGE_1 = "ADD_STAGE_1"
	STATE_ADD_STAGE_2 = "ADD_STAGE_2"
)

func NewProcessorModel() processorModel {
	return processorModel{
		CycleCount:     0,
		CurInstruction: "",
		State:          STATE_NO_OP,
		XRegVal:        1,
		PendingXRegVal: 0,
	}
}

func (p *processorModel) StartNewCycle() {
	switch p.State {
	case STATE_IDLE:
		fmt.Printf("Starting a new cycle, but already in idle...\n")
		os.Exit(-1)

	case STATE_NO_OP:
		p.State = STATE_IDLE
		p.CurInstruction = ""

	case STATE_ADD_STAGE_1:
		p.State = STATE_ADD_STAGE_2

	case STATE_ADD_STAGE_2:
		p.State = STATE_IDLE
		p.XRegVal = p.XRegVal + p.PendingXRegVal
		p.PendingXRegVal = 0
		p.CurInstruction = ""

	default:
		fmt.Printf("Unknown state\n")
		os.Exit(-1)
	}

	p.CycleCount++
}

func (p *processorModel) GetSignalStrength() int {
	return p.CycleCount * p.XRegVal
}

func (p *processorModel) IsBusy() bool {
	return p.State != STATE_IDLE
}

func (p *processorModel) ReadyForNextInstruction() bool {
	return p.State == STATE_IDLE
}

func (p *processorModel) StartNewInstruction(instruction string) {
	if p.ReadyForNextInstruction() {
		if instruction == "noop" {
			p.State = STATE_NO_OP
		} else {
			p.State = STATE_ADD_STAGE_1
			instrElems := strings.Split(instruction, " ")
			val, _ := strconv.Atoi(instrElems[1])
			p.PendingXRegVal = val
		}
		p.CurInstruction = instruction
	} else {
		fmt.Printf("ERROR : Not ready for new instruction; still in the %s state", p.State)
		os.Exit(-1)
	}
}

func (p processorModel) String() string {
	return fmt.Sprintf("Cycle: %03d State: %12s Inst: %12s XReg: %06d Signal: %06d\n", p.CycleCount, p.State, p.CurInstruction, p.XRegVal, p.GetSignalStrength())
}

func Part1(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	fmt.Printf("Processing %d instructions\n", len(lines))
	cyclesToEval := []int{20, 60, 100, 140, 180, 220}

	numInstructionsProcessed := 0
	processor := NewProcessorModel()
	sumSignalStrength := 0
	for numInstructionsProcessed < len(lines) || processor.IsBusy() {
		processor.StartNewCycle()

		if processor.ReadyForNextInstruction() && numInstructionsProcessed < len(lines) {
			processor.StartNewInstruction(lines[numInstructionsProcessed])
			numInstructionsProcessed++
		}

		for _, evalCycle := range cyclesToEval {
			if processor.CycleCount == evalCycle {
				sumSignalStrength += processor.GetSignalStrength()
				fmt.Printf("%s", processor)
			}
		}
		// fmt.Printf("%s", processor)
	}

	return sumSignalStrength
}

func Part2(input string) int {
	input = strings.TrimSuffix(input, "\n")
	lines := strings.Split(input, "\n")

	fmt.Printf("Processing %d instructions\n", len(lines))

	numInstructionsProcessed := 0
	processor := NewProcessorModel()

	for numInstructionsProcessed < len(lines) || processor.IsBusy() {
		processor.StartNewCycle()

		if processor.ReadyForNextInstruction() && numInstructionsProcessed < len(lines) {
			processor.StartNewInstruction(lines[numInstructionsProcessed])
			numInstructionsProcessed++
		}

		curPixelPos := (processor.CycleCount - 1) % 40
		if processor.XRegVal >= (curPixelPos-1) && processor.XRegVal <= (curPixelPos+1) {
			fmt.Printf("#")
		} else {
			fmt.Printf(".")
		}

		if processor.CycleCount%40 == 0 {
			fmt.Printf("\n")
		}
	}

	return 0
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
