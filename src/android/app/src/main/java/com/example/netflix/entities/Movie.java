package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.PrimaryKey;
import androidx.room.TypeConverters;
import com.example.netflix.converters.StringListConverter;
import com.google.gson.annotations.SerializedName;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;


@Entity(tableName = "movies")
public class Movie implements Serializable {
    @PrimaryKey
    @NonNull
    @ColumnInfo(name = "id")
    @SerializedName("_id")
    private String id;
    private String name;
    private String description;
    private List<String> actors;
    private String published;
    private int age_limit;
    private List<String> creators;
    private List<String> categories;
    @SerializedName("photo")
    private String photoId;
    private String video;
    public Movie(@NonNull String id, String name, String description, List<String> actors, String published,int age_limit, List<String> creators, List<String> categories, String photoId, String video) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.actors = new ArrayList<>(actors);
        this.published = published;
        this.age_limit = age_limit;
        this.creators = new ArrayList<>(creators);
        this.categories = new ArrayList<>(categories);
        this.photoId = photoId;
        this.video = video;
    }

    public Movie() {
        this.id = "temp";
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

    public List<String> getActors() {
        return actors;
    }

    public void setActors(List<String> actors) {
        this.actors = actors;
    }

    public String getPublished() {
        return published;
    }

    public void setPublished(String published) {
        this.published = published;
    }

    public int getAge_limit() {
        return age_limit;
    }

    public void setAge_limit(int age_limit) {
        this.age_limit = age_limit;
    }

    public List<String> getCreators() {
        return creators;
    }

    public void setCreators(List<String> creators) {
        this.creators = creators;
    }

    public List<String> getCategories() {
        return categories;
    }

    public void setCategories(List<String> categories) {
        this.categories = categories;
    }

    public String getPhotoId() {
        return photoId;
    }

    public void setPhotoId(String photoId) {
        this.photoId = photoId;
    }

    public String getVideo() {
        return video;
    }

    public void setVideo(String video) {
        this.video = video;
    }
}