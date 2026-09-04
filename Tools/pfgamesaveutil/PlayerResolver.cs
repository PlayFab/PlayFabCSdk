using PlayFab;
using PlayFab.AuthenticationModels;
using PlayFab.ProfilesModels;

namespace PfGameSaveUtil;

public static class PlayerResolver
{
    public static async Task<string?> ResolveTitlePlayerIdAsync(string titleId, string secretKey, string playfabId)
    {
        Console.WriteLine($"Resolving PlayFabId {playfabId} to title_player_account...");
        
        PlayFabSettings.staticSettings.TitleId = titleId;
        PlayFabSettings.staticSettings.DeveloperSecretKey = secretKey;

        var authResult = await PlayFabAuthenticationAPI.GetEntityTokenAsync(new GetEntityTokenRequest());
        if (authResult.Error != null)
        {
            Console.Error.WriteLine($"Authentication failed: {authResult.Error.ErrorMessage}");
            return null;
        }

        var request = new GetTitlePlayersFromMasterPlayerAccountIdsRequest
        {
            MasterPlayerAccountIds = new List<string> { playfabId }
        };

        var result = await PlayFabProfilesAPI.GetTitlePlayersFromMasterPlayerAccountIdsAsync(request);
        if (result.Error != null)
        {
            Console.Error.WriteLine($"Failed to resolve PlayFabId: {result.Error.ErrorMessage}");
            return null;
        }

        if (result.Result.TitlePlayerAccounts == null || 
            !result.Result.TitlePlayerAccounts.TryGetValue(playfabId, out var entityKey) ||
            entityKey == null)
        {
            Console.Error.WriteLine($"No title_player_account found for PlayFabId {playfabId} in title {titleId}");
            return null;
        }

        Console.WriteLine($"Resolved to entity ID: {entityKey.Id}");
        return entityKey.Id;
    }
}
