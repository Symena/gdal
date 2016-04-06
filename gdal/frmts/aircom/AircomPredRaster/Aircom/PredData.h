#pragma once

#include <cstdint>

namespace aircom {

struct PredData
{
	// Core data ...
	std::int64_t		nX_cm;					// Easting
	std::int64_t		nY_cm;					// Northing
	std::int32_t		nAntennaHeight_cm;
	std::int32_t		nGroundHeight_cm;
	std::uint32_t		nResolution_cm;
	std::uint32_t		nRadius_cm;
	float				fFrequency_MHz;
	std::uint64_t		nModelCRC;
	std::uint32_t		nPredFlags;				// flags describing targeted prediction

	// Additional data, required if the 'PredAccFlag_TargetMaskedPredFile' flag is set ...
	std::uint64_t		nAntennaCRC;
	double				fAntennaMechanicalTilt_deg;
	double				fAntennaAzimuth_deg;

	// Additional data, required if the 'PredAccFlag_TargetCorrectedPredFile' flag is set ...
	std::uint16_t		nCwWeight;
	float				fCwRolloff;
};

}