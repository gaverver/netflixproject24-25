package com.example.netflix.api;

import android.util.Log;

import androidx.lifecycle.MutableLiveData;

import com.example.netflix.RetrofitClient;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;
import com.example.netflix.entities.User;
import com.example.netflix.repositories.UserDao;


import java.util.regex.Matcher;
import java.util.regex.Pattern;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;

public class UserAPI {
    private UserDao dao;
    Retrofit retrofit;
    UserWebServiceAPI userWebServiceAPI;
    public UserAPI(UserDao dao) {
        this.dao = dao;
        // get retrofit and initialize userWebServiceAPI as needed.
        retrofit = RetrofitClient.getInstance().getRetrofit();
        userWebServiceAPI = retrofit.create(UserWebServiceAPI.class);
    }



    public void create(User user, WebResponse res) {
        Log.d("hello", "f" + user.getPassword());
        // create an api call to the web server for user creation
        Call<Void> call = userWebServiceAPI.createUser(user);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> call, Response<Void> response) {
                if (response.isSuccessful()) {
                    // get the created userID from the location header
                    String locationHeader = response.headers().get("Location");
                    if (locationHeader != null) {
                        // extract the id from the location header(it is /users/:id)
                        Pattern pattern = Pattern.compile("/users/([a-fA-F0-9]+)");
                        Matcher matcher = pattern.matcher(locationHeader);
                        if (matcher.find()) {
                            user.setId(matcher.group(1));
                        }
                    }
                    new Thread(() -> {
                        // insert the created user to the room
                        dao.insert(user);
                        // set the response status to the returned response status - the operation was successful
                        res.setResponseCode(response.code());
                        res.setResponseMsg("User Created");
                    }).start();

                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<Void> call, Throwable t) {

                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Registration Failed" + t.getMessage());
            }
        });
    }

    public void getUser(String id, WebResponse res, MutableLiveData<User> user) {
        // create an api call to the web server for getUserById
        Call<User> call = userWebServiceAPI.getUser(id);
        call.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        // insert the created user to the room
                        dao.insert(response.body());
                        // update the mutable live data to the given user
                        user.postValue(response.body());
                        // set the response status to the returned response status - the operation was successful
                        res.setResponseCode(response.code());
                        res.setResponseMsg("ok");
                    }).start();

                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                // return response code that tells that there was error in server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }
}
