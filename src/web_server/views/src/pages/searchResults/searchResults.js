import './searchResults.css';
import React, { useState, useEffect } from "react";
import { useParams, useNavigate } from "react-router-dom";
import Menu from '../../components/menu/menu';
import MoviePic from '../../components/moviePic/moviePic'

// import helper functions
const helper = require('../../utils').default;

const SearchResults = () => {
  // Get the query from the URL parameters
  const { query } = useParams();
  const navigate = useNavigate();

  // State to manage results and loading state
  const [listResults, setListResults] = useState([]);
  // State for knowing if there were no movies from the result
  const [noMovies, setNoMovies] = useState(false);
  // State for the user that use this page
  const [user, setUser] = useState(null);
  // state for the the URL (after convert) of the photo of the user
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
      try {
        const list_res = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/search/${query}`);
        // if the server crushed
        if (!list_res.ok) {
          return navigate("/serverError");
        }
        
        // gets the list of results
        const results = await list_res.json();
        // results.movies contain the list of movies as returned from the fetch
        setListResults(results.movies);
        // check if no movies were found
        setNoMovies(results.movies.length === 0);
      } catch (error) {
        // if the server crushed
        navigate("/serverError");
      }
    };

    fetchData();
  }, [query, navigate]); // call this whenever query changed

  // Render the component
  return (
    <div className="results-page">
      {/* the top menu component */}
      {user && (
        <Menu
          username={user.username}
          photo={photoURL}
          isAdmin={user.privilegeLevel === 1}
        />
      )}
      {/* display message if no movies were found */}
      {noMovies && (
        <div className="noMoviesText">No movies were found</div>
      )}
      {/* display relevant movies in a grid */}
      <div className="movies-grid">
        {listResults.map((movie) => (
          <MoviePic key={movie._id} id={movie._id} />
        ))}
      </div>
    </div>
  );
};


export default SearchResults;
