import './MovieAdmin.css'
import React, { useState } from "react";
import FullMovieEditor from '../../components/FullMovieEditor';
import MovieCreator from '../../components/MovieCreator';
import CategoryCreator from '../../components/CategoryCreator'
import CategoryEditor from '../../components/CategoryEditor';

function CategoryAdmin() {
    const [activeTab, setActiveTab] = useState("tab1");
    const [categoryId, setCategoryId] = useState(null);
    const handleTabClick = (tab) => {
      setActiveTab(tab);
    };
  
    return (
      <div>
        <div className="tabs">
          <span
            className={`tab ${activeTab === "tab1" ? "active" : ""}`}
            onClick={() => handleTabClick("tab1")}
          >
            Edit/Delete Category
          </span>
          <span
            className={`tab ${activeTab === "tab2" ? "active" : ""}`}
            onClick={() => handleTabClick("tab2")}
          >
            Add Category
          </span>
        </div>
  
        <div className="tab-content">
          {activeTab === "tab1" && (
            <div>
                <div>
                <label htmlFor="name">category id:</label>
                <input
                    type="text"
                    id="category-id"
                    value={categoryId}
                />
                <button 
                    type="button" 
                    onClick={() => setCategoryId(document.getElementById("category-id").value)}
                    >
                    search category
                </button>
                </div>
            <CategoryEditor categoryId={categoryId}/>
            </div>
          )}
          {activeTab === "tab2" && (
            <CategoryCreator/>
          )}
        </div>
      </div>
    );
}



export default CategoryAdmin;