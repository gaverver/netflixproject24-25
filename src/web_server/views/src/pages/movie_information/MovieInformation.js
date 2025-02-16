import "./MovieInformation.css";
import React, { useEffect, useState } from "react";
import Menu from "../../components/menu/menu";
import MovieInfoDisplay from "../../components/MovieInfoDisplay";
import { useNavigate, useParams } from "react-router-dom";
const helper = require('../../utils').default;
function MovieInformation() {
  const {id} = useParams();
  const navigate = useNavigate();
  const [userId, setUserID] = useState("");

  const [user, setUser] = useState(null);
  // state for the the URL (after convert) of the photo of the user
  const [photoURL, setPhotoURL] = useState(null);
  const fetchUserId = async (token) => {
    const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/tokens/${token}`);
    if (!response.ok) {
      console.error("token isn't valid");
      //navigate home
      // navigate("/home")
      return;
    }
    const data = await response.json();
    setUserID(data.userId)
  }
  // Fetch data and handle user authentication
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
    const userToken = sessionStorage.getItem('jwt');
    fetchUserId(userToken);
  }, [id]);

  return (
    <div>
        {user && (
          <Menu
            username={user.username}
            photo={photoURL}
            isAdmin={user.privilegeLevel === 1}
          />
        )}
        <MovieInfoDisplay key={id} id={id} userId={userId} />
    </div>
  );
}

export default MovieInformation;