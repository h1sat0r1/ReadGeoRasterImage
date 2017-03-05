/***************************************************************************************
    Main.cpp
***************************************************************************************/


/* Include */
#include "MainHeader.h"
#include "Class.h"


/* Functions */
GMat* gMatRead(const char* _fname, GDALAccess _r);


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	GDALAllRegister();

	GMat* img = gMatRead(argv[1], GA_ReadOnly);

	if (img == NULL)
	{
		printf("[ERROR] Cannot read %s.\n", argv[1]);
		exit(-1);
	}

	// Display Information
	img->dispInfo();

	// Display Preview
	img->dispPrev(1);


	printf("\nEND\n");
}
