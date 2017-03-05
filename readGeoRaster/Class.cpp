/***************************************************************************************
    Class.cpp
***************************************************************************************/

/* Include */
#include "Class.h"

////////////////////////////////////////////////////////////////////////////
//   GMat: GDALDatasetクラスを継承してOpenCVのcv::Matを要素に追加したクラス
////////////////////////////////////////////////////////////////////////////
//	class GMat : public GDALDataset
//	{
//	private:
//		std::vector<cv::Mat>* prev;
//	
//	public:
//		cv::Mat*  mat;
//		void      setMat();
//		void      dispInfo();
//		void      dispPrev(int _b);
//	
//		// Constractor
//		GMat();
//	
//		// Destractor
//		~GMat();
//	};
GMat::GMat()
{
	return;
}
//
GMat::~GMat()
{
	delete mat;
	return;
}
//
void GMat::setMat()
{
	// Size
	const int x = GetRasterXSize();
	const int y = GetRasterYSize();
	const int c = GetRasterCount();

	// Mat initialization 
	mat = new cv::Mat(y, x, DataType(c)); // cv::Mat_<Tp>は使えないがこうするとシンプルになる

	// Temporal valuables
	std::vector<cv::Mat> tvec, tvec_prev;
	cv::Mat  tmat_prev = cv::Mat(y, x, CV_8U);
	cv::Mat  tmat      = cv::Mat(y, x, DataType(1));
	
	// Temporal valuables for stretching
	cv::Scalar  mean, stddev;
	double      min, max;
	double      alpha, beta;

	// To RasterIO
	GDALRasterBand* poBand;
	Type* pafScan;
	int   nSize = x*y;

/*
	printf(
		"size: x%d * y%d * c%d\n",
		x, y, c);

	printf(
		"datatype: %s\n\n",
		GDALGetDataTypeName(GetRasterBand(1)->GetRasterDataType()));
*/

	for (int k = 0; k < c; k++)
	{
		// Malloc 
		poBand = GetRasterBand(k + 1);
		pafScan = (Type *)CPLMalloc(sizeof(Type)*nSize);

		// Reading all area
		poBand->RasterIO(
			GF_Read,
			0, 0,
			x, y, 
			pafScan, 
			x, y, 
			poBand->GetRasterDataType(),
			0, 0);

		// Write to mat using pointer
		for (int j = 0; j < y; j++)
		{
			Type* p = tmat.ptr<Type>(j);
			for (int i = 0; i < x; i++)
			{
				p[i] = pafScan[i + j*x];
			}
			//i
		}
		//j

		tvec.push_back(tmat.clone());

		// Add uchar image to prev
		meanStdDev(tmat, mean, stddev);
		cv::minMaxLoc(tmat, &min, &max);
		alpha = 254.0 * PREV_SCALE / mean[0];
		beta = 1.0;
		tmat.convertTo(tmat_prev, CV_8U, alpha, beta);

#if 0
		cv::imshow("test", tmat_prev);
		cv::waitKey(); cv::destroyAllWindows();
#endif
		
		tvec_prev.push_back(tmat_prev.clone());
	}
	//k

	// Converting std::vector<cv::Mat> to cv::Mat_<cv::Vec<Type,N>>
	cv::merge(tvec, *mat);

	// Set to GMat::prev
	prev = new std::vector<cv::Mat>(tvec_prev);

	return;
}
//
void GMat::dispInfo()
{
	/* Display information about the raster file */
	double adfGeoTransform[6];

	printf(
		"Driver: %s/%s\n\n",
		GetDriver()->GetDescription(),
		GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

	printf(
		"Size: x%d * y%d * c%d\n",
		GetRasterXSize(),
		GetRasterYSize(),
		GetRasterCount());

	printf(
		"Datatype: %s\n\n",
		GDALGetDataTypeName(GetRasterBand(1)->GetRasterDataType()));


	if (GetProjectionRef() != NULL)
	{
		printf(
			"Projection: %s\n\n",
			GetProjectionRef());
	}


	if (GetGeoTransform(adfGeoTransform) == CE_None)
	{
		printf(
			"Origin     = (%.6f,%.6f)\n",
			adfGeoTransform[0],
			adfGeoTransform[3]);

		printf(
			"Pixel Size = (%.6f,%.6f)\n",
			adfGeoTransform[1],
			adfGeoTransform[5]);
	}

	return;
}
//
void GMat::dispPrev(int _b)
{
	if (_b == 0 || _b > prev->size())
	{
		printf(
			"[ERROR]Band num range: 1 to %d",
			prev->size());

		return;
	}

	const int x = GetRasterXSize();
	const int y = GetRasterYSize();

	std::string windowname = "Band: ";
	windowname += std::to_string(_b);

	cv::namedWindow(windowname, CV_WINDOW_FREERATIO);
	cv::resizeWindow(windowname, 640, (int)(640 * y / x));
	cv::imshow(windowname, (*prev)[_b - 1]);
	cv::waitKey(); cv::destroyAllWindows();

	return;
}
//
void GMat::dispPrev(int _b, int _windowtype)
{
	if (_b == 0 || _b > prev->size())
	{
		printf(
			"[ERROR]Band num range: 1 to %d",
			prev->size());

		return;
	}

	const int x = GetRasterXSize();
	const int y = GetRasterYSize();

	std::string windowname = "Band: ";
	windowname += std::to_string(_b);
	
	cv::namedWindow(windowname, _windowtype);
	cv::resizeWindow(windowname, 640, (int)(640 * y / x));
	cv::imshow(windowname, (*prev)[_b - 1]);
	cv::waitKey(); cv::destroyAllWindows();

	return;
}


////////////////////////////////////////////////////////////////////////////
//   gMatRead()
////////////////////////////////////////////////////////////////////////////
GMat* gMatRead(const char* _fname, GDALAccess _r)
{
	// GMat*型インスタンス生成，画像読み込み
	GMat* g_ = (GMat*)GDALOpen(_fname, _r);
	
	if (g_ == NULL)	return g_;

	// GMat->matに読み込み
	g_->setMat();

	// Return
	return g_;
}
