package com.example.netflix.activities;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import android.util.Base64;
import java.io.ByteArrayOutputStream;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatImageButton;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.TokenRequest;
import com.example.netflix.WebResponse;
import com.example.netflix.api.TokenAPI;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.User;
import com.example.netflix.viewmodels.ImageViewModel;
import com.example.netflix.viewmodels.UserViewModel;
import com.google.android.material.textfield.TextInputEditText;


public class RegistrationPage extends AppCompatActivity {
    private ImageView profileImageView;
    private ActivityResultLauncher<Intent> pickImageLauncher;
    private ActivityResultLauncher<Intent> takePictureLauncher;
    private TextInputEditText username;
    private TextInputEditText password;
    private TextInputEditText email;
    private TextInputEditText phoneNumber;
    private Button register;
    private AppCompatImageButton home;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_registration_page);
        username = findViewById(R.id.usernameInput);
        password = findViewById(R.id.passwordInput);
        email = findViewById(R.id.emailInput);
        phoneNumber = findViewById(R.id.phoneNumberInput);
        register = findViewById(R.id.register);
        home = findViewById(R.id.backButton);
        // the sign in should navigate to the login page.
        TextView signInLink = findViewById(R.id.signInLink);
        signInLink.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(RegistrationPage.this, LoginPage.class);
                startActivity(intent);
            }
        });

        home.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(RegistrationPage.this, HomePage.class);
                startActivity(intent);
            }
        });

        profileImageView = findViewById(R.id.profileImageView);
        Button uploadImageButton = findViewById(R.id.uploadImageButton);

        // Initialize image picker launcher (Gallery)
        pickImageLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                        Uri imageUri = result.getData().getData();
                        profileImageView.setImageURI(imageUri); // Sets selected image
                    }
                }
        );

        // Initialize camera launcher (Camera)
        takePictureLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == RESULT_OK && result.getData() != null) {
                        Bundle extras = result.getData().getExtras();
                        Bitmap imageBitmap = (Bitmap) extras.get("data");
                        profileImageView.setImageBitmap(imageBitmap); // Sets taken image as bitmap
                    }
                }
        );

        // Set button click listener to show options for Gallery or Camera
        uploadImageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showImageSourceDialog();
            }
        });

        register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                registerUser();
            }
        });
    }

    // Show a dialog to choose between Camera or Gallery
    private void showImageSourceDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select Image Source")
                .setItems(new String[]{"Gallery", "Camera"}, (dialog, which) -> {
                    switch (which) {
                        case 0: // Gallery
                            openGallery();
                            break;
                        case 1: // Camera
                            openCamera();
                            break;
                    }
                })
                .create()
                .show();
    }

    // Opens the gallery for image selection
    private void openGallery() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        pickImageLauncher.launch(intent);
    }

    // Opens the camera for taking a picture
    private void openCamera() {
        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        takePictureLauncher.launch(intent);
    }
    private boolean isImageUploaded() {
        return profileImageView.getDrawable() != null;
    }


    private byte[] convertImageToByteArray() {
        // Check if an image has been uploaded
        if (!isImageUploaded()) {
            Toast.makeText(getApplicationContext(), "Please upload a profile image", Toast.LENGTH_SHORT).show();
            return null; // Return null or an empty string to indicate no image
        }

        // Get the Bitmap from ImageView
        profileImageView.setDrawingCacheEnabled(true);
        profileImageView.buildDrawingCache();
        Bitmap bitmap = profileImageView.getDrawingCache();

        // Convert Bitmap to ByteArrayOutputStream
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream); // PNG format, 100 quality
        byte[] byteArray = byteArrayOutputStream.toByteArray();

        // Encode ByteArray to Base64
        return byteArray;
    }

    private void registerUser() {
        SharedPreferences sharedPreferences = getSharedPreferences("MyPrefs", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        String usernameText = username.getText().toString().trim();
        String passwordText = password.getText().toString().trim();
        String emailText = email.getText().toString().trim();
        String phoneNumberText = phoneNumber.getText().toString().trim();
        // check if input isn't empty
        if (usernameText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter a username", Toast.LENGTH_SHORT).show();
            return;
        }
        if (passwordText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter a password", Toast.LENGTH_SHORT).show();
            return;
        }
        if (emailText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter an email", Toast.LENGTH_SHORT).show();
            return;
        }
        if (phoneNumberText.isEmpty()) {
            Toast.makeText(getApplicationContext(), "please enter a phoneNumber", Toast.LENGTH_SHORT).show();
            return;
        }
        if (!isImageUploaded()) {
            Toast.makeText(getApplicationContext(), "Please upload a profile image", Toast.LENGTH_SHORT).show();
            return;
        }
        byte[] base64_picture = convertImageToByteArray();
        if (base64_picture == null) {
            Toast.makeText(getApplicationContext(), "Please upload a profile image", Toast.LENGTH_SHORT).show();
            return;
        }
        // create the necessary objects to use the web api and get response
        UserViewModel userViewModel = new UserViewModel();
        TokenAPI api = new TokenAPI();
        ImageViewModel imageViewModel = new ImageViewModel();
        WebResponse imageCreation = new WebResponse();
        WebResponse userCreation = new WebResponse();
        WebResponse tokenCreation = new WebResponse();
        Image image = new Image(base64_picture, "application/octet-stream");
        imageViewModel.create(image, imageCreation);
        // create an image and observe a response
        imageCreation.getResponseMsg().observe(RegistrationPage.this, new Observer<String>() {
            @Override
            public void onChanged(String s1) {
                imageCreation.getResponseCode().observe(RegistrationPage.this, new Observer<Integer>() {
                    @Override
                    public void onChanged(Integer integer1) {
                        if (integer1 != 201) {
                            // if it wasn't successful then pop up an appropriate message
                            Toast.makeText(getApplicationContext(), s1, Toast.LENGTH_SHORT).show();
                        } else {
                            // if it was successful, create a user
                            User enteredUser = new User(usernameText, passwordText, emailText, phoneNumberText, s1);
                            userViewModel.create(enteredUser, userCreation);
                            userCreation.getResponseCode().observe(RegistrationPage.this, new Observer<Integer>() {
                                @Override
                                public void onChanged(Integer code) {
                                    if (code != 201) {
                                        // if it wasn't successful then pop up an appropriate message
                                        userCreation.getResponseMsg().observe(RegistrationPage.this, new Observer<String>() {
                                            @Override
                                            public void onChanged(String s) {
                                                Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                                            }
                                        });
                                    } else {
                                        // if it was successful, create a token
                                        TokenRequest req = new TokenRequest(enteredUser.getUsername(), enteredUser.getPassword());
                                        api.createToken(req, tokenCreation);
                                        tokenCreation.getResponseMsg().observe(RegistrationPage.this, new Observer<String>() {
                                            @Override
                                            public void onChanged(String s) {
                                                tokenCreation.getResponseCode().observe(RegistrationPage.this, new Observer<Integer>() {
                                                    @Override
                                                    public void onChanged(Integer integer) {
                                                        if (integer != 201) {
                                                            // if it wasn't successful then pop up an appropriate message
                                                            Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
                                                        } else {
                                                            // add token, userId, privilegeLevel to the sharedPreference
                                                            editor.putString("token", s);
                                                            editor.putString("userId", enteredUser.getId());
                                                            editor.putInt("privilegeLevel", 0);
                                                            editor.apply();
                                                            Toast.makeText(getApplicationContext(), "registration successful", Toast.LENGTH_SHORT).show();
                                                            // move to the homePage after registration.
                                                            Intent intent = new Intent(RegistrationPage.this, HomePage.class);
                                                            startActivity(intent);
                                                        }

                                                    }
                                                });
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

