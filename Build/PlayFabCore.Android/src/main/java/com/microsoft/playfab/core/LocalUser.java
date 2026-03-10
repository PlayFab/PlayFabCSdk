package com.microsoft.playfab.core;

import com.google.android.gms.games.GamesSignInClient;
import android.util.Log;
import java.util.concurrent.Executors;

public class LocalUser {

    private static final String TAG = "LocalUser";

    @SuppressWarnings("unused")
    public static void GetServerAuthTokenAsync(GamesSignInClient signInClient, String OAuth2WebClientId, long context)
    {
        Log.i(TAG, "GetServerAuthTokenAsync starting");
        signInClient.requestServerSideAccess(OAuth2WebClientId, true).addOnCompleteListener(Executors.newCachedThreadPool(), task ->
        {
            Log.i(TAG, "GetServerAuthTokenAsync complete");
            if (task.isSuccessful())
            {
                Log.i(TAG, "GetServerAuthTokenAsync succeeded");
                OnGetServerAuthTokenSuccess(context, task.getResult());
            }
            else
            {
                OnGetServerAuthTokenFailed(context, task.getException().getMessage());
            }
        });
    }

    private static native void OnGetServerAuthTokenSuccess(long context, String token);
    private static native void OnGetServerAuthTokenFailed(long context, String errorMessage);
}
