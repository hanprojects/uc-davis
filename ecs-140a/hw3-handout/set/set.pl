% Sources/References:
% For Union: https://stackoverflow.com/questions/46899153/making-a-union-of-two-lists-in-prolog
% For Intersection: https://stackoverflow.com/questions/9615002/intersection-and-union-of-2-lists
% For Equal: https://stackoverflow.com/questions/30157590/prolog-determining-if-elements-in-list-are-equal-if-order-does-not-matter/30158248

% ------------------ UNION SECTION ------------------ %
checkunion([], [], []).
checkunion([], Set, Set).
checkunion(Set, [], Set).

checkunion(Set1, [Head2 | Tail2], Union) :-
    member(Head2, Set1), 
    checkunion(Set1, Tail2, Union).

checkunion(Set1, [Head2 | Tail2], [Head2 | Union]) :-
    not(member(Head2, Set1)), 
    checkunion(Set1, Tail2, Union). 

isUnion(Set1,Set2,Union) :-
    checkunion(Set1, Set2, Union).

% ------------------ INTER SECTION ------------------ %
checkinter(_, [], []).
checkinter([], _, []).

checkinter([Head1 | Tail1], Set2, [Head1 | Inter]) :-
    member(Head1, Set2),
    checkinter(Tail1, Set2, Inter).

checkinter([_ | Tail1], Set2, Inter) :-
    checkinter(Tail1, Set2, Inter).

isIntersection(Set1,Set2,Intersection) :-
    checkinter(Set1, Set2, Intersection).

% ------------------ EQUAL SECTION ------------------ %
checkequal([], []).

checkequal([Head1 | Tail1], Set2) :-
    select(Head1, Set2, NewSet), % able to use select(), lecture slide mentions this already
    checkequal(Tail1, NewSet).

isEqual(Set1,Set2) :-
    checkequal(Set1, Set2).