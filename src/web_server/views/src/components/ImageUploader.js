import React, { useState } from "react";

const ImageUploader = ({selectedFile, setSelectedFile}) => {

  const handleFileChange = (event) => {
    const file = event.target.files[0];
    setSelectedFile(file);
  };

  return (
    <div className="imageselector">
        <input
          type="file"
          accept="image/*"
          onChange={handleFileChange}
          id="fileInput"
        />
        {selectedFile && (
            <div>
            <p>Selected File: {selectedFile.name}</p>
            <img src = {URL.createObjectURL(selectedFile)}/>
            <button onClick={() => {
                const fileInput = document.querySelector('input[type="file"]');
                fileInput.value = null;
                setSelectedFile(null)
              }}>x</button>
            </div>
            )}
    </div>
  );
};

export default ImageUploader;