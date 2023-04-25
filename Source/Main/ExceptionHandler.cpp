

#include "ExceptionHandler.h"



ExceptionHandler::ExceptionHandler()
{

}


ExceptionHandler::~ExceptionHandler()
{

}


void ExceptionHandler::Exception_Layer_Add_Fail( vector< BaseLayer* > *Model )
{
	if( (int)Model->size() > 0 )
	{
		for( int i = 0; i < (int)Model->size(); i++ )
		{
			delete Model->at( i );
		}

		Model->clear();
	}

	exit( 0 );	// 예외처리로 인한 프로그램 종료
}