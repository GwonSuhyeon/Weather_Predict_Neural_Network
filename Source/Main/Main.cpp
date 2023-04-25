

#include "Main.h"


int main( void )
{
	NeuralNetwork		WeatherDnnModel;
	bool				NN_buildResult;
	bool				NN_learnResult;
	bool				NN_deleteResult;


	DataPreProcess		DataPreProcessor;
	bool				DPP_initResult;
	bool				DPP_loadResult;
	bool				DPP_scalerResult;
	bool				DPP_deleteResult;
	bool				DPP_splitResult;

	int					targetIndex			= 0;
	float				targetMin			= MIN_DEFAULT_VALUE;
	float				targetMax			= MAX_DEFAULT_VALUE;

	int					trainDataCnt		= 0;
	int					testDataCnt			= 0;

	float				**trainX			= nullptr;
	float				*trainY				= nullptr;
	float				**testX				= nullptr;
	float				*testY				= nullptr;

	
	/*
	// 랜덤함수값 정규분포 따르는지 확인하는 코드
	random_device					randomDevice;
	mt19937							mersenneTwister( randomDevice() );
	normal_distribution<float>		randomCreator( RANDOM_MEAN, RANDOM_STANDARD_DEVIATION );
	float value;
	string text;
	FILE *fp;

	fp = fopen( "D:/SVN/Weather_Predict_Neural_Network/random.txt", "wt" );
	
	for( int i = 0; i < 100000; i++ )
	{
		value = randomCreator( mersenneTwister );
		text = to_string( value );
		fprintf( fp, text.c_str() );
		fprintf( fp, "\n" );
	}
	fclose( fp );
	return 0;
	*/
	
	//::gLog_LogFile.LS_AddLog( _T( "ERROR - CAcceptSes)" ) );

	{
		DPP_initResult = DataPreProcessor.DPP_Init();
		if( DPP_initResult == INIT_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor init fail - Main.cpp" ) );

			return -1;
		}

		DPP_loadResult = DataPreProcessor.DPP_File_Load();
		if( DPP_loadResult == false )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor file load fail - Main.cpp" ) );

			return -1;
		}

		DPP_scalerResult = DataPreProcessor.DPP_MinMax_Scaling( -1, 1 );
		if( DPP_scalerResult == SCALER_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor data scaling fail - Main.cpp" ) );

			return -1;
		}

		DPP_splitResult = DataPreProcessor.DPP_Train_Test_Split( E_NOT_SHUFFLE, (float)0.1, (float)0.9, &trainX, &trainY, &testX, &testY );
		if( DPP_splitResult == SPLIT_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor data split fail - Main.cpp" ) );

			return -1;
		}

		DPP_deleteResult = DataPreProcessor.DPP_Delete_WeatherData();
		if( DPP_deleteResult == DELETE_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DaataPreProcessor delete variable fail - Main.cpp" ) );

			return -1;
		}
	}


	{
		targetMin = DataPreProcessor.DPP_Get_TargetMin( targetIndex );
		if( targetMin >= MIN_DEFAULT_VALUE )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor get min value fail - Main.cpp" ) );

			return -1;
		}
		
		targetMax = DataPreProcessor.DPP_Get_TargetMax( targetIndex );
		if( targetMax <= MAX_DEFAULT_VALUE )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor get max value fail - Main.cpp" ) );

			return -1;
		}

		trainDataCnt = DataPreProcessor.DPP_Get_TrainDataCnt();
		if( trainDataCnt == -1 )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor get train data count fail - Main.cpp" ) );

			return -1;
		}

		testDataCnt = DataPreProcessor.DPP_Get_TestDataCnt();
		if( testDataCnt == -1 )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor get test data count fail - Main.cpp" ) );

			return -1;
		}

		DPP_deleteResult = DataPreProcessor.DPP_Delete_S_MinMaxScaler();
		if( DPP_deleteResult == DELETE_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "DataPreProcessor delete scaler struct fail - Main.cpp" ) );

			return -1;
		}
	}


	{
		NN_buildResult = WeatherDnnModel.NN_Model_Build();
		if( NN_buildResult == BUILD_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "WeatherDnnModel build fail - Main.cpp" ) );

			return -1;
		}

		NN_learnResult = WeatherDnnModel.NN_Model_Learn( 100, 128, (float)0.01, E_MGD, E_MSE, trainX, trainY, trainDataCnt, targetMin, targetMax );
		if( NN_learnResult == LEARN_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "WeatherDnnModel learn fail - Main.cpp" ) );

			return -1;
		}
	}


	{
		NN_deleteResult = WeatherDnnModel.NN_Delete_LossFunction();
		if( NN_deleteResult == DELETE_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "WeatherDnnModel delete loss function fail - Main.cpp" ) );

			return -1;
		}

		NN_deleteResult = WeatherDnnModel.NN_Delete_Optimizer();
		if( NN_deleteResult == DELETE_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "WeatherDnnModel delete optimizer fail - Main.cpp" ) );

			return -1;
		}

		NN_deleteResult = WeatherDnnModel.NN_Delete_Vector_Memory();
		if( NN_deleteResult == DELETE_FAIL )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "WeatherDnnModel delete model vector fail - Main.cpp" ) );

			return -1;
		}
	}


	for( int i = 0; i < trainDataCnt; i++ )
	{
		if( *( trainX + i ) == nullptr )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( trainX ) fail - Main.cpp" ) );

			return false;
		}

		delete[] * ( trainX + i );
	}

	if( trainX == nullptr )
	{
		::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( trainX ) fail - Main.cpp" ) );

		return false;
	}

	delete[] trainX;

	if( trainY == nullptr )
	{
		::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( trainY ) fail - Main.cpp" ) );

		return false;
	}

	delete[] trainY;


	for( int i = 0; i < testDataCnt; i++ )
	{
		if( *( testX + i ) == nullptr )
		{
			::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( testX ) fail - Main.cpp" ) );

			return false;
		}

		delete[] * ( testX + i );
	}

	if( testX == nullptr )
	{
		::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( testX ) fail - Main.cpp" ) );

		return false;
	}

	delete[] testX;

	if( testY == nullptr )
	{
		::gLog_ErrorLog.LS_AddLog( _T( "Delete variable( testY ) fail - Main.cpp" ) );

		return false;
	}

	delete[] testY;


	return 0;
}


/*
bool operator++( E_File_Number &currentFile, int )
{
	switch( currentFile )
	{
	case E_FIRST:
	{
		currentFile = E_SECOND;
	}
	break;


	case E_SECOND:
	{
		currentFile = E_THIRD;
	}
	break;


	case E_THIRD:
	{
		currentFile = E_FOURTH;
	}
	break;


	case E_FOURTH:
	{
		currentFile = E_FIFTH;
	}
	break;


	case E_FIFTH:
	{
		currentFile = E_SIXTH;
	}
	break;


	case E_SIXTH:
	{
		currentFile = E_SEVENTH;
	}
	break;


	case E_SEVENTH:
	{
		currentFile = E_EIGHTH;
	}
	break;


	case E_EIGHTH:
	{
		currentFile = E_NINTH;
	}
	break;


	case E_NINTH:
	{
		currentFile = E_TENTH;
	}
	break;


	default:
	{
		return false;
	}
	}


	return true;
}
*/