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
    List<Movie> getMovies();

    @Query("SELECT * FROM movie WHERE id = :id")
    Movie get(String id);

    @Insert
    void insert(Movie... movies);

    @Update
    void update(Movie... movies);

    @Delete
    void delete(Movie... movies);

    @Query("DELETE FROM movie WHERE id = :movieId")
    void deleteById(String movieId);

}
