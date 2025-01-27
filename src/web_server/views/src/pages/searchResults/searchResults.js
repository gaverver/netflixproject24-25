import './searchResults.css';
import React, { useState, useEffect } from "react";
import { useParams, useNavigate } from "react-router-dom";
import Menu from '../../components/menu/menu';
import MoviePic from '../../components/moviePic/moviePic'
import { getUserFromToken, convertToURL } from '../../utils';

const SearchResults = () => {
  // Get the query from the URL parameters
  const { query } = useParams();
  const navigate = useNavigate();

  // State to manage results and loading state
  const [listResults, setListResults] = useState([]);
  const [noMovies, setNoMovies] = useState(false);
  const [user, setUser] = useState(null);

  // Fetch data and handle user authentication
  useEffect(() => {
    const fetchData = async () => {
      // Check if the user is authenticated
      const userToken = sessionStorage.getItem('jwt');
      if (!userToken) {
        return navigate("/login");
      }

      // Get the user from the token
      const user = getUserFromToken(userToken);
      if (!user) {
        return navigate("/login");
      }
      // Store the user in state
      setUser(user);

      // Fetch the movies based on the search query
      try {
        const res = await fetch(`http://localhost:3001/api/movies/search/${query}`);
        if (!res.ok) {
          return navigate("/serverError");
        }
        
        // gets the list of results
        const results = await res.json();
        setListResults(results);
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
          photo={convertToURL(user.picture)}
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
          <MoviePic id={movie.id} />
        ))}
      </div>
    </div>
  );
};


export default SearchResults;
