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

public class AndroidTestClient extends Activity {
    private static final String TAG = "AndroidTestClient";

@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null)
        {
            System.loadLibrary("PlayFabSDKTestApp.Android");
            Log.i(TAG, "Loaded PlayFabSDKTestApp.Android Library");

            InitializeApp(getApplicationContext());
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

    public boolean StartTests() {
        boolean ret = RunTests();
        finish();
        return ret;
    }

    public byte[] GetBufferFromFile(String filename) throws IOException {
        try {
            AssetManager manager = getApplicationContext().getAssets();
            InputStream is = manager.open(filename);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            return buffer;
        }
        catch (IOException e) {
            return null;
        }
    }

    public String GetDeviceToken() {
        return Secure.getString(getApplicationContext().getContentResolver(),Secure.ANDROID_ID);
    }

    private native void InitializeApp(Context context);
    private native void UpdateInstances(Context context);

    private native boolean RunTests();
}