package com.example.netflix.data;

import android.content.Context;

import com.example.netflix.converters.StringArrayConverter;
import com.example.netflix.entities.Image;

import androidx.room.Database;
import androidx.room.Room;
import androidx.room.RoomDatabase;
import androidx.room.TypeConverters;

import com.example.netflix.converters.StringListConverter;
import com.example.netflix.entities.Category;
import com.example.netflix.entities.Movie;
import com.example.netflix.entities.User;
import com.example.netflix.repositories.CategoryDao;
import com.example.netflix.repositories.ImageDao;
import com.example.netflix.repositories.MovieDao;
import com.example.netflix.repositories.UserDao;

@TypeConverters({StringListConverter.class, StringArrayConverter.class})
@Database(entities = {User.class, Movie.class, Image.class, Category.class}, version=2)
public abstract class LocalDatabase extends RoomDatabase {
    public abstract UserDao userDao();
    public abstract MovieDao movieDao();
    public abstract CategoryDao categoryDao();
    public abstract ImageDao imageDao();

    // Holder class for lazy initialization
    private static class Holder {
        private static LocalDatabase INSTANCE;
    }

    public static void initInstance(Context context) {
        // Initialize the database with context (should be called once)
        if (Holder.INSTANCE == null) {
            Holder.INSTANCE = Room.databaseBuilder(
                    context.getApplicationContext(),
                    LocalDatabase.class,
                    "local_database"
            ).build();
        }
    }

    public static LocalDatabase getInstance() {
        // throw an exception if db isn't initialized, as it should be in the start.
        if (Holder.INSTANCE == null) {
            throw new IllegalStateException("Database not initialized.");
        }
        return Holder.INSTANCE;
    }

}
