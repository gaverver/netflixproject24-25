package com.example.netflix.api;

import com.example.netflix.GetMoviesResponse;
import com.example.netflix.SearchResultsResponse;
import com.example.netflix.entities.Movie;

import java.util.List;
import java.util.Map;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.DELETE;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Path;

public interface MovieWebServiceAPI {
    @GET("/api/movies/{id}")
    Call<Movie> getMovie(@Path("id") String id);

    @POST("/api/movies")
    Call<Void> createMovie(@Header("Authorization") String token, @Body Movie movie);

    @PUT("/api/movies/{id}")
    Call<Void> putMovie(@Header("Authorization") String token, @Path("id") String id, @Body Movie movie);

    @DELETE("/api/movies/{id}")
    Call<Void> deleteMovie(@Header("Authorization") String token, @Path("id") String id);

    @GET("/api/movies/{id}/recommend")
    Call<List<String>> getRecommendation(@Header("Authorization") String token, @Header("userId") String userId, @Path("id") String MovieId);

    @POST("/api/movies/{id}/recommend")
    Call<List<String>> addWatchedMovie(@Header("Authorization") String token, @Header("userId") String userId, @Path("id") String MovieId);

    @GET("/api/movies/search/{query}")
    Call<SearchResultsResponse> searchMovies(@Path("query") String query);

    @GET("/api/movies")
    Call<GetMoviesResponse> getMovies(@Header("Authorization") String token, @Header("userId") String userId);
}
