

#include "../LogSystem/LogSystem.h"
#include "DataPreProcess.h"
#include "../NeuralNetwork/NeuralNetwork.h"


#define		BUILD_SUCCESS		true
#define		BUILD_FAIL			false
#define		LEARN_FAIL			false
#define		DELETE_FAIL			false


using namespace std;


//bool operator++( E_File_Number &currentEnum, int );	// enum E_File_Number의 멤버 증감을 위한 연산자