import React, { useState, useEffect } from "react";
import "./moviePic.css";
import { useNavigate } from 'react-router-dom';
import { getUserFromToken, convertToURL } from "../../utils";

const MoviePic = ({ id }) => {
  // State for movie details
  const [movie, setMovie] = useState(null);
  // State for error handling
  const [error, setError] = useState(false);
  const navigate = useNavigate();

  // this function will be called everytime the 
  useEffect(() => {
    const fetchMovie = async () => {
      try {
        // Fetch movie details
        const res = await fetch(`http://localhost:3001/api/movies/${id}`);
        if (!res.ok) {
          throw new Error("Failed to fetch movie");
        }
        const data = await res.json();
        setMovie(data); // Update state with movie details
      } catch (err) {
        console.error(err);
        setError(true); // Set error state if fetch fails
      }
    };

    fetchMovie();
  }, [id]); // Run the effect only when `id` changes

  const whenClicked = () => {
    navigate(`/movies/${id}`); // Navigate to movie details page
  };

  // Show a placeholder or error message if the movie is not yet loaded
  if (error) {
    navigate("/serverError");
  }

  // convert the movie's image to a url s.t it will be easy to display it on the screen
  const imageURL = convertToURL(movie.photo);

  // the react component
  return (
    <div className="movie-pic-container" onClick={whenClicked}>
      <img src={imageURL} alt="Movie Poster" className="movie-pic" />
    </div>
  );
};

export default MoviePic;
