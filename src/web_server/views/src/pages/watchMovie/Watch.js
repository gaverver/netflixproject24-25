// import "./Watch.css";
import React from "react";
import { useNavigate, useParams } from "react-router";
import VideoStreamer from "../../components/VideoStreamer";

function Watch() {
    const {id} = useParams();
    return (
        <VideoStreamer videoId={id} />
    );
}

export default Watch;
