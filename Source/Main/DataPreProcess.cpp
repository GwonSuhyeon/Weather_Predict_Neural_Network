

#include "DataPreProcess.h"


DataPreProcess::DataPreProcess()
{

}


DataPreProcess::~DataPreProcess()
{

}


/*
E_File_Number DataPreProcess::operator+( E_File_Number currentEnum )
{
	E_File_Number	nextEnum = E_FIRST;


	switch( currentEnum )
	{
	case E_FIRST:
	{
		nextEnum = E_SECOND;
	}
	break;


	case E_SECOND:
	{
		nextEnum = E_THIRD;
	}
	break;


	case E_THIRD:
	{
		nextEnum = E_FOURTH;
	}
	break;


	case E_FOURTH:
	{
		nextEnum = E_FIFTH;
	}
	break;


	case E_FIFTH:
	{
		nextEnum = E_SIXTH;
	}
	break;


	case E_SIXTH:
	{
		nextEnum = E_SEVENTH;
	}
	break;


	case E_SEVENTH:
	{
		nextEnum = E_EIGHTH;
	}
	break;


	case E_EIGHTH:
	{
		nextEnum = E_NINTH;
	}
	break;


	case E_NINTH:
	{
		nextEnum = E_TENTH;
	}
	break;


	default:
		break;
	}


	return nextEnum;
}
*/


bool DataPreProcess::DPP_Errno_Checker( errno_t fopenResult )
{
	switch( fopenResult )
	{
		// case에서는 fopen_s함수에서 에러가 발생한다면 주로 발생 할 것 같다고 예상되는 에러코드들만 확인한다.
	case 0:
	{
		// 에러 발생 안함.
		;
	}
	break;


	case ENOENT:		// No such file or directory
	case EINTR:			// Interrupted function
	case EAGAIN:		// No more processes or not enough memory or maximum nesting level reached
	case ENOMEM:		// Not enough memory
	case ENFILE:		// Too many files open in system
	case EFBIG:			// File too large
	case ENAMETOOLONG:	// Filename too long


	{
		// 주요 에러들은 여기에서 로그 남기기.
		::gLog_LogFile.LS_AddLog( _T( "File open fail [ Errno : major error ] - DataPreProcess.cpp" ) );

		return false;
	}


	default:
	{
		// 주요 에러가 아닌 경우에는 여기에서 로그를 남긴다.
		::gLog_LogFile.LS_AddLog( _T( "File open fail [ Errno : not major error ] - DataPreProcess.cpp" ) );

		return false;
	}
	break;


	}


	return true;
}


bool DataPreProcess::DPP_Init()
{
	this->m_fp_WeatherData		= nullptr;

	this->m_totalReadCnt		= 0;
	this->m_weatherElementsCnt	= 0;

	this->m_trainDataCnt		= 0;
	this->m_testDataCnt			= 0;
	

	return true;
}


