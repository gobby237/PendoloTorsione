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
double calcolaStdev (vector<double> &); 
int main ()
{
    vector<double> t, a, max, t_max, tf, A_ris, Tf_ris, wf_ris, y_fit, x_fit, sigma_tf, A_min_ris, min, t_min, pp, max_a; 

    // INSERISCI FILE IN INPUT 

    // vector<string> nome = {"0_96Hz.txt"}; 


    /*
        PARAMETRI FIT CURVA RISONANZA 


    */
    
    
    vector<string> nome = {
        "0_9574.txt",
        "0_93269.txt",
        "0_94059.txt",
        "0_94384.txt",
        "0_94697.txt",
        "0_95012.txt",
        "0_95223.txt",
        "0_95299.txt",
        "0_95374.txt",
        "0_95450.txt",
        "0_95526.txt",
        "0_95618.txt",
        "0_95694.txt",
       // "0_95816.txt",
        "0_95893.txt",
        "0_96046.txt",
        "0_96216.txt",
        "0_96525.txt",
        "0_97009.txt",
        "0_97482.txt",
        "0_98280.txt",
       // "0_92308.txt",
        "0_93897.txt",
        "0_94221.txt",
        "0_94533.txt",
        "0_94862.txt",
        "0_95178.txt",
        "0_95253.txt",
        "0_95329.txt",
        "0_95420.txt",
        // "0_95496.txt",
        "0_95572.txt",
        "0_95648.txt",
        "0_95770.txt",
        "0_95847.txt",
        "0_95969.txt",
        "0_96123.txt",
        // "0_96293.txt",
        "0_96759.txt",
        "0_97245.txt",
        "0_97720.txt",
        "0_99469.txt",
        // con anche i dati della prima giornata 
        // "0_96Hz.txt", 
         "0_95688Hz.txt", "0_9502Hz.txt", "0_9577Hz.txt" , "0_9756Hz.txt" , "0_93897Hz.txt" , "0_94384Hz.txt" , "0_94503Hz.txt" , "0_94847Hz.txt" , "0_95178Hz.txt", "0_95329Hz.txt" , "0_95405Hz.txt" , "0_95496Hz.txt" , "0_95557Hz.txt" , "0_95572Hz.txt" , "0_95618Hz.txt", "0_95648Hz.txt" , "0_95688Hz.txt" , "0_95724Hz.txt" , 
         //"0_95816Hz.txt" 
         "0_96108Hz.txt" , "0_96277Hz.txt", "0_96448Hz.txt", "0_96603Hz.txt" , "0_96759Hz.txt" , "0_96993Hz.txt" , "0_97087Hz.txt" 
         // "0_97403Hz.txt"
    };
    
    
    
    // vector<string> nome = {"0_96293.txt"}; 

    double val1, val2, val3, mediaTf, value ,mediaA, sommaTf, sommaA, T_presa, T_presa_dec, wf, resto, T_presa_int_dec, thr, Sigma_tf, Sigma_wf, Sigma_a_max, amp_max; 
    int T_presa_int = 0;
    int resto_int = 0;   
    bool Regione_fit = false;
    int startIdx = 0;
    
    // NEL FILE CORRETTO QUESTO COMMENTO VA TOLTO 
    ofstream out ("wf_plot.txt", std::ios::app); 

    // PROVA PER VEDERE I MAX 
    //ofstream out ("massimi.txt", std::ios::app);

    cout << "*******************************************" << endl; 
    cout << "*     ANALISI DATI PENDOLO A TORSIONE     *" << endl; 
    cout << "*              (fase a regime)            *" << endl;   
    cout << "*******************************************" << endl; 
    cout << "Nella fase a regime si calcolano: " << endl; 
    cout << "-Tf medio: periodo medio di oscillazione; " << endl; 
    cout << "-wf medio: velocità angolare di oscillazione media = 2pi/Tf; " << endl; 
    cout << "-A_max media: ampiezza media dei picchi nella presa; " << endl; 
    cout << "**************************************************************************************************************************************************************************" << endl;     
    cout << endl; 
    cout << "Inserire il valore di cui scalare thr: "; 
    cin >> value; 


    cout << "* NOME FILE          * TEMPO PRESA *   THR    * N. PICCHI * A_max MEDIO ± sigma_a_max * tf MEDIO ± sigma_tf  * wf MEDIO ± sigma_tf   * Offset     *  p-p/2 MEDIO ± sigma_pp  *" << endl;
    cout << "**************************************************************************************************************************************************************************" << endl;     
 
    for (int i = 0; i < nome.size(); i ++ )
    {

        amp_max = 0; 
        t.clear(); 
        a.clear(); 
        max.clear(); 
        t_max.clear();
        min.clear(); 
        tf.clear();
        pp.clear(); 
        x_fit.clear();
        y_fit.clear();
        t_min.clear(); 
        thr = 0; 
        Sigma_tf = 0; 
        Sigma_wf = 0; 
        Sigma_a_max = 0; 

        ifstream fin (nome.at(i));

        if (!fin)
        {
            cout << "File errato o assente!" << endl; 
        }


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
        thr = thr_max * value;

        if (nome.at(i) == "0_93269.txt")
            thr = 0.5 * thr_max; 
    

        // prendo l'ultimo tempo come tempo di presa, e poi converto in minuti e secondi 
        T_presa = t.back(); 
        T_presa_int = t.back(); // prenda la parte intera
        T_presa_dec = T_presa - T_presa_int;  
        
        

        // troviamo i massimi 
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

        // troviamo i minimi 
        inRegion = false; 
        for (int i = 0; i < (int)a.size(); i++) {
            if (!inRegion && i < (int)a.size() && a.at(i) < -thr) {
                inRegion = true;
                start = i;
            }
            else if (inRegion && (a.at(i) >= -thr)) {
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
                    // salvo all'interno di un vettore i minimi 
                    min.push_back(y_peak);
                    t_min.push_back(x_peak); 
                }
                inRegion = false;
            }
        }

        // stimo p-p/2
 
        // trovo prima se ci sono più massimi o minimi 
        if (max.size() >= min.size())
        {
            for (int i = 0; i < min.size(); i ++ )
            {
                pp.push_back((max.at(i) - min.at(i))/2); // metto all'interno del vettore l'iesima differenza tra max e min in valore assoluto 
            }
        }
        else
        {
            for (int i = 0; i < max.size(); i ++ )
            {
                pp.push_back((max.at(i) - min.at(i))/2); // metto all'interno del vettore l'iesima differenza tra max e min in valore assoluto 
            }
        }

        // trovo così la media e la stdev con l'altro metodo 


        
        // abbiamo trovato tutti i massimi ed i tempi  

        // utilizziamo sempre il numero maggiore di massimi per stimare il Tf 
        // Calcola i periodi solo se ci sono almeno 2 massimi
        if (t_max.size() >= 2) {
            for (size_t i = 0; i < t_max.size() - 1; i += 2) {
                tf.push_back(t_max.at(i+1) - t_max.at(i));
            }
        } else {
            cout << "Massimi insufficienti per calcolare il periodo. Salto il file." << endl;
            continue;
        }


        mediaA = 0;  
        sommaA = 0; 
        for (int i = 0; i < max.size(); i ++ )
        {
            sommaA += max.at(i); 
        }
        // calcolo l'ampiezza del picco media 
        mediaA = sommaA/max.size();
        

        mediaTf = 0; 
        sommaTf = 0; 
        for (int i = 0; i < tf.size(); i ++ )
        {
            sommaTf += tf.at(i); 
        }

        // calcolo Tf medio e wf medio 
        mediaTf = sommaTf/tf.size();
        wf = (2*M_PI)/mediaTf; 


        // calcolo la sigma di tf, a_max e wf  
        Sigma_tf = calcolaStdev(tf)/sqrt(tf.size());
        Sigma_wf = (Sigma_tf*(2*M_PI))/(pow(mediaTf,2));
        Sigma_a_max = calcolaStdev(max); 


        // aggiungo ai vettori risultati i diversi valori
        A_ris.push_back(mediaA); // ampiezza media 
        Tf_ris.push_back(mediaTf); // tf medio 
        wf_ris.push_back(wf); // wf medio 
        sigma_tf.push_back(Sigma_tf); // sigma di tf 


        //
        //  PROVA PER VEDERE DOVE SONO I PUNTI DI MASSIMO INDIVIDUATI (E MINIMI)
        // 
        //


        
        /*
        for (int i = 0; i < t_max.size(); i ++)
        {
            out << t_max.at(i) << " " << max.at(i) << "\n"; 
        }
        for (int i = 0; i < t_min.size(); i ++ )
        {
            out << t_min.at(i) << " " << min.at(i) << "\n"; 
        }
            */
        
        
        
        
        
       
     // Sostituisci la riga di output con:
         cout << "* " << left << setw(18) << nome.at(i) 
         << " * " << fixed << setprecision(2) << setw(11) << t.back()  // Tempo presa (senza formattazione fissa)
         << " * " << setw(8) << thr        // Thr (senza formattazione fissa)
         << " * " << setw(9) << tf.size()  // Numero picchi (intero, niente decimali)
         << " * " << fixed << setprecision(4) << setw(12) << mediaA*2*M_PI   // 4 decimali
         << "± " << fixed << setprecision(4) << setw(11) << Sigma_a_max*2*M_PI  // 4 decimali
         << " * " << fixed << setprecision(4) << setw(9) << mediaTf  // 4 decimali
         << "± " << fixed << setprecision(4) << setw(9) << Sigma_tf  // 4 decimali
         << " * " << fixed << setprecision(4) << setw(9) << wf      // 4 decimali
         << "± " << fixed << setprecision(4) << setw(10) << Sigma_wf  // 4 decimali
         << " * " << fixed << setprecision(4) << setw(10) << abs(*max_element(a.begin(), a.end()) + *min_element(a.begin(), a.end()))
         << " * " << fixed << setprecision(4) << setw(13) << accumulate(pp.begin(), pp.end(), 0.0)*2*M_PI/pp.size()
         << "± " << fixed << setprecision(4) << setw(9) << calcolaStdev(pp)*2*M_PI
         << " *" << endl;

         // riempio il vettore con l'ampiezza media massima di ogni file 
         max_a.push_back(accumulate(pp.begin(), pp.end(), 0.0)*2*M_PI/pp.size()); 
        

        
    }


    
    /*
    // mettiamo nel file per il grafico 
    for (int i = 0; i < A_ris.size(); i ++ )
    {
        out << wf_ris.at(i) << "    " << max_a.at(i) << "\n"; 
    }
      */  
    
    
    
    
    
    cout << "**************************************************************************************************************************************************************************" << endl;    
    
    cout << endl << "Programma dei Meccanici di precisione, B10 ©" << endl;

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
