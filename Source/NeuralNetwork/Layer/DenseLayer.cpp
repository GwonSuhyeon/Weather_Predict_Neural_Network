

#include "DenseLayer.h"


DenseLayer::DenseLayer( E_LayerType layerType, int row, int column, E_ActivationType activationType, string structureName ) :BaseLayer( layerType, activationType, structureName )
{
	bool initResult;

	initResult = BL_Init_ChildLayer( row, column );
	if( initResult == false )
	{
		m_exceptionOccurReport = E_EXCEPTION_OCCUR;
	}
}


DenseLayer::~DenseLayer()
{
	if( this->m_neuron != nullptr )
	{
		delete[ ] this->m_neuron;
#ifdef _DEBUG
		cout << BL_Get_BaseLayer_Name() << " - neuron 반환 성공" << endl;
#endif
	}

	if( this->m_weight != nullptr )
	{
		delete[ ] this->m_weight;
#ifdef _DEBUG
		cout << BL_Get_BaseLayer_Name() << " - weight 반환 성공" << endl;
#endif
	}

	if( this->m_bias != nullptr )
	{
		delete[ ] this->m_bias;
#ifdef _DEBUG
		cout << BL_Get_BaseLayer_Name() << " - bias 반환 성공" << endl;
#endif
	}
}


E_Exception DenseLayer::DL_Get_BaseLayer_Exception_Report()
{
	return BaseLayer::BL_Get_Exception_Report();
}


E_Exception DenseLayer::DL_Get_Exception_Report()
{
	return this->m_exceptionOccurReport;
}


void DenseLayer::BL_Print_Layer_Info()
{
	cout << "Layer Type : Dense(" << BL_Get_BaseLayer_Type() << ")" << endl;
	cout << "Row : " << this->m_row << endl << "Column : " << this->m_column << endl;
	cout << "---------------------------------" << endl << endl;

	return;
}


bool DenseLayer::BL_Init_ChildLayer( int row, int column )
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


bool DenseLayer::BL_Set_Neuron()
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


bool DenseLayer::BL_Update_Neuron( int neuronNumber, float updateValue )
{
	//*( this->m_neuron + neuronNumber ) = updateValue;
	*( this->m_neuron + neuronNumber ) = ROUND( updateValue );


	return true;
}


int DenseLayer::BL_Get_NeuronSize()
{
	return this->m_neuronSize;
}


bool DenseLayer::BL_Set_Weight( int size )
{
	random_device					randomDevice;
	mt19937							mersenneTwister( randomDevice() );
	normal_distribution<float>		randomCreator( RANDOM_MEAN, RANDOM_STANDARD_DEVIATION );


	if( size == NULL )
	{
		return false;
	}


	this->m_weightSize = size;

	this->m_weight = new float[ this->m_weightSize ] {};
	if( this->m_weight == nullptr )
	{
		// 메모리 동적 할당 실패
		return false;
	}

	this->m_gradientWeight = new float[ this->m_weightSize ] {};
	if( this->m_gradientWeight == nullptr )
	{
		// 메모리 할당 실패
		return false;
	}

	this->m_upStreamGradient = new float[ this->m_neuronSize ] {};
	if( this->m_upStreamGradient == nullptr )
	{
		return false;
	}


#ifdef _DEBUG
	cout << BL_Get_BaseLayer_Name() << " - weight 할당 성공(" << this->m_weightSize << ")" << endl << endl;
#endif

	for( int i = 0; i < this->m_weightSize; i++ )
	{
		*( m_weight + i ) = WEIGHT_INITIAL_VALUE;
	}


	
	FILE *fp;
	int nextNeuronSize = 0;
	nextNeuronSize = this->m_weightSize / this->m_neuronSize;

	if( nextNeuronSize > 1 )
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseWeight1_Init.txt", "wt" );

		for( int i = 0; i < this->m_neuronSize; i++ )
		{
			float value = 0;

			for( int k = i; k < this->m_weightSize; k += this->m_neuronSize )
			{
				value = *( this->m_weight + k );
				std::string text = std::to_string( value );

				if( k < ( this->m_weightSize - (this->m_weightSize / nextNeuronSize ) ) )
				{
					text = text + ",";
				}

				fprintf( fp, text.c_str() );
			}

			fprintf( fp, "\n" );
		}
		fclose( fp );
	}
	else
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseWeight2_Init.txt", "wt" );

		float value = 0;

		for( int k = 0; k < this->m_weightSize; k += nextNeuronSize )
		{
			value = *( this->m_weight + k );
			std::string text = std::to_string( value );

			if( k < ( this->m_weightSize - 1 ) )
			{
				text = text + ",";
			}

			fprintf( fp, text.c_str() );
		}

		fprintf( fp, "\n" );

		fclose( fp );
	}
	


	return true;
}


float DenseLayer::BL_Get_GradientWeight( int gradientIndex )
{
	return *( this->m_gradientWeight + gradientIndex );
}


