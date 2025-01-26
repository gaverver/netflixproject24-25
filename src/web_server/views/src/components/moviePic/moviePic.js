import React from "react";
import "./moviePic.css";
import { useNavigate } from 'react-router-dom';

const imageService = require('../../../../services/images');

const MoviePic = async ({ id }) => {
  const navigate = useNavigate();

  // gets the details on the movie
  const res = await fetch(`http://localhost:3000/api/movies/${id}`);
  // somthing failed
  if (!res.ok) {
    return null;
  }
  // else the movie is returned
  const movie = res.json();
  // else, gets the image of the movie as a url
  const imageURL = imageService.convertToURL(movie.photo);

  // this function will navigate to the details on that movie - page
  const whenClicked = () => {
    navigate(`/movies/${id}`);
  }

  return (
    <div className="movie-pic-container" onClick={whenClicked}>
      <img src={imageURL} alt="Movie Poster" className="movie-pic" />
    </div>
  );
};

export default MoviePic;
