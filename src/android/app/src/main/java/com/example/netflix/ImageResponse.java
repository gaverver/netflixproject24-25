package com.example.netflix;

import com.google.gson.annotations.SerializedName;

import java.util.List;

public class ImageResponse {
    @SerializedName("_id")
    private String id;

    @SerializedName("data")
    private BufferData bufferData;

    public static class BufferData {
        @SerializedName("type")
        private String type;

        @SerializedName("data")
        private List<Integer> data;

        public String getType() {
            return type;
        }

        public void setType(String type) {
            this.type = type;
        }

        public List<Integer> getData() {
            return data;
        }

        public void setData(List<Integer> data) {
            this.data = data;
        }
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public BufferData getBufferData() {
        return bufferData;
    }

    public void setBufferData(BufferData bufferData) {
        this.bufferData = bufferData;
    }

    // Helper method to convert List<Integer> to byte[]
    public byte[] getByteArray() {
        if (bufferData != null && bufferData.getData() != null) {
            List<Integer> intList = bufferData.getData();
            byte[] byteArray = new byte[intList.size()];
            for (int i = 0; i < intList.size(); i++) {
                byteArray[i] = intList.get(i).byteValue();
            }
            return byteArray;
        }
        return null;
    }
}