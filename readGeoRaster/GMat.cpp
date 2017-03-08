/***************************************************************************************
    Class.cpp
***************************************************************************************/

/* Include */
#include "GMat.h"


////////////////////////////////////////////////////////////////////////////
//   GMat: GDALDatasetクラスを継承してOpenCVのcv::Matを要素に追加したクラス
////////////////////////////////////////////////////////////////////////////
//	class GMat : public GDALDataset
//	{
//	public:
//		// Valuables
//		vector<cv::Mat>* mat;
//		string*          fname;
//	
//		// Functions
//		void      setMat();
//		void      setFileName(const char* _f);
//		void      dispInfo();
//		void      dispPrev(int _b, int _windowtype);
//	
//		// Constractor
//		GMat();
//	
//		// Destractor
//		~GMat();
//	};
////////////////////////////////////////////////////////////////////////////
GMat::GMat()
{
	origin  = new cv::Point2d(0.0, 0.0);
	pixSize = new cv::Point2d(0.0, 0.0);
	return;
}


/////////////////////////////////////////////////////////
GMat::~GMat()
{
	delete mat;
	delete origin;
	delete pixSize;
	return;
}


/////////////////////////////////////////////////////////
void GMat::gmSetMat()
{
	// Size
	const int x = GetRasterXSize();
	const int y = GetRasterYSize();
	const int c = GetRasterCount();


	// Temporal valuables
	std::vector<cv::Mat> tvec;
	cv::Mat  tmat = cv::Mat(y, x, DataType(1));


	// To RasterIO
	GDALRasterBand* poBand;
	Type* pafScan;
	int   nSize = x*y;

	cout << endl;

	for (int k = 0; k < c; k++)
	{
		cout << "...Reading band " << k + 1 << "\r";
		
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

		CPLFree(pafScan);
	}
	//k

	cout << "Finish reading " << c << " bands\n\n";

	mat = new std::vector<cv::Mat>(tvec);

	return;
}


/////////////////////////////////////////////////////////
void GMat::gmSetLocation()
{
	double adfGeoTransform[6];

	if (GetGeoTransform(adfGeoTransform) == CE_None)
	{
		origin->x  = adfGeoTransform[0];
		origin->y  = adfGeoTransform[3];
		pixSize->x = adfGeoTransform[1];
		pixSize->y = adfGeoTransform[5];
	}

	return;
}


/////////////////////////////////////////////////////////
void GMat::gmInit()
{
	gmSetMat();

	gmSetLocation();

	return;
}


/////////////////////////////////////////////////////////
void GMat::gmDispInfo()
{
	/* Display information about the raster file */
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("  Information of the geospatial raster image                               \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	printf(
		"Driver     : %s/%s\n",
		GetDriver()->GetDescription(),
		GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

	printf(
		"Size       : x%d * y%d * c%d\n",
		GetRasterXSize(),
		GetRasterYSize(),
		GetRasterCount());

	printf(
		"Datatype   : %s\n",
		GDALGetDataTypeName(GetRasterBand(1)->GetRasterDataType()));


	if (GetProjectionRef() != NULL)
	{
		std::cout
			<< "Projection : "
			<< GetProjectionRef()
			<< std::endl;// << std::endl;
	}


	printf(
		"Origin     = (%20.10f, %20.10f)\n",
		origin->x,
		origin->y);

	printf(
		"Pixel Size = (%20.10f, %20.10f)\n",
		pixSize->x,
		pixSize->y);

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");


	return;
}


/////////////////////////////////////////////////////////
void GMat::gmDispPrev(int _b, int _windowtype)
{	
	
	if (_b == 0 || _b > mat->size())
	{
		std::cout
			<<"[ERROR]Band num range: 1 to "
			<< mat->size();
		return;
	}

	const int x = GetRasterXSize();
	const int y = GetRasterYSize();

	// Temporal mat
	cv::Mat tmat = mat->at(_b-1).clone();
	cv::Mat m_;

	// Temporal valuables for stretching
	cv::Scalar  mean, stddev;
	double      min, max;
	double      alpha, beta;

	// Mean, Stddev, min, max
	meanStdDev(tmat, mean, stddev);
	cv::minMaxLoc(tmat, &min, &max);
	alpha = 254.0 * PREV_SCALE / mean[0];
	beta = 1.0;
	tmat.convertTo(m_, CV_8U, alpha, beta);

	// Window name
	std::string windowname = "Band: ";
	windowname += std::to_string(_b);
	
	// Show
	cv::namedWindow(windowname, _windowtype);
	cv::moveWindow(windowname, 80, 80);
	cv::resizeWindow(windowname, WINDOW_WIDTH, (int)(WINDOW_WIDTH * y / x));
	cv::imshow(windowname, m_);
	cv::waitKey();
	//cv::destroyAllWindows();

	return;
}



////////////////////////////////////////////////////////////////////////////
//   gMatRead()
////////////////////////////////////////////////////////////////////////////
GMat* gMatRead(char* _fname, GDALAccess _r)
{
	GDALAllRegister();
	
	// GDALOpen
	GMat* g_ = (GMat*)GDALOpen(_fname, _r);

	// Exception
	if (g_ == NULL)
	{
		printf("[ERROR] Cannot read %s.\n", _fname);
		exit(-1);
	}
	
	// Init
	g_->gmInit();

	// Return
	return g_;
}
