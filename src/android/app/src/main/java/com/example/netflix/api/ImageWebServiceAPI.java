package com.example.netflix.api;

import com.example.netflix.ImageResponse;
import com.example.netflix.entities.Image;

import okhttp3.RequestBody;
import okhttp3.Response;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.POST;
import retrofit2.http.DELETE;
import retrofit2.http.Path;

public interface ImageWebServiceAPI {
    // upload image as raw binary data with dynamic content type based on file extension
    @POST("/images")
    // default content type for the body
    @Headers("Content-Type: application/octet-stream")
    Call<Void> uploadImage(
            // the data of the image as RequestBody object
            @Body RequestBody image
            // add content type dynamically (jpg, png, ...)
//            @Header("Content-Type") String contentType
    );

    // getting image details
    @GET("/images/{id}")
    Call<ImageResponse> getImage(@Path("id") String id);

    // deleting image by id
    @DELETE("/images/{id}")
    Call<Void> deleteImage(@Path("id") String id);
}