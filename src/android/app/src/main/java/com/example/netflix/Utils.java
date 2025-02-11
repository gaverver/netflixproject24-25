package com.example.netflix;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.ImageView;
import android.util.Log;

import org.json.JSONObject;

import retrofit2.Response;

public class Utils {
    public static void handleError(Response<?> response, WebResponse res) {
        int responseCode = response.code();
        String errorMessage = "Unknown error";
        try {
            if (response.errorBody() != null) {
                Log.d("SomeTag", "im here with error: " + response.errorBody().string());
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

    public static void setImageFromByteArray(ImageView imageView, byte[] imageData) {
        // using Bitmap to insert the image to the imageView
        if (imageData != null && imageData.length > 0) {
            Bitmap bitmap = BitmapFactory.decodeByteArray(imageData, 0, imageData.length);
            imageView.setImageBitmap(bitmap);
        }
        // else, image cannot be inserted to the ImageView
    }
}
