package com.example.netflix.activities;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;
import androidx.appcompat.widget.Toolbar;

import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.LiveData;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.WebResponse;
import com.example.netflix.adapters.CategoryAdapter;
import com.example.netflix.entities.Category;
import com.example.netflix.fragments.NavigationDrawerFragment;
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
    private Toolbar toolbar;
    private DrawerLayout drawerLayout;
    private View rootView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_categories_page);
        categoriesRecyclerView = findViewById(R.id.categoriesRecyclerView);
        categoriesRecyclerView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.VERTICAL, false));
        rootView = findViewById(android.R.id.content).getRootView();

        //add toolbar
        toolbar = findViewById(R.id.menuToolbar);

        drawerLayout = findViewById(R.id.drawer_layout);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.open_drawer, R.string.close_drawer);
        drawerLayout.addDrawerListener(toggle);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setHomeButtonEnabled(true);
        }

        toggle.syncState();

        //add fragment for navigation drawer
        if (savedInstanceState == null) {
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            NavigationDrawerFragment navigationDrawerFragment = new NavigationDrawerFragment();
            transaction.replace(R.id.navigation_fragment, navigationDrawerFragment); // Add to container
            transaction.commit();
        }

        fetchCategories();

        updateTheme();
    }

    @Override
    protected void onResume() {
        super.onResume();
        updateTheme();
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

    private void updateTheme() {
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("app_prefs", Context.MODE_PRIVATE);
        boolean isDarkMode = sharedPreferences.getBoolean("isDarkMode", true);
        //WindowInsetsControllerCompat windowInsetsController = new WindowInsetsControllerCompat(getWindow(), getWindow().getDecorView());

        //if (isDarkMode) {
        //    windowInsetsController.setAppearanceLightStatusBars(false);
        //} else {
        //    windowInsetsController.setAppearanceLightStatusBars(true);
        //}

        int color = isDarkMode ? ContextCompat.getColor(this, R.color.black)
                : ContextCompat.getColor(this, R.color.white);

        rootView.setBackgroundColor(color);
    }
}
