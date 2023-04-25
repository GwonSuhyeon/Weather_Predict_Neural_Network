

#include "MiniBatchGradientDescent.h"


MiniBatchGradientDescent::MiniBatchGradientDescent()
{

}


MiniBatchGradientDescent::~MiniBatchGradientDescent()
{

}


void MiniBatchGradientDescent::BO_Init()
{
	return;
}


float MiniBatchGradientDescent::BO_Compute_Optimizing_Value( float gradientValue, float learningRate )
{
	//return ( -1 * gradientValue * learningRate );
	return ROUND( ( -1 * ROUND( gradientValue ) * ROUND( learningRate ) ) );
}