/*
* 2nd project FLP - Turing machine
* @author: Bc. Alena Tesarova (xtesar36)
* @date: 04/2020
*/

:- dynamic prechod/3.
:- discontiguous createPredicates/1.
:- discontiguous createPredicatesAssert/1.

% function from /Samples/cat.pl in FLP folder
% Reads line from stdin, terminates on LF or EOF.
readLine(L, C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		readLine(LL, _), %atom_chars(C,[Cd]),
		[C|LL] = L).

%Tests if character is EOF or LF.
isEOFEOL(C) :-
	C == end_of_file; 
	(char_code(C,Code), Code==10).

print(C) :- write(C), write('\n').

% Function makes a list from input
makeListFromLine(List) :- 
	readLine(Head, C),
	(
		C == end_of_file,
		List = [], !
	;
		makeListFromLine(ListTale), 
		List = [Head|ListTale]
	).

/*************************** PROCESSING INPUT ****************************************/
% Finds last item in the List
lastItemInTheList([LastItem], LastItem).
lastItemInTheList([_|T], LastItem) :- lastItemInTheList(T,LastItem).

deleteLastItemInTheList([_], []).
deleteLastItemInTheList([H|T], [H|NewList]) :- deleteLastItemInTheList(T,NewList).

detectionFinalState([]) :- false, !.
detectionFinalState([Head|Tale]) :- detectFinal(Head); detectionFinalState(Tale).
detectFinal([_,_,State,_]) :- State == 'F', true, !.

% Deletes space from all the rules
%deleteSpacesFromRules(Rules, NewRules)
deleteSpacesFromRules([],[]).
deleteSpacesFromRules([Head|Tale], [NewHead|NewTale]) :- 
	deleteSpacesFromList(Head, NewHead),
	deleteSpacesFromRules(Tale, NewTale).

% Deletes space from  one rule
%deleteSpacesFromList(List,NewListWithoutSpaces) 
deleteSpacesFromList([],[]).
deleteSpacesFromList([Head|Tale],[Head|NewTale]) :- Head \= ' ', deleteSpacesFromList(Tale,NewTale).
deleteSpacesFromList([_|Tale],NewListWithoutSpaces) :- deleteSpacesFromList(Tale,NewListWithoutSpaces).

% Adds initial state S to initial configuration
addInitialStateToInput(InputList, NewInputList) :- 
	NewInputList = ['S'|InputList].

separate([Head|[SymbolAfter|Tale]], BeforeList, Head, SymbolAfter, After) :- 
		char_type(Head, upper),
		After = Tale,
		BeforeList = [],
		!.
separate([Head|[Symbol|Tale]], [Head|BeforeList], State, SymbolOnTape, After) :- separate([Symbol|Tale], BeforeList,  State, SymbolOnTape, After).

/*************************** PRINTING *******************************/
%Function formats a list and prints it to output
printList(L) :- 
	writef("Printing list:"),
	write(L),
	write("\n").	

printConfig([]) :- write('\n').
printConfig([H|T]) :- 
	write(H),
	printConfig(T).

% Prints tape
printTape([]) :- !.
printTape([Head|Tape]) :-
	printConfig(Head),
	printTape(Tape).

join([],[]).
join([[Before,State,Symbol, After]|Tale], [HeadNew|ConfigNew]) :- joinConfig([Before,State,Symbol, After], HeadNew2), append(HeadNew2, [State|[Symbol|After]], HeadNew), join(Tale,ConfigNew).
joinConfig([[], _, _, _],[]). 
joinConfig([[Head|Before], State, Symbol, After], HeadNew) :- joinConfig([Before,State, Symbol, After],HeadNew2), append(HeadNew2,[Head],HeadNew).
/*************************************************************************/

/********************* ADDING DYNAMIC PREDICATES **************************/
%prechod('left',[[], _, _, _], [_]) :-write("ERROR, out of tape"), halt.

%addItemToListInTheEnd(List, NewItem, NewList)
addItemToListInTheEnd([],NewItem,[NewItem]) :- !.
addItemToListInTheEnd([Head|Tale], NewItem, [Head|NewListTale]) :- addItemToListInTheEnd(Tale,NewItem,NewListTale).

createPredicates([]) :- !.
createPredicates([Head|Tale]) :- createPredicatesAssert(Head), createPredicates(Tale).
createPredicatesAssert([StateFrom, SymbolAfter, StateNext, 'R']) :- 
	assert(
		prechod('right',[BeforeList, StateFrom, SymbolAfter, [Head|Tail]], [ [SymbolAfter|BeforeList], StateNext, Head, Tail]) ), 
	assert(
		prechod('right',[BeforeList, StateFrom, SymbolAfter, [Head]], [ [SymbolAfter|BeforeList], StateNext, Head, []]) ),
	assert(
		prechod('right',[BeforeList, StateFrom, SymbolAfter, []], [ [SymbolAfter|BeforeList], StateNext, '', []]) ),!.

createPredicatesAssert([StateFrom, SymbolAfter, StateNext, 'L']) :- 
	assert( 
		prechod('left', [ [Head|BeforeList], StateFrom, SymbolAfter, EndList], [BeforeList, StateNext, Head, [SymbolAfter|EndList] ] ) ), 
	assert( 
		prechod('left', [ [Head], StateFrom, SymbolAfter, EndList], [[], StateNext, Head, [SymbolAfter|EndList] ] ) ), !.

createPredicatesAssert([StateFrom, SymbolAfter, StateNext, TapeNext]) :- 
	assert( prechod('normal',[BeforeList, StateFrom, SymbolAfter, EndList], [BeforeList, StateNext, TapeNext, EndList])), !.

% createPredicates([['S',a,'B',a],['B',a,'B','R'], ['B',b,'B','R'],['B',c,'B','a'],['B',c,'F','c'],['B',c,'B','a']]).

/************** MAIN FUNCTION MOVE *****************************************************/
move([_,'F',_,_],[],_).
move(Config, [NewConfig|Next],Iterate) :- \+ Iterate == 10000, Inc is Iterate + 1, move(NewConfig,Next,Inc), prechod(_,Config, NewConfig).	
move(_,_,10000) :- writef("ERROR: NTS reached operation limit\n").

/*************** ENTRY POINT ************************************************************/
prolog :-
	prompt(_, ''),
	makeListFromLine(InputList),
	lastItemInTheList(InputList,InputString),			 % last row indetifies the input of turingmachine
	deleteLastItemInTheList(InputList,RulesOfMachine),	 % without the input we will get just rules that we need
	addInitialStateToInput(InputString, InputStringWithInitial),
	deleteSpacesFromRules(RulesOfMachine,RulesOfMachineWithoutSpace),
	printConfig(InputStringWithInitial), 				% printing initial configuration
	createPredicates(RulesOfMachineWithoutSpace),
	!, detectionFinalState(RulesOfMachineWithoutSpace),
	separate(InputStringWithInitial,  Pred, State, Symbol, After),
	Config2 = [Pred, State, Symbol, After],
	!,
	move(Config2,ConfigFinal, 0),
	%printList(ConfigFinal),
	join(ConfigFinal, ConfigFinalTogether),
	printTape(ConfigFinalTogether).


	
