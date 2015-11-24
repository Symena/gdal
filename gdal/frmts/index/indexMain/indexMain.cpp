#include "Dataset.h"
#include "ogr_srs_api.h"

#include <ios>


using namespace std;

void GDALInfoPrintMetadata(GDALDataset* hObject,
	const char *pszDomain,
	const char *pszDisplayedname,
	const char *pszIndent)
{
	int i;
	char **papszMetadata;
	int bIsxml = FALSE;

	if (pszDomain != NULL && EQUALN(pszDomain, "xml:", 4))
		bIsxml = TRUE;

	papszMetadata = hObject->GetMetadata(pszDomain);
	if (CSLCount(papszMetadata) > 0)
	{
		printf("%s%s:\n", pszIndent, pszDisplayedname);
		for (i = 0; papszMetadata[i] != NULL; i++)
		{
			if (bIsxml)
				printf("%s%s\n", pszIndent, papszMetadata[i]);
			else
				printf("%s  %s\n", pszIndent, papszMetadata[i]);
		}
	}
}

void PrintAdditionalMetadata(GDALDataset* dataset)
{
	char **papszExtraMDDomainsExpanded = NULL;
	int iMDD;


	char** papszMDDList = dataset->GetMetadataDomainList();
	char** papszIter = papszMDDList;

	while (papszIter != NULL && *papszIter != NULL)
	{
		if (!EQUAL(*papszIter, "") &&
			!EQUAL(*papszIter, "IMAGE_STRUCTURE") &&
			!EQUAL(*papszIter, "SUBDATASETS") &&
			!EQUAL(*papszIter, "GEOLOCATION") &&
			!EQUAL(*papszIter, "RPC"))
		{
			papszExtraMDDomainsExpanded = CSLAddString(papszExtraMDDomainsExpanded, *papszIter);
		}
		papszIter++;
	}
	CSLDestroy(papszMDDList);

	for (iMDD = 0; iMDD < CSLCount(papszExtraMDDomainsExpanded); iMDD++)
	{
		char pszDisplayedname[256];
		snprintf(pszDisplayedname, 256, "Metadata (%s)", papszExtraMDDomainsExpanded[iMDD]);
		GDALInfoPrintMetadata(dataset, papszExtraMDDomainsExpanded[iMDD], pszDisplayedname, "");
	}
}

static int
GDALInfoReportCorner(GDALDatasetH hDataset,
	OGRCoordinateTransformationH hTransform,
	const char * corner_name,
	double x, double y)

{
	double	dfGeoX, dfGeoY;
	double	adfGeoTransform[6];

	printf("%-11s ", corner_name);

	/* -------------------------------------------------------------------- */
	/*      Transform the point into georeferenced coordinates.             */
	/* -------------------------------------------------------------------- */
	if (GDALGetGeoTransform(hDataset, adfGeoTransform) == CE_None)
	{
		dfGeoX = adfGeoTransform[0] + adfGeoTransform[1] * x
			+ adfGeoTransform[2] * y;
		dfGeoY = adfGeoTransform[3] + adfGeoTransform[4] * x
			+ adfGeoTransform[5] * y;
	}

	else
	{
		CPLprintf("(%7.1f,%7.1f)\n", x, y);
		return FALSE;
	}

	/* -------------------------------------------------------------------- */
	/*      Report the georeferenced coordinates.                           */
	/* -------------------------------------------------------------------- */
	if (ABS(dfGeoX) < 181 && ABS(dfGeoY) < 91)
	{
		CPLprintf("(%12.7f,%12.7f) ", dfGeoX, dfGeoY);
	}
	else
	{
		CPLprintf("(%12.3f,%12.3f) ", dfGeoX, dfGeoY);
	}

	/* -------------------------------------------------------------------- */
	/*      Transform to latlong and report.                                */
	/* -------------------------------------------------------------------- */
	if (hTransform != NULL
		&& OCTTransform(hTransform, 1, &dfGeoX, &dfGeoY, NULL))
	{

		printf("(%s,", GDALDecToDMS(dfGeoX, "Long", 2));
		printf("%s)", GDALDecToDMS(dfGeoY, "Lat", 2));
	}

	printf("\n");

	return TRUE;
}

void PrintBounds(GDALDataset* dataset)
{
	OGRCoordinateTransformationH hTransform = NULL;

	/* -------------------------------------------------------------------- */
	/*      Report corners.                                                 */
	/* -------------------------------------------------------------------- */
	printf("Corner Coordinates:\n");
	GDALInfoReportCorner(dataset, hTransform, "Upper Left",
		0.0, 0.0);
	GDALInfoReportCorner(dataset, hTransform, "Lower Left",
		0.0, GDALGetRasterYSize(dataset));
	GDALInfoReportCorner(dataset, hTransform, "Upper Right",
		GDALGetRasterXSize(dataset), 0.0);
	GDALInfoReportCorner(dataset, hTransform, "Lower Right",
		GDALGetRasterXSize(dataset),
		GDALGetRasterYSize(dataset));
	GDALInfoReportCorner(dataset, hTransform, "Center",
		GDALGetRasterXSize(dataset) / 2.0,
		GDALGetRasterYSize(dataset) / 2.0);
}

int main()
{
 	GDALOpenInfo openInfo(R"(q:\Data\Scenarios\Work\Asset\GSM Drive Test data\GSM\Height\Height Data\index.txt)", GA_ReadOnly);

 	auto dataset = aircom_map::Dataset::Open(&openInfo);

	GDALInfoPrintMetadata(dataset, NULL, "Metadata", "");
	PrintAdditionalMetadata(dataset);
	PrintBounds(dataset);

	return 0;
}

