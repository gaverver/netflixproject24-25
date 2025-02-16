package com.example.netflix.activities;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.LiveData;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.adapters.CategoryAdapter;
import com.example.netflix.entities.Category;
import com.example.netflix.viewmodels.CategoryViewModel;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class CategoriesPage extends AppCompatActivity {
    private RecyclerView categoriesRecyclerView;
    private CategoryAdapter categoryAdapter;
    private final CategoryViewModel categoryViewModel = new CategoryViewModel();
    private final WebResponse res = new WebResponse();
    private LiveData<List<Category>> categoriesLiveData;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_categories_page);
        categoriesRecyclerView = findViewById(R.id.categoriesRecyclerView);
        categoriesRecyclerView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));

        fetchCategories();
    }

    private void fetchCategories() {
        // reload the ROOM and fetch all categories from the server
        categoryViewModel.reload(res);
        res.getResponseCode().observe(this, code -> {
            // if the reload failed
            if (code != 200) {
                // make a toast to let the user know, but still show him what he gets from his ROOM
                Toast.makeText(getApplicationContext(), "Failed to fetch categories from the server", Toast.LENGTH_LONG).show();
            }

            // gets all existing categories
            categoriesLiveData = categoryViewModel.getAll();
            categoriesLiveData.observe(this, categories -> {
                // show only the categories whose movies' list isn't empty
                categoryAdapter = new CategoryAdapter(this, categories.stream().filter(new Predicate<Category>() {
                    @Override
                    public boolean test(Category category) {
                        return category.getMovies().length != 0;
                    }
                }).collect(Collectors.toList()));
                categoriesRecyclerView.setAdapter(categoryAdapter);
            });
        });
    }
}
