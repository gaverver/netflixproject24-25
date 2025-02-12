package com.example.netflix;

import com.example.netflix.entities.Movie;

import java.util.List;

// a response from the SearchResults
public class SearchResultsResponse {
    private List<Movie> movies;

    public List<Movie> getMovies() {
        return movies;
    }
}

