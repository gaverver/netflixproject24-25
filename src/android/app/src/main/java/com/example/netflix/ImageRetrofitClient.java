package com.example.netflix;

import com.example.netflix.entities.Image;
import com.example.netflix.ImageDeserializer;

import com.google.gson.GsonBuilder;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class ImageRetrofitClient {
    private final Retrofit retrofit;

    // Private constructor
    private ImageRetrofitClient() {
        retrofit = new Retrofit.Builder()
                .baseUrl(MyApplication.getAppContext().getString(R.string.BaseUrl))
                .addConverterFactory(GsonConverterFactory.create(new GsonBuilder()
                        .registerTypeAdapter(Image.class, new ImageDeserializer()) // Custom deserializer for Image
                        .create()))
                .build();
    }

    // Static inner class for lazy initialization
    private static class Holder {
        private static final ImageRetrofitClient INSTANCE = new ImageRetrofitClient();
    }

    // Public method to get the singleton instance
    public static ImageRetrofitClient getInstance() {
        return Holder.INSTANCE;
    }

    // Getter for the API interface
    public Retrofit getRetrofit() {
        return retrofit;
    }
}