bool DataPreProcess::DPP_File_Load()
{
	/*
	* errno_t는 int 자료형의 typedef 선언이다.
	* 다양한 함수들이 실행 후에 에러가 발생하면,
	* 에러의 종류에 따라서 에러 상수값을 errno_t 자료형의 전역변수 errno에 저장된다.
	*
	* https://docs.microsoft.com/en-us/previous-versions/t3ayayh1(v=vs.140)
	* -> errno_t 에러 코드
	*/

	/*
	* fopen_s의 리턴값으로 errno_t를 주는데,
	* 이 errno_t를 자료형으로 가지는 전역변수 errno에 에러 상수값을 저장하는
	* _set_errno와 저장된 에러 상수값을 가져오는 _get_errno는 fopen_s함수 실행의 어느 부분에서
	* 호출되는 것일까??
	* 내 생각에는, fopen_s 실행 중에 에러가 발생하면 그때 errno에 _set_errno로 에러코드를 저장하고,
	* fopen_s함수를 종료 시키면서 _get_errno로 에러코드를 가져와서 리턴값으로 주는 건가??
	* (_set_errno, _get_errno는 errno.h에 있음.)
	* (errno_t는 corecrt.h에 있음.)
	*
	* 만약, 프로그램 실행 중에 여러곳에서 동시에 errno_t에 해당하는 에러가 발생해서
	* 동시에 전역변수 errno에 에러 상수값을 저장하려고 접근해도 괜찮은건가??
	* 쓰레드들이 같은 메모리에 동시에 접근하는거랑 같은 상황인건가??
	*/


	FILE		*fp_dataInfo		= nullptr;
	//string		str_dataInfo;
	char		c_dataInfo[ 30 ];

	FILE		*fp_fileList		= nullptr;
	//string		dataFileAddress;
	char		c_dataFileAddress[ 100 ];

	FILE		*fp_weatherData		= nullptr;
	//string		str_weatherData;
	char		c_weatherData[ 100 ];

	char		*readText			= nullptr;
	char		*readPosition		= nullptr;	// 읽은 텍스트
	char		*remainPosition		= nullptr;	// 읽어야 할(남은) 텍스트

	errno_t		fopenResult;
	bool		errnoCheck;

	bool		fpCloseResult;


	// elements 가져오기
	{
		fopenResult = fopen_s( &fp_dataInfo, WEATHER_DATA_INFO_ADDRESS, "r" );
		errnoCheck = DPP_Errno_Checker( fopenResult );
		if( errnoCheck == false )
		{
			// 파일 오픈 실패
			return false;
		}

		fscanf_s( fp_dataInfo, "%s\n", c_dataInfo, sizeof(c_dataInfo) );
		/*
		if( c_dataInfo[0] == '\n' )
		{
			// 반복문 사용할때는 continue로 변경.
			;
		}
		*/

		readPosition = strtok_s( c_dataInfo, "= ", &remainPosition );
		if( readPosition == nullptr )
		{
			::gLog_LogFile.LS_AddLog( _T( "Invalid input format in WeatherData_Info file" ) );

			fpCloseResult = DPP_File_Close( fp_dataInfo );
			if( fpCloseResult == false )
			{
				return false;
			}

			return false;
		}

		while( 1 )
		{
			if( readPosition == nullptr )
			{
				break;
			}

			readText = readPosition;

			readPosition = strtok_s( nullptr, "= ", &remainPosition );
		}

		if( readText == nullptr )
		{
			fpCloseResult = DPP_File_Close( fp_dataInfo );
			if( fpCloseResult == false )
			{
				return false;
			}

			return false;
		}

		this->m_weatherElementsCnt = atoi( readText );

		fpCloseResult = DPP_File_Close( fp_dataInfo );
		if( fpCloseResult == false )
		{
			return false;
		}

		fp_dataInfo = nullptr;
	}
	
	
	fopenResult = fopen_s( &fp_fileList, WEATHER_DATA_FILE_LIST_ADDRESS, "r" );
	errnoCheck = DPP_Errno_Checker( fopenResult );
	if( errnoCheck == false )
	{
		// 파일 오픈 실패
		return false;
	}


	readText		= nullptr;
	readPosition	= nullptr;
	remainPosition	= nullptr;
	if( fp_fileList == nullptr )
	{
		return false;
	}

	while( fscanf_s( fp_fileList, "%s\n", c_dataFileAddress, sizeof(c_dataFileAddress) ) != EOF )
	{
		if( c_dataFileAddress[ 0 ] == '\n' )
		{
			continue;
		}

		fopenResult = fopen_s( &fp_weatherData, c_dataFileAddress, "r" );
		errnoCheck = DPP_Errno_Checker( fopenResult );
		if( errnoCheck == false )
		{
			// 파일 오픈 실패
			return false;
		}

		
		if( fp_weatherData == nullptr )
		{
			fpCloseResult = DPP_File_Close( fp_fileList );
			if( fpCloseResult == false )
			{
				return false;
			}

			return false;
		}

		while( fscanf_s( fp_weatherData, "%s\n", c_weatherData, sizeof(c_weatherData) ) != EOF )
		{
			int		findIndex		= 0;
			float	*allocVector	= nullptr;
			float	*currentVector	= nullptr;


			while( c_weatherData[ findIndex ] != '\0' )
			{
				// 잘못된 문자 입력 여부 검사
				if( ( c_weatherData[ findIndex ] < 0 ) && ( c_weatherData[ findIndex ] > 9 ) && c_weatherData[ findIndex ] != '-' &&
					( c_weatherData[ findIndex ] != '.' ) && ( c_weatherData[ findIndex ] != ',' ) && ( c_weatherData[ findIndex ] != ' ' ) )
				{
					::gLog_LogFile.LS_AddLog( _T( "Invalid value input in WeatherData file ( .txt or .csv )" ) );

					fpCloseResult = DPP_File_Close( fp_weatherData );
					if( fpCloseResult == false )
					{
						return false;
					}

					return false;
				}

				findIndex++;
			}


			allocVector = new float[ this->m_weatherElementsCnt ];
			if( allocVector == nullptr )
			{
				fpCloseResult = DPP_File_Close( fp_weatherData );
				if( fpCloseResult == false )
				{
					return false;
				}

				// float 동적할당 메모리 해제.
				for( int vectorIndex = 0; vectorIndex < (int)this->m_v_WeatherData.size(); vectorIndex++ )
				{
					float *travelData = nullptr;

					travelData = this->m_v_WeatherData[ vectorIndex ];
					if( travelData == nullptr )
					{
						return false;
					}

					delete[] travelData;
				}

				return false;
			}


			this->m_v_WeatherData.emplace_back( allocVector );

			currentVector = this->m_v_WeatherData.back();
			readPosition = strtok_s( c_weatherData, ", ", &remainPosition );
			for( int readDataCnt = 0; readDataCnt < this->m_weatherElementsCnt; readDataCnt++ )
			{
				if( readPosition == nullptr )
				{
					::gLog_LogFile.LS_AddLog( _T( "Weather data elements is lacking in WeatherData file" ) );

					fpCloseResult = DPP_File_Close( fp_weatherData );
					{
						return false;
					}

					// float 동적할당 메모리 해제.
					for( int vectorIndex = 0; vectorIndex < (int)this->m_v_WeatherData.size(); vectorIndex++ )
					{
						float *travelVector = nullptr;

						travelVector = this->m_v_WeatherData[ vectorIndex ];
						if( travelVector == nullptr )
						{
							return false;
						}

						delete[] travelVector;
					}

					return false;
				}

				readText = readPosition;
				*( currentVector + readDataCnt ) = (float)atof( readText );

				readPosition = strtok_s( nullptr, ", ", &remainPosition );
			}

			this->m_totalReadCnt++;
		}


		fpCloseResult = DPP_File_Close( fp_weatherData );
		if( fpCloseResult == false )
		{
			return false;
		}

		fp_weatherData = nullptr;
	}


	fpCloseResult = DPP_File_Close( fp_fileList );
	if( fpCloseResult == false )
	{
		return false;
	}

	fp_fileList = nullptr;


	return true;
}


