package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity(tableName = "images")
public class Image {

    @PrimaryKey
    @NonNull
    private String id;

    // convert to BLOB object in the ROOM
    @ColumnInfo(name = "data", typeAffinity = ColumnInfo.BLOB)
    private byte[] data; // Store image as byte array

    private String contentType; // MIME type (e.g., "image/png")

    // Constructor
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