bool DenseLayer::BL_Update_GradientWeight( int currentNeuronIndex, int weightIndex, float error, float upStreamGradient, int batchSize )
{
	float neuronValue;
	float localStreamGradient;
	float gradient;

	
	if( currentNeuronIndex < 0 || currentNeuronIndex >= this->m_neuronSize )
	{
		return false;
	}

	if( weightIndex < 0 || weightIndex >= this->m_weightSize )
	{
		return false;
	}


	//neuronValue				= *( this->m_neuron + currentNeuronIndex );
	neuronValue = ROUND( *( this->m_neuron + currentNeuronIndex ) );

	//localStreamGradient		= -2 * error * neuronValue;
	localStreamGradient = ROUND( -2 * ROUND( error ) * neuronValue );

	//gradient				= ( localStreamGradient * upStreamGradient ) / batchSize;
	gradient = ROUND( ROUND( localStreamGradient * ROUND( upStreamGradient ) ) / batchSize );


	*( this->m_gradientWeight + weightIndex ) += gradient;
	
	return true;
}


float DenseLayer::BL_Get_GradientBias( int gradientIndex )
{
	return *( this->m_gradientBias + gradientIndex );
}


bool DenseLayer::BL_Update_GradientBias( int biasIndex, float error, float upStreamGradient, int batchSize )
{
	float localStreamGradient;
	float gradient;


	//localStreamGradient		= -2 * error;
	localStreamGradient = ROUND( -2 * ROUND( error ) );

	//gradient				= ( localStreamGradient * upStreamGradient ) / batchSize;
	gradient = ROUND( ROUND( ( localStreamGradient * ROUND( upStreamGradient ) ) ) / batchSize );


	*( this->m_gradientBias + biasIndex ) += gradient;

	return true;
}


bool DenseLayer::BL_Clear_GradientBias()
{
	for( int i = 0; i < this->m_biasSize; i++ )
	{
		*( this->m_gradientBias + i ) = 0;
	}


	return true;
}


float DenseLayer::BL_Get_Weight( int weightIndex )
{
	return *( this->m_weight + weightIndex );
}


int DenseLayer::BL_Get_WeightSize()
{
	return this->m_weightSize;
}


bool DenseLayer::BL_Set_Bias( int size )
{
	if( size <= 0 )
	{
		return false;
	}


	this->m_biasSize = size;

	this->m_bias = new float[size] { };
	if( this->m_bias == nullptr )
	{
		return false;
	}

	this->m_gradientBias = new float[size] { };
	if( this->m_gradientBias == nullptr )
	{
		return false;
	}


	return true;
}


float DenseLayer::BL_Get_Bias( int biasIndex )
{
	return *( this->m_bias + biasIndex );
}


int DenseLayer::BL_Get_BiasSize()
{
	return this->m_biasSize;
}


bool DenseLayer::BL_Clear_GradientWeight()
{
	for( int i = 0; i < this->m_weightSize; i++ )
	{
		*( this->m_gradientWeight + i ) = 0;
	}

	return true;
}


bool DenseLayer::BL_UpdatE_UpStreamGradient( int neuronIndex, float updateValue )
{
	if( neuronIndex < 0 || neuronIndex >= this->m_neuronSize )
	{
		return false;
	}

	//*( this->m_upStreamGradient + neuronIndex ) += updateValue;
	*( this->m_upStreamGradient + neuronIndex ) = ROUND( *( this->m_upStreamGradient + neuronIndex ) ) + ROUND( updateValue );


	return true;
}


float DenseLayer::BL_Get_UpStreamGradient( int neuronIndex )
{
	return *( this->m_upStreamGradient + neuronIndex );
}


bool DenseLayer::BL_Clear_UpStreamGradient()
{
	for( int i = 0; i < this->m_neuronSize; i++ )
	{
		*( this->m_upStreamGradient + i ) = 0;
	}

	return true;
}


bool DenseLayer::BL_ForewardPass( BaseLayer *nextLayer )
{
	float		biasValue		= 0;
	float		activatedValue	= 0;
	bool		updateResult;
	
	int			nextLayerType;

	//float		*nextNeuron;
	int			nextNeuronSize;
	float		*sectionWeight;
	int			sectionBoundary;

	float		neuronSum		= 0;
	float		neuronValue		= 0;
	float		weightValue		= 0;

	BaseActivationFunction	*activationFunc;


	if( nextLayer == nullptr )
	{
		// nextLayer를 참조할 수 없음.
		return false;
	}

	nextNeuronSize = nextLayer->BL_Get_NeuronSize();
	if( nextNeuronSize == NULL )
	{
		// 다음 레이어의 뉴런 크기 획득 실패
		return false;
	}


	// 활성화 함수 로드
	activationFunc = BL_Get_Activation_Function();
	if( activationFunc == nullptr )
	{
		return false;
	}

	// 순전파 과정 수행
	for( int neuronNumber = 0; neuronNumber < nextNeuronSize; neuronNumber++ )
	{
		neuronSum		= 0;
		sectionBoundary = ( neuronNumber * this->m_neuronSize );
		sectionWeight	= this->m_weight + sectionBoundary;

		for( int k = 0; k < this->m_neuronSize; k++ )
		{
			neuronValue = *( this->m_neuron + k );
			weightValue = *( sectionWeight + k );

			//neuronSum += neuronValue * weightValue;
			neuronSum += ROUND( ROUND( neuronValue ) * ROUND( weightValue ) );

		}

		biasValue = BL_Get_Bias( neuronNumber );
		//neuronSum += biasValue;
		neuronSum = ROUND( neuronSum ) + ROUND( biasValue );


		nextLayerType = nextLayer->BL_Get_BaseLayer_Type();
		if( nextLayerType == E_Output )
		{
			updateResult = nextLayer->BL_Update_Neuron( neuronNumber, neuronSum );
		}
		else
		{
			//activatedValue = activationFunc->BAF_Activation_To_Neuron( neuronSum );
			activatedValue = ROUND( activationFunc->BAF_Activation_To_Neuron( neuronSum ) );
			updateResult = nextLayer->BL_Update_Neuron( neuronNumber, activatedValue );
		}

		if( updateResult == false )
		{
			return false;
		}
	}


	return true;
}


