#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>  // Per setw
#include <iomanip>  // Necessario per setprecision
#include <limits>   // Necessario per numeric_limits
using namespace std; 
double thr_cal(vector<double>); 
int main ()
{
    vector<double> t, a, max, t_max, tf, A_ris, Tf_ris, wf_ris, y_fit, x_fit, sigma_tf; 

    // INSERISCI FILE IN INPUT 
    vector<string> nome = {"0_96Hz.txt", "0_95688Hz.txt", "0_9502Hz.txt", "0_9577Hz.txt" , "0_9756Hz.txt" , "0_93897Hz.txt" , "0_94384Hz.txt" , "0_94503Hz.txt" , "0_94847Hz.txt" , "0_95178Hz.txt", "0_95329Hz.txt" , "0_95405Hz.txt" , "0_95496Hz.txt" , "0_95557Hz.txt" , "0_95572Hz.txt" , "0_95618Hz.txt", "0_95648Hz.txt" , "0_95688Hz.txt" , "0_95724Hz.txt" , "0_95816Hz.txt" , "0_96108Hz.txt" , "0_96277Hz.txt", "0_96448Hz.txt", "0_96603Hz.txt" , "0_96759Hz.txt" , "0_96993Hz.txt" , "0_97087Hz.txt" , "0_97403Hz.txt"}; 

    // vector<string> nome = {"0_96277Hz.txt"}; 

    double val1, val2, val3, mediaTf, mediaA, sommaTf, sommaA, T_presa, T_presa_dec, wf, resto, T_presa_int_dec, thr, Sigma_tf; 
    int T_presa_int = 0;
    int resto_int = 0;   
    bool Regione_fit = false;
    int startIdx = 0;
    
    // NEL FILE CORRETTO QUESTO COMMENTO VA TOLTO 
    ofstream out ("wf_plot.txt", std::ios::app); 

    // PROVA PER VEDERE I MAX 
    // ofstream out ("massimi.txt", std::ios::app);

    cout << "*******************************************" << endl; 
    cout << "*     ANALISI DATI PENDOLO A TORSIONE     *" << endl; 
    cout << "*              (fase a regime)            *" << endl;   
    cout << "*******************************************" << endl; 
    cout << "Nella fase a regime si calcolano: " << endl; 
    cout << "-Tf medio: periodo medio di oscillazione; " << endl; 
    cout << "-wf medio: velocità angolare di oscillazione media = 2pi/Tf; " << endl; 
    cout << "-A_max media: ampiezza media dei picchi nella presa; " << endl; 
    cout << "*********************************************************************************************************" << endl;     
    cout << endl; 


    cout << "* NOME FILE          * TEMPO PRESA *   THR    * N. PICCHI * A_max MEDIO * tf MEDIO ± sigma_tf  * wf MEDIO ± sigma_tf         *" << endl;
    cout << "*********************************************************************************************************" << endl;     
 
    for (int i = 0; i < nome.size(); i ++ )
    {
        t.clear(); 
        a.clear(); 
        max.clear(); 
        t_max.clear();
        tf.clear();
        x_fit.clear();
        y_fit.clear();
        thr = 0; 

        ifstream fin (nome.at(i));

        if (!fin)
        {
            cout << "File errato o assente!" << endl; 
        }

        // cout << "*******************************************" << endl << endl; 
        // cout << "NOME FILE: " << nome.at(i) << endl; 


        while (fin>>val1>>val2>>val3)
        {
            t.push_back(val1); 
            a.push_back(val3); 

        }
        
       if (a.empty() || t.empty()) {
        cout << "Errore: nessun dato letto dal file " << nome.at(i) << ". Salto questo file." << endl;
        continue;
        }

        
        double thr_max = *max_element(a.begin(), a.end());
        thr = thr_max * 0.60;
    

        // prendo l'ultimo tempo come tempo di presa, e poi converto in minuti e secondi 
        T_presa = t.back(); 
        T_presa_int = t.back(); // prenda la parte intera
        T_presa_dec = T_presa - T_presa_int;  
        
        // cout << "TEMPO PRESA: " << T_presa_int/60 << "min   " << T_presa_int % 60 << "secondi   "  << T_presa_dec << "millesimi   " << endl; 
        // cout << "IL THR (0.60*max): " << thr << endl; 
        

        // abbiamo i vettori con i tempi e le ampiezze 

        // Rilevo regioni sopra soglia e faccio fit quadratico
        
        bool inRegion = false;
        int start = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            if (!inRegion && i < (int)a.size() && a.at(i) > thr) {
                inRegion = true;
                start = i;
            }
            else if (inRegion && (a.at(i) <= thr)) {
                int end = i - 1;
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
                inRegion = false;
            }
        }

        
        // abbiamo trovato tutti i massimi ed i tempi  

        // utilizziamo sempre il numero maggiore di massimi per stimare il Tf 
        // Calcola i periodi solo se ci sono almeno 2 massimi
        if (t_max.size() >= 2) {
            for (size_t i = 0; i < t_max.size() - 1; i += 2) {
                tf.push_back(t_max[i+1] - t_max[i]);
            }
        } else {
            cout << "Massimi insufficienti per calcolare il periodo. Salto il file." << endl;
            continue;
        }

        /*
        cout << "NUMERO PICCHI (utilizzati per calcolare wf medio): " << tf.size(); 
        if (tf.size()>=20)
        {
            cout << " --> sono sufficenti!" << endl; 
        }
        else
        {
            cout << " --> non sono sufficenti! " << endl; 
        }
        */


        mediaA = 0;  
        sommaA = 0; 
        for (int i = 0; i < max.size(); i ++ )
        {
            sommaA += max.at(i); 
        }

        mediaA = sommaA/max.size();
        
        // metto all'interno di un vettore tutte le ampiezze massime

        mediaTf = 0; 
        sommaTf = 0; 
        for (int i = 0; i < tf.size(); i ++ )
        {
            sommaTf += tf.at(i); 
        }


        mediaTf = sommaTf/tf.size();
        sommaTf = 0; 
        for (int i = 0; i < tf.size(); i ++ )
        {
            sommaTf = sommaTf + pow((tf.at(i) - mediaTf),2); 
        }
        Sigma_tf = sqrt(sommaTf/(tf.size()-1))/sqrt(tf.size()); 

        wf = (2*M_PI)/mediaTf; 

        // aggiungo ai vettori risultati i diversi valori
        A_ris.push_back(mediaA); // ampiezza media 
        Tf_ris.push_back(mediaTf); // tf medio 
        wf_ris.push_back(wf); // wf medio 
        sigma_tf.push_back(Sigma_tf); // sigma di tf 

        /*
        cout << "A_max MEDIO: " << mediaA << endl; 
        cout << "Tf MEDIO: " << mediaTf << endl; 
        cout << "wf MEDIO: " << wf << endl; 
        cout << endl; 
        cout << "*******************************************" << endl; 
        */

        //
        //
        //  PROVA PER VEDERE DOVE SONO I PUNTI DI MASSIMO INDIVIDUATI 
        // 
        //


        
        /*
        for (int i = 0; i < t_max.size(); i ++)
        {
            out << t_max.at(i) << " " << max.at(i) << "\n"; 

        }
        */
       
     // Sostituisci la riga di output con:
         cout << "* " << left << setw(18) << nome.at(i) 
         << " * " << fixed << setprecision(2) << setw(11) << t.back()  // Tempo presa (senza formattazione fissa)
         << " * " << setw(8) << thr        // Thr (senza formattazione fissa)
         << " * " << setw(9) << tf.size()  // Numero picchi (intero, niente decimali)
         << " * " << fixed << setprecision(4) << setw(11) << mediaA  // 4 decimali
         << " * " << fixed << setprecision(4) << setw(9) << mediaTf  // 4 decimali
         << "± " << fixed << setprecision(4) << setw(9) << Sigma_tf  // 4 decimali
         << " * " << fixed << setprecision(4) << setw(15) << wf      // 4 decimali
         << " *" << endl;
        
        
        
    }

    
    
    // mettiamo nel file per il grafico 
    for (int i = 0; i < A_ris.size(); i ++ )
    {
        // out << wf_ris.at(i) << "    " << A_ris.at(i) << "\n"; 
    }
    
    cout << "*********************************************************************************************************" << endl;    
    
    cout << endl << "Programma dei Meccanici di precisione, B10 ©" << endl;

    return 0;
}

