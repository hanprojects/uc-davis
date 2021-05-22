% Syntax: `;` for OR || `,` for AND
% ----------------------------------

% Main functions

/* All novels published during the year 1953 or 1996*/
year_1953_1996_novels(Book) :-
    novel(Book, 1953); novel(Book, 1996).

/* List of all novels published during the period 1800 to 1900*/
period_1800_1900_novels(Book) :-
    novel(Book, YEAR), YEAR >= 1800, YEAR =< 1900.

/* Characters who are fans of LOTR */
lotr_fans(Fan) :-
    fan(Fan, BOOKLIST), member(the_lord_of_the_rings, BOOKLIST).

/* Authors of the novels owned by Chandler */
author_names(Author) :-
    author(Author, BOOKLIST), fan(chandler, FANLIST), member(X, FANLIST), member(X, BOOKLIST).

/* Characters who are fans of Brandon Sanderson's novels */
fans_names(Fan) :-
    fan(Fan, FANLIST), author(brandon_sanderson, BOOKLIST), member(X, FANLIST), member(X, BOOKLIST).

/* Novels common between either of Phoebe, Ross, and Monica */
mutual_novels(Book) :-
    fan(phoebe, P), fan(ross, R), fan(monica, M), 
    findmutualnovels(Book, P, R, M).

findmutualnovels(Book, P, R, M):-
    member(Book, P), member(Book, R); % Phoebe and Ross is mutual member of books
    member(Book, P), member(Book, M); % OR Phoebe and Monica is mutual member of books
    member(Book, R), member(Book, M). % OR Ross and Monica is mutual member of books