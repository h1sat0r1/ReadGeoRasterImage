#pragma once
/***************************************************************************************
   MainHeader.h
***************************************************************************************/

/* Include */
#include <gdal.h>
#include <gdal_priv.h>
#include <gdal_alg.h>
#include <gdal_frmts.h>
#include <cpl_conv.h>
#include <iostream>
#include <stdio.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


/* Datatype Definition */
using DataType = double; //ここも追々入力ファイルにしたがって自動決定できるようにしたい


/* Class: GMat */
class GMat : public GDALDataset
{
private:

public:
	cv::Mat*  mat;
	void      setMat();
	void      dispInfo();

	// Constractor
	GMat();

	// Destractor
	~GMat();
};