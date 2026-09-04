package com.microsoft.playfab.sdk;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import java.io.*;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import com.google.android.gms.games.AuthenticationResult;
import com.google.android.gms.games.PlayGames;
import com.google.android.gms.games.PlayGamesSdk;
import com.google.android.gms.games.GamesSignInClient;
import com.google.android.gms.games.PlayersClient;
import com.google.android.gms.tasks.Task;
import com.google.android.gms.tasks.Tasks;

public class AndroidTestClient extends Activity {
    private static final String TAG = "AndroidTestClient";
    private static Task<String> getServerAuthCodeTask;
    private static String serverAuthToken;

@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        PlayGamesSdk.initialize(this);

        // This will needs to be done by the title in the onCreate(..) callback of their application class
        // It will pop a Google Play UI prompting the user to sign in.
        GamesSignInClient gamesSignInClient = PlayGames.getGamesSignInClient(this);

        String playerId = "NonDurableAndroidPlayerId";
        try {
            AuthenticationResult authResult = Tasks.await(
                gamesSignInClient.isAuthenticated(),
                3,
                TimeUnit.SECONDS);
            if (authResult.isAuthenticated()) {
                Log.i(TAG, "Authentication Success");

                playerId = Tasks.await(
                    PlayGames.getPlayersClient(this).getCurrentPlayerId(),
                    3,
                    TimeUnit.SECONDS);
                Log.i(TAG, "PlayerId: " + playerId);
            }
        }
        catch(Exception e) {
            Log.e(TAG, "Exception in Google Play Game authentication: " + e.getMessage());
            Log.e(TAG, "Continuing with non-durable playerId");
        }

        String OAUTH_2_WEB_CLIENT_ID = "912588564112-etm77dodeqkqtblm5seq468icat2nng2.apps.googleusercontent.com";
        getServerAuthCodeTask = gamesSignInClient.requestServerSideAccess(OAUTH_2_WEB_CLIENT_ID, false);

        setContentView(R.layout.activity_main);

        if (savedInstanceState == null)
        {
            System.loadLibrary("PlayFabSDKTestApp.Android");
            Log.i(TAG, "Loaded PlayFabSDKTestApp.Android Library");

            InitializeApp(
                getApplicationContext(),
                playerId,
                gamesSignInClient
            );
        }
        else
        {
            // Our JNI code holds a reference to this instance. If we were
            // destroyed and recreated, we should update those references.
            // Really, we should update those references when we are
            // deinitialized here...but this is a test app :)
            UpdateInstances(getApplicationContext());
        }

        Button button = this.findViewById(R.id.button_startTool);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                RunTests();
            }
        });
    }

    // Set to true on the UI thread once the background test thread has
    // finished running RunTests() and posted the result back. The JUnit
    // harness in androidTest/ polls this to know when to assert the result.
    public volatile boolean testsCompleted = false;
    public volatile boolean testsPassed = false;
    private Thread testThread;

    public void StartTests() {
        if (testThread != null) {
            return;
        }
        // Run the native test loop on a background thread so the activity's
        // main thread stays responsive. Running RunTests() directly on the UI
        // thread blocks input dispatch for the full duration of the test suite
        // and trips Android's keyDispatchingTimedOut ANR watchdog (~5s),
        // causing the instrumentation harness to kill the test before it can
        // complete. When the native call returns, post back to the UI thread
        // to publish the result and finish() the activity.
        testThread = new Thread(new Runnable() {
            @Override
            public void run() {
                boolean ret = false;
                try {
                    ret = RunTests();
                }
                catch (Throwable t) {
                    Log.e(TAG, "RunTests threw: " + t.getMessage());
                }
                final boolean finalRet = ret;
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        testsPassed = finalRet;
                        testsCompleted = true;
                        finish();
                    }
                });
            }
        }, "PlayFabTestRunner");
        testThread.start();
    }

    public byte[] GetBufferFromFile(String filename) throws IOException {
        AssetManager assetManager = getApplicationContext().getAssets();
        try (InputStream is = assetManager.open(filename);
             ByteArrayOutputStream buffer = new ByteArrayOutputStream()) {
            // Do not trust InputStream.available(): it is documented as an
            // approximation and for compressed Android assets it has been seen
            // to under-report, causing the previous fixed-size buffer to leak
            // trailing uninitialised bytes (e.g. a stray U+FFFD past the
            // closing '}' of testTitleData.json) when copied into the output.
            // Read in fixed-size chunks until EOF instead.
            byte[] chunk = new byte[4096];
            int nRead;
            while ((nRead = is.read(chunk, 0, chunk.length)) != -1) {
                buffer.write(chunk, 0, nRead);
            }
            return buffer.toByteArray();
        }
        catch (IOException e) {
            return null;
        }
    }

    public String GetDeviceToken() {
        return Secure.getString(getApplicationContext().getContentResolver(),Secure.ANDROID_ID);
    }

    public String GetServerAuthToken() throws ExecutionException, InterruptedException {
        if (getServerAuthCodeTask.isComplete()) {
            if (getServerAuthCodeTask.isSuccessful()) {
                return getServerAuthCodeTask.getResult();
            } else {
                return "";
            }
        } else {
            try {
                return Tasks.await(getServerAuthCodeTask, 10, TimeUnit.SECONDS);
            }
            catch (TimeoutException e) {
                Log.e(TAG, "Timed out waiting for Google Play Games server auth token");
                return "";
            }
        }
    }

    private native void InitializeApp(Context context, String playerId, GamesSignInClient signInClient);
    private native void UpdateInstances(Context context);

    private native boolean RunTests();
}