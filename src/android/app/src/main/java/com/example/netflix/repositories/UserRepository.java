package com.example.netflix.repositories;

import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.UserAPI;
import com.example.netflix.data.LocalDatabase;
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
        api.getUser(id, webRes, res);
        return res;
    }
}
