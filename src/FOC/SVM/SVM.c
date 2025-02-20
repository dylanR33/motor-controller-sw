#include "SVM.h"

#include "Transform.h"

#define SQRT3_2 0.866025404

static const uint8_t sextantLUT[] = { 6, 2, 1, 4, 5, 3 };


void SVM_ModifiedClarke( AlphaBeta* alphaBeta, IJK* ijk )
{
    ijk->i =  ( SQRT3_2 * alphaBeta->alpha ) - ( 0.5 * alphaBeta->beta );
    ijk->j =  alphaBeta->beta;
    ijk->k = -( SQRT3_2 * alphaBeta->alpha ) - ( 0.5 * alphaBeta->beta );
}


Sextant SVM_CalculateSextant( IJK* ijk )
{
    uint8_t iSign, jSign, kSign;

    // Get signs of i,j,k ( + = 1, - = 0 )
    iSign = ( ijk->i > 0 )? 1 : 0;
    jSign = ( ijk->j > 0 )? 1 : 0;
    kSign = ( ijk->k > 0 )? 1 : 0;

    // Find element n of sextantLUT
    uint8_t n = iSign + ( 2 * jSign ) + ( 4 * kSign );

    // Return element n ( index = n - 1 )
    return sextantLUT[ n - 1 ];
}


static uint32_t AreOnTimesNegative( OnTimeT* T )
{
    return ( T->Tx < 0 || T->Ty < 0 || T->T0 < 0 );
}


void SVM_DetermineOnTimeT( OnTimeT* T, IJK* ijk, Sextant nSextant )
{
    switch ( nSextant )
    {
        case SEXTANT_1:
            T->Tx = ijk->i;
            T->Ty = ijk->j;
            break;
        case SEXTANT_2:
            T->Tx = -ijk->k;
            T->Ty = -ijk->i;
            break;
        case SEXTANT_3:
            T->Tx = ijk->j;
            T->Ty = ijk->k;
            break;
        case SEXTANT_4:
            T->Tx = -ijk->i;
            T->Ty = -ijk->j;
            break;
        case SEXTANT_5:
            T->Tx = ijk->k;
            T->Ty = ijk->i;
            break;
        case SEXTANT_6:
            T->Tx = -ijk->j;
            T->Ty = -ijk->k;
            break;
        default:
            return;
    }
    T->T0 = 1 - ( T->Tx + T->Ty );

    if ( AreOnTimesNegative( T ) )
    {
        T->Tx = 0;
        T->Ty = 0;
        T->T0 = 0;
    }
}


void SVM_DeterminePulseWidths( PulseWidths* uvw, OnTimeT* T, Sextant nSextant )
{
    switch ( nSextant )
    {
        case SEXTANT_1:
            uvw->u = T->Tx + T->Ty + 0.5*( T->T0 );
            uvw->v = T->Ty + 0.5*( T->T0 );
            uvw->w = 0.5*( T->T0 );
            break;
        case SEXTANT_2:
            uvw->u = T->Tx + 0.5*( T->T0 );
            uvw->v = T->Tx + T->Ty + 0.5*( T->T0 );
            uvw->w = 0.5*( T->T0 );
            break;
        case SEXTANT_3:
            uvw->u = 0.5*( T->T0 );
            uvw->v = T->Tx + T->Ty + 0.5*( T->T0 );
            uvw->w = T->Ty + 0.5*( T->T0 );
            break;
        case SEXTANT_4:
            uvw->u = 0.5*( T->T0 );
            uvw->v = T->Tx + 0.5*( T->T0 );
            uvw->w = T->Tx + T->Ty + 0.5*( T->T0 );
            break;
        case SEXTANT_5:
            uvw->u = T->Ty + 0.5*( T->T0 );
            uvw->v = 0.5*( T->T0 );
            uvw->w = T->Tx + T->Ty + 0.5*( T->T0 );
            break;
        case SEXTANT_6:
            uvw->u = T->Tx + T->Ty + 0.5*( T->T0 );
            uvw->v = 0.5*( T->T0 );
            uvw->w = T->Tx + 0.5*( T->T0 );
            break;
        default:
            return;
    }
}
