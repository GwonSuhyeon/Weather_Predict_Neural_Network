


#include "BaseLayer.h"


class OutputLayer : public BaseLayer
{
public:
	OutputLayer( E_LayerType layerType, int row, int column, string structureName );
	~OutputLayer();


	virtual bool		BL_Init_ChildLayer( int row, int column );

	virtual void		BL_Print_Layer_Info();

	virtual bool		BL_Set_Neuron();
	virtual bool		BL_Update_Neuron( int neuronNumber, float updateValue );
	virtual int			BL_Get_NeuronSize();

	virtual bool		BL_Set_Weight( int size );
	virtual float		BL_Get_Weight( int weightIndex );
	virtual int			BL_Get_WeightSize();

	virtual bool		BL_Set_Bias( int size );
	virtual float		BL_Get_Bias( int biasIndex );
	virtual int			BL_Get_BiasSize();

	virtual float		BL_Get_GradientWeight( int gradientIndex );
	virtual bool		BL_Update_GradientWeight( int currentNeuronIndex, int weightIndex, float error, float upStreamGradient, int batchSize );
	virtual bool		BL_Clear_GradientWeight();

	virtual float		BL_Get_GradientBias( int gradientIndex );
	virtual bool		BL_Update_GradientBias( int biasIndex, float error, float upStreamGradient, int batchSize );
	virtual bool		BL_Clear_GradientBias();

	virtual bool		BL_UpdatE_UpStreamGradient( int neuronIndex, float updateValue );
	virtual float		BL_Get_UpStreamGradient( int neuronIndex );
	virtual bool		BL_Clear_UpStreamGradient();

	virtual bool		BL_Weight_Optimizing( int weightIndex, float optimizingValue );
	virtual bool		BL_Bias_Optimizing( int biasIndex, float optimizingValue );

	virtual float		BL_Get_Predicted_Value();

	E_Exception			OL_Get_BaseLayer_Exception_Report();
	E_Exception			OL_Get_Exception_Report();

	virtual void		BL_Save_LearningData();



private:

	E_Exception		m_exceptionOccurReport;

	int				m_row;
	int				m_column;

	float			*m_neuron		= nullptr;	// 이전 레이어의 뉴런값과 가중치가 곱해진 값을 저장하는 뉴런
	int				m_neuronSize;

	float			*m_upStreamGradient = nullptr;
};