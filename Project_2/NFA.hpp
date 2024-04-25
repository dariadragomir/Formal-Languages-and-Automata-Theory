#ifndef NFA_HPP_INCLUDED
#define NFA_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <map>
#include <string>

#include "DFA.hpp"
using namespace std;
class NFA
{
    int stareInitiala;
    set<int> stariFinale;

    vector<map<char, set<int>>> tranzitii;

    static char lambda;
    static const string SIGMA; //alfabetul
public:
    NFA();
    NFA(int);

    friend istream& operator >> (istream&, NFA&);
    friend ostream& operator << (ostream&, const NFA&);

    void lambda_NFA_to_NFA();

    explicit operator DFA();
};


#endif // NFA_HPP_INCLUDED