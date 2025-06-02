#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>  // Per setw
#include <iomanip>  // Necessario per setprecision
#include <limits>   // Necessario per numeric_limits
#include <numeric>
using namespace std;
double calcolaStdev(vector<double> &); 
int main ()
{
    vector<double> a, t, t_max, t_min, max, min, Ts; 
    double val1, val2, val3, thr, max_rel, ts_medio, sigma_ts, ws, sigma_ws; 
    string nome = ""; 

    ifstream fin ("smorzamento_0_95618.txt");

    // ofstream out ("check_max.txt", std::ios::app); 
    

    while (fin>>val1>>val2>>val3)
    {
        // prendo i dati dell'ampiezza e del tempo solo quando il motore è spento 
        if (val2==0)
        {
            a.push_back(val3); 
            t.push_back(val1); 
        }
    }

    /*
    
    int j = 0; 
    int start = 0, end = 0; 
    for (int i = 2; i < (int)a.size() - 2; i++) 
    {
        thr = 0; 
        start = 0; 
        end = 0; 

        if ( a.at(i-1) <= a.at(i) && a.at(i-2) <= a.at(i) && a.at(i + 1) <= a.at(i) && a.at(i+2) <= a.at(i))
        {
            thr = 0.6*a.at(i); // definiamo il threshold 
            
            
            start = i;
            while (start > 0 && a.at(start) > thr) {
                --start;
            }
            

            if (a.at(start) <= thr) ++start;

            end = i;
            while (end + 1 < a.size() && a.at(end) > thr) {
                ++end;
            }
            

            int m = end - start + 1;
            if (m >= 3) {
                // somma per minimi quadrati
                double S0 = m, S1=0,S2=0,S3=0,S4=0;
                double Sy=0,Sxy=0,Sx2y=0;
                for (int j = start; j <= end; j++) {
                    double x = t.at(j);
                    double y = a.at(j);
                    double x2 = x*x;
                    S1   += x;
                    S2   += x2;
                    S3   += x2 * x;
                    S4   += x2 * x2;
                    Sy   += y;
                    Sxy  += x * y;
                    Sx2y += x2 * y;
                }
                // sistema normale 3x3 aumentato
                double M[3][4] = {
                    {S4,  S3,  S2,   Sx2y},
                    {S3,  S2,  S1,   Sxy },
                    {S2,  S1,  S0,   Sy  }
                };
                // gauss
                for (int r = 0; r < 3; r++) {
                    double piv = M[r][r];
                    if (fabs(piv) < 1e-12) break;
                    for (int c = r; c < 4; ++c) M[r][c] /= piv;
                    for (int rr = r+1; rr < 3; ++rr) {
                        double f = M[rr][r];
                        for (int c = r; c < 4; ++c)
                            M[rr][c] -= f * M[r][c];
                    }
                }
                // risolvo a ritroso
                double c = M[2][3];
                double b = M[1][3] - M[1][2]*c;
                double a_coef = M[0][3] - M[0][1]*b - M[0][2]*c;
                // vertice parabola
                double x_peak = -b/(2*a_coef);
                double y_peak = a_coef*x_peak*x_peak + b*x_peak + c;
                t_max.push_back(x_peak);
                max.push_back(y_peak);
            }
            
        }

        
    

    }

    // elimino l'ultimo elemento nel vettore, perché potrebbe essere sbagliato 
    t_max.pop_back(); 
    max.pop_back(); 

    for (int i = 0; i < t_max.size(); i ++)
    {
        out << t_max.at(i) << " " << max.at(i) << "\n"; 
    }

    t_max.clear(); 
    max.clear();

    */

    ifstream fin1 ("check_max.txt"); 

    while (fin1>>val1>>val2)
    {
        t_max.push_back(val1); 
        max.push_back(val2);
    }

    // stimiamo ts 

    for (int i = 0; i < t_max.size() - 2; i ++ )
    {
        Ts.push_back(t_max.at(i+1) - t_max.at(i));
        i++; 
    }

    // media e stdev di Ts 
    ts_medio = accumulate(Ts.begin(), Ts.end(), 0.0)/Ts.size();
    sigma_ts = calcolaStdev(Ts); 

    // media e stdev di ws 
    ws = (2*M_PI)/ts_medio;
    sigma_ws = (sigma_ts*(2*M_PI))/(pow(ws,2));

    cout << "ANALISI DATI IN FASE DI SMORZAMENTO " << endl << endl; 
    cout << "NOME FILE: " << nome << endl; 
    cout << "stima Ts: " << ts_medio << " pm " << sigma_ts << endl; 
    cout << "stima ws: " << ws << " pm " << sigma_ws << endl; 
    


    return 0; 
}
double calcolaStdev(vector<double>& dati) {
    if (dati.empty()) return 0.0;

    double mean = accumulate(dati.begin(), dati.end(), 0.0) / dati.size();
    // media

    double sum_sq = 0.0;
    for (double x : dati) {
        sum_sq += (x - mean) * (x - mean);
    }
    
    return sqrt(sum_sq / (dati.size() - 1)); 
}