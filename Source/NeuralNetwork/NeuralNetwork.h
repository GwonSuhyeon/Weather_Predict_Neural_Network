

#include "../Main/ExceptionHandler.h"

#include "../LogSystem/LogSystem.h"

#include "Layer/InputLayer.h"
#include "Layer/OutputLayer.h"
#include "Layer/DenseLayer.h"

#include "LossFunction/MeanSquaredError.h"

#include "Optimizer/MiniBatchGradientDescent.h"

#include <vector>
#include <string>
#include <iomanip>
#include <cmath>


#define		MAX_LAYER_CNT			255		// �𵨿� �߰��� �� �ִ� ���̾� �ִ� ����


enum E_PropagationType
{
	E_FOREWARD_PROP = 0,
	E_BACKWARD_PROP = 1
};


enum E_Learning_Progress
{
	E_EPOCH_PROGRESS			= 0,
	E_GRADIENT_UPDATE_PROGRESS	= 1
};


class NeuralNetwork
{
public:
	NeuralNetwork();
	~NeuralNetwork();

	
	E_Exception		NN_Get_Exception_Report();

	bool			NN_Init();
	bool			NN_Init_LossFunction( E_LossFunction lossFunction );
	bool			NN_Init_Optimizer( int BatchSize, E_Optimizer optimizer, int trainDataCnt );

	bool			NN_Model_Build();
	bool			NN_Model_Learn( int Epoch, int BatchSize, float LearningRate, E_Optimizer optimizer, E_LossFunction lossFunction, float **trainX, float *trainY, int trainDataCnt, float targetMin, float targetMax );
	int				NN_Model_Predict();

	bool			NN_Add_Layer( E_LayerType layerType, int row, int column, E_ActivationType activationType, string structureName );
	bool			NN_Max_Layer_Checker();

	bool			NN_Propagation( float *unitTrainX, float unitTrainY, int BatchSize, int dataIndex, E_PropagationType propagationType );

	bool			NN_Print_Learning_Progress( E_Learning_Progress progressType, int repeat, int Epoch, int *batchUpdateCnt, float unitTrainY, int BatchSize, int trainDataCnt, float targetMin, float targetMax );

	bool			NN_Delete_LossFunction();
	bool			NN_Delete_Optimizer();
	bool			NN_Delete_Vector_Memory();


private:

	E_Exception		m_exceptionOccurReport;	// ���� �߻� ���θ� üũ�ϱ� ���� ����

	/*
	vector<shared_ptr<ModelStructure>> Model;

	�� �ڵ带 ����Ϸ���

	Model.push_back(shared_ptr<ModelStructure>(IL));

	�̷��� �ٲ�� ��.
	*/
	//vector<unique_ptr<ModelStructure>> Model;
	vector< BaseLayer* >		Model;

	BaseLossFunction			*m_LossFunction;
	E_LossFunction				m_Type_LossFunction;

	BaseOptimizer				*m_Optimizer;
	E_Optimizer					m_Type_Optimizer;

	E_UpStreamGradientOccur		m_ComputeOccur;

};