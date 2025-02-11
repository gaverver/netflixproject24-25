package com.example.netflix.fragments;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.Toast;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;

import com.example.netflix.R;
import com.example.netflix.entities.Category;
import com.example.netflix.viewmodels.CategoryViewModel;
import com.example.netflix.WebResponse;

import java.util.Arrays;
import java.util.List;

public class EditCategoryFragment extends Fragment {

    private CategoryViewModel categoryViewModel;
    private EditText etId, etName, etMovies;
    private Switch swPromoted;
    private Button btnSearch, btnEditCategory;
    private ViewGroup editContainer;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_edit_category, container, false);

        categoryViewModel = new CategoryViewModel();

        etId = view.findViewById(R.id.et_id);
        etName = view.findViewById(R.id.et_name);
        etMovies = view.findViewById(R.id.et_movies);
        swPromoted = view.findViewById(R.id.sw_promoted);
        btnSearch = view.findViewById(R.id.btn_search);
        btnEditCategory = view.findViewById(R.id.btn_edit_category);
        editContainer = view.findViewById(R.id.edit_container);

        // Hide editing fields initially
        editContainer.setVisibility(View.GONE);

        btnSearch.setOnClickListener(v -> searchCategory());
        btnEditCategory.setOnClickListener(v -> editCategory());

        return view;
    }
    private void updateCategory(List<Category> categories, String id) {
        Category category = new Category();
        boolean flag = false;
        for (Category c : categories) {
            if (c.getId().equals(id)) {
                category = c;
                flag = true;
                break;
            }
        }
        if (flag) {
            Toast.makeText(getContext(), "got the category successfully", Toast.LENGTH_SHORT).show();
            etName.setText(category.getName());
            etMovies.setText(String.join(",", category.getMovies()));
            swPromoted.setChecked(category.getPromoted());
            editContainer.setVisibility(View.VISIBLE);
        } else {
            Toast.makeText(getContext(), "category not found", Toast.LENGTH_SHORT).show();
        }
    }
    private void searchCategory() {
        String id = etId.getText().toString().trim();
        if (id.isEmpty()) {
            Toast.makeText(getContext(), "Please enter an ID", Toast.LENGTH_SHORT).show();
            return;
        }

        categoryViewModel.getAll().observe(this.getViewLifecycleOwner(), new Observer<List<Category>>() {
            @Override
            public void onChanged(List<Category> categories) {
                updateCategory(categories, id);
            }
        });

        WebResponse webResponseForReload = new WebResponse();
        webResponseForReload.getResponseCode().observe(this.getViewLifecycleOwner(), new Observer<Integer>() {
            @Override
            public void onChanged(Integer integer) {
                if (integer != 200) {
                    Toast.makeText(getContext(), "Response code: " + integer.toString(), Toast.LENGTH_SHORT).show();
                }
            }
        });
        webResponseForReload.getResponseMsg().observe(this.getViewLifecycleOwner(), new Observer<String>() {
            @Override
            public void onChanged(String s) {
                if (!s.equals("Ok")) {
                    Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show();
                }
            }
        });
        categoryViewModel.reload(webResponseForReload);
    }

    private void editCategory() {
        String id = etId.getText().toString();
        String name = etName.getText().toString();
        boolean promoted = swPromoted.isChecked();
        String[] movies = etMovies.getText().toString().split(",");

        Category category = new Category(id, name, promoted, movies);

        WebResponse webResponse = new WebResponse();

        webResponse.getResponseMsg().observe(this.getViewLifecycleOwner(), new Observer<String>() {
            @Override
            public void onChanged(String s) {
                Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show();
            }
        });

        categoryViewModel.update(category, webResponse);
    }
}
