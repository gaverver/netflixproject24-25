package com.example.netflix.repositories;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Update;

import com.example.netflix.entities.Movie;

import java.util.List;

@Dao
public interface MovieDao {
    @Query("SELECT * FROM movie")
    List<Movie> index();

    @Query("SELECT * FROM movie WHERE id = :id")
    Movie get(int id);

    @Insert
    void insert(Movie... movies);

    @Update
    void update(Movie... movies);

    @Delete
    void delete(Movie... movies);

}
