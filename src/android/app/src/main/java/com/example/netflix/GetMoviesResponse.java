package com.example.netflix;

import com.google.gson.annotations.SerializedName;
import java.util.List;
import java.util.Map;

public class GetMoviesResponse {
    @SerializedName("movies")
    private Map<String, List<String>> movies;

    public Map<String, List<String>> getMovies() {
        return movies;
    }
}
