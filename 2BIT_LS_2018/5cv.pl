% CV6 - PROLOG

% ----------------------------------------------------------------
% RODINA

% pravidla
strodic(C,X) :- rodic(C,P), rodic(P,X).
rodic(C,P) :- otec(C,P).
rodic(C,P) :- matka(C,P).

% fakta
otec(adam,michal).
otec(maria,tomas).
matka(adam,maria).

% cíl
% ?- strodic(adam,X).

% ----------------------------------------------------------------
% ARITMETICKE PREDIKATY

% maximum dvoch cisel
max(X,Y,X) :- X > Y,!.
max(_,Y,Y).

% sucet po zadane cislo
sumto(1,1) :- !.
sumto(N, RES) :- N1 is N-1, sumto(N1, R1), RES is R1 +N. 

% naslednik cisla 
succ2(X,Y) :- integer(X), Y is X+1, !.
succ2(X,Y) :- integer(Y), X is Y-1.

% ----------------------------------------------------------------
% PREDIKATY NAD ZOZNAMAMI

% test na zoznam
isList([]).
isList([_|_]).

% test na clenstvo v zozname
isElm(X,[X|_]) :- !.
isElm(X,[_|T]) :- isElm(X,T).

% 2 prvky su vedla seba v zozname
nextto2(X,Y,[X,Y|_]) :- !.
nextto2(X,Y,[_|T] ) :- nextto2(X,Y,T)  .

% posledny prvok zoznamu
last2(X,[X]) :- !.
last2(X, [_|T]) :- last2(X,T).

% ----------------------------------------------------------------
% SAMOSTATNA ULOHA
% Zakazane akekolvek vstavane predikaty okrem tych, 
% ktore boli pouzite na tomto cviceni.

% CV1
% Vytvorte predikat na splostenie vnoreneho zoznamu (nebude obsahovat 
% podzoznami), mozete si pomoct predikatom na spojenie 2 zoznamov.

% ?- app([a,b],[c],L).
% L = [a, b, c].
% ?- flatt([1,2,[3,4],[],[5,[6,7]]],P).
% P = [1, 2, 3, 4, 5, 6, 7].

%addElementToList(A, L, M) :- addElementTolist(A, L, [A|L]).

isLinear([]).
isLinear([H|T]) :- not(isList(H)) , isLinear(T) .

app([], L, L) :- !.
app(L,[],L) :- !.
app([H|T], L, [H|T2]) :- app(T, L, T2) .


%flatt(P, L) :- isLinear(P). 

%flatt([H|T],P) :- flatt([H|T],P).

flatt([],[]) :- !.
flatt(X,[X]) :- not(isList(X)).
flatt([H|T],P) :- flatt(H,R1), flatt(T,R2),  app(R1, R2, P).

%flatt([H|T],P) :- not(isList(H)), ,flatt(T,P), app()

%flatt(T, P). 



% Premia - predikat na otestovanie, ci sa jedna o linearny zoznam.
% ?- isLinear([1,2,3]).
% true.


 
%   (2b) - len pomocny predikat
%  (+1b) - premia test linearnosti

% spojenie 2 linearnych zoznamov
%app(_,_,_).

% splostenie zoznamu
%flatt(_,_).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%priprava na zkousku

%maximalni prvek seznamu
max_l([X], X) :- !.
max_l([X|T], M) :- max_l(T, M) , M >= X , !.
max_l([_|T], M) :- max_l(T, M).

%soucet 2 poslednich cisel seznamu

soucet([X, Y], Z) :- Z is X + Y, !.
soucet([_|T], Z) :- soucet(T,Z).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%nejvetsi cislo seznamu i s vnorenymi seznami

greater(A,B,C) :- A > B, C is A, !.
greater(A,B,C) :- C is B.

get_max_memeber([M], Z) :- Z is M , !.
get_max_memeber([H|T], Z) :- get_max_memeber(T, TAIL_MAX), greater(H,TAIL_MAX,Z).

