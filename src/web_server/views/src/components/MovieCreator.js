import React, { useState, useEffect } from "react";
import MoviecatEditor from "./MoviecatEditor";
import StringListEditor from "./StringListEditor";
import "./MovieEditor.css"
import ImageUploader from "./ImageUploader";
import VideoUploader from "./VideoUploader";

const MovieCreator = () => {
    const [name, setName] = useState('');
    const [description, setDescription] = useState('')
    const [actors, setActors] = useState([]);
    const [age_limit, setAge_limit] = useState('');
    const [creators, setCreators] = useState([]);
    const [categories, setCategories] = useState([]);
    const [selectedPhoto, setSelectedPhoto] = useState(null);
    const [selectedVideo, setSelectedVideo] = useState(null);
    const token = sessionStorage.getItem("jwt");

    const handleSubmit = async (e) => {
        e.preventDefault();
        if (!selectedPhoto) {
            alert("you must upload a photo");
            return;
        }
        if (!selectedVideo) {
            alert("you must upload a video");
            return;
        }
        const updatedMovie = {
            name,
            description,
            actors,
            age_limit,
            creators,
            categories,
            // photo,
            // video
        };
        //uploading image first
        try {
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/images`, {
            method: "POST",
            headers: {
                "Content-Type": "application/octet-stream",
                'Authorization': `Bearer ${token}`,
            },
            body: new Blob([selectedPhoto]),
            });
    
            if (!response.ok) {
                alert("Failed to upload the image.");
                return;
            }
            if (response.status === 201) {
                const locationHeader = response.headers.get("Location");
                if (locationHeader) {
                    const locationId = locationHeader.split("/").pop();
                    updatedMovie.photo = locationId
                    setSelectedPhoto(null)
                }
            }
        } catch (error) {
            alert("Failed to upload the image.");
            return;
        }
        //uploading video
        const formData = new FormData();
        formData.append("video", selectedVideo);

        try {
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/videos`, {
                method: "POST",
                body: formData,
                headers: {
                    'Authorization': `Bearer ${token}`,
                },
            });

            if (!response.ok) {
                alert("Failed to upload the video.");
                return;
            }
            if (response.status === 201) {
                const locationHeader = response.headers.get("Location");
                if (locationHeader) {
                    const locationId = locationHeader.split("/").pop();
                    updatedMovie.video = locationId
                    setSelectedVideo(null)
                }
            }

        } catch (error) {
            alert("Failed to upload the video.");
            return
        }
        //send POST
        try {
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies`, {
                method: "POST",
                headers: {
                "Content-Type": "application/json",
                'Authorization': `Bearer ${token}`,
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
            if (response.status === 201) {
                alert("created successfully")
            }
        
        } catch (error) {
            console.error("Error updating movie:", error);
        }
    };

    return (
        <div className="movieform">
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
                <label htmlFor="description">Description:</label>
                <textarea
                    id="description"
                    value={description}
                    onChange={(e) => setDescription(e.target.value)}
                    required
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
                <ImageUploader selectedFile = {selectedPhoto} setSelectedFile = {setSelectedPhoto} />
                </div>
                
                <div>
                <label htmlFor="video">Video:</label>
                <VideoUploader selectedVideo = {selectedVideo} setSelectedVideo = {setSelectedVideo} />
                </div>

                <button type="submit" className="save-button">Create Movie</button>

            </form>
        </div>
    )


};

export default MovieCreator;