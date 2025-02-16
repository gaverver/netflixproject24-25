package com.example.netflix.api;

import android.content.Context;
import android.content.SharedPreferences;

import androidx.lifecycle.MutableLiveData;

import com.example.netflix.GetMoviesResponse;
import com.example.netflix.MyApplication;
import com.example.netflix.RetrofitClient;
import com.example.netflix.SearchResultsResponse;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.Movie;
import com.example.netflix.repositories.MovieDao;
import com.example.netflix.viewmodels.ImageViewModel;

import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import android.os.AsyncTask;
import android.util.Log;

public class MovieAPI {
    private MovieDao dao;
    private Retrofit retrofit;
    private MovieWebServiceAPI movieWebServiceAPI;
    private ImageViewModel imageViewModel;
    private String userId;
    private String token;

    private void updateTokens() {
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        this.token = sharedPreferences.getString("token", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VySWQiOiI2Nzk2YmY3OWJkMjA1ZmI0YzQ5ZjdiNjEiLCJwcml2aWxlZ2VMZXZlbCI6MSwiaWF0IjoxNzM5NDY5Mjc3fQ._bQpbrAZUUJlXAZD77mJ1NWECk10KqSkpiTKt33mLoY");
        this.userId = sharedPreferences.getString("userId", "679c0a6a0a6cabd498442b92");
    }

    public MovieAPI(MovieDao movieDao) {
        this.dao = movieDao;

        retrofit = RetrofitClient.getInstance().getRetrofit();

        movieWebServiceAPI = retrofit.create(MovieWebServiceAPI.class);

        imageViewModel = new ImageViewModel();

        updateTokens();

    }


    public void getReloadedMovie(String id, WebResponse res, MutableLiveData<Movie> movieMutableLiveData) {
        Call<Movie> call = movieWebServiceAPI.getMovie(id);
        call.enqueue(new Callback<Movie>() {
            @Override
            public void onResponse(Call<Movie> call, Response<Movie> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        Movie movie = dao.get(response.body().getId());
                        imageViewModel.get(response.body().getId(), new WebResponse());
                        if (movie == null) {
                            dao.insert(response.body());
                        } else {
                            dao.update(response.body());
                        }
                        if (movieMutableLiveData != null) {
                            movieMutableLiveData.postValue(response.body());
                        }
                        // set the response status to the returned response status - the operation was successful
                        res.setResponseCode(response.code());
                        res.setResponseMsg("ok");
                    }).start();
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<Movie> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void createMovie(Movie movie, WebResponse res) {
        updateTokens();
        Call<Void> call = movieWebServiceAPI.createMovie(token, movie);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> call, Response<Void> response) {
                if (response.isSuccessful()) {
                    // get the created userID from the location header
                    String locationHeader = response.headers().get("Location");
                    if (locationHeader != null) {
                        // extract the id from the location header(it is /users/:id)
                        Pattern pattern = Pattern.compile("[^/]+$");
                        Matcher matcher = pattern.matcher(locationHeader);
                        if (matcher.find()) {
                            movie.setId(matcher.group());
                        }
                    }
                    new Thread(() -> {
                        // insert the created user to the room
                        dao.insert(movie);
                    }).start();
                    // set the response status to the returned response status - the operation was successful
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Movie Created");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<Void> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Creation of movie failed" + t.getMessage());
            }
        });


    }

    public void putMovie(Movie movie, WebResponse res) {
        updateTokens();
        Call<Void> call = movieWebServiceAPI.putMovie(token, movie.getId(), movie);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> call, Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // Update the movie in the local database
                        dao.update(movie);
                    }).start();
                    // Set the response status to the returned response status
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Movie Updated Successfully");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<Void> call, Throwable t) {
                // Return response code that indicates there was an error in the server
                res.setResponseCode(500);
                res.setResponseMsg("Update of movie failed: " + t.getMessage());
            }
        });
    }

    public void deleteMovie(String movieId, WebResponse res) {
        updateTokens();
        Call<Void> call = movieWebServiceAPI.deleteMovie(token, movieId);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> call, Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // Delete the movie from the local database
                        dao.deleteById(movieId);
                    }).start();
                    // Set the response status to the returned response status
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Movie Deleted Successfully");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<Void> call, Throwable t) {
                // Return response code that indicates there was an error in the server
                res.setResponseCode(500);
                res.setResponseMsg("Deletion of movie failed: " + t.getMessage());
            }
        });
    }

    public void getRecommendationIds(String id, WebResponse res, MutableLiveData<List<String>> movieIds) {
        updateTokens();
        Call<List<String>> call = movieWebServiceAPI.getRecommendation(token, userId, id);
        call.enqueue(new Callback<List<String>>() {
            @Override
            public void onResponse(Call<List<String>> call, Response<List<String>> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        movieIds.postValue(response.body());
                    }).start();
                    // set the response status to the returned response status - the operation was successful
                    res.setResponseCode(response.code());
                    res.setResponseMsg("ok");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<List<String>> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void addWatchedMovie(String id, WebResponse res) {
        updateTokens();
        Call<List<String>> call = movieWebServiceAPI.addWatchedMovie(token, userId, id);
        call.enqueue(new Callback<List<String>>() {
            @Override
            public void onResponse(Call<List<String>> call, Response<List<String>> response) {
                if (response.isSuccessful() && response.body() != null) {
                    // set the response status to the returned response status - the operation was successful
                    res.setResponseCode(response.code());
                    res.setResponseMsg("ok");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<List<String>> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void searchMovies(String query, WebResponse res, MutableLiveData<List<Movie>> moviesMutableLiveData) {
        Call<SearchResultsResponse> call = movieWebServiceAPI.searchMovies(query);
        call.enqueue(new Callback<SearchResultsResponse>() {
            @Override
            public void onResponse(Call<SearchResultsResponse> call, Response<SearchResultsResponse> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        moviesMutableLiveData.postValue(response.body().getMovies());
                    }).start();
                    // set the response status to the returned response status - the operation was successful
                    res.setResponseCode(response.code());
                    res.setResponseMsg("ok");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<SearchResultsResponse> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void getMovies(WebResponse res,  MutableLiveData<Map<String, List<String>>> moviesMutableLiveData) {
        updateTokens();
        Log.d("hello", "token is: " + token);
        Log.d("hello", "userId is: " + userId);
        Call<GetMoviesResponse> call = movieWebServiceAPI.getMovies(token, userId);
        call.enqueue(new Callback<GetMoviesResponse>() {
            @Override
            public void onResponse(Call<GetMoviesResponse> call, Response<GetMoviesResponse> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        dao.clear();
                        GetMoviesResponse getMoviesResponse = response.body();
                        Map<String, List<String>> responseBody = getMoviesResponse.getMovies();
                        for (Map.Entry<String, List<String>> entry : responseBody.entrySet()) {
                            List<String> values = entry.getValue();
                            for (String value : values) {
                                getReloadedMovie(value,res,null);
                            }
                        }
                        // set the response status to the returned response status - the operation was successful
                        res.setResponseCode(response.code());
                        res.setResponseMsg("ok");
                        moviesMutableLiveData.postValue(responseBody);
                    }).start();
                } else {
                    res.setResponseMsg(response.message());
                    res.setResponseCode(response.code());
                    Log.d("hello", String.valueOf(response.code()));
                    //Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<GetMoviesResponse> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

}