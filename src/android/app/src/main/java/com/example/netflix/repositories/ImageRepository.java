package com.example.netflix.repositories;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.api.ImagesAPI;
import com.example.netflix.entities.Image;
import com.example.netflix.repositories.ImageDao;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.security.auth.callback.Callback;

public class ImageRepository {
    private final ImageDao imageDao;
    private final ImagesAPI imagesAPI;
    private final ExecutorService executorService;

    public ImageRepository(ImageDao imageDao) {
        this.imageDao = imageDao;
        this.imagesAPI = new ImagesAPI(imageDao);
        this.executorService = Executors.newSingleThreadExecutor(); // Background thread
    }

    public void uploadImage(Image image, WebResponse res) {
        executorService.execute(() -> {
            imagesAPI.insertImage(image, res);
        });
    }

    public LiveData<Image> getImage(String id, WebResponse res) {
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

    public void deleteImage(String id, WebResponse res) {
        executorService.execute(() -> {
            imagesAPI.deleteImage(id, res);
        });
    }
}
