/*
 * Combination of .tlh/.tli for PredRasterModule90.dll
 */

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

namespace aircom {

//
// Forward references and typedefs
//

struct __declspec(uuid("a30306e2-69dd-4294-8a97-7a9275336f2c"))
/* LIBID */ __PREDRASTERMODULELib;
struct /* coclass */ CAircomPredRaster;
struct __declspec(uuid("3d895c63-ba0e-47d2-be12-ffe644d62f97"))
/* interface */ IPredRaster5;
struct __declspec(uuid("c70e9382-091a-4fe4-a1f9-ae6ee73bb4e8"))
/* interface */ IAircomPredRaster4;
struct __declspec(uuid("5d6c8991-c045-485b-a2db-6dba7152c9f8"))
/* interface */ IAircomPredRaster3;
struct __declspec(uuid("63426358-d3af-4761-97ec-186976ade80a"))
/* interface */ IAircomPredRaster2;
struct __declspec(uuid("906114f7-61e8-46a2-abbc-eaed7dc6fd61"))
/* interface */ IAircomPredRaster;
enum RasterSectionType;
struct __declspec(uuid("b334345d-e9e9-4304-8a1e-6dd84f8be9d0"))
/* interface */ IRegion2;
struct __declspec(uuid("37216056-f9a9-420f-89c4-dada8accdd0f"))
/* interface */ IRegion;
struct _REGION;
struct _REGIONEX;
struct __declspec(uuid("50444a1f-5aac-4926-aa04-c8471cb6115c"))
/* interface */ IAircomPredRasterIterator2;
struct __declspec(uuid("7b65016e-0b5c-489d-98dd-d42a5646ebd4"))
/* interface */ IAircomPredRasterIterator;
enum IteratorValueCompare;
struct __declspec(uuid("a6d2440c-74fe-4add-a413-2277e0f835a8"))
/* interface */ IPredRasterTileIterator;
struct __declspec(uuid("366e07fd-92f4-4cf1-8a54-1b7c2e3b7af5"))
/* interface */ IRasterTile;
struct /* coclass */ CAircomPredRasterIterator;
struct /* coclass */ CPredRasterTileIterator;
struct /* coclass */ CRasterTile;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IAircomPredRaster, __uuidof(IAircomPredRaster));
_COM_SMARTPTR_TYPEDEF(IAircomPredRaster2, __uuidof(IAircomPredRaster2));
_COM_SMARTPTR_TYPEDEF(IAircomPredRaster3, __uuidof(IAircomPredRaster3));
_COM_SMARTPTR_TYPEDEF(IRegion, __uuidof(IRegion));
_COM_SMARTPTR_TYPEDEF(IRegion2, __uuidof(IRegion2));
_COM_SMARTPTR_TYPEDEF(IAircomPredRasterIterator, __uuidof(IAircomPredRasterIterator));
_COM_SMARTPTR_TYPEDEF(IAircomPredRasterIterator2, __uuidof(IAircomPredRasterIterator2));
_COM_SMARTPTR_TYPEDEF(IAircomPredRaster4, __uuidof(IAircomPredRaster4));
_COM_SMARTPTR_TYPEDEF(IRasterTile, __uuidof(IRasterTile));
_COM_SMARTPTR_TYPEDEF(IPredRasterTileIterator, __uuidof(IPredRasterTileIterator));
_COM_SMARTPTR_TYPEDEF(IPredRaster5, __uuidof(IPredRaster5));

//
// Type library items
//

struct __declspec(uuid("98a67a67-9dab-407a-aa74-aef504c165ee"))
CAircomPredRaster;
    // [ default ] interface IPredRaster5
    // interface IAircomPredRaster4
    // interface IAircomPredRaster3
    // interface IAircomPredRaster2
    // interface IAircomPredRaster

enum RasterSectionType
{
    RasterSectionType_Char = 0,
    RasterSectionType_UnsignedChar = 1,
    RasterSectionType_Short = 2,
    RasterSectionType_UnsignedShort = 3,
    RasterSectionType_Int = 4,
    RasterSectionType_UnsignedInt = 5,
    RasterSectionType_Float = 6,
    RasterSectionType_Double = 7,
    RasterSectionType_Int64 = 8,
    RasterSectionType_UnsignedInt64 = 9
};

struct __declspec(uuid("906114f7-61e8-46a2-abbc-eaed7dc6fd61"))
IAircomPredRaster : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT Add (
        unsigned long * pnHandle );
    HRESULT AddSection (
        unsigned long nHandle,
        enum RasterSectionType eType,
        __int64 iXcm,
        __int64 iYcm,
        int iRadius_cm,
        int iResolution_cm,
        unsigned long * pnSectionNumber );
    HRESULT SetUCharValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        unsigned char value );
    HRESULT SetShortValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        short value );
    HRESULT SetFloatValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        float value );
    HRESULT PointAdd (
        unsigned long * pnHandle );
    HRESULT PointAddSection (
        unsigned long nPointHandle,
        enum RasterSectionType eType,
        unsigned long * pnSectionNumber );
    HRESULT PointSetUChar (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short nMaskingAngle,
        unsigned char nLoss );
    HRESULT PointSetShort (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short nMaskingAngle,
        short nLoss );
    HRESULT PointSetFloat (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short nMaskingAngle,
        float fLoss );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_Add (
        /*[out]*/ unsigned long * pnHandle ) = 0;
      virtual HRESULT __stdcall raw_AddSection (
        /*[in]*/ unsigned long nHandle,
        /*[in]*/ enum RasterSectionType eType,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ int iRadius_cm,
        /*[in]*/ int iResolution_cm,
        /*[out]*/ unsigned long * pnSectionNumber ) = 0;
      virtual HRESULT __stdcall raw_SetUCharValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ unsigned char value ) = 0;
      virtual HRESULT __stdcall raw_SetShortValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ short value ) = 0;
      virtual HRESULT __stdcall raw_SetFloatValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ float value ) = 0;
      virtual HRESULT __stdcall raw_PointAdd (
        /*[out]*/ unsigned long * pnHandle ) = 0;
      virtual HRESULT __stdcall raw_PointAddSection (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ enum RasterSectionType eType,
        /*[out]*/ unsigned long * pnSectionNumber ) = 0;
      virtual HRESULT __stdcall raw_PointSetUChar (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ short nMaskingAngle,
        /*[in]*/ unsigned char nLoss ) = 0;
      virtual HRESULT __stdcall raw_PointSetShort (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ short nMaskingAngle,
        /*[in]*/ short nLoss ) = 0;
      virtual HRESULT __stdcall raw_PointSetFloat (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ short nMaskingAngle,
        /*[in]*/ float fLoss ) = 0;
};

