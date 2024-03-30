f = "input.txt"
g = "output.txt"
LAMBDA = '.'

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
            x, y, l = int(tranzitie[0]), int(tranzitie[1]), tranzitie[2]
            if (x, l) in tranzitii:
                tranzitii[(x, l)].append(y)
            else:
                tranzitii[(x, l)] = [y]
        stare_initiala = int(file.readline().strip())
        nrF = int(file.readline().strip())
        stari_finale = set(int(file.readline().strip()) for _ in range(nrF))
        nr_cuvinte = int(file.readline().strip())
        cuvinte = []
        for _ in range(nr_cuvinte):
            cuvant = file.readline().strip()
            cuvinte.append(cuvant)
    return nr_stari, stari, tranzitii, stare_initiala, stari_finale, nr_cuvinte, cuvinte

def lambda_inchideri(stare, tranzitii):
    inchidere = set()
    de_vizitat = [stare]
    while de_vizitat:
        s = de_vizitat.pop()
        inchidere.add(s)
        if (s, LAMBDA) in tranzitii:
            for st in tranzitii[(s, LAMBDA)]:
                if st not in inchidere:
                    de_vizitat.append(st)
    return inchidere

def verificare(nfa, cuvant):
    inchidere_initiala = lambda_inchideri(nfa['st_initiala'], nfa['tranz'])
    stari_curente = inchidere_initiala
    drum = [inchidere_initiala]
    for litera in cuvant:
        stari_urmatoare = set()
        for stare in stari_curente:
            if (stare, litera) in nfa['tranz']:
                for st in nfa['tranz'][(stare, litera)]:
                    stari_urmatoare.update(lambda_inchideri(st, nfa['tranz']))
        stari_curente = stari_urmatoare
        drum.append(stari_urmatoare)
    for stare in stari_curente:
        if stare in nfa['st_finala']:
            return True, drum
    return False, []

nr_stari, stari, tranzitii, stare_initiala, stari_finale, nr_cuvinte, cuvinte = citire()
nfa = {'stari': stari, 'tranz': tranzitii, 'st_initiala': stare_initiala, 'st_finala': stari_finale}
with open(g, 'w') as file:
    for cuvant in cuvinte:
        rezultat, drum = verificare(nfa, cuvant)
        if rezultat:
            file.write("DA")
            for i in range(len(drum)):
                file.write(" ")
                for stare in drum[i]:
                    file.write(str(stare) + " ")
            file.write('\n')
        else:
            file.write("NU\n")
