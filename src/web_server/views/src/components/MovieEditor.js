import React, { useState, useEffect } from "react";
import MoviecatEditor from "./MoviecatEditor";
import StringListEditor from "./StringListEditor";
import "./MovieEditor.css"
import DBImage from "./DBImage";
import ImageUploader from "./ImageUploader";

const MovieEditor = ({id}) => {
    const [name, setName] = useState('');
    const [description, setDescription] = useState('')
    const [actors, setActors] = useState([]);
    const [age_limit, setAge_limit] = useState('');
    const [creators, setCreators] = useState([]);
    const [categories, setCategories] = useState([]);
    const [photo, setPhoto] = useState(null);
    const [selectedFile, setSelectedFile] = useState(null);

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
            photo
        };
        //uploading image first
        const previousPhoto = photo;
        if (selectedFile) {
            try {
                const response = await fetch("http://localhost:3001/images", {
                method: "POST",
                headers: {
                    "Content-Type": "application/octet-stream",
                },
                body: new Blob([selectedFile]),
                });
        
                if (!response.ok) {
                    alert("Failed to upload the file.");
                    return;
                }
                if (response.status === 201) {
                    const locationHeader = response.headers.get("Location");
                    if (locationHeader) {
                        const locationId = locationHeader.split("/").pop();
                        setPhoto(locationId)
                        updatedMovie.photo = locationId
                        setSelectedFile(null)
                    }
                }

            } catch (error) {
                alert("Failed to upload the file.");
                return;
            }
        }
        //send put
    
        try {
            const response = await fetch(`http://localhost:3001/api/movies/${id}`, {
                method: "PUT",
                headers: {
                "Content-Type": "application/json",
                },
                body: JSON.stringify(updatedMovie),
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
                if (selectedFile) {
                    const response2 = await fetch(`http://localhost:3001/images/${previousPhoto}`, {
                        method: "DELETE"
                    })   
                }
                alert("update successfully")
            }
        
        } catch (error) {
            console.error("Error updating movie:", error);
        }
        fetchMovie()
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

                <StringListEditor list = {actors} setList = {setActors} label = {"Actors"} placeholder = {"New Actor"} itemName = {"actor"} />

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


                <StringListEditor list = {creators} setList = {setCreators} label = {"Creators"} placeholder = {"New Creator"} itemName = {"creator"} />

                <MoviecatEditor categories={ categories } setCategories={ setCategories } />

                <div>
                <label htmlFor="photo">Photo:</label>
                <DBImage photo={photo} />
                <ImageUploader selectedFile = {selectedFile} setSelectedFile = {setSelectedFile} />
                </div>

                <button type="submit">Save Changes</button>
            </form>
        </div>
    )


};

export default MovieEditor;