
#ifndef DFA_HPP_INCLUDED
#define DFA_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
using namespace std;
class DFA
{
    int stareInitiala;
    set<int> stariFinale;

    vector<map<char, int>> tranzitii;

    vector <bool> eStareInaccesibila;
    vector <bool> stareInaccesib;

    static const string SIGMA; //alfabetul
public:
    DFA();
    DFA(vector <map <char, int>>, int, set <int>);

    friend istream& operator >> (istream&, DFA&);   
    friend ostream& operator << (ostream&, const DFA&);

    void marcareStariInaccesibile();

    void marcareDeadStates();

    bool eStareAccesibila(int) const;

    DFA minimizare();
};

#endif // DFA_HPP_INCLUDED
