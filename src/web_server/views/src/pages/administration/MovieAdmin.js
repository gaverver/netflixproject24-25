import './MovieAdmin.css'
import React, { useState } from "react";
import FullMovieEditor from '../../components/FullMovieEditor';
import MovieCreator from '../../components/MovieCreator';
import { useNavigate } from "react-router-dom";

function MovieAdmin() {
    const [activeTab, setActiveTab] = useState("tab1");
    const navigate = useNavigate();

    const handleTabClick = (tab) => {
      setActiveTab(tab);
    };
  
    return (
      <div className="movie-admin-container">
        <button className="admin-page-button" onClick={() => navigate("/admin")}>Admin Page</button>
        <div className="tabs">
          <span
            className={`tab ${activeTab === "tab1" ? "active" : ""}`}
            onClick={() => handleTabClick("tab1")}
          >
            Edit/Delete Movie
          </span>
          <span
            className={`tab ${activeTab === "tab2" ? "active" : ""}`}
            onClick={() => handleTabClick("tab2")}
          >
            Add Movie
          </span>
        </div>
  
        <div className="tab-content">
          {activeTab === "tab1" && (
            <FullMovieEditor/>
          )}
          {activeTab === "tab2" && (
            <MovieCreator/>
          )}
        </div>
      </div>
    );
}



export default MovieAdmin;