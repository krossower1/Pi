#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <string>

/**
* @brief Calculates Pi using the Riemman sum method and is responsible for starting threads
* @param argv[1] The amount of rectangles to divide
* @param argv[2] The amount of threads to run
*
**/

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Uzycie: " << argv[0]
                  << " <liczba_podzialow> <liczba_watkow>\n";
        return 1;
    }

    const long long numSteps = std::stoll(argv[1]);
    const int numThreads = std::stoi(argv[2]);

    if (numThreads < 1 || numThreads > 50)
    {
        std::cerr << "Liczba watkow musi byc z zakresu 1–50\n";
        return 1;
    }

    const double step = 1.0 / static_cast<double>(numSteps);
    std::vector<double> partialSums(numThreads, 0.0);
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Lambda wykonywana przez kazdy watek
    auto integrate = [&](int threadId)
    {
        long long startIdx = threadId * numSteps / numThreads;
        long long endIdx   = (threadId + 1) * numSteps / numThreads;

        double localSum = 0.0;

        for (long long i = startIdx; i < endIdx; ++i)
        {
            double x = (i + 0.5) * step;
            localSum += 4.0 / (1.0 + x * x);
        }

        partialSums[threadId] = localSum;
    };

    // Uruchomienie watkow
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(integrate, i);
    }

    // Oczekiwanie na zakonczenie watkow
    for (auto& t : threads)
    {
        t.join();
    }

    double pi = 0.0;
    for (double sum : partialSums)
    {
        pi += sum;
    }
    pi *= step;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Przyblizona wartosc PI: " << pi << "\n";
    std::cout << "Czas obliczen: " << elapsed.count() << " s\n";
    std::cout << "Liczba podzialow: " << numSteps << "\n";
    std::cout << "Liczba watkow: " << numThreads << "\n";

    return 0;
}
