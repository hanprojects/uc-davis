package nfa

import (
	"sync"
)

// A nondeterministic Finite Automaton (NFA) consists of states,
// symbols in an alphabet, and a transition function.

// A state in the NFA is represented as an unsigned integer.
type state uint

// Given the current state and a symbol, the transition function
// of an NFA returns the set of next states the NFA can transition to
// on reading the given symbol.
// This set of next states could be empty.
type TransitionFunction func(st state, sym rune) []state

// Reachable returns true if there exists a sequence of transitions
// from `transitions` such that if the NFA starts at the start state
// `start` it would reach the final state `final` after reading the
// entire sequence of symbols `input`; Reachable returns false otherwise.
func Reachable(
	// `transitions` tells us what our NFA looks like
	transitions TransitionFunction,
	// `start` and `final` tell us where to start, and where we want to end up
	start, final state,
	// `input` is a (possible empty) list of symbols to apply.
	input []rune,
) bool {
	// TODO
	// panic("TODO: implement this!")

	// ------------------------- STUDENT's ANSWER --------------------------- //
	// Source: same thing with NFA of Go/Clisp we did before, just need to create
	// channel and wait for processes to return
	if len(input) == 0 {
		return start == final
	}

	var wg sync.WaitGroup

	// Ideally, to create channel to handle recursions (Source: Piazza @383)
	ch := make(chan bool)
	possible_states := transitions(start, input[0])
	remaining_input := input[1:]

	for _, next_poss_state := range possible_states {
		wg.Add(1)
		go isReachable(transitions, next_poss_state, final, remaining_input, ch)
		// wg.Wait() // TODO: too long of execution, maybe remove this

		if <-ch {
			wg.Done()
			return true
		}
	}

	close(ch)

	return false
}

// Helper function to check if it's reachable from created channel
// Source: Own solution in Clisp
func isReachable(
	transitions TransitionFunction,
	start, final state,
	input []rune,
	ch chan bool,
) {
	if len(input) == 0 {
		ch <- (start == final)
	} else {
		ch <- Reachable(transitions, start, final, input)
	}

	return
}
