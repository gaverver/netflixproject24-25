package com.example.netflix.fragments;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;

import com.example.netflix.R;
import com.example.netflix.activities.HomePage;
import com.example.netflix.activities.SearchResultsPage;
import com.google.android.material.navigation.NavigationView;

public class NavigationDrawerFragment extends Fragment {

    private DrawerLayout drawerLayout;
    private NavigationView navigationView;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_navigation_drawer, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        drawerLayout = getActivity().findViewById(R.id.drawer_layout);
        if (drawerLayout == null) {
            Log.e("NavigationDrawer", "DrawerLayout is null! Check activity XML.");
            return;
        }

        navigationView = view.findViewById(R.id.navigation_view);
        if (navigationView == null) {
            Log.e("NavigationDrawer", "NavigationView is NULL! Check fragment_navigation_drawer.xml");
            return;
        }

        View headerView = navigationView.getHeaderView(0);
        Switch themeSwitch = headerView.findViewById(R.id.switch_dark_mode);
        Button signOutButton = headerView.findViewById(R.id.btn_sign_out);

        SharedPreferences prefs = requireActivity().getSharedPreferences("AppPrefs", getActivity().MODE_PRIVATE);
        boolean isDarkMode = prefs.getBoolean("dark_mode", false);
        themeSwitch.setChecked(isDarkMode);

        themeSwitch.setOnCheckedChangeListener((buttonView, isChecked) -> {
            SharedPreferences.Editor editor = prefs.edit();
            editor.putBoolean("dark_mode", isChecked);
            editor.apply();

            AppCompatDelegate.setDefaultNightMode(isChecked ?
                    AppCompatDelegate.MODE_NIGHT_YES :
                    AppCompatDelegate.MODE_NIGHT_NO);
        });

        signOutButton.setOnClickListener(v -> {
            prefs.edit().clear().apply();
            Intent intent = new Intent(getActivity(), SearchResultsPage.class);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            startActivity(intent);
            getActivity().finish();
        });

        drawerLayout.closeDrawer(GravityCompat.START);
    }
}