struct __declspec(uuid("63426358-d3af-4761-97ec-186976ade80a"))
IAircomPredRaster2 : IAircomPredRaster
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT DeleteHandle (
        unsigned long nRasterHandle );
    HRESULT PointDeleteHandle (
        unsigned long nPointHandle );
    HRESULT PointGetUChar (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short * nMaskingAngle,
        unsigned char * nLoss );
    HRESULT PointGetShort (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short * nMaskingAngle,
        short * nLoss );
    HRESULT PointGetFloat (
        unsigned long nPointHandle,
        unsigned long nSectionNumber,
        short * nMaskingAngle,
        float * fLoss );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_DeleteHandle (
        /*[in]*/ unsigned long nRasterHandle ) = 0;
      virtual HRESULT __stdcall raw_PointDeleteHandle (
        /*[in]*/ unsigned long nPointHandle ) = 0;
      virtual HRESULT __stdcall raw_PointGetUChar (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ short * nMaskingAngle,
        /*[out]*/ unsigned char * nLoss ) = 0;
      virtual HRESULT __stdcall raw_PointGetShort (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ short * nMaskingAngle,
        /*[out]*/ short * nLoss ) = 0;
      virtual HRESULT __stdcall raw_PointGetFloat (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ short * nMaskingAngle,
        /*[out]*/ float * fLoss ) = 0;
};

struct __declspec(uuid("5d6c8991-c045-485b-a2db-6dba7152c9f8"))
IAircomPredRaster3 : IAircomPredRaster2
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT AddNew (
        long bIsUnmasked,
        unsigned long * pnRasterHandle );
    HRESULT Delete (
        unsigned long nRasterHandle );
    HRESULT AddStandardSection (
        unsigned long nRasterHandle,
        unsigned long nSectionID,
        __int64 iXcm,
        __int64 iYcm,
        int iRadius_cm,
        int iResolution_cm );
    HRESULT AddExtendedSection (
        unsigned long nRasterHandle,
        unsigned long nSectionID,
        unsigned long nSectionNumber,
        enum RasterSectionType eType,
        __int64 iXcm,
        __int64 iYcm,
        int iRadius_cm,
        int iResolution_cm );
    HRESULT SetCharValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        char value );
    HRESULT SetUShortValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        unsigned short value );
    HRESULT SetIntValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        int value );
    HRESULT SetUIntValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        unsigned int value );
    HRESULT SetDoubleValueAt (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        __int64 iXcm,
        __int64 iYcm,
        double value );
    HRESULT PointAddNew (
        long bIsUnmasked,
        unsigned long * pnPointHandle );
    HRESULT PointDelete (
        unsigned long nPointHandle );
    HRESULT PointAddStandardSection (
        unsigned long nPointHandle,
        unsigned long nSectionID );
    HRESULT PointAddExtendedSection (
        unsigned long nPointHandle,
        unsigned long nSectionID,
        unsigned long nSectionNumber,
        enum RasterSectionType eType );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_AddNew (
        /*[in]*/ long bIsUnmasked,
        /*[out]*/ unsigned long * pnRasterHandle ) = 0;
      virtual HRESULT __stdcall raw_Delete (
        /*[in]*/ unsigned long nRasterHandle ) = 0;
      virtual HRESULT __stdcall raw_AddStandardSection (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionID,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ int iRadius_cm,
        /*[in]*/ int iResolution_cm ) = 0;
      virtual HRESULT __stdcall raw_AddExtendedSection (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionID,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ enum RasterSectionType eType,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ int iRadius_cm,
        /*[in]*/ int iResolution_cm ) = 0;
      virtual HRESULT __stdcall raw_SetCharValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ char value ) = 0;
      virtual HRESULT __stdcall raw_SetUShortValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ unsigned short value ) = 0;
      virtual HRESULT __stdcall raw_SetIntValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ int value ) = 0;
      virtual HRESULT __stdcall raw_SetUIntValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ unsigned int value ) = 0;
      virtual HRESULT __stdcall raw_SetDoubleValueAt (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ __int64 iXcm,
        /*[in]*/ __int64 iYcm,
        /*[in]*/ double value ) = 0;
      virtual HRESULT __stdcall raw_PointAddNew (
        /*[in]*/ long bIsUnmasked,
        /*[out]*/ unsigned long * pnPointHandle ) = 0;
      virtual HRESULT __stdcall raw_PointDelete (
        /*[in]*/ unsigned long nPointHandle ) = 0;
      virtual HRESULT __stdcall raw_PointAddStandardSection (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionID ) = 0;
      virtual HRESULT __stdcall raw_PointAddExtendedSection (
        /*[in]*/ unsigned long nPointHandle,
        /*[in]*/ unsigned long nSectionID,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ enum RasterSectionType eType ) = 0;
};

#pragma pack(push, 4)

struct _REGION
{
    int m_eastMin;
    int m_width;
    int m_height;
    int m_northMax;
    short m_resolution;
};

#pragma pack(pop)

