import './runRandomMovie.css';
import React, { useState, useEffect } from "react";
import { useNavigate } from 'react-router-dom';

const RunRandomMovie = ({ id }) => {
  const [movieDetails, setMovieDetails] = useState(null);
  
  const navigate = useNavigate();

  // this function will be called everytime the 
  useEffect(() => {
    const fetchMovie = async () => {
      try {
        // fetching to get movie details
        const movie_res = await fetch (`http://localhost:3001/api/movies/${id}`);
        // convert to json
        const json = await movie_res.json();
        setMovieDetails(json);
      } catch (err) {
        navigate('/serverError');
      }
    };

    fetchMovie();
  }, [id]); // Run the effect only when `id` changes

  const whenClicked = () => {
    navigate(`/movies/${id}`); // Navigate to movie details page
  };

  // the react component
  return (
    <div>
      {movieDetails && (
        <div>
            <div>{movieDetails.description}</div>
            <div>{movieDetails.actors}</div>
        </div>
      )}
      <button className="play-random-movie" onClick={whenClicked} >Play</button>
    </div>
  );
};

export default RunRandomMovie;
