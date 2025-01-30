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
    // if darkmode is applied, set it
    if (savedMode === 'true') {
      setIsDarkMode(true);
      // this line is creating a subclass of the body tag, used to decorate the components-darkmode
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

  // Trigger search navigation when searchText changes (only when it changes, then navigate)
  useEffect(() => {
    // trim is for checking that the text isn't empty because then, navigate could lead to notFoundPage  
    if (searchText.trim()) {
      navigate(`/movies/search/${searchText}`);
    }
  }, [searchText, navigate]); // Whenever searchText changes, activate this function

  // when the user enter text into the search input, this will be called
  const handleSearch = async (e) => {
    // getting the text
    const text = e.target.value;
    // set it to the global variable
    setSearchText(text);
    /* Now, ONLY! when the set is done, the useEffect above will be called, and navigate will be used */
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
        <input type="text" placeholder="Search" onChange={handleSearch} className="search-input" />
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
          {isDarkMode ? 'Light Mode' : 'Dark Mode'}
        </button>
        
        {/* Administration button (only if he's admin) */}
        {isAdmin && (
          <Link to="/admin" className="action-button">
            Administration
          </Link>
        )}

        {/* Categories page button */}
        <Link to="/categories" className="action-button">
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
      <div className="logo2">
        <Link to="/">Catflix</Link>
      </div>
    </header>
  );
};

export default Menu;
