

#include "ReLU.h"



ReLU::ReLU()
{

}


ReLU::~ReLU()
{

}


float ReLU::BAF_Activation_To_Neuron( float x )
{
	float returnValue;

	if( x > 0 )
		returnValue = x;
	else if( x <= 0 )
		returnValue = 0;

	return returnValue;
}