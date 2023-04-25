

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork()
{
	bool initResult;

	initResult = NN_Init();
	if( initResult == false )
	{
		this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
	}
}


NeuralNetwork::~NeuralNetwork()
{

}


bool NeuralNetwork::NN_Init()
{
	this->m_exceptionOccurReport = E_EXCEPTION_NOT_OCCUR;

	this->m_ComputeOccur		 = E_NOT_COMPUTED;

	return true;
}


bool NeuralNetwork::NN_Init_LossFunction( E_LossFunction lossFunction )
{
	switch( lossFunction )
	{
	case E_MSE:
	{
		this->m_LossFunction = new MeanSquaredError();
	}
	break;


	default:
		break;
	}

	if( this->m_LossFunction == nullptr )
	{
		return false;
	}

	this->m_Type_LossFunction = lossFunction;
	this->m_LossFunction->BLF_Init();


	return true;
}


bool NeuralNetwork::NN_Init_Optimizer( int BatchSize, E_Optimizer optimizer, int trainDataCnt )
{
	if( BatchSize <= 0 || BatchSize > trainDataCnt )
	{
		return false;
	}


	switch( optimizer )
	{
	case E_BGD:
	{
		if( BatchSize != trainDataCnt )
		{
			// BGD�� ��ġ����� �н������� ��ü ũ��� ���;� �Ѵ�.
			::gLog_LogFile.LS_AddLog( _T( "Invalid batch size input in BGD optimizer" ) );

			return false;
		}

		this->m_Optimizer = new MiniBatchGradientDescent();
	}
	break;


	case E_SGD:
	{
		if( BatchSize != 1 )
		{
			// SGD�� ��ġ����� 1�� ���;� �Ѵ�.
			::gLog_LogFile.LS_AddLog( _T( "Invalid batch size input in SGD optimizer" ) );

			return false;
		}

		this->m_Optimizer = new MiniBatchGradientDescent();
	}
	break;


	case E_MGD:
	{
		if( BatchSize <= 1 || BatchSize >= trainDataCnt )
		{
			// MGD�� ��ġ����� 1 ���ٴ� ũ��, �н������� ��ü ũ�⺸�ٴ� �۾ƾ� �Ѵ�.
			::gLog_LogFile.LS_AddLog( _T( "Invalid batch size input in MGD optimizer" ) );

			return false;
		}

		this->m_Optimizer = new MiniBatchGradientDescent();
	}
	break;


	default:
		break;
	}

	if( this->m_Optimizer == nullptr )
	{
		return false;
	}

	this->m_Type_Optimizer = optimizer;
	this->m_Optimizer->BO_Init();


	return true;
}


bool NeuralNetwork::NN_Delete_LossFunction()
{
	if( this->m_LossFunction == nullptr )
	{
		return false;
	}

	switch( this->m_Type_LossFunction )
	{
	case E_MSE:
	{
		MeanSquaredError *mse = (MeanSquaredError*)this->m_LossFunction;
		delete mse;
		std::cout << endl << "�ս� �Լ� ��ȯ ����" << endl;
	}
	break;


	default:
		break;
	}


	return true;
}


bool NeuralNetwork::NN_Delete_Optimizer()
{
	if( this->m_Optimizer == nullptr )
	{
		return false;
	}

	switch( this->m_Type_Optimizer )
	{
	case E_MGD:
	{
		MiniBatchGradientDescent *mgd = (MiniBatchGradientDescent*)this->m_Optimizer;
		delete mgd;
		std::cout << endl << "��Ƽ������ ��ȯ ����" << endl << endl;
	}
	break;


	default:
		break;
	}


	return true;
}


E_Exception NeuralNetwork::NN_Get_Exception_Report()
{
	return this->m_exceptionOccurReport;
}


