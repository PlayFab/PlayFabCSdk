cmake_minimum_required(VERSION 3.6)

# file list similar to PlayFabServicesTestApp.Common.vcxitems
function(GET_PLAYFAB_SERVICES_TEST_APP_COMMMON_FILES
         OUT_PLAYFAB_SERVICES_TEST_APP_COMMMON_FILES
         PATH_TO_SOURCE
         )

    set(${OUT_PLAYFAB_SERVICES_TEST_APP_COMMMON_FILES}
        "${PATH_TO_SOURCE}/Generated/GeneratedTestsBootstrapper.h"
        "${PATH_TO_SOURCE}/Operations/Core/AuthenticationOperations.cpp"
        "${PATH_TO_SOURCE}/Operations/Core/AuthenticationOperations.h"
        "${PATH_TO_SOURCE}/Operations/Core/CoreOperations.cpp"
        "${PATH_TO_SOURCE}/Operations/Core/CoreOperations.h"
        "${PATH_TO_SOURCE}/Operations/Services/ServicesOperations.cpp"
        "${PATH_TO_SOURCE}/Operations/Services/ServicesOperations.h"
        "${PATH_TO_SOURCE}/Platform/Generic/MemoryManager.cpp"
        "${PATH_TO_SOURCE}/Platform/Generic/MemoryManager.h"
        "${PATH_TO_SOURCE}/Platform/PlatformUtils.h"
        "${PATH_TO_SOURCE}/Platform/PlayFabPal.h"
        "${PATH_TO_SOURCE}/TestAppPch.cpp"
        "${PATH_TO_SOURCE}/TestAppPch.h"
        "${PATH_TO_SOURCE}/TestFramework/CoreTestClass.cpp"
        "${PATH_TO_SOURCE}/TestFramework/CoreTestClass.h"
        "${PATH_TO_SOURCE}/TestFramework/ServicesTestClass.cpp"
        "${PATH_TO_SOURCE}/TestFramework/ServicesTestClass.h"
        "${PATH_TO_SOURCE}/TestFramework/TestClass.cpp"
        "${PATH_TO_SOURCE}/TestFramework/TestClass.h"
        "${PATH_TO_SOURCE}/TestFramework/TestContext.cpp"
        "${PATH_TO_SOURCE}/TestFramework/TestContext.h"
        "${PATH_TO_SOURCE}/TestFramework/TestException.h"
        "${PATH_TO_SOURCE}/TestFramework/TestReport.cpp"
        "${PATH_TO_SOURCE}/TestFramework/TestReport.h"
        "${PATH_TO_SOURCE}/TestFramework/TestRunner.cpp"
        "${PATH_TO_SOURCE}/TestFramework/TestRunner.h"
        "${PATH_TO_SOURCE}/TestFramework/TestTypes.h"
        "${PATH_TO_SOURCE}/Tests/Core/AuthenticationTests.cpp"
        "${PATH_TO_SOURCE}/Tests/Core/AuthenticationTests.h"
        "${PATH_TO_SOURCE}/Tests/Core/EventPipelineTests.cpp"
        "${PATH_TO_SOURCE}/Tests/Core/EventPipelineTests.h"
        "${PATH_TO_SOURCE}/Tests/Services/AccountManagementTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/CatalogTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/CharacterTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/CloudScriptTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/DataTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/ExperimentationTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/FriendsTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/GroupsTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/InventoryTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/LocalizationTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/MultiplayerServerTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/PlatformSpecificTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/PlayerDataManagementTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/ProfilesTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/PushNotificationsTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/SegmentsTestsImpl.cpp"
        "${PATH_TO_SOURCE}/Tests/Services/TitleDataManagementTestsImpl.cpp"
        PARENT_SCOPE
        )

endfunction()