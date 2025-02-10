package com.example.netflix.fragments;


import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Switch;
import androidx.fragment.app.Fragment;
import com.example.netflix.R;
import com.example.netflix.entities.Category;
import com.example.netflix.viewmodels.CategoryViewModel;
import com.example.netflix.WebResponse;

public class AddCategoryFragment extends Fragment {

    private CategoryViewModel categoryViewModel;
    private EditText etName, etMovies;
    private Switch swPromoted;
    private Button btnAddCategory;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_add_category, container, false);

        categoryViewModel = new CategoryViewModel();

        etName = view.findViewById(R.id.et_name);
        etMovies = view.findViewById(R.id.et_movies);
        swPromoted = view.findViewById(R.id.sw_promoted);
        btnAddCategory = view.findViewById(R.id.btn_add_category);

        btnAddCategory.setOnClickListener(v -> addCategory());

        return view;
    }

    private void addCategory() {
        String name = etName.getText().toString();
        boolean promoted = swPromoted.isChecked();
        String[] movies = etMovies.getText().toString().split(",");

        Category category = new Category(name, promoted, movies);

        categoryViewModel.create(category, "your_token_here", new WebResponse());
    }
}

