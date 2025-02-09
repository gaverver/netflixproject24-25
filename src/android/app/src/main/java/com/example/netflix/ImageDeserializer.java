package com.example.netflix;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonDeserializationContext;
import com.google.gson.JsonDeserializer;

import java.lang.reflect.Type;
import java.util.List;

import com.example.netflix.entities.Image;

public class ImageDeserializer implements JsonDeserializer<Image> {
    @Override
    public Image deserialize(JsonElement json, Type typeOfT, JsonDeserializationContext context) {
        JsonObject jsonObject = json.getAsJsonObject();

        // Get the contentType and image data
        String contentType = jsonObject.get("contentType").getAsString();
        JsonElement dataElement = jsonObject.get("data");

        // Convert the List<Integer> data to byte[]
        List<Integer> dataList = context.deserialize(dataElement, List.class);
        byte[] imageData = new byte[dataList.size()];

        // Convert each Integer to byte and store it in the byte[] array
        for (int i = 0; i < dataList.size(); i++) {
            imageData[i] = dataList.get(i).byteValue(); // Convert Integer to byte
        }

        // Create and return the Image object
        return new Image(imageData, contentType);
    }
}
