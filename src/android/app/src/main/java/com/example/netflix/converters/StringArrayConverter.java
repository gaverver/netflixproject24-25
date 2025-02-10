package com.example.netflix.converters;

import androidx.room.TypeConverter;
import com.google.gson.Gson;

public class StringArrayConverter {
    private static final Gson gson = new Gson();

    @TypeConverter
    public static String[] fromString(String value) {
        if (value == null) {
            return null;
        }
        return gson.fromJson(value, String[].class);
    }

    @TypeConverter
    public static String fromStringArray(String[] array) {
        if (array == null) {
            return null;
        }
        return gson.toJson(array);
    }
}