bool DenseLayer::BL_BackwardPass( int previousNeuronSize, float *upStreamBundle, float error, int batchSize, E_UpStreamGradientOccur computeOccur )
{
	// Weight Gradient 계산
	for( int currentNeuronIndex = 0; currentNeuronIndex < this->m_neuronSize; currentNeuronIndex++ )
	{
		int weightIndex = currentNeuronIndex;

		for( int previousNeuronIndex = 0; previousNeuronIndex < previousNeuronSize; previousNeuronIndex++ )
		{
			float	upStreamGradient;
			bool	updateResult;


			upStreamGradient = *( upStreamBundle + previousNeuronIndex );

			if( computeOccur == E_NOT_COMPUTED )
			{
				float updateStream;
				float currentWeight;

				currentWeight = BL_Get_Weight( weightIndex );
				//updateStream = upStreamGradient * currentWeight;
				updateStream = ROUND( ROUND( upStreamGradient ) * ROUND( currentWeight ) );


				updateResult = BL_UpdatE_UpStreamGradient( currentNeuronIndex, updateStream );
				if( updateResult == false )
				{
					return false;
				}
			}

			updateResult = BL_Update_GradientWeight( currentNeuronIndex, weightIndex, error, upStreamGradient, batchSize );
			if( updateResult == false )
			{
				return false;
			}

			weightIndex += this->m_neuronSize;
		}
	}

	// Bias Gradient 계산
	for( int biasIndex = 0; biasIndex < this->m_biasSize; biasIndex++ )
	{
		float	upStreamGradient;

		bool	updateResult;


		upStreamGradient = *( upStreamBundle + biasIndex );

		updateResult = BL_Update_GradientBias( biasIndex, error, upStreamGradient, batchSize );
		if( updateResult == false )
		{
			return false;
		}
	}


	return true;
}


bool DenseLayer::BL_Weight_Optimizing( int weightIndex, float optimizingValue )
{
	//*( this->m_weight + weightIndex ) += optimizingValue;
	*( this->m_weight + weightIndex ) = ROUND( *( this->m_weight + weightIndex ) ) + ROUND( optimizingValue );


	return true;
}


bool DenseLayer::BL_Bias_Optimizing( int biasIndex, float optimizingValue )
{
	//*( this->m_bias + biasIndex ) += optimizingValue;
	*( this->m_bias + biasIndex ) = ROUND( *( this->m_bias + biasIndex ) ) + ROUND( optimizingValue );


	return true;
}


void DenseLayer::BL_Save_LearningData()
{
	FILE *fp;
	int nextNeuronSize = 0;
	nextNeuronSize = this->m_weightSize / this->m_neuronSize;

	if( nextNeuronSize > 1 )
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseWeight1_Learn.txt", "wt" );

		for( int i = 0; i < this->m_neuronSize; i++ )
		{
			float value = 0;

			for( int k = i; k < this->m_weightSize; k += this->m_neuronSize )
			{
				value = *( this->m_weight + k );
				std::string text = std::to_string( value );

				if( k < ( this->m_weightSize - ( this->m_weightSize / nextNeuronSize ) ) )
				{
					text = text + ",";
				}

				fprintf( fp, text.c_str() );
			}

			fprintf( fp, "\n" );
		}
		fclose( fp );
	}
	else
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseWeight2_Learn.txt", "wt" );

		float value = 0;

		for( int k = 0; k < this->m_weightSize; k += nextNeuronSize )
		{
			value = *( this->m_weight + k );
			std::string text = std::to_string( value );

			if( k < ( this->m_weightSize - 1 ) )
			{
				text = text + ",";
			}

			fprintf( fp, text.c_str() );
		}

		fprintf( fp, "\n" );

		fclose( fp );
	}


	float value = 0;
	if( nextNeuronSize > 1 )
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseBias1_Learn.txt", "wt" );
	}
	else
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DenseBias2_Learn.txt", "wt" );
	}


	for( int k = 0; k < this->m_biasSize; k++ )
	{
		value = *( this->m_bias + k );
		std::string text = std::to_string( value );

		fprintf( fp, text.c_str() );
		fprintf( fp, "\n" );
	}

	fclose( fp );
}