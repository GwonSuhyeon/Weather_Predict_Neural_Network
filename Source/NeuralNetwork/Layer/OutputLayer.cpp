

#include "OutputLayer.h"


OutputLayer::OutputLayer( E_LayerType layerType, int row, int column, string structureName ) :BaseLayer( layerType, E_Invalid_ActivationType, structureName )
{
	bool initResult;

	initResult = BL_Init_ChildLayer( row, column );
	if( initResult == false )
	{
		m_exceptionOccurReport = E_EXCEPTION_OCCUR;
	}
}


OutputLayer::~OutputLayer()
{
	if( m_neuron != nullptr )
	{
		delete[ ] m_neuron;
#ifdef _DEBUG
		cout << BL_Get_BaseLayer_Name() << " - neuron 반환 성공" << endl;
#endif
	}
}


E_Exception OutputLayer::OL_Get_BaseLayer_Exception_Report()
{
	return BaseLayer::BL_Get_Exception_Report();
}


E_Exception OutputLayer::OL_Get_Exception_Report()
{
	return this->m_exceptionOccurReport;
}


void OutputLayer::BL_Print_Layer_Info()
{
	cout << "Layer Type : Output(" << BL_Get_BaseLayer_Type() << ")" << endl;
	cout << "Row : " << this->m_row << endl << "Column : " << this->m_column << endl;
	cout << "---------------------------------" << endl << endl;

	return;
}


bool OutputLayer::BL_Init_ChildLayer( int row, int column )
{
	if( row <= 0 || column <= 0 )
	{
		::gLog_LogFile.LS_AddLog( _T( "Row or column is invalid value input in layer init" ) );

		return false;
	}


	this->m_exceptionOccurReport	= E_EXCEPTION_NOT_OCCUR;
	this->m_row						= row;
	this->m_column					= column;
	this->m_neuronSize				= row * column;


	return true;
}


bool OutputLayer::BL_Set_Neuron()
{
	m_neuron = new float[ this->m_neuronSize ];
	if( m_neuron == nullptr )
	{
		// 메모리 동적 할당 실패
		return false;
	}

#ifdef _DEBUG
	cout << BL_Get_BaseLayer_Name() << " - neuron 할당 성공(" << this->m_neuronSize << ")" << endl << endl;
#endif

	return true;
}


bool OutputLayer::BL_Update_Neuron( int neuronNumber, float updateValue )
{
	*( this->m_neuron + neuronNumber ) = updateValue;

	return true;
}


int OutputLayer::BL_Get_NeuronSize()
{
	return this->m_neuronSize;
}


bool OutputLayer::BL_Set_Weight( int size )
{
	this->m_upStreamGradient = new float[ size ] {};
	if( this->m_upStreamGradient == nullptr )
	{
		return false;
	}

	for( int i = 0; i < this->m_neuronSize; i++ )
	{
		*( this->m_upStreamGradient + i ) = 1;
	}
	

	return true;
}


float OutputLayer::BL_Get_GradientWeight( int gradientIndex )
{
	return (float)65535;
}


bool OutputLayer::BL_Update_GradientWeight( int currentNeuronIndex, int weightIndex, float error, float upStreamGradient, int batchSize )
{
	return false;
}


float OutputLayer::BL_Get_Weight( int weightIndex )
{
	return (float)65535;
}


int OutputLayer::BL_Get_WeightSize()
{
	return -1;
}


bool OutputLayer::BL_Set_Bias( int size )
{
	return false;
}


float OutputLayer::BL_Get_Bias( int biasIndex )
{
	return (float)65535;
}


int OutputLayer::BL_Get_BiasSize()
{
	return -1;
}


bool OutputLayer::BL_Clear_GradientWeight()
{
	return false;
}


float OutputLayer::BL_Get_GradientBias( int gradientIndex )
{
	return (float)65535;
}


bool OutputLayer::BL_Update_GradientBias( int biasIndex, float error, float upStreamGradient, int batchSize )
{
	return false;
}


bool OutputLayer::BL_Clear_GradientBias()
{
	return false;
}


bool OutputLayer::BL_UpdatE_UpStreamGradient( int neuronIndex, float updateValue )
{
	return false;
}


float OutputLayer::BL_Get_UpStreamGradient( int neuronIndex )
{
	return *( this->m_upStreamGradient + neuronIndex );
}


bool OutputLayer::BL_Clear_UpStreamGradient()
{
	return false;
}


float OutputLayer::BL_Get_Predicted_Value()
{
	return *this->m_neuron;
}


bool OutputLayer::BL_Weight_Optimizing( int weightIndex, float optimizingValue )
{
	return false;
}


bool OutputLayer::BL_Bias_Optimizing( int biasIndex, float optimizingValue )
{
	return false;
}


void OutputLayer::BL_Save_LearningData()
{
	return;
}