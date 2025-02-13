package com.example.netflix.fragments;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.Movie;
import com.example.netflix.viewmodels.MovieViewModel;
import com.example.netflix.viewmodels.VideoViewModel;
import com.example.netflix.viewmodels.ImageViewModel;
import com.example.netflix.WebResponse;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;

public class AddMovieFragment extends Fragment {

    private static final int PICK_VIDEO_REQUEST = 1;
    private static final int PICK_IMAGE_REQUEST = 2;

    private MovieViewModel movieViewModel;
    private VideoViewModel videoViewModel;
    private ImageViewModel imageViewModel;
    private EditText etName, etDescription, etActors, etAgeLimit, etCreators, etCategories, etPublished;
    private Button btnAddMovie, btnSelectVideo, btnSelectImage;
    private String videoId, photoId;
    private Uri videoUri, imageUri;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_add_movie, container, false);

        movieViewModel = new MovieViewModel();
        videoViewModel = new VideoViewModel();
        imageViewModel = new ImageViewModel();

        etName = view.findViewById(R.id.et_name);
        etDescription = view.findViewById(R.id.et_description);
        etActors = view.findViewById(R.id.et_actors);
        etAgeLimit = view.findViewById(R.id.et_age_limit);
        etCreators = view.findViewById(R.id.et_creators);
        etCategories = view.findViewById(R.id.et_categories);
        etPublished = view.findViewById(R.id.et_published);
        btnAddMovie = view.findViewById(R.id.btn_add_movie);
        btnSelectVideo = view.findViewById(R.id.btn_select_video);
        btnSelectImage = view.findViewById(R.id.btn_select_image);

        btnSelectVideo.setOnClickListener(v -> selectVideo());
        btnSelectImage.setOnClickListener(v -> selectImage());
        btnAddMovie.setOnClickListener(v -> addMovie());

        return view;
    }

    private void selectVideo() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Video.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, PICK_VIDEO_REQUEST);
    }

    private void selectImage() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, PICK_IMAGE_REQUEST);
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == getActivity().RESULT_OK && data != null) {
            if (requestCode == PICK_VIDEO_REQUEST) {
                videoUri = data.getData();
                Toast.makeText(getContext(), "Video selected", Toast.LENGTH_SHORT).show();
            } else if (requestCode == PICK_IMAGE_REQUEST) {
                imageUri = data.getData();
                Toast.makeText(getContext(), "Image selected", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void addMovie() {
        if (videoUri == null || imageUri == null) {
            Toast.makeText(getContext(), "Please select both video and image", Toast.LENGTH_SHORT).show();
            return;
        }

        uploadVideo();
    }

    private void uploadVideo() {
        try {
            InputStream inputStream = getContext().getContentResolver().openInputStream(videoUri);
            File tempFile = File.createTempFile("video", ".mp4", getContext().getCacheDir());

            try (OutputStream outputStream = new java.io.FileOutputStream(tempFile)) {
                byte[] buffer = new byte[4 * 1024]; // 4KB buffer
                int read;
                while ((read = inputStream.read(buffer)) != -1) {
                    outputStream.write(buffer, 0, read);
                }
                outputStream.flush();
            }

            WebResponse videoRes = new WebResponse();
            videoRes.getResponseCode().observe(getViewLifecycleOwner(), code -> {
                Log.d("SomeTag","im here with code " + code);
                if (code == 201) {
                    videoRes.getResponseMsg().observe(getViewLifecycleOwner(), msg -> {
                        videoId = msg;
                        uploadImage();
                    });
                } else {
                    Toast.makeText(getContext(), "Video upload failed", Toast.LENGTH_SHORT).show();
                }
            });
            videoRes.getResponseMsg().observe(getViewLifecycleOwner(), msg -> {
                Log.d("SomeTag","response message: " + msg);
            });
            videoViewModel.uploadVideo(tempFile, videoRes);
        } catch (IOException e) {
            Log.e("SomeTag", "Error creating temp file: " + e.getMessage());
            Toast.makeText(getContext(), "Error preparing video: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }
    }

    private void uploadImage() {
        try {
            InputStream is = getContext().getContentResolver().openInputStream(imageUri);
            byte[] imageData = new byte[is.available()];
            is.read(imageData);
            is.close();

            String contentType = getContext().getContentResolver().getType(imageUri);
            Log.d("SomeTag", "image type: " + contentType);
            Image image = new Image(imageData, contentType);

            WebResponse imageRes = new WebResponse();
            imageRes.getResponseCode().observe(getViewLifecycleOwner(), code -> {
                if (code == 201) {
                    photoId = image.getId();
                    Log.d("SomeTag", "im here and this is the photo id: " + image.getId());
                    createMovie();
                } else {
                    Toast.makeText(getContext(), "Image upload failed", Toast.LENGTH_SHORT).show();
                }
            });
            imageViewModel.create(image, imageRes);
        } catch (IOException e) {
            e.printStackTrace();
            Toast.makeText(getContext(), "Error reading image file", Toast.LENGTH_SHORT).show();
        }
    }

    private void createMovie() {
        String name = etName.getText().toString().trim();
        String description = etDescription.getText().toString().trim();
        String creatorsString = etCreators.getText().toString().trim();

        // Check each required field individually
        if (TextUtils.isEmpty(name)) {
            Toast.makeText(getContext(), "Please enter the movie name", Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(description)) {
            Toast.makeText(getContext(), "Please enter the movie description", Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(creatorsString)) {
            Toast.makeText(getContext(), "Please enter the movie creators", Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(photoId)) {
            Toast.makeText(getContext(), "Please select an image for the movie", Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(videoId)) {
            Toast.makeText(getContext(), "Please select a video for the movie", Toast.LENGTH_SHORT).show();
            return;
        }

        String[] creators = creatorsString.split(",");

        // Set default values for optional fields
        String[] actors = TextUtils.isEmpty(etActors.getText()) ? new String[0] : etActors.getText().toString().split(",");
        int ageLimit = TextUtils.isEmpty(etAgeLimit.getText()) ? 0 : Integer.parseInt(etAgeLimit.getText().toString());
        String[] categories = TextUtils.isEmpty(etCategories.getText()) ? new String[0] : etCategories.getText().toString().split(",");

        // Set current date as default for published
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
        String published = TextUtils.isEmpty(etPublished.getText()) ? sdf.format(new Date()) : etPublished.getText().toString();

        Movie movie = new Movie("", name, description, Arrays.asList(actors), published, ageLimit,
                Arrays.asList(creators), Arrays.asList(categories), photoId, videoId);

        WebResponse res = new WebResponse();
        res.getResponseCode().observe(getViewLifecycleOwner(), code -> {
            Toast.makeText(getContext(), "Response code: " + code, Toast.LENGTH_SHORT).show();
        });

        res.getResponseMsg().observe(getViewLifecycleOwner(), msg -> {
            Toast.makeText(getContext(), "Response message: " + msg, Toast.LENGTH_LONG).show();
        });
        movieViewModel.createMovie(movie, res);
    }
}