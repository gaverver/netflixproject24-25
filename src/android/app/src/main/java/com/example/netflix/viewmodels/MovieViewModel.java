package com.example.netflix.viewmodels;

import androidx.lifecycle.LiveData;

import com.example.netflix.entities.Movie;
import com.example.netflix.repositories.MovieRepository;
import com.example.netflix.WebResponse;

import java.util.List;
import java.util.Map;

public class MovieViewModel {

    private MovieRepository movieRepository;

    public MovieViewModel() {
        this.movieRepository = new MovieRepository();
    }

    public LiveData<Movie> getMovie(String id, WebResponse webRes) {
        return movieRepository.getMovie(id, webRes);
    }

    public LiveData<Movie> getReloadedMovie(String id, WebResponse webRes) {
        return movieRepository.getReloadedMovie(id, webRes);
    }

    public void createMovie(Movie movie, WebResponse res) {
        movieRepository.createMovie(movie, res);
    }

    public void putMovie(Movie movie, WebResponse res) {
        movieRepository.putMovie(movie, res);
    }

    public void deleteMovie(String movieId, WebResponse res) {
        movieRepository.deleteMovie(movieId, res);
    }

    public LiveData<List<String>> getRecommendationIds(String id, WebResponse res) {
        return movieRepository.getRecommendationIds(id, res);
    }

    public void addWatchedMovie(String id, WebResponse res) {
        movieRepository.addWatchedMovie(id, res);
    }

    public LiveData<List<Movie>> searchMovies(String query, WebResponse res) {
        return movieRepository.searchMovies(query, res);
    }

    public LiveData<Map<String, List<String>>> getMovies(WebResponse res) {
        return movieRepository.getMovies(res);
    }


}