package com.example.netflix.api;

import com.example.netflix.entities.Category;

import java.util.List;

import okhttp3.RequestBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.PATCH;
import retrofit2.http.POST;
import retrofit2.http.DELETE;
import retrofit2.http.Path;

public interface CategoryWebServiceAPI {
    // gets all the categories
    @GET("/api/categories")
    Call<List<Category>> getAllCategories();

    // create a new category (add the token in the headers because only admins could do it)
    @POST("/api/categories")
    Call<Void> insertCategory(@Body Category category, @Header("Authorization") String token);

    // get details on  a specific category
    @GET("/api/categories/{id}")
    Call<Category> getCategory(@Path("id") String id);

    // update an existing category (add the token in the headers because only admins could do it)
    @PATCH("/api/categories/{id}")
    Call<Void> updateCategory(@Path("id") String id, @Body Category new_category, @Header("Authorization") String token);

    // delete an existing category (add the token in the headers because only admins could do it)
    @DELETE("/api/categories/{id}")
    Call<Void> deleteCategory(@Path("id") String id, @Header("Authorization") String token);
}