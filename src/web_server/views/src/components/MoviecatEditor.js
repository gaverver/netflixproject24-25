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

    const handleAdd = async () => {
        const categoryId = newCategory.trim();
        if (categoryId !== "") {
            try {
                if (categories.includes(categoryId)) {
                    alert(`Category ID ${categoryId} already selected.`);
                    return;
                }
                const response = await fetch(`http://localhost:3001/api/categories/${categoryId}`);
                if (response.status === 404) {
                alert(`Category ID ${categoryId} does not exist.`);
                return;
                } else if (!response.ok) {
                    alert("An error occurred while checking the category.");
                    return;
                }
        
                setCategories([...categories, categoryId]);
                setNewCategory("");
            } catch (error) {
                alert("An error occurred while checking the category.");
            }
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
          onKeyPress={ (e) => {
            if (e.key === "Enter") {
                e.preventDefault();
                handleAdd();
            }
            }}
        />
        <button onClick={handleAdd}>Add Category</button>
      </div>
    </div>
  );
};

export default MoviecatEditor;