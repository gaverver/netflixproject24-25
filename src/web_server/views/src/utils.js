/* this file will have some helper functions */
import { verify } from 'jsonwebtoken';

// this function will get a token and return the fields as a json of the user beind it or null if failed
const getUserFromToken = async (token) => {
    try {
        // verifying the token
        const decode = verify(token, process.env.JWT_SECRET);
        // gets the user id
        const userId = decode.userId;
        // find him in the database
        const user = await User.findById(userId);
        // if the user is not found, return null
        // else return his fields values in a json
        return user || null;
    } catch (error) {
        // if there is no such user or the verification has failed
        return null;
    }
}

// helper function that get an image id and return a url for the image
const convertToURL = async (id) => {
    const res = await fetch(`http://localhost:3001/images/${id}`);
    // if image not found return null
    if(!res.ok) return null;
    // else convert to url
    const image = await res.json();
    const base64Image = image.data.toString("base64");
    const imageURL = `data:image/${image.contentType};base64,${base64Image}`;
    return imageURL;
}

export default { getUserFromToken, convertToURL };