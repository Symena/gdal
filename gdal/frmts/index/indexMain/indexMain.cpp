#include "IndexDataset.h"

int main()
{
 	GDALOpenInfo openInfo(R"(q:\Data\Scenarios\Work\Asset\GSM Drive Test data\GSM\Height\Height Data\index.txt)", GA_ReadOnly);
// 
 	auto dataset = IndexDataset::Open(&openInfo);
	dataset->SetMetadataItem("name", "value", "domain");
	dataset->SetMetadataItem("name1", "value2", "domain");

	auto t = dataset->GetMetadata("domain");
	return 0;
}

