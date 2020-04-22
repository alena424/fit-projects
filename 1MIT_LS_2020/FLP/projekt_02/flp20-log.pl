/*
* 2nd project FLP - Turing machine
* @author: Bc. Alena Tesarova (xtesar36)
* @date: 04/2020
*/

% function from /Samples/cat.pl in FLP folder
%Reads line from stdin, terminates on LF or EOF.
readLine(L, C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		readLine(LL, _), %atom_chars(C,[Cd]),
		[C|LL] = L).

%Tests if character is EOF or LF.
isEOFEOL(C) :-
	%C == end_of_file, halt; 
	C == end_of_file; 
	(char_code(C,Code), Code==10).

print(C) :- write(C), write('\n').

% Function makes a list from input
makeListFromLine(List) :- 
	readLine(Head, C),
	writef("Reading: \n"),
	(
		C == end_of_file,
		writef("End of line: \n"),
		List = [], !
	;
		writef("Calling: \n"),
		makeListFromLine(ListTale), 
		List = [Head|ListTale]
	).

% Finds last item in the List
lastItemInTheList([LastItem], LastItem).
lastItemInTheList([_|T], LastItem) :- lastItemInTheList(T,LastItem).

deleteLastItemInTheList([_], []).
deleteLastItemInTheList([H|T], [H|NewList]) :- deleteLastItemInTheList(T,NewList).


%deleteSpacesFromRules(Rules, NewRules)
deleteSpacesFromRules([],[]).
deleteSpacesFromRules([Head|Tale], [NewHead|NewTale]) :- 
	deleteSpacesFromList(Head, NewHead),
	deleteSpacesFromRules(Tale, NewTale).

%deleteSpacesFromList(List,NewListWithoutSpaces) 
deleteSpacesFromList([],[]).
deleteSpacesFromList([Head|Tale],[Head|NewTale]) :- Head \= ' ', deleteSpacesFromList(Tale,NewTale).
deleteSpacesFromList([_|Tale],NewListWithoutSpaces) :- deleteSpacesFromList(Tale,NewListWithoutSpaces).

addInitialStateToInput(InputList, NewInputList) :- 
	NewInputList = ['S'|InputList].

%Function formats a list and prints it to output
printList(L) :- 
	writef("Printing list:"),
	write(L),
	write("\n").	

printConfig([]) :- write('\n').
printConfig([H|T]) :- 
	write(H),
	printConfig(T).


%changeState(RulesOfMachine,ActualState,SymbolOnTape,FoundRule)
%findRuleFromList([],_,_,_,[],_,_,_) :- writef("error\n"), fail. 
findRuleFromList([Head|Tale],ActualState,SymbolOnTape,ConfigurationString,FoundRule,NewConfiguration,ConfigList,NewState) :-
		(
			findRule(Head, ActualState, SymbolOnTape),
			FoundRule = Head,
			FoundRule = [_|[_|[NewState|NewSymbolOnTapeList]]],
			getFirst(NewSymbolOnTapeList,NewSymbolOnTape),

			applyRule(ConfigurationString,ActualState, SymbolOnTape, NewConfiguration,NewState,NewSymbolOnTape)
			/*(
				isAlreadyInList(ConfigList,NewConfiguration), % loop detected
				findRuleFromList(Tale,ActualState,SymbolOnTape,ConfigurationString,FoundRule,NewConfiguration,ConfigList,NewState) % we will try to find a different one 
			;
				!
			)*/
		;
			findRuleFromList(Tale,ActualState,SymbolOnTape,ConfigurationString,FoundRule,NewConfiguration,ConfigList,NewState)
		).

%findRule(Rule,ActualState,SymbolOnTape)
findRule(Rule,ActualState,SymbolOnTape) :-
	Rule = [StateFrom,SymbolOn,_,_],
	StateFrom == ActualState,
	SymbolOnTape == SymbolOn.


%changeConfig(ConfigurationString,ActualState, SymbolOnTape,NewState,NewSymbolOnTape,NewConfigString)
changeConfig([],_,_,_,_,[]).
changeConfig([Head|[_|Tale]],Head, SymbolOnTape,NewState,NewSymbolOnTape,[NewState|[NewSymbolOnTape|TaleNew]]) :-
	changeConfig(Tale,Head,SymbolOnTape,NewState,NewSymbolOnTape,TaleNew).

