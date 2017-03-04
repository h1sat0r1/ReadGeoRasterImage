/***************************************************************************************
    Main.cpp
	GDALライブラリを使ってGeographical Raster Dataを読み込むコード
***************************************************************************************/


/* Include */
#include "MainHeader.h"


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

#if 0
	printf(
		"Size: x%d * y%d * c%d\n",
		img->mat->cols,
		img->mat->rows,
		img->mat->channels());
	
	cv::Mat r;
	cv::cvtColor(*(img->mat), r, cv::COLOR_BGR2RGB);
	cv::namedWindow("result", CV_WINDOW_FREERATIO);
	cv::imshow("result", r);
	cv::waitKey(); cv::destroyAllWindows();
#endif


	printf("\nEND\n");
}
