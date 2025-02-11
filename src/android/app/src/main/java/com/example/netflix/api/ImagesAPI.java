package com.example.netflix.api;

import android.util.Log;

import androidx.annotation.NonNull;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.Utils;
import com.example.netflix.entities.Image;
import com.example.netflix.repositories.ImageDao;
import com.example.netflix.ImageRetrofitClient;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import okhttp3.MediaType;
import okhttp3.RequestBody;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.Call;
import retrofit2.Callback;

/*
 * IMPORTANT NOTE:
 * this class will handle requests s.a create, get and delete,
 * and will ALWAYS! check in the server in get commands.
 * (even if the image is already in the ROOM DB)
 * and so in the ImageRepository class, get method should check for the image in the ROOM first!
*/

public class ImagesAPI {
    private final ImageDao dao;
    Retrofit retrofit;
    ImageWebServiceAPI imageWebServiceAPI;

    public ImagesAPI(ImageDao dao) {
        this.dao = dao;
        // using the retrofit for images
        retrofit = ImageRetrofitClient.getInstance().getRetrofit();
        imageWebServiceAPI = retrofit.create(ImageWebServiceAPI.class);
    }

    public void insertImage(Image image, WebResponse res) {
        RequestBody requestBody = RequestBody.create(MediaType.parse(image.getContentType()), image.getData());
        Call<Void> call = imageWebServiceAPI.uploadImage(requestBody, image.getContentType());
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NonNull Call<Void> call, @NonNull Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // gets the id from the headers
                        String locationHeaders = response.headers().get("Location");
                        if (locationHeaders != null) {
                            // using REGEX to extract the id
                            Pattern pattern = Pattern.compile("/images/([a-fA-F0-9]+)");
                            Matcher matcher = pattern.matcher(locationHeaders);
                            if (matcher.find()) {
                                // set the id that mongoDB gave to it
                                image.setId(matcher.group(1));
                            }
                            dao.insert(image);
                        }
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Image Uploaded");
                    }).start();

                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(@NonNull Call<Void> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void getImage(String id, MutableLiveData<Image> data, WebResponse res) {
        Call<Image> call = imageWebServiceAPI.getImage(id);
        call.enqueue(new Callback<Image>() {
            @Override
            public void onResponse(@NonNull Call<Image> call, @NonNull Response<Image> response) {
                if (response.isSuccessful() && response.body() != null) {
                    Log.d("ImagesAPI", "Image fetched successfully: " + response.body().toString());
                    new Thread(() -> {
                        Image image = response.body();
                        image.setId(id);
                        dao.insert(image);
                        data.postValue(image);
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Ok");
                    }).start();
                } else {
                    Log.d("ImagesAPI", "Failed to fetch image: " + response.message());
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(@NonNull Call<Image> call, @NonNull Throwable t) {
                Log.d("ImagesAPI", "Error fetching image: " + t.getMessage());
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }


    public void deleteImage(String id, WebResponse res) {
        Call<Void> call = imageWebServiceAPI.deleteImage(id);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NonNull Call<Void> call, @NonNull Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // find the image in the ROOM
                        Image image = dao.get(id);
                        // if image found
                        if (image != null) {
                            // delete the image from the Room database
                            dao.delete(image);
                        }
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Image Deleted");
                    }).start();

                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(@NonNull Call<Void> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }
}
