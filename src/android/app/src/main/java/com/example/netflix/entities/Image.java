package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

import com.google.gson.annotations.SerializedName;

import java.io.Serializable;

@Entity(tableName = "images")
public class Image implements Serializable {

    @PrimaryKey
    @NonNull
    @SerializedName("_id")
    private String id = ""; // temporary placeholder, will be updated later

    @ColumnInfo(name = "data", typeAffinity = ColumnInfo.BLOB)
    private byte[] data; // store image as byte array

    private String contentType; // MIME type (e.g., "image/png")

    public Image() {};

    // constructor without id (for new images before MongoDB assigns an ID)
    public Image(byte[] data, String contentType) {
        this.data = data;
        this.contentType = contentType;
    }

    // constructor with id (for when MongoDB assigns an ID)
    public Image(@NonNull String id, byte[] data, String contentType) {
        this.id = id;
        this.data = data;
        this.contentType = contentType;
    }

    // Getters and Setters
    @NonNull
    public String getId() {
        return id;
    }

    public void setId(@NonNull String id) {
        this.id = id;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public String getContentType() {
        return contentType;
    }

    public void setContentType(String contentType) {
        this.contentType = contentType;
    }
}
