// propagazione errori sistematici ad ogni teta max e min
#include <iostream>
#include <fstream>
using namespace std; 
int main ()
{
    ifstream fin1 ("check_max_err.txt");
    ifstream fin2 ("check_min_err.txt"); 

    ofstream out1 ("check_max_rad_err");
    ofstream out2 ("check_min_rad_err");

    

    return 0; 
}