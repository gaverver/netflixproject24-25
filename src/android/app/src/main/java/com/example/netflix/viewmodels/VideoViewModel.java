package com.example.netflix.viewmodels;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.netflix.WebResponse;
import com.example.netflix.repositories.VideoRepository;

import java.io.File;

public class VideoViewModel extends ViewModel {
    private final VideoRepository videoRepository;

    public VideoViewModel() {
        videoRepository = new VideoRepository();
    }



    public void uploadVideo(File videoFile, WebResponse res) {
        videoRepository.uploadVideo(videoFile, res);
    }
}
