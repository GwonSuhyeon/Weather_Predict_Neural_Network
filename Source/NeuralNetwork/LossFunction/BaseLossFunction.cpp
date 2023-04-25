

#include "BaseLossFunction.h"


BaseLossFunction::BaseLossFunction()
{

}


BaseLossFunction::~BaseLossFunction()
{

}


bool BaseLossFunction::BLF_MSE_Error_Compute( float targetValue, float predictedValue )
{
	return false;
}


bool BaseLossFunction::BLF_MSE_Loss_Compute( int BatchSize )
{
	return false;
}


float BaseLossFunction::BLF_MSE_Get_Error()
{
	return (float)INVALID_RETURN_VALUE;
}


float BaseLossFunction::BLF_MSE_Get_Loss()
{
	return (float)INVALID_RETURN_VALUE;
}


bool BaseLossFunction::BLF_MSE_Clear_Error()
{
	return false;
}