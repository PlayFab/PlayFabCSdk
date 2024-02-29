package com.microsoft.playfab.sdk;

import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.util.Log;
import androidx.test.core.app.ActivityScenario;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

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
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                activity.onActivity(a -> {
                        testsPassed = a.StartTests();
                    }
                );
            }
        });

        try  {
            while (!testsFinished) {
                SystemClock.sleep(1000);
                activity.onActivity(a -> {
                    testsFinished = a.isDestroyed();
                });
            }
        }
        catch (Exception e) {}

        Assert.assertTrue(testsPassed);
    }
}