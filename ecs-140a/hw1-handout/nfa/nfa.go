package nfa

// A state in the NFA is labeled by a single integer.
type state uint

// TransitionFunction tells us, given a current state and some symbol, which
// other states the NFA can move to.
//
// Deterministic automata have only one possible destination state,
// but we're working with non-deterministic automata.
type TransitionFunction func(st state, act rune) []state

func Reachable(
	// `transitions` tells us what our NFA looks like
	transitions TransitionFunction,
	// `start` and `final` tell us where to start, and where we want to end up
	start, final state,
	// `input` is a (possible empty) list of symbols to apply.
	input []rune,
) bool {
	// TODO Write the Reachable function,
	// return true if the nfa accepts the input and can reach the final state with that input,
	// return false otherwise
	// panic("TODO: implement this!")

	// -------------------------- STUDENT'S ANSWER -------------------------- //

	// Return true/false if input is empty and the start state is not the final
	// state (the accept state)
	if len(input) == 0 { // TODO: cannot use nil comparison, double check why
		return start == final
	}

	// Get all possible transitions
	possible_trans := transitions(start, input[0])

	if len(possible_trans) == 0 {
		return false
	}

	// Take the remaining sequence of the input
	// Source: https://tour.golang.org/moretypes/7
	remain_in := input[1:]

	// Use recursion to do the same thing with next transition
	for i := 0; i < len(possible_trans); i++ {
		if Reachable(transitions, possible_trans[i], final, remain_in) {
			return true
		}
	}

	return false
}
