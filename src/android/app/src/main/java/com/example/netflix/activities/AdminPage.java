package com.example.netflix.activities;

import android.os.Bundle;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.example.netflix.R;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import com.example.netflix.R;
import com.example.netflix.fragments.AddCategoryFragment;
import com.example.netflix.fragments.AddMovieFragment;
import com.example.netflix.fragments.EditCategoryFragment;
import com.example.netflix.fragments.EditMovieFragment;
import com.example.netflix.fragments.NavigationDrawerFragment;
import com.google.android.material.bottomnavigation.BottomNavigationView;

public class AdminPage extends AppCompatActivity {

    private DrawerLayout drawerLayout;
    private Toolbar toolbar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_admin_page);
        //add toolbar
        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        drawerLayout = findViewById(R.id.drawer_layout);

        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.open_drawer, R.string.close_drawer);
        drawerLayout.addDrawerListener(toggle);

        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setHomeButtonEnabled(true);
        }

        toggle.syncState();

        BottomNavigationView bottomNav = findViewById(R.id.bottom_navigation);
        bottomNav.setOnItemSelectedListener(item -> {
            Fragment selectedFragment = null;
            int itemId = item.getItemId();

            if (itemId == R.id.nav_add_movie) {
                selectedFragment = new AddMovieFragment();
            } else if (itemId == R.id.nav_edit_movie) {
                selectedFragment = new EditMovieFragment();
            } else if (itemId == R.id .nav_add_category) {
                selectedFragment = new AddCategoryFragment();
            } else if (itemId == R.id.nav_edit_category) {
                selectedFragment = new EditCategoryFragment();
            }

            if (selectedFragment != null) {
                getSupportFragmentManager().beginTransaction()
                        .replace(R.id.fragment_container, selectedFragment)
                        .commit();
            }

            return true;
        });


        // Set default fragment
        if (savedInstanceState == null) {
            //transaction for loading the fragments to their containers
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            NavigationDrawerFragment navigationDrawerFragment = new NavigationDrawerFragment();
            transaction.replace(R.id.navigation_fragment, navigationDrawerFragment);
            transaction.replace(R.id.fragment_container, new AddMovieFragment());
            transaction.commit();
        }
    }
}

