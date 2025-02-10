package com.example.netflix.viewmodels;

import android.content.Context;
import android.content.SharedPreferences;

import androidx.lifecycle.LiveData;

import com.example.netflix.MyApplication;
import com.example.netflix.WebResponse;
import com.example.netflix.entities.Category;
import com.example.netflix.repositories.CategoryRepository;

import java.util.List;

public class CategoryViewModel {
    private final CategoryRepository repository;
    // a field for the user's token
    private String token = "";
    public CategoryViewModel() {
        repository = new CategoryRepository();
    }

    // a function to get the token from the sharedPreferences of the user that currently using the device
    // this function will be called every time an operation that needs token will happen
    private void updateTokens() {
        SharedPreferences sharedPreferences = MyApplication.getAppContext().getSharedPreferences("MyPrefs", Context.MODE_PRIVATE);
        this.token = sharedPreferences.getString("token", null);
    }

    public void create(Category category, WebResponse res) {
        updateTokens();
        repository.addCategory(category, token, res);
    }

    public LiveData<Category> get(String id, WebResponse res) {
        return repository.getCategory(id, res);
    }

    public void update(Category category, WebResponse res) {
        updateTokens();
        repository.updateCategory(category, token, res);
    }

    public void delete(String id, WebResponse res) {
        updateTokens();
        repository.deleteCategory(id, token, res);
    }

    public LiveData<List<Category>> getAll() {
        return repository.getAll();
    }

    public void reload(WebResponse res) {
        repository.reload(res);
    }
}
