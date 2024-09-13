#include <stdio.h>
#include <omp.h>

#define STEPCOUNTER 1000000

int main() {
    long i;
    double pi = 0;

    #pragma omp parallel 
    {
        double partial_pi = 0;

        #pragma omp for
        for (i = 0; i < STEPCOUNTER; i++) {
            double p1 = 1.0 / (i * 4.0 + 1.0); 
            double p2 = 1.0 / (i * 4.0 + 3.0);
            partial_pi += p1 - p2;
        }

        #pragma omp critical
        pi += partial_pi;
    }

    pi *= 4.0;
    printf("%f\n", pi);

    return 0;
}