bool NeuralNetwork::NN_Model_Build()
{
	BaseLayer	*currentLayer;

	int			weightSize;
	int			currentNeuronSize;
	int			nextNeuronSize;

	bool		layerAddResult;
	bool		neuronInitResult;
	bool		weightInitResult;
	bool		biasInitResult;


	layerAddResult = NN_Add_Layer( E_Input, 1, 10, E_ReLU, "My Layer1" );
	if( layerAddResult == false )
	{
		return false;
	}

	layerAddResult = NN_Add_Layer( E_Dense, 1, 10, E_ReLU, "My Layer2" );
	if( layerAddResult == false )
	{
		return false;
	}

	layerAddResult = NN_Add_Layer( E_Dense, 1, 10, E_ReLU, "My Layer3" );
	if( layerAddResult == false )
	{
		return false;
	}

	layerAddResult = NN_Add_Layer( E_Output, 1,	1, E_Invalid_ActivationType, "" );
	if( layerAddResult == false )
	{
		return false;
	}


	if( m_exceptionOccurReport == E_EXCEPTION_OCCUR )
	{
		return false;
	}


	if( (int)Model.size() <= 0 )
	{
		return false;
	}


	for( int i = 0; i < (int)Model.size(); i++ )
	{
		neuronInitResult = Model[ i ]->BL_Set_Neuron();
		if( neuronInitResult == false )
		{
			// ���̾� ���� ���� �ʱ�ȭ ����
			return false;
		}

		if( Model[ i ] == Model.back() )
		{
			// ������ ���̾��� ��쿡�� m_upStreamGradient�� �ʱ�ȭ �Ѵ�.
			currentNeuronSize = Model[ i ]->BL_Get_NeuronSize();
			weightInitResult = Model[ i ]->BL_Set_Weight( currentNeuronSize );
			if( weightInitResult == false )
			{
				// ���̾� ���� �ʱ�ȭ ����
				return false;
			}

			break;
		}


		currentNeuronSize = Model[ i ]->BL_Get_NeuronSize();
		nextNeuronSize = Model[ i + 1 ]->BL_Get_NeuronSize();
		weightSize = currentNeuronSize * nextNeuronSize;

		//initResult = Model[ i ]->LayerInit( INIT_TO_WEIGHT, weightSize );
		weightInitResult = Model[ i ]->BL_Set_Weight( weightSize );
		if( weightInitResult == false )
		{
			// ���̾� ���� ����ġ �ʱ�ȭ ����
			return false;
		}

		biasInitResult = Model[ i ]->BL_Set_Bias( nextNeuronSize );
		if( biasInitResult == false )
		{
			// ���̾� ���� Bias �ʱ�ȭ ����
			return false;
		}
	}


#ifdef _DEBUG
	if( (int)Model.size() <= 0 )
	{
		return false;
	}

	for( int i = 0; i < (int)Model.size(); i++ )
	{
		currentLayer = Model[ i ];
		if( currentLayer == nullptr )
		{
			// ���̾� ���� ����
			return false;
		}

		if( currentLayer->BL_Get_BaseLayer_Type() == E_Input )
		{
			currentLayer->BL_Print_BaseLayer_Info();
			currentLayer->BL_Print_Layer_Info();
			currentLayer->BL_func();
		}
		else
		{
			currentLayer->BL_Print_BaseLayer_Info();
			currentLayer->BL_Print_Layer_Info();
		}
		currentLayer = nullptr;
	}
#endif


	return true;
}


