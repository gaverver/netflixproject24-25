import "./Watch.css";
import React, { useEffect, useState } from "react";
import { useParams } from "react-router";
import VideoStreamer from "../../components/VideoStreamer";
import { useNavigate } from "react-router-dom";

function Watch() {
    const {id} = useParams();
    const navigate = useNavigate();
    const [videoId, setVideoId] = useState("")
    

    const sign = async () => {
        const token = sessionStorage.getItem("jwt");
        if (!token) {
            console.error("No token found");
            //navigate home
            navigate("/home")
            return;
        }
        const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/tokens/${token}`);
        if (!response.ok) {
          console.error("token isn't valid");
          //navigate home
          navigate("/home")
          return;
        }
        const data = await response.json();
        const userId = data.userId;
        const response2 = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}/recommend`, {
            method: "POST",
            headers: {
            "userid": `${userId}`,
            'Authorization': `Bearer ${token}`
            },
        });
        // const data2 = await response2.json();
        // console.log(data2)
    }

    const fetchVideo = async () => {
        const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/api/movies/${id}`);
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
    
        const data = await response.json();

        setVideoId(data.video)
    }

    useEffect(() => {
        fetchVideo();
        sign();
    }, []);

    return (
        <div className="watchPage">
            <button
            className="btn btn-light position-absolute top-0 end-0 m-3 border rounded-circle shadow"
            id = "backhome"
            onClick={() => navigate("/")}
            >
            <i className="bi bi-arrow-left"></i>
            </button>
            <VideoStreamer videoId={videoId} controls={true}/>
        </div>
    );
}

export default Watch;
