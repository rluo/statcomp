#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
// docker cmd:
// docker run --rm -it -v  $PWD:/data -w /data rossiluo/autograder g++ -O2 -o estimate_pi estimate_pi.cpp -std=c++17
// docker run --rm -it -v  $PWD:/data -w /data rossiluo/autograder ./estimate_pi 10000
// Or putting two lines together
// docker run --rm -it -v  $PWD:/data -w /data rossiluo/autograder bash -e -c  "g++ -O2 -o estimate_pi estimate_pi.cpp -std=c++17 && ./estimate_pi 10000"

double estimate_pi(int n, int seed)
{
    int count = 0;
    std::mt19937 generator(seed); // Mersenne Twister pseudo-random generator
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    for (int i = 0; i < n; ++i)
    {
        double x = distribution(generator);
        double y = distribution(generator);

        if (x * x + y * y <= 1)
        {
            count++;
        }
    }

    return 4.0 * count / n;
}

int main(int argc, char *argv[])
{
    int n;
    int seed = 100; // Default seed value

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_points> [seed]" << std::endl;
        return 1;
    }

    n = std::atoi(argv[1]);
    if (argc >= 3)
    {
        seed = std::atoi(argv[2]);
    }

    auto start = std::chrono::high_resolution_clock::now();
    double pi = estimate_pi(n, seed);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Estimate of Pi: " << pi << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
