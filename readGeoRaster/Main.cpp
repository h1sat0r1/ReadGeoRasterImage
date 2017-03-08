/***************************************************************************************
    Main.cpp
***************************************************************************************/


/* Include */
#include "MainHeader.h"
#include "GMat.h"


/* Functions */
GMat* gMatRead(char* _fname, GDALAccess _r);



//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// Read
	GMat* img = gMatRead(argv[1], GA_ReadOnly);


	// Display Information
	img->gmDispInfo();
	

	// Display Preview
	for(int c = 1; c <= img->mat->size(); c++) img->gmDispPrev(c, CV_WINDOW_FREERATIO);


	printf("\nEND\n");

#if 0
	GDALAllRegister();

	GDALDataset* poDataset = (GDALDataset*)GDALOpen(argv[1], GA_ReadOnly);

	/* Display information about the raster file */
	double adfGeoTransform[6];

	printf(
		"\nDriver: %s/%s\n\n",
		poDataset->GetDriver()->GetDescription(),
		poDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));

	printf(
		"Size: x%d * y%d * c%d\n",
		poDataset->GetRasterXSize(),
		poDataset->GetRasterYSize(),
		poDataset->GetRasterCount());

	printf(
		"Datatype: %s\n\n",
		GDALGetDataTypeName(poDataset->GetRasterBand(1)->GetRasterDataType()));


	if (poDataset->GetProjectionRef() != NULL)
	{
		std::cout
			<< "Projection: "
			<< poDataset->GetProjectionRef()
			<< std::endl << std::endl;
	}


	if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
	{
		printf(
			"Origin     = (%.12f, %.12f)\n",
			adfGeoTransform[0],
			adfGeoTransform[3]);

		printf(
			"Pixel Size = (%.12f, %.12f)\n",
			adfGeoTransform[1],
			adfGeoTransform[5]);
	}

#endif

	return 0;
}
