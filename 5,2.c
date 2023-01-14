#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define in_file "Input.txt"
#define out_file "Output.txt"
#define bin_file "Binary.bin"

double function_x (double);
double** make_array (int);
void remove_array (double**, int);
void makeFiles (int, double, double, FILE*, FILE*);
void read_binary (double**, FILE*, int*);
void array_print (double**, int);

int main()
{
    char group[10];
    char name[10];
    char second_name[10];
    double x1;
    double x2;
    double delta;
    int N;
    FILE* fp = fopen (in_file, "r");
    FILE* fp_out = fopen (out_file, "w");
    FILE* fp_bin = fopen (bin_file, "w+b");
    if (fp == NULL || fp_out == NULL || fp_bin == NULL)
    {
        printf("File can't be opened");
        exit(0);
    }
    fscanf(fp, "%lf %lf %d %lf %s %s %s", &x1, &x2, &N, &delta, group, name, second_name);
    fclose(fp);
    if (N == 0)
    {
        N = (x2 - x1) / delta + 1;
    }
    else if (delta == 0)
    {
        delta = (x2 - x1) / (N - 1);
    }
    printf("%s %s, %s\n", name, second_name, group);
    fprintf(fp_out, "Made by %s %s from %s\n", name, second_name, group);
    makeFiles (N, x1, delta, fp_out, fp_bin);
    double** A = make_array(N);
    if (A == NULL)
    {
        printf("\nArray couldn't be created");
        exit(0);
    }
    N=0;
    read_binary (A, fp_bin, &N);
    array_print (A, N);
    remove_array (A, N);
    return 0;
}

double function_x (double x)
{
    return pow(x / 100 - 5, 5) - pow(x / 50 + 10, 4) - pow(x / 25 - 15, 3) - pow(x, 2) - 10;
}

double** make_array (int N)
{
    double** A = (double**) malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++)
    {
        A[i] = (double*) malloc(3 * sizeof(double));
    }
    return A;
}

void remove_array (double** A, int N)
{
    for (int i = 0; i < N; i++)
    {
        free (A[i]);
    }
    free (A);
}

void makeFiles (int N, double x, double delta, FILE* fp_out, FILE* fp_bin)
{
    fprintf(fp_out, "N\tx\tf(x)\n");
    printf("N\tx\tf(x)\n");
    fwrite(&N, sizeof(int), 1, fp_bin);
    for (int i = 0; i < N; i++)
    {
        double temp = function_x(x);
        fprintf(fp_out, "%d\t", i+1);
        fprintf(fp_out, "%.2lf\t", x);
        fprintf(fp_out, "%lf\n", temp);
        fwrite(&x, sizeof(double), 1, fp_bin);
        fwrite(&temp, sizeof(double), 1, fp_bin);
        x += delta;
    }
    fclose(fp_out);
    fclose(fp_bin);
}

void read_binary (double** A, FILE* fp_bin, int* N)
{
    fp_bin = fopen(bin_file, "r+b");
    if (fp_bin==NULL)
    {
        printf("File can't be opened");
        return;
    }
    int k;
    fread(&k, sizeof(int), 1, fp_bin);
    *N = k;
    for (int i = 0; i < *N; i++)
    {
        A[i][0] = i + 1;
        fread(&A[i][1], sizeof(double), 1, fp_bin);
        fread(&A[i][2], sizeof(double), 1, fp_bin);
    }
    fclose (fp_bin);
}

void array_print (double** A, int N)
{
    for (int i = 0; i < N; i++)
    {
        printf("%.lf\t%.2lf\t%lf\n", A[i][0], A[i][1], A[i][2]);
        if (i % 25 == 0 && i != 0)
        {
            system("pause");
        }
    }
}
