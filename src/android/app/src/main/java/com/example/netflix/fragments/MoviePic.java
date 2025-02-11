package com.example.netflix.fragments;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.Movie;
import com.example.netflix.viewmodels.ImageViewModel;
import com.example.netflix.viewmodels.MovieViewModel;

public class MoviePic extends Fragment {

    private static final String ARG_MOVIE_ID = "movie_id";
    private ImageView imageView;

    public static MoviePic newInstance(String id) {
        MoviePic fragment = new MoviePic();
        Bundle args = new Bundle();
        args.putString(ARG_MOVIE_ID, id);
        fragment.setArguments(args);
        return fragment;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_movie_pic, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        imageView = view.findViewById(R.id.imageView);
        MovieViewModel movieModel = new MovieViewModel();
        ImageViewModel imageViewModel = new ImageViewModel();

        if (getArguments() != null) {
            String movieId = getArguments().getString(ARG_MOVIE_ID);
            if (movieId != null) {
                Log.d("MoviePic", "Fetching movie with ID: " + movieId);

                WebResponse movie_res = new WebResponse();
                LiveData<Movie> movieLiveData = movieModel.getMovie(movieId, movie_res);

                Log.d("MoviePic", "Observing movieLiveData...");
                movieLiveData.observe(getViewLifecycleOwner(), new Observer<Movie>() {
                    @Override
                    public void onChanged(Movie movie) {
                        Log.d("MoviePic", "Movie data changed");
                        movie_res.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
                            @Override
                            public void onChanged(Integer code) {
                                Log.d("MoviePic", "Response code for movie: " + code);
                                if (code != null && code.equals(200)) {
                                    String photoId = movie.getPhotoId();
                                    Log.d("MoviePic", "Movie retrieved successfully. Photo ID: " + photoId);

                                    if (photoId != null) {
                                        fetchAndDisplayImage(photoId, imageViewModel);
                                    } else {
                                        Toast.makeText(getContext(), "Movie has no associated image", Toast.LENGTH_SHORT).show();
                                    }
                                } else {
                                    Toast.makeText(getContext(), "Error fetching movie", Toast.LENGTH_SHORT).show();
                                }
                            }
                        });
                    }
                });
            }
        }
    }

    private void fetchAndDisplayImage(String photoId, ImageViewModel imageViewModel) {

        Log.d("MoviePic", "Fetching image for photoId: " + photoId);
        WebResponse image_res = new WebResponse();
        LiveData<Image> imageLiveData = imageViewModel.get(photoId, image_res);

        Log.d("MoviePic", "Observing imageLiveData...");
        imageLiveData.observe(getViewLifecycleOwner(), new Observer<Image>() {
            @Override
            public void onChanged(Image image) {
                Log.d("MoviePic", "Inside onChanged for image.");
                image_res.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
                    @Override
                    public void onChanged(Integer code) {
                        Log.d("MoviePic", "Response code for image: " + code);
                        if (code != null && code.equals(200)) {
                            Log.d("MoviePic", "Image retrieved successfully");
                            byte[] imageData = image.getData();

                            if (imageData != null && imageData.length > 0) {
                                Log.d("MoviePic", "Image data size: " + imageData.length);
                                Bitmap bitmap = BitmapFactory.decodeByteArray(imageData, 0, imageData.length);

                                // Scaling the image to fit the ImageView size
                                Bitmap scaledBitmap = Bitmap.createScaledBitmap(bitmap, imageView.getWidth(), imageView.getHeight(), true);

                                // Ensure the image view is set on the UI thread
                                getActivity().runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        imageView.setImageBitmap(scaledBitmap);
                                    }
                                });
                            } else {
                                Toast.makeText(getContext(), "Image data is empty", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(getContext(), "Error fetching image", Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }
        });
    }
}
