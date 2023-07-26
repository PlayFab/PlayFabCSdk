#include "TestAppPch.h"
#include "DataTests.h"

namespace PlayFab
{
namespace Test
{

void DataTests::AddTests()
{
    AddTest("TestAbortFileUploads", &DataTests::TestAbortFileUploads);
    AddTest("TestDeleteFiles", &DataTests::TestDeleteFiles);
    AddTest("TestFinalizeFileUploads", &DataTests::TestFinalizeFileUploads);
    AddTest("TestGetFiles", &DataTests::TestGetFiles);
    AddTest("TestGetObjects", &DataTests::TestGetObjects);
    AddTest("TestInitiateFileUploads", &DataTests::TestInitiateFileUploads);
    AddTest("TestSetObjects", &DataTests::TestSetObjects);

}

}
}
