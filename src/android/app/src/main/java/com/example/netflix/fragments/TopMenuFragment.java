package com.example.netflix.fragments;

import com.example.netflix.R;

import android.health.connect.datatypes.SleepSessionRecord;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.SearchView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Observer;

import com.example.netflix.MyApplication;
import com.example.netflix.R;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;
import com.example.netflix.activities.SearchResultsPage;
import com.example.netflix.activities.HomePage;
import com.example.netflix.activities.CategoriesPage;
import com.example.netflix.activities.AdminPage;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.User;
import com.example.netflix.viewmodels.ImageViewModel;
import com.example.netflix.viewmodels.UserViewModel;

import java.util.Objects;

public class TopMenuFragment extends Fragment {

    private View rootView;
    private SearchView searchView;
    private Button homeButton;
    private Button adminButton;
    private Button categoriesButton;
    private TextView userName;

    public TopMenuFragment() {
        // empty
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootView = inflater.inflate(R.layout.fragment_top_menu, container, false);

        // Initialize Views
        searchView = rootView.findViewById(R.id.navigateButton);
        homeButton = rootView.findViewById(R.id.home_page_button);
        adminButton = rootView.findViewById(R.id.administration_page_button);
        categoriesButton = rootView.findViewById(R.id.categories_page_button);
        userName = rootView.findViewById(R.id.userName);

        // getting if the user is an admin through the sharedPreference
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        int privilegedLevel = sharedPreferences.getInt("privilegeLevel", 0);
        if (privilegedLevel == 0) {
            // no admin button
            adminButton.setVisibility(View.GONE);
        }

        // gets the user's name and picture to show it on the screen
        getsUserDetails(sharedPreferences);

        setupListeners();

        return rootView;
    }

    private void getsUserDetails(SharedPreferences sharedPreferences) {
        ImageView userPic = rootView.findViewById(R.id.userPic);
        // gets the user's name and picture to show it on the screen
        String userId = sharedPreferences.getString("userId", "679c0a6a0a6cabd498442b92");
        UserViewModel userServices = new UserViewModel();
        ImageViewModel imageServices = new ImageViewModel();
        WebResponse res = new WebResponse();
        WebResponse res2 = new WebResponse();
        LiveData<User> user2 = userServices.get(userId, res);

        // observe the response code
        res.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
            @Override
            public void onChanged(Integer responseCode1) {
                if (responseCode1 == 200) {
                    // observe to see the user Live data changes
                    user2.observe(getViewLifecycleOwner(), new Observer<User>() {
                        @Override
                        public void onChanged(User user) {
                            // set user name
                            userName.setText(user.getUsername());
                            // try to get the user's image
                            LiveData<Image> image = imageServices.get(user.getPicture(), res2);
                            // observe where it's changed
                            res2.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
                                @Override
                                public void onChanged(Integer responseCode2) {
                                    if (responseCode2 == 200) {
                                        // observe when the image is changed
                                        image.observe(getViewLifecycleOwner(), new Observer<Image>() {
                                            @Override
                                            public void onChanged(Image image) {
                                                // upload the image to the imageView
                                                Utils.setImageFromByteArray(getContext() ,userPic, image.getData());
                                            }
                                        });
                                    } else {
                                        // raise a toast
                                        Toast.makeText(getContext(), "Failed to fetch picture, failed with code: " + responseCode1, Toast.LENGTH_LONG).show();
                                    }
                                }
                            });
                        }
                    });
                } else {
                    // raise a toast
                    Toast.makeText(getContext(), "Failed to fetch user details, failed with code: " + responseCode1, Toast.LENGTH_LONG).show();
                }
            }
        });
    }

    private void setupListeners() {

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                Intent intent = new Intent(getActivity(), SearchResultsPage.class);
                intent.putExtra("SEARCH_QUERY", query);
                startActivity(intent);
                return true;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                return false;
            }
        });

        homeButton.setOnClickListener(v -> startActivity(new Intent(getActivity(), HomePage.class)));
        categoriesButton.setOnClickListener(v -> startActivity(new Intent(getActivity(), CategoriesPage.class)));
        adminButton.setOnClickListener(v -> startActivity(new Intent(getActivity(), AdminPage.class)));
    }
}