package com.example.netflix.viewmodels;

import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.entities.User;
import com.example.netflix.repositories.UserRepository;

public class UserViewModel {
    private UserRepository repository;
    public UserViewModel() {
        repository = new UserRepository();
    }

    public void create(User user, WebResponse res) {
        repository.create(user, res);
    }

    public MutableLiveData<User> get(String id, WebResponse res) {
        return repository.get(id, res);
    }
}
