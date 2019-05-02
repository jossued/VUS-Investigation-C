//
// Created by jossue on 2/05/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NUMTHRESHOLDS 5
#define ELEMENTS 130560
const int FRAMES = ELEMENTS / 160;

#define FICEHROLIM  "limites.dat"

typedef struct _thresholds {
    char tipo[2];
    double threshold;
} ThresholdsStruct;

typedef struct _frameclassif {
    int frame;
    char clasificacion;
} FramesClassifStruct;

int main(void) {

    ThresholdsStruct RLIMS[NUMTHRESHOLDS];
/*  //Clasificacion Energia
    LeerLimites(RINTERV);

    CorregirIntervalos(RINTERV);

    EscribirClasificacionManual(RINTERV);

    FramesClassifStruct RFCLASSIF[FRAMES];

    SepararIntervalos(RINTERV, RFCLASSIF);

    EscribirClasificacionFrames(RFCLASSIF);
    */

    return 0;
}