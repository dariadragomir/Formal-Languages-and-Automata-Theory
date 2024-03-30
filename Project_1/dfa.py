f = "input.txt"
g = "output.txt"

def citire():
    with open(f, 'r') as file:
        nr_stari = int(file.readline().strip())
        stari = []
        for stare in file.readline().strip().split():
            stari.append(int(stare))
        nr_tranzitii = int(file.readline().strip())
        tranzitii = {}
        for _ in range(nr_tranzitii):
            tranzitie = file.readline().strip().split()
            x, y, l = int(tranzitie[0]), int(tranzitie[1]), (tranzitie[2])
            tranzitii[(x, l)] = y
        stare_initiala = int(file.readline().strip())
        nrF = int(file.readline().strip())
        stari_finale = set(int(file.readline().strip()) for _ in range(nrF))
        nr_cuvinte = int(file.readline().strip())
        cuvinte = []
        for _ in range(nr_cuvinte):
            cuvant = file.readline().strip()
            cuvinte.append(cuvant)
    return nr_stari, stari, tranzitii, stare_initiala, stari_finale, nr_cuvinte, cuvinte

def verificare(dfa, cuvant):
    stare_curenta = dfa['st_initiala']
    drum = [stare_curenta]
    for litera in cuvant:
        if (stare_curenta, litera) in dfa['tranz']:
            stare_curenta = dfa['tranz'][(stare_curenta, litera)]
            drum.append(stare_curenta)
        else:
            return False, []
    if stare_curenta in dfa['stari']:
        return True, drum   
    return False, []

nr_stari, stari, tranzitii, stare_initiala, stari_finale, nr_cuvinte, cuvinte = citire()
dfa = {'stari': stari, 'tranz': tranzitii, 'st_initiala': stare_initiala, 'st_finala': stari_finale}
with open(g, 'w') as file:
    for cuvant in cuvinte:
        rezultat, drum = verificare(dfa, cuvant)
        if rezultat:
            file.write("DA")
            for i in range(len(drum)):
                file.write(str(drum[i]) + ' ')
            file.write('\n')
        else:
            file.write("NU")