#include <iostream>
#include "DFA.h"
using namespace std;

int main()
{
	//Test for different deterministic finite automata

	cout << "\nDFA1:\n";
	DFA dfa1;
	dfa1.addState(true);
	dfa1.addTransition(0, 'a', 1);
	dfa1.addTransition(0, 'b', 1);
	dfa1.addTransition(1, 'a', 0);
	dfa1.addTransition(1, 'b', 0);
	dfa1.addTransition(1, 'c', 1);
	cout << dfa1;
	cout << dfa1.isInLanguage("ababba") << endl;
	cout << dfa1.isInLanguage("aaacccccccccccccccccccba") << endl;

	cout << "\nDFA2:\n";
	DFA dfa2(2, 0, { 0 });
	cout << dfa2;
	dfa2.addTransition(0, 'a', 0);
	dfa2.addTransition(0, 'b', 1);					 
	dfa2.addTransition(1, 'a', 1);
	dfa2.addTransition(1, 'b', 0);
	cout << dfa2;
	cout << dfa2.isInLanguage("aaaaabb") << endl;

	// UNION of automata dfa1 and dfa3
	cout << "\nDFA3:\n";
	DFA dfa3 = dfa1 + dfa2;
	cout << dfa3;
	cout << dfa3.isInLanguage("abbaba") << endl;
	cout << dfa3.isInLanguage("aaaaabbaa") << endl;
	cout << dfa3.isInLanguage("aaacccccccccccccccccccba") << endl;


	// COMPLEMENT of dfa3
	cout << "\nDFA4:\n";
	DFA dfa4 = !dfa3;
	cout << !dfa3;
	cout << dfa4.isInLanguage("abbaba") << endl;
	cout << dfa4.isInLanguage("aaaaabbaa") << endl;
	cout << dfa4.isInLanguage("aaacccccccccccccccccccba") << endl;

	// INTERSECTION of dfa1 and dfa2
	cout << "\nDFA5:\n";
	DFA dfa5 = dfa1*dfa2;
	cout << dfa5; 
	cout << dfa5.isInLanguage("aaabb") << endl; // accepted by both dfa1 and dfa2 so returns 1
	cout << dfa5.isInLanguage("aab") << endl;   // accepted by dfa1 but not dfa2 so returns 0
	
	cout << "\nDFA7:\n";
	DFA dfa7(1, 0, { 0 });
	dfa7.addState(true);
	dfa7.addTransition(1, 'a', 0);
	dfa7.addTransition(0, 'a', 1);
	cout << "reg. expr. test: \n"<<dfa7.regularExpression() << endl; // looks ugly but ultimately equates to  a + a(aa)*

	cout << endl;

	cout << "\nDFA6:\n";
	DFA dfa6;
	dfa6.addState(false);
	dfa6.addState(true);
	dfa6.addTransition(0, 'a', 1);
	dfa6.addTransition(1, 'b', 2);
	cout << dfa6.regularExpression() << endl;

	system("pause");
	return 0;
}