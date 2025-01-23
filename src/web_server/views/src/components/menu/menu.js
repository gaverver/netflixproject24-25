import './menu.css';
import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';

const menu = ({ username, photo, isLoggedIn, isAdmin }) => {
  const [isDarkMode, setIsDarkMode] = useState(false);

  useEffect(() => {
    const savedMode = localStorage.getItem('darkMode');
    if (savedMode === 'true') {
      setIsDarkMode(true);
      document.body.classList.add('dark-mode');
    }
  }, []);

  const toggleDarkMode = () => {
    setIsDarkMode(!isDarkMode);
    localStorage.setItem('darkMode', !isDarkMode);
    document.body.classList.toggle('dark-mode');
  };

  return (
    <header className={`menu ${isDarkMode ? 'dark-mode' : ''}`}>
      <div className="logo">Catflix</div>

      <div className="user-info">
        {isLoggedIn ? (
          <>
            <img src={photo} alt={username} />
            <span>{username}</span>
          </>
        ) : (
          <Link to="/login">Login</Link>
        )}
      </div>

      <div className="actions">
        <input type="text" placeholder="Search" />
        <button>Search</button>
        <Link to="/api/homeForLoggedInUsers">Home</Link>
        <Link to="/api/categories">Categories</Link>
        {isLoggedIn && (
          <>
            <button onClick={toggleDarkMode}>
              {isDarkMode ? 'Light Mode' : 'Dark Mode'}
            </button>
            {isAdmin && <Link to="/admin">Administration</Link>}
            <Link to="/logout">Sign Out</Link>
          </>
        )}
      </div>
    </header>
  );
}

export default menu;
