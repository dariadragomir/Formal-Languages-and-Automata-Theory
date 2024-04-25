#include "DFA.hpp"

#include <queue>
using namespace std;
const string DFA::SIGMA = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

DFA::DFA() {}

DFA::DFA(vector <map <char, int>> transitions, int start_state, set <int> final_states): 
    tranzitii(transitions), stareInitiala(start_state), stariFinale(final_states)
{}

ostream& operator << (ostream& out, const DFA& automat)
{
    out << "Numar de stari = " << (int)automat.tranzitii.size() - 1 << "\n";
    for (int stare = 1; stare < (int)automat.tranzitii.size(); ++stare)
        for (auto& map : automat.tranzitii[stare]) 
            if (map.second) 
                out << stare << " --" << map.first << "--> " << map.second << '\n';

    out << "Stare initiala = " << automat.stareInitiala << '\n';

    out << "Stari finale: ";
    for (auto& state : automat.stariFinale) 
        out << state << ' ';
    out << '\n';

    return out;
}

void DFA::marcareStariInaccesibile()
{
    stareInaccesib = vector<bool>((int)tranzitii.size(), true); 
    queue<int> q;
    q.push(stareInitiala);
    stareInaccesib[stareInitiala] = false;
    while (!q.empty()) {
        int state = q.front();
        q.pop();
        for (auto& newTransition : tranzitii[state]) 
            if (stareInaccesib[newTransition.second]) {
                stareInaccesib[newTransition.second] = false; 
                q.push(newTransition.second);  
            }
    }
}
//starile din care nu se poate ajunge intr-o stare finala
void DFA::marcareDeadStates()
{
    int nrStari = (int)tranzitii.size();
    vector<vector<int>> reverseTranzitii(nrStari);

    for (int stare = 1; stare < nrStari; ++stare)
        for (auto map : tranzitii[stare])
            reverseTranzitii[map.second].emplace_back(stare);

    // isDeadState[q] = 1 daca nu pot ajunge in stare finala din q
    eStareInaccesibila = vector<bool>(nrStari, true); 
    queue<int> q;
    for (auto& state : stariFinale) {  // incepand de la setul de stari finale
        q.push(state);
        eStareInaccesibila[state] = false;
    }
    while (!q.empty()) {
        int stare = q.front();
        q.pop();
        for (auto& new_state : reverseTranzitii[stare]) { // mergem la fiecare stare care poate fi accesata
            if (eStareInaccesibila[new_state] == true) {
                eStareInaccesibila[new_state] = false;
                q.push(new_state);
            }
        }
    }
}

bool DFA::eStareAccesibila(int stare) const
{
    return (eStareInaccesibila[stare] == false && stareInaccesib[stare] == false);
}

// minimizare DFA
DFA DFA::minimizare()
{
    marcareStariInaccesibile();
    marcareDeadStates();

    // Gasim starile echivalente
    int nrStari = (int)tranzitii.size();

    vector <vector <bool>> suntEchivalente(nrStari, vector <bool>(nrStari, true));

	for (int q = 1; q < nrStari; ++q) {
		for (int r = q + 1; r < nrStari; ++r) {
			bool qEStareFinala = stariFinale.find(q) != stariFinale.end();
			bool rEStareFinala = stariFinale.find(r) != stariFinale.end();
			if (qEStareFinala != rEStareFinala) 
				suntEchivalente[q][r] = false;
        }
	}

    // pentru fiecare pereche de stari (q, r)
    // daca starile (tranzitii(q, input_symbol), tranzitii(r, input_symbol)) nu sunt echivalente =>atunci (q, r) nu sunt echivalente
    bool aFostModificat = 1;
    while (aFostModificat) {
        aFostModificat = 0;
        for (int q = 1; q < nrStari; ++q) {
		    for (int r = q + 1; r < nrStari; ++r) {
                for (char input_symbol : SIGMA) {
                    if (tranzitii[q][input_symbol] && tranzitii[r][input_symbol]) {
                        int qNext = tranzitii[q][input_symbol];
                        int rNext = tranzitii[r][input_symbol];
                        if (!suntEchivalente[qNext][rNext] && suntEchivalente[q][r]) {
                            suntEchivalente[q][r] = false;
                            aFostModificat = 1;
                        }
                    }
                }
            }
        }
    }
  
    vector <set<int>> grupe;
    vector <bool> vizitat(nrStari, false);
    for (int stare = 1; stare < nrStari; ++stare) {
        if (!vizitat[stare]) {
            vizitat[stare] = true;
            grupe.push_back({stare});
            int poz_cur = grupe.size() - 1;    // pozitia grupei curente de stari

            queue <int> q;  //in q vom avea grupul de stari echivalente cu "stare"
            q.push(stare);
            while (!q.empty()) {
                int q1 = q.front();
                q.pop();
                // iteram prin toate starile echivalente cu q1
                for (int q2 = q1 + 1; q2 < nrStari; ++q2) {
                    if (suntEchivalente[q1][q2]) {
                        if (!vizitat[q2]) {
                            vizitat[q2] = true;
                            q.push(q2);
                            grupe[poz_cur].insert(q2);
                        }
                    }
                }
            }
        }
    }
    vector <int> state_index(nrStari);

    for (size_t index = 0; index < (int)grupe.size(); ++index)
        for (auto& state : grupe[index]) 
            state_index[state] = index + 1;

    int new_nrStari = (int)grupe.size() + 1;

    int new_stareInitiala = state_index[stareInitiala];
    set<int> new_stariFinale;
    vector<map<char, int>> new_tranzitii(new_nrStari);

    for (int new_stare = 1; new_stare < new_nrStari; ++new_stare) {
        int stare = *grupe[new_stare - 1].begin();     //aleg prima stare din grupa
        // toate starile din grupa sunt echvalente cu prima
        if (stariFinale.find(stare) != stariFinale.end()) 
            new_stariFinale.insert(new_stare);

        for (auto map : tranzitii[stare]) 
            if (map.second) 
                if (eStareAccesibila(map.second))
                    new_tranzitii[new_stare][map.first] = state_index[map.second];
    }

    return DFA(new_tranzitii, new_stareInitiala, new_stariFinale);
}