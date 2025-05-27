#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
using namespace std; 
string InputFile(); 
int main ()
{
    vector<double> t, a, max, t_max, tf; 
    double val1, val2, val3, mediaTf, mediaA, sommaTf, sommaA; 
    string nome; 

    nome = InputFile(); 

    fstream fin (nome);

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


    mediaTf = 0; 
    sommaTf = 0; 
    for (int i = 0; i < tf.size(); i ++ )
    {
        sommaTf += tf.at(i); 
    }

    mediaTf = sommaTf/tf.size();

    cout << "L'ampiezza media vale: " << mediaA << endl; 
    cout << "Tf medio vale: " << mediaTf << endl; 


    return 0;
}
string InputFile ()
{
    string Nome; 

    cout << "Inserisci nome file: "; 
    cin >> Nome; 

    return Nome; 
}