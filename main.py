import subprocess
import re
import matplotlib.pyplot as plt

# -----------------------------
# Konfiguracja testów
# -----------------------------
exe_path = "./pi"  # ścieżka do programu C++
num_steps_list = [10_000_000, 50_000_000, 100_000_000]  # 3 linie
threads_list = [1, 2, 4, 8, 16, 32]  # oś X

# Słownik: {numSteps: [(threads, time), ...]}
results = {}

time_regex = re.compile(r"Czas obliczen:\s+([\d.]+)\s+s")

# -----------------------------
# Uruchamianie testów
# -----------------------------
for num_steps in num_steps_list:
    results[num_steps] = []
    for threads in threads_list:
        print(f"Uruchamiam: podzialy={num_steps}, watki={threads}")

        process = subprocess.run(
            [exe_path, str(num_steps), str(threads)],
            capture_output=True,
            text=True
        )

        if process.returncode != 0:
            print("Blad programu:")
            print(process.stderr)
            continue

        match = time_regex.search(process.stdout)
        if not match:
            print("Nie udalo sie odczytac czasu!")
            continue

        elapsed_time = float(match.group(1))
        results[num_steps].append((threads, elapsed_time))

# -----------------------------
# Rysowanie wykresu
# -----------------------------
plt.figure(figsize=(10, 6))

for num_steps, data in results.items():
    threads = [x[0] for x in data]
    times = [x[1] for x in data]
    plt.plot(threads, times, marker='o', label=f"Podziały = {num_steps}")

plt.xlabel("Liczba watkow")
plt.ylabel("Czas [s]")
plt.title("Czas obliczen w zaleznosci od liczby watkow")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()