package com.example.netflix.tokens;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Path;
public interface TokenWebServiceAPI {
    @GET("/api/tokens/{id}")
    Call<String> getIdFromToken(@Path("id") String id);
    @POST("/api/tokens")
    Call<String> createToken(@Body TokenRequest token);
}
