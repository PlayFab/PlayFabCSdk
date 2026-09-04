package com.microsoft.playfab.sdk;

import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.util.Log;
import androidx.test.core.app.ActivityScenario;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.google.android.gms.games.GamesSignInClient;
import com.google.android.gms.games.PlayGames;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.concurrent.TimeUnit;

@RunWith(AndroidJUnit4.class)
public class AndroidAutomatedTest {
    ActivityScenario<AndroidTestClient> activity;
    boolean testsPassed;
    boolean testsFinished = false;

    @Before
    public void Initialize() {
        activity = ActivityScenario.launch(AndroidTestClient.class);
    }

    @Test
    public void RunAllTests() {
        // Kick off the test run. StartTests() returns immediately; it spawns
        // a background thread that drives the native test loop and posts the
        // result + activity finish() back to the UI thread when complete. This
        // keeps the activity's main thread responsive so Android does not ANR
        // the instrumentation harness mid-run (keyDispatchingTimedOut).
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
               activity.onActivity(a -> {
                        a.StartTests();
                    }
                );
            }
        });

        long deadline = SystemClock.elapsedRealtime() + TimeUnit.MINUTES.toMillis(60);
        try {
            while (!testsFinished && SystemClock.elapsedRealtime() < deadline) {
                SystemClock.sleep(1000);
                activity.onActivity(a -> {
                    if (a.testsCompleted) {
                        testsPassed = a.testsPassed;
                        testsFinished = true;
                    }
                    else if (a.isDestroyed()) {
                        // Activity gone without publishing a result -
                        // treat as failure so JUnit reports it rather
                        // than spinning forever.
                        testsFinished = true;
                    }
                });
            }
        }
        catch (Exception e) {
            Log.e("AndroidAutomatedTest", "Failed while waiting for test completion", e);
        }

        Assert.assertTrue("Android native tests did not complete before timeout", testsFinished);
        Assert.assertTrue(testsPassed);
    }
}