struct __declspec(uuid("37216056-f9a9-420f-89c4-dada8accdd0f"))
IRegion : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT SetRegion (
        struct _REGION * pRegion );
    HRESULT GetRegion (
        struct _REGION * pRegion );
    HRESULT Set (
        __int64 nEastMin,
        __int64 nNorthMax,
        int nWidth,
        int nHeight,
        short nRes );
    HRESULT Get (
        __int64 * pnEastMin,
        __int64 * pnNorthMax,
        int * pnWidth,
        int * pnHeight,
        short * pnRes );
    HRESULT SetEastMin (
        __int64 nEastMin );
    HRESULT GetEastMin (
        __int64 * pEastMin );
    HRESULT GetEastMax (
        __int64 * pEastMax );
    HRESULT GetNorthMin (
        __int64 * pNorthMin );
    HRESULT SetNorthMax (
        __int64 nNorthMax );
    HRESULT GetNorthMax (
        __int64 * pNorthMax );
    HRESULT SetResolution (
        short nRes );
    HRESULT GetResolution (
        short * pRes );
    HRESULT SetDataWidth (
        int nWidth );
    HRESULT SetDataHeight (
        int nHeight );
    HRESULT GetDataWidth (
        int * pnWidth );
    HRESULT GetDataHeight (
        int * pnHeight );
    HRESULT GetWorldWidth (
        int * pnWidth );
    HRESULT GetWorldHeight (
        int * pnHeight );
    HRESULT Copy (
        struct IRegion * pRegion );
    HRESULT Equals (
        struct IRegion * pRegion,
        long * pbResult );
    HRESULT NotEqualTo (
        struct IRegion * pRegion,
        long * pbResult );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_SetRegion (
        /*[in]*/ struct _REGION * pRegion ) = 0;
      virtual HRESULT __stdcall raw_GetRegion (
        /*[out]*/ struct _REGION * pRegion ) = 0;
      virtual HRESULT __stdcall raw_Set (
        /*[in]*/ __int64 nEastMin,
        /*[in]*/ __int64 nNorthMax,
        /*[in]*/ int nWidth,
        /*[in]*/ int nHeight,
        /*[in]*/ short nRes ) = 0;
      virtual HRESULT __stdcall raw_Get (
        /*[out]*/ __int64 * pnEastMin,
        /*[out]*/ __int64 * pnNorthMax,
        /*[out]*/ int * pnWidth,
        /*[out]*/ int * pnHeight,
        /*[out]*/ short * pnRes ) = 0;
      virtual HRESULT __stdcall raw_SetEastMin (
        /*[in]*/ __int64 nEastMin ) = 0;
      virtual HRESULT __stdcall raw_GetEastMin (
        /*[out]*/ __int64 * pEastMin ) = 0;
      virtual HRESULT __stdcall raw_GetEastMax (
        /*[out]*/ __int64 * pEastMax ) = 0;
      virtual HRESULT __stdcall raw_GetNorthMin (
        /*[out]*/ __int64 * pNorthMin ) = 0;
      virtual HRESULT __stdcall raw_SetNorthMax (
        /*[in]*/ __int64 nNorthMax ) = 0;
      virtual HRESULT __stdcall raw_GetNorthMax (
        /*[out]*/ __int64 * pNorthMax ) = 0;
      virtual HRESULT __stdcall raw_SetResolution (
        /*[in]*/ short nRes ) = 0;
      virtual HRESULT __stdcall raw_GetResolution (
        /*[out]*/ short * pRes ) = 0;
      virtual HRESULT __stdcall raw_SetDataWidth (
        /*[in]*/ int nWidth ) = 0;
      virtual HRESULT __stdcall raw_SetDataHeight (
        /*[in]*/ int nHeight ) = 0;
      virtual HRESULT __stdcall raw_GetDataWidth (
        /*[out]*/ int * pnWidth ) = 0;
      virtual HRESULT __stdcall raw_GetDataHeight (
        /*[out]*/ int * pnHeight ) = 0;
      virtual HRESULT __stdcall raw_GetWorldWidth (
        /*[out]*/ int * pnWidth ) = 0;
      virtual HRESULT __stdcall raw_GetWorldHeight (
        /*[out]*/ int * pnHeight ) = 0;
      virtual HRESULT __stdcall raw_Copy (
        /*[in]*/ struct IRegion * pRegion ) = 0;
      virtual HRESULT __stdcall raw_Equals (
        /*[in]*/ struct IRegion * pRegion,
        /*[out]*/ long * pbResult ) = 0;
      virtual HRESULT __stdcall raw_NotEqualTo (
        /*[in]*/ struct IRegion * pRegion,
        /*[out]*/ long * pbResult ) = 0;
};

#pragma pack(push, 8)

struct _REGIONEX
{
    __int64 m_eastMin;
    __int64 m_northMax;
    int m_EPSG;
    int m_width;
    int m_height;
    int m_resolution;
};

#pragma pack(pop)

