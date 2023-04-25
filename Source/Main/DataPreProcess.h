

#include "../LogSystem/LogSystem.h"

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <string>
#include <vector>
#include <random>
#include <cmath>


#define		WEATHER_DATA_FILE_LIST_ADDRESS	"D:/SVN/Weather_Predict_Neural_Network/WP_NN_Data_Folder/Weather_File_List.txt"
#define		WEATHER_DATA_INFO_ADDRESS		"D:/SVN/Weather_Predict_Neural_Network/WP_NN_Data_Folder/WeatherData_Info.txt"

#define		INIT_FAIL					false
#define		OPEN_FAIL					false
#define		READ_FAIL					false
#define		CLOSE_FAIL					false
#define		SCALER_FAIL					false
#define		SPLIT_FAIL					false

#define		MIN_DEFAULT_VALUE	9999.0
#define		MAX_DEFAULT_VALUE	-9999.0


using namespace std;


enum E_EmptyCheck
{
	E_NOT_EMPTY		= 0,	// empty() == false
	E_EMPTY			= 1		// empty() == true
};


enum E_Split_Shuffle
{
	E_SHUFFLE		= 0,
	E_NOT_SHUFFLE	= 1
};


enum E_File_Number
{
	E_FIRST		= 0,
	E_SECOND	= 1,
	E_THIRD		= 2,
	E_FOURTH	= 3,
	E_FIFTH		= 4,
	E_SIXTH		= 5,
	E_SEVENTH	= 6,
	E_EIGHTH	= 7,
	E_NINTH		= 8,
	E_TENTH		= 9
};


struct S_MinMaxScaler
{
	float *s_Feature_Min = nullptr;
	float *s_Feature_Max = nullptr;
};


struct S_WeatherDataInfo
{
	int elements;
};


class DataPreProcess
{
public:
	DataPreProcess();
	~DataPreProcess();


	//E_File_Number	operator+( E_File_Number currentEnum );		// enum E_File_Number의 멤버 증감을 위한 연산자
	bool	DPP_Errno_Checker( errno_t fopenResult );					// fopen_s 함수의 리턴값인 errno의 내용 확인

	bool	DPP_Init();									// 클래스 초기화

	bool	DPP_File_Load();
	bool	DPP_File_Close( FILE *fp_filePtr );

	bool	DPP_MinMax_Scaling( int minRange, int maxRange );						// 데이터 최소최대 스케일링
	
	bool	DPP_Train_Test_Init( float ***trainX, float **trainY, float ***testX, float **testY );
	bool	DPP_Train_Test_Split( E_Split_Shuffle shuffleCheck,  float trainRatio, float testRatio, float ***trainX, float **trainY, float ***testX, float **testY );	// 학습, 테스트 데이터 분리
	
	int		DPP_Get_ElementsCnt();
	float	DPP_Get_TargetMin( int index );
	float	DPP_Get_TargetMax( int index );
	int		DPP_Get_TrainDataCnt();
	int		DPP_Get_TestDataCnt();

	bool	DPP_Delete_S_MinMaxScaler();
	bool	DPP_Delete_WeatherData();

private:

	FILE*				m_fp_WeatherData;

	S_MinMaxScaler		m_S_MinMaxScaler;
	S_WeatherDataInfo	m_S_DataInfo;

	vector<float *>		m_v_WeatherData;
	int					m_totalReadCnt;							// 현재까지 읽어온 날씨 데이터 개수
	int					m_weatherElementsCnt;

	int					m_trainDataCnt;
	int					m_testDataCnt;

};




