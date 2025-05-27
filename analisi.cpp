#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std; 
string InputFile(); 
int main ()
{
    vector<double> t, a, max, t_max, tf, A_ris, Tf_ris; 

    // INSERISCI FILE IN INPUT 
    vector<string> nome = {"0_96Hz.txt", "0_95688Hz.txt"}; 
    
    double val1, val2, val3, mediaTf, mediaA, sommaTf, sommaA; 
     
 
    for (int i = 0; i < nome.size(); i ++ )
    {
        t.clear(); 
        a.clear(); 
        max.clear(); 
        t_max.clear();
        tf.clear();

        fstream fin (nome.at(i));

        if (!fin)
        {
            cout << "File errato o assente!" << endl; 
        }


        while (fin>>val1>>val2>>val3)
        {
            t.push_back(val1); 
            a.push_back(val3); 
        }

        // abbiamo i vettori con i tempi e le ampiezze 

        for (int i = 2; i < a.size() - 1; i ++ )
        {
            if ((a.at(i) > a.at(i+1) ) && (a.at(i) > a.at(i-1)) && (a.at(i) > a.at(i-2)) && (a.at(i) > a.at(i+2) ))
            {
                max.push_back(a.at(i)); 
                t_max.push_back(t.at(i)); 
            }
        }
        // abbiamo trovato tutti i massimi ed i tempi  


        // utilizziamo sempre il numero maggiore di massimi per stimare il Tf 
        for (int i = 0; i < t_max.size() - 1; i++)
        {
            tf.push_back(t_max.at(i+1) - t_max.at(i)); 
            i++; 
        }

        mediaA = 0;  
        sommaA = 0; 
        for (int i = 0; i < max.size(); i ++ )
        {
            sommaA += max.at(i); 
        }

        mediaA = sommaA/max.size();
        
        // metto all'interno di un vettore tutte le ampiezze massime
        A_ris.push_back(mediaA); 


        mediaTf = 0; 
        sommaTf = 0; 
        for (int i = 0; i < tf.size(); i ++ )
        {
            sommaTf += tf.at(i); 
        }

        mediaTf = sommaTf/tf.size();

        Tf_ris.push_back(mediaTf);

        cout << "L'ampiezza media vale: " << mediaA << endl; 
        cout << "Tf medio vale: " << mediaTf << endl; 
    
    }




    return 0;
}