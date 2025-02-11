package com.example.netflix.fragments;

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

public class MenuFragment extends Fragment {

    private Button toggleButton;
    private LinearLayout menuBar;
    private SearchView searchView;
    private boolean isMenuVisible = false;
    private Switch modeSwitch;
    private boolean isDark = true;
    private Button homeButton;
    private Button adminButton;
    private Button categoriesButton;
    private Button signOutButton;
    private View rootView;

    private TextView userName;

    public MenuFragment() {
        // empty
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        rootView = inflater.inflate(R.layout.fragment_menu, container, false);

        // Initialize Views
        toggleButton = rootView.findViewById(R.id.toggleButton);
        menuBar = rootView.findViewById(R.id.menuBar);
        searchView = rootView.findViewById(R.id.navigateButton);
        modeSwitch = rootView.findViewById(R.id.dark_light_mode);
        homeButton = rootView.findViewById(R.id.home_page_button);
        adminButton = rootView.findViewById(R.id.administration_page_button);
        categoriesButton = rootView.findViewById(R.id.categories_page_button);
        signOutButton = rootView.findViewById(R.id.sign_out_button);
        userName = rootView.findViewById(R.id.userName);

        // getting if the user is an admin through the sharedPreference
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        int privilegedLevel = sharedPreferences.getInt("privilegeLevel", 0);
        if (privilegedLevel == 0) {
            // no admin button
            adminButton.setVisibility(View.GONE);
        }

        // set the mode (dark/light) in the shared preference to know the mode even after switching activities
        boolean isDarkMode = sharedPreferences.getBoolean("isDarkMode", true);
        // if we are in Light-Mode
        if (!isDarkMode) {
            isDark = false;
            updateTheme();
        }

        // gets the user's name and picture to show it on the screen
        getsUserDetails(sharedPreferences);

        setupListeners();

        return rootView;
    }

    private void getsUserDetails(SharedPreferences sharedPreferences) {
        ImageView userPic = rootView.findViewById(R.id.userPic);
        // gets the user's name and picture to show it on the screen
        String userId = sharedPreferences.getString("userId", null);
        UserViewModel userServices = new UserViewModel();
        ImageViewModel imageServices = new ImageViewModel();
        WebResponse res = new WebResponse();
        LiveData<User> user = userServices.get(userId, res);

        // observe the response code
        res.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
            @Override
            public void onChanged(Integer responseCode1) {
                if (responseCode1 == 200) {
                    // observe to see the user Live data changes
                    user.observe(getViewLifecycleOwner(), new Observer<User>() {
                        @Override
                        public void onChanged(User user) {
                            // set user name
                            userName.setText(user.getUsername());
                            // try to get the user's image
                            LiveData<Image> image = imageServices.get(user.getPicture(), res);
                            // observe where it's changed
                            res.getResponseCode().observe(getViewLifecycleOwner(), new Observer<Integer>() {
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
                                        Toast.makeText(getContext(), "Failed to fetch picture, failed with code: " + responseCode1, Toast.LENGTH_SHORT).show();
                                    }
                                }
                            });
                        }
                    });
                } else {
                    // raise a toast
                    Toast.makeText(getContext(), "Failed to fetch user details, failed with code: " + responseCode1, Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void setupListeners() {
        toggleButton.setOnClickListener(v -> toggleMenu());

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

        modeSwitch.setOnCheckedChangeListener((buttonView, isChecked) -> {
            isDark = !isChecked;
            updateTheme();
        });

        signOutButton.setOnClickListener(v -> {
            // removing everything from the shared preference (token, privileged level etc...)
            SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.clear();
            editor.apply();
            // and go to login
            Intent intent = new Intent(getActivity(), SearchResultsPage.class);
            startActivity(intent);
        });
    }

    private void toggleMenu() {
        if (!isMenuVisible) {
            slideMenu(true);
            toggleButton.setText(">");
        } else {
            slideMenu(false);
            toggleButton.setText("<");
        }
        isMenuVisible = !isMenuVisible;
    }

    private void slideMenu(boolean show) {
        Animation slideAnimation;
        if (show) {
            slideAnimation = new TranslateAnimation(menuBar.getWidth(), 0, 0, 0);
        } else {
            slideAnimation = new TranslateAnimation(0, menuBar.getWidth(), 0, 0);
        }
        slideAnimation.setDuration(300);
        slideAnimation.setFillAfter(true);
        menuBar.startAnimation(slideAnimation);
    }

    // update the theme according to the current state
    private void updateTheme() {
        // first, put the mode in the shared preference
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean("isDarkMode", isDark);
        editor.apply();

        int backgroundColor = isDark ? R.color.black : R.color.white;
        int textColor = isDark ? R.color.white : R.color.black;

        rootView.setBackgroundResource(backgroundColor);
        modeSwitch.setText(isDark ? "Dark-Mode" : "Light-Mode");

        // Update text colors for buttons and other views as needed
        homeButton.setTextColor(getResources().getColor(textColor));
        categoriesButton.setTextColor(getResources().getColor(textColor));
        adminButton.setTextColor(getResources().getColor(textColor));
        userName.setTextColor(getResources().getColor(textColor));
    }
}

