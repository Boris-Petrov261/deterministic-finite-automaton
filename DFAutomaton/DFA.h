/*
Author: Boris Petrov 
Date: 17.6.2015
This small project presents a possible implementation of a deterministic finite automaton.
A deterministic finite automaton(DFA) recognizes a particular regular language and is used 
to evaluate whether a given string(containing symbols) is in that language. In that case we say
that the string is accepted by the DFA.
From a mathematical persepective a DFA is a quintuple: <Q, S, F, A, f> where
Q - set of states
F - subset of Q, final states
S - starting state
A - alphabet (set of symbols, can be latin chars, digits etc)
f - transition function. Given a state and a symbol from the alphabet, returns another state  e.g. f(q0, a) = q1
A string is accepted by the DFA if after going through its whole length the transition function 
returns a final state
*/


#ifndef DFA_H
#define DFA_H
#include <iostream>
#include <utility>  //std::pair
#include <map>		//std::map
#include <set>		//std::set
#include <string>

using namespace std;

class DFA
{
private:
	//extended transition function of DFA
	//takes any string and returns the state which the automaton is in when it has gone through all the letters in the string
	int transition(int, string) const ;
	int transition(int, char) const;

	int startState;
	int numberOfStates;

	set<int> states;
	set<int> finalStates;
	set<char> alphabet;
	
	map<pair<int, char>, int> transitionTable;
	
public:
	/* @param _numberOfStates - cardinality of the states set, each state's name is an integer from 0 to _numberOfStates
	   @param _startState - the initial state
	   @param _finalStates - subset of {0, ... , _numberOfStates-1} */
	DFA(int _numberOfStates = 1, int _startState = 0, set<int> _finalStates = {});
public:
	bool isInLanguage(string) const;
	void addState(bool);
	bool addTransition(int, char, int);
	void addErrorState();
	string regularExpression() const;
	friend ostream& operator<<(ostream&, const DFA&);

	//operations
	DFA operator+(const DFA&) const;		// UNION
	DFA operator*(const DFA&) const;		// INTERSECTION
	DFA operator!() const;					// COMPLEMENT

	int getNumberOfStates() const;
	int getStartStateIndex() const;

	//returns true if argument is a valid state index
	bool setFinal(int); 
	
};

#endif