import React, { useEffect, useState, useRef } from "react";
const VideoStreamer = ({ videoId, controls }) => {
    const [videoSrc, setVideoSrc] = useState("");
    const videoRef = useRef(null);

    useEffect(() => {
      const fetchVideo = async () => {
          const token = sessionStorage.getItem("jwt");
          if (!token) {
            console.error("No token found");
            //navigate home
            return;
          }

          try {
              const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/videos/watch/${videoId}`, {
                  method: "GET",
                  headers: {
                      Authorization: `Bearer ${token}`,
                      range: `bytes=0-`
                  },
              });
              if (!response.ok) {
                throw new Error("Failed to fetch video metadata");
              }

              const contentLength = response.headers.get("Content-Length");
              if (contentLength > 0) {
                  setVideoSrc(`http://localhost:${process.env.REACT_APP_PORT}/videos/watch/${videoId}`)
              }
          } catch (error) {
              console.error("Error fetching video:", error);
          }
      };

      if (videoId) {
        fetchVideo();
      }
    }, [videoId]);

    return (
      <div className="movie">
        {videoSrc ? (
          <video ref={videoRef} id="player" autoPlay controls={controls} controlsList="nodownload" muted="muted">
              <source src={videoSrc} />
          </video>
        ) : (
          <p>Loading video...</p>
        )}
      </div>
    );
};

export default VideoStreamer;
