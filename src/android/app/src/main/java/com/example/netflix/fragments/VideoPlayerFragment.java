package com.example.netflix.fragments;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import com.example.netflix.R;
import com.google.android.exoplayer2.ExoPlayer;
import com.google.android.exoplayer2.MediaItem;
import com.google.android.exoplayer2.PlaybackException;
import com.google.android.exoplayer2.Player;
import com.google.android.exoplayer2.ui.PlayerView;
import com.google.android.exoplayer2.source.MediaSource;
import com.google.android.exoplayer2.source.ProgressiveMediaSource;
import com.google.android.exoplayer2.upstream.DefaultHttpDataSource;

public class VideoPlayerFragment extends Fragment {

    private static final String VIDEO_URL = "video_url";
    private static final String TAG = "VideoPlayerFragment";

    private PlayerView playerView;
    private ExoPlayer exoPlayer;
    private String videoUrl;

    public static VideoPlayerFragment newInstance(String videoUrl) {
        VideoPlayerFragment fragment = new VideoPlayerFragment();
        Bundle args = new Bundle();
        args.putString(VIDEO_URL, videoUrl);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_video_player, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        playerView = view.findViewById(R.id.playerView);

        if (getArguments() != null) {
            videoUrl = getArguments().getString(VIDEO_URL);
            Log.d(TAG, "Streaming from: " + videoUrl);
        }

        setupExoPlayer();
    }

    public void setupExoPlayer() {
        if (videoUrl == null || videoUrl.isEmpty()) {
            Log.e(TAG, "Video URL is null or empty");
            return;
        }

        exoPlayer = new ExoPlayer.Builder(requireContext()).build();
        playerView.setPlayer(exoPlayer);

        MediaItem mediaItem = new MediaItem.Builder()
                .setUri(videoUrl)
                .build();

        MediaSource mediaSource = new ProgressiveMediaSource.Factory(
                new DefaultHttpDataSource.Factory()
                        .setAllowCrossProtocolRedirects(true) // Handle redirects
        ).createMediaSource(mediaItem);

        exoPlayer.setMediaSource(mediaSource);
        exoPlayer.prepare();
        exoPlayer.play();

        // Error handling
        exoPlayer.addListener(new Player.Listener() {
            @Override
            public void onPlayerError(@NonNull PlaybackException error) {
                Log.e(TAG, "Playback error: " + error.getMessage());
            }
        });
    }

    @Override
    public void onStop() {
        super.onStop();
        if (exoPlayer != null) {
            exoPlayer.pause();
            exoPlayer.release();
            exoPlayer = null;
        }
    }
}