bool DataPreProcess::DPP_File_Close( FILE *fp_filePtr )
{
	if( fp_filePtr == nullptr )
	{
		::gLog_LogFile.LS_AddLog( _T( "File close fail - DataPreProcess.cpp" ) );

		return false;
	}

	fclose( fp_filePtr );


	return true;
}


bool DataPreProcess::DPP_MinMax_Scaling( int minRange, int maxRange )
{
	float	*pMemoryCheck		= nullptr;
	float	value				= 0.0;

	float	minValue			= MIN_DEFAULT_VALUE;
	float	maxValue			= MAX_DEFAULT_VALUE;
	int		rangeValue			= 0;

	/*
	FILE *fp;
	if( element == 0 )
	{
		fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/NotScaling.txt", "wt" );
		for( int i = 0; i < WEATHER_DATA_TOTAL_CNT; i++ )
		{
			float value = *( *( m_pWeatherData + i ) + 0 );
			std::string text = std::to_string( value );
			fprintf( fp, text.c_str() );
			fprintf( fp, "\n" );
		}
		fclose( fp );
	}
	*/
	
	if( minRange >= maxRange )
	{
		::gLog_LogFile.LS_AddLog( _T( "Min value is larger than max value in MinMaxScaler" ) );

		return false;
	}

	rangeValue = maxRange - minRange;

	
	this->m_S_MinMaxScaler.s_Feature_Min = new float[ this->m_weatherElementsCnt ];
	if( this->m_S_MinMaxScaler.s_Feature_Min == nullptr )
	{
		return false;
	}

	this->m_S_MinMaxScaler.s_Feature_Max = new float[ this->m_weatherElementsCnt ];
	if( this->m_S_MinMaxScaler.s_Feature_Max == nullptr )
	{
		return false;
	}
	

	for( int element = 0; element < this->m_weatherElementsCnt; element++ )
	{
		for( int currentData = 0; currentData < this->m_totalReadCnt; currentData++ )
		{
			float *travelVector = nullptr;

			travelVector = this->m_v_WeatherData[ currentData ];
			pMemoryCheck = travelVector + element;
			if( pMemoryCheck == nullptr )
			{
				return false;
			}

			value = *pMemoryCheck;
			if( value < minValue )
			{
				minValue = value;
			}
			
			if( value > maxValue )
			{
				maxValue = value;
			}
		}


		*( m_S_MinMaxScaler.s_Feature_Min + element ) = minValue;
		*( m_S_MinMaxScaler.s_Feature_Max + element ) = maxValue;


		for( int i = 0; i < this->m_totalReadCnt; i++ )
		{
			float *travelVector = nullptr;
			float originalValue = 0;
			float scaledValue	= 0;
			float roundValue	= 0;

			travelVector = this->m_v_WeatherData[ i ];
			originalValue = *( travelVector + element );

			scaledValue = ( originalValue - minValue ) / ( maxValue - minValue ) * rangeValue + minRange;
			//roundValue = roundf( scaledValue * float(pow(10, 6)) / (float)pow(10, 6));
			*( travelVector + element ) = scaledValue;
		}

		/*
		float *travelVector = nullptr;

		travelVector = this->m_v_WeatherData[ 0 ];
		*( travelVector + 0 ) = 0;
		for( int i = 1; i < this->m_weatherElementsCnt; i++ )
		{
			*( travelVector + i ) = 1;
		}

		*/

	}

	
	FILE *fp;
	fopen_s( &fp, "D:/SVN/Weather_Predict_Neural_Network/DataScaling.txt", "wt" );
	for( int i = 0; i < this->m_totalReadCnt; i++ )
	{
		float *travelVector = nullptr;
		float value = 0;

		travelVector = this->m_v_WeatherData[ i ];

		for( int k = 0; k < this->m_weatherElementsCnt; k++ )
		{
			value = *( travelVector + k );
			std::string text = std::to_string( value );
			
			if( k < this->m_weatherElementsCnt - 1 )
			{
				text = text + ",";
			}
			else
			{
				text = text + "\n";
			}

			fprintf( fp, text.c_str() );
		}
	}
	fclose( fp );
	


	return true;
}


