import "./MovieAutocomplete.css"
import React, { useState } from 'react';

const MovieAutocomplete = ({setId}) => {
  const [query, setQuery] = useState('');
  const [suggestions, setSuggestions] = useState([]);
  const [isLoading, setIsLoading] = useState(false);

  // Fetch movie names based on the query
  const fetchMovies = async (searchQuery) => {
    try {
      setIsLoading(true);
      const response = await fetch(`http://localhost:3001/api/movies/search/${searchQuery}`);
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
  
      const data = await response.json();
  
      // Access the array of movies inside the JSON object
      if (data.movies && Array.isArray(data.movies)) {
        const movieNames = data.movies.map((movie) => ({ name: movie.name, id: movie._id }));
        setSuggestions(movieNames);
      } else {
        console.warn('API response does not contain a valid movies array:', data);
        setSuggestions([]);
      }
    } catch (error) {
      console.error('Error fetching movies:', error);
      setSuggestions([]);
    } finally {
      setIsLoading(false);
    }
  };

  // Handle input change
  const handleChange = (e) => {
    const inputValue = e.target.value;
    setQuery(inputValue);

    if (inputValue) {
      fetchMovies(inputValue);
    } else {
      setSuggestions([]);
    }
  };

  // Handle selection of a suggestion
  const handleSelect = (name, movie_id) => {
    setQuery(name); // Set input value to the selected name
    setSuggestions([]); // Clear suggestions
    setId(movie_id)
  };

  return (
    <div className="movieautocomplete">
      <input
        type="text"
        value={query}
        onChange={handleChange}
        placeholder="Search for a movie..."
        aria-label="Search for a movie"
      />
      {isLoading && <p>Loading...</p>}
      {suggestions.length > 0 && (
        <ul className="suggestions-list">
          {suggestions.map((movie, index) => (
            <li key={index} onClick={() => handleSelect(movie.name, movie.id)}>
              {movie.name}
            </li>
          ))}
        </ul>
      )}
    </div>
  );
};

export default MovieAutocomplete;