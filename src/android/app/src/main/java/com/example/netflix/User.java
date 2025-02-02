package com.example.netflix;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

import java.util.ArrayList;
import java.util.List;

@Entity
public class User {
   @PrimaryKey
   @NonNull
   private String id;
   private String username;
   private String picture;

    public User(@NonNull String id, String username, String picture) {
       this.id = id;
       this.username = username;
       this.picture = picture;
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

