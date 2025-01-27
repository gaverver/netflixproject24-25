// components/ImageUploader.js
import React, { useRef, useState } from "react";

const ImageUploader = ({ onImageUpload }) => {
    const imageInput = useRef(null);
    const [imageUploaded, setImageUploaded] = useState(false);

    const handlePictureChange = (event) => {
        const file = event.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (e) => {
                // change the src of the img tag to the image that the user entered
                if (imageInput.current) {
                    imageInput.current.src = e.target.result;
                    // display the image as a block
                    imageInput.current.style.display = "block";
                    // update that the image was uploaded
                    setImageUploaded(true);
                    onImageUpload(e.target.result);
                }
            };
            reader.readAsDataURL(file);
        }
    };

    const removeImage = () => {
        if (imageInput.current) {
            // change the src and display of the image so it will not show it
            imageInput.current.src = "";
            imageInput.current.style.display = "none";
            setImageUploaded(false);
            onImageUpload(null);
            // remove the image from the selection
            const fileInput = document.querySelector('input[type="file"]');
            fileInput.value = null;

        }
    };
    // return the html(view) of the image uploader
    return (
        <div>
            <input type="file" accept="image/*" onChange={handlePictureChange} />
            <img ref={imageInput} alt="Preview" style={{ display: "none", maxWidth: "400px" }} />
            {imageUploaded && <button type="button" onClick={removeImage}>Remove Image</button>}
        </div>
    );
};

export default ImageUploader;
