package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.PrimaryKey;
import androidx.room.TypeConverters;

import com.example.netflix.converters.StringArrayConverter;
import com.example.netflix.converters.StringListConverter;
import com.google.gson.annotations.SerializedName;

import java.io.Serializable;

@TypeConverters({StringListConverter.class, StringArrayConverter.class})
@Entity(tableName = "categories")
public class Category implements Serializable {
    @PrimaryKey
    @NonNull
    private String id = ""; // temporary placeholder, will be updated later
    private String name;
    private Boolean promoted;
    @SerializedName("movieIds")
    private String[] movies;

    public Category() {};

    // constructor without id (for new categories before MongoDB assigns an ID)
    public Category(String name, Boolean promoted, String[] movies) {
        this.name = name;
        this.promoted = promoted;
        this.movies = movies;
    }

    // constructor
    public Category(@NonNull String id, String name, Boolean promoted, String[] movies) {
        this.id = id;
        this.name = name;
        this.promoted = promoted;
        this.movies = movies;
    }

    // getters
    public String getName() {
        return name;
    }

    public Boolean getPromoted() {
        return promoted;
    }

    @NonNull
    public String getId() {
        return id;
    }

    public String[] getMovies() {
        return movies;
    }


    // setters
    public void setName(String name) {
        this.name = name;
    }

    public void setPromoted(Boolean promoted) {
        this.promoted = promoted;
    }

    public void setId(@NonNull String id) {
        this.id = id;
    }

    public void setMovies(String[] movies) {
        this.movies = movies;
    }
}

