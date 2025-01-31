import "./Watch.css";
import React from "react";
import { useNavigate, useParams } from "react-router";
import VideoStreamer from "../../components/VideoStreamer";
import Menu from "../../components/menu/menu";

function Watch() {
    const {id} = useParams();
    return (
        <div>
            <Menu />
            <VideoStreamer videoId={id} />
        </div>
    );
}

export default Watch;