%%%%%%%%%%%%%%%%%%%%%%%%%%
%faktorial cisla

factorial(N,R) :- N < 2, R is 1, !.
factorial(X,Y) :- A is X - 1, factorial(A,RR), Y is RR*A.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%fibbonacci
fib(N,R) :- N < 2, R is 1, !.
fib(X,Y) :- A is X - 1, factorial(A,RR), Y is RR+A.

%%%%%%%%%%%%%%%%%%%%%%%%
%predposledni
predp([X,_], X) :- !.
predp([_|T], X) :- predp(T, X).

%%%%%%%%%%%%%%%%%%%%%%%
%member
member([H|_],H) :- !.
member([_|T],A) :- member(T,A). 

%%%%%%%%%%%%%%%%%%%%%%%%
%vlozit prvni
vloz(S, A, S) :- member(A,S) , !.
vloz(S, A, [A|S]).

%%%%%%%%%%%%%%%%%%%%%%%%
%vlozit posledni
vlozpo(A, [], [A]) :- !. 
vlozpo(A, [H|T], [H|N]) :- vlozpo(A, T, N) .

%smaz P
smaz(P, [P|T],T ) :- !.
smaz(P, [H|T], [H|N]) :- smaz(P, T, N) .

% pocet prvku lin. seznamu
pocet([], 0) :- !.
pocet([H|T], P) :- pocet(T, NN), P is NN + 1 . 

%%%%%%%%%%%%%%%%%%%%%
% maximum ze seznamu 2
max2([P], P) :- !.
max2([H1, H2|T], Max) :- H1 >= H2 , max2([H1|T], Max). 
max2([H1, H2|T], Max) :- H1 < H2 , max2([H2|T], Max).


% soucet vsechn prvku
sums([], 0) :- !.
sums([H|T], S) :- sums(T, NN), S is NN + H. 

% index  prvku seznamu
%index2([], P, I) :- I is false , !.
index2([P|_], P, 0) :- !.
index2([H|T], P, I) :-  index2(T, P, NN) , I is NN + 1. 

% index  max prvku seznamu
%index3([], I) :- !.
index3([P|T], 0) :- max2([P|T], P).
index3([H|T], I) :- index3(T, NN) , I is NN + 1. 

% obrat seznam
%obrat([P], [H|K])
%obrat([H|S], [H|N]) :- obrat(S, N).

generuj(O, D, []) :- D < O .
generuj(O, D, [H|T]) :- D >= O, NN is O +1, H is NN-1, generuj(NN, D, T). 

%zistit, ci je linearny zoznam mnozina (teda, ci sa v nom nachadzaju unikatne prvky - neopakuju sa)

isSet([]) :- !.
isSet([H|T]) :- not(member(T, H)), isSet(T).


appseznam([],P,S) :- appseznam([P],P,[P]) ,!.
appseznam(S,P,[P|S]) :-  not(member(S, P)).

% Prolog - součet prvního kladného čísla a posledního záporného čísla v seznamu

prvnikladne([],K) :- fail, !.
prvnikladne([H|T],K) :- H > 0 , K is H ,!.
prvnikladne([H|T],K) :- H < 0 , prvnikladne(T,K) .

prvnizaporne([],K) :- fail, !.
prvnizaporne([H|T],K) :- H < 0 , K is H ,!.
prvnizaporne([H|T],K) :- H >= 0 , prvnizaporne(T,K) .

soucetobou(L, S) :-  prvnizaporne(L, P) , prvnikladne(L,W) , S is P + W , !.

% druhe maximum
druhemax(S, M) :- max2(S, P) , smaz(S, P, N) ,  max2(N, M) .

% smaze prvek ze seznamu 
smaz([], P, S) :- fail.
smaz([P|T],P,T).
smaz([H|T], P, [H|S]) :- smaz(T, P, S).

% soucet 2 maximlanich hodnot
soucetMax(S, M) :-  max2(S, MAX1) , druhemax(S, MAX2), M is MAX1 + MAX2 , !.