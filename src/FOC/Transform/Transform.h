#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__


#include <stdint.h>


typedef struct
{
    float a;
    float b;
    float c;
} PhaseVariables;


typedef struct
{
    float alpha;
    float beta;
} AlphaBeta;


typedef struct
{
    float d;
    float q;
} DirectQuadrature;

void Transform_Clarke( PhaseVariables* abc, AlphaBeta* alphaBeta );


void Transform_InverseClarke( AlphaBeta* alphaBeta, PhaseVariables* abc );


void Transform_Park( AlphaBeta* alphaBeta, DirectQuadrature* dq, float theta );


void Transform_InversePark( DirectQuadrature* dq, AlphaBeta* alphaBeta, float theta );

#endif
