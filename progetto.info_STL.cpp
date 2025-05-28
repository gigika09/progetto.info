#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <limits>

using namespace std;

// Struct
struct Studente {
    int matricola;
    string cognome;
    string nome;
};

struct Materia {
    string codice;
    string descrizione;
};

struct Corso {
    string codice;
    string descrizione;
};

struct RigaCSV {
    Corso corso;
    Materia materia;
    Studente studente;
};

// Prototipi
void mostraMenu();
void caricaDatiDaCSV(const string&, vector<RigaCSV>&);
void salvaDatiCSV(const string&, const vector<RigaCSV>&);
void ricercaStudentePerMatricola(const vector<RigaCSV>&);
void ricercaStudentePerCognome(const vector<RigaCSV>&);
void studentiPerCorso(const vector<RigaCSV>&);
void stampaDatiCorso(const vector<RigaCSV>&);
void contaStudentiPerCorso(const vector<RigaCSV>&);
void contaMateriePerCorso(const vector<RigaCSV>&);
void ricercaMaterieTestuale(const vector<RigaCSV>&);
void inserisciNuovoStudente(vector<RigaCSV>&);
string toLower(const string&);
bool isNumber(const string&);
void clearInputBuffer();
int getIntInput(const string& prompt);

// Dichiarazione file
const string filename = "corsi_studenti.csv";



int main() {
    vector<RigaCSV> dati;
    caricaDatiDaCSV(filename, dati);

    int scelta;
    do {
        system("cls");
        mostraMenu();
        scelta = getIntInput("Scelta: ");

        system("cls");

        switch (scelta) {
            case 1: ricercaStudentePerMatricola(dati); break;
            case 2: ricercaStudentePerCognome(dati); break;
            case 3: studentiPerCorso(dati); break;
            case 4: stampaDatiCorso(dati); break;
            case 5: contaStudentiPerCorso(dati); break;
            case 6: contaMateriePerCorso(dati); break;
            case 7: ricercaMaterieTestuale(dati); break;
            case 8: inserisciNuovoStudente(dati); break;
            case 9: salvaDatiCSV(filename, dati); cout << "Dati salvati. Uscita.\n"; break;
            default: cout << "Scelta non valida.\n"; break;
        }

        if (scelta != 9) {
            cout << "\nPremi INVIO per continuare...";
            clearInputBuffer();
        }

    } while (scelta != 9);

    return 0;
}

// Funzioni
string toLower(const string& s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

bool isNumber(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        cin >> input;
        if (isNumber(input)) {
            clearInputBuffer();
            return stoi(input);
        }
        cout << "Input non valido. Inserisci un numero.\n";
        clearInputBuffer();
    }
}

void mostraMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Ricerca corsi per matricola\n";
    cout << "2. Ricerca corsi per cognome\n";
    cout << "3. Elenco studenti per corso\n";
    cout << "4. Dati del corso\n";
    cout << "5. Numero studenti per corso\n";
    cout << "6. Numero materie per corso\n";
    cout << "7. Ricerca testuale materie\n";
    cout << "8. Inserisci nuovo studente\n";
    cout << "9. Salva ed esci\n";
}

void caricaDatiDaCSV(const string& filename, vector<RigaCSV>& dati) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire il file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string campo;
        RigaCSV r;

        getline(ss, r.corso.codice, ',');
        getline(ss, r.corso.descrizione, ',');
        getline(ss, r.materia.codice, ',');
        getline(ss, r.materia.descrizione, ',');
        getline(ss, campo, ',');

        if (!isNumber(campo)) {
            cerr << "Errore: campo matricola non valido nella riga: " << line << endl;
            continue;
        }
        r.studente.matricola = stoi(campo);
        getline(ss, r.studente.cognome, ',');
        getline(ss, r.studente.nome);

        dati.push_back(r);
    }
    file.close();
}

void salvaDatiCSV(const string& filename, const vector<RigaCSV>& dati) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire il file per il salvataggio" << endl;
        return;
    }

    for (const auto& r : dati) {
        file << r.corso.codice << ',' << r.corso.descrizione << ','
             << r.materia.codice << ',' << r.materia.descrizione << ','
             << r.studente.matricola << ',' << r.studente.cognome << ',' << r.studente.nome << '\n';
    }
    file.close();
}

void ricercaStudentePerMatricola(const vector<RigaCSV>& dati) {
    int matricola = getIntInput("Inserisci matricola: ");
    bool trovato = false;

    for (const auto& r : dati) {
        if (r.studente.matricola == matricola) {
            cout << r.corso.codice << " - " << r.corso.descrizione << " | Materia: "
                 << r.materia.codice << " - " << r.materia.descrizione << endl;
            trovato = true;
        }
    }

    if (!trovato) {
        cout << "Nessun corso trovato per la matricola inserita.\n";
    }
}

void ricercaStudentePerCognome(const vector<RigaCSV>& dati) {
    string cognome;
    cout << "Inserisci cognome: ";
    getline(cin, cognome);
    bool trovato = false;

    for (const auto& r : dati) {
        if (toLower(r.studente.cognome) == toLower(cognome)) {
            cout << r.studente.matricola << " " << r.studente.nome << " " << r.studente.cognome
                 << " - Corso: " << r.corso.codice << " - " << r.corso.descrizione
                 << " | Materia: " << r.materia.codice << " - " << r.materia.descrizione << endl;
            trovato = true;
        }
    }

    if (!trovato) {
        cout << "Nessuno studente trovato con questo cognome.\n";
    }
}

