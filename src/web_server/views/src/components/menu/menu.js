import './menu.css';
import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';

const Menu = ({ username, photo, isLoggedIn, isAdmin }) => {
  const [isDarkMode, setIsDarkMode] = useState(false);

  // Load dark mode preference on initial render
  useEffect(() => {
    const savedMode = localStorage.getItem('darkMode');
    if (savedMode === 'true') {
      setIsDarkMode(true);
      document.body.classList.add('dark-mode');
    }
  }, []);

  // Toggle dark/light mode
  const toggleDarkMode = () => {
    const newMode = !isDarkMode;
    setIsDarkMode(newMode);
    localStorage.setItem('darkMode', newMode);
    document.body.classList.toggle('dark-mode', newMode);
  };

  return (
    <header className={`menu ${isDarkMode ? 'dark-mode' : ''}`}>
      {/* Logo as an Image */}
      <div className="logo">
        <Link to="/">
          <img src="/path-to-your-logo.png" alt="MyApp Logo" />
        </Link>
      </div>

      {/* User Info */}
      <div className="user-info">
        {isLoggedIn && (
          <>
            <img src={photo} alt={`${username}'s avatar`} />
            <span>{username}</span>
          </>
        )}
      </div>

      {/* Actions */}
      <div className="actions">
        <Link to="/" className="action-button">
          Home
        </Link>
        <Link to="/api/categories" className="action-button">
          Categories
        </Link>
        {isLoggedIn && (
          <>
            <button onClick={toggleDarkMode} className="action-button">
              {isDarkMode ? 'Light Mode' : 'Night Mode'}
            </button>
            {isAdmin && (
              <Link to="/admin" className="action-button">
                Administration
              </Link>
            )}
            <Link to="/logout" className="action-button">
              Sign Out
            </Link>
          </>
        )}
        <input type="text" placeholder="Search" className="search-input" />
        <button className="search-button">Search</button>
      </div>
    </header>
  );
};

export default Menu;
