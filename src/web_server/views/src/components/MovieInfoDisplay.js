import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import VideoStreamer from './VideoStreamer'
import "./MovieInfoDisplay.css"
import MoviePic from "./moviePic/moviePic"

const MovieInfoDisplay = ({id, userId}) => {
    const navigate = useNavigate();
    const [name, setName] = useState('');
    const [description, setDescription] = useState('')
    const [actors, setActors] = useState([]);
    const [age_limit, setAge_limit] = useState('');
    const [creators, setCreators] = useState([]);
    const [categories, setCategories] = useState([]);
    const [video, setVideo] = useState(null);
    const [recommendMovies,setRecommendMovies] = useState([]);

    const play = () => {
        navigate(`/movies/watch/${id}`)
    }

    const fetchMovie = async () => {
        try {
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}`);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
        
            const data = await response.json();
            setName(data.name)
            setDescription(data.description)
            setActors(data.actors)
            setAge_limit(data.age_limit)
            setCreators(data.creators)
            for (const category of data.categories) {
                const response2 = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/categories/${category}`);
                const data2 = await response2.json();
                if (!response2.ok || !data2.name) {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
                setCategories((prevCategories) => [...prevCategories, data2.name]);
            }
            setVideo(data.video)

        } catch (error) {
            console.error('Error fetching movies:', error);
            navigate("/serverError");
        }

    };

    const fetchRecommendation = async () => {
        try {
            const token = sessionStorage.getItem("jwt");
            if (!token) {
            console.error("No token found");
            //navigate home
            // navigate("/home")
            return;
            }
            console.log(userId)
            const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}/recommend`, {
                method: "GET",
                headers: {
                "userid": `${userId}`,
                'Authorization': `Bearer ${token}`
                },
            });
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            const data = await response.json();
            // console.log(data)
            if (!(data.length === 0 || (data.length === 1 && data[0] === ''))) {
                setRecommendMovies(data);
            }
        } catch (error) {
            console.error('Error fetching movies:', error);
            // navigate("/serverError");
        }
    }

    useEffect(() => {
        if (id) {
        fetchMovie();
        }
    }, [id]);

    useEffect(() => {
        if (userId) {
        fetchRecommendation();
        }
    }, [userId]);

    if (id === '')
        return null;

    return (
        <div className="movieDisplay">
            <VideoStreamer videoId={video} controls={false}/>
            <div className="random-movie-info">
                <div className="movieButtons">
                    <button className="play-random-movie" onClick={play}>
                        Play
                    </button>
                </div>
            </div>
            <h1>{name}</h1>
            <p><strong>Description:</strong> {description}</p>
            <p><strong>Age Limit:</strong> {age_limit}</p>
            <h3>Actors</h3>
            <ul>
                {actors.map((actor, index) => (
                    <li key={index}>{actor}</li>
                ))}
            </ul>
            <h3>Creators</h3>
            <ul>
                {creators.map((creator, index) => (
                    <li key={index}>{creator}</li>
                ))}
            </ul>
            <h3>Categories</h3>
            <ul>
                {categories.map((category, index) => (
                    <li key={index}>{category}</li>
                ))}
            </ul>
            <ul>
                {recommendMovies.map((recommendation, index) => (
                    <li key={index}>
                        <MoviePic id={recommendation} />
                    </li>
                ))}
            </ul>
        </div>
    );    
    
};

export default MovieInfoDisplay;