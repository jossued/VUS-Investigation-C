//
// Created by jossue on 26/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUMTHRESHOLDS 5
#define NUMPHONEMS 90
#define FICHEROINTERVALOS "manual_class.dat"
#define FICHEROINTERVALOS0 "manual_class_0.dat"
#define FICHEROFRAMES "manual_class_frames.dat"
#define FICHEROLIM  "limites.dat"
#define FICHEROENERGYLOG  "frame_energy_log.dat"

#define ELEMENTS 130560
const int FRAMES = ELEMENTS / 160;

typedef struct _thresholds {
    char tipo[2];
    double threshold;
} ThresholdsStruct;

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

typedef struct _framelog {
    int frame;
    double log_val;
    char clasificacion;
} FramesLogStruct;


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
                    RI[i].interv_tiempo, RI[i].clasificacion);
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
            fprintf(outfile, "%d\t%c\n", RFC[i].frame + 1, RFC[i].clasificacion);
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

void CorregirIntervalos(IntervalsStruct *RI) {
    RI[0].interv_inicio = RI[0].interv_inicio - 57.072;
    RI[0].interv_fin = RI[0].interv_fin - 57.072;

    double sum = 0.79;
    double int_t = 0;

    for (int i = 1; i < NUMPHONEMS; ++i) {
        RI[i].interv_inicio = RI[i].interv_inicio - 58.482 + 0.79;
        RI[i].interv_fin = RI[i].interv_fin - 58.482 + 0.79;
        /*
        int_t = RINTERV[i].interv_fin - RINTERV[i].interv_inicio;
        if(RINTERV[i].interv_tiempo - int_t != 0){
            printf("Error intervalo de tiempo %lf %d %lf\n",RINTERV[i].interv_tiempo, i+1, int_t);
        }
        */
        sum += RI[i].interv_tiempo;

//        printf("%lf\t%lf\n",RINTERV[i].interv_inicio, RINTERV[i].interv_fin);
    }

    printf("Suma tiempo: %lf\n", sum);
}

void SepararIntervalos(IntervalsStruct *RI, FramesClassifStruct *RFC) {
    int ninterv = 0;
    int pos = 0;
    for (int i = 0; i < NUMPHONEMS; ++i) {
        ninterv = RI[i].interv_tiempo * 100;
//        printf("#intervalos: %d\n", ninterv);
        for (int j = pos; j < ninterv + pos; ++j) {
            RFC[j].frame = j;
            RFC[j].clasificacion = RI[i].clasificacion;
//            printf("%d, %c\n", RFC[j].frame, RFC[j].clasificacion);
        }
        pos = ninterv + pos;
//        printf("pos: %d\n", pos);
    }
}


void leerLimites(ThresholdsStruct *RT) {
    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHEROLIM, "r");

    char strA[20] = {};
    int i = 0;
    int count0 = 0;
    int count1 = 0;

    if (file == NULL) {
        perror("Error abriendo archivo");
        exit(1);
    } else {
        printf("Archivo abierto correctamente\n");

        while (fscanf(file, "%s", strA) != EOF) {
//            printf("StrA %s\n", strA);
            if (i % 2 == 0) {
                strcpy(RT[count0].tipo, strA);
//                printf("%s\n", RT[count0].tipo);
                count0++;
            } else {
                sscanf(strA, "%lf", &RT[count1].threshold);
//                printf("%.15lf\n", RT[count1].threshold);
                count1++;
            }
            i++;
        }
        fclose(file);
    }
}

void leerEnergiaLog(FramesLogStruct *RFL) {
    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHEROENERGYLOG, "r");

    char strA[20] = {};
    int i = 0;

    int count0 = 0;
    int count1 = 0;

    if (file == NULL) {
        perror("Error abriendo archivo");
        exit(1);
    } else {
        printf("Archivo abierto correctamente\n");

        while (fscanf(file, "%s", strA) != EOF) {
//            printf("StrA %s\n", strA);
            if (i % 2 == 0) {
                sscanf(strA, "%d", &RFL[count0].frame);
//                printf("%d\n", RFL[count0].frame);
                count0++;
//                int c = getchar();
            } else {
                sscanf(strA, "%lf", &RFL[count1].log_val);
//                printf("%d\t%.15lf\n", count1, RFL[count1].log_val);
                count1++;
            }
            i++;
        }
        fclose(file);
    }
}

