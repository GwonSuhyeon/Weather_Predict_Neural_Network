#ifndef		__MODELSTRUCTURE_H__
#define		__MODELSTRUCTURE_H__

#include "../../LogSystem/LogSystem.h"

#include "../ActivationFunction/BaseActivationFunction.h"
#include "../ActivationFunction/ReLU.h"

#include "../../Main/CommonDefine.h"

#include <iostream>
#include <random>
#include <string>
#include <cmath>


#define		INIT_TO_NEURON		1
#define		INIT_TO_WEIGHT		2

#define		RANDOM_MEAN						0
#define		RANDOM_STANDARD_DEVIATION		0.1
/*

가중치를 표준정규분포 형태로 초기화 시, 분산을 1로 잡으면 히든레이어들의 뉴런 개수를 늘렸을 때, 에러 발생 함.

분산을 0.5로 잡으면 뉴런 개수 20개 까지는 연산이 가능함을 확인함.
분산을 0.1로 잡으면 뉴런 개수 100개까지는 연산이 가능함을 확인함.

에러의 원인으로 뉴런 값 * 가중치 값의 합계인 neuronSum 변수의 오버플로우로 예상이 되지만, 아직 정확하게는 파악하지 못함.

*/

#define		WEIGHT_INITIAL_VALUE	randomCreator( mersenneTwister )
//#define		WEIGHT_INITIAL_VALUE	1.20


using namespace		std;


enum E_Exception
{
	E_EXCEPTION_OCCUR			= 0,
	E_EXCEPTION_NOT_OCCUR		= 1
};

enum E_LayerType
{
	E_Input			= 0,
	E_Output		= 1,
	E_Dense			= 2
};


enum E_UpStreamGradientOccur
{
	E_COMPUTED = 0,
	E_NOT_COMPUTED = 1
};


class BaseLayer
{
public:
	BaseLayer( E_LayerType layerType, E_ActivationType activationType, string structureName );
	//~ModelStructure() {}
	virtual ~BaseLayer();


	E_Exception					BL_Get_Exception_Report();

	bool						BL_Init_BaseLayer( E_LayerType layerType, string structureName );
	bool						BL_Init_Activation_Function( E_ActivationType activationType );

	string						BL_Get_BaseLayer_Name();
	E_LayerType					BL_Get_BaseLayer_Type();

	void						BL_Print_BaseLayer_Info();
	
	bool						BL_Delete_Activation_Function();
	BaseActivationFunction*		BL_Get_Activation_Function();


#ifdef _DEBUG
	virtual void				BL_func();
#endif

	virtual bool		BL_Init_ChildLayer( int row, int column )					= 0;

	virtual bool		BL_Set_Neuron()												= 0;
	virtual bool		BL_Update_Neuron( int neuronNumber, float updateValue )		= 0;
	virtual int			BL_Get_NeuronSize()											= 0;

	virtual bool		BL_Set_Weight( int size )									= 0;
	virtual float		BL_Get_Weight( int weightIndex )							= 0;
	virtual int			BL_Get_WeightSize()											= 0;
	
	virtual bool		BL_Set_Bias( int size )										= 0;
	virtual float		BL_Get_Bias( int biasIndex )								= 0;
	virtual int			BL_Get_BiasSize()											= 0;
	
	virtual bool		BL_ForewardPass( float *trainData, BaseLayer *nextLayer );	// InputLayer의 순전파를 위한 함수 오버로드
	virtual bool		BL_ForewardPass( BaseLayer *nextLayer );					// DensetLayer의 순전파를 위한 함수 오버로드	
	
	virtual bool		BL_BackwardPass( int previousNeuronSize, float *upStreamBundle, float error, int batchSize, E_UpStreamGradientOccur computeOccur );

	virtual float		BL_Get_GradientWeight( int gradientIndex )																										= 0;
	virtual bool		BL_Update_GradientWeight( int currentNeuronIndex, int weightIndex, float error, float upStreamGradient, int batchSize )		= 0;
	virtual bool		BL_Clear_GradientWeight()																									= 0;

	virtual float		BL_Get_GradientBias( int gradientIndex )														= 0;
	virtual bool		BL_Update_GradientBias( int biasIndex, float error, float upStreamGradient, int batchSize )		= 0;
	virtual bool		BL_Clear_GradientBias()																			= 0;

	virtual bool		BL_UpdatE_UpStreamGradient( int neuronIndex, float updateValue )	= 0;
	virtual float		BL_Get_UpStreamGradient( int neuronIndex )							= 0;
	virtual bool		BL_Clear_UpStreamGradient()											= 0;

	virtual bool		BL_Weight_Optimizing( int weightIndex, float optimizingValue )	= 0;
	virtual bool		BL_Bias_Optimizing( int biasIndex, float optimizingValue )		= 0;

	virtual	void		BL_Print_Layer_Info()												= 0;
	virtual float		BL_Get_Predicted_Value();

	virtual void		BL_Save_LearningData() = 0;


private:

	E_Exception					m_exception_report;

	string						m_layerName;
	E_LayerType					m_layertype;

	BaseActivationFunction		*m_activationFunction;
	E_ActivationType			m_Type_ActivationFunction;

};



#endif