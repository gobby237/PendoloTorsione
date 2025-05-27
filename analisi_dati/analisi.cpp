#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std; 
string InputFile(); 
int main ()
{
    vector<double> t, a, max, t_max, tf, A_ris, Tf_ris, wf_ris; 

    // INSERISCI FILE IN INPUT 
    vector<string> nome = {"0_96Hz.txt", "0_95688Hz.txt", "0_9502Hz.txt", "0_9577Hz.txt" , "0_9756Hz.txt" , "0_93897Hz.txt" , "0_94384Hz.txt" , "0_94503Hz.txt" , "0_94847Hz.txt" , "0_95178Hz.txt", "0_95329Hz.txt" , "0_95405Hz.txt" , "0_95496Hz.txt" , "0_95557Hz.txt" , "0_95572Hz.txt" , "0_95618Hz.txt", "0_95648Hz.txt" , "0_95688Hz.txt" , "0_95724Hz.txt" , "0_95816Hz.txt" , "0_96108Hz.txt" , "0_96277Hz.txt", "0_96448Hz.txt", "0_96603Hz.txt" , "0_96759Hz.txt" , "0_96993Hz.txt" , "0_97087Hz.txt" , "0_97403Hz.txt"}; 
    
    double val1, val2, val3, mediaTf, mediaA, sommaTf, sommaA, T_presa, T_presa_dec, wf, resto, T_presa_int_dec; 
    int T_presa_int = 0;
    int resto_int = 0;  
    


    cout << "*******************************************" << endl; 
    cout << "*     ANALISI DATI PENDOLO A TORSIONE     *" << endl; 
    cout << "*              (fase a regime)            *" << endl;   
    cout << "*******************************************" << endl; 
    cout << "Nella fase a regime si calcolano: " << endl; 
    cout << "-Tf medio: periodo medio di oscillazione; " << endl; 
    cout << "-wf medio: velocità angolare di oscillazione media = 2pi/Tf; " << endl; 
    cout << "-A_max media: ampiezza media dei picchi nella presa; " << endl; 
    cout << endl; 

     
 
    for (int i = 0; i < nome.size(); i ++ )
    {
        t.clear(); 
        a.clear(); 
        max.clear(); 
        t_max.clear();
        tf.clear();

        ifstream fin (nome.at(i));

        if (!fin)
        {
            cout << "File errato o assente!" << endl; 
        }

        cout << "*******************************************" << endl << endl; 
        cout << "NOME FILE: " << nome.at(i) << endl; 


        while (fin>>val1>>val2>>val3)
        {
            t.push_back(val1); 
            a.push_back(val3); 
        }

        // prendo l'ultimo tempo come tempo di presa, e poi converto in minuti e secondi 
        T_presa = t.back(); 
        T_presa_int = t.back(); // prenda la parte intera
        T_presa_dec = T_presa - T_presa_int;  
        cout << "TEMPO PRESA: " << T_presa_int/60 << "min   " << T_presa_int % 60 << "secondi   "  << T_presa_dec << "millesimi   " << endl; 



        // abbiamo i vettori con i tempi e le ampiezze 

        for (int i = 2; i < a.size() - 2; i ++ )
        {
            if ((a.at(i) > a.at(i+1) ) && (a.at(i) > a.at(i-1)) && (a.at(i) > a.at(i-2)) && (a.at(i) > a.at(i+2) ))
            {
                max.push_back(a.at(i)); 
                t_max.push_back(t.at(i)); 
            }
        }
        // abbiamo trovato tutti i massimi ed i tempi  


        // utilizziamo sempre il numero maggiore di massimi per stimare il Tf 
        for (int i = 0; i < t_max.size() - 2; i++)
        {
            tf.push_back(t_max.at(i+1) - t_max.at(i)); 
            i++; 
        }

        cout << "NUMERO PICCHI (utilizzati per calcolare wf medio): " << tf.size(); 
        if (tf.size()>=20)
        {
            cout << " --> sono sufficenti!" << endl; 
        }
        else
        {
            cout << " --> non sono sufficenti! " << endl; 
        }


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
        wf = (2*M_PI)/mediaTf; 

        // aggiungo ai vettori risultati i diversi valori
        A_ris.push_back(mediaA); 
        Tf_ris.push_back(mediaTf);
        wf_ris.push_back(wf); 

        cout << "A_max MEDIO: " << mediaA << endl; 
        cout << "Tf MEDIO: " << mediaTf << endl; 
        cout << "wf MEDIO: " << wf << endl; 
        cout << endl; 
        cout << "*******************************************" << endl; 
    
    }




    return 0;
}