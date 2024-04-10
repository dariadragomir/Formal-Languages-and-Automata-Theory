#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <stack>
using namespace std;

const char LAMBDA = '.';

class LNFA {
private:
    int nr_stari;
    vector<int> stari;
    int nr_cuvinte;
    vector<string> cuvinte;
    int stare_initiala;
    set<int> stari_finale;
    int nr_tranzitii;
    map<pair<int, char>, vector<int>> tranzitii;

public:
    LNFA(const string& input_file) {
        ifstream file(input_file);

        file >> nr_stari;
        stari.resize(nr_stari);
        for (int i = 0; i < nr_stari; ++i) 
            file >> stari[i];

        file >> nr_tranzitii;
        for (int i = 0; i < nr_tranzitii; ++i) {
            int x, y;
            char l;
            file >> x >> y >> l;
            if (l == LAMBDA) 
                l = '\0'; 
            tranzitii[{x, l}].push_back(y);
        }

        file >> stare_initiala;
        int nrF;
        file >> nrF;
        for (int i = 0; i < nrF; ++i) {
            int f;
            file >> f;
            stari_finale.insert(f);
        }

        file >> nr_cuvinte;
        cuvinte.resize(nr_cuvinte);
        for (int i = 0; i < nr_cuvinte; ++i) 
            file >> cuvinte[i];

        file.close();
    }

    set<int> lambda_inchideri(int stare) {
        set<int> inchidere;
        stack<int> de_vizitat;
        de_vizitat.push(stare);

        while (!de_vizitat.empty()) {
            int s = de_vizitat.top();
            de_vizitat.pop();
            inchidere.insert(s);

            for (int st : tranzitii[{s, '\0'}]) 
                if (inchidere.find(st) == inchidere.end()) 
                    de_vizitat.push(st);
        }
        return inchidere;
    }

    bool verificare(const string& cuvant) {
        set<int> inchidere_initiala = lambda_inchideri(stare_initiala);
        set<int> stari_curente = inchidere_initiala;

        for (char litera : cuvant) {
            set<int> stari_urmatoare;
            for (int stare : stari_curente) {
                for (int st : tranzitii[{stare, litera}]) {
                    set<int> inchidere = lambda_inchideri(st);
                    stari_urmatoare.insert(inchidere.begin(), inchidere.end());
                }
            }
            stari_curente = stari_urmatoare;
        }

        for (int stare : stari_curente) 
            if (stari_finale.find(stare) != stari_finale.end()) 
                return true;
        return false;
    }

    void afisare() {
        for (const string& cuvant : cuvinte) {
            bool rezultat = verificare(cuvant);
            if (rezultat) 
                cout << "DA" << endl;
            else 
                cout << "NU" << endl;
        }
    }
};

int main() {
    string input_file = "input.txt";

    LNFA lnfa(input_file);
    lnfa.afisare();
    return 0;
}
