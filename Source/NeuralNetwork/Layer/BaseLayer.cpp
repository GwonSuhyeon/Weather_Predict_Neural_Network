


#include "BaseLayer.h"



BaseLayer::BaseLayer( E_LayerType layerType, E_ActivationType activationType, string structureName )
{
	bool initResult;

	initResult = BL_Init_BaseLayer( layerType, structureName );
	if( initResult == false )
	{
		this->m_exception_report = E_EXCEPTION_OCCUR;
	}

	// OutputLayer는 활성화 함수가 없기 때문에 activationType이 E_Invalid_ActivationType으로 들어온다.
	// 따라서, OutputLayer가 아닐때만 실행함.
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
		// 예외를 예외처리 객체로 넘겨주면 다른 곳에서 그 예외처리 객체를 통해서 예외발생 여부를 확인한다.
	}

#ifdef _DEBUG
	cout << "BaseLayer - " << m_layerName << " 반환 성공" << endl << endl;
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
		// 활성화 함수가 이미 초기화됨.
		// Init함수를 다른 곳에서도 호출해서 중복호출이 되었다는 의미임.
		return false;
	}


	switch( activationType )
	{
	case E_ReLU:
	{
		m_activationFunction = new ReLU();	// 활성화 함수 클래스를 ActivationFunctino 클래스로 업캐스팅 함. 객체 소멸 시 다운캐스팅 필요함.
		break;
	}


	default:
		break;
	}

	if( m_activationFunction == nullptr )
	{
		// 활성화 함수 생성 실패
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
		cout << "활성화 함수 반환 성공" << endl;
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