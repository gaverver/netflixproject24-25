package com.example.netflix.repositories;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.MovieAPI;
import com.example.netflix.data.LocalDatabase;
import com.example.netflix.entities.Movie;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MovieRepository {
    private MovieDao dao;
    private MovieAPI api;

    private final ExecutorService executorService;

    public MovieRepository() {
        LocalDataBase db = LocalDataBase.getInstance();
        this.dao = db.movieDao();
        this.api = new MovieAPI(dao);
        this.executorService = Executors.newSingleThreadExecutor();
    }

    public LiveData<Movie> getMovie(String id, WebResponse webRes) {
        MutableLiveData<Movie> liveData = new MutableLiveData<>();
        executorService.execute(() -> {
            Movie movie = dao.get(id);
            if (movie == null) {
                api.getReloadedMovie(id, webRes, liveData);
            } else {
                liveData.postValue(movie);
                webRes.setResponseCode(200);
                webRes.setResponseMsg("Movie fetched from local database");
            }
        });
//        api.getMovie(id, webRes, res);
        return liveData;
    }

    public LiveData<Movie> getReloadedMovie(String id, WebResponse webRes) {
        MutableLiveData<Movie> res = new MutableLiveData<>();
        api.getReloadedMovie(id, webRes, res);
        return res;
    }

    public void createMovie(Movie movie, WebResponse res) {
        api.createMovie(movie, res);
    }

    public void putMovie(Movie movie, WebResponse res) {
        api.putMovie(movie, res);
    }

    public void deleteMovie(String movieId, WebResponse res) {
        api.deleteMovie(movieId, res);
    }

    public LiveData<List<String>> getRecommendationIds(String id, WebResponse res) {
        MutableLiveData<List<String>> movieIds = new MutableLiveData<>();
        api.getRecommendationIds(id, res, movieIds);
        return movieIds;
    }

    public void addWatchedMovie(String id, WebResponse res) {
        api.addWatchedMovie(id, res);
    }

    public LiveData<List<Movie>> searchMovies(String query, WebResponse res) {
        MutableLiveData<List<Movie>> moviesMutableLiveData = new MutableLiveData<>();
        api.searchMovies(query, res, moviesMutableLiveData);
        return moviesMutableLiveData;
    }

    public LiveData<Map<String, List<String>>> getMovies(WebResponse res) {
        MutableLiveData<Map<String, List<String>>> moviesMutableLiveData = new MutableLiveData<>();
        api.getMovies(res, moviesMutableLiveData);
        return moviesMutableLiveData;
    }

}
