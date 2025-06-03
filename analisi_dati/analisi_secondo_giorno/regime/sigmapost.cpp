#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct DataPoint {
    double x;
    double y;
};

vector<DataPoint> readData(const string& filename) {
    vector<DataPoint> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: impossibile aprire il file " << filename << endl;
        exit(1);
    }
    double x, y;
    while (file >> x >> y) {
        data.push_back({x, y});
    }
    file.close();
    return data;
}

double calculatePosteriorSigma(const vector<DataPoint>& data, double a, double b, double c, double x_min, double x_max) {
    int count = 0;
    double sum_squared_residuals = 0.0;
    
    for (const auto& point : data) {
        if (point.x >= x_min && point.x <= x_max) {
            double y_fit = a * point.x * point.x + b * point.x + c;
            double residual = point.y - y_fit;
            sum_squared_residuals += residual * residual;
            count++;
        }
    }
    
    if (count <= 3) {
        cerr << "Errore: numero insufficiente di punti nel range (" << count << "). Necessari almeno 4 punti." << endl;
        exit(1);
    }
    
    return sqrt(sum_squared_residuals / (count - 3));
}

int main() {
    string filename;
    cout << "Inserisci il percorso del file di dati: ";
    cin >> filename;

    double a, b, c;
    cout << "Inserisci i parametri della parabola (a b c): ";
    cin >> a >> b >> c;

    double x_min, x_max;
    cout << "Inserisci il range delle x (x_min x_max): ";
    cin >> x_min >> x_max;

    vector<DataPoint> data = readData(filename);
    double sigma = calculatePosteriorSigma(data, a, b, c, x_min, x_max);

    cout << "Sigma posteriori nel range [" << x_min << ", " << x_max << "]: " << sigma << endl;

    return 0;
}