



#include "BaseLayer.h"



class DenseLayer : public BaseLayer
{
public:
	DenseLayer( E_LayerType layerType, int row, int column, E_ActivationType activationType, string structureName );
	~DenseLayer();
	

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

	virtual bool		BL_ForewardPass( BaseLayer *nextLayer );	// ��� ���� �����Լ��� �������̵�

	virtual bool		BL_BackwardPass( int previousNeuronSize, float *upStreamBundle, float error, int batchSize, E_UpStreamGradientOccur computeOccur );

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

	E_Exception			DL_Get_BaseLayer_Exception_Report();
	E_Exception			DL_Get_Exception_Report();

	virtual void		BL_Save_LearningData();



private:

	E_Exception		m_exceptionOccurReport;	// ���� �߻� ���θ� üũ�ϱ� ���� ����

	int				m_row;
	int				m_column;

	float			*m_neuron			= nullptr;	// ���� ���̾��� �������� ����ġ�� ������ ���� �����ϴ� ����
	int				m_neuronSize;

	float			*m_weight			= nullptr;	// ����ġ
	float			*m_gradientWeight	= nullptr;	// ����ġ Gradient
	int				m_weightSize;

	float			*m_bias				= nullptr;
	float			*m_gradientBias		= nullptr;
	int				m_biasSize;

	float			*m_upStreamGradient = nullptr;

};