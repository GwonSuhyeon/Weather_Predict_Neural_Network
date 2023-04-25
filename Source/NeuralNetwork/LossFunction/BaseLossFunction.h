#ifndef __BASELOSSFUNCTION_H__
#define __BASELOSSFUNCTION_H__


#include "../../Main/CommonDefine.h"

#include <cmath>>


#define		INVALID_RETURN_VALUE	65535


enum E_LossFunction
{
	E_MSE = 0		// Mean Squared Error
};


class BaseLossFunction
{
public:
	BaseLossFunction();
	~BaseLossFunction();


	virtual void	BLF_Init()				= 0;

	virtual bool	BLF_MSE_Error_Compute( float targetValue, float predictedValue );
	virtual bool	BLF_MSE_Loss_Compute( int BatchSize );
	virtual float	BLF_MSE_Get_Error();
	virtual float	BLF_MSE_Get_Loss();
	virtual bool	BLF_MSE_Clear_Error();
	

private:



};





#endif