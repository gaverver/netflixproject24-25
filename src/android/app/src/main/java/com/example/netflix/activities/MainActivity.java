package com.example.netflix.activities;

import android.content.Context;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;
import androidx.core.app.ActivityOptionsCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.example.netflix.R;
import com.example.netflix.fragments.MoviePic;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Context context = getApplicationContext();
        context.deleteDatabase("local_database");
        setContentView(R.layout.activity_main);
        String movieId = "67ab7dfd6229abc387392c2e";  // Example movie ID

        // Create a new instance of the MoviePic fragment
        MoviePic moviePicFragment = MoviePic.newInstance(movieId);

        // Add the fragment to the fragment container
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.fragment_container, moviePicFragment); // Replace existing fragment if any
        fragmentTransaction.addToBackStack(null); // Optional: Adds this transaction to the back stack
        fragmentTransaction.commit();

        AppCompatButton getStartedButton = findViewById(R.id.getStartedButton);
        getStartedButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Handle button click
            }
        });
    }
}
