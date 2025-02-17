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

import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.Observer;
import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.adapters.MoviesAdapter;
import com.example.netflix.entities.Movie;
import com.example.netflix.fragments.NavigationDrawerFragment;
import com.example.netflix.fragments.VideoPlayerFragment;
import com.example.netflix.viewmodels.MovieViewModel;
import com.example.netflix.WebResponse;

public class MovieDetailsActivity extends AppCompatActivity {

    private MovieViewModel movieViewModel;
    private TextView tvName, tvDescription, tvActors, tvAgeLimit, tvCreators, tvCategories, tvPublished;
    private Button btnWatchFullScreen;

    private DrawerLayout drawerLayout;
    private Toolbar toolbar;


    private RecyclerView recommendationsRecycler;
    private MoviesAdapter recommendationsAdapter;
    private String videoUrl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_movie_details);
        //add toolbar
        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        drawerLayout = findViewById(R.id.drawer_layout);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.open_drawer, R.string.close_drawer);
        drawerLayout.addDrawerListener(toggle);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setHomeButtonEnabled(true);
        }

        toggle.syncState();

        movieViewModel = new MovieViewModel();

        // Initialize views
        tvName = findViewById(R.id.tv_name);
        tvDescription = findViewById(R.id.tv_description);
        tvActors = findViewById(R.id.tv_actors);
        tvAgeLimit = findViewById(R.id.tv_age_limit);
        tvCreators = findViewById(R.id.tv_creators);
        tvCategories = findViewById(R.id.tv_categories);
        tvPublished = findViewById(R.id.tv_published);
        btnWatchFullScreen = findViewById(R.id.btn_watch_full_screen);

        recommendationsRecycler = findViewById(R.id.recommendations_recycler);
        recommendationsRecycler.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false));

        // Get movie ID from intent
        String movieId = getIntent().getStringExtra("MOVIE_ID");
        if (movieId == null) {
            Toast.makeText(this, "Movie ID not provided", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // Load movie details
        loadMovieDetails(movieId);
        loadRecommendations(movieId);
        //add fragment for navigation drawer
        if (savedInstanceState == null) {
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            NavigationDrawerFragment navigationDrawerFragment = new NavigationDrawerFragment();
            transaction.replace(R.id.navigation_fragment, navigationDrawerFragment); // Add to container
            transaction.commit();
        }
    }

    private void loadMovieDetails(String movieId) {
        movieViewModel.getMovie(movieId, new WebResponse()).observe(this, new Observer<Movie>() {
            @Override
            public void onChanged(Movie movie) {
                if (movie != null) {
                    updateUI(movie);
                    videoUrl = MyApplication.getAppContext().getString(R.string.BaseUrl) + "videos/watch/" + movie.getVideo();
                    setupVideoPlayer(videoUrl);
                    setupWatchFullScreenButton(movie);
                } else {
                    Toast.makeText(MovieDetailsActivity.this, "Failed to load movie details", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void updateUI(Movie movie) {
        tvName.setText(movie.getName());
        tvDescription.setText(movie.getDescription());
        tvActors.setText("Actors: " + String.join(", ", movie.getActors()));
        tvAgeLimit.setText("Age Limit: " + movie.getAge_limit());
        tvCreators.setText("Creators: " + String.join(", ", movie.getCreators()));
        tvCategories.setText("Categories: " + String.join(", ", movie.getCategories()));
        tvPublished.setText("Published: " + movie.getPublished());
    }

    private void setupVideoPlayer(String videoUrl) {
        VideoPlayerFragment videoPlayerFragment = VideoPlayerFragment.newInstance(videoUrl);
        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.video_container, videoPlayerFragment);
        transaction.commit();
    }

    private void setupWatchFullScreenButton(final Movie movie) {
        btnWatchFullScreen.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MovieDetailsActivity.this, VideoPlayerActivity.class);
                intent.putExtra("video_id_key", movie.getVideo());
                intent.putExtra("movie_id_key", movie.getId());
                startActivity(intent);
            }
        });
    }

    private void loadRecommendations(String movieId) {
        movieViewModel.getRecommendationIds(movieId, new WebResponse()).observe(this, recommendationIds -> {
            if (recommendationIds != null && !recommendationIds.isEmpty()) {
                recommendationsAdapter = new MoviesAdapter(this, recommendationIds);
                recommendationsRecycler.setAdapter(recommendationsAdapter);
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();

        // Find the VideoPlayerFragment
        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.video_container);
        if (videoFragment != null) {
            // Stop and release the ExoPlayer
            Log.d("SOLID", "im here 3");
            videoFragment.releasePlayer();
//            // Begin transaction to remove the fragment
//            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
//            transaction.remove(videoFragment);
//            transaction.commit();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.video_container);
        if (videoFragment != null) {
            videoFragment.resumePlayer();
        } else {
            VideoPlayerFragment videoPlayerFragment = VideoPlayerFragment.newInstance(this.videoUrl);
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.replace(R.id.video_container, videoPlayerFragment);
            transaction.commit();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        VideoPlayerFragment videoFragment = (VideoPlayerFragment) getSupportFragmentManager().findFragmentById(R.id.video_container);
        if (videoFragment != null) {
            videoFragment.releasePlayer();
            // Begin transaction to remove the fragment
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.remove(videoFragment);
            transaction.commit();

        }
    }
}