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

import java.util.List;

public class EditCategoryFragment extends Fragment {

    private CategoryViewModel categoryViewModel;
    private EditText etId, etName, etMovies;
    private Switch swPromoted;
    private Button btnSearch, btnEditCategory, btnDeleteCategory;
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
        btnDeleteCategory = view.findViewById(R.id.btn_delete_category);
        editContainer = view.findViewById(R.id.edit_container);

        // Hide editing fields and delete button initially
        editContainer.setVisibility(View.GONE);
        btnDeleteCategory.setVisibility(View.GONE);

        btnSearch.setOnClickListener(v -> searchCategory());
        btnEditCategory.setOnClickListener(v -> editCategory());
        btnDeleteCategory.setOnClickListener(v -> deleteCategory());

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
            Toast.makeText(getContext(), "Got the category successfully", Toast.LENGTH_SHORT).show();
            etName.setText(category.getName());
            etMovies.setText(String.join(",", category.getMovies()));
            swPromoted.setChecked(category.getPromoted());
            editContainer.setVisibility(View.VISIBLE);
            btnDeleteCategory.setVisibility(View.VISIBLE);
        } else {
            Toast.makeText(getContext(), "Category not found", Toast.LENGTH_SHORT).show();
            editContainer.setVisibility(View.GONE);
            btnDeleteCategory.setVisibility(View.GONE);
        }
    }

    private void searchCategory() {
        String id = etId.getText().toString().trim();
        if (id.isEmpty()) {
            Toast.makeText(getContext(), "Please enter an ID", Toast.LENGTH_SHORT).show();
            return;
        }

        categoryViewModel.getAll().observe(getViewLifecycleOwner(), categories -> updateCategory(categories, id));

        WebResponse webResponseForReload = new WebResponse();
        webResponseForReload.getResponseCode().observe(getViewLifecycleOwner(), integer -> {
            if (integer != 200) {
                Toast.makeText(getContext(), "Response code: " + integer, Toast.LENGTH_SHORT).show();
            }
        });
        webResponseForReload.getResponseMsg().observe(getViewLifecycleOwner(), s -> {
            if (!s.equals("Ok")) {
                Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show();
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

        webResponse.getResponseMsg().observe(getViewLifecycleOwner(), s ->
                Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show());

        categoryViewModel.update(category, webResponse);
    }

    private void deleteCategory() {
        String id = etId.getText().toString();

        WebResponse webResponse = new WebResponse();

        webResponse.getResponseMsg().observe(getViewLifecycleOwner(), s -> {
            Toast.makeText(getContext(), "Response message: " + s, Toast.LENGTH_LONG).show();
            if (s.equals("Ok")) {
                // Clear fields and hide edit container and delete button if deletion is successful
                etId.setText("");
                etName.setText("");
                etMovies.setText("");
                swPromoted.setChecked(false);
                editContainer.setVisibility(View.GONE);
                btnDeleteCategory.setVisibility(View.GONE);
            }
        });

        categoryViewModel.delete(id, webResponse);
    }
}

