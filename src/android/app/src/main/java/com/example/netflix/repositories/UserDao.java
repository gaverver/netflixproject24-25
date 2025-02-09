package com.example.netflix.repositories;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;
import androidx.room.Update;

import com.example.netflix.entities.User;

@Dao
public interface UserDao {
    @Query("SELECT * FROM user WHERE id = :id")
    User get(String id);
    @Insert
    void insert(User... users);

    @Update
    void update(User... users);

    @Delete
    void delete(User... users);

}
