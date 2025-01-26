import React, { useState, useEffect } from "react";

const MoviecatEditor = ({ categories, setCategories }) => {
  const [newCategory, setNewCategory] = useState("");

  useEffect(() => {
    console.log("Categories updated:", categories);
  }, [categories]);

  const handleDelete = (index) => {
    const updatedCategories = categories.filter((_, i) => i !== index);
    setCategories(updatedCategories);
  };

  const handleAdd = () => {
    if (newCategory.trim() !== "") {
      setCategories([...categories, newCategory.trim()]);
      setNewCategory("");
    }
  };

  return (
    <div className="moviecat-editor">
      <label>Categories</label>
      <ul>
        {categories.map((category, index) => (
          <li key={index}>
            {category} 
            <button onClick={() => handleDelete(index)}>Delete</button>
          </li>
        ))}
      </ul>
      <div>
        <input
          type="text"
          placeholder="New category ID"
          value={newCategory}
          onChange={(e) => setNewCategory(e.target.value)}
        />
        <button onClick={handleAdd}>Add Category</button>
      </div>
    </div>
  );
};

export default MoviecatEditor;