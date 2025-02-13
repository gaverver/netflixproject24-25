package com.example.netflix.adapters;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.fragment.app.FragmentActivity;
import androidx.lifecycle.LifecycleOwner;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.R;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;
import com.example.netflix.activities.MovieDetailsActivity;
import com.example.netflix.fragments.MoviePic;
import com.example.netflix.viewmodels.ImageViewModel;
import com.example.netflix.viewmodels.MovieViewModel;

import java.util.List;

public class MoviesAdapter extends RecyclerView.Adapter<MoviesAdapter.MovieViewHolder> {

    private List<String> movieIds;
    private Context context;

    public MoviesAdapter(Context context, List<String> movieIds) {
        this.context = context;
        this.movieIds = movieIds;
    }

    @NonNull
    @Override
    public MovieViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_movie_recommendation, parent, false);
        return new MovieViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MovieViewHolder holder, int position) {
        String movieId = movieIds.get(position);

        Log.d("SomeTag", "Position: " + position + " value: " + movieId);

        // Fetch movie details and load image
        MovieViewModel movieModel = new MovieViewModel();
        WebResponse movieResponse = new WebResponse();

        movieModel.getMovie(movieId, movieResponse).observe((LifecycleOwner) context, movie -> {
            if (movie != null) {
                String photoId = movie.getPhotoId();
                if (photoId != null) {
                    ImageViewModel imageViewModel = new ImageViewModel();
                    WebResponse imageResponse = new WebResponse();
                    imageViewModel.get(photoId, imageResponse).observe((LifecycleOwner) context, image -> {
                        if (image != null && image.getData() != null) {
                            Utils.setImageFromByteArray(context, holder.movieImageView, image.getData());
                        }
                    });
                }
            }
        });

        // Click to open Movie Details
        holder.movieImageView.setOnClickListener(v -> {
            Intent intent = new Intent(context, MovieDetailsActivity.class);
            intent.putExtra("MOVIE_ID", movieId);
            context.startActivity(intent);
        });
    }


    @Override
    public int getItemCount() {
        return movieIds.size();
    }

    public static class MovieViewHolder extends RecyclerView.ViewHolder {
        ImageView movieImageView;

        public MovieViewHolder(@NonNull View itemView) {
            super(itemView);
            movieImageView = itemView.findViewById(R.id.movieImageView);
        }
    }

}
