#include "TestAppPch.h"
#include "CharacterTests.h"
#include "CharacterOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> CharacterTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> CharacterTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestClientGetAllUsersCharacters(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestClientGetCharacterData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestClientGetCharacterReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestClientGrantCharacterToUser(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestClientUpdateCharacterData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerDeleteCharacterFromUser(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerGetAllUsersCharacters(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerGetCharacterData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerGetCharacterInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerGetCharacterReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerGrantCharacterToUser(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerUpdateCharacterData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerUpdateCharacterInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void CharacterTests::TestServerUpdateCharacterReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}
