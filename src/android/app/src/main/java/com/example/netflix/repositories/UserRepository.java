package com.example.netflix.repositories;

import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.UserAPI;
import com.example.netflix.data.LocalDatabase;
import com.example.netflix.entities.Image;
import com.example.netflix.entities.User;

public class UserRepository {
    private UserDao dao;
    private UserAPI api;
    public UserRepository() {
        LocalDatabase db = LocalDatabase.getInstance();
        this.dao = db.userDao();
        this.api = new UserAPI(this.dao);
    }

    public void create(User user, WebResponse webRes) {
        api.create(user, webRes);
    }

    public MutableLiveData<User> get(String id, WebResponse webRes) {
        MutableLiveData<User> res = new MutableLiveData<>();
        new Thread(() -> {

            // first, check if the image exists in the local Room database
            User localUser = dao.get(id);

            if (localUser == null) {

                // if not in Room, fetch from the API and insert it
                api.getUser(id, webRes, res);

            } else {
                // found in ROOM, no need for API fetch
                res.postValue(localUser);
                webRes.setResponseCode(200);
                webRes.setResponseMsg("ok");
            }

        }).start();
        return res;
    }
}
