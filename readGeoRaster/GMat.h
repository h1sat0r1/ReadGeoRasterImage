#pragma once
/***************************************************************************************
	Class.h
***************************************************************************************/

#include <stdio.h>
#include <iostream>
#include <gdal.h>
#include <gdal_priv.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp> 


/* Name space */
using namespace std;


/* Data Type Definition (ここも追々入力ファイルにしたがって自動決定できるようにしたい) */
using Type = ushort;      /* uchar, ushort, float */
#define DataDepth CV_16U  /* uchar:CV_8U, unsigned short:CV_16U, float:CV_32F */
#define DataType(n) CV_MAKETYPE((DataDepth),(n))


/* Const num */
#define PREV_SCALE   0.4
#define WINDOW_WIDTH 640


/* Class: GMat */
class GMat : public GDALDataset
{
	void gmSetMat();
	void gmSetLocation();

public:
	// Valuables
	vector<cv::Mat>* mat;
	cv::Point2d*     origin;
	cv::Point2d*     pixSize;

	// Functions
	void gmInit();
	void gmDispInfo();
	void gmDispPrev(int _b, int _windowtype);


	// Constractor
	GMat();

	// Destractor
	~GMat();
};