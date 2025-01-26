import React, { useState, useEffect } from "react";
import MoviecatEditor from "./MoviecatEditor";

const MovieEditor = ({id}) => {
    const [name, setName] = useState('');
    const [description, setDescription] = useState('')
    const [actors, setActors] = useState([]);
    const [age_limit, setAge_limit] = useState('');
    const [creators, setCreators] = useState([]);
    const [categories, setCategories] = useState([]);
    const [photo, setPhoto] = useState('');

    const fetchMovie = async () => {
        try {
            const response = await fetch(`http://localhost:3001/api/movies/${id}`);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        
            const data = await response.json();
            setName(data.name)
            setDescription(data.description)
            setActors(data.actors)
            setAge_limit(data.age_limit)
            setCreators(data.creators)
            setCategories(data.categories)
            setPhoto(data.photo)

        } catch (error) {
            console.error('Error fetching movies:', error);
        }

    };


    const handleSubmit = async (e) => {
        e.preventDefault();
        const updatedMovie = {
            name,
            description,
            actors,
            age_limit,
            creators,
            categories,
            photo,
        };
    
        try {
            const response = await fetch(`http://localhost:3001/api/movies/${id}`, {
                method: "PUT",
                headers: {
                "Content-Type": "application/json",
                },
                body: JSON.stringify(updatedMovie),
            });
        
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        
            const data = await response.json();
            console.log("Movie updated successfully:", data);
        } catch (error) {
            console.error("Error updating movie:", error);
        }
    };

    useEffect(() => {
        if (id) {
        fetchMovie();
        }
    }, [id]);

    if (id === '')
        return null;

    return (
        <div className="movieeditor">
            <form onSubmit={handleSubmit}>
                <div>
                <label htmlFor="name">Name:</label>
                <input
                    type="text"
                    id="name"
                    value={name}
                    onChange={(e) => setName(e.target.value)}
                />
                </div>

                <div>
                <label htmlFor="description">Description:</label>
                <textarea
                    id="description"
                    value={description}
                    onChange={(e) => setDescription(e.target.value)}
                />
                </div>

                <div>
                <label htmlFor="actors">Actors (comma-separated):</label>
                <input
                    type="text"
                    id="actors"
                    value={actors.join(", ")}
                    onChange={(e) => setActors(e.target.value.split(",").map((a) => a.trim()))}
                />
                </div>

                <div>
                <label htmlFor="age_limit">Age Limit:</label>
                <input
                    type="number"
                    id="age_limit"
                    value={age_limit}
                    min="1"
                    max="18"
                    onChange={(e) => {
                        const value = parseInt(e.target.value, 10);
                        if (value >= 1 && value <= 18) {
                            setAge_limit(value);
                        } else if (value < 1) {
                            setAge_limit(1);
                        } else if (value > 18) {
                            setAge_limit(18);
                        }
                    }}
                />
                </div>

                <div>
                <label htmlFor="creators">Creators (comma-separated):</label>
                <input
                    type="text"
                    id="creators"
                    value={creators.join(", ")}
                    onChange={(e) => setCreators(e.target.value.split(",").map((c) => c.trim()))}
                />
                </div>

                <MoviecatEditor categories={ categories } setCategories={ setCategories } />

                <div>
                <label htmlFor="photo">Photo URL:</label>
                <input
                    type="text"
                    id="photo"
                    value={photo}
                    onChange={(e) => setPhoto(e.target.value)}
                />
                </div>

                <button type="submit">Save Changes</button>
            </form>
        </div>
    )


};

export default MovieEditor;