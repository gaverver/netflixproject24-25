  import './logged_in_home.css';
  import React, { useState, useEffect } from "react";
  import { useNavigate } from "react-router-dom";
  import Menu from '../../components/menu/menu';
  import MovieList from '../../components/movieList/movieList';
  import RunRandomMovie from '../../components/runRandomMovie/runRandomMovie';

  // import helper functions
  const helper = require('../../utils').default;

  const HomeForLoggedIn = () => {
    const navigate = useNavigate();

    // State for all promoted categories in a list 
    const [categoryList, setCategoryList] = useState([]);
    // State for the user that use this page
    const [user, setUser] = useState(null);
    // State for the URL (after convert) of the photo of the user
    const [photoURL, setPhotoURL] = useState(null);
    // State for the random movie run in background
    const [randomMovieId, setRandomMovieId] = useState(null);

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

        // gets the movies from "/api/movies"
        const movies = await fetch (`http://localhost:${process.env.REACT_APP_PORT}/api/movies`, {
          // the headers needed for this fetch
          headers: {
            'userId': json._id,
            'Authorization': `Bearer ${userToken}`
          }
        });
        const movies_json = (await movies.json());
        setCategoryList(movies_json.movies);
        // pick a random movie once categories are loaded
        if (movies_json.movies) {
          pickRandomMovie(movies_json.movies);
        }
      };

      fetchData();
    }, []);

    // function to pick a random movie from all categories
    const pickRandomMovie = (movies) => {
      // flatten all movie IDs into a single array
      const allMovies = Object.values(movies).flat();
      if (allMovies.length > 0) {
        const randomId = allMovies[Math.floor(Math.random() * allMovies.length)];
        setRandomMovieId(randomId);
      }
    };

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
        {/* random movie on background */}
        { randomMovieId && (
          <RunRandomMovie id={randomMovieId} className="random-movie" />
        )}
        {/* display different categories in different lines */}
        <div className="categories-lines">
          {/* go through all the categoires (including the 20 last watched movies) */}
          {Object.entries(categoryList).map(([name, movieIds]) => (
            <MovieList key={name} list={movieIds} contentType={name} />
          ))}
        </div>
      </div>
    );
  };


  export default HomeForLoggedIn;
