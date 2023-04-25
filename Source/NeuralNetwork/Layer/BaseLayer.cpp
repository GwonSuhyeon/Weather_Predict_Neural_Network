


#include "BaseLayer.h"



BaseLayer::BaseLayer( E_LayerType layerType, E_ActivationType activationType, string structureName )
{
	bool initResult;

	initResult = BL_Init_BaseLayer( layerType, structureName );
	if( initResult == false )
	{
		this->m_exception_report = E_EXCEPTION_OCCUR;
	}

	// OutputLayer�� Ȱ��ȭ �Լ��� ���� ������ activationType�� E_Invalid_ActivationType���� ���´�.
	// ����, OutputLayer�� �ƴҶ��� ������.
	if( activationType != E_Invalid_ActivationType )
	{
		initResult = BL_Init_Activation_Function( activationType );
		if( initResult == false )
		{
			this->m_exception_report = E_EXCEPTION_OCCUR;
		}
	}
}


BaseLayer::~BaseLayer()
{
	bool deleteResult;

	deleteResult = BL_Delete_Activation_Function();
	if( deleteResult == false )
	{
		// ���ܸ� ����ó�� ��ü�� �Ѱ��ָ� �ٸ� ������ �� ����ó�� ��ü�� ���ؼ� ���ܹ߻� ���θ� Ȯ���Ѵ�.
	}

#ifdef _DEBUG
	cout << "BaseLayer - " << m_layerName << " ��ȯ ����" << endl << endl;
#endif
}


E_Exception BaseLayer::BL_Get_Exception_Report()
{
	return this->m_exception_report;
}


bool BaseLayer::BL_Init_BaseLayer( E_LayerType layerType , string structureName )
{
	if( structureName.empty() == true )
	{
		return false;
	}

	this->m_exception_report = E_EXCEPTION_NOT_OCCUR;
	this->m_layertype = layerType;
	this->m_layerName = structureName;

	return true;
}


bool BaseLayer::BL_Init_Activation_Function( E_ActivationType activationType )
{
	if( m_activationFunction != nullptr )
	{
		// Ȱ��ȭ �Լ��� �̹� �ʱ�ȭ��.
		// Init�Լ��� �ٸ� �������� ȣ���ؼ� �ߺ�ȣ���� �Ǿ��ٴ� �ǹ���.
		return false;
	}


	switch( activationType )
	{
	case E_ReLU:
	{
		m_activationFunction = new ReLU();	// Ȱ��ȭ �Լ� Ŭ������ ActivationFunctino Ŭ������ ��ĳ���� ��. ��ü �Ҹ� �� �ٿ�ĳ���� �ʿ���.
		break;
	}


	default:
		break;
	}

	if( m_activationFunction == nullptr )
	{
		// Ȱ��ȭ �Լ� ���� ����
		return false;
	}

	this->m_Type_ActivationFunction = activationType;

	return true;
}


bool BaseLayer::BL_Delete_Activation_Function()
{
	if( this->m_activationFunction == nullptr )
	{
		return false;
	}

	switch( this->m_Type_ActivationFunction )
	{
	case E_ReLU:
	{
		ReLU *relu = (ReLU*)this->m_activationFunction;
		delete relu;
		cout << "Ȱ��ȭ �Լ� ��ȯ ����" << endl;
		break;
	}


	default:
		break;
	}


	return true;
}


BaseActivationFunction* BaseLayer::BL_Get_Activation_Function()
{
	return this->m_activationFunction;
}


void BaseLayer::BL_Print_BaseLayer_Info()
{
	cout << "---------------------------------" << endl;
	cout << "Layer Name : " << this->m_layerName << endl;

	return;
}


string BaseLayer::BL_Get_BaseLayer_Name()
{
	return this->m_layerName;
}


E_LayerType BaseLayer::BL_Get_BaseLayer_Type()
{
	return this->m_layertype;
}


#ifdef _DEBUG
void BaseLayer::BL_func()
{
	return;
}
#endif


float BaseLayer::BL_Get_Predicted_Value()
{
	return (float)99.0;
}


bool BaseLayer::BL_ForewardPass( float *trainData, BaseLayer *nextLayer )
{
	return false;
}


bool BaseLayer::BL_ForewardPass( BaseLayer *nextLayer )
{
	return false;
}


bool BaseLayer::BL_BackwardPass( int previousNeuronSize, float *upStreamBundle, float error, int batchSize, E_UpStreamGradientOccur computeOccur )
{
	return false;
}