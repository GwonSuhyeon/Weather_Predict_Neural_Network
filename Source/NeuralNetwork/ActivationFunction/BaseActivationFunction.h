

#ifndef __ACTIVATIONFUNCTION_H__
#define __ACTIVATIONFUNCTION_H__


#define		INVALID_RETURN_VALUE	65535


enum E_ActivationType
{
	E_ReLU						= 0,

	E_Invalid_ActivationType	= 99
};


class BaseActivationFunction
{
public:
	BaseActivationFunction();
	~BaseActivationFunction();


	virtual float	BAF_Activation_To_Neuron( float x );


private:



};





#endif