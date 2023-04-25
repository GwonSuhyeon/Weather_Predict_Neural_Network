#ifndef __BASEOPTIMIZER_H__
#define __BASEOPTIMIZER_H__


#include "../../Main/CommonDefine.h"

#include <cmath>


#define		INVALID_RETURN_VALUE	65535


enum E_Optimizer
{
	/*
	* Batch 라는 단어는 학습데이터 전체를 의미한다.
	* Mini-Batch 라는 단어는 Batch를 batch size만큼씩 나눈 데이터 집합을 의미한다.
	* 따라서, batch size는 Mini-Batch의 크기이며, Batch / batch size는 mini-Batch의 개수이다.
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