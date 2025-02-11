package com.example.netflix.repositories;

import com.example.netflix.WebResponse;
import com.example.netflix.api.UserAPI;
import com.example.netflix.api.VideoAPI;
import com.example.netflix.data.LocalDatabase;

import java.io.File;

public class VideoRepository {

    private VideoAPI api;
    public VideoRepository() {
        this.api = new VideoAPI();
    }

    public void uploadVideo(File videoFile, WebResponse res) {
        api.uploadVideo(videoFile, res);
    }
}
