#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "winbgi2.h"
#define M_PI   3.14159265358979323846

void initPositions(double* x, double* y, int N) {
    int i;
    double positionsX[] = { 200.0, 240.0, 280.0, 220.0, 260.0, 240.0, 240.0 };
    double positionsY[] = { 100.0, 100.0, 100.0, 140.0, 140.0, 180.0, 380.0 };
    for (i = 0; i < N; i++) {
        x[i] = positionsX[i];
        y[i] = positionsY[i];
    }
}

void display(double* x, double* y, int N) {
    int i;
    for (i = 0; i < N; i++) {
        circle(x[i], y[i], 20);
    }
}

void showTable(double* x, double* y, int N) {
    int i;
    for (i = 0; i < N; i++) {
        printf("%lf\t%lf\n", x[i], y[i]);
    }
}

void initSpeed(double* x, double* y, int N) {
    int i;
    double speedsX[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.2 };
    double speedsY[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1 };
    for (i = 0; i < N; i++) {
        x[i] = speedsX[i];
        y[i] = speedsY[i];
    }
}

void move(double* x, double* y, double* a, double* b, int N) {
    int i;
    for (i = 0; i < N; i++) {
        x[i] += a[i];
        y[i] += b[i];
    }
}

void collideWall(double* x, double* y, double* a, double* b, double* friction, int N) {
    int i;
    for (i = 0; i < N; i++) {
        if (x[i] + a[i] < 20) {
            a[i] = a[i] * (-1) * friction[i];
        }
        if (x[i] + a[i] > 380) {
            a[i] = a[i] * (-1) * friction[i];
        }
        if (y[i] + b[i] < 20) {
            b[i] = b[i] * (-1) * friction[i];
        }
        if (y[i] + b[i] > 380) {
            b[i] = b[i] * (-1) * friction[i];
        }
    }
}

double showEnergy(double* a, double* b, int N) {
    int i;
    double z = 0;
    double h = 0;
    for (i = 0; i < N; i++) {
        h = sqrt((a[i] * a[i] + b[i] * b[i]));
        z = z + h;
    }
    printf("%lf\n", z);
    return z;
}

void collideBall(double* x, double* y, double* a, double* b, double* friction, int N) {
    int i, q;
    for (i = 0; i < N; i++) {
        double z;
        for (q = 0; q < i; q++) {
            z = sqrt((x[i] + a[i] - x[q] - a[q]) * (x[i] + a[i] - x[q] - a[q]) + (y[i] + b[i] - y[q] - b[q]) * (y[i] + b[i] - y[q] - b[q]));
            if (q != i) {
                if (z < 40) {
                    double r = a[i];
                    double g = b[i];
                    a[i] = a[q] * friction[i];
                    b[i] = b[q] * friction[i];
                    a[q] = r * friction[q];
                    b[q] = g * friction[q];
                }
            }
        }
    }
}

void initFriction(double* friction, int N) {
    int i;
    double frictionValues[] = { 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 0.9 }; // Dodaj tutaj wartoœci tarcia dla ka¿dej kuli
    for (i = 0; i < N; i++) {
        friction[i] = frictionValues[i];
    }
}

int main() {

    int N = 7;

    graphics(500, 500);
    srand(time(NULL));

    double x[10], y[10];
    double xV[10], yV[10];
    double friction[10];

    initPositions(x, y, N);
    initSpeed(xV, yV, N);
    initFriction(friction, N);

    showTable(x, y, N);
    printf("\n");
    showTable(xV, yV, N);
    printf("\n");

    clock_t start, end;
    double elapsed_time;
    double energy;

    // Tworzenie unikalnej nazwy pliku
    char filename[80];
    time_t now = time(NULL);
    sprintf_s(filename, sizeof(filename), "positions_%lld.txt", (long long)now);

    FILE* file;
    if (fopen_s(&file, filename, "w") != 0) {
        printf("Cannot open file\n");
        return 1;
    }

    start = clock();

    for (int j = 0; j < 20000; j++) {

        collideBall(x, y, xV, yV, friction, N);
        collideWall(x, y, xV, yV, friction, N);

        move(x, y, xV, yV, N);
        energy = showEnergy(xV, yV, N);
        display(x, y, N);

        animate(200);
        clear();

        line(0, 0, 400, 0);
        line(0, 0, 0, 400);
        line(400, 0, 400, 400);
        line(0, 400, 400, 400);

        end = clock();
        elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Time elapsed: %.2f s, Energy: %lf\n", elapsed_time, energy);
        if (j == 0) {
            fprintf(file, "Time, Energy, ");
            for (int k = 0; k < N; k++) {
                fprintf(file, "Ball %d X, Ball %d Y, ", k + 1, k + 1);
            }
            fprintf(file, "\n");
        }
        fprintf(file, "%.2f, %lf, ", elapsed_time, energy);
        for (int k = 0; k < N; k++) {
            fprintf(file, "%lf, %lf, ", x[k], y[k]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    wait();
    return 0;
}
