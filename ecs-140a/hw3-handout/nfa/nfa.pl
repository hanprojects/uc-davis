% Source: Logic from NFA problem from Go/Clisp Project in the class

% list of states is empty
checkreachable(State, State, []).

% list of states is 1
% TODO: All 16 test cases passed but %Fail = 25.0 for nfa.pl (weird...)
% checkreachable(Start, Final, [State]) :-
%    transition(Start, Final, State).

% list of states is more than 1 -> use logic of Go/Clisp project
checkreachable(Start, Final, [Head | Tail]) :-
    transition(Start, Head, RemainingStates),
    member(NextState, RemainingStates),
    checkreachable(NextState, Final, Tail).

reachable(StartState, FinalState, Input) :-
    checkreachable(StartState, FinalState, Input).