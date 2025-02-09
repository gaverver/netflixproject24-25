package com.example.netflix.viewmodels;

import androidx.lifecycle.LiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.entities.Category;
import com.example.netflix.repositories.CategoryRepository;

import java.util.List;

public class CategoryViewModel {
    private final CategoryRepository repository;
    public CategoryViewModel() {
        repository = new CategoryRepository();
    }

    public void create(Category category, String token, WebResponse res) {
        repository.addCategory(category, token, res);
    }

    public LiveData<Category> get(String id, WebResponse res) {
        return repository.getCategory(id, res);
    }

    public void update(Category category, String token, WebResponse res) {
        repository.updateCategory(category, token, res);
    }

    public void delete(String id, String token, WebResponse res) {
        repository.deleteCategory(id, token, res);
    }

    public LiveData<List<Category>> getAll() {
        return repository.getAll();
    }

    public void reload(WebResponse res) {
        repository.reload(res);
    }
}
