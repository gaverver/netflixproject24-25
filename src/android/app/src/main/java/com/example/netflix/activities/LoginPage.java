package com.example.netflix.activities;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.result.ActivityResultLauncher;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.TokenRequest;
import com.example.netflix.WebResponse;
import com.example.netflix.api.TokenAPI;
import com.example.netflix.entities.User;
import com.example.netflix.viewmodels.UserViewModel;
import com.google.android.material.textfield.TextInputEditText;


public class LoginPage extends AppCompatActivity {
    private TextInputEditText username;
    private TextInputEditText password;
    private Button login;
    private AppCompatImageButton home;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login_page);
        username = findViewById(R.id.usernameInput);
        password = findViewById(R.id.passwordInput);
        login = findViewById(R.id.login);
        home = findViewById(R.id.backButton);
        // the sign in should navigate to the login page.
        TextView RegisterLink = findViewById(R.id.RegisterLink);
        RegisterLink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginPage.this, RegistrationPage.class);
                startActivity(intent);
            }
        });



        login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                loginUser();
            }
        });
        home.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(LoginPage.this, HomePage.class);
                startActivity(intent);
            }
        });

    }


    private void loginUser() {
        SharedPreferences sharedPreferences = getSharedPreferences("MyPrefs", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        String usernameText = username.getText().toString().trim();
        String passwordText = password.getText().toString().trim();
        // check if input isn't empty
        if (usernameText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter a username", Toast.LENGTH_SHORT).show();
            return;
        }
        if (passwordText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter a password", Toast.LENGTH_SHORT).show();
            return;
        }

        // create the necessary objects to use the web api and get response
        TokenAPI api = new TokenAPI();
        WebResponse tokenCreation = new WebResponse();
        TokenRequest req = new TokenRequest(usernameText, passwordText);
        WebResponse getUser = new WebResponse();
        WebResponse getUserFromToken = new WebResponse();
        UserViewModel userViewModel = new UserViewModel();
        // create a token and observe a response
        api.createToken(req, tokenCreation);

        tokenCreation.getResponseMsg().observe(LoginPage.this, new Observer<String>() {
            @Override
            public void onChanged(String s) {
                tokenCreation.getResponseCode().observe(LoginPage.this, new Observer<Integer>() {
                    @Override
                    public void onChanged(Integer integer) {
                        if (integer != 201) {
                            // if it wasn't successful then pop up an appropriate message
                            Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                        } else {
                            // if it was successful, get the user id from the token
                            api.getIdFromToken(s, getUserFromToken);
                            getUserFromToken.getResponseMsg().observe(LoginPage.this, new Observer<String>() {
                                @Override
                                public void onChanged(String ID) {
                                    getUserFromToken.getResponseCode().observe(LoginPage.this, new Observer<Integer>() {
                                        @Override
                                        public void onChanged(Integer resCode) {
                                            if (resCode != 200) {
                                                // if it wasn't successful then pop up an appropriate message
                                                Toast.makeText(getApplicationContext(), ID, Toast.LENGTH_SHORT).show();
                                            } else {
                                                // if it was successful, get the user from the id
                                                MutableLiveData<User> user = userViewModel.get(ID, getUser);
                                                getUser.getResponseCode().observe(LoginPage.this, new Observer<Integer>() {
                                                    @Override
                                                    public void onChanged(Integer code1) {
                                                        if (code1 == 200) {
                                                            user.observe(LoginPage.this, new Observer<User>() {
                                                                @Override
                                                                public void onChanged(User user) {
                                                                    // update the shared preference for future usages.
                                                                    editor.putString("token", s);
                                                                    editor.putString("userId", user.getId());
                                                                    editor.putInt("privilegeLevel", user.getPrivilegeLevel());
                                                                    editor.apply();
                                                                    Toast.makeText(getApplicationContext(), "login successful", Toast.LENGTH_SHORT).show();
                                                                    // move to the homePage after registration.
                                                                    Intent intent = new Intent(LoginPage.this, HomePage.class);
                                                                    startActivity(intent);
                                                                }
                                                            });
                                                        } else {
                                                            getUser.getResponseMsg().observe(LoginPage.this, new Observer<String>() {
                                                                @Override
                                                                public void onChanged(String err) {
                                                                    // if it wasn't successful then pop up an appropriate message
                                                                    Toast.makeText(getApplicationContext(), err, Toast.LENGTH_SHORT).show();
                                                                }
                                                            });
                                                        }
                                                    }
                                                });
                                            }
                                        }
                                    });
                                }
                            });
                        }

                    }
                });
            }
        });


    }



}

