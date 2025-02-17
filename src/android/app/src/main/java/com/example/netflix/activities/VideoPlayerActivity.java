package com.example.netflix.activities;

import android.os.Bundle;
import android.content.Intent;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentTransaction;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.fragments.VideoPlayerFragment;
import com.example.netflix.viewmodels.MovieViewModel;

public class VideoPlayerActivity extends AppCompatActivity {

    private static final String VIDEO_ID_KEY = "video_id_key";
    private static final String MOVIE_ID_KEY = "movie_id_key";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_player);

        // Get the video ID from the Intent
        String videoId = getIntent().getStringExtra(VIDEO_ID_KEY);
        if (videoId != null) {
            // Construct the video URL from the video ID
            String videoUrl = MyApplication.getAppContext().getString(R.string.BaseUrl) + "videos/watch/" + videoId;

            // Create and display the fragment
            VideoPlayerFragment videoPlayerFragment = VideoPlayerFragment.newInstance(videoUrl);
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.replace(R.id.fragment_container, videoPlayerFragment);
            transaction.commit();
            String movieId = getIntent().getStringExtra(MOVIE_ID_KEY);
            MovieViewModel movieViewModel = new MovieViewModel();
            movieViewModel.addWatchedMovie(movieId, new WebResponse());

        }
    }
}
