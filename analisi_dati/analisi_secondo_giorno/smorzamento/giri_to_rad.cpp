#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std; 
int main ()
{
    ifstream fin ("check_max.txt"); 

    ofstream out ("check_max_rad.txt", std::ios::app); 

    double val1, val2; 

    vector<double> y, x; 


    while (fin>>val1>>val2)
    {
        y.push_back(val2);
        x.push_back(val1);  
    }

    for (int i = 0; i < y.size(); i ++ )
    {
        out << x.at(i) << " " << log(y.at(i)*2*M_PI) << "\n"; 
    }

    

    
    return 0; 
}