void ClasificarVUSEnergia(ThresholdsStruct *RL, FramesLogStruct *REL) {

    for (int i = 0; i < FRAMES; ++i) {
//        printf("%lf > %lf\n", REL[i].log_val, RL[0].threshold );
        if (REL[i].log_val > RL[0].threshold) {
//            printf("%lf > %lf\n", REL[i].log_val, RL[0].threshold );
//            strcpy(REL[i].clasificacion, "V");
            REL[i].clasificacion = 'V';

        } else if (REL[i].log_val < RL[0].threshold && REL[i].log_val > RL[1].threshold) {
//            strcpy(REL[i].clasificacion, 'U');
            REL[i].clasificacion = 'U';
        } else if (REL[i].log_val < RL[1].threshold) {
//            strcpy(REL[i].clasificacion, 'S');
            REL[i].clasificacion = 'S';
        }
//        printf("%d\t%c\n", REL[i].frame,REL[i].clasificacion);
    }

}

void ResultadosClassEnergia(FramesLogStruct *REL, FramesClassifStruct *RFMC) {
    int TPv = 0;
    int TNv = 0;
    int FPv = 0;
    int FNv = 0;

    int TPu = 0;
    int TNu = 0;
    int FPu = 0;
    int FNu = 0;

    int TPs = 0;
    int TNs = 0;
    int FPs = 0;
    int FNs = 0;

    for (int i = 0; i < FRAMES; ++i) {
        if (RFMC[i].clasificacion == REL[i].clasificacion) {
//            printf("Iguales: M -> %c A -> %c \n", RFMC[i].clasificacion, REL[i].clasificacion);
            switch (RFMC[i].clasificacion) {
                case 'V':
                    printf("Iguales: Mv -> %c Av -> %c \n", RFMC[i].clasificacion, REL[i].clasificacion);
                    TNu++;
                    TPv++;
                    TNs++;
                    break;
                case 'U':
                    printf("Iguales: Mu -> %c Au -> %c \n", RFMC[i].clasificacion, REL[i].clasificacion);
                    TPu++;
                    TNv++;
                    TNs++;
                    break;
                case 'S':
                    printf("Iguales: Ms -> %c As -> %c \n", RFMC[i].clasificacion, REL[i].clasificacion);
                    TNu++;
                    TNv++;
                    TPs++;
                    break;
            }
        } else if (RFMC[i].clasificacion == 'V' && REL[i].clasificacion == 'U') {
            

//            printf("Diferentes: (M -> %c A -> %c) \n", RFMC[i].clasificacion, REL[i].clasificacion);
        }
    }


}

int main(void) {

    IntervalsStruct RINTERV[NUMPHONEMS];

    LeerClasificacionManual(RINTERV);
    CorregirIntervalos(RINTERV);
    EscribirClasificacionManual(RINTERV);

    FramesClassifStruct RFMANUALCLASSIF[FRAMES];

    SepararIntervalos(RINTERV, RFMANUALCLASSIF);
    EscribirClasificacionFrames(RFMANUALCLASSIF);

    //Clasificacion Energia
    ThresholdsStruct RLIMS[NUMTHRESHOLDS];
    FramesClassifStruct RFENERGYCLASSIF[FRAMES];
    FramesLogStruct RENERGYLOG[FRAMES];

    leerLimites(RLIMS);
    leerEnergiaLog(RENERGYLOG);

    ClasificarVUSEnergia(RLIMS, RENERGYLOG);
    ResultadosClassEnergia(RENERGYLOG, RFMANUALCLASSIF);

    return 0;
}