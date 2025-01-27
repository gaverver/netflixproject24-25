import './MovieAdmin.css'
import React, { useState } from "react";
import FullMovieEditor from '../../components/FullMovieEditor';

function MovieAdmin() {
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
            Tab 1
          </span>
          <span
            className={`tab ${activeTab === "tab2" ? "active" : ""}`}
            onClick={() => handleTabClick("tab2")}
          >
            Tab 2
          </span>
        </div>
  
        <div className="tab-content">
          {activeTab === "tab1" && (
            <FullMovieEditor/>
          )}
          {activeTab === "tab2" && (
            <div>
              <h2>Form 2</h2>
              <form>
                <label>Email: </label>
                <input type="email" />
              </form>
            </div>
          )}
        </div>
      </div>
    );
}



export default MovieAdmin;