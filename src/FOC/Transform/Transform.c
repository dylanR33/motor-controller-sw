#include "Transform.h"

#include "Wave.h"


#define ONE_SQRT3 0.577350269
#define TWO_SQRT3 1.154700538
#define SQRT3_TWO 0.866025404


void Transform_Clarke( PhaseVariables* abc, AlphaBeta* alphaBeta )
{
    alphaBeta->alpha = abc->a;
    alphaBeta->beta = ( ONE_SQRT3 * abc->a ) + ( TWO_SQRT3 * abc->b );
}


void Transform_InverseClarke( AlphaBeta* alphaBeta, PhaseVariables* abc )
{
    abc->a = alphaBeta->alpha;
    abc->b = ( -0.5 * alphaBeta->alpha ) + ( SQRT3_TWO * alphaBeta->beta );
    abc->c = ( -0.5 * alphaBeta->alpha ) - ( SQRT3_TWO * alphaBeta->beta );
}

void Transform_Park( AlphaBeta* alphaBeta, DirectQuadrature* dq, float theta )
{
    dq->d = (  alphaBeta->alpha * Wave_Cos( theta ) ) + ( alphaBeta->beta * Wave_Sin( theta ) );
    dq->q = ( -alphaBeta->alpha * Wave_Sin( theta ) ) + ( alphaBeta->beta * Wave_Cos( theta ) );
}


void Transform_InversePark( DirectQuadrature* dq, AlphaBeta* alphaBeta, float theta )
{
    alphaBeta->alpha = ( dq->d * Wave_Cos( theta ) ) - ( dq->q * Wave_Sin( theta ) );
    alphaBeta->beta  = ( dq->d * Wave_Sin( theta ) ) + ( dq->q * Wave_Cos( theta ) );
}

