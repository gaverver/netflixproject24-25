import "./Admin.css";
import React from "react";
import { useNavigate } from "react-router";

function Admin() {
  const navigate = useNavigate();

  return (
    <div className="admin-container">
      <h1 className="admin-title">Admin Panel</h1>
      <div className="button-container">
        <button className="admin-button" onClick={() => navigate("/admin/categories")}>
          Manage Categories
        </button>
        <button className="admin-button" onClick={() => navigate("/admin/movies")}>
          Manage Movies
        </button>
      </div>
    </div>
  );
}

export default Admin;
