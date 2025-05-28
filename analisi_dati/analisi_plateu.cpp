#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <numeric>    
using namespace std;

// Funzione per raffinamento parabolico del tempo di picco
double refinePeakTime(const vector<double>& t, const vector<double>& a, int idx) {
    double A = a[idx];
    double thr = 0.75 * A;
    int n = t.size();
    // Trova gli estremi dell'intervallo sopra la soglia
    int left = idx, right = idx;
    while (left > 0 && a[left-1] >= thr) left--;
    while (right < n-1 && a[right+1] >= thr) right++;
    int m = right - left + 1;
    if (m < 3) {
        // Pochi punti: non eseguo fit
        return t[idx];
    }
    // Calcola somme per regressione quadratica
    double S0 = m, Sx=0, Sxx=0, Sxxx=0, Sxxxx=0, Sy=0, Sxy=0, Sxxy=0;
    for(int j=left; j<=right; ++j) {
        double x = t[j], y = a[j];
        Sx   += x;
        Sxx  += x*x;
        Sxxx += x*x*x;
        Sxxxx+= x*x*x*x;
        Sy   += y;
        Sxy  += x*y;
        Sxxy += x*x*y;
    }
    // Matrice aumentata 3x4 per il sistema normale
    double mat[3][4] = {
        { S0,  Sx,   Sxx,  Sy  },
        { Sx,  Sxx,  Sxxx, Sxy },
        { Sxx, Sxxx, Sxxxx,Sxxy}
    };
    // Eliminazione gaussiana
    for(int k=0; k<3; ++k) {
        double pivot = mat[k][k];
        if (fabs(pivot) < 1e-12) {
            return t[idx];
        }
        for(int j=k; j<4; ++j) mat[k][j] /= pivot;
        for(int i=k+1; i<3; ++i) {
            double f = mat[i][k];
            for(int j=k; j<4; ++j) {
                mat[i][j] -= f * mat[k][j];
            }
        }
    }
    // Soluzione a ritroso
    double p2 = mat[2][3];
    double p1 = mat[1][3] - mat[1][2]*p2;
    double t_refined = -p1 / (2.0 * p2);
    return t_refined;
}

int main ()
{
    vector<double> t, a, maxAmp, t_max, tf_raw;
    vector<int> idx_max;
    vector<double> Tf_ris, wf_ris, A_ris;

    vector<string> nome = {"0_96Hz.txt"};

    double val1, val2, val3;
    double T_presa, T_presa_dec;
    int T_presa_int;

    ofstream out ("massimi.txt", ios::app);
    cout << "*******************************************" << endl;
    cout << "*     ANALISI DATI PENDOLO A TORSIONE     *" << endl;
    cout << "*              (fase a regime)            *" << endl;
    cout << "*******************************************" << endl;
    cout << "Nella fase a regime si calcolano: " << endl;
    cout << "-Tf medio: periodo medio di oscillazione; " << endl;
    cout << "-wf medio: velocità angolare di oscillazione media = 2pi/Tf; " << endl;
    cout << "-A_max media: ampiezza media dei picchi nella presa; " << endl << endl;

    for (int fi = 0; fi < nome.size(); fi++)
    {
        t.clear(); 
        a.clear(); 
        maxAmp.clear(); 
        t_max.clear();
        idx_max.clear();
        tf_raw.clear();

        ifstream fin (nome[fi]);
        if (!fin) {
            cout << "File errato o assente!" << endl;
            continue;
        }

        cout << "*******************************************" << endl << endl;
        cout << "NOME FILE: " << nome[fi] << endl;

        while (fin >> val1 >> val2 >> val3) {
            t.push_back(val1);
            a.push_back(val3);
        }

        // tempo di presa
        T_presa = t.back();
        T_presa_int = static_cast<int>(T_presa);
        T_presa_dec = T_presa - T_presa_int;
        cout << "TEMPO PRESA: " 
             << (T_presa_int/60) << "min   " 
             << (T_presa_int % 60) << "secondi   " 
             << T_presa_dec << "millesimi" << endl << endl;

        // rilevazione massimi grezzi
        for (int i = 2; i < a.size() - 2; i++)
        {
            bool isMax = (a[i] > a[i-1] && a[i] > a[i-2] && a[i] > a[i+1] && a[i] > a[i+2])
                         || (a[i] > a[i-2] && a[i] > a[i-1] && a[i] == a[i+1] && a[i] == a[i+2])
                         || (a[i] > a[i-2] && a[i] > a[i-1] && a[i] > a[i+1] && a[i] == a[i+2])
                         || (a[i] > a[i-2] && a[i] > a[i-1] && a[i] == a[i+1] && a[i] > a[i+2]);
            if (isMax) {
                maxAmp.push_back(a[i]);
                t_max.push_back(t[i]);
                idx_max.push_back(i);
            }
        }

        // rifinisco tempi di massimo con fit parabolico
        vector<double> t_max_refined;
        for (int j = 0; j < idx_max.size(); ++j) {
            double tref = refinePeakTime(t, a, idx_max[j]);
            t_max_refined.push_back(tref);
        }

        // calcolo intervalli Tf e wf
        vector<double> Tf, wf;
        for (int j = 0; j + 2 < (int)t_max_refined.size(); j += 2) {
            double dt = t_max_refined[j+2] - t_max_refined[j];
            Tf.push_back(dt);
            wf.push_back(2.0 * M_PI / dt);
        }

        cout << "NUMERO PICCHI (utilizzati per calcolare wf medio): " << Tf.size();
        if (Tf.size() >= 20) cout << " --> sono sufficienti!" << endl;
        else               cout << " --> non sono sufficienti!" << endl;

        // medie
        double mediaA  = accumulate(maxAmp.begin(), maxAmp.end(), 0.0) / maxAmp.size();
        double mediaTf = accumulate(Tf.begin(), Tf.end(), 0.0)     / Tf.size();
        double mediaWf = accumulate(wf.begin(), wf.end(), 0.0)     / wf.size();

        // deviazioni standard
        double varTf = 0, varWf = 0;
        for (double v : Tf)  varTf += (v - mediaTf)*(v - mediaTf);
        for (double v : wf)  varWf += (v - mediaWf)*(v - mediaWf);
        double stdTf = sqrt(varTf / Tf.size());
        double stdWf = sqrt(varWf / wf.size());

        // memorizzo risultati globali
        A_ris.push_back(mediaA);
        Tf_ris.push_back(mediaTf);
        wf_ris.push_back(mediaWf);

        // output su console
        cout << "A_max MEDIO: "   << mediaA  << endl;
        cout << "Tf MEDIO: "      << mediaTf << endl;
        cout << "wf MEDIO: "      << mediaWf << endl;
        cout << "Tf DEV STD: "    << stdTf   << endl;
        cout << "wf DEV STD: "    << stdWf   << endl << endl;
        cout << "*******************************************" << endl;

        // salvo sui file i massimi grezzi
        for (int j = 0; j < t_max.size(); ++j) {
            out << t_max[j] << "    " << maxAmp[j] << "\n";
        }
    }

    // scrivo relazione wf vs A_max per grafico
    for (int i = 0; i < A_ris.size(); i++) {
        out << wf_ris[i] << "    " << A_ris[i] << "\n";
    }

    return 0;
}
