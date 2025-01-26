import './searchResults.css';
import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import Menu from '../../components/menu/menu'

const tokensServices = require('../../../../services/tokens');
const imagesServices = require('../../../../services/images');

async function searchResults( {query} ) {
  let noMovies = false;
  const navigate = useNavigate();
  // if there is no token in sessionStorage then, navigate to "/login" page
  const userToken = sessionStorage.getItem('jwt');
  if (!userToken) {
    navigate("/login");
  }
  // get the user from the token
  const user = tokensServices.getUserFromToken(userToken);
  // if there is no such user beind that token, navigate to "/login" page
  if (user === null) { 
    navigate("/login");
  }
  // else, the user is connected and he can see the results
  const res = await fetch(`http://localhost:3000/api/movies/search/${query}`);
  // somthing failed
  if (!res.ok) {
    navigate("/serverError");
  }
  // gets the list of relevant movies
  const listResults = res.json();
  // if no movies were found, know it for showing text for the user
  if (listResults.length() === 0) {
    noMovies = true;
  }

  // the html of the page + components
  return (
    <div className="results-page">
      <Menu username={user.username} photo={imagesServices.convertToURL(user.picture)} isAdmin={user.privilegeLevel === 1} />
      {/* if no movies were found, show that text on the screen */}
      {noMovies && (
          <div className="noMoviesText">
            no movies were found
          </div>
      )}
      <div className="movies-grid">
        {listResults.map((movie) => (
          <MoviePic id={movie} />
        ))}
      </div>
    </div>
  );
}



export default searchResults;