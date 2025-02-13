package com.example.netflix.repositories;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Update;

import com.example.netflix.entities.Category;

import java.util.List;

@Dao
public interface CategoryDao {
    // returning all existing categories in the ROOM database
    @Query("SELECT * FROM categories")
    List<Category> getAll();
    // returning a specific Category from the ROOM
    @Query("SELECT * FROM categories WHERE id = :id")
    Category get(String id);
    // insert a specific category to the ROOM
    @Insert
    void insert(Category... categories);

    // update a specific category in the ROOM
    @Update
    void update(Category... categories);

    // delete a specific category from the ROOM
    @Delete
    void delete(Category... categories);

    // drop this table from the ROOM (clear all the data in it)
    @Query("DELETE FROM categories")
    void clear();
}
