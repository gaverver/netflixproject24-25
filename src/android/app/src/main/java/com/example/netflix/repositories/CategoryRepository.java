package com.example.netflix.repositories;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.CategoryAPI;
import com.example.netflix.entities.Category;
import com.example.netflix.repositories.CategoryDao;
import com.example.netflix.data.LocalDatabase;

import java.time.LocalDateTime;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.security.auth.callback.Callback;

public class CategoryRepository {
    private CategoryListData dataList;
    private final CategoryDao dao;
    private final CategoryAPI api;
    private final ExecutorService executorService;

    public CategoryRepository() {
        LocalDatabase db = LocalDatabase.getInstance();
        dao = db.categoryDao();
        dataList = new CategoryListData();
        api = new CategoryAPI(dao, dataList);
        executorService = Executors.newSingleThreadExecutor(); // new thread for each task
    }

    class CategoryListData extends MutableLiveData<List<Category>> {
        // constructor
        public CategoryListData() {
            super();
            setValue(new LinkedList<Category>());
        }

        // this method will be called every time the dataList changes in the api to update the dataList
        @Override
        protected void onActive() {
            super.onActive();

            // run it on a background thread
            executorService.execute(() -> {
                dataList.postValue(dao.getAll());
            });
        }
    }

    public void addCategory(Category category, String token, WebResponse res) {
        api.insertCategory(category, token, res);
    }

    public LiveData<Category> getCategory(String id, WebResponse res) {
        // get category from Room (returns LiveData for automatic updates)
        MutableLiveData<Category> liveData = new MutableLiveData<>();

        // run in background thread because there are ROOM accesses
        executorService.execute(() -> {
            // first, check if the category exists in the local Room database
            Category localCategory = dao.get(id);
            if (localCategory == null) {
                // if not in Room, fetch from the API and insert it
                api.getCategory(id, liveData, res);
            } else {
                // already in ROOM, no need for fetching from server
                res.setResponseCode(200);
                res.setResponseMsg("found category");
                liveData.postValue(localCategory);
            }
        });

        return liveData;
    }

    public void updateCategory(Category category, String token, WebResponse res) {
        api.updateCategory(category, token, res);
    }

    public void deleteCategory(String id, String token, WebResponse res) {
        api.deleteCategory(id, token, res);
    }

    // method to get all existing categories in the ROOM database
    public LiveData<List<Category>> getAll() {
        return dataList;
    }

    // method to reload all categories (gets all categories from the server into the ROOM)
    public void reload(WebResponse res) {
        api.reload(res);
    }
}