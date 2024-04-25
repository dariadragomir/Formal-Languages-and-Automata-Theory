#include "NFA.hpp"
#include <queue>
using namespace std;
const string NFA::SIGMA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
char NFA::lambda = '$';
NFA::NFA() {}

NFA::NFA(int nr_stari): tranzitii(vector<map<char, set<int>>>(nr_stari + 1)) // +1 pentru ca starea initiala e pe poz 1 
{}
    
istream& operator >> (istream& in, NFA& automat)
{
    int nrStari, nrTranzitii;
    in >> nrStari;
    in >> nrTranzitii;

    automat = NFA(nrStari);

    for (int i = 0; i < nrTranzitii; ++i) {
        int q1, q2; // starea initiala, starea finala din tranzitia curenta
        char ch;    // caracterul din alfabet

        if (ch == automat.lambda && q1 == q2) 
            continue;

        automat.tranzitii[q1][ch].insert(q2);
    }

    in >> automat.stareInitiala; // q0

    int nrStariFInale;
    in >> nrStariFInale; 

    for (int i = 0; i < nrStariFInale; ++i) {
        int q;
        in >> q;
        automat.stariFinale.insert(q);   // retin starile finale intr-un unordered set
    }

    return in;
}

ostream& operator << (ostream& out, const NFA& automat)
{
    out << "Numarul de stari = " << (int)automat.tranzitii.size() - 1 << "\n";
    for (int stare = 1; stare < (int)automat.tranzitii.size(); ++stare) {
        for (auto& map : automat.tranzitii[stare]) 
            for (auto& next_state : map.second) 
                out << stare << " --" << map.first << "--> " << next_state << '\n';
    }
    out << "Starea initiala = " << automat.stareInitiala << '\n';

    out << "Stari finale: ";
    for (auto& state : automat.stariFinale) 
        out << state << ' ';
    out << '\n';

    return out;
}

void NFA::lambda_NFA_to_NFA()
{
    int nr_states = tranzitii.size();

    bool aFostModificat = 1;
    while (aFostModificat) {
        aFostModificat = 0;

        // cat timp nu mai apar noi tranzitii de adaugat la delta
        // pentru fiecare p, q, r in Q:
        // daca (p, lambda, q), (q, a, r) sunt in delta, adaugam (p, a, r) la delta 

        for (int p = 1; p < nr_states; p++) {   
            if (!tranzitii[p].count(lambda))
                continue;

            for (int q : tranzitii[p][lambda]) {
                for (auto& map : tranzitii[q]) 
                    for (auto& r : map.second) {
                        tranzitii[p][map.first].insert(r);   
                        aFostModificat = 1;
                    }

                // daca q e stare finala si p nu e stare finala
                if (stariFinale.find(q) != stariFinale.end() && stariFinale.find(p) == stariFinale.end()) {
                    stariFinale.insert(p);     // p stare finala
                    aFostModificat = 1;
                }
            }
            tranzitii[p][lambda].clear();     // stergem lambda-tranzitiile
        }
    }
}

// Conversia NFA to DFA
NFA::operator DFA()
{
    vector <map <char, int>> tranzitiiNoi;
    tranzitiiNoi.emplace_back(); // indexarea starilor incepe de la 1 =>pozitia 0 nu e folosita

    int nouaStareInitiala = stareInitiala;
    set <int> noiStariFinale; 

    map <set <int>, int> indexSubsetStare;
    set <int> aux; // noua stare curenta in DFA creata dintr-un subset de stari din NFA

    queue<set<int>> queue;

    int indexCurent = 1;
    aux.insert(stareInitiala);
    queue.push(aux);
    if (stariFinale.find(stareInitiala) != stariFinale.end())    // e stare finala
        noiStariFinale.insert(indexCurent);

    indexSubsetStare[aux] = indexCurent;
    tranzitiiNoi.emplace_back(map<char, int>());
    aux.clear();

    indexCurent++;

    while (!queue.empty()) {
        set<int> StariCurente = queue.front();
        queue.pop();

        for (char input_symbol : SIGMA) {
            for (auto& p : StariCurente) 
                for (auto& q: tranzitii[p][input_symbol]) 
                    aux.insert(q);

            if (aux.empty())
                continue;
         
            if (indexSubsetStare.find(aux) == indexSubsetStare.end()) {
                indexSubsetStare[aux] = indexCurent;
                indexCurent++;
                for (auto& q : aux) 
                    if (stariFinale.find(q) != stariFinale.end())    // e stare finala
                        noiStariFinale.insert(indexSubsetStare[aux]);
                
                tranzitiiNoi.emplace_back(map<char, int>());
                queue.push(aux);
            }  
            tranzitiiNoi[indexSubsetStare[StariCurente]][input_symbol] = indexSubsetStare[aux];
            aux.clear();
        }
    }

    return DFA(tranzitiiNoi, nouaStareInitiala, noiStariFinale);
}