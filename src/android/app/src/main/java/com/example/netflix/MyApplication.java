package com.example.netflix;

import android.app.Application;
import android.content.Context;

import com.example.netflix.data.LocalDatabase;

public class MyApplication extends Application {
    private static MyApplication instance;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        LocalDatabase.initInstance(this);
    }

    public static MyApplication getInstance() {
        return instance;
    }

    public static Context getAppContext() {
        return getInstance().getApplicationContext();
    }
}
