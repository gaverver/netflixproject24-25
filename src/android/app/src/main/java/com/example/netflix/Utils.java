package com.example.netflix;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.transition.Transition;
import android.util.Log;
import android.widget.ImageView;

import androidx.annotation.Nullable;
import androidx.annotation.NonNull;

import com.bumptech.glide.Glide;
import com.bumptech.glide.request.target.CustomTarget;

import org.json.JSONObject;

import retrofit2.Response;

public class Utils {
    public static void handleError(Response<?> response, WebResponse res) {
        int responseCode = response.code();
        String errorMessage = "Unknown error";
        try {
            if (response.errorBody() != null) {
                // get the error that is written in the response
                String errorBody = response.errorBody().string();
                JSONObject jsonObject = new JSONObject(errorBody);
                errorMessage = jsonObject.optString("error", errorMessage);
            }
        } catch (Exception e) {
            // return response code that tells that there was error in server
            responseCode = 500;
            errorMessage = "Internal Server Error";
        }
        // update the values in the webResponse Object
        res.setResponseCode(responseCode);
        res.setResponseMsg(errorMessage);
    }

    // helper function to help upload images into imageView object
    public static void setImageFromByteArray(Context context, ImageView imageView, byte[] imageData) {
        if (imageData != null && imageData.length > 0) {
            Glide.with(context)
                    .load(imageData)
                    .placeholder(android.R.drawable.ic_menu_gallery) // Built-in Android placeholder
                    .error(android.R.drawable.ic_dialog_alert) // Built-in Android error icon
                    .into(new CustomTarget<Drawable>() {
                        @Override
                        public void onResourceReady(@NonNull Drawable resource, @Nullable com.bumptech.glide.request.transition.Transition<? super Drawable> transition) {
                            imageView.setImageDrawable(resource);  // Set the loaded image to the ImageView
                        }

                        @Override
                        public void onLoadFailed(@Nullable Drawable errorDrawable) {
                            imageView.setImageDrawable(errorDrawable);
                            Log.e("setImageFromByteArray", "Failed to load image");
                        }

                        @Override
                        public void onLoadCleared(@Nullable Drawable placeholder) {
                            imageView.setImageDrawable(placeholder);
                        }
                    });
        } else {
            Log.e("setImageFromByteArray", "Image data is null or empty");
            imageView.setImageResource(android.R.drawable.ic_dialog_alert); // Built-in Android error icon
        }
    }
}
