#include <iostream>
#include <cmath>
#include <chrono>

int main() {
    const double startAngle = 10.0;
    const double endAngle = 1e20;

    for (double angle = startAngle; angle <= endAngle; angle *= 10)
    {
        double accum = 0.0;
        auto startTime = std::chrono::high_resolution_clock::now();

        for (int ii = 0; ii < 10'000'000; ++ii)
        {
            // Compute sine and cosine for the current angle
            double sine = sin(angle);
            double cosine = cos(angle);

            accum += sine + cosine;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

        std::cout << "Angle: " << static_cast<uint64_t>(angle) << " Accum: " << accum << " | Compute Time: " << duration.count() << " microseconds\n";
    }

    return 0;
}
