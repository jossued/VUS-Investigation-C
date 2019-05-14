//
// Created by jossue on 26/04/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define NUMTHRESHOLDS 5
#define NUMPHONEMS 90
#define FICHEROINTERVALOS "manual_class.dat"
#define FICHEROINTERVALOS0 "manual_class_0.dat"
#define FICHEROFRAMES   "manual_class_frames.dat"

#define FICHEROLIM      "limites.dat"
#define FICHEROPROMS    "promedios.dat"

#define FICHEROENERGYLOG    "frame_energy_log.dat"
#define FICHEROINKLOG       "frame_ink_log.dat"

#define FICHEROERROR    "frame_error_log.dat"

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

typedef struct _framelogclassif {
    int frame;
    double log_val;
    char clasificacion;
} FramesLogClassifStruct;


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


void escribirLimites(ThresholdsStruct *RT) {
    FILE *outfile;
    outfile = fopen(FICHEROLIM, "w");

    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        // write struct to file
        for (int i = 0; i < NUMTHRESHOLDS - 1; ++i) {
            fprintf(outfile, "%s\t%.15lf\n", RT[i].tipo, RT[i].threshold);
        }
        fprintf(outfile, "%s\t%.15lf", RT[NUMTHRESHOLDS - 1].tipo, RT[NUMTHRESHOLDS - 1].threshold);
        if (fwrite != 0)
            printf("Escritura en archivo!\n");
        else
            printf("error escribiendo archivo!\n");

        // close file
        fclose(outfile);
    }
}

void EscribirClasificacionFrames(FramesLogClassifStruct *RFC) {
    FILE *outfile;
    outfile = fopen(FICHEROFRAMES, "w");


    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {

        for (int i = 0; i < FRAMES; ++i) {
            fprintf(outfile, "%d\t%c\n", RFC[i].frame, RFC[i].clasificacion);

        }

        if (fwrite != 0) {
            printf("contents to file written successfully !\n");
        } else
            printf("error writing file !\n");

        // close file
        fclose(outfile);
    }
}

void EscribirClasificacionFramesNum(FramesLogClassifStruct *RFC, char *str, FramesLogClassifStruct *RCE, int band) {
    FILE *outfile;
    outfile = fopen(str, "w");


    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        for (int i = 0; i < FRAMES; ++i) {
            if (band == 0) {
                switch (RFC[i].clasificacion) {
                    case 'V':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 2);
                        break;
                    case 'U':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 1);
                        break;
                    case 'S':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 0);
                        break;
                }
            } else if (band == 1 && RCE[i].clasificacion == '1') {
                switch (RFC[i].clasificacion) {
                    case 'V':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 2);
                        break;
                    case 'U':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 1);
                        break;
                    case 'S':
                        fprintf(outfile, "%d\t%d\n", RFC[i].frame, 0);
                        break;
                }
            }
        }

        if (fwrite != 0) {
            printf("contents to file written successfully !\n");
        } else
            printf("error writing file !\n");

        // close file
        fclose(outfile);
    }
}