void studentiPerCorso(const vector<RigaCSV>& dati) {
    string codiceInput;
    bool trovato = false;
    string codiceOriginale; // Per mantenere il case originale

    do {
        cout << "Inserisci codice corso (es. C001 o c001): ";
        getline(cin, codiceInput);

        set<int> studentiVisti;
        trovato = false;
        string codiceLower = toLower(codiceInput);

        // Cerca il corso e mantieni il case originale
        for (const auto& r : dati) {
            if (toLower(r.corso.codice) == codiceLower) {
                if (!trovato) {
                    codiceOriginale = r.corso.codice; // Memorizza il case originale
                    cout << "\nStudenti del corso " << codiceOriginale << ":\n";
                }

                if (studentiVisti.insert(r.studente.matricola).second) {
                    cout << " - " << r.studente.matricola << ": "
                         << r.studente.cognome << " " << r.studente.nome << endl;
                }
                trovato = true;
            }
        }

        if (!trovato) {
            cout << "Codice corso non trovato. Riprova.\n";
        } else {
            cout << "Totale studenti: " << studentiVisti.size() << endl;
        }
    } while (!trovato);
}

void stampaDatiCorso(const vector<RigaCSV>& dati) {
    string codiceInput;
    bool trovato = false;
    do {
        cout << "Inserisci codice corso: ";
        getline(cin, codiceInput);

        map<string, vector<pair<int, string>>> materiaStudenti; // Materia -> lista studenti
        string codiceLower = toLower(codiceInput);
        trovato = false;

        // Raccogli tutti i dati
        for (const auto& r : dati) {
            if (toLower(r.corso.codice) == codiceLower) {
                trovato = true;
                string key = r.materia.codice + " - " + r.materia.descrizione;
                string studente = to_string(r.studente.matricola) + " " +
                                 r.studente.cognome + " " + r.studente.nome;
                materiaStudenti[key].emplace_back(r.studente.matricola, studente);
            }
        }

        if (trovato) {
            cout << "\nDettagli corso " << codiceInput << ":\n";
            for (const auto& [materia, studenti] : materiaStudenti) {
                cout << "\nMateria: " << materia << "\n";
                cout << "Studenti (" << studenti.size() << "):\n";
                for (const auto& [matricola, nome] : studenti) {
                    cout << " - " << nome << endl;
                }
            }
        } else {
            cout << "Codice corso non trovato. Riprova.\n";
        }
    } while (!trovato);
}

void contaStudentiPerCorso(const vector<RigaCSV>& dati) {
    map<string, set<int>> corsoStudenti;
    for (const auto& r : dati) {
        corsoStudenti[r.corso.codice].insert(r.studente.matricola);
    }
    for (const auto& p : corsoStudenti) {
        cout << "Corso " << p.first << ": " << p.second.size() << " studenti\n";
    }
}

void contaMateriePerCorso(const vector<RigaCSV>& dati) {
    map<string, set<string>> corsoMaterie;
    for (const auto& r : dati) {
        corsoMaterie[r.corso.codice].insert(r.materia.codice);
    }
    for (const auto& p : corsoMaterie) {
        cout << "Corso " << p.first << ": " << p.second.size() << " materie\n";
    }
}

void ricercaMaterieTestuale(const vector<RigaCSV>& dati) {
    string s;
    cout << "Inserisci materia da cercare (nel codice o descrizione delle materie): ";
    getline(cin, s);

    if (s.empty()) {
        cout << "Nessun termine di ricerca inserito.\n";
        return;
    }

    s = toLower(s);
    map<string, pair<string, set<pair<int, string>>>> risultati; // Mappa: codice materia -> (descrizione, set di studenti)

    for (const auto& r : dati) {
        string codiceLower = toLower(r.materia.codice);
        string descLower = toLower(r.materia.descrizione);

        if (codiceLower.find(s) != string::npos || descLower.find(s) != string::npos) {
            // Aggiungi la materia se non è già presente
            if (risultati.find(r.materia.codice) == risultati.end()) {
                risultati[r.materia.codice].first = r.materia.descrizione;
            }
            // Aggiungi lo studente (matricola e nome completo)
            string studente = r.studente.cognome + " " + r.studente.nome + " (" + to_string(r.studente.matricola) + ")";
            risultati[r.materia.codice].second.insert({r.studente.matricola, studente});
        }
    }

    if (risultati.empty()) {
        cout << "Nessuna materia trovata contenente '" << s << "'\n";
        return;
    }

    // Stampa i risultati
    for (const auto& [codice, info] : risultati) {
        const auto& [descrizione, studenti] = info;
        cout << "\nMateria: " << codice << " - " << descrizione << "\n";
        cout << "Studenti iscritti (" << studenti.size() << "):\n";

        for (const auto& [matricola, nome] : studenti) {
            cout << "  " << nome << "\n";
        }
    }
    cout << endl;
}
void inserisciNuovoStudente(vector<RigaCSV>& dati) {
    int matricola = getIntInput("Matricola: ");

    string nome, cognome;
    cout << "Cognome: ";
    getline(cin, cognome);
    cout << "Nome: ";
    getline(cin, nome);

    string codice_materia;
    bool materiaTrovata = false;

    do {
        cout << "Codice materia: ";
        cin >> codice_materia;
        clearInputBuffer();

        for (const auto& r : dati) {
            if (toLower(r.materia.codice) == toLower(codice_materia)) {
                RigaCSV nuova = r;
                nuova.studente = {matricola, cognome, nome};
                dati.push_back(nuova);
                cout << "Studente inserito nel corso " << r.corso.codice << ".\n";
                materiaTrovata = true;
                break;
            }
        }

        if (!materiaTrovata) {
            cout << "Materia non trovata. Riprova.\n";
        }

    } while (!materiaTrovata);
}
