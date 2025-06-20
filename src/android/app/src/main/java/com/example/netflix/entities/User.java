package com.example.netflix.entities;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.Ignore;
import androidx.room.PrimaryKey;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

@Entity(tableName="users")
public class User {
   @PrimaryKey
   @NonNull
   @SerializedName("_id")
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
   @Ignore
   private String password;
   private String picture;



  
   public User(String id, String username, String picture) {
      this.id = id;
      this.username = username;
      this.picture = picture;
   }
    public User(String username, String password, String email, String phoneNumber, String picture) {
       // temp value that will be changed afterwards.
       this.id = "temp";
       this.username = username;
       this.picture = picture;
       this.password = password;
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

   public int getPrivilegeLevel() {
      return privilegeLevel;
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

   public String getPassword() {
      return password;
   }
}

