package com.example.netflix.activities;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.Toolbar;

import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.LiveData;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.adapters.CategoryAdapter;
import com.example.netflix.entities.Category;
import com.example.netflix.entities.Movie;
import com.example.netflix.fragments.NavigationDrawerFragment;
import com.example.netflix.fragments.VideoPlayerFragment;
import com.example.netflix.viewmodels.CategoryViewModel;
import com.example.netflix.viewmodels.MovieViewModel;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class HomePage extends AppCompatActivity {
    private RecyclerView categoriesRecyclerView;
    private CategoryAdapter categoryAdapter;
    private final MovieViewModel movieViewModel = new MovieViewModel();
    private final WebResponse res = new WebResponse();
    private final WebResponse res2 = new WebResponse();
    private LiveData<Map<String, List<String>>> allMovies;
    private String randomMovieId;
    private Button playButton;
    private Button moreInfoButton;
    private TextView movieTitle;
    private TextView ageLimit;
    private View rootView;
    private Toolbar toolbar;
    private DrawerLayout drawerLayout;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        categoriesRecyclerView = findViewById(R.id.categoriesRecyclerView);
        playButton = findViewById(R.id.playButton);
        moreInfoButton = findViewById(R.id.moreInfoButton);
        movieTitle = findViewById(R.id.movieTitle);
        ageLimit = findViewById(R.id.ageLimit);
        categoriesRecyclerView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));
        rootView = findViewById(android.R.id.content).getRootView();

        //add toolbar
        toolbar = findViewById(R.id.menuToolbar2);

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
            transaction.replace(R.id.navigation_fragment2, navigationDrawerFragment); // Add to container
            transaction.commit();
        }

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
                // stream a random movie to the screen
                randomMovieId = getRandomMovie(movies);
                LiveData<Movie> randomMovie = movieViewModel.getMovie(randomMovieId, res2);
                res2.getResponseCode().observe(this, code2 -> {
                    // if failed
                    if (code2 != 200) {
                        // make a toast to let the user know
                        Toast.makeText(getApplicationContext(), "Failed to stream random movie", Toast.LENGTH_LONG).show();
                    } else {
                        randomMovie.observe(this, movie -> {
                            Log.d("hello", "random movie is: " + movie.getId());
                            // set the name, age limit and buttons of the movie
                            movieTitle.setText(movie.getName());
                            ageLimit.setText(String.valueOf(movie.getAge_limit()) + "+");

                            playButton.setOnClickListener(v -> {
                                if (randomMovieId != null) {
                                    Intent intent = new Intent(HomePage.this, VideoPlayerActivity.class);
                                    intent.putExtra("video_id_key", movie.getVideo());
                                    intent.putExtra("movie_id_key", movie.getId());
                                    startActivity(intent);
                                }
                            });

                            moreInfoButton.setOnClickListener(v -> {
                                if (randomMovieId != null) {
                                    Intent intent = new Intent(HomePage.this, MovieDetailsActivity.class);
                                    intent.putExtra("MOVIE_ID", randomMovieId);
                                    startActivity(intent);
                                }
                            });

                            // streaming the video
                            String videoId = movie.getVideo();
                            String videoUrl = MyApplication.getAppContext().getString(R.string.BaseUrl) + "videos/watch/" + videoId;
                            setupVideoPlayer(videoUrl);
                        });
                    }
                });

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

    private void setupVideoPlayer(String videoUrl) {
        VideoPlayerFragment videoPlayerFragment = VideoPlayerFragment.newInstance(videoUrl);
        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.runRandomMovie, videoPlayerFragment);
        transaction.commit();
    }

    // a function to convert from Map<String, List<String>> to List<Category>
    private List<Category> getMoviesFromResponse(Map<String, List<String>> movies) {
        List<Category> results = new ArrayList<>();
        for (Map.Entry<String, List<String>> entry : movies.entrySet()) {
            results.add(new Category(entry.getKey(), true, entry.getValue().toArray(new String[0])));
        }
        return results;
    }

    private static String getRandomMovie(Map<String, List<String>> movies) {
        if (movies == null || movies.isEmpty()) {
            return ""; // No movies available
        }

        List<String> categories = new ArrayList<>(movies.keySet());
        Random random = new Random();

        while (!categories.isEmpty()) {
            // Pick a random category
            String randomCategory = categories.get(random.nextInt(categories.size()));
            List<String> movieList = movies.get(randomCategory);

            // If the selected category has movies, return a random movie
            if (movieList != null && !movieList.isEmpty()) {
                return movieList.get(random.nextInt(movieList.size()));
            }

            // If the selected category is empty, remove it and try again
            categories.remove(randomCategory);
        }

        return ""; // No movies in any category
    }

    @Override
    protected void onPause() {
        super.onPause();

        // Find the VideoPlayerFragment
        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.runRandomMovie);
        if (videoFragment != null) {
            // Stop and release the ExoPlayer
            videoFragment.releasePlayer();
            // Begin transaction to remove the fragment
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.remove(videoFragment);
            transaction.commit();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.runRandomMovie);
        if (videoFragment != null) {
            //videoFragment.setupExoPlayer();
        }
    }

    @Override
    protected void onStart() {
        super.onStart();

        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.runRandomMovie);
        if (videoFragment != null) {
            videoFragment.setupExoPlayer();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.runRandomMovie);
        if (videoFragment != null) {
            videoFragment.releasePlayer();
            // Begin transaction to remove the fragment
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.remove(videoFragment);
            transaction.commit();
        }
    }
}

