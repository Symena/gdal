/*
 * Combination of .tlh/.tli for PredAccessModule90.dll
 */

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

namespace aircom {

//
// Forward references and typedefs
//

struct __declspec(uuid("aa003e78-49f1-49a5-9f7e-7a47fc933fc7"))
/* LIBID */ __PREDACCESSMODULELib;
struct /* coclass */ CAircomPredAccess;
struct __declspec(uuid("13b1a7e2-c3c3-4ca1-8df5-0714a38bf028"))
/* interface */ IAircomPredAccess4;
struct __declspec(uuid("657bac15-c73c-46fc-add9-fbb1e325a94a"))
/* interface */ IAircomPredAccess3;
struct __declspec(uuid("944bbc31-e991-465f-a9ad-e33a90fb30ec"))
/* interface */ IAircomPredAccess2;
struct __declspec(uuid("6e0be1d8-7148-491a-8cde-0e66c1c118f3"))
/* interface */ IAircomPredAccess;
struct PredData;
enum PredDataFormat;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IAircomPredAccess, __uuidof(IAircomPredAccess));
_COM_SMARTPTR_TYPEDEF(IAircomPredAccess2, __uuidof(IAircomPredAccess2));
_COM_SMARTPTR_TYPEDEF(IAircomPredAccess3, __uuidof(IAircomPredAccess3));
_COM_SMARTPTR_TYPEDEF(IAircomPredAccess4, __uuidof(IAircomPredAccess4));

//
// Type library items
//

struct __declspec(uuid("a6ec9a6c-3262-4dee-aa29-2dec97fbed2d"))
CAircomPredAccess;
    // [ default ] interface IAircomPredAccess4
    // interface IAircomPredAccess3
    // interface IAircomPredAccess2
    // interface IAircomPredAccess

#pragma pack(push, 8)

struct PredData
{
    __int64 nX_cm;
    __int64 nY_cm;
    long nAntennaHeight_cm;
    long nGroundHeight_cm;
    unsigned long nResolution_cm;
    unsigned long nRadius_cm;
    float fFrequency_MHz;
    unsigned __int64 nModelCRC;
    unsigned long nPredFlags;
    unsigned __int64 nAntennaCRC;
    double fAntennaMechanicalTilt_deg;
    double fAntennaAzimuth_deg;
    unsigned short nCwWeight;
    float fCwRolloff;
};

#pragma pack(pop)

enum PredDataFormat
{
    PredDataFormat_PathlossCorrection = 0,
    PredDataFormat_PathlossCorrectionEx = 1
};

