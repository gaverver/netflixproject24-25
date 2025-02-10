package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;

import java.util.ArrayList;
import java.util.List;

@Entity(foreignKeys = @ForeignKey(
        entity = Image.class,
        parentColumns = "id",
        childColumns = "picture",
        onDelete = ForeignKey.SET_NULL
))
public class User {
   @PrimaryKey
   @NonNull
   private String id;
   private String username;
   @Ignore
   private String email;
   @Ignore
   private String phoneNumber;
   @Ignore
   private int privilegeLevel;
   @Ignore
   private List<String> movies_watched;
   private String picture;

   public User() {

   }
   public User(String id, String username, String picture) {
      this.id = id;
      this.username = username;
      this.picture = picture;
   }
    public User(String username, String picture, String email, String phoneNumber) {
       // temp value that will be changed afterwards.
       this.id = "temp";
       this.username = username;
       this.picture = picture;
       this.email = email;
       this.phoneNumber = phoneNumber;
       this.privilegeLevel = 0;
       this.movies_watched = new ArrayList<>();
    }

   public String getUsername() {
      return username;
   }

   public String getPicture() {
      return picture;
   }



   @NonNull
   public String getId() {
      return id;
   }


   public void setPicture(String picture) {
      this.picture = picture;
   }


   public void setUsername(String username) {
      this.username = username;
   }

   public void setId(@NonNull String id) {
      this.id = id;
   }
}

