package com.example.netflix;

import androidx.lifecycle.MutableLiveData;

public class WebResponse {
    private final MutableLiveData<Integer> responseCode;
    private final MutableLiveData<String> ResponseMsg;

    public WebResponse() {
        this.responseCode = new MutableLiveData<>();
        this.ResponseMsg = new MutableLiveData<>();
    }
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
