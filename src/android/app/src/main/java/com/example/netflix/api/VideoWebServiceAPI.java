package com.example.netflix.api;

import okhttp3.MultipartBody;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Header;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.Part;

public interface VideoWebServiceAPI {
    @Multipart
    @POST("videos")
    Call<ResponseBody> uploadVideo(@Header("Authorization") String token, @Part MultipartBody.Part video);
}
