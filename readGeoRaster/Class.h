#pragma once
/***************************************************************************************
	Class.h
***************************************************************************************/

#include <stdio.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp> 

/* Data Type Definition (ここも追々入力ファイルにしたがって自動決定できるようにしたい) */
using Type = unsigned short;       /* uchar, unsigned short, float */
#define DataDepth CV_16U  /* uchar:CV_8U, unsigned short:CV_16U, float:CV_32F */
#define DataType(n) CV_MAKETYPE((DataDepth),(n))
#define PREV_SCALE 0.3


/* Class: GMat */
class GMat : public GDALDataset
{
private:
	std::vector<cv::Mat>* prev;

public:
	cv::Mat*  mat;
	void      setMat();
	void      dispInfo();
	void      dispPrev(int _b);
	void GMat::dispPrev(int _b, int _windowtype);

	// Constractor
	GMat();

	// Destractor
	~GMat();
};