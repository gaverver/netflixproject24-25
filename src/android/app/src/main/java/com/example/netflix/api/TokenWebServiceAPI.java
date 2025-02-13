package com.example.netflix.api;
import com.example.netflix.FromToken;
import com.example.netflix.TokenRequest;
import com.example.netflix.TokenResponse;
import okhttp3.RequestBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Path;
public interface TokenWebServiceAPI {
    @GET("/api/tokens/{token}")
    Call<FromToken> getIdFromToken(@Path("token") String token);
    @POST("/api/tokens")
    Call<TokenResponse> createToken(@Body TokenRequest token);
}