struct __declspec(uuid("6e0be1d8-7148-491a-8cde-0e66c1c118f3"))
IAircomPredAccess : IUnknown
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT Initialise (
        _bstr_t bstrEnterprisePredFolder,
        _bstr_t bstrUserWorkingFolder,
        unsigned short nReTryCount,
        unsigned short * pnStatusCode );
    HRESULT ExtractPathlossPrediction (
        struct PredData * pPredData,
        enum PredDataFormat eUserFileFormat,
        unsigned short * pnStatusCode );
    HRESULT ExtractPathlossPredictionEx (
        struct PredData * pPredData,
        enum PredDataFormat eUserFileFormat,
        _bstr_t bstrUserFileName,
        unsigned short * pnStatusCode );
    HRESULT InstallPathlossCorrection (
        struct PredData * pPredData,
        _bstr_t bstrUserPLCFileName,
        unsigned short * pnStatusCode );
    HRESULT InstallPathlossCorrectionEx (
        struct PredData * pPredData,
        _bstr_t bstrUserPLCFileName,
        _bstr_t bstrUserFileNameExtension,
        unsigned short * pnStatusCode );
    HRESULT UninstallPathlossCorrection (
        struct PredData * pPredData,
        unsigned short * pnStatusCode );
    HRESULT UninstallPathlossCorrectionEx (
        struct PredData * pPredData,
        _bstr_t bstrUserFileNameExtension,
        unsigned short * pnStatusCode );
    HRESULT GenWorkspaceFilename (
        struct PredData * pPredData,
        BSTR * pbstrRsltFilename,
        unsigned short * pnStatusCode );
    HRESULT GenPathlossCorrectionPathEx (
        struct PredData * pPredData,
        _bstr_t bstrUserFileNameExtension,
        BSTR * pbstrRsltFilePath,
        unsigned short * pnStatusCode );
    HRESULT IsEnterpriseFolderAccessible (
        char bReadable,
        char bWritable,
        unsigned short * pnStatusCode );
    HRESULT IsWorkingFolderAccessible (
        char bReadable,
        char bWritable,
        unsigned short * pnStatusCode );
    HRESULT GetStatusText (
        unsigned short nStatusCode,
        BSTR * pbstrRsltStatusText );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_Initialise (
        /*[in]*/ BSTR bstrEnterprisePredFolder,
        /*[in]*/ BSTR bstrUserWorkingFolder,
        /*[in]*/ unsigned short nReTryCount,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_ExtractPathlossPrediction (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ enum PredDataFormat eUserFileFormat,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_ExtractPathlossPredictionEx (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ enum PredDataFormat eUserFileFormat,
        /*[in]*/ BSTR bstrUserFileName,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_InstallPathlossCorrection (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ BSTR bstrUserPLCFileName,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_InstallPathlossCorrectionEx (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ BSTR bstrUserPLCFileName,
        /*[in]*/ BSTR bstrUserFileNameExtension,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_UninstallPathlossCorrection (
        /*[in]*/ struct PredData * pPredData,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_UninstallPathlossCorrectionEx (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ BSTR bstrUserFileNameExtension,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_GenWorkspaceFilename (
        /*[in]*/ struct PredData * pPredData,
        /*[out]*/ BSTR * pbstrRsltFilename,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_GenPathlossCorrectionPathEx (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ BSTR bstrUserFileNameExtension,
        /*[out]*/ BSTR * pbstrRsltFilePath,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_IsEnterpriseFolderAccessible (
        /*[in]*/ char bReadable,
        /*[in]*/ char bWritable,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_IsWorkingFolderAccessible (
        /*[in]*/ char bReadable,
        /*[in]*/ char bWritable,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_GetStatusText (
        /*[in]*/ unsigned short nStatusCode,
        /*[out]*/ BSTR * pbstrRsltStatusText ) = 0;
};

struct __declspec(uuid("944bbc31-e991-465f-a9ad-e33a90fb30ec"))
IAircomPredAccess2 : IAircomPredAccess
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT ExtractPathlossPredictionRaster (
        struct PredData * pPredData,
        unsigned long * pnRasterHandle,
        unsigned short * pnStatusCode );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_ExtractPathlossPredictionRaster (
        /*[in]*/ struct PredData * pPredData,
        /*[out]*/ unsigned long * pnRasterHandle,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
};

struct __declspec(uuid("657bac15-c73c-46fc-add9-fbb1e325a94a"))
IAircomPredAccess3 : IAircomPredAccess2
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT ExtractPredictionSections (
        struct PredData * pPredData,
        const _variant_t & vSectionIDspec,
        enum PredDataFormat eUserFileFormat,
        unsigned short * pnStatusCode );
    HRESULT ExtractPredictionSectionsEx (
        struct PredData * pPredData,
        const _variant_t & vSectionIDspec,
        enum PredDataFormat eUserFileFormat,
        _bstr_t bstrUserFileName,
        unsigned short * pnStatusCode );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_ExtractPredictionSections (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ VARIANT vSectionIDspec,
        /*[in]*/ enum PredDataFormat eUserFileFormat,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
      virtual HRESULT __stdcall raw_ExtractPredictionSectionsEx (
        /*[in]*/ struct PredData * pPredData,
        /*[in]*/ VARIANT vSectionIDspec,
        /*[in]*/ enum PredDataFormat eUserFileFormat,
        /*[in]*/ BSTR bstrUserFileName,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
};

struct __declspec(uuid("13b1a7e2-c3c3-4ca1-8df5-0714a38bf028"))
IAircomPredAccess4 : IAircomPredAccess3
{
    //
    // Wrapper methods for error-handling
    //

    HRESULT DoesPredictionExist (
        struct PredData * pPredData,
        unsigned short * pnStatusCode );

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall raw_DoesPredictionExist (
        /*[in]*/ struct PredData * pPredData,
        /*[out]*/ unsigned short * pnStatusCode ) = 0;
};

//
// Wrapper method implementations
//

/* .tli pasted directly below
#include "c:\dev\gdal\gdal\frmts\aircom\aircomtypelibraries\x64\debug\predaccessmodule90.tli"
 */

//
// interface IAircomPredAccess wrapper method implementations
//

inline HRESULT IAircomPredAccess::Initialise ( _bstr_t bstrEnterprisePredFolder, _bstr_t bstrUserWorkingFolder, unsigned short nReTryCount, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_Initialise(bstrEnterprisePredFolder, bstrUserWorkingFolder, nReTryCount, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::ExtractPathlossPrediction ( struct PredData * pPredData, enum PredDataFormat eUserFileFormat, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_ExtractPathlossPrediction(pPredData, eUserFileFormat, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::ExtractPathlossPredictionEx ( struct PredData * pPredData, enum PredDataFormat eUserFileFormat, _bstr_t bstrUserFileName, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_ExtractPathlossPredictionEx(pPredData, eUserFileFormat, bstrUserFileName, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::InstallPathlossCorrection ( struct PredData * pPredData, _bstr_t bstrUserPLCFileName, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_InstallPathlossCorrection(pPredData, bstrUserPLCFileName, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::InstallPathlossCorrectionEx ( struct PredData * pPredData, _bstr_t bstrUserPLCFileName, _bstr_t bstrUserFileNameExtension, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_InstallPathlossCorrectionEx(pPredData, bstrUserPLCFileName, bstrUserFileNameExtension, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::UninstallPathlossCorrection ( struct PredData * pPredData, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_UninstallPathlossCorrection(pPredData, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::UninstallPathlossCorrectionEx ( struct PredData * pPredData, _bstr_t bstrUserFileNameExtension, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_UninstallPathlossCorrectionEx(pPredData, bstrUserFileNameExtension, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::GenWorkspaceFilename ( struct PredData * pPredData, BSTR * pbstrRsltFilename, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_GenWorkspaceFilename(pPredData, pbstrRsltFilename, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::GenPathlossCorrectionPathEx ( struct PredData * pPredData, _bstr_t bstrUserFileNameExtension, BSTR * pbstrRsltFilePath, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_GenPathlossCorrectionPathEx(pPredData, bstrUserFileNameExtension, pbstrRsltFilePath, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::IsEnterpriseFolderAccessible ( char bReadable, char bWritable, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_IsEnterpriseFolderAccessible(bReadable, bWritable, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::IsWorkingFolderAccessible ( char bReadable, char bWritable, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_IsWorkingFolderAccessible(bReadable, bWritable, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess::GetStatusText ( unsigned short nStatusCode, BSTR * pbstrRsltStatusText ) {
    HRESULT _hr = raw_GetStatusText(nStatusCode, pbstrRsltStatusText);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredAccess2 wrapper method implementations
//

inline HRESULT IAircomPredAccess2::ExtractPathlossPredictionRaster ( struct PredData * pPredData, unsigned long * pnRasterHandle, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_ExtractPathlossPredictionRaster(pPredData, pnRasterHandle, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredAccess3 wrapper method implementations
//

inline HRESULT IAircomPredAccess3::ExtractPredictionSections ( struct PredData * pPredData, const _variant_t & vSectionIDspec, enum PredDataFormat eUserFileFormat, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_ExtractPredictionSections(pPredData, vSectionIDspec, eUserFileFormat, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

inline HRESULT IAircomPredAccess3::ExtractPredictionSectionsEx ( struct PredData * pPredData, const _variant_t & vSectionIDspec, enum PredDataFormat eUserFileFormat, _bstr_t bstrUserFileName, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_ExtractPredictionSectionsEx(pPredData, vSectionIDspec, eUserFileFormat, bstrUserFileName, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

//
// interface IAircomPredAccess4 wrapper method implementations
//

inline HRESULT IAircomPredAccess4::DoesPredictionExist ( struct PredData * pPredData, unsigned short * pnStatusCode ) {
    HRESULT _hr = raw_DoesPredictionExist(pPredData, pnStatusCode);
    if (FAILED(_hr)) _com_issue_errorex(_hr, this, __uuidof(this));
    return _hr;
}

} // namespace aircom

#pragma pack(pop)
