package com.example.netflix.api;

import android.content.Context;
import android.content.SharedPreferences;

import com.example.netflix.MyApplication;
import com.example.netflix.RetrofitClient;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;
import com.example.netflix.entities.Movie;
import com.example.netflix.repositories.MovieDao;

import java.io.File;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.RequestBody;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;

public class VideoAPI {

    private String token;
    private String userId;

    private Retrofit retrofit;
    private VideoWebServiceAPI videoWebServiceAPI;
    private void updateTokens() {
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        this.token = sharedPreferences.getString("token", null);
        this.userId = sharedPreferences.getString("userId", null);
    }

    public VideoAPI() {

        retrofit = RetrofitClient.getInstance().getRetrofit();

        videoWebServiceAPI = retrofit.create(VideoWebServiceAPI.class);

        updateTokens();

    }

    public void uploadVideo(File videoFile, WebResponse res) {
        updateTokens();
        RequestBody requestFile = RequestBody.create(MediaType.parse("video/mp4"), videoFile);
        MultipartBody.Part body = MultipartBody.Part.createFormData("video", videoFile.getName(), requestFile);
        Call<ResponseBody> call = videoWebServiceAPI.uploadVideo(token, body);
        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                if (response.isSuccessful()) {
                    // get the created userID from the location header
                    String locationHeader = response.headers().get("Location");
                    String videoId = "";
                    if (locationHeader != null) {
                        // extract the id from the location header(it is /users/:id)
                        Pattern pattern = Pattern.compile("[^/]+$");
                        Matcher matcher = pattern.matcher(locationHeader);
                        if (matcher.find()) {
                            videoId = matcher.group();
                        }
                    }
                    // set the response status to the returned response status - the operation was successful
                    res.setResponseCode(response.code());
                    res.setResponseMsg(videoId);
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Creation of movie failed" + t.getMessage());
            }
        });
    }

}
