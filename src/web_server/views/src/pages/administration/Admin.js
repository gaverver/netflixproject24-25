import "./Admin.css";
import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router";
import Menu from "../../components/menu/menu";
const helper = require('../../utils').default;
function Admin() {
  const navigate = useNavigate();
  const [photoURL, setPhotoURL] = useState(null);    
  const [user, setUser] = useState(null);
  useEffect(() => {
    const fetchData = async () => {
      // Check if the user is authenticated
      const userToken = sessionStorage.getItem('jwt');
      // if the user isn't connected, navigate him to the login page
      if (!userToken) {
        return navigate("/login");
      }

      // Get the user from the token
      const currentUser = await helper.getUserFromToken(userToken);
      // if the user isn't connected, navigate him to the login page
      if (!currentUser) {
        return navigate("/login");
      }
      // gets the user by fetching to api/user/:id
      const user_res = await fetch (`http://localhost:${process.env.REACT_APP_PORT}/api/users/${currentUser}`);
      // convert to json
      const json = await user_res.json();
      // Store the user in state
      setUser(json);
      // Fetch the photo URL asynchronously
      const photoURL = await helper.convertToURL(json.picture);
      // set it to the state
      setPhotoURL(photoURL);
      // Fetch the movies based on the search query
    };

    fetchData();
  }, []);
  return (
    <div className="admin-container">
      {user && (
          <Menu
            username={user.username}
            photo={photoURL}
            isAdmin={user.privilegeLevel === 1}
          />
        )}
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
