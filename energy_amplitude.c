//
// Created by jossue on 10/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <string.h>
#include <time.h>

#define ELEMENTS 354240
#define FICHERO "o1_201704121431.dat"
#define RESULTADOS "energy_amplitude.txt"
//#define numThreads 2

typedef struct {
    char tiempo[20];
    char amplitud[20];
    double energia;
} amplitudes;


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

void calcularEnergias (amplitudes *R){

    double test;
    int i=0;
    for (int j = 0; j < ELEMENTS*2; ++j) {
//        printf("%s", R[j].amplitud);
//        test = strtod(R[j].amplitud, NULL);
        if (j % 2 != 0){
            sscanf(R[j].amplitud, "%lf", &test);
            R[i].energia=pow(test,2);
//            printf("%.10lf\n", R[i].energia);
            i++;
        }
    }
}

/*
int calcularDistancias(moleculas *R){

    int bothAB = 0;
    int onlyA = 0;
    int onlyB = 0;

    int *a;
    int *b;

    FILE *fp;
    fp = fopen(RESULTADOS, "w");
    int x = 1;
    int *binA = malloc(4);
    int *binB = malloc(4);

    int tid;
    int i=0;
    int j=0;
    int k=0;
    int m=0;
    //omp_set_num_threads(2);
    for (int j = 0; j < ELEMENTS; ++j) {
        for (int i = j + 1; i < ELEMENTS; ++i) {

                for (k = 0; k < 512; ++k) {
                    //printf("%c , %c\n",*((REGISTROS+j)->valor +k),*((REGISTROS+i)->valor +k));
                    b = hex_bin(*(R[i].valor + k), &binA[0]);
                    a = hex_bin(*(R[j].valor + k), &binB[0]);

                    for (int m = 0; m < 4; ++m) { //   *(a+j)
                        //printf("%d , %d\n",*(a+j),*(b+j));
                        if (*(a + m) && *(b + m)) {
                        	//#pragma critical
                            bothAB++;
                        }
                        if (*(a + m) && *(b + m) == 0) {
                            //#pragma critical
                            onlyA++;
                        }
                        if (*(b + m) && *(a + m) == 0) {
                            //#pragma critical
                            onlyB++;
                        }
                    }

                }



            double sim = (double) bothAB / (onlyA + onlyB + bothAB);
            double dist = 1 - sim;

            fprintf(fp, "%d  %s,%s   Distancia: %.10f\n", x, R[j].id, R[i].id, dist);
            bothAB = 0;
            onlyA = 0;
            onlyB = 0;
            x++;

        }

    }


    fclose(fp);
    return x;
}

*/

int main(void) {

    //amplitudes REGISTROS[ELEMENTS];
    amplitudes *REGISTROS = malloc(2 * ELEMENTS * sizeof(amplitudes));
    leerDatos(REGISTROS);
    calcularEnergias(REGISTROS);


    FILE *outfile;

    // open file for writing
    outfile = fopen (RESULTADOS, "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit (1);
    }
    // write struct to file
    fwrite (&REGISTROS, sizeof(amplitudes), 1, outfile);

    if(fwrite != 0)
        printf("contents to file written successfully !\n");
    else
        printf("error writing file !\n");

    // close file
    fclose (outfile);

    /*
    clock_t begin = clock();
    calcularDistancias(REGISTROS);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Tiempo: %.5f segundos\n", time_spent);

    */
    return 0;

}
