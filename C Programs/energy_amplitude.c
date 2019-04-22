//
// Created by jossue on 10/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#define ELEMENTS 118080 //354240
#define FRAMES 738
#define FICHEROAMP "o1.201704121431.16.dat"
#define FICHERONORM "norm_test.txt"

#define RESULTADOSE "frame_energy.dat"
#define RESULTADOSI "frame_ink.dat"
//#define numThreads 2

typedef struct _amplitudes {
    char tiempo[20];
    char amplitud[20];
    double amplitud_cuad;
    double distance;
    double amplitud_norm;
} amplitudes;

typedef struct _frames {
    double energia;
    double ink;
} FramesStruct;


void leerDatos(amplitudes *R) {

    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHEROAMP, "r");

    char strA[20] = {};

    double number;
    int count = 0;
    int i = 0;

    if (file == NULL) {
        perror("Error abriendo archivo");
        exit(1);
    } else {
        printf("Archivo abierto correctamente\n");

        while (fscanf(file, "%s", strA) != EOF) {
//            printf(" %s\n", strA);
            if (i % 2 == 0) {
                strcpy(R[i].tiempo, strA);
//                printf("%s\n", R[i].tiempo);

            } else {
                strcpy(R[i].amplitud, strA);
//                printf("%s\n", R[i].amplitud);
//                int c = getchar();
            }
            i++;
        }
        fclose(file);
    }


}

void leerAmplitudNorm(amplitudes *R) {

    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHERONORM, "r");

    char strA[20] = {};

    double number;
    int count = 0;
    int i = 0;

    if (file == NULL) {
        perror("Error abriendo archivo");
        exit(1);
    } else {
        printf("Archivo abierto correctamente\n");

        while (fscanf(file, "%s", strA) != EOF) {
//            printf("%s\n", strA);
            sscanf(strA, "%lf", &R[i].amplitud_norm);
//            printf("%lf\n",R[i].amplitud_norm);
//            int c = getchar();
            i++;
        }
        fclose(file);
    }


}

void calcularCuad(amplitudes *R) {

    double test;
    int i = 0;
    for (int j = 0; j < ELEMENTS * 2; ++j) {
//        printf("%s\n", R[j].tiempo);
//        int c = getchar();
//        test = strtod(R[j].amplitud, NULL);
        if (j % 2 != 0) {
//            printf("%s\n", R[j].amplitud);
//            int c = getchar();
            sscanf(R[j].amplitud, "%lf", &test);
            R[i].amplitud_cuad = pow(test, 2);
//            printf("%.10lf\n", R[i].amplitud_cuad);
            i++;
        }
    }
}

void calcularDistancias(amplitudes *R) {

    double test;
    double y0 = 0.0;
    int i = 1;
    R[0].distance = sqrt(pow(R[0].amplitud_norm - y0, 2) + 1);
    for (int j = 1; j < ELEMENTS; ++j) {
//        printf("%lf\n", R[j].norm_test);
//        int c = getchar();
        R[j].distance = sqrt(pow(R[j].amplitud_norm - R[j - 1].amplitud_norm, 2) + 1);
//        printf("%.10lf\n", R[j].distance);
//        int c = getchar();

    }
}

void escribirStruct(FramesStruct *RF, int band) {
    FILE *outfile;
    if (band == 0) {
        outfile = fopen(RESULTADOSE, "w");
    } else if (band == 1) {
        outfile = fopen(RESULTADOSI, "w");
    }
    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        // write struct to file
        if (band == 0) {
            for (int i = 0; i < FRAMES; ++i) {
                fprintf(outfile, "%d\t%.15lf\n", i + 1, RF[i].energia);
            }
        } else if (band == 1) {
            for (int i = 0; i < FRAMES; ++i) {
                fprintf(outfile, "%d\t%.15lf\n", i + 1, RF[i].ink);
            }
        }
        if (fwrite != 0)
            printf("contents to file written successfully !\n");
        else
            printf("error writing file !\n");

        // close file
        fclose(outfile);
    }
}

void calcularEnergia(amplitudes *RA, FramesStruct *RF) {
    double sum = 0.0;
    int j = 0;

    for (int i = 0; i < ELEMENTS; i = i + 160) {
//        printf("%lf\n", RA[i].amplitud_cuad);
        for (int k = i; k < i + 160; ++k) {
            sum += RA[k].amplitud_cuad;
        }
        RF[j].energia = sum;
//        printf("%d Suma %lf\n", i, sum);
//        printf("%d Energia %.10lf\n", j, RF[j].energia);
//        int c = getchar();

        j++;

        sum = 0.00;

    }
}

void calcularInk(amplitudes *RA, FramesStruct *RF) {
    double sum = 0.0;
    int j = 0;

    for (int i = 0; i < ELEMENTS; i = i + 160) {
//        printf("%lf\n", RA[i].amplitud_cuad);
        for (int k = i; k < i + 160; ++k) {
            sum += RA[k].distance;
        }
        RF[j].ink = sum/160;
//        printf("%d Suma %lf\n", i, sum);
//        printf("%d Energia %.10lf\n", j, RF[j].energia);
//        int c = getchar();

        j++;

        sum = 0.00;

    }
}


int main(void) {

    //amplitudes REGISTROS[ELEMENTS];
    amplitudes *REGISTROS = malloc(2 * ELEMENTS * sizeof(amplitudes));
    leerDatos(REGISTROS);
    calcularCuad(REGISTROS);

    FramesStruct RFRAMES[FRAMES];
    calcularEnergia(REGISTROS, RFRAMES);
    escribirStruct(RFRAMES, 0); //0 -> Energia

    leerAmplitudNorm(REGISTROS);
    calcularDistancias(REGISTROS);
    calcularInk(REGISTROS, RFRAMES);

    escribirStruct(RFRAMES, 1); //1 -> Ink
    /*
    clock_t begin = clock();
    calcularDistancias(REGISTROS);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Tiempo: %.5f segundos\n", time_spent);

    */
    return 0;

}