bool DataPreProcess::DPP_Train_Test_Init( float ***trainX, float **trainY, float ***testX, float **testY )
{
	int elementCnt = 0;

	
	elementCnt = this->m_weatherElementsCnt;

	*trainX = new float*[ this->m_trainDataCnt ];
	if( *trainX == nullptr )
	{
		return false;
	}

	for( int i = 0; i < this->m_trainDataCnt; i++ )
	{
		*( *trainX + i ) = new float[ elementCnt ];
		if( *( *trainX + i ) == nullptr )
		{
			return false;
		}
	}

	*trainY = new float[ this->m_trainDataCnt ];
	if( *trainY == nullptr )
	{
		return false;
	}


	*testX = new float*[ this->m_testDataCnt ];
	if( *testX == nullptr )
	{
		return false;
	}

	for( int i = 0; i < this->m_testDataCnt; i++ )
	{
		*( *testX + i ) = new float[ elementCnt ];
		if( *( *testX + i ) == nullptr )
		{
			return false;
		}
	}

	*testY = new float[ this->m_testDataCnt ];
	if( *testY == nullptr )
	{
		return false;
	}


	return true;
}


bool DataPreProcess::DPP_Train_Test_Split( E_Split_Shuffle shuffleCheck, float trainRatio, float testRatio, float ***trainX, float **trainY, float ***testX, float **testY )
{
	int *dataIndexSequence = nullptr;
	int sequencePosition = 0;

	bool initResult;


	if( trainRatio <= 0 || testRatio < 0 )
	{
		// 학습 데이터셋은 반드시 필요하기 때문에 양수여야 하고, 테스트 데이터셋 비율은 반드시 필요한 것은 아니기 때문에 0 이상의 양수이면 된다.
		::gLog_LogFile.LS_AddLog( _T( "Train ratio or test ratio is invalid value in Train-Test split" ) );

		return false;
	}

	if( ( trainRatio + testRatio ) != 1 )
	{
		// 학습, 테스트 데이터셋 비율은 합계가 1이여야 함.
		::gLog_LogFile.LS_AddLog( _T( "Train ratio or test ratio is invalid value in Train-Test split" ) );

		return false;
	}


	this->m_trainDataCnt	= (int)( this->m_totalReadCnt * trainRatio );
	this->m_testDataCnt		= (int)( this->m_totalReadCnt - this->m_trainDataCnt );


	dataIndexSequence = new int[ this->m_totalReadCnt ];
	if( dataIndexSequence == nullptr )
	{
		return false;
	}

	for( int i = 0; i < this->m_totalReadCnt; i++ )
	{
		*( dataIndexSequence + i ) = i;
	}


	if( shuffleCheck == E_SHUFFLE )
	{
		random_device					randomDevice;
		mt19937							mersenneTwister( randomDevice() );
		uniform_int_distribution		randomCreator( 0, ( this->m_totalReadCnt - 1 ) );

		int firstRandom = 0;
		int secondRandom = 0;
		int temp = 0;


		for( int k = 0; k < this->m_totalReadCnt; k++ )
		{
			firstRandom = randomCreator( mersenneTwister );
			secondRandom = randomCreator( mersenneTwister );

			while( firstRandom == secondRandom )
			{
				secondRandom = randomCreator( mersenneTwister );
			}

			temp = *( dataIndexSequence + firstRandom );
			*( dataIndexSequence + firstRandom ) = *( dataIndexSequence + secondRandom );
			*( dataIndexSequence + secondRandom ) = temp;
		}
	}


	initResult = DPP_Train_Test_Init( trainX, trainY, testX, testY );
	if( initResult == false )
	{
		return false;
	}


	for( int i = 0; i < this->m_trainDataCnt; i++ )
	{
		int		vectorIndex		= 0;
		float	*travelVector	= nullptr;


		vectorIndex = *( dataIndexSequence + sequencePosition );
		travelVector = this->m_v_WeatherData[ vectorIndex ];
		if( travelVector == nullptr )
		{
			return false;
		}

		*( *trainY + i ) = *( travelVector + 0 );

		for( int k = 1; k < this->m_weatherElementsCnt; k++ )
		{
			*( *( *trainX + i ) + ( k - 1 ) ) = *( travelVector + k );
		}

		sequencePosition++;
	}

	for( int i = 0; i < this->m_testDataCnt; i++ )
	{
		int		vectorIndex		= 0;
		float	*travelVector	= nullptr;


		vectorIndex = *( dataIndexSequence + sequencePosition );
		travelVector = this->m_v_WeatherData[ vectorIndex ];
		if( travelVector == nullptr )
		{
			return false;
		}

		*( *testY + i ) = *( travelVector + 0 );

		for( int j = 1; j < this->m_weatherElementsCnt; j++ )
		{
			*( *( *testX + i ) + ( j - 1 ) ) = *( travelVector + j );
		}

		sequencePosition++;
	}


	return true;
}


