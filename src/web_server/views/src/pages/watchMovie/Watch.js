import "./Watch.css";
import React from "react";
import { useParams } from "react-router";
import VideoStreamer from "../../components/VideoStreamer";
import { useNavigate } from "react-router-dom";

function Watch() {
    const {id} = useParams();
    const navigate = useNavigate();
    
    return (
        <div className="watchPage">
            <button
            className="btn btn-light position-absolute top-0 end-0 m-3 border rounded-circle shadow"
            id = "backhome"
            onClick={() => navigate("/")}
            >
            <i className="bi bi-arrow-left"></i>
            </button>
            <VideoStreamer videoId={id} controls={true}/>
        </div>
    );
}

export default Watch;
