import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import os

# --- Funzione sinusoidale ---
def sinusoide(t, A, w, phi, offset):
    return A * np.sin(w * t + phi) + offset

# --- Leggi dati da file ---
def leggi_dati(nome_file):
    tempi = []
    ampiezze = []
    with open(nome_file, 'r') as file:
        for riga in file:
            parti = riga.strip().split()
            if len(parti) >= 3:
                try:
                    tempo = float(parti[0])
                    ampiezza = float(parti[2])
                    tempi.append(tempo)
                    ampiezze.append(ampiezza)
                except ValueError:
                    continue
    return np.array(tempi), np.array(ampiezze)

# --- Calcolo MAD (deviazione mediana assoluta) ---
def mad(data):
    mediana = np.median(data)
    return np.median(np.abs(data - mediana))

# --- Stima frequenza angolare da FFT ---
def stima_frequenza_fft(tempi, ampiezze):
    dt = tempi[1] - tempi[0]
    n = len(ampiezze)
    fft = np.fft.rfft(ampiezze - np.mean(ampiezze))
    frequenze = np.fft.rfftfreq(n, d=dt)
    indice_picco = np.argmax(np.abs(fft))
    return 2 * np.pi * frequenze[indice_picco]  # restituisce ω = 2πf

# --- Fitting iterativo con rimozione outlier ---
def fitting_e_filtraggio(tempi, ampiezze, soglia=3.5, iterazioni=3):
    w0 = stima_frequenza_fft(tempi, ampiezze)
    A0 = (np.max(ampiezze) - np.min(ampiezze)) / 2
    phi0 = 0
    offset0 = np.mean(ampiezze)
    popt = [A0, w0, phi0, offset0]

    for _ in range(iterazioni):
        try:
            popt, _ = curve_fit(sinusoide, tempi, ampiezze, p0=popt)
        except RuntimeError:
            break

        residui = ampiezze - sinusoide(tempi, *popt)
        soglia_mad = soglia * mad(residui)
        maschera = np.abs(residui) < soglia_mad

        tempi = tempi[maschera]
        ampiezze = ampiezze[maschera]

    return tempi, ampiezze, popt

# --- Salva dati filtrati ---
def salva_dati(nome_file, tempi, ampiezze):
    with open(nome_file, 'w') as f:
        for t, a in zip(tempi, ampiezze):
            f.write(f"{t:.6f}\t{a:.6f}\n")

# --- Grafico confronto ---
def mostra_confronto(tempi_orig, ampiezze_orig, tempi_corr, ampiezze_corr, parametri_fit, nome_output_plot):
    plt.figure(figsize=(12, 6))
    plt.plot(tempi_orig, ampiezze_orig, 'm+', label='Originali', alpha=0.4)
    plt.plot(tempi_corr, ampiezze_corr, 'g.', label='Corretti', markersize=4)

    if parametri_fit is not None:
        t_fit = np.linspace(min(tempi_orig), max(tempi_orig), 2000)
        y_fit = sinusoide(t_fit, *parametri_fit)
        plt.plot(t_fit, y_fit, 'b-', label='Fit Sinusoidale', linewidth=2)

    plt.xlabel("Tempo (s)")
    plt.ylabel("Ampiezza")
    plt.title("Confronto: Originali vs Corretti vs Fit")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(nome_output_plot)
    plt.show()

# --- Funzione principale ---
def main():
    nome_file = input("Inserisci il nome del file (.txt): ").strip()
    if not os.path.exists(nome_file):
        print("File non trovato.")
        return

    tempi, ampiezze = leggi_dati(nome_file)
    tempi_corr, ampiezze_corr, parametri_fit = fitting_e_filtraggio(tempi, ampiezze)

    nome_file_out = nome_file.replace('.txt', '') + "_corretto.txt"
    salva_dati(nome_file_out, tempi_corr, ampiezze_corr)
    print(f"Dati corretti salvati in: {nome_file_out}")

    nome_plot = nome_file.replace('.txt', '') + "_grafico.png"
    mostra_confronto(tempi, ampiezze, tempi_corr, ampiezze_corr, parametri_fit, nome_plot)
    print(f"Grafico salvato come: {nome_plot}")

if __name__ == "__main__":
    main()