void EscribirErrorEnergyFrames(ThresholdsStruct *RLIM, FramesLogClassifStruct *RERR, FramesLogClassifStruct *RCE, int band) {
    FILE *outfile;
    outfile = fopen(FICHEROERROR, "w");


    char src[100];
    if (outfile == NULL) {
        fprintf(stderr, "\nError abriendo archivo\n");
        exit(1);
    } else {
        for (int i = 0; i < FRAMES; ++i) {
            if (band == 0 && RERR[i].clasificacion == '1') {
                fprintf(outfile, "%d\t%lf\t%lf\t%lf\n", RERR[i].frame, RERR[i].log_val, RLIM[0].threshold, RLIM[1].threshold);
            } else if (band == 1 && RCE[i].clasificacion == '1' && RERR[i].clasificacion == '1') {
                fprintf(outfile, "%d\t%lf\t%lf\t%lf\n", RERR[i].frame, RERR[i].log_val, RLIM[0].threshold, RLIM[1].threshold);
            }
        }

        if (fwrite != 0) {
            printf("contents to file written successfully !\n");
        } else
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

void SepararIntervalos(IntervalsStruct *RI, FramesLogClassifStruct *RFC) {
    int ninterv = 0;
    int pos = 0;
    for (int i = 0; i < NUMPHONEMS; ++i) {
        ninterv = RI[i].interv_tiempo * 100;
//        printf("#intervalos: %d\n", ninterv);
        for (int j = pos; j < ninterv + pos; ++j) {
            RFC[j].frame = j + 1;
            RFC[j].clasificacion = RI[i].clasificacion;
//            printf("%d, %c\n", RFC[j].frame, RFC[j].clasificacion);
        }
        pos = ninterv + pos;
//        printf("pos: %d\n", pos);
    }
}


void leerPromedios(ThresholdsStruct *RT) {
    printf("Entro a leerDatos\n");
    FILE *file = fopen(FICHEROPROMS, "r");

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

void leerFicheroLog(FramesLogClassifStruct *RFL, int band) {
    printf("Entro a leerDatos\n");

    FILE *file;

    if (band == 0) {
        file = fopen(FICHEROENERGYLOG, "r");
    } else if (band == 1) {
        file = fopen(FICHEROINKLOG, "r");
    }

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

void ClasificarVUS(ThresholdsStruct *RL, FramesLogClassifStruct *REL, int band) {

    if (band == 0) {
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
    } else if (band == 1) {
        for (int i = 0; i < FRAMES; ++i) {
            if (REL[i].log_val > RL[3].threshold) {
                REL[i].clasificacion = 'U';
            } else if (REL[i].log_val < RL[3].threshold) {
                REL[i].clasificacion = 'V';
            }
//            printf("%d\t%c\n", REL[i].frame, REL[i].clasificacion);
        }
    }

}

void ResultadosClassEnergia(FramesLogClassifStruct *REL, FramesLogClassifStruct *RFMC, FramesLogClassifStruct *RCE,
                            int band) {
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

    int classifV = 0;
    int classifU = 0;
    int classifS = 0;

    if (band == 0) {
        for (int i = 0; i < FRAMES; ++i) {
            if (RFMC[i].clasificacion == 'V') {
                classifV++;
                if (REL[i].clasificacion == 'V') {
                    TPv++;
                    TNu++;
                    TNs++;
                } else if (REL[i].clasificacion == 'U') {
                    FNv++;
                    FPu++;
                    TNs++;
                } else if (REL[i].clasificacion == 'S') {
                    FNv++;
                    TNu++;
                    FPs++;
                }
            } else if (RFMC[i].clasificacion == 'U') {
                classifU++;
                if (REL[i].clasificacion == 'U') {
                    TPu++;
                    TNv++;
                    TNs++;
                } else if (REL[i].clasificacion == 'V') {
                    FPv++;
                    FNu++;
                    TNs++;
                } else if (REL[i].clasificacion == 'S') {
                    TNv++;
                    FNu++;
                    FPs++;
                }
            } else if (RFMC[i].clasificacion == 'S') {
                classifS++;
                if (REL[i].clasificacion == 'S') {
                    TNv++;
                    TNu++;
                    TPs++;
                } else if (REL[i].clasificacion == 'V') {
                    FPv++;
                    TNu++;
                    FNs++;
                } else if (REL[i].clasificacion == 'U') {
                    TNv++;
                    FPu++;
                    FNs++;
                }
            }
        }
    } else if (band == 1) {
        for (int i = 0; i < FRAMES; ++i) {
            if (RCE[i].clasificacion == '1') {
                if (RFMC[i].clasificacion == 'V') {
                    classifV++;
                    if (REL[i].clasificacion == 'V') {
                        TPv++;
                        TNu++;
                    } else if (REL[i].clasificacion == 'U') {
                        FNv++;
                        FPu++;
                    }
                } else if (RFMC[i].clasificacion == 'U') {
                    classifU++;
                    if (REL[i].clasificacion == 'U') {
                        TPu++;
                        TNv++;
                    } else if (REL[i].clasificacion == 'V') {
                        FPv++;
                        FNu++;
                    }
                } else if (RFMC[i].clasificacion == 'S') {
                    classifS++;
                    if (REL[i].clasificacion == 'V') {
                        FPv++;
                        TNu++;
                    } else if (REL[i].clasificacion == 'U') {
                        TNv++;
                        FPu++;
                    }
                }
            }
        }
    }


    printf("\nV(%d) U(%d) S(%d)\n", classifV, classifU, classifS);
    int totalframes = classifV + classifU + classifS;
    double percentV = (double) classifV / (double) totalframes;
    double percentU = (double) classifU / (double) totalframes;
    double percentS = (double) classifS / (double) totalframes;

    printf("V(%.15lf) U(%.15lf) S(%.15lf)\n", percentV, percentU, percentS);
/*
    printf("Num v: %d %d %d %d\n", TPv, TNv, FPv, FNv);
    printf("Num u: %d\n", TPu + TNu + FPu + FNu);
    printf("Num s: %d\n", TPs + TNs + FPs + FNs);
*/
    //Realizar las formulas
    //V
    double precisionV = (double) TPv / (double) (TPv + FPv);
    double recallV = (double) TPv / (double) (TPv + FNv);

    double accuracyV = (double) (TPv + TNv) / (double) (TPv + FPv + FNv + TNv);
    double FmeasureV = 2 * (precisionV * recallV) / (precisionV + recallV);

    printf("Precision V: %lf\n", precisionV);
    printf("Recall V: %.5lf\n", recallV);
    printf("Accuracy V: %.5lf\n", accuracyV);
    printf("Fmeasure V: %.5lf\n", FmeasureV);
    //U
    double precisionU = (double) TPu / (double) (TPu + FPu);
    double recallU = (double) TPu / (double) (TPu + FNu);

    double accuracyU = (double) (TPu + TNu) / (double) (TPu + FPu + FNu + TNu);
    double FmeasureU = 2 * (precisionU * recallU) / (precisionU + recallU);

    printf("Precision U: %lf\n", precisionU);
    printf("Recall U: %.5lf\n", recallU);
    printf("Accuracy U: %.5lf\n", accuracyU);
    printf("Fmeasure U: %.5lf\n", FmeasureU);

    double precisionT = 0.0;
    double recallT = 0.0;

    double accuracyT = 0.0;

    if (band == 0) {
        //S
        double precisionS = (double) TPs / (double) (TPs + FPs);
        double recallS = (double) TPs / (double) (TPs + FNs);

        double accuracyS = (double) (TPs + TNs) / (double) (TPs + FPs + FNs + TNs);
        double FmeasureS = 2 * (precisionS * recallS) / (precisionS + recallS);

        printf("Precision S: %lf\n", precisionS);
        printf("Recall S: %.5lf\n", recallS);
        printf("Accuracy S: %.5lf\n", accuracyS);
        printf("Fmeasure S: %.5lf\n", FmeasureS);

        //TOTAL?

        precisionT = precisionV * percentV + precisionU * percentU + precisionS * percentS;
        recallT = recallV * percentV + recallU * percentU + recallS * percentS;

        accuracyT = accuracyV * percentV + accuracyU * percentU + accuracyS * percentS;
    } else if (band == 1) {
        //TOTAL?

        precisionT = precisionV * percentV + precisionU * percentU;
        recallT = recallV * percentV + recallU * percentU;

        accuracyT = accuracyV * percentV + accuracyU * percentU;
    }

    double FmeasureT = 2 * (precisionT * recallT) / (precisionT + recallT);

    printf("Precision T: %lf\n", precisionT);
    printf("Recall T: %.5lf\n", recallT);
    printf("Accuracy T: %.5lf\n", accuracyT);
    printf("Fmeasure T: %.5lf\n", FmeasureT);

}


void EncontrarFramesCertidumbre(FramesLogClassifStruct *REL, FramesLogClassifStruct *RIL, FramesLogClassifStruct *RCE) {

    for (int i = 0; i < FRAMES; ++i) {
        RCE[i].frame = i + 1;
        if (REL[i].clasificacion == 'V' && RIL[i].clasificacion == 'V') {
            RCE[i].clasificacion = '1';
//            printf("\n%d", RCE[i].frame);
        } else if (REL[i].clasificacion == 'U' && RIL[i].clasificacion == 'U') {
            RCE[i].clasificacion = '1';
//            printf("\n%d", RCE[i].frame);
        } else {
            RCE[i].clasificacion = '0';
        }
    }


}

void EncontrarFramesError(FramesLogClassifStruct *REL, FramesLogClassifStruct *RM, FramesLogClassifStruct *RERR) {

    for (int i = 0; i < FRAMES; ++i) {
        RERR[i].frame = i + 1;
        RERR[i].log_val = REL[i].log_val;
        if (REL[i].clasificacion == RM[i].clasificacion) {
            RERR[i].clasificacion = '0';
        } else {
            RERR[i].clasificacion = '1';
//            printf("\n%d", RERR[i].frame);
        }
    }


}

void ResultadosClassInk(FramesLogClassifStruct *REL, FramesLogClassifStruct *RFMC) {
    int TPv = 0;
    int TNv = 0;
    int FPv = 0;
    int FNv = 0;

    int TPu = 0;
    int TNu = 0;
    int FPu = 0;
    int FNu = 0;

    int classifV = 0;
    int classifU = 0;

    for (int i = 0; i < FRAMES; ++i) {
        if (RFMC[i].clasificacion == 'V') {
            classifV++;
            if (REL[i].clasificacion == 'V') {
                TPv++;
                TNu++;
            } else if (REL[i].clasificacion == 'U') {
                FNv++;
                FPu++;
            }
        } else if (RFMC[i].clasificacion == 'U') {
            classifU++;
            if (REL[i].clasificacion == 'U') {
                TPu++;
                TNv++;
            } else if (REL[i].clasificacion == 'V') {
                FPv++;
                FNu++;
            }
        }
    }

    printf("\nV(%d) U(%d)\n", classifV, classifU);
    double percentV = (double) classifV / (double) FRAMES;
    double percentU = (double) classifU / (double) FRAMES;

    printf("V(%.15lf) U(%.15lf)\n", percentV, percentU);
/*
    printf("Num v: %d %d %d %d\n", TPv, TNv, FPv, FNv);
    printf("Num u: %d\n", TPu + TNu + FPu + FNu);
    printf("Num s: %d\n", TPs + TNs + FPs + FNs);
*/
    //Realizar las formulas
    //V
    double precisionV = (double) TPv / (double) (TPv + FPv);
    double recallV = (double) TPv / (double) (TPv + FNv);

    double accuracyV = (double) (TPv + TNv) / (double) (TPv + FPv + FNv + TNv);
    double FmeasureV = 2 * (precisionV * recallV) / (precisionV + recallV);

    printf("Precision V: %lf\n", precisionV);
    printf("Recall V: %.5lf\n", recallV);
    printf("Accuracy V: %.5lf\n", accuracyV);
    printf("Fmeasure V: %.5lf\n", FmeasureV);
    //U
    double precisionU = (double) TPu / (double) (TPu + FPu);
    double recallU = (double) TPu / (double) (TPu + FNu);

    double accuracyU = (double) (TPu + TNu) / (double) (TPu + FPu + FNu + TNu);
    double FmeasureU = 2 * (precisionU * recallU) / (precisionU + recallU);

    printf("Precision U: %lf\n", precisionU);
    printf("Recall U: %.5lf\n", recallU);
    printf("Accuracy U: %.5lf\n", accuracyU);
    printf("Fmeasure U: %.5lf\n", FmeasureU);

    //TOTAL?

    double precisionT = precisionV * percentV + precisionU * percentU;
    double recallT = recallV * percentV + recallU * percentU;

    double accuracyT = accuracyV * percentV + accuracyU * percentU;
    double FmeasureT = 2 * (precisionT * recallT) / (precisionT + recallT);

    printf("Precision T: %lf\n", precisionT);
    printf("Recall T: %.5lf\n", recallT);
    printf("Accuracy T: %.5lf\n", accuracyT);
    printf("Fmeasure T: %.5lf\n", FmeasureT);


}


void establecerLimites(ThresholdsStruct *RT, double promLogE, double promLogI, bool lim_author) {

    /*
     * Límites establecidos por el autor:
     *
     * Energía V-U -> promLogE
     * Energía U-S -> 2*sqrt(prom(logE))
     * Energía Respiración -> sqrt(4.5 * promLogE)
     *
     * Ink V-U -> promLogI
     *
     */

    printf("Promedio logE: %.15lf \n", promLogE);
    strcpy(RT[0].tipo, "Eu");

    RT[0].threshold = (lim_author) ? promLogE : 0.5 * promLogE;

    double limSilencio = 0;
    double limRespiracion = 0;

    int mult_logE;
    mult_logE = (lim_author) ? 2 : 4;

    if (promLogE < -1) {
        promLogE = fabs(promLogE);

        limSilencio = -1 * mult_logE * sqrt(promLogE);
        limRespiracion = -1 * sqrt(4.5 * promLogE);

    } else {
        limSilencio = mult_logE * sqrt(promLogE);
        limRespiracion = sqrt(4.5 * promLogE);
    }

    printf("Limite silencio 2*sqrt(prom(logE)): %.15lf \n", limSilencio);
    printf("Mejor limite silencio? 3*sqrt(prom(logE)): %.15lf \n", -1 * 3 * sqrt(promLogE));
    printf("Limite respiración sqrt(4.5*prom(logE)): %.15lf \n", limRespiracion);

    strcpy(RT[1].tipo, "Ed");
    RT[1].threshold = limSilencio;
    strcpy(RT[2].tipo, "Er");
    RT[2].threshold = limRespiracion;

    printf("Promedio Ink: %.15lf \n", promLogI);

    strcpy(RT[3].tipo, "I ");
    RT[3].threshold = lim_author ? promLogI : 2 * promLogI;


}

int main(void) {

    IntervalsStruct RINTERV[NUMPHONEMS];

    LeerClasificacionManual(RINTERV);
    CorregirIntervalos(RINTERV);
    EscribirClasificacionManual(RINTERV);

    FramesLogClassifStruct RFMANUALCLASSIF[FRAMES];

    SepararIntervalos(RINTERV, RFMANUALCLASSIF);
    EscribirClasificacionFrames(RFMANUALCLASSIF);

    //Clasificacion Energia
    ThresholdsStruct RLIMS[NUMTHRESHOLDS];
    ThresholdsStruct RPROMS[2];
    FramesLogClassifStruct RENERGYLOG[FRAMES];


    leerPromedios(RPROMS);

//    printf("\n%.15lf %.15lf\n", RPROMS[0].threshold, RPROMS[1].threshold);
    bool lim_author = true;

    establecerLimites(RLIMS, RPROMS[0].threshold, RPROMS[1].threshold, lim_author);

    leerFicheroLog(RENERGYLOG, 0); //0 Energia

    ClasificarVUS(RLIMS, RENERGYLOG, 0); //0 Energia

    FramesLogClassifStruct RINKLOG[FRAMES];

//    ResultadosClassEnergia(RENERGYLOG, RFMANUALCLASSIF, RCERT, 0); //0 No usar Frames de Certidumbre

    //Clasificacion Ink

    leerFicheroLog(RINKLOG, 1); //1 Ink

    ClasificarVUS(RLIMS, RINKLOG, 1); //1 Ink

    ResultadosClassInk(RINKLOG, RFMANUALCLASSIF);

    int cert = 0;

    //Clasificación Frames con/sin incertidumbre
    FramesLogClassifStruct RCERT[FRAMES];
    EncontrarFramesCertidumbre(RENERGYLOG, RINKLOG, RCERT);
    ResultadosClassEnergia(RENERGYLOG, RFMANUALCLASSIF, RCERT, cert); //1 Considerando Solo Certidumbre

    char energy_classif_int[20] = "energy_class_int.dat";
    char ink_classif_int[20] = "ink_class_int.dat";
    char manual_classif_int[20] = "manual_class_int.dat";

    EscribirClasificacionFramesNum(RFMANUALCLASSIF, manual_classif_int, RCERT, cert);
    EscribirClasificacionFramesNum(RENERGYLOG, energy_classif_int, RCERT, cert);
    EscribirClasificacionFramesNum(RINKLOG, ink_classif_int, RCERT, cert);

    //Clasificación Frames con Error
    FramesLogClassifStruct RERROR[FRAMES];
    EncontrarFramesError(RENERGYLOG, RFMANUALCLASSIF, RERROR);

    EscribirErrorEnergyFrames(RLIMS,RERROR, RCERT, cert);


    return 0;
}