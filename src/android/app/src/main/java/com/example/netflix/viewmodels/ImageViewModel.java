package com.example.netflix.viewmodels;

import android.util.Log;

import androidx.lifecycle.LiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.entities.Image;
import com.example.netflix.repositories.ImageRepository;

public class ImageViewModel {
    private final ImageRepository repository;
     public ImageViewModel() {
         repository = new ImageRepository();
     }

     public void create(Image image, WebResponse res) {
         repository.uploadImage(image, res);
     }

     public LiveData<Image> get(String id, WebResponse res) {
         return repository.getImage(id, res);
     }

     public void delete(String id, WebResponse res) {
         repository.deleteImage(id, res);
     }
}