struct __declspec(uuid("b334345d-e9e9-4304-8a1e-6dd84f8be9d0"))
IRegion2 : IRegion
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT SetRegionEx (
        struct _REGIONEX * pRegion );
    HRESULT GetRegionEx (
        struct _REGIONEX * pRegion );
    HRESULT SetEx (
        __int64 nEastMin,
        __int64 nNorthMax,
        int nWidth,
        int nHeight,
        int nRes );
    HRESULT GetEx (
        __int64 * pnEastMin,
        __int64 * pnNorthMax,
        int * pnWidth,
        int * pnHeight,
        int * pnRes );
    HRESULT UpdateResolution (
        int nNewRes );
    HRESULT SetResolutionEx (
        int nNewRes );
    HRESULT GetResolutionEx (
        int * pRes );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_SetRegionEx (
        /*[in]*/ struct _REGIONEX * pRegion ) = 0;
      virtual HRESULT __stdcall raw_GetRegionEx (
        /*[out]*/ struct _REGIONEX * pRegion ) = 0;
      virtual HRESULT __stdcall raw_SetEx (
        /*[in]*/ __int64 nEastMin,
        /*[in]*/ __int64 nNorthMax,
        /*[in]*/ int nWidth,
        /*[in]*/ int nHeight,
        /*[in]*/ int nRes ) = 0;
      virtual HRESULT __stdcall raw_GetEx (
        /*[out]*/ __int64 * pnEastMin,
        /*[out]*/ __int64 * pnNorthMax,
        /*[out]*/ int * pnWidth,
        /*[out]*/ int * pnHeight,
        /*[out]*/ int * pnRes ) = 0;
      virtual HRESULT __stdcall raw_UpdateResolution (
        /*[in]*/ int nNewRes ) = 0;
      virtual HRESULT __stdcall raw_SetResolutionEx (
        /*[in]*/ int nNewRes ) = 0;
      virtual HRESULT __stdcall raw_GetResolutionEx (
        /*[out]*/ int * pRes ) = 0;
};

enum IteratorValueCompare
{
    IteratorValueCompare_None = 0,
    IteratorValueCompare_LessThan = 1,
    IteratorValueCompare_Equals = 2,
    IteratorValueCompare_GreaterThan = 3
};

struct __declspec(uuid("7b65016e-0b5c-489d-98dd-d42a5646ebd4"))
IAircomPredRasterIterator : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT Attach (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber );
    HRESULT SetUCharSkip (
        enum IteratorValueCompare eValueCompare,
        unsigned char ucContextValue );
    HRESULT SetShortSkip (
        enum IteratorValueCompare eValueCompare,
        short nContextValue );
    HRESULT SetFloatSkip (
        enum IteratorValueCompare eValueCompare,
        float fContextValue );
    HRESULT GetNextUChar (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        unsigned char * pValueAtXY );
    HRESULT GetNextShort (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        short * pValueAtXY );
    HRESULT GetNextFloat (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        float * pValueAtXY );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_Attach (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber ) = 0;
      virtual HRESULT __stdcall raw_SetUCharSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ unsigned char ucContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetShortSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ short nContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetFloatSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ float fContextValue ) = 0;
      virtual HRESULT __stdcall raw_GetNextUChar (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ unsigned char * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextShort (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ short * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextFloat (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ float * pValueAtXY ) = 0;
};

struct __declspec(uuid("50444a1f-5aac-4926-aa04-c8471cb6115c"))
IAircomPredRasterIterator2 : IAircomPredRasterIterator
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT AttachEx (
        unsigned long nRasterHandle,
        unsigned long nSectionNumber,
        struct IRegion2 * pTargetRegion );
    HRESULT GetRegion (
        struct IRegion2 * * ppRegion );
    HRESULT SetCharSkip (
        enum IteratorValueCompare eValueCompare,
        char cContextValue );
    HRESULT SetUShortSkip (
        enum IteratorValueCompare eValueCompare,
        unsigned short nContextValue );
    HRESULT SetIntSkip (
        enum IteratorValueCompare eValueCompare,
        int nContextValue );
    HRESULT SetUIntSkip (
        enum IteratorValueCompare eValueCompare,
        unsigned int nContextValue );
    HRESULT SetDoubleSkip (
        enum IteratorValueCompare eValueCompare,
        double fContextValue );
    HRESULT GetNextChar (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        char * pValueAtXY );
    HRESULT GetNextUShort (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        unsigned short * pValueAtXY );
    HRESULT GetNextInt (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        int * pValueAtXY );
    HRESULT GetNextUInt (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        unsigned int * pValueAtXY );
    HRESULT GetNextDouble (
        __int64 * pnX_cm,
        __int64 * pnY_cm,
        double * pValueAtXY );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_AttachEx (
        /*[in]*/ unsigned long nRasterHandle,
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ struct IRegion2 * pTargetRegion ) = 0;
      virtual HRESULT __stdcall raw_GetRegion (
        /*[out]*/ struct IRegion2 * * ppRegion ) = 0;
      virtual HRESULT __stdcall raw_SetCharSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ char cContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetUShortSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ unsigned short nContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetIntSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ int nContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetUIntSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ unsigned int nContextValue ) = 0;
      virtual HRESULT __stdcall raw_SetDoubleSkip (
        /*[in]*/ enum IteratorValueCompare eValueCompare,
        /*[in]*/ double fContextValue ) = 0;
      virtual HRESULT __stdcall raw_GetNextChar (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ char * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextUShort (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ unsigned short * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextInt (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ int * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextUInt (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ unsigned int * pValueAtXY ) = 0;
      virtual HRESULT __stdcall raw_GetNextDouble (
        /*[out]*/ __int64 * pnX_cm,
        /*[out]*/ __int64 * pnY_cm,
        /*[out]*/ double * pValueAtXY ) = 0;
};

struct __declspec(uuid("c70e9382-091a-4fe4-a1f9-ae6ee73bb4e8"))
IAircomPredRaster4 : IAircomPredRaster3
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT Attach (
        unsigned long nRasterHandle );
    HRESULT Detach (
        long bDeleteHandle );
    HRESULT GetHandle (
        unsigned long * pnRasterHandle );
    HRESULT GetRegion (
        unsigned long nSectionNumber,
        struct IRegion2 * * ppRegion );
    HRESULT GetIterator (
        unsigned long nSectionNumber,
        struct IAircomPredRasterIterator2 * * ppRasterIterator );
    HRESULT GetIteratorEx (
        unsigned long nSectionNumber,
        struct IRegion2 * pTargetRegion,
        struct IAircomPredRasterIterator2 * * ppRasterIterator );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_Attach (
        /*[in]*/ unsigned long nRasterHandle ) = 0;
      virtual HRESULT __stdcall raw_Detach (
        /*[in]*/ long bDeleteHandle ) = 0;
      virtual HRESULT __stdcall raw_GetHandle (
        /*[out]*/ unsigned long * pnRasterHandle ) = 0;
      virtual HRESULT __stdcall raw_GetRegion (
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ struct IRegion2 * * ppRegion ) = 0;
      virtual HRESULT __stdcall raw_GetIterator (
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ struct IAircomPredRasterIterator2 * * ppRasterIterator ) = 0;
      virtual HRESULT __stdcall raw_GetIteratorEx (
        /*[in]*/ unsigned long nSectionNumber,
        /*[in]*/ struct IRegion2 * pTargetRegion,
        /*[out]*/ struct IAircomPredRasterIterator2 * * ppRasterIterator ) = 0;
};

struct __declspec(uuid("366e07fd-92f4-4cf1-8a54-1b7c2e3b7af5"))
IRasterTile : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT GetTileRegion (
        struct _REGIONEX * pRegion );
    unsigned __int64 GetPixelCount ( );
    HRESULT GetCharData (
        unsigned long numElements,
        char * pRasterData );
    HRESULT GetUCharData (
        unsigned long numElements,
        unsigned char * pRasterData );
    HRESULT GetShortData (
        unsigned long numElements,
        short * pRasterData );
    HRESULT GetUShortData (
        unsigned long numElements,
        unsigned short * pRasterData );
    HRESULT GetIntData (
        unsigned long numElements,
        int * pRasterData );
    HRESULT GetUIntData (
        unsigned long numElements,
        unsigned int * pRasterData );
    HRESULT GetInt64Data (
        unsigned long numElements,
        __int64 * pRasterData );
    HRESULT GetUInt64Data (
        unsigned long numElements,
        unsigned __int64 * pRasterData );
    HRESULT GetFloatData (
        unsigned long numElements,
        float * pRasterData );
    HRESULT GetDoubleData (
        unsigned long numElements,
        double * pRasterData );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_GetTileRegion (
        /*[out]*/ struct _REGIONEX * pRegion ) = 0;
      virtual HRESULT __stdcall raw_GetPixelCount (
        /*[out,retval]*/ unsigned __int64 * pNumPix ) = 0;
      virtual HRESULT __stdcall raw_GetCharData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ char * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetUCharData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ unsigned char * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetShortData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ short * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetUShortData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ unsigned short * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetIntData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ int * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetUIntData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ unsigned int * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetInt64Data (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ __int64 * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetUInt64Data (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ unsigned __int64 * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetFloatData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ float * pRasterData ) = 0;
      virtual HRESULT __stdcall raw_GetDoubleData (
        /*[in]*/ unsigned long numElements,
        /*[in,out]*/ double * pRasterData ) = 0;
};

struct __declspec(uuid("a6d2440c-74fe-4add-a413-2277e0f835a8"))
IPredRasterTileIterator : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    IRasterTilePtr GetNextTile ( );
    IRasterTilePtr GetTile (
        unsigned int tile_x,
        unsigned int tile_y );
    unsigned int GetNumTiles ( );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_GetNextTile (
        /*[out,retval]*/ struct IRasterTile * * ppTile ) = 0;
      virtual HRESULT __stdcall raw_GetTile (
        /*[in]*/ unsigned int tile_x,
        /*[in]*/ unsigned int tile_y,
        /*[out,retval]*/ struct IRasterTile * * ppTile ) = 0;
      virtual HRESULT __stdcall raw_GetNumTiles (
        /*[out,retval]*/ unsigned int * pNumTiles ) = 0;
};

