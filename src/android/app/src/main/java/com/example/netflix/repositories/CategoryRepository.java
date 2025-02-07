package com.example.netflix.repositories;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.CategoryAPI;
import com.example.netflix.api.ImagesAPI;
import com.example.netflix.entities.Category;
import com.example.netflix.entities.Image;
import com.example.netflix.repositories.ImageDao;
import com.example.netflix.data.LocalDatabase;

import java.time.LocalDateTime;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.security.auth.callback.Callback;

public class CategoryRepository {
    private CategoryListData dataList;
    private final CategoryDao dao;
    private final CategoryAPI api;
    private final ExecutorService executorService;

    public CategoryRepository() {
        LocalDatebase db = LocalDatebase.getInstance();
        dao = db.categoryDao();
        dataList = new CategoryListData();
        api = new CategoryAPI(dao, dataList);
        executorService = Executors.newSingleThreadExecutor(); // Background thread
    }

    public void addCategory(Category category, String token, WebResponse res) {
        executorService.execute(() -> {
            api.insertCategory(category, token, res);
        });
    }

    public LiveData<Image> getCategory(String id, WebResponse res) {
        // get image from Room (returns LiveData for automatic updates)
        LiveData<Image> liveData = imageDao.getLiveData(id);

        executorService.execute(() -> {
            // first, check if the image exists in the local Room database
            Image localImage = imageDao.get(id);
            if (localImage == null) {
                // If not in Room, fetch from the API and insert it
                imagesAPI.getImage(id, res);
                // Room LiveData will update automatically when inserted
            }
        });

        return liveData;
    }

    public void deleteCategory(String id, WebResponse res) {
        executorService.execute(() -> {
            imagesAPI.deleteImage(id, res);
        });
    }
}