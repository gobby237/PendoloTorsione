#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>
#include <string>
using namespace std;
int main() {

    string nome; 
    cout << "Inserire nome file: "; 
    cin >> nome;

    ifstream file(nome);
    if (!file) {
        cerr << "Errore nell'aprire il file." << endl;
        return 1;
    }

    vector<double> x, y;
    double xi, yi;

    while (file >> xi >> yi) {
        x.push_back(xi);
        y.push_back(yi);
    }

    int n = x.size();
    if (n < 2) {
        cerr << "Servono almeno due punti per una regressione lineare." << endl;
        return 1;
    }

    double sum_x = accumulate(x.begin(), x.end(), 0.0);
    double sum_y = accumulate(y.begin(), y.end(), 0.0);
    double sum_xx = 0.0;
    double sum_xy = 0.0;

    for (int i = 0; i < n; ++i) {
        sum_xx += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    double mean_x = sum_x / n;
    double mean_y = sum_y / n;

    double denom = sum_xx - n * mean_x * mean_x;
    double m = (sum_xy - n * mean_x * mean_y) / denom;
    double q = mean_y - m * mean_x;

    // Calcolo della devianza residua
    double s_res = 0.0;
    for (int i = 0; i < n; ++i) {
        double y_fit = m * x[i] + q;
        s_res += pow(y[i] - y_fit, 2);
    }

    double sigma_y = sqrt(s_res / (n - 2));

    // Incertezze su m e q
    double delta = n * sum_xx - sum_x * sum_x;
    double sigma_m = sigma_y * sqrt(n / delta);
    double sigma_q = sigma_y * sqrt(sum_xx / delta);

    // Coefficiente di determinazione R^2
    double s_tot = 0.0;
    for (int i = 0; i < n; ++i) {
        s_tot += pow(y[i] - mean_y, 2);
    }
    double R2 = 1.0 - s_res / s_tot;

    cout << "=== Regressione lineare Y = m*X + q ===" << endl;
    cout << "m (pendenza):      " << m << " ± " << sigma_m << endl;
    cout << "q (intercetta):    " << q << " ± " << sigma_q << endl;
    cout << "R^2 (fit quality): " << R2 << endl;

    return 0;
}