changeConfig([Head|Tale],ActualState, SymbolOnTape,NewState,NewSymbolOnTape,[Head|TaleNew]) :-
	changeConfig(Tale,ActualState,SymbolOnTape,NewState,NewSymbolOnTape,TaleNew).

%switchToRight(ConfigurationString,ActualState,NewConfigString)
moveRightOnTape([],_,[]).

% last symbol
moveRightOnTape([Head],Head,[' '|[Head|TaleNew]]) :-
	moveRightOnTape(_,Head,TaleNew).

moveRightOnTape([Head|[Symbol|Tale]],Head,[Symbol|[Head|TaleNew]]) :-
	moveRightOnTape(Tale,Head,TaleNew).

moveRightOnTape([Head|Tale],ActualState,[Head|TaleNew]) :- 
	moveRightOnTape(Tale,ActualState,TaleNew).


checkHeadWhileMovingLeft([Head|_],Head) :-
	writef("We got out of tape on left side"),
	fail.

moveLeftOnTape([],_,[]).
moveLeftOnTape([Symbol|[Head|Tale]],Head,[Head|[Symbol|TaleNew]]) :-
	moveLeftOnTape(Tale,Head,TaleNew).

moveLeftOnTape([Head|Tale],ActualState,[Head|TaleNew]) :- 
	moveLeftOnTape(Tale,ActualState,TaleNew).


moveOnTape().

%applyRule(ConfigurationString,ActualState, SymbolOnTape,FoundRule, NewConfiguration,NewState,NewSymbolOnTape)
applyRule(ConfigurationString,ActualState, SymbolOnTape, NewConfiguration,NewState,NewSymbolOnTape) :-
(
	NewSymbolOnTape == 'R',
	moveRightOnTape(ConfigurationString,ActualState,NewConfiguration)
;
	NewSymbolOnTape == 'L',
	checkHeadWhileMovingLeft(ConfigurationString,ActualState),
	moveLeftOnTape(ConfigurationString,ActualState,NewConfiguration)
;
	changeConfig(ConfigurationString,ActualState, SymbolOnTape,NewState,NewSymbolOnTape,NewConfiguration)

).

%findSymbolOnTape(ConfigurationString, ActualState,SymbolOnTape) :-
findSymbolOnTape([],_,' '). % if we dont find, we will but blank
findSymbolOnTape([Head|[Symbol|_]],Head,Symbol).
findSymbolOnTape([_|Tale],ActualState,SymbolOnTape) :- findSymbolOnTape(Tale,ActualState,SymbolOnTape).

emptyList([]) :- true.
emptyList(_) :- false.

getFirst([H],H).

%isAlreadyInList(List, Element)
isAlreadyInList([],_) :- false.
isAlreadyInList([Head|_], Head) :- true.
isAlreadyInList([_|Tale], Element) :- isAlreadyInList(Tale,Element).

simulateTuring(ConfigurationString,RulesOfMachine,ActualState,ConfigList) :-
	%writef("start SIM\n"),
	findSymbolOnTape(ConfigurationString, ActualState,SymbolOnTape),

	findRuleFromList(RulesOfMachine,ActualState,SymbolOnTape,ConfigurationString,FoundRule,NewConfiguration,NewTape,NewState),
	(
		emptyList(FoundRule),
		false
	;
		printConfig(NewConfiguration),
		(
			NewState == 'F',	 % we got to the final state so we can finish simulation
			print([NewConfiguration|ConfigList]),
			true
		;
			simulateTuring(NewConfiguration,RulesOfMachine,NewState,NewConfig),
			ConfigList = [NewConfiguration|NewConfig]
		)
	).
	

%entry point
prolog :-
	prompt(_, ''),
	makeListFromLine(InputList),
	printList(InputList),
	lastItemInTheList(InputList,InputString),			 % last row indetifies the input of turingmachine
	deleteLastItemInTheList(InputList,RulesOfMachine),	 % without the input we will get just rules that we need
	addInitialStateToInput(InputString, InputStringWithInitial),
	deleteSpacesFromRules(RulesOfMachine,RulesOfMachineWithoutSpace),
	
	printList(InputStringWithInitial),
	printList(RulesOfMachineWithoutSpace),
	printConfig(InputStringWithInitial),
	!,
	simulateTuring(InputStringWithInitial,RulesOfMachineWithoutSpace,'S',[InputStringWithInitial]).


	
