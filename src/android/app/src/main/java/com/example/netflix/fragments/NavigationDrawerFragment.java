package com.example.netflix.fragments;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;

import com.example.netflix.activities.AdminPage;
import com.example.netflix.activities.CategoriesPage;
import com.example.netflix.activities.HomePage;
import com.google.android.material.navigation.NavigationView;

import com.example.netflix.R;

public class NavigationDrawerFragment extends Fragment {

    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ActionBarDrawerToggle drawerToggle;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_navigation_drawer, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // Ensure that the parent Activity has a DrawerLayout
        drawerLayout = getActivity().findViewById(R.id.drawer_layout);
        if (drawerLayout == null) {
            Log.e("NavigationDrawer", "DrawerLayout is null! Check activity XML.");
            return; // Prevent further execution
        }

        navigationView = view.findViewById(R.id.navigation_view);

        if (navigationView == null) {
            Log.e("SomeTag", "NavigationView is NULL! Check fragment_navigation_drawer.xml");
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

        navigationView.setNavigationItemSelectedListener(item -> {
            Intent intent = null;
            if (item.getItemId() == R.id.nav_home) {
                Log.d("SomeTag", "Im here!!!");
                intent = new Intent(getActivity(), AdminPage.class);
            } else if (item.getItemId() == R.id.nav_settings) {
                intent = new Intent(getActivity(), CategoriesPage.class);
            }
            if (intent != null) {
                Log.d("SomeTag", "Im here 2!!!");
                startActivity(intent);
                getActivity().finish();
            }
            drawerLayout.closeDrawer(GravityCompat.START);
            return true;
        });
    }
}
