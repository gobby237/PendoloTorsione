#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;
int main ()
{
    vector<double> a, t, t_max, t_min, max, min; 
    double val1, val2, val3, thr, max_rel; 
    string nome = ""; 

    cout << "Inserire nome file: "; 
    cin >> nome; 

    ifstream fin (nome);

    ofstream out ("check_max.txt", std::ios::app); 
    
    while (fin>>val1>>val2>>val3)
    {
        // prendo i dati dell'ampiezza e del tempo solo quando il motore è spento 
        if (val2==0)
        {
            a.push_back(val3); 
            t.push_back(val1); 
        }
    }


    
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


    return 0; 
}
