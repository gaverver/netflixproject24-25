import './logged_in_home.css';
import React, { useState, useEffect } from "react";
import { useParams, useNavigate } from "react-router-dom";
import Menu from '../../components/menu/menu';
import MovieList from '../../components/movieList/movieList'

// import helper functions
const helper = require('../../utils').default;

const HomeForLoggedIn = () => {
  const navigate = useNavigate();

  // State for all promoted categories in a list 
  const [categoryList, setCategoryList] = useState([]);
  // State for the 20 last movies the user watched
  const [watchedMovies, SetWatchedMovies] = useState([]);
  // State for the user that use this page
  const [user, setUser] = useState(null);
  // State for the URL (after convert) of the photo of the user
  const [photoURL, setPhotoURL] = useState(null);

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
      const user_res = await fetch (`http://localhost:3001/api/users/${currentUser}`);
      // convert to json
      const json = await user_res.json();
      // Store the user in state
      setUser(json);
      // Store the last watched movies in state
      SetWatchedMovies(json.watched_movies);
      // Fetch the photo URL asynchronously
      const photoURL = await helper.convertToURL(json.picture);
      // set it to the state
      setPhotoURL(photoURL);
      // Fetch all categories
      try {
        const list_res = await fetch("http://localhost:3001/api/categories");
        // if the server crushed
        if (!list_res.ok) {
          return navigate("/serverError");
        }
        
        // gets the list of categories
        const categories = await list_res.json();
        // take only the promoted categories
        const promotedCategories = categories.filter(category => category.promoted);
        // set it to the state
        setCategoryList(promotedCategories);
      } catch (error) {
        // if the server crushed
        navigate("/serverError");
      }
    };

    fetchData();
  }, []);

  // Render the component
  return (
    <div className="home-page-logged-in">
      {/* the top menu component */}
      {user && (
        <Menu
          username={user.username}
          photo={photoURL}
          isAdmin={user.privilegeLevel === 1}
        />
      )}
      {/* display different categories in different lines */}
      <div className="categories-lines">
        { /* watched movies */ }
          <MovieList key={"watched movies"} list={watchedMovies} contentType={"Watched Movies"}/>

        {categoryList.map((category) => (
          <MovieList key={category._id} list={category.movieIds} contentType={category.name}/>
        ))}
      </div>
    </div>
  );
};


export default HomeForLoggedIn;
