package com.example.netflix.fragments;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
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

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

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

    // TODO - fix this function and see if it gets the user correctly
    private void getsUserDetails(SharedPreferences sharedPreferences) {
        // gets the user's name and picture to show it on the screen
        String userId = sharedPreferences.getString("userId", null);
        UserViewModel userServices = new UserViewModel();
        ImageViewModel imageServices = new ImageViewModel();
        WebResponse res = new WebResponse();
        LiveData<User> user = userServices.get(userId, res);
        if (res.getResponseCode() != 200) {
            // an error occur
            return;
        }
        TextView userName = rootView.findViewById(R.id.userName);
        ImageView userPic = rootView.findViewById(R.id.userPic);
        // set user name
        userName.setText(Objects.requireNonNull(user.getValue()).getUsername());
        LiveData<Image> image = imageServices.get(user.getValue().getPicture(), res);
        if (res.getResponseCode() != 200) {
            // an error occur
            return;
        }
        // set user picture
        Utils.setImageFromByteArray(userPic, Objects.requireNonNull(image.getValue()).getData());
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
        // ... update other views as needed
    }
}

