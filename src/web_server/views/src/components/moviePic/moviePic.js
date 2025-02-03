import React, { useState, useEffect } from "react";
import "./moviePic.css";
import { useNavigate } from 'react-router-dom';

// import helper functions
const helper = require('../../utils').default;

const MoviePic = ({ id }) => {
  // State for error handling
  const [error, setError] = useState(false);
  // State for the URL (after convert) of the photo of the movie
  const [photoURL, setPhotoURL] = useState(null);
  const navigate = useNavigate();

  // this function will be called everytime the 
  useEffect(() => {
    const fetchMovie = async () => {
      try {
        // Fetch movie details
        const res = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}`);
        if (!res.ok) {
          // will be catch later
          throw new Error("Failed to fetch movie");
        }
        const data = await res.json();
        // set the photo into the state
        const photo = await helper.convertToURL(data.photo);
        setPhotoURL(photo);
      } catch (err) {
        console.error(err);
        setError(true); // Set error state if fetch fails
      }
    };

    fetchMovie();
  }, [id]); // Run the effect only when `id` changes

  const whenClicked = () => {
    navigate(`/movies/information/${id}`); // Navigate to movie details page
  };

  // Show a placeholder or error message if the movie is not yet loaded
  if (error) {
    navigate("/serverError");
  }

  // the react component
  return (
    <div className="movie-pic-container" onClick={whenClicked}>
      <img src={photoURL} alt="Movie Poster" className="movie-pic" />
    </div>
  );
};

export default MoviePic;
