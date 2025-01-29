// import './CategoryAdmin.css'
import React, { useState } from "react";
import FullMovieEditor from '../../components/FullMovieEditor';
import MovieCreator from '../../components/MovieCreator';
import CategoryCreator from '../../components/CategoryCreator'

function CategoryAdmin() {
    const [activeTab, setActiveTab] = useState("tab1");

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
            // <FullMovieEditor/>
            <h1>hi</h1>
          )}
          {activeTab === "tab2" && (
            <CategoryCreator/>
          )}
        </div>
      </div>
    );
}



export default CategoryAdmin;