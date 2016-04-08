#pragma once

#include "AircomPredRaster/API.h"

#include <gmock/gmock.h>

namespace aircom { namespace pred_raster {

struct MockPredRaster : public IPredRaster5
{
	// Inherited via IPredRaster5
	virtual HRESULT QueryInterface(REFIID riid, void ** ppvObject) override {
		(*ppvObject) = this;
		return S_OK;
	}
	MOCK_METHOD0(AddRef, ULONG(void));
	MOCK_METHOD0(Release, ULONG (void));
	MOCK_METHOD1(raw_Add, HRESULT(unsigned long * pnHandle));
	MOCK_METHOD7(raw_AddSection, HRESULT(unsigned long nHandle, RasterSectionType eType, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm, unsigned long * pnSectionNumber));
	MOCK_METHOD5(raw_SetUCharValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned char value));
	MOCK_METHOD5(raw_SetShortValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, short value));
	MOCK_METHOD5(raw_SetFloatValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, float value));
	MOCK_METHOD1(raw_PointAdd, HRESULT(unsigned long * pnHandle));
	MOCK_METHOD3(raw_PointAddSection, HRESULT(unsigned long nPointHandle, RasterSectionType eType, unsigned long * pnSectionNumber));
	MOCK_METHOD4(raw_PointSetUChar, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, unsigned char nLoss));
	MOCK_METHOD4(raw_PointSetShort, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, short nLoss));
	MOCK_METHOD4(raw_PointSetFloat, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, float fLoss));
	MOCK_METHOD1(raw_DeleteHandle, HRESULT(unsigned long nRasterHandle));
	MOCK_METHOD1(raw_PointDeleteHandle, HRESULT(unsigned long nPointHandle));
	MOCK_METHOD4(raw_PointGetUChar, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, unsigned char * nLoss));
	MOCK_METHOD4(raw_PointGetShort, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, short * nLoss));
	MOCK_METHOD4(raw_PointGetFloat, HRESULT(unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, float * fLoss));
	MOCK_METHOD2(raw_AddNew, HRESULT(long bIsUnmasked, unsigned long * pnRasterHandle));
	MOCK_METHOD1(raw_Delete, HRESULT(unsigned long nRasterHandle));
	MOCK_METHOD6(raw_AddStandardSection, HRESULT(unsigned long nRasterHandle, unsigned long nSectionID, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm));
	MOCK_METHOD8(raw_AddExtendedSection, HRESULT(unsigned long nRasterHandle, unsigned long nSectionID, unsigned long nSectionNumber, RasterSectionType eType, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm));
	MOCK_METHOD5(raw_SetCharValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, char value));
	MOCK_METHOD5(raw_SetUShortValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned short value));
	MOCK_METHOD5(raw_SetIntValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, int value));
	MOCK_METHOD5(raw_SetUIntValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned int value));
	MOCK_METHOD5(raw_SetDoubleValueAt, HRESULT(unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, double value));
	MOCK_METHOD2(raw_PointAddNew, HRESULT(long bIsUnmasked, unsigned long * pnPointHandle));
	MOCK_METHOD1(raw_PointDelete, HRESULT(unsigned long nPointHandle));
	MOCK_METHOD2(raw_PointAddStandardSection, HRESULT(unsigned long nPointHandle, unsigned long nSectionID));
	MOCK_METHOD4(raw_PointAddExtendedSection, HRESULT(unsigned long nPointHandle, unsigned long nSectionID, unsigned long nSectionNumber, RasterSectionType eType));
	MOCK_METHOD1(raw_Attach, HRESULT(unsigned long nRasterHandle));
	MOCK_METHOD1(raw_Detach, HRESULT(long bDeleteHandle));
	MOCK_METHOD1(raw_GetHandle, HRESULT(unsigned long * pnRasterHandle));
	MOCK_METHOD2(raw_GetRegion, HRESULT(unsigned long nSectionNumber, IRegion2 ** ppRegion));
	MOCK_METHOD2(raw_GetRegionEx, HRESULT(unsigned long nSectionNumber, struct _REGIONEX * pRegion));
	MOCK_METHOD2(raw_GetIterator, HRESULT(unsigned long nSectionNumber, IAircomPredRasterIterator2 ** ppRasterIterator));
	MOCK_METHOD3(raw_GetIteratorEx, HRESULT(unsigned long nSectionNumber, IRegion2 * pTargetRegion, IAircomPredRasterIterator2 ** ppRasterIterator));
	MOCK_METHOD1(raw_GetNumSections, HRESULT(unsigned int * pNumSections));
	MOCK_METHOD2(raw_GetSectionNumList, HRESULT(unsigned long numSections, unsigned long * pSectionNums));
	MOCK_METHOD2(raw_GetSectionDataType, HRESULT(unsigned long nSectionNumber, RasterSectionType * peType));
	MOCK_METHOD2(raw_CreateTileIterator, HRESULT(unsigned long nSectionNumber, IPredRasterTileIterator ** ppTileIter));
};

}}