package com.example.netflix.repositories;

import androidx.lifecycle.LiveData;
import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

import com.example.netflix.entities.Image;

@Dao
public interface ImageDao {
    // doing get query
    @Query("SELECT * FROM images WHERE id = :id")
    Image get(String id);

    // doing get query but returning a live data
    @Query("SELECT * FROM images WHERE id = :id")
    LiveData<Image> getLiveData(String id);

    // doing post/insert query
    @Insert
    void insert(Image... images);

    // doing delete query
    @Delete
    void delete(Image... images);

    // note: no update method because we don't have any update method for images in our js server
}
