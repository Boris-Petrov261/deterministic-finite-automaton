#include "DFA.h"


DFA::DFA(int _numberOfStates, int _startState, set<int> _finalStates) : numberOfStates(_numberOfStates), startState(_startState), finalStates(_finalStates)
{
	for (int i = 0; i < numberOfStates; i++)
		states.insert(i);

	for (char c = 'a'; c <= 'z'; c++)
		alphabet.insert(c);
	for (char c = '0'; c <= '9'; c++)
		alphabet.insert(c);
}

int DFA::transition(int fromState, string input) const
{
	//start from the initial state and change after every transition
	int endState = fromState;
	for (int i = 0; i < input.length(); i++)
	{
		if (transitionTable.count(make_pair(endState, input.at(i))) != 0)
			endState = transitionTable.at(make_pair(endState, input.at(i)));
		else
		{
			cout << "Error: Undefined transition" << endl;
			return -1;
		}
	}
	//return the current state which the automaton is in
	return endState;
}

int DFA::transition(int fromState, char input) const 
{
	//check if transition is defined
	if (transitionTable.count(make_pair(startState, input)) != 0)
		return transitionTable.at(make_pair(startState, input));
	else
	{
		cout << "Error: Undefined transition" << endl;
		return -1;
	}
	
}

bool DFA::isInLanguage(string input) const
{
	int checkState = transition(startState, input);

	if (finalStates.count(checkState) != 0)
		return true;
	else
		return false;
}

void DFA::addState(bool isFinal)
{
	if (isFinal)
		finalStates.insert(numberOfStates);

	states.insert(numberOfStates++);
}

bool DFA::addTransition(int from, char input, int to)
{
	//check for valid input before adding transition
	if (states.count(from) != 0 && alphabet.count(input) != 0 && states.count(to) != 0)
	{
		// f(from, input) = to 
		transitionTable.insert(pair<pair<int, char>, int>(pair<int, char>(from, input), to));
		return true;
	}
	
	return false;
}

void DFA::addErrorState()
{
	int errorStateIndex = numberOfStates;
	//add the error state
	addState(false);

	for (int i = 0; i < numberOfStates; i++)
	{
		for (char c : alphabet)
		{
			if (transitionTable.count(make_pair(i, c)) == 0)
				addTransition(i, c, errorStateIndex);
		}
	}
}

ostream& operator<<(ostream& out, const DFA& dfa)
{
	// print state set
	out << "States: { ";
	for (int i : dfa.states)
	{
		out << i << ", ";
	}
	cout << "\b\b";  //Cursor moves 2 positions backwards
	out << " }" << endl;

	// print final state set
	out << "Final states: { ";
	for (int i : dfa.finalStates)
	{
		out << i << ", ";
	}
	if (!dfa.finalStates.empty())
		cout << "\b\b";  
	out << " }" << endl;

	// print alphabet
	out << "Alphabet: { ";
	for (char c : dfa.alphabet)
	{
		out << c << ", ";
	}
	cout << "\b\b";  
	out << " }" << endl;

	// print start state
	out << "Start state: " << dfa.startState << endl;

	// print state transitions
	out << "Transitions: " << endl;

	for (int i : dfa.states)
	{
		out << "State " << i << ":\n";
		for (char c : dfa.alphabet)
		{
			if (dfa.transitionTable.count(make_pair(i, c)) != 0)
				out << "f(" << i << "," << c << ") = " << dfa.transitionTable.at(make_pair(i, c)) << endl;
		}
	}

	return out;
}

