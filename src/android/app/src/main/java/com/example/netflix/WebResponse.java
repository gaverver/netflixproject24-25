package com.example.netflix;

import androidx.lifecycle.MutableLiveData;

public class WebResponse {
    private MutableLiveData<Integer> responseCode;
    private MutableLiveData<String> ResponseMsg;

    public void setResponseCode(int responseCode) {
        this.responseCode.postValue(responseCode);
    }

    public void setResponseMsg(String responseMsg) {
        this.ResponseMsg.postValue(responseMsg);
    }

    public MutableLiveData<Integer> getResponseCode() {
        return responseCode;
    }

    public MutableLiveData<String> getResponseMsg() {
        return ResponseMsg;
    }
}
