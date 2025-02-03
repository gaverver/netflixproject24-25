package com.example.netflix;

public class WebResponse {
    private int responseCode;
    private String ResponseMsg;

    public void setResponseCode(int responseCode) {
        this.responseCode = responseCode;
    }

    public void setResponseMsg(String responseMsg) {
        ResponseMsg = responseMsg;
    }

    public int getResponseCode() {
        return responseCode;
    }

    public String getResponseMsg() {
        return ResponseMsg;
    }
}