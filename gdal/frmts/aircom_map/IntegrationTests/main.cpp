#include "gdal_priv.h"

#include <iostream>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace boost::filesystem;

namespace TeocoEnterpriseMapData {
	struct IntegrationTest {

		const path input;
		const path reference;
		const path batchFilePath;
		const string mode;

		IntegrationTest(const path& baseDir, const std::string& mode)
			: input(baseDir / "input")
			, reference(baseDir / "reference")
			, batchFilePath(input / "batch.txt")
			, mode(mode)
		{
			if (!exists(input))
				throw std::invalid_argument(input.string() + " does not exist");

			if (!exists(reference))
				throw std::invalid_argument(reference.string() + " does not exist");

			if (!exists(batchFilePath))
				throw std::invalid_argument(batchFilePath.string() + " does not exist");

			if (mode != "ref" && mode != "test")
				throw std::invalid_argument(mode + " mode is invalid" );
		}

		int run()
		{
			int errorCount = 0;
			ifstream is(batchFilePath.c_str());
			is.exceptions(ifstream::failbit | ifstream::badbit);
			string line;
			// skip first line
			getline(is, line);
			while (!is.eof())
			{
				getline(is, line);
				istringstream iss(line);
				string inputFolder;
				string outname;
				int xMin, xMax, yMin, yMax, resolution;
				if (!(iss >> inputFolder >> xMin >> xMax >> yMin >> yMax >> resolution >> outname))
					throw runtime_error("Could not process batch file line: " + line);
			
				
				try
				{
					if (!readAndCompare(input / inputFolder, xMin, xMax, yMin, yMax, resolution, outname))
						errorCount++;
				}
				catch (exception& e)
				{
					cout << e.what() << endl;
					errorCount++;
				}
			}

			return errorCount;
		}

	private:
		bool readAndCompare(const path& inputFolder, int xMin, int xMax, int yMin, int yMax, int resolution, const string& outname)
		{
			auto indexFilePath = inputFolder / "index.txt";
			if (!exists(indexFilePath))
				return true;		

			if (mode == "ref")
				cout << "Creating reference " << outname << " based on " << inputFolder.filename() << endl;
			else
				cout << "Testing " << outname << endl;
			auto referenceFilePath = reference / outname;
			referenceFilePath.replace_extension(".bil");

			if (mode == "test" && !exists(referenceFilePath))
				throw std::runtime_error(referenceFilePath.string() + " does not exist");

			GDALDataset* poDataset;
			poDataset = (GDALDataset*)GDALOpen(indexFilePath.string().c_str(), GA_ReadOnly);
			if (poDataset == nullptr)
				throw std::runtime_error(CPLGetLastErrorMsg());			
			
			double geotransform[6];
			poDataset->GetGeoTransform(geotransform);
			int xOffset = xMin - (int)geotransform[0];
			int yOffset = yMin - (int)geotransform[3];
			int width = xMax - xMin;
			int height = yMax - yMin;
			int bufferWidth = std::lround(double(width) / resolution);
			int bufferHeight = std::lround(double(height) / resolution);
			int actualResolution = std::lround(double(width) / bufferWidth);

			const auto bufferLength = bufferWidth * bufferHeight;
			auto buffer = make_unique<short[]>(bufferLength);
			poDataset->RasterIO(GF_Read, xOffset, yOffset, width, height, buffer.get(), bufferWidth, bufferHeight, GDT_Int16
				, 1, nullptr, 0, 0, 0, nullptr);

			if (mode == "ref")
			{
				ofstream bil(referenceFilePath.c_str(), ios_base::binary);
				bil.exceptions(ofstream::badbit | ofstream::failbit);
				bil.write(reinterpret_cast<char*>(buffer.get()), bufferWidth * bufferHeight * sizeof(short));
				bil.flush();

				referenceFilePath.replace_extension("hdr");
				ofstream hdr(referenceFilePath.c_str());
				hdr << "ncols " << bufferWidth << endl;
				hdr << "nrows " << bufferHeight << endl;
				hdr << "ulxmap " << (xMin + (double)actualResolution / 2) << endl;
				hdr << "ulymap " << (yMin + (bufferHeight - 0.5) * actualResolution) << endl;
				hdr << "xdim " << actualResolution << endl;
				hdr << "ydim " << actualResolution << endl;
				hdr << "nbits " << (sizeof(short) * 8) << endl;
				hdr << "pixeltype SIGNEDINT" << endl;
				hdr << "undefined_values -9999" << endl;				
			}
			else
			{
				ifstream bil(referenceFilePath.c_str(), ios_base::binary);				
				bil.seekg(0, ios_base::end);
				size_t size = bil.tellg();
				if (size != bufferLength * sizeof(short))
					throw std::runtime_error("Mismatching buffer sizes");

				bil.seekg(0, ios_base::beg);
				auto referenceBuffer = std::make_unique<char[]>(size);
				bil.read(referenceBuffer.get(), size);

				auto result = memcmp(referenceBuffer.get(), buffer.get(), size);
				if (result != 0)
					throw std::runtime_error("Pixel mismatch");
			}
		
			GDALClose(poDataset);
			return true;
		}
		
	};
}

int main(int argc, char ** argv)

{
	try
	{
		if (argc != 3)
		{
			cout << "Usage: IntegrationTests.exe <PathToTestdata> <mode(ref/test)>" << endl;
			return -1;
		}

		GDALAllRegister();
		auto test = TeocoEnterpriseMapData::IntegrationTest(argv[1], argv[2]);
		return test.run();
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "... error occurred" << endl;
	}

	return -2;
}