struct __declspec(uuid("3d895c63-ba0e-47d2-be12-ffe644d62f97"))
IPredRaster5 : IAircomPredRaster4
{
    //
    // Wrapper methods for error-handling
    //

    unsigned int GetNumSections ( );
    HRESULT GetSectionNumList (
        unsigned long numSections,
        unsigned long * pSectionNums );
    enum RasterSectionType GetSectionDataType (
        unsigned long nSectionNumber );
    HRESULT GetRegionEx (
        unsigned long nSectionNumber,
        struct _REGIONEX * pRegion );
    IPredRasterTileIteratorPtr CreateTileIterator (
        unsigned long nSectionNumber );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_GetNumSections (
        /*[out,retval]*/ unsigned int * pNumSections ) = 0;
      virtual HRESULT __stdcall raw_GetSectionNumList (
        /*[in]*/ unsigned long numSections,
        /*[in,out]*/ unsigned long * pSectionNums ) = 0;
      virtual HRESULT __stdcall raw_GetSectionDataType (
        /*[in]*/ unsigned long nSectionNumber,
        /*[out,retval]*/ enum RasterSectionType * peType ) = 0;
      virtual HRESULT __stdcall raw_GetRegionEx (
        /*[in]*/ unsigned long nSectionNumber,
        /*[out]*/ struct _REGIONEX * pRegion ) = 0;
      virtual HRESULT __stdcall raw_CreateTileIterator (
        /*[in]*/ unsigned long nSectionNumber,
        /*[out,retval]*/ struct IPredRasterTileIterator * * ppTileIter ) = 0;
};

struct __declspec(uuid("356a9ff3-cad9-4f3a-a8c5-9c962984ee37"))
CAircomPredRasterIterator;
    // [ default ] interface IAircomPredRasterIterator2
    // interface IAircomPredRasterIterator

struct __declspec(uuid("a363c973-720d-48a0-95d2-b82d82b72ea5"))
CPredRasterTileIterator;
    // [ default ] interface IPredRasterTileIterator

struct __declspec(uuid("ad9b37ff-8624-4375-8eb8-10192d7313f2"))
CRasterTile;
    // [ default ] interface IRasterTile

//
// Wrapper method implementations
//

/* .tli pasted directly below
#include "c:\dev\gdal\gdal\frmts\aircom\aircomtypelibraries\x64\debug\predrastermodule90.tli"
 */

