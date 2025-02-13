package com.example.netflix.activities;

import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.LiveData;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.adapters.CategoryAdapter;
import com.example.netflix.entities.Category;
import com.example.netflix.viewmodels.CategoryViewModel;
import com.example.netflix.viewmodels.MovieViewModel;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class HomePage extends AppCompatActivity {
    private RecyclerView categoriesRecyclerView;
    private CategoryAdapter categoryAdapter;
    private final MovieViewModel movieViewModel = new MovieViewModel();
    private final WebResponse res = new WebResponse();
    private LiveData<Map<String, List<String>>> allMovies;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        categoriesRecyclerView = findViewById(R.id.categoriesRecyclerView);
        categoriesRecyclerView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));

        fetchMovies();
    }

    private void fetchMovies() {
        // reload the ROOM and fetch all categories from the server
        allMovies = movieViewModel.getMovies(res);
        res.getResponseCode().observe(this, code -> {
            // if the reload failed
            if (code != 200) {
                // make a toast to let the user know, but still show him what he gets from his ROOM
                Toast.makeText(getApplicationContext(), "Failed to fetch movies from the server", Toast.LENGTH_LONG).show();
            }

            // wait for the movies to fetch
            allMovies.observe(this, movies -> {
                // gets all the movies in categories form
                List<Category> finalMovies = getMoviesFromResponse(movies);
                // show only the categories whose movies' list isn't empty
                categoryAdapter = new CategoryAdapter(this, finalMovies.stream().filter(new Predicate<Category>() {
                    @Override
                    public boolean test(Category category) {
                        return category.getMovies().length != 0;
                    }
                }).collect(Collectors.toList()));
                categoriesRecyclerView.setAdapter(categoryAdapter);
            });
        });
    }

    private List<Category> getMoviesFromResponse(Map<String, List<String>> movies) {
        List<Category> results = new ArrayList<>();
        for (Map.Entry<String, List<String>> entry : movies.entrySet()) {
            results.add(new Category(entry.getKey(), true, entry.getValue().toArray(new String[0])));
        }
        return results;
    }
}
