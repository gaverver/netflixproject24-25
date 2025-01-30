import React, { useState, useEffect } from "react";
import MovieAutocomplete from "./MovieAutocomplete";
import "./CategoryEditor.css"

const CategoryEditor = ( {categoryId, setCategoryId} ) => {
    const [name, setName] = useState(null);
    const [promoted, setPromoted] = useState(false)
    const [movieIds, setMovieIds] = useState([]);
    const [movieNames, setMovieNames] = useState([]);
    const [currentId, setCurrentId] = useState('')

    const fetchCategory = async () => {
        try {
            const response = await fetch(`http://localhost:3001/api/categories/${categoryId}`);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        
            const data = await response.json();
            setName(data.name)
            setPromoted(data.promoted)
            setMovieIds(data.movieIds)
            let nameList = [];
            for (const id of data.movieIds) {
                const response = await fetch(`http://localhost:3001/api/movies/${id}`);
                if (!response.ok) {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
                const movieData = await response.json();
                nameList = [...nameList, movieData.name];
            }
            setMovieNames(nameList)

        } catch (error) {
            setName(null)
            // console.error('Error fetching movies:', error);
        }

    };

    const handleDelete = async () => {
        try {
            const response = await fetch(`http://localhost:3001/api/categories/${categoryId}`, {
                method: "DELETE"
            });
            if (response.status === 404 || response.status === 400) {
                const errorData = await response.json();
                alert(errorData.error || `Error: ${response.status}`);
                return;
            }
            if (!response.ok) {
                alert("try again")
                return
            }
            if (response.status === 204) {
                setCategoryId(null);
                alert("deleted successfully")
            }
        
        } catch (error) {
            console.error("Error updating movie:", error);
        }
    }


    const handleSubmit = async (e) => {
        e.preventDefault();
        if (!name) {
            alert("you must select a name");
            return;
        }
        const updatedCategory = {
            name,
            promoted,
            movieIds
        };
        //send POST
        try {
            const response = await fetch(`http://localhost:3001/api/categories/${categoryId}`, {
                method: "PATCH",
                headers: {
                "Content-Type": "application/json",
                },
                body: JSON.stringify(updatedCategory),
            });
            if (response.status === 404 || response.status === 400) {
                const errorData = await response.json();
                alert(errorData.error || `Error: ${response.status}`);
                return;
            }
            if (!response.ok) {
                alert("try again")
                return
            }
            if (response.status === 204) {
                alert("updated successfully")
            }
        
        } catch (error) {
            console.error("Error updating movie:", error);
        }
    };
    
    const handleDeleteMovie = (index) => {
        const updatedidlist = movieIds.filter((_, i) => i !== index);
        setMovieIds(updatedidlist);
        const updatednameslist = movieNames.filter((_, i) => i !== index);
        setMovieNames(updatednameslist);
        console.log(updatedidlist)
      };
    
    useEffect(() => {
        const updateMovies = async () => {
            if (currentId) {
                const response = await fetch(`http://localhost:3001/api/movies/${currentId}`, {
                    method: "GET",
                    headers: {
                    "Content-Type": "application/json",
                    },
                });
                if (response.status === 404 || response.status === 400) {
                    const errorData = await response.json();
                    alert(errorData.error || `Error: ${response.status}`);
                    return;
                }
                if (!response.ok) {
                    alert("try again")
                    return
                }
                console.log([...movieIds, currentId])
                setMovieIds([...movieIds, currentId]);
                setMovieNames([...movieNames, (await response.json()).name])
            }
        }

        updateMovies()
    }, [currentId]);

    useEffect(() => {
        fetchCategory();
    }, [categoryId]);
    
    if (!name) {
        return null;
    }

    return (
        <div className="categoryform">
            <form onSubmit={handleSubmit}>
                <div>
                <label htmlFor="name">Name:</label>
                <input
                    type="text"
                    id="name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                    required
                />
                </div>

                <div>
                <label htmlFor="promoted">Promoted:</label>
                <input
                    type="checkbox"
                    id="promoted"
                    checked={promoted}
                    onChange={(e) => setPromoted(e.target.checked)}
                />
                </div>

                <div>
                <label htmlFor="movies">Movies To Add:</label>
                <ul>
                    {movieNames.map((name, index) => (
                    <li key={index}>
                        {name} 
                        <button onClick={() => handleDeleteMovie(index)}>Delete</button>
                    </li>
                    ))}
                </ul>
                <MovieAutocomplete setId={setCurrentId}/>
                </div>

                <div className="button-container">
                    <button type="submit" className="save-button">Save Changes</button>
                    <button type="button" className="delete-button" onClick={handleDelete}>Delete</button>
                </div>
            </form>
        </div>
    )
};

export default CategoryEditor;