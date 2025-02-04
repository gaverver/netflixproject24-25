package com.example.netflix.api;

import androidx.annotation.NonNull;
import androidx.lifecycle.MutableLiveData;

import com.example.netflix.WebResponse;
import com.example.netflix.Utils;
import com.example.netflix.entities.Category;
import com.example.netflix.repositories.CategoryDao;
import com.example.netflix.RetrofitClient;

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import okhttp3.MediaType;
import okhttp3.RequestBody;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.Call;
import retrofit2.Callback;

/*
 * IMPORTANT NOTE:
 * this class will handle requests s.a create, get, getAll, update and delete,
 * and will ALWAYS! check in the server in get commands.
 * (even if the category is already in the ROOM DB)
 * and so in the CategoryRepository class, get method should check for the category in the ROOM first!
 */

public class CategoryAPI {
    private MutableLiveData<List<Category>> categoryListData;
    private final CategoryDao dao;
    Retrofit retrofit;
    CategoryWebServiceAPI categoryWebServiceAPI;

    public CategoryAPI(CategoryDao dao, MutableLiveData<List<Category>> categoryListData) {
        this.dao = dao;
        this.categoryListData = categoryListData;
        // using regular retrofit
        retrofit = RetrofitClient.getInstance().getRetrofit();
        categoryWebServiceAPI = retrofit.create(CategoryWebServiceAPI.class);
    }

    public void insertCategory(Category category, String token, WebResponse res) {
        Call<Void> call = categoryWebServiceAPI.insertCategory(category, token);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NonNull Call<Void> call, @NonNull Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // gets the id from the headers
                        String locationHeaders = response.headers().get("Location");
                        if (locationHeaders != null) {
                            // using REGEX to extract the id
                            Pattern pattern = Pattern.compile("/api/categories/([a-fA-F0-9]+)");
                            Matcher matcher = pattern.matcher(locationHeaders);
                            if (matcher.find()) {
                                // set the id that mongoDB gave to it
                                category.setId(matcher.group(1));
                            }
                            dao.insert(category);
                        }
                        // updates the live data that contains all the categories in the ROOM right now
                        categoryListData.postValue(dao.getAll());
                    }).start();
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Category Created");
                } else {
                    if (response.code() == 401 || response.code() == 403) {
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Token Is Required/Invalid");
                    } else {
                        Utils.handleError(response, res);
                    }
                }
            }

            @Override
            public void onFailure(@NonNull Call<Void> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void getCategory(String id, WebResponse res) {
        Call<Category> call = categoryWebServiceAPI.getCategory(id);
        call.enqueue(new Callback<Category>() {
            @Override
            public void onResponse(@NonNull Call<Category> call, @NonNull Response<Category> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        // gets the category from the body and insert it to the ROOM DB
                        Category category = response.body();
                        category.setId(id);

                        // insert the category into the Room database
                        dao.insert(category);
                        // updates the live data that contains all the categories in the ROOM right now
                        categoryListData.postValue(dao.getAll());
                    }).start();
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Ok");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(@NonNull Call<Category> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void updateCategory(Category category, String token, WebResponse res) {
        Call<Void> call = categoryWebServiceAPI.updateCategory(category.getId(), category, token);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NonNull Call<Void> call, @NonNull Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // if category not found in ROOM, then insert it and only then update
                        if (dao.get(category.getId()) == null) {
                            dao.insert(category);
                        }
                        // update the category in the Room database
                        dao.update(category);
                        // updates the live data that contains all the categories in the ROOM right now
                        categoryListData.postValue(dao.getAll());
                    }).start();
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Category Updated");
                } else {
                    if (response.code() == 401 || response.code() == 403) {
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Token Is Required/Invalid");
                    } else {
                        Utils.handleError(response, res);
                    }
                }
            }

            @Override
            public void onFailure(@NonNull Call<Void> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void deleteCategory(String id, String token, WebResponse res) {
        Call<Void> call = categoryWebServiceAPI.deleteCategory(id, token);
        call.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(@NonNull Call<Void> call, @NonNull Response<Void> response) {
                if (response.isSuccessful()) {
                    new Thread(() -> {
                        // find the category in the ROOM
                        Category category = dao.get(id);
                        // if category found
                        if (category != null) {
                            // delete the category from the Room database
                            dao.delete(category);
                        }
                        // updates the live data that contains all the categories in the ROOM right now
                        categoryListData.postValue(dao.getAll());
                    }).start();
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Category Deleted");
                } else {
                    if (response.code() == 401 || response.code() == 403) {
                        res.setResponseCode(response.code());
                        res.setResponseMsg("Token Is Required/Invalid");
                    } else {
                        Utils.handleError(response, res);
                    }
                }
            }

            @Override
            public void onFailure(@NonNull Call<Void> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }

    public void reload(WebResponse res) {
        // gets all the existing categories in the server
        Call<List<Category>> call = categoryWebServiceAPI.getAllCategories();
        call.enqueue(new Callback<List<Category>>() {
            @Override
            public void onResponse(@NonNull Call<List<Category>> call, @NonNull Response<List<Category>> response) {
                if (response.isSuccessful() && response.body() != null) {
                    new Thread(() -> {
                        // clears the ROOM database
                        dao.clear();
                        // inserts all the categories from the response to the ROOM
                        dao.insert(response.body().toArray(new Category[0]));
                        // updating the CategoryListData s.t it will be the live data from the ROOM
                        categoryListData.postValue(dao.getAll());
                    }).start();
                    res.setResponseCode(response.code());
                    res.setResponseMsg("Ok");
                } else {
                    Utils.handleError(response, res);
                }
            }

            @Override
            public void onFailure(@NonNull Call<List<Category>> call, @NonNull Throwable t) {
                // can't connect to server
                res.setResponseCode(500);
                res.setResponseMsg("Internal Server Error" + t.getMessage());
            }
        });
    }
}