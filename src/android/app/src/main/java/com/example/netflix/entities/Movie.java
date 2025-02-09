package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.PrimaryKey;

@Entity(foreignKeys = @ForeignKey(
        entity = Image.class,
        parentColumns = "id",
        childColumns = "photoId",
        onDelete = ForeignKey.SET_NULL
), tableName = "movies")
public class Movie {
    @PrimaryKey
    @NonNull
    private String id;
    private String name;
    private String description;
    private String[] actors;
    private int age_limit;
    private String[] creators;
    private String[] categories;
    private String photoId;

    public Movie(@NonNull String id, String name, String description, String[] actors, int age_limit, String[] creators, String[] categories, String photoId) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.actors = actors;
        this.age_limit = age_limit;
        this.creators = creators;
        this.categories = categories;
        this.photoId = photoId;
    }

    @NonNull
    public String getId() {
        return id;
    }

    public void setId(@NonNull String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String[] getActors() {
        return actors;
    }

    public void setActors(String[] actors) {
        this.actors = actors;
    }

    public int getAge_limit() {
        return age_limit;
    }

    public void setAge_limit(int age_limit) {
        this.age_limit = age_limit;
    }

    public String[] getCreators() {
        return creators;
    }

    public void setCreators(String[] creators) {
        this.creators = creators;
    }

    public String[] getCategories() {
        return categories;
    }

    public void setCategories(String[] categories) {
        this.categories = categories;
    }

    public String getPhotoId() {
        return photoId;
    }

    public void setPhotoId(String photoId) {
        this.photoId = photoId;
    }
}