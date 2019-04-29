//
// Created by jossue on 26/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMPHONEMS 90
#define FICHEROINTERVALOS "manual_class.dat"
#define FICHEROINTERVALOS0 "manual_class_0.dat"
#define FICHEROFRAMES "manual_class_frames.dat"

#define ELEMENTS 130560
const int FRAMES = ELEMENTS / 160;

typedef struct _intervals {
    char simbolo[4];
    double interv_inicio;
    double interv_fin;
    double interv_tiempo;
    char clasificacion;
} IntervalsStruct;

typedef struct _frameclassif {
    int frame;
    char clasificacion;
} FramesClassifStruct;

void EscribirClasificacionManual(IntervalsStruct *RI) {
    FILE *outfile;
    outfile = fopen(FICHEROINTERVALOS0, "w");

    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {

        for (int i = 0; i < NUMPHONEMS; ++i) {
            fprintf(outfile, "%s\t%.3lf\t%.3lf\t%.2lf\t%c\n", RI[i].simbolo, RI[i].interv_inicio, RI[i].interv_fin,
                    RI[i].interv_tiempo,RI[i].clasificacion);
        }

        if (fwrite != 0)
            printf("contents to file written successfully !\n");
        else
            printf("error writing file !\n");

        // close file
        fclose(outfile);
    }
}

void EscribirClasificacionFrames(FramesClassifStruct *RFC) {
    FILE *outfile;
    outfile = fopen(FICHEROFRAMES, "w");

    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {

        for (int i = 0; i < FRAMES; ++i) {
            fprintf(outfile, "%d\t%c\n", RFC[i].frame+1, RFC[i].clasificacion);
        }

        if (fwrite != 0)
            printf("contents to file written successfully !\n");
        else
            printf("error writing file !\n");

        // close file
        fclose(outfile);
    }
}

void LeerClasificacionManual(IntervalsStruct *RI) {

    FILE *file = fopen(FICHEROINTERVALOS, "r");

    char strA[35] = {};

    double number;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int i = 0;

    if (file == NULL) {
        perror("Error abriendo archivo");
        exit(1);
    } else {
        printf("Archivo abierto correctamente\n");

        while (fscanf(file, "%s", strA) != EOF) {
            switch (i) {
                case 0:
                    strcpy(RI[count0].simbolo, strA);
//                    printf("%d %s\n",count0, RI[count0].simbolo);
                    count0++;
                    break;
                case 1:
                    sscanf(strA, "%lf", &RI[count1].interv_inicio);
//                    printf("%d %f\n",count1, RI[count1].interv_inicio);
                    count1++;
                    break;
                case 2:
                    sscanf(strA, "%lf", &RI[count2].interv_fin);
//                    printf("%d %f\n",count2, RI[count2].interv_fin);
                    count2++;
                    break;
                case 3:
                    sscanf(strA, "%lf", &RI[count3].interv_tiempo);
//                    printf("%d %f\n",count3, RI[count3].interv_tiempo);
                    count3++;
                    break;
                case 4:
                    sscanf(strA, "%c", &RI[count4].clasificacion);
//                    printf("%d %c\n", count4, RI[count4].clasificacion);
                    count4++;
                    i = -1;
                    break;

            }
//            printf("%s\n", strA);
//            sscanf(strA, "%lf", &R[i].amplitud_norm);
//            printf("%lf\n",R[i].amplitud_norm);
//            int c = getchar();
            i++;
        }
        fclose(file);
    }
}

void CorregirIntervalos(IntervalsStruct *RI){
    RI[0].interv_inicio = RI[0].interv_inicio - 57.072;
    RI[0].interv_fin = RI[0].interv_fin - 57.072;

    double sum = 0.79;
    double int_t = 0;

    for (int i = 1; i < NUMPHONEMS; ++i) {
        RI[i].interv_inicio = RI[i].interv_inicio - 58.482+0.79;
        RI[i].interv_fin = RI[i].interv_fin - 58.482+0.79;
        /*
        int_t = RINTERV[i].interv_fin - RINTERV[i].interv_inicio;
        if(RINTERV[i].interv_tiempo - int_t != 0){
            printf("Error intervalo de tiempo %lf %d %lf\n",RINTERV[i].interv_tiempo, i+1, int_t);
        }
        */
        sum += RI[i].interv_tiempo;

//        printf("%lf\t%lf\n",RINTERV[i].interv_inicio, RINTERV[i].interv_fin);
    }

    printf("Suma tiempo: %lf\n",sum);
}

void SepararIntervalos(IntervalsStruct *RI, FramesClassifStruct *RFC){
    int ninterv = 0;
    int pos = 0;
    for (int i = 0; i < NUMPHONEMS ; ++i) {
        ninterv = RI[i].interv_tiempo*100;
        printf("#intervalos: %d\n",ninterv);
        for (int j = pos; j < ninterv+pos; ++j) {
            RFC[j].frame=j;
            RFC[j].clasificacion=RI[i].clasificacion;
            printf("%d, %c\n", RFC[j].frame, RFC[j].clasificacion);
        }
        pos = ninterv+pos;
        printf("pos: %d\n",pos);
    }
}

int main(void) {

    IntervalsStruct RINTERV[NUMPHONEMS];

    LeerClasificacionManual(RINTERV);

    CorregirIntervalos(RINTERV);

    EscribirClasificacionManual(RINTERV);

    FramesClassifStruct RFCLASSIF[FRAMES];

    SepararIntervalos(RINTERV, RFCLASSIF);

    EscribirClasificacionFrames(RFCLASSIF);

    return 0;
}