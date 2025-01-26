import './menu.css';
import React, { useState, useEffect } from 'react';
import { Link, useNavigate } from 'react-router-dom';

const Menu = ({ username, photo, isAdmin }) => {
  // getters and satters for the darkMode and search query parts
  const [isDarkMode, setIsDarkMode] = useState(false);
  const [searchText, setSearchText] = useState("");
  const navigate = useNavigate();

  // load dark mode preference on initial render
  useEffect(() => {
    // gets the mode (dark/light) from the localStorage and save it
    const savedMode = localStorage.getItem('darkMode');
    if (savedMode === 'true') {
      setIsDarkMode(true);
      document.body.classList.add('dark-mode');
    }
  }, []);

  // toggle dark/light mode (this function will be called if the Night/Light mode button will be pushed)
  const toggleDarkMode = () => {
    // gets the current mode (dark/light) and switching it
    const newMode = !isDarkMode;
    setIsDarkMode(newMode);
    // setting the mode in localStorage so that the mode will be set even if routing to another page
    localStorage.setItem('darkMode', newMode);
    // giving the body in the html the dark/light-mode theme using different classes in the css files
    document.body.classList.toggle('dark-mode', newMode);
  };

  // this will search for moveis with the current query
  const searchForMovies = () => {
    // gets the query
    const query = searchText;
    // navigate to see relevant moveis
    navigate(`/movies/search/${query}`);
  }

  // when the user enter text into the search input, this will be called
  const handleSearch = (e) => {
    // getting the text
    const text = e.target.value;
    // set it to the global variable
    setSearchText(text);
    // search for movies using search query
    searchForMovies();
  }

  // this will be called if the signout button is pushed
  const handleSignOut = () => {
    // first remove the token from the user (disconnect him)
    sessionStorage.removeItem("jwt");
    // then, route him to the login page
    navigate("/login");
  }

  return (
    <header className={`menu ${isDarkMode ? 'dark-mode' : ''}`}>
      {/* User Info */}
      <div className="user-info">
          <img src={photo} alt={`${username}'s avatar`} className="user-photo" />
          <span className="username">{username}</span>
      </div>

      {/* Search Section */}
      <div className="search-section">
        <input type="text" placeholder="Search" onChange={(e) => handleSearch(e)}
        onKeyDown={(e) => handleSearch(e)} className="search-input" />
        <img
          src="/images_for_decoration/search_icon.jpg"
          alt="Search"
          className="search-icon"
        />
      </div>

      {/* Actions */}
      <div className="actions">

        {/* Night/Light mode button */}
        <button onClick={toggleDarkMode} className="action-button-nightMode">
          {isDarkMode ? 'Light Mode' : 'Night Mode'}
        </button>
        
        {/* Administration button (only if he's admin) */}
        {isAdmin && (
          <Link to="/administration" className="action-button">
            Administration
          </Link>
        )}

        {/* Categories page button */}
        <Link to="/api/categories" className="action-button">
          Categories
        </Link>

        {/* Home page button */}
        <Link to="/" className="action-button">
          Home
        </Link>

        {/* signout button */}
        <button onClick={handleSignOut} className="action-button">
          Sign Out
        </button>
      </div>

      {/* Logo (when clicked, rout to the home page (for logged-in users) ) */}
      <div className="logo">
        <Link to="/">Catflix</Link>
      </div>
    </header>
  );
};

export default Menu;
