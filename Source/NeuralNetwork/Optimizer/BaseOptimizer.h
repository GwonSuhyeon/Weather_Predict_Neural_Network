#ifndef __BASEOPTIMIZER_H__
#define __BASEOPTIMIZER_H__


#include "../../Main/CommonDefine.h"

#include <cmath>


#define		INVALID_RETURN_VALUE	65535


enum E_Optimizer
{
	/*
	* Batch ��� �ܾ�� �н������� ��ü�� �ǹ��Ѵ�.
	* Mini-Batch ��� �ܾ�� Batch�� batch size��ŭ�� ���� ������ ������ �ǹ��Ѵ�.
	* ����, batch size�� Mini-Batch�� ũ���̸�, Batch / batch size�� mini-Batch�� �����̴�.
	*/

	E_BGD = 0,	// Batch Gradient Descent
	E_SGD = 1,	// Stochastic Gradient Descent
	E_MGD = 2	// Mini-Batch Gradient Descent
};


class BaseOptimizer
{
public:
	BaseOptimizer();
	~BaseOptimizer();


	virtual void	BO_Init() = 0;

	virtual float	BO_Compute_Optimizing_Value( float gradientWeight, float learningRate )	= 0;


private:



};





#endif