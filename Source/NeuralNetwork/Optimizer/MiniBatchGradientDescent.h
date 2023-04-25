

#include "BaseOptimizer.h"


class MiniBatchGradientDescent : public BaseOptimizer
{
public:
	MiniBatchGradientDescent();
	~MiniBatchGradientDescent();


	virtual void	BO_Init();

	virtual float	BO_Compute_Optimizing_Value( float gradientValue, float learningRate );


private:


};