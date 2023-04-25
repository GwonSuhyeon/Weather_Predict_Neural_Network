

#include "BaseLossFunction.h"


class MeanSquaredError : public BaseLossFunction
{
public:
	MeanSquaredError();
	~MeanSquaredError();


	virtual void	BLF_Init();

	virtual bool	BLF_MSE_Error_Compute( float targetValue, float predictedValue );
	virtual bool	BLF_MSE_Loss_Compute( int BatchSize );

	virtual float	BLF_MSE_Get_Error();
	virtual float	BLF_MSE_Get_Loss();

	virtual bool	BLF_MSE_Clear_Error();


private:

	float	m_error;
	float	m_totalError;
	float	m_loss;

};