import React, { useState, useEffect } from "react";
import MovieAutocomplete from "./MovieAutocomplete";

const CategoryCreator = () => {
    const [name, setName] = useState('');
    const [promoted, setPromoted] = useState(false)
    const [movieIds, setMovieIds] = useState([]);
    const [movieNames, setMovieNames] = useState([]);
    const [currentId, setCurrentId] = useState('')
    const token = sessionStorage.getItem("jwt");


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
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/categories`, {
                method: "POST",
                headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${token}`,
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
            if (response.status === 201) {
                alert("created successfully")
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
                const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${currentId}`, {
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

                <button type="submit" className="save-button">Create Category</button>

            </form>
        </div>
    )


};

export default CategoryCreator;