int DataPreProcess::DPP_Get_ElementsCnt()
{
	return this->m_weatherElementsCnt;
}


float DataPreProcess::DPP_Get_TargetMin( int index )
{
	return *( this->m_S_MinMaxScaler.s_Feature_Min + index );
}


float DataPreProcess::DPP_Get_TargetMax( int index )
{
	return *( this->m_S_MinMaxScaler.s_Feature_Max + index );
}


int DataPreProcess::DPP_Get_TrainDataCnt()
{
	if( this->m_trainDataCnt == 0 )
		return -1;
	else
		return this->m_trainDataCnt;
}


int DataPreProcess::DPP_Get_TestDataCnt()
{
	if( this->m_testDataCnt == 0 )
	{
		if( this->m_trainDataCnt == this->m_totalReadCnt )
			return this->m_testDataCnt;
		else
			return -1;
	}


	return this->m_testDataCnt;
}


bool DataPreProcess::DPP_Delete_S_MinMaxScaler()
{
	if( this->m_S_MinMaxScaler.s_Feature_Min == nullptr )
	{
		return false;
	}

	delete[] this->m_S_MinMaxScaler.s_Feature_Min;


	if( this->m_S_MinMaxScaler.s_Feature_Max == nullptr )
	{
		return false;
	}

	delete[] this->m_S_MinMaxScaler.s_Feature_Max;


	return true;
}


bool DataPreProcess::DPP_Delete_WeatherData()
{
	float *travelVector = nullptr;

	for( int i = 0; i < this->m_totalReadCnt; i++ )
	{
		travelVector = this->m_v_WeatherData[ i ];
		if( travelVector == nullptr )
		{
			return false;
		}

		delete[] travelVector;

		travelVector = nullptr;
	}

	return true;
}