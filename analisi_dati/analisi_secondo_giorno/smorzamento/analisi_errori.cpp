// propagazione errori sistematici ad ogni teta max e min
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std; 
int main ()
{
    ifstream fin1 ("check_max_err.txt");
    ifstream fin2 ("check_min_err.txt"); 

    ofstream out1 ("check_max_rad_err.txt");
    ofstream out2 ("check_min_rad_err.txt");

    double val1, val2, val3; 
    vector<double> x, y, erry; 

    while (fin1>>val1>>val2>>val3)
    {
        x.push_back(val1); 
        y.push_back(val2); 
        erry.push_back(val3); 
    }

    for (int i = 0; i < x.size(); i ++)
    {
        out1 << x.at(i) << " " << log(abs(y.at(i)*2*M_PI)) << " " << 0.02/abs(y.at(i)*2*M_PI) << "\n "; 
    }

    x.clear(); 
    y.clear(); 
    erry.clear(); 

    while (fin2>>val1>>val2>>val3)
    {
        x.push_back(val1); 
        y.push_back(val2);
        erry.push_back(val3);         
    }

    for (int i = 0; i < x.size(); i ++)
    {
        out2 << x.at(i) << " " << log(abs(y.at(i)*2*M_PI)) << " " << 0.02/abs(y.at(i)*2*M_PI) << "\n "; 
    }


    return 0; 
}