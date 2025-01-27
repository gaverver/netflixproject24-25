import React, { useState } from "react";

const VideoUploader = ({selectedVideo, setSelectedVideo}) => {

  const handleFileChange = (event) => {
    const file = event.target.files[0];
    setSelectedVideo(file);
  };

  return (
    <div className="videoselector">
        <input
          type="file"
          accept="video/*"
          onChange={handleFileChange}
          id="fileInput"
        />
        {selectedVideo && (
            <div>
            <p>Selected File: {selectedVideo.name}</p>
            <video
            width="300"
            controls
            src={URL.createObjectURL(selectedVideo)}/>
            <button onClick={() => {
                const fileInput = document.querySelector('input[type="file"][accept="video/*"]');
                fileInput.value = null;
                setSelectedVideo(null)
            }}>x</button>
            </div>
            )}
    </div>
  );
};

export default VideoUploader;