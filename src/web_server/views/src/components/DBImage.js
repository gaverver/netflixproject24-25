import React, { useEffect, useState } from 'react';

const MovieImage = ({ photo }) => {
  const [imageSrc, setImageSrc] = useState('');
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const fetchImage = async () => {
      try {
        const response = await fetch(`http://localhost:${process.env.REACT_APP_PORT}/images/${photo}`);
        if (!response.ok) {
          throw new Error('Image not found');
        }

        const imageJson = await response.json();
        const { data, contentType } = imageJson;
        const base64String = uint8ArrayToBase64(new Uint8Array(data.data));
        const imageUrl = `data:${contentType};base64,${base64String}`;

        setImageSrc(imageUrl);
        setLoading(false);
      } catch (error) {
        console.error('Error loading image:', error);
        setLoading(false);
      }
    };

    fetchImage();

  }, [photo]);

  const uint8ArrayToBase64 = (uint8Array) => {
    let binaryString = '';
    for (let i = 0; i < uint8Array.length; i++) {
      binaryString += String.fromCharCode(uint8Array[i]);
    }
    return window.btoa(binaryString);
  };

  return (
    <div>
      {loading ? (
        <p>Loading image...</p>
      ) : (
        <img src={imageSrc} alt="Movie" />
      )}
    </div>
  );
};

export default MovieImage;