// Convert DFA to regular expression using Kleene's algorithm
// https://en.wikipedia.org/wiki/Kleene%27s_algorithm
string DFA::regularExpression() const
{
	// this will be our result
	string resultingExpression = "";

	//create the array R[i][j][k+1] where R[s][m][r] is the regular expression(as a string) of all the words 
	//that take the automaton from state s to state m without passing through a state numbered higher than r
	//Note: s and m can be higher than r but no intermediate state can
	// # will be the empty set
	// $ will be epsilon (empty word)
	int n = numberOfStates;
	string*** R = new string**[n+1];
	for (int i = 0; i <= n; i++)
		R[i] = new string*[n+1];
	for (int i = 0; i <= n; i++)
	for (int j = 0; j <= n; j++)
		R[i][j] = new string[n+1];


	//initial sets for k = 0
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			for (char c : alphabet)
			{
				if (transitionTable.count(make_pair(i-1, c)) != 0)
				if (transitionTable.at(make_pair(i-1, c)) == j-1)
				{
					R[i][j][0] = R[i][j][0] + c + "+";
				}

			}

			if (!R[i][j][0].empty())
			{
				R[i][j][0].pop_back();
				if (i == j)
					R[i][j][0] = R[i][j][0] + string("+$");
			}
			else // no transition from i to j
			{
				if (i == j)
					R[i][j][0] = "$"; //empty word
				else
					R[i][j][0] = "#"; //empty set
			}

		}
	}

	
	// induction k = 1.....n+1
	for (int k = 1; k <=n; k++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				R[i][j][k] = "(" + R[i][j][k - 1] + ")" + "+" + "(" + R[i][k][k - 1] + ")." + "(" + R[k][k][k - 1] + ")*." + "(" + (R[k][j][k - 1]) + ")";
			//	cout << R[i][j][k] << endl;
			}
		}
	}
	
	for (int i : finalStates)
	{
		resultingExpression = resultingExpression + R[startState + 1][i + 1][n] + "+";
	}
	//delete last plus sign
	resultingExpression.pop_back();

	
	
	//free up memory
	for (int i = 0; i <= n; i++)
	for (int j = 0; j <= n; j++)
		delete[] R[i][j];
	for (int i = 0; i <= n; i++)
		delete[] R[i];
	delete[] R;

	// expression is not very pretty but it is correct; simplification is possible using kleene algebra
	return resultingExpression;
} 

//OPERATIONS

//UNION
DFA DFA::operator+(const DFA& other) const
{ 
	// to get a union dfa we need to create a dfa with  numberOfStates*other.numberOfStates states 
	// the transtition table will be a multiplication of the dfa's tables

	int resultNumberOfStates = numberOfStates*other.numberOfStates;
	int resultStartState = startState*other.numberOfStates + other.startState;
	DFA resultingAutomaton(resultNumberOfStates, resultStartState, {}); // leave final states set empty for now

	// add the transitions for the new automaton
	for (int i = 0; i < numberOfStates; i++)
	{
		for (int j = 0; j < other.numberOfStates; j++)
		{
			for (char c : alphabet)
			{
				int currentState = i*other.numberOfStates + j;
				if (finalStates.count(i) != 0 || other.finalStates.count(j) != 0)
					resultingAutomaton.finalStates.insert(currentState);

				int firstDFATransition = 0, secondDFATransition = 0;
				bool firstHasTransition = false, secondHasTransition = false;
				if (transitionTable.count(make_pair(i, c)) != 0)
				{
					firstDFATransition = transitionTable.at(make_pair(i, c));
					firstHasTransition = true;
				}
				if (other.transitionTable.count(make_pair(j, c)) !=0)
				{
					secondDFATransition = other.transitionTable.at(make_pair(j, c));
					secondHasTransition = true;
				}

				if (firstHasTransition != false || secondHasTransition != false)
					resultingAutomaton.addTransition(currentState, c, firstDFATransition*other.numberOfStates + secondDFATransition);

			}
		}
	}

	return resultingAutomaton;
}

//INTERSECTION
DFA DFA::operator*(const DFA& other) const
{
	// De Morgan's law
	return !(!(*this) + !other);
}

//COMPLEMENT
DFA DFA::operator!() const
{
	// create new dfa with the same characteristics 
	DFA dfa(*this);

	// change every non-final state to final and every final state to non-final
	for(int i = 0; i < dfa.numberOfStates; i++)
	{
		if(finalStates.count(i) != 0)
			dfa.finalStates.erase(i);
		else
			dfa.finalStates.insert(i);
	}

	return dfa;
}

int DFA::getNumberOfStates() const
{
	return numberOfStates;
}

int DFA::getStartStateIndex() const
{
	return startState;
}

bool DFA::setFinal(int index)
{
	if (states.count(index) != 0)
	{
		//check if state is not already final
		if (finalStates.count(index) == 0)
			finalStates.insert(index);
		return true;
	}
	else //invalid index
		return false;
}

/*
bool DFA::isLanguageFinite() const
{
	bool hasCycle = false;
	// we need to find whether there are any cycles in the automaton
	

}

/*
bool DFA::isLanguageEmpty() const
{

}



bool DFA::isLanguageComplete() const
{

} */

