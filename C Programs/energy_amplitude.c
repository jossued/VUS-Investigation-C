//
// Created by jossue on 10/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#define ELEMENTS 130560
const int FRAMES = ELEMENTS / 160;

#define FICHEROAMP "o1_201704121431_svu.dat"
#define FICHERONORM "norm_svu.txt"

#define RESULTADOSE     "frame_energy.dat"
#define RESULTADOSEL    "frame_energy_log.dat"
#define RESULTADOSI     "frame_ink.dat"
#define RESULTADOSIL    "frame_ink_log.dat"
#define FICEHROPROM     "promedios.dat"
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
    double energia_log;
    double ink;
    double ink_log;
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
            printf("Escritura en archivo %d!\n", band);
        else
            printf("error escribiendo archivo!\n");

        // close file
        fclose(outfile);
    }
}

double escribirStructLog(FramesStruct *RF, int band) {
//    printf("FRAMES %d\n", FRAMES);
    FILE *outfile;
    double sum = 0.0;
    double promedio_log = 0.0;
    if (band == 0) {
        outfile = fopen(RESULTADOSEL, "w");
    } else if (band == 1) {
        outfile = fopen(RESULTADOSIL, "w");
    }
    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
//        return -1;
    } else {
        // write struct to file
        if (band == 0) {
            for (int i = 0; i < FRAMES; ++i) {
                fprintf(outfile, "%d\t%.15lf\n", i + 1, (RF[i].energia_log));
                sum += (RF[i].energia_log);
            }

        } else if (band == 1) {
            for (int i = 0; i < FRAMES; ++i) {
                fprintf(outfile, "%d\t%.15lf\n", i + 1, RF[i].ink_log);
                sum += RF[i].ink_log;
            }
        }
        promedio_log = sum / FRAMES;

        fclose(outfile);

        if (fwrite != 0) {
            printf("Escritura en archivo log %d!\n", band);
            return promedio_log;
        } else {
            printf("error escribiendo archivo log!\n");
            return -1;
        }

    }
}

void escribirPromedios(double Pe, double Pi) {
    FILE *outfile;
    outfile = fopen(FICEHROPROM, "w");

    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        fprintf(outfile, "PE\t%.15lf\n", Pe);
        fprintf(outfile, "PI\t%.15lf", Pi);
        if (fwrite != 0)
            printf("Escritura en archivo!\n");
        else
            printf("error escribiendo archivo!\n");

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
        RF[j].energia_log = log10(sum);
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
        RF[j].ink = sum / 160;
        RF[j].ink_log = log10(sum / 160);

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
    double promedio_logE = escribirStructLog(RFRAMES, 0); //0 -> Energia

    leerAmplitudNorm(REGISTROS);
    calcularDistancias(REGISTROS);
    calcularInk(REGISTROS, RFRAMES);

    escribirStruct(RFRAMES, 1); //1 -> Ink
    double promedio_logI = escribirStructLog(RFRAMES, 1); //1 -> Ink

    escribirPromedios(promedio_logE, promedio_logI);
    return 0;

}
