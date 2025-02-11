package com.example.netflix.fragments;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.entities.Movie;
import com.example.netflix.viewmodels.MovieViewModel;
import com.example.netflix.WebResponse;

import java.util.List;

public class EditMovieFragment extends Fragment {

    private MovieViewModel movieViewModel;
    private EditText etId, etName, etDescription, etActors, etAgeLimit, etCreators, etCategories, etPublished;
    private Button btnSearch, btnEditMovie, btnDeleteMovie;
    private ViewGroup editContainer;
    private String photoId;
    private String videoId;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_edit_movie, container, false);

        movieViewModel = new MovieViewModel();

        etId = view.findViewById(R.id.et_id);
        etName = view.findViewById(R.id.et_name);
        etDescription = view.findViewById(R.id.et_description);
        etActors = view.findViewById(R.id.et_actors);
        etAgeLimit = view.findViewById(R.id.et_age_limit);
        etCreators = view.findViewById(R.id.et_creators);
        etCategories = view.findViewById(R.id.et_categories);
        etPublished = view.findViewById(R.id.et_published);
        btnSearch = view.findViewById(R.id.btn_search);
        btnEditMovie = view.findViewById(R.id.btn_edit_movie);
        btnDeleteMovie = view.findViewById(R.id.btn_delete_movie);
        editContainer = view.findViewById(R.id.edit_container);

        editContainer.setVisibility(View.GONE);
        btnDeleteMovie.setVisibility(View.GONE);

        btnSearch.setOnClickListener(v -> searchMovie());
        btnEditMovie.setOnClickListener(v -> editMovie());
        btnDeleteMovie.setOnClickListener(v -> deleteMovie());

        photoId = "";
        videoId = "";

        return view;
    }

    private void updateMovie(Movie movie, String id) {
        if (movie.getId().equals(id)) {
            etName.setText(movie.getName());
            etDescription.setText(movie.getDescription());
            etActors.setText(String.join(",", movie.getActors()));
            etAgeLimit.setText(String.valueOf(movie.getAge_limit()));
            etCreators.setText(String.join(",", movie.getCreators()));
            etCategories.setText(String.join(",", movie.getCategories()));
            etPublished.setText(movie.getPublished());

            editContainer.setVisibility(View.VISIBLE);
            btnDeleteMovie.setVisibility(View.VISIBLE);
            return;
        }
        Toast.makeText(getContext(), "Movie not found", Toast.LENGTH_SHORT).show();
        editContainer.setVisibility(View.GONE);
        btnDeleteMovie.setVisibility(View.GONE);
    }

    private void searchMovie() {
        String id = etId.getText().toString().trim();
        if (id.isEmpty()) {
            Toast.makeText(getContext(), "Please enter an ID", Toast.LENGTH_SHORT).show();
            return;
        }

        movieViewModel.getMovie(id, new WebResponse()).observe(getViewLifecycleOwner(), movie -> {
            photoId = movie.getPhotoId();
            videoId = movie.getVideo();
            updateMovie(movie, id);
        });
    }

    private void editMovie() {
        String id = etId.getText().toString().trim();
        String name = etName.getText().toString().trim();
        String description = etDescription.getText().toString().trim();
        String[] actors = etActors.getText().toString().split(",");
        int ageLimit = Integer.parseInt(etAgeLimit.getText().toString().trim());
        String[] creators = etCreators.getText().toString().split(",");
        String[] categories = etCategories.getText().toString().split(",");
        String published = etPublished.getText().toString().trim();

        Movie movie = new Movie(id, name, description, List.of(actors), published, ageLimit, List.of(creators), List.of(categories), photoId, videoId);

        WebResponse webResponse = new WebResponse();
        webResponse.getResponseMsg().observe(getViewLifecycleOwner(), s ->
                Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show());

        movieViewModel.putMovie(movie, webResponse);
    }

    private void deleteMovie() {
        String id = etId.getText().toString().trim();

        WebResponse webResponse = new WebResponse();
        webResponse.getResponseMsg().observe(getViewLifecycleOwner(), s -> {
            Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show();
            if (s.equals("Ok")) {
                etId.setText("");
                etName.setText("");
                etDescription.setText("");
                etActors.setText("");
                etAgeLimit.setText("");
                etCreators.setText("");
                etCategories.setText("");
                etPublished.setText("");
                editContainer.setVisibility(View.GONE);
                btnDeleteMovie.setVisibility(View.GONE);
            }
        });

        movieViewModel.deleteMovie(id, webResponse);
    }
}
