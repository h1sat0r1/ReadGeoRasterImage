/***************************************************************************************
    Class.cpp
***************************************************************************************/

/* Include */
#include <stdio.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp> 

/* Data Type Definition */ //ここも追々入力ファイルにしたがって自動決定できるようにしたい
using Type = uchar;
#define DataDepth CV_8U//CV_32F
#define DataType(n) CV_MAKETYPE((DataDepth),(n))


////////////////////////////////////////////////////////////////////////////
//   GMat: GDALDatasetクラスを継承してOpenCVのcv::Matを要素に追加したクラス
////////////////////////////////////////////////////////////////////////////
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
//
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
	std::vector<cv::Mat> tvec;
	cv::Mat tmat = cv::Mat(y, x, DataType(1));

	// To RasterIO
	GDALRasterBand* poBand;
	Type* pafScan;
	int   nSize = x*y;

	for (int k = 0; k < c; k++)
	{
		// Malloc 
		poBand = GetRasterBand(k + 1);
		pafScan = (Type *)CPLMalloc(sizeof(Type)*nSize);

		// Reading all area
		poBand->RasterIO(GF_Read, 0, 0, x, y, pafScan, x, y, poBand->GetRasterDataType(), 0, 0);

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
	}
	//k

	// Converting std::vector<cv::Mat> to cv::Mat_<cv::Vec<Type,N>>
	cv::merge(tvec, *mat);

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
			"Projection: `%s'\n\n",
			GetProjectionRef());
	}

	if (GetGeoTransform(adfGeoTransform) == CE_None)
	{
		printf(
			"Origin = (%.6f,%.6f)\n",
			adfGeoTransform[0],
			adfGeoTransform[3]);

		printf(
			"Pixel Size = (%.6f,%.6f)\n",
			adfGeoTransform[1],
			adfGeoTransform[5]);
	}

	return;
}


////////////////////////////////////////////////////////////////////////////
//   gMatRead()
////////////////////////////////////////////////////////////////////////////
GMat* gMatRead(const char* _fname, GDALAccess _r)
{
	// GMat*型インスタンス生成，画像読み込み
	GMat* g_ = (GMat*)GDALOpen(_fname, _r);
	
	// GMat->matに読み込み
	g_->setMat();

	// Return
	return g_;
}
