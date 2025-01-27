import './searchResults.css';
import React, { useState, useEffect } from "react";
import { useParams, useNavigate } from "react-router-dom";
import Menu from '../../components/menu/menu';
import MoviePic from '../../components/moviePic/moviePic'

const helper = require('../../utils').default;

const SearchResults = () => {
  // Get the query from the URL parameters
  const { query } = useParams();
  const navigate = useNavigate();

  // State to manage results and loading state
  const [listResults, setListResults] = useState([]);
  const [noMovies, setNoMovies] = useState(false);
  const [user, setUser] = useState(null);
  const [photoURL, setPhotoURL] = useState(null);

  // Fetch data and handle user authentication
  useEffect(() => {
    const fetchData = async () => {
      // Check if the user is authenticated
      const userToken = sessionStorage.getItem('jwt');
      if (!userToken) {
        return navigate("/login");
      }

      // Get the user from the token
      const currentUser = await helper.getUserFromToken(userToken);
      console.log(currentUser);
      if (!currentUser) {
        return navigate("/login");
      }
      // gets the user by fetching to api/user/:id
      const user_res = await fetch (`http://localhost:3001/api/users/${currentUser}`);
      const json = await user_res.json();
      // Store the user in state
      setUser(json);
      console.log(json);
      // Fetch the photo URL asynchronously
      const photoURL = await helper.convertToURL(json.picture);
      setPhotoURL(photoURL);
      // Fetch the movies based on the search query
      try {
        const list_res = await fetch(`http://localhost:3001/api/movies/search/${query}`);
        if (!list_res.ok) {
          return navigate("/serverError");
        }
        
        // gets the list of results
        const results = await list_res.json();
        setListResults(results.movies);
        console.log(listResults);
        // Check if no movies were found
        setNoMovies(results.length === 0);
      } catch (error) {
        console.error("Failed to fetch movies:", error);
        navigate("/serverError");
      }
    };

    fetchData();
  }, [query, navigate]); // Dependency array

  // Render the component
  return (
    <div className="results-page">
      {user && (
        <Menu
          username={user.username}
          photo={photoURL}
          isAdmin={user.privilegeLevel === 1}
        />
      )}
      {/* Display message if no movies are found */}
      {noMovies && (
        <div className="noMoviesText">No movies were found</div>
      )}
      {/* Display movies in a grid */}
      <div className="movies-grid">
        {listResults.map((movie) => (
          <MoviePic key={movie._id} id={movie._id} />
        ))}
      </div>
    </div>
  );
};


export default SearchResults;