bool NeuralNetwork::NN_Model_Learn( int Epoch, int BatchSize, float LearningRate, E_Optimizer optimizer, E_LossFunction lossFunction, float **trainX, float *trainY, int trainDataCnt, float targetMin, float targetMax )
{
	float	*unitTrainX;		// �н� ������ 1��Ʈ
	float	unitTrainY;			// unitTrainX�� �ش��ϴ� ����(����) �� 1��

	float	targetValue;		// ���� ��
	float	predictedValue;		// ���� ��
	//float	lossValue;			// �ս� ��

	bool	propResult;
	bool	lossResult;
	bool	optimizerResult;
	bool	progressResult;

	int		backwardStep;


/*
	FILE *fp_predict;
	FILE *fp_target;
	float *predictArray;
	float *targetArray;
	int arrayIndex = 0;

	predictArray = new float[ 1000 ] {};
	targetArray = new float[ 1000 ] {};

	fopen_s( &fp_predict, "D:/SVN/Weather_Predict_Neural_Network/predict.txt", "wt" );
	fopen_s( &fp_target, "D:/SVN/Weather_Predict_Neural_Network/target.txt", "wt" );
*/




	if( Epoch <= 0 || BatchSize <= 0 || BatchSize > trainDataCnt )
	{
		// ���� �Ǵ� ��ġ������ ���� 0 �Ǵ� ���� �Ǵ� �н������� ��ü ũ�⺸�� ū ���� ���Դ�.
		::gLog_LogFile.LS_AddLog( _T( "Epoch or batch size is invalid value - NeuralNetwork.cpp : NN_Model_Learn" ) );

		return false;
	}

	if( trainX == nullptr || trainY == nullptr )
	{
		// �н������͸� �߸� �Ѿ��.
		::gLog_LogFile.LS_AddLog( _T( "TrainX or TrainY is nullptr - NeuralNetwork.cpp : NN_Model_Learn" ) );

		return false;
	}

	if( (int)Model.size() == 0 )
	{
		// �� ���Ϳ� ���̾ �������� ����
		return false;
	}

	
	lossResult = NN_Init_LossFunction( lossFunction );
	if( lossResult == false )
	{
		return false;
	}

	optimizerResult = NN_Init_Optimizer( BatchSize, optimizer, trainDataCnt );
	if( optimizerResult == false )
	{
		return false;
	}


	backwardStep = -1;
	for( int repeat = 0; repeat < Epoch; repeat++ )
	{
		int batchUpdateCnt = 0;

		backwardStep += BatchSize;

		
		progressResult = NN_Print_Learning_Progress( E_EPOCH_PROGRESS, repeat, Epoch, nullptr, NULL, NULL, NULL, NULL, NULL );
		if( progressResult == false )
		{
			return false;
		}
		

		for( int dataIndex = 0; dataIndex < trainDataCnt; dataIndex++ )
		{
			unitTrainX = *( trainX + dataIndex );
			unitTrainY = *( trainY + dataIndex );


			// ������
			propResult = NN_Propagation( unitTrainX, unitTrainY, BatchSize, dataIndex, E_FOREWARD_PROP );
			if( propResult == false )
			{
				::gLog_LogFile.LS_AddLog( _T( "Model foreward propagation fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

				return false;
			}


			targetValue		= unitTrainY;
			predictedValue	= Model.back()->BL_Get_Predicted_Value();

			// error ���
			lossResult = m_LossFunction->BLF_MSE_Error_Compute( targetValue, predictedValue );
			if( lossResult == false )
			{
				::gLog_LogFile.LS_AddLog( _T( "Model error compute fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

				return false;
			}
			

			// ������
			propResult = NN_Propagation( nullptr, NULL, BatchSize, NULL, E_BACKWARD_PROP );
			if( propResult == false )
			{
				::gLog_LogFile.LS_AddLog( _T( "Model backward propagation fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

				return false;
			}

			if( this->m_ComputeOccur == E_NOT_COMPUTED )
			{
				this->m_ComputeOccur = E_COMPUTED;
			}


			// Weight, Bias ����ȭ ������Ʈ
			if( dataIndex == backwardStep )
			{
				int		gradientWeighttSize = 0;
				int		gradientBiasSize = 0;
				float	gradientValue = 0;

				float	optimizingValue = 0;
				bool	optimizingResult;


				// ��� error �հ��� ��հ��� loss ���
				lossResult = m_LossFunction->BLF_MSE_Loss_Compute( BatchSize );
				if( lossResult == false )
				{
					::gLog_LogFile.LS_AddLog( _T( "Model loss compute fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

					return false;
				}


				// ���⼭ ���� gradient�� ����ġ, Bias ������Ʈ ����
				for( int i = 0; i < (int)Model.size() - 1; i++ )
				{
					gradientWeighttSize = Model[ i ]->BL_Get_WeightSize();
					for( int gradientIndex = 0; gradientIndex < gradientWeighttSize; gradientIndex++ )
					{
						gradientValue		= Model[ i ]->BL_Get_GradientWeight( gradientIndex );
						optimizingValue		= this->m_Optimizer->BO_Compute_Optimizing_Value( gradientValue, LearningRate );
						
						optimizingResult = Model[ i ]->BL_Weight_Optimizing( gradientIndex, optimizingValue );
						if( optimizingResult == false )
						{
							::gLog_LogFile.LS_AddLog( _T( "Model weight optimizing fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

							return false;
						}
					}

					gradientBiasSize = Model[ i ]->BL_Get_BiasSize();
					for( int gradientIndex = 0; gradientIndex < gradientBiasSize; gradientIndex++ )
					{
						gradientValue		= Model[ i ]->BL_Get_GradientBias( gradientIndex );
						optimizingValue		= this->m_Optimizer->BO_Compute_Optimizing_Value( gradientValue, LearningRate );

						optimizingResult = Model[ i ]->BL_Bias_Optimizing( gradientIndex, optimizingValue );
						if( optimizingResult == false )
						{
							::gLog_LogFile.LS_AddLog( _T( "Model bias optimizing fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

							return false;
						}
					}
				}

				
				progressResult = NN_Print_Learning_Progress( E_GRADIENT_UPDATE_PROGRESS, NULL, NULL, &batchUpdateCnt, unitTrainY, BatchSize, trainDataCnt, targetMin, targetMax );
				if( progressResult == false )
				{
					::gLog_LogFile.LS_AddLog( _T( "Model learning progress print fail - NeuralNetwork.cpp : NN_Model_Learn" ) );

					return false;
				}
				

				/*
				if( ( batchUpdateCnt % ( trainDataCnt / BatchSize / 10 ) ) == 0 )
				{
					if( batchUpdateCnt > 1 )
					{
						*( predictArray + arrayIndex ) = ( Model.back()->BL_Get_Predicted_Value() + 1 ) / 2 * ( targetMax - targetMin ) + targetMin;
						*( targetArray + arrayIndex ) = ( unitTrainY + 1 ) / 2 * ( targetMax - targetMin ) + targetMin;
						arrayIndex++;
					}
				}
				*/

				lossResult = m_LossFunction->BLF_MSE_Clear_Error();
				if( lossResult == false )
				{
					return false;
				}

				for( int i = 0; i < (int)Model.size() - 1; i++ )
				{
					// ���� �ϱ� ���ؼ� 0���� �ʱ�ȭ

					Model[ i ]->BL_Clear_GradientWeight();
					Model[ i ]->BL_Clear_UpStreamGradient();
					Model[ i ]->BL_Clear_GradientBias();
				}

				if( this->m_ComputeOccur == E_COMPUTED )
				{
					this->m_ComputeOccur = E_NOT_COMPUTED;
				}

				backwardStep += BatchSize;
			}
		}
		backwardStep = -1;

	}


	/*
	for( int i = 0; i < 1000; i++ )
	{
		string predict = to_string( *( predictArray + i ) );
		string target = to_string( *( targetArray + i ) );

		fprintf( fp_predict, predict.c_str() );
		fprintf( fp_predict, "\n" );

		fprintf( fp_target, target.c_str() );
		fprintf( fp_target, "\n" );
	}
	fclose( fp_predict );
	fclose( fp_target );
	*/


	
	for( int i = 0; i < (int)Model.size(); i++ )
	{
		Model[ i ]->BL_Save_LearningData();
	}
	



	return true;
}


bool NeuralNetwork::NN_Propagation( float *unitTrainX, float unitTrainY, int BatchSize, int dataIndex, E_PropagationType propagationType )
{
	BaseLayer	*nextLayer;

	int			endLayer;

	float		error;

	bool		forewardResult;
	bool		backwardResult;


	if( propagationType == E_FOREWARD_PROP )
	{
		// ������
		for( int k = 0; k < (int)Model.size(); k++ )
		{
			if( Model[ k ] == nullptr )
			{
				// �����Ϸ��� ���̾ �������� ����
				return false;
			}

			// ���� ������ ���̾�� ������ �׻� OutputLayer�̹Ƿ� ���̻� �����ĸ� �� �� ����.
			if( Model[ k ] != Model.back() )
			{
				nextLayer = Model[ k + 1 ];
				if( nextLayer == nullptr )
				{
					// ���� ���̾� ���� ����
					return false;
				}

				if( Model[ k ] == Model.front() )
				{
					/*
					* Foreward_Prop �Լ����� ������ �н������� �Է��� �ٸ� �Լ��� ���� �и��ϰ�,
					* �Է·��̾�, ���緹�̾�� ��� ���� Foreward_Prop �Լ��� ���������� ����ϵ��� ����.
					*/

					// ù��° ���̾�� �׻� inputlayer�̸�, �н������͸� �Է����� �ޱ� ������ ���� ó�����ش�.
					forewardResult = Model[ k ]->BL_ForewardPass( unitTrainX, nextLayer );
					if( forewardResult == false )
					{
						return false;
					}
				}
				else
				{
					// InputLayer�� ������ �ٸ� ���̾���� ���̾��� Ư���� �°� ȣ�����ش�.
					forewardResult = Model[ k ]->BL_ForewardPass( nextLayer );
					if( forewardResult == false )
					{
						return false;
					}
				}
				nextLayer = nullptr;
			}
		}
	}
	else if( propagationType == E_BACKWARD_PROP )
	{
		// ������
		// ��ġ������� ������ ���ִ°��� Mini-Batch����ϰ����̴�.
		// 
		// �����ͼ��� �Ĺ� �κп��� ��ġ������� ������ �������� �ʴ� ��� ��� ó���ؾ� �ϴ��� ã�ƺ� ��.


		// Gradient ���
		endLayer	= Model.size() - 2;

		error		= this->m_LossFunction->BLF_MSE_Get_Error();

		for( int currentLayerIndex = endLayer; currentLayerIndex >= 0; currentLayerIndex-- )
		{
			int		previousLayerIndex	= 0;
			float	*upStreamBundle		= nullptr;

			int		currentNeuronSize	= 0;
			int		previousNeuronSize	= 0;
			int		currentWeightSize	= 0;
			int		currentBiasSize		= 0;

			int		weightBoundary		= 0;


			previousLayerIndex	= currentLayerIndex + 1;

			currentNeuronSize	= Model[ currentLayerIndex ]->BL_Get_NeuronSize();
			currentWeightSize	= Model[ currentLayerIndex ]->BL_Get_WeightSize();
			previousNeuronSize	= currentWeightSize / currentNeuronSize;

			upStreamBundle = new float[previousNeuronSize] { };
			if( upStreamBundle == nullptr )
			{
				return false;
			}

			for( int i = 0; i < previousNeuronSize; i++ )
			{
				//*( upStreamBundle + i ) = Model[ previousLayerIndex ]->BL_Get_UpStreamGradient( i );
				*( upStreamBundle + i ) = ROUND( Model[ previousLayerIndex ]->BL_Get_UpStreamGradient( i ) );
			}


			backwardResult = Model[ currentLayerIndex ]->BL_BackwardPass( previousNeuronSize, upStreamBundle, error, BatchSize, this->m_ComputeOccur );
			if( backwardResult == false )
			{
				return false;
			}

			/*
			if( upStreamBundle == nullptr )
			{
				return false;
			}
			*/
			delete[] upStreamBundle;
		}
	}


	return true;
}


bool NeuralNetwork::NN_Print_Learning_Progress( E_Learning_Progress progressType, int repeat, int Epoch, int *batchUpdateCnt, float unitTrainY, int BatchSize, int trainDataCnt, float targetMin, float targetMax )
{
	if( targetMin > targetMax )
	{
		return false;
	}


	switch( progressType )
	{
	case E_EPOCH_PROGRESS:
	{
		cout << endl << endl << "EPOCH [ " << repeat + 1 << " / " << Epoch << " ]" << endl;
	}
	break;


	case E_GRADIENT_UPDATE_PROGRESS:
	{
		float	predict		= ( Model.back()->BL_Get_Predicted_Value() + 1 ) / 2 * ( targetMax - targetMin ) + targetMin;
		float	target		= ( unitTrainY + 1 ) / 2 * ( targetMax - targetMin ) + targetMin;

		string	str_predict	= " Predict : ";
		string	str_target	= " Target : ";
		string	str_loss	= " Loss : ";


		if( ( *batchUpdateCnt % ( trainDataCnt / BatchSize / 10 ) ) == 0 )
		{
			*batchUpdateCnt += 1;

			if( *batchUpdateCnt > 1 )
			{
				cout << str_predict << left << setw( 13 ) << predict << str_target << left << setw( 10 ) << target << str_loss << m_LossFunction->BLF_MSE_Get_Loss() * 100 << endl;
			}

			cout << "[" << right << setw( 3 ) << *batchUpdateCnt << " / " << ( trainDataCnt / BatchSize ) << "] ";
			cout << "*";
		}
		else
		{
			*batchUpdateCnt += 1;
			cout << "*";
		}

		if( *batchUpdateCnt == ( trainDataCnt / BatchSize ) )
		{
			int precede		= ( trainDataCnt / BatchSize ) - ( ( trainDataCnt / BatchSize / 10 ) * 10 );
			int setwValue	= (int)( trainDataCnt / BatchSize / 10 ) + str_predict.size() - precede;

			cout << right << setw( setwValue ) << str_predict << left << setw( 13 ) << predict << str_target << left << setw( 10 ) << target << str_loss << m_LossFunction->BLF_MSE_Get_Loss() * 100 << endl;
		}
	}
	break;
	}


	return true;
}


/*
int NeuralNetwork::Model_Predict()
{
}
*/

bool NeuralNetwork::NN_Add_Layer( E_LayerType layerType, int row, int column, E_ActivationType activationType, string structureName )
{
	if( m_exceptionOccurReport == E_EXCEPTION_OCCUR )
	{
		// �̹� ������ �߻��߱� ������ �����ִ� ��� �۾��� ���ؼ��� Add_Layer�Լ��� ����Ǹ� �ٷ� ��ҵȴ�.
		return false;
	}

	if( NN_Max_Layer_Checker() == false )
	{
		::gLog_LogFile.LS_AddLog( _T( "Layer is maximun" ) );

		m_exceptionOccurReport = E_EXCEPTION_OCCUR;

		return false;
	}


	switch( layerType )
	{
	case E_Input:
	{
		if( structureName.empty() == true )
			structureName = "InputLayer" + to_string( (int)Model.size() + (unsigned __int8)1 );

		InputLayer		*L_Input	= new InputLayer( E_Input, row, column, activationType, structureName );
		
		if( L_Input == nullptr )
		{
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;

			return false;
		}
		
		if( L_Input->IL_Get_BaseLayer_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Input;

			return false;
		}

		if( L_Input->IL_Get_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			// ���̾� ��ü ������ ���������� ���̾� ���ο��� ������ �ʱ�ȭ�ϴ� ���� ����
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Input;

			return false;
		}

		Model.emplace_back( L_Input );
	}
	break;


	case E_Dense:
	{
		if( structureName.empty() )
			structureName = "DenseLayer" + to_string( (int)Model.size() + (unsigned __int8)1 );

		DenseLayer		*L_Dense	= new DenseLayer( E_Dense, row, column, activationType, structureName );

		if( L_Dense == nullptr )
		{
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;

			return false;
		}

		if( L_Dense->DL_Get_BaseLayer_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Dense;

			return false;
		}

		if( L_Dense->DL_Get_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			// ���̾� ��ü ������ ���������� ���̾� ���ο��� ������ �ʱ�ȭ�ϴ� ���� ����
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Dense;

			return false;
		}

		Model.emplace_back( L_Dense );
	}
	break;


	case E_Output:
	{
		if( structureName.empty() )
			structureName = "OutputLayer" + to_string( (int)Model.size() + (unsigned __int8)1 );

		OutputLayer		*L_Output	= new OutputLayer( E_Output, row, column, structureName );

		if( L_Output == nullptr )
		{
			// ���̾� ��ü ���� ����
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			return false;
		}

		if( L_Output->OL_Get_BaseLayer_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Output;

			return false;
		}

		if( L_Output->OL_Get_Exception_Report() == E_EXCEPTION_OCCUR )
		{
			// ���̾� ��ü ������ ���������� ���̾� ���ο��� ������ �ʱ�ȭ�ϴ� ���� ����
			this->m_exceptionOccurReport = E_EXCEPTION_OCCUR;
			delete L_Output;

			return false;
		}

		Model.emplace_back( L_Output );
	}
	break;


	}


	return true;
}


bool NeuralNetwork::NN_Max_Layer_Checker()
{
	if( (int)Model.size() < MAX_LAYER_CNT )
		return true;
	else // ���̾� ������ �̹� �ִ�(255��)�̱� ������ �� �̻� ���̾ ���� ���� ����.
		return false;
}


bool NeuralNetwork::NN_Delete_Vector_Memory()
{
	if( (int)Model.size() == 0 )
	{
		return false;
	}

	for( int i = 0; i < (int)Model.size(); i++ )
	{
		if( Model[ i ] != nullptr )
		{
			delete Model[ i ];
		}
	}

	Model.clear();	// ���� ������ �����͵�(����)�� �����ǰ�, ���� ��ü�� �޸𸮴� ��� �Ҵ�Ǿ� �ִ�.

	// clear()�� ���� size�� �پ�����, capacity�� �״�� ���ұ� ������ ���� ��ü�� �޸𸮵� ���� ����� ��.

	return true;
}