

#include "BaseActivationFunction.h"


class ReLU : public BaseActivationFunction
{
public:
	ReLU();
	~ReLU();


	virtual float	BAF_Activation_To_Neuron( float x );



private:



};