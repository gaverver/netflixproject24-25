package com.example.netflix.activities;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.ContextCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.LiveData;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.adapters.MoviesAdapter;
import com.example.netflix.entities.Movie;
import com.example.netflix.fragments.NavigationDrawerFragment;
import com.example.netflix.viewmodels.MovieViewModel;

import java.util.ArrayList;
import java.util.List;

public class SearchResultsPage extends AppCompatActivity {
    private String query;
    private final MovieViewModel movieServices = new MovieViewModel();
    private final WebResponse res = new WebResponse();
    private LiveData<List<Movie>> results;
    private TextView noMoviesFound;
    private RecyclerView recyclerView;
    private MoviesAdapter adapter;
    private View rootView;
    private Toolbar toolbar;
    private DrawerLayout drawerLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //Context context = getApplicationContext();
        //context.deleteDatabase("local_database");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search_results_page);
        Intent intent = getIntent();
        query = intent.getStringExtra("SEARCH_QUERY");
        noMoviesFound = findViewById(R.id.noMoviesFound);
        recyclerView = findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false));
        // Default state: hide "no movies" text
        noMoviesFound.setVisibility(View.GONE);
        recyclerView.setVisibility(View.GONE);

        // Set up RecyclerView with a GridLayoutManager (3 columns)
        recyclerView.setLayoutManager(new GridLayoutManager(this, 3));

        rootView = findViewById(android.R.id.content).getRootView();

        //add toolbar
        toolbar = findViewById(R.id.menuToolbar3);

        drawerLayout = findViewById(R.id.drawer_layout);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.open_drawer, R.string.close_drawer);
        drawerLayout.addDrawerListener(toggle);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setHomeButtonEnabled(true);
        }

        toggle.syncState();

        //add fragment for navigation drawer
        if (savedInstanceState == null) {
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            NavigationDrawerFragment navigationDrawerFragment = new NavigationDrawerFragment();
            transaction.replace(R.id.navigation_fragment3, navigationDrawerFragment); // Add to container
            transaction.commit();
        }

        searchForMovies();

    }

    private List<String> convertToIds(List<Movie> movies) {
        List<String> Ids = new ArrayList<>();
        for (Movie movie : movies) {
            Ids.add(movie.getId());
        }
        return Ids;
    }

    private void searchForMovies() {
        results = movieServices.searchMovies(query, res);
        res.getResponseCode().observe(this, code -> {
            if (code != 200) {
                Toast.makeText(getApplicationContext(), "Error searching for movies", Toast.LENGTH_LONG).show();
                return;
            }
            results.observe(this, movies -> {
                // convert to List of movies' ids
                List<String> movieIds = convertToIds(movies);

                if (movies.isEmpty()) {
                    noMoviesFound.setVisibility(View.VISIBLE);
                    recyclerView.setVisibility(View.GONE);
                } else {
                    noMoviesFound.setVisibility(View.GONE);
                    recyclerView.setVisibility(View.VISIBLE);
                    adapter = new MoviesAdapter(this, movieIds);
                    recyclerView.setAdapter(adapter);
                }
            });
        });
    }
}
