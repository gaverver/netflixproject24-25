package com.example.netflix;

import java.util.concurrent.Executors;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class RetrofitClient {
    private final Retrofit retrofit;

    // Private constructor
    private RetrofitClient() {
        retrofit = new Retrofit.Builder()
                .baseUrl(MyApplication.getAppContext().getString(R.string.BaseUrl))
                .callbackExecutor(Executors.newSingleThreadExecutor())
                .addConverterFactory(GsonConverterFactory.create())
                .build();

    }

    // Static inner class for lazy initialization
    private static class Holder {
        private static final RetrofitClient INSTANCE = new RetrofitClient();
    }

    // Public method to get the singleton instance
    public static RetrofitClient getInstance() {
        return Holder.INSTANCE;
    }

    // Getter for the API interface
    public Retrofit getRetrofit() {
        return retrofit;
    }
}

