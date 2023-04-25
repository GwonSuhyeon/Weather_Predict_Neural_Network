

#include "MeanSquaredError.h"


MeanSquaredError::MeanSquaredError()
{
	BLF_Init();
}


MeanSquaredError::~MeanSquaredError()
{

}


void MeanSquaredError::BLF_Init()
{
	this->m_error		= 0;
	this->m_totalError	= 0;
	this->m_loss		= 0;
}


bool MeanSquaredError::BLF_MSE_Error_Compute( float targetValue, float predictedValue )
{
	// cmath의 pow 함수를 써야 될까?

	//this->m_error		=	targetValue - predictedValue;
	this->m_error = ROUND( targetValue ) - ROUND( predictedValue );

	//this->m_totalError	+=	( this->m_error * this->m_error );
	this->m_totalError += ( ROUND( ROUND( this->m_error ) * ROUND( this->m_error ) ) );



	return true;
}


bool MeanSquaredError::BLF_MSE_Loss_Compute( int BatchSize )
{
	if( BatchSize <= 0 )
	{
		return false;
	}

	//this->m_loss = this->m_totalError / BatchSize;
	this->m_loss = ROUND( ROUND( this->m_totalError ) / BatchSize );

	return true;
}


float MeanSquaredError::BLF_MSE_Get_Error()
{
	return this->m_error;
}


float MeanSquaredError::BLF_MSE_Get_Loss()
{
	return this->m_loss;
}


bool MeanSquaredError::BLF_MSE_Clear_Error()
{
	this->m_totalError = 0;

	return true;
}