package com.example.netflix.api;

import android.util.Log;

import com.example.netflix.FromToken;
import com.example.netflix.RetrofitClient;
import com.example.netflix.TokenRequest;
import com.example.netflix.TokenResponse;
import com.example.netflix.Utils;
import com.example.netflix.WebResponse;


import okhttp3.MediaType;
import okhttp3.RequestBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;

public class TokenAPI {
    Retrofit retrofit;
    TokenWebServiceAPI tokenWebServiceAPI;

    public TokenAPI() {
        retrofit = RetrofitClient.getInstance().getRetrofit();
        tokenWebServiceAPI = retrofit.create(TokenWebServiceAPI.class);
    }

    public void createToken(TokenRequest token, WebResponse res) {
        // create token with given info
        Call<TokenResponse> call = tokenWebServiceAPI.createToken(token);
        call.enqueue(new Callback<TokenResponse>() {
            @Override
            public void onResponse(Call<TokenResponse> call, Response<TokenResponse> response) {
                if (response.isSuccessful() && response.body() != null) {
                    // update the web response for future usages with the response
                    res.setResponseCode(response.code());
                    res.setResponseMsg(response.body().getToken());
                } else {
                    // handle errors
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<TokenResponse> call, Throwable t) {
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void getIdFromToken(String token, WebResponse res) {
        // get the id from the token
        Call<FromToken> call = tokenWebServiceAPI.getIdFromToken(token);
        call.enqueue(new Callback<FromToken>() {
            @Override
            public void onResponse(Call<FromToken> call, Response<FromToken> response) {
                if (response.isSuccessful() && response.body() != null) {
                    // update the web response for future usages with the response
                    res.setResponseCode(response.code());
                    res.setResponseMsg(response.body().getuserId());
                } else {
                    // handle errors
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(Call<FromToken> call, Throwable t) {
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }
}
