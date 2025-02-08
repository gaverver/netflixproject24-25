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
        imagesAPI.insertImage(image, res);
    }

    public LiveData<Image> getImage(String id, WebResponse res) {
        // get image from Room (returns LiveData for automatic updates)
        MutableLiveData<Image> liveData = new MutableLiveData<>();

        executorService.execute(() -> {
            // first, check if the image exists in the local Room database
            Image localImage = imageDao.get(id);
            if (localImage == null) {
                // if not in Room, fetch from the API and insert it
                imagesAPI.getImage(id, liveData, res);
            } else {
                // found in ROOM, no need for API fetch
                liveData.postValue(localImage);
            }
        });

        return liveData;
    }

    public void deleteImage(String id, WebResponse res) {
        imagesAPI.deleteImage(id, res);
    }
}
