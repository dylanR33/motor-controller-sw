#include "PIController.h"

float PIController_Control( float setPoint, PIController* pi )
{
    // Calculate error
    float error = setPoint - pi->processVar;

    // Anti-Windup
    float antiWindup = pi->awGain * ( pi->prevCmdSat - pi->prevCmdUnsat ) * pi->T; 

    // Calculate curent integral then carry it
    pi->integral += ( pi->iGain * error * pi->T ) + antiWindup;

    // Raw command out
    float command = ( pi->pGain * error ) + pi->integral;

    // Save command prior to stauration
    pi->prevCmdUnsat = command;

    // Saturation checks
    float commandSat;
    if      ( command > pi->max )    commandSat = pi->max;
    else if ( command < pi->min )    commandSat = pi->min;
    else                             commandSat = command;

    // Save command post saturation checks
    pi->prevCmdSat = commandSat;

    return commandSat;
}
