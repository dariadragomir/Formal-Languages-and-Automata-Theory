#include "NFA.hpp"
#include <fstream>
using namespace std;
ifstream fin("input.txt");
ofstream fout("output.txt");
int main()
{
    NFA automat;
    fin >> automat;
    fout << "Automatul initial:\n" << automat << '\n';

    automat.lambda_NFA_to_NFA();
    fout << "Automatul dupa eliminarea lambda-tranzitiilor:\n" << automat << '\n';

    DFA dfa = DFA(automat);
    fout << "Automatul dupa transformarea din NFA in DFA:\n" << dfa << '\n';

    dfa = dfa.minimizare();
    fout << "DFA-ul minimizat:\n" << dfa << '\n';
    fin.close();
    fout.close();
    return 0;
}