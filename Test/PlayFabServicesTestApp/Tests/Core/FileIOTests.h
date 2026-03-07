#pragma once

#include "CoreTestClass.h"

namespace PlayFab
{
namespace Test
{

class FileIOTests : public CoreTestClass
{
public:
    using CoreTestClass::CoreTestClass;

private:
    // TestClass overrides
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    // Test Methods
    void TestGetAvailableStorageSize(TestContext& testContext);
    void TestModifyFileTimes(TestContext& testContext);
    void TestMoveFile(TestContext& testContext);
    void TestEnumFiles(TestContext& testContext);
    void TestEnumDirectories(TestContext& testContext);
    void TestReadFileBytes(TestContext& testContext);

private:
    Vector<char> m_fileSystemBuffer;
    String m_pathRoot{ "." };
};

}
}
