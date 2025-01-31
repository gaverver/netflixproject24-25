package com.example.netflix;

import androidx.annotation.NonNull;
import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity
public class User {
   @PrimaryKey
   @NonNull
   private String id;
   private String username;
   private String password;

   private String email;
   private String phoneNumber;
   private int privilegeLevel;
   private String picture;

    public User(@NonNull String id, String username, String password, String email, String phoneNumber, int privilegeLevel, String picture) {
       this.id = id;
       this.username = username;
       this.password = password;
       this.email = email;
       this.phoneNumber = phoneNumber;
       this.privilegeLevel = privilegeLevel;
       this.picture = picture;
    }

   public String getUsername() {
      return username;
   }

   public String getEmail() {
      return email;
   }

   public String getPhoneNumber() {
      return phoneNumber;
   }

   public int getPrivilegeLevel() {
      return privilegeLevel;
   }

   public String getPicture() {
      return picture;
   }

   public void setEmail(String email) {
      this.email = email;
   }

   public void setPassword(String password) {
      this.password = password;
   }

   @NonNull
   public String getId() {
      return id;
   }

   public void setPhoneNumber(String phoneNumber) {
      this.phoneNumber = phoneNumber;
   }

   public void setPicture(String picture) {
      this.picture = picture;
   }

   public void setPrivilegeLevel(int privilegeLevel) {
      this.privilegeLevel = privilegeLevel;
   }

   public void setUsername(String username) {
      this.username = username;
   }

   public void setId(@NonNull String id) {
      this.id = id;
   }
}

