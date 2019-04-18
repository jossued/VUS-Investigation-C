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
#define FICHERO "o1_201704121431.dat"
#define RESULTADOS "energy_amplitud.dat"
//#define numThreads 2

typedef struct _amplitudes {
    char tiempo[20];
    char amplitud[20];
    double amplitud_cuad;
} amplitudes;

typedef struct _frames {
    double energia;
} FramesStruct;


void leerDatos(amplitudes *R) {

    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHERO, "r");

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
            //printf("%f\n", R[i].tiempo);
            i++;
        }
        fclose(file);
    }


}

void calcularCuad(amplitudes *R) {

    double test;
    int i = 0;
    for (int j = 0; j < ELEMENTS * 2; ++j) {
//        printf("%s", R[j].amplitud);
//        test = strtod(R[j].amplitud, NULL);
        if (j % 2 != 0) {
            sscanf(R[j].amplitud, "%lf", &test);
            R[i].amplitud_cuad = pow(test, 2);
//            printf("%.10lf\n", R[i].energia);
            i++;
        }
    }
}

void escribirStruct(amplitudes *R) {
    FILE *outfile = fopen(RESULTADOS, "w");
    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        // write struct to file
        for (int i = 0; i < 2 * ELEMENTS; ++i) {
//            strcpy(src,R[i]);
//            fwrite(src, sizeof(amplitudes), 1, outfile);
//            printf("%s\n", R[i]);
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
        printf("%d Energia %.10lf\n", j, RF[j].energia);
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
    //escribirStruct(REGISTROS);

    FramesStruct RFRAMES[FRAMES];
    calcularEnergia(REGISTROS, RFRAMES);

    /*
    clock_t begin = clock();
    calcularDistancias(REGISTROS);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Tiempo: %.5f segundos\n", time_spent);

    */
    return 0;

}