//
// interface IAircomPredRaster wrapper method implementations
//

inline HRESULT IAircomPredRaster::Add ( unsigned long * pnHandle ) {
    HRESULT _hr = raw_Add(pnHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::AddSection ( unsigned long nHandle, enum RasterSectionType eType, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm, unsigned long * pnSectionNumber ) {
    HRESULT _hr = raw_AddSection(nHandle, eType, iXcm, iYcm, iRadius_cm, iResolution_cm, pnSectionNumber);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::SetUCharValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned char value ) {
    HRESULT _hr = raw_SetUCharValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::SetShortValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, short value ) {
    HRESULT _hr = raw_SetShortValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::SetFloatValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, float value ) {
    HRESULT _hr = raw_SetFloatValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::PointAdd ( unsigned long * pnHandle ) {
    HRESULT _hr = raw_PointAdd(pnHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::PointAddSection ( unsigned long nPointHandle, enum RasterSectionType eType, unsigned long * pnSectionNumber ) {
    HRESULT _hr = raw_PointAddSection(nPointHandle, eType, pnSectionNumber);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::PointSetUChar ( unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, unsigned char nLoss ) {
    HRESULT _hr = raw_PointSetUChar(nPointHandle, nSectionNumber, nMaskingAngle, nLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::PointSetShort ( unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, short nLoss ) {
    HRESULT _hr = raw_PointSetShort(nPointHandle, nSectionNumber, nMaskingAngle, nLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster::PointSetFloat ( unsigned long nPointHandle, unsigned long nSectionNumber, short nMaskingAngle, float fLoss ) {
    HRESULT _hr = raw_PointSetFloat(nPointHandle, nSectionNumber, nMaskingAngle, fLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredRaster2 wrapper method implementations
//

inline HRESULT IAircomPredRaster2::DeleteHandle ( unsigned long nRasterHandle ) {
    HRESULT _hr = raw_DeleteHandle(nRasterHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster2::PointDeleteHandle ( unsigned long nPointHandle ) {
    HRESULT _hr = raw_PointDeleteHandle(nPointHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster2::PointGetUChar ( unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, unsigned char * nLoss ) {
    HRESULT _hr = raw_PointGetUChar(nPointHandle, nSectionNumber, nMaskingAngle, nLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster2::PointGetShort ( unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, short * nLoss ) {
    HRESULT _hr = raw_PointGetShort(nPointHandle, nSectionNumber, nMaskingAngle, nLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster2::PointGetFloat ( unsigned long nPointHandle, unsigned long nSectionNumber, short * nMaskingAngle, float * fLoss ) {
    HRESULT _hr = raw_PointGetFloat(nPointHandle, nSectionNumber, nMaskingAngle, fLoss);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredRaster3 wrapper method implementations
//

inline HRESULT IAircomPredRaster3::AddNew ( long bIsUnmasked, unsigned long * pnRasterHandle ) {
    HRESULT _hr = raw_AddNew(bIsUnmasked, pnRasterHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::Delete ( unsigned long nRasterHandle ) {
    HRESULT _hr = raw_Delete(nRasterHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::AddStandardSection ( unsigned long nRasterHandle, unsigned long nSectionID, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm ) {
    HRESULT _hr = raw_AddStandardSection(nRasterHandle, nSectionID, iXcm, iYcm, iRadius_cm, iResolution_cm);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::AddExtendedSection ( unsigned long nRasterHandle, unsigned long nSectionID, unsigned long nSectionNumber, enum RasterSectionType eType, __int64 iXcm, __int64 iYcm, int iRadius_cm, int iResolution_cm ) {
    HRESULT _hr = raw_AddExtendedSection(nRasterHandle, nSectionID, nSectionNumber, eType, iXcm, iYcm, iRadius_cm, iResolution_cm);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::SetCharValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, char value ) {
    HRESULT _hr = raw_SetCharValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::SetUShortValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned short value ) {
    HRESULT _hr = raw_SetUShortValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::SetIntValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, int value ) {
    HRESULT _hr = raw_SetIntValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::SetUIntValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, unsigned int value ) {
    HRESULT _hr = raw_SetUIntValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::SetDoubleValueAt ( unsigned long nRasterHandle, unsigned long nSectionNumber, __int64 iXcm, __int64 iYcm, double value ) {
    HRESULT _hr = raw_SetDoubleValueAt(nRasterHandle, nSectionNumber, iXcm, iYcm, value);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::PointAddNew ( long bIsUnmasked, unsigned long * pnPointHandle ) {
    HRESULT _hr = raw_PointAddNew(bIsUnmasked, pnPointHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::PointDelete ( unsigned long nPointHandle ) {
    HRESULT _hr = raw_PointDelete(nPointHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::PointAddStandardSection ( unsigned long nPointHandle, unsigned long nSectionID ) {
    HRESULT _hr = raw_PointAddStandardSection(nPointHandle, nSectionID);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster3::PointAddExtendedSection ( unsigned long nPointHandle, unsigned long nSectionID, unsigned long nSectionNumber, enum RasterSectionType eType ) {
    HRESULT _hr = raw_PointAddExtendedSection(nPointHandle, nSectionID, nSectionNumber, eType);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IRegion wrapper method implementations
//

inline HRESULT IRegion::SetRegion ( struct _REGION * pRegion ) {
    HRESULT _hr = raw_SetRegion(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetRegion ( struct _REGION * pRegion ) {
    HRESULT _hr = raw_GetRegion(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::Set ( __int64 nEastMin, __int64 nNorthMax, int nWidth, int nHeight, short nRes ) {
    HRESULT _hr = raw_Set(nEastMin, nNorthMax, nWidth, nHeight, nRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::Get ( __int64 * pnEastMin, __int64 * pnNorthMax, int * pnWidth, int * pnHeight, short * pnRes ) {
    HRESULT _hr = raw_Get(pnEastMin, pnNorthMax, pnWidth, pnHeight, pnRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::SetEastMin ( __int64 nEastMin ) {
    HRESULT _hr = raw_SetEastMin(nEastMin);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetEastMin ( __int64 * pEastMin ) {
    HRESULT _hr = raw_GetEastMin(pEastMin);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetEastMax ( __int64 * pEastMax ) {
    HRESULT _hr = raw_GetEastMax(pEastMax);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetNorthMin ( __int64 * pNorthMin ) {
    HRESULT _hr = raw_GetNorthMin(pNorthMin);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::SetNorthMax ( __int64 nNorthMax ) {
    HRESULT _hr = raw_SetNorthMax(nNorthMax);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetNorthMax ( __int64 * pNorthMax ) {
    HRESULT _hr = raw_GetNorthMax(pNorthMax);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::SetResolution ( short nRes ) {
    HRESULT _hr = raw_SetResolution(nRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetResolution ( short * pRes ) {
    HRESULT _hr = raw_GetResolution(pRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::SetDataWidth ( int nWidth ) {
    HRESULT _hr = raw_SetDataWidth(nWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::SetDataHeight ( int nHeight ) {
    HRESULT _hr = raw_SetDataHeight(nHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetDataWidth ( int * pnWidth ) {
    HRESULT _hr = raw_GetDataWidth(pnWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetDataHeight ( int * pnHeight ) {
    HRESULT _hr = raw_GetDataHeight(pnHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetWorldWidth ( int * pnWidth ) {
    HRESULT _hr = raw_GetWorldWidth(pnWidth);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::GetWorldHeight ( int * pnHeight ) {
    HRESULT _hr = raw_GetWorldHeight(pnHeight);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::Copy ( struct IRegion * pRegion ) {
    HRESULT _hr = raw_Copy(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::Equals ( struct IRegion * pRegion, long * pbResult ) {
    HRESULT _hr = raw_Equals(pRegion, pbResult);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion::NotEqualTo ( struct IRegion * pRegion, long * pbResult ) {
    HRESULT _hr = raw_NotEqualTo(pRegion, pbResult);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IRegion2 wrapper method implementations
//

inline HRESULT IRegion2::SetRegionEx ( struct _REGIONEX * pRegion ) {
    HRESULT _hr = raw_SetRegionEx(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::GetRegionEx ( struct _REGIONEX * pRegion ) {
    HRESULT _hr = raw_GetRegionEx(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::SetEx ( __int64 nEastMin, __int64 nNorthMax, int nWidth, int nHeight, int nRes ) {
    HRESULT _hr = raw_SetEx(nEastMin, nNorthMax, nWidth, nHeight, nRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::GetEx ( __int64 * pnEastMin, __int64 * pnNorthMax, int * pnWidth, int * pnHeight, int * pnRes ) {
    HRESULT _hr = raw_GetEx(pnEastMin, pnNorthMax, pnWidth, pnHeight, pnRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::UpdateResolution ( int nNewRes ) {
    HRESULT _hr = raw_UpdateResolution(nNewRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::SetResolutionEx ( int nNewRes ) {
    HRESULT _hr = raw_SetResolutionEx(nNewRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRegion2::GetResolutionEx ( int * pRes ) {
    HRESULT _hr = raw_GetResolutionEx(pRes);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredRasterIterator wrapper method implementations
//

inline HRESULT IAircomPredRasterIterator::Attach ( unsigned long nRasterHandle, unsigned long nSectionNumber ) {
    HRESULT _hr = raw_Attach(nRasterHandle, nSectionNumber);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::SetUCharSkip ( enum IteratorValueCompare eValueCompare, unsigned char ucContextValue ) {
    HRESULT _hr = raw_SetUCharSkip(eValueCompare, ucContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::SetShortSkip ( enum IteratorValueCompare eValueCompare, short nContextValue ) {
    HRESULT _hr = raw_SetShortSkip(eValueCompare, nContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::SetFloatSkip ( enum IteratorValueCompare eValueCompare, float fContextValue ) {
    HRESULT _hr = raw_SetFloatSkip(eValueCompare, fContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::GetNextUChar ( __int64 * pnX_cm, __int64 * pnY_cm, unsigned char * pValueAtXY ) {
    HRESULT _hr = raw_GetNextUChar(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::GetNextShort ( __int64 * pnX_cm, __int64 * pnY_cm, short * pValueAtXY ) {
    HRESULT _hr = raw_GetNextShort(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator::GetNextFloat ( __int64 * pnX_cm, __int64 * pnY_cm, float * pValueAtXY ) {
    HRESULT _hr = raw_GetNextFloat(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredRasterIterator2 wrapper method implementations
//

inline HRESULT IAircomPredRasterIterator2::AttachEx ( unsigned long nRasterHandle, unsigned long nSectionNumber, struct IRegion2 * pTargetRegion ) {
    HRESULT _hr = raw_AttachEx(nRasterHandle, nSectionNumber, pTargetRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetRegion ( struct IRegion2 * * ppRegion ) {
    HRESULT _hr = raw_GetRegion(ppRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::SetCharSkip ( enum IteratorValueCompare eValueCompare, char cContextValue ) {
    HRESULT _hr = raw_SetCharSkip(eValueCompare, cContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::SetUShortSkip ( enum IteratorValueCompare eValueCompare, unsigned short nContextValue ) {
    HRESULT _hr = raw_SetUShortSkip(eValueCompare, nContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::SetIntSkip ( enum IteratorValueCompare eValueCompare, int nContextValue ) {
    HRESULT _hr = raw_SetIntSkip(eValueCompare, nContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::SetUIntSkip ( enum IteratorValueCompare eValueCompare, unsigned int nContextValue ) {
    HRESULT _hr = raw_SetUIntSkip(eValueCompare, nContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::SetDoubleSkip ( enum IteratorValueCompare eValueCompare, double fContextValue ) {
    HRESULT _hr = raw_SetDoubleSkip(eValueCompare, fContextValue);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetNextChar ( __int64 * pnX_cm, __int64 * pnY_cm, char * pValueAtXY ) {
    HRESULT _hr = raw_GetNextChar(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetNextUShort ( __int64 * pnX_cm, __int64 * pnY_cm, unsigned short * pValueAtXY ) {
    HRESULT _hr = raw_GetNextUShort(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetNextInt ( __int64 * pnX_cm, __int64 * pnY_cm, int * pValueAtXY ) {
    HRESULT _hr = raw_GetNextInt(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetNextUInt ( __int64 * pnX_cm, __int64 * pnY_cm, unsigned int * pValueAtXY ) {
    HRESULT _hr = raw_GetNextUInt(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRasterIterator2::GetNextDouble ( __int64 * pnX_cm, __int64 * pnY_cm, double * pValueAtXY ) {
    HRESULT _hr = raw_GetNextDouble(pnX_cm, pnY_cm, pValueAtXY);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredRaster4 wrapper method implementations
//

inline HRESULT IAircomPredRaster4::Attach ( unsigned long nRasterHandle ) {
    HRESULT _hr = raw_Attach(nRasterHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster4::Detach ( long bDeleteHandle ) {
    HRESULT _hr = raw_Detach(bDeleteHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster4::GetHandle ( unsigned long * pnRasterHandle ) {
    HRESULT _hr = raw_GetHandle(pnRasterHandle);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster4::GetRegion ( unsigned long nSectionNumber, struct IRegion2 * * ppRegion ) {
    HRESULT _hr = raw_GetRegion(nSectionNumber, ppRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster4::GetIterator ( unsigned long nSectionNumber, struct IAircomPredRasterIterator2 * * ppRasterIterator ) {
    HRESULT _hr = raw_GetIterator(nSectionNumber, ppRasterIterator);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredRaster4::GetIteratorEx ( unsigned long nSectionNumber, struct IRegion2 * pTargetRegion, struct IAircomPredRasterIterator2 * * ppRasterIterator ) {
    HRESULT _hr = raw_GetIteratorEx(nSectionNumber, pTargetRegion, ppRasterIterator);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IRasterTile wrapper method implementations
//

inline HRESULT IRasterTile::GetTileRegion ( struct _REGIONEX * pRegion ) {
    HRESULT _hr = raw_GetTileRegion(pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline unsigned __int64 IRasterTile::GetPixelCount ( ) {
    unsigned __int64 _result = 0;
    HRESULT _hr = raw_GetPixelCount(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IRasterTile::GetCharData ( unsigned long numElements, char * pRasterData ) {
    HRESULT _hr = raw_GetCharData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetUCharData ( unsigned long numElements, unsigned char * pRasterData ) {
    HRESULT _hr = raw_GetUCharData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetShortData ( unsigned long numElements, short * pRasterData ) {
    HRESULT _hr = raw_GetShortData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetUShortData ( unsigned long numElements, unsigned short * pRasterData ) {
    HRESULT _hr = raw_GetUShortData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetIntData ( unsigned long numElements, int * pRasterData ) {
    HRESULT _hr = raw_GetIntData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetUIntData ( unsigned long numElements, unsigned int * pRasterData ) {
    HRESULT _hr = raw_GetUIntData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetInt64Data ( unsigned long numElements, __int64 * pRasterData ) {
    HRESULT _hr = raw_GetInt64Data(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetUInt64Data ( unsigned long numElements, unsigned __int64 * pRasterData ) {
    HRESULT _hr = raw_GetUInt64Data(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetFloatData ( unsigned long numElements, float * pRasterData ) {
    HRESULT _hr = raw_GetFloatData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IRasterTile::GetDoubleData ( unsigned long numElements, double * pRasterData ) {
    HRESULT _hr = raw_GetDoubleData(numElements, pRasterData);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IPredRasterTileIterator wrapper method implementations
//

inline IRasterTilePtr IPredRasterTileIterator::GetNextTile ( ) {
    struct IRasterTile * _result = 0;
    HRESULT _hr = raw_GetNextTile(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IRasterTilePtr(_result, false);
}

inline IRasterTilePtr IPredRasterTileIterator::GetTile ( unsigned int tile_x, unsigned int tile_y ) {
    struct IRasterTile * _result = 0;
    HRESULT _hr = raw_GetTile(tile_x, tile_y, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IRasterTilePtr(_result, false);
}

inline unsigned int IPredRasterTileIterator::GetNumTiles ( ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_GetNumTiles(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

//
// interface IPredRaster5 wrapper method implementations
//

inline unsigned int IPredRaster5::GetNumSections ( ) {
    unsigned int _result = 0;
    HRESULT _hr = raw_GetNumSections(&_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IPredRaster5::GetSectionNumList ( unsigned long numSections, unsigned long * pSectionNums ) {
    HRESULT _hr = raw_GetSectionNumList(numSections, pSectionNums);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline enum RasterSectionType IPredRaster5::GetSectionDataType ( unsigned long nSectionNumber ) {
    enum RasterSectionType _result;
    HRESULT _hr = raw_GetSectionDataType(nSectionNumber, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _result;
}

inline HRESULT IPredRaster5::GetRegionEx ( unsigned long nSectionNumber, struct _REGIONEX * pRegion ) {
    HRESULT _hr = raw_GetRegionEx(nSectionNumber, pRegion);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline IPredRasterTileIteratorPtr IPredRaster5::CreateTileIterator ( unsigned long nSectionNumber ) {
    struct IPredRasterTileIterator * _result = 0;
    HRESULT _hr = raw_CreateTileIterator(nSectionNumber, &_result);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return IPredRasterTileIteratorPtr(_result, false);
}

} // namespace aircom

#pragma pack(pop)
