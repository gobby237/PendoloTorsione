#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Struttura per i dati
struct DataPoint {
    double x;
    double y;
    double err_y;
};

int main() {
    string filename;
    cout << "Inserisci il nome del file: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Errore nell'apertura del file." << endl;
        return 1;
    }

    vector<DataPoint> data;
    double x, y, err_y;

    // Lettura dati
    while (infile >> x >> y >> err_y) {
        data.push_back({x, y, err_y});
    }
    infile.close();

    if (data.size() < 2) {
        cerr << "Numero insufficiente di dati per il fit." << endl;
        return 1;
    }

    // Fit lineare ponderato: y = a*x + b
    double S = 0, Sx = 0, Sy = 0, Sxx = 0, Sxy = 0;

    for (const auto& point : data) {
        double w = 1.0 / (point.err_y * point.err_y);
        S    += w;
        Sx   += point.x * w;
        Sy   += point.y * w;
        Sxx  += point.x * point.x * w;
        Sxy  += point.x * point.y * w;
    }

    double Delta = S * Sxx - Sx * Sx;
    if (Delta == 0) {
        cerr << "Errore: determinante nullo, impossibile effettuare il fit." << endl;
        return 1;
    }

    double a = (S * Sxy - Sx * Sy) / Delta;
    double b = (Sxx * Sy - Sx * Sxy) / Delta;

    double sigma_a = sqrt(S / Delta);
    double sigma_b = sqrt(Sxx / Delta);

    cout << "\n=== Risultati del fit lineare ponderato ===" << endl;
    cout << "a (pendenza)     = " << a << " ± " << sigma_a << endl;
    cout << "b (intercetta)   = " << b << " ± " << sigma_b << endl;

        // Calcolo di rho (coefficiente di correlazione tra a e b)
    double cov_ab = -Sx / Delta;
    double rho = cov_ab / (sigma_a * sigma_b);

    cout << "rho (correlazione a, b) = " << rho << endl;


    return 0;
}
