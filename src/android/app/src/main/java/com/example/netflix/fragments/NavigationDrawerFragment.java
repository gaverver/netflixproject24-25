package com.example.netflix.fragments;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.Switch;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.activities.AdminPage;
import com.example.netflix.activities.HomePage;
//import com.example.netflix.activities.LoginPage;
import com.example.netflix.activities.MainActivity;
import com.example.netflix.viewmodels.UserViewModel;
import com.google.android.material.navigation.NavigationView;

public class NavigationDrawerFragment extends Fragment {

    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ActionBarDrawerToggle drawerToggle;
    private Switch themeSwitch;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_navigation_drawer, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        Log.d("NavigationDrawer", getActivity().getLocalClassName());
        drawerLayout = getActivity().findViewById(R.id.drawer_layout);
        if (drawerLayout == null) {
            Log.e("NavigationDrawer", "DrawerLayout is null! Check activity XML.");
            return;
        }

        navigationView = view.findViewById(R.id.navigation_view);
        if (navigationView == null) {
            Log.e("NavigationDrawer", "NavigationView is null! Check fragment_navigation_drawer.xml");
            return;
        }
        SharedPreferences prefs = getActivity().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        int isAdmin = prefs.getInt("privilegeLevel", 0);
        // Hide Admin Panel
        Menu menu = navigationView.getMenu();
        MenuItem adminItem = menu.findItem(R.id.nav_admin);
        Log.d("IsAdmin", "" + isAdmin);
        if (adminItem != null) {
            if (isAdmin > 0) {
                adminItem.setVisible(true);
            } else {
                adminItem.setVisible(false);
            }
        }

        // Setup Drawer Toggle
        drawerToggle = new ActionBarDrawerToggle(
                getActivity(),
                drawerLayout,
                R.string.open_drawer,
                R.string.close_drawer
        );
        drawerLayout.addDrawerListener(drawerToggle);
        drawerToggle.syncState();

        // Set Navigation Item Click Listener
        navigationView.setNavigationItemSelectedListener(item -> {
            Log.d("NavigationDrawer", "Menu item clicked: " + item.getTitle()); // Debugging line

            int itemId = item.getItemId();
            if (itemId == R.id.nav_home) {
                startActivity(new Intent(getActivity(), HomePage.class));
            } else if (itemId == R.id.nav_admin) {
                startActivity(new Intent(getActivity(), AdminPage.class));
            } else if (itemId == R.id.nav_sign_out) {
                signOut();
            }

            drawerLayout.closeDrawer(GravityCompat.START);
            return true;
        });


        // Get Switch from Header
        View headerView = navigationView.getHeaderView(0);
        themeSwitch = headerView.findViewById(R.id.switch_theme);

        // Load Theme Preference
//        boolean isDarkMode = prefs.getBoolean("isDarkMode", false);
//        themeSwitch.setChecked(isDarkMode);

        // Toggle Theme
        themeSwitch.setOnCheckedChangeListener((CompoundButton buttonView, boolean isChecked) -> {
            SharedPreferences.Editor editor = prefs.edit();
            editor.putBoolean("isDarkMode", isChecked);
            editor.apply();

            boolean isDarkMode1 = isChecked;
            Log.d("Hello23", isDarkMode1 ? "dark from fragment drawer" : "light from fragment drawer");
            // Set the theme based on the dark mode preference
            if (isDarkMode1) {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
            } else {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
            }

//            getActivity().recreate();
        });
    }

    private void signOut() {
        // Clear shared preferences
        SharedPreferences prefs = getActivity().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        String userId = prefs.getString("userId", "");
        prefs.edit().clear().apply();

        UserViewModel userViewModel = new UserViewModel();
        userViewModel.removeById(userId);

        // Navigate to Login
        Intent intent = new Intent(getActivity(), MainActivity.class);
        startActivity(intent);
        getActivity().finish();
    }
}