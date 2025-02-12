package com.example.netflix.adapters;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.FragmentActivity;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.R;
import com.example.netflix.activities.MovieDetailsActivity;
import com.example.netflix.fragments.MoviePic;

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

        // Load the fragment inside the FragmentContainerView
        if (context instanceof FragmentActivity) {
            ((FragmentActivity) context).getSupportFragmentManager()
                    .beginTransaction()
                    .replace(holder.fragmentContainer.getId(), MoviePic.newInstance(movieId))
                    .commitAllowingStateLoss();
        }

        // Navigate to movie details when clicked
        holder.fragmentContainer.setOnClickListener(v -> {
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
        View fragmentContainer;

        public MovieViewHolder(@NonNull View itemView) {
            super(itemView);
            fragmentContainer = itemView.findViewById(R.id.movie_pic_container);
        }
    }
}
