#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

char *poc = const_cast<char *>("600000001");

char *crt = const_cast<char *>("\n----------------------------------------------------\n");

struct Datum {
    int *_dan, *_mjesec, *_godina;

    void Unos(int d, int m, int g) {
        _dan = new int(d);
        _mjesec = new int(m);
        _godina = new int(g);
    }

    void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }

    void Dealociraj() {
        delete _dan;
        delete _mjesec;
        delete _godina;
        _dan = _mjesec = _godina = nullptr;
    }
};

struct Artikal {
    char *_naziv;
    float _cijena;

    void Unos(const char *naziv, float cijena) {
        int vel = static_cast<int>(strlen(naziv) + 1);
        _naziv = new char[vel];
        strcpy(_naziv, naziv);
        _cijena = cijena;
    }

    void Dealociraj() {
        delete[]_naziv;
        _naziv = nullptr;
    }

    void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};

struct Racun {
//format broja racuna: 006005491
//osigurati automatsko generisanje broja racuna pocevsi od 000000001
    char _brojRacuna[10];
    Datum _datumKreiranja;
    Artikal *_artikli=nullptr;
    int *_kolicine=nullptr;//cuva informaciju o kolicini svakog artikla na racunu
    int _brojArtikala=0;
//Potpisi funkcija trebaju odgovarati onima u main-u
//1. Unos
//2. Dealociraj
/*3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom.
    Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
            korisnik vise puta pokusa dodati isti artikal.
4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude
    identican, ali je svakako pozeljno). Prilikom svakog ispisa, racun
            je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca
    pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
    */
    void Unos(const char *brRac, Datum datum) {
        strcpy(_brojRacuna, poc);
        _datumKreiranja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
    }

    void DodajArtikal(Artikal artikal, int kol) {
        if (_artikli == nullptr) {
            _artikli = new Artikal[_brojArtikala+1];
            _artikli[0].Unos(artikal._naziv, artikal._cijena);
            _kolicine = new int[_brojArtikala+1];
            _kolicine[0] = kol;
            _brojArtikala++;
            return;
        }

        for (int k = 0; k < _brojArtikala; ++k) {
            if(strstr(_artikli[k]._naziv,artikal._naziv)!=nullptr){
                _kolicine[k]+=kol;
                return;
            }
        }

        auto *temp=new Artikal[_brojArtikala];
        auto *tempKol=new int[_brojArtikala];

        for (int i = 0; i < _brojArtikala; ++i) {
            temp[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
            tempKol[i]=_kolicine[i];
        }
        _artikli->Dealociraj();
        delete[] _artikli;
        delete[] _kolicine;

        _artikli=new Artikal[_brojArtikala+1];
        _kolicine=new int[_brojArtikala+1];

        for (int j = 0; j < _brojArtikala; ++j) {
            _artikli[j].Unos(temp[j]._naziv, temp[j]._cijena);
            _kolicine[j]=tempKol[j];
        }

        _artikli[_brojArtikala].Unos(artikal._naziv, artikal._cijena);
        _kolicine[_brojArtikala]=kol;
        _brojArtikala++;
    }

    void Ispis() {
        cout<<crt;
        cout<<_brojRacuna<<endl;
        _datumKreiranja.Ispis();
        for (int i = 0; i < _brojArtikala; ++i) {
            _artikli[i].Ispis();
            cout<<" kol. "<<_kolicine[i]<<endl;
        }
        cout<<"Ukupno artikala: "<<_brojArtikala<<endl;
        cout<<crt;

    }

    Artikal *BinarnaPretraga(const char * naziv) {
        //ne razumijem binarnu pretragu po stringu pa cu impl regularnu

        for (int k = 0; k < _brojArtikala; ++k) {
            if(strstr(_artikli[k]._naziv,naziv)!=nullptr){
                return &_artikli[k];
            }
        }
        return nullptr;
    }

    void Dealociraj() {
        delete[] _kolicine;
        _artikli->Dealociraj();
    }
};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna

Artikal *PronadjiNajskupljiArtikal(Artikal *_artikli, int brojArtikala) {
    float najskuplji=0;
    for (int i = 0; i < brojArtikala; ++i) {
        if(_artikli[i]._cijena>najskuplji){
            najskuplji = _artikli[i]._cijena;
        }
    }

    for (int j = 0; j < brojArtikala; ++j) {
        if(_artikli[j]._cijena==najskuplji){
            return &_artikli[j];
        }
    }
    return nullptr;
}

const char *GenerisiSljedeciBrojRacuna() {
    long brRac = strtol(poc, nullptr, 10);
    ++brRac;

    poc = new char[strlen(to_string(brRac).c_str()) + 1];
    strcpy(poc, to_string(brRac).c_str());
    return poc;

}

int main() {
//za eventualne nejasnoce analizirati testni program i ispise na slici
    Datum danas;
    danas.Unos(13, 6, 2016);
    Artikal cigarete, cokolada, sok;
    cigarete.Unos("Cigarete", 2.23);
    cokolada.Unos("Cokolada", 1.23);
    sok.Unos("Cappy", 2.10);
    Racun racun;
    racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
    racun.DodajArtikal(cokolada, 5);
    racun.DodajArtikal(sok, 10);
    racun.DodajArtikal(cigarete, 2);
    racun.DodajArtikal(cigarete, 5);
    racun.Ispis();
    Artikal * p1 = racun.BinarnaPretraga("Cokolada");
    cout << "Binarnom pretragom pronadjen artikal -> ";
    p1->Ispis();
    racun.Ispis();
    Artikal * p2 = PronadjiNajskupljiArtikal(racun._artikli, racun._brojArtikala);//potrebni parametri*/
    cout << "Rekurzivno pronadjen najskuplji artikal -> ";
    p2->Ispis();
    cout << endl;
//dealocirati zauzetu memoriju
    racun.Dealociraj();
    return 0;
}

