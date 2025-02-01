package com.example.netflix;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Path;

public interface UserWebServiceAPI {
    @POST("/api/users")
    Call<Void> createUser(@Body User user);

    @GET("/api/users/{id}")
    Call<User> getUser(@Path("id") String id);
}
