package com.example.netflix.api;

import com.example.netflix.repositories.ImageDao;
import com.example.netflix.api.ImageWebServiceAPI;
import
import retrofit2.Retrofit;

public class ImagesAPI {
    private ImageDao dao;
    Retrofit retrofit;
    ImageWebServiceAPI imageWebServiceAPI;

    public ImagesAPI(ImageDao dao) {
        this.dao = dao;
        retrofit = ;
        imageWebServiceAPI = retrofit.create(ImageWebServiceAPI.class);
}










public class PostAPI {
2 private MutableLiveData<List<Post>> postListData;
3 private PostDao dao;
4 Retrofit retrofit;
5 WebServiceAPI webServiceAPI;
6
        7 public PostAPI(MutableLiveData<List<Post>> postListData, PostDao dao) {
        8 this.postListData = postListData;
        9 this.dao = dao;
        10
        11 retrofit = new Retrofit.Builder()
        12 .baseUrl(MyApplication.context.getString(R.string.BaseUrl))
        13 .addConverterFactory(GsonConverterFactory.create())
        14 .build();
        15 webServiceAPI = retrofit.create(WebServiceAPI.class);
        16 }
17
        18 public void get() {
        19 Call<List<Post>> call = webServiceAPI.getPosts();
        20 call.enqueue(new Callback<List<Post>>() {
21 @Override
22 public void onResponse(Call<List<Post>> call, Response<List<Post>> response) {
                23
                24 new Thread(() -> {
                    25 dao.clear();
                    26 dao.insertList(response.body());
                    27 postListData.postValue(dao.get());
                    28 }).start();
                29 }
30
        31 @Override
32 public void onFailure(Call<List<Post>> call, Throwable t) {}
33 });
        34 }
35}
22
Dem
