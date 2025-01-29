/* this file will have some helper functions */

// this function will get a token and return the fields as a json of the user beind it or null if failed
const getUserFromToken = async (token) => {
    try {
        // verifying the token
        const res = await fetch(`http://localhost:3001/api/tokens/${token}`);
        // if userId not found or token is invalid return null
        if(!res.ok) return null;
        // else return the userId beind the token
        const json = await res.json();
        return json.userId;
    } catch (error) {
        // if there is no such user or the verification has failed
        return null;
    }
};

// helper function that get an image id and return a url for the image
const convertToURL = async (id) => {
    const res = await fetch(`http://localhost:3001/images/${id}`);
    if (!res.ok) return null;
  
    const image = await res.json();
  
    // Convert the image data (Blob) to base64 string using FileReader
    const base64Image = await new Promise((resolve, reject) => {
      const reader = new FileReader();
      reader.onloadend = () => {
        resolve(reader.result.split(',')[1]);  // Extract base64 string from data URL
      };
      reader.onerror = reject;
      
      // Create a Blob from the image data and read it as base64
      const blob = new Blob([new Uint8Array(image.data.data)], { type: image.contentType });
      reader.readAsDataURL(blob);  // Read the Blob as a data URL
    });
  
    // Construct the image URL
    const imageURL = `data:image/${image.contentType};base64,${base64Image}`;
    return imageURL;
  };
  
  

export default { getUserFromToken, convertToURL };