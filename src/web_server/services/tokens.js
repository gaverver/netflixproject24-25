// require the users modle, because I want to search for specific users
const User = require('../models/users');
const jwt = require('jsonwebtoken');

// returns a token for the user with the username and passowrd below or null if not exist
const createToken = async (user_name, pass) => {
    // it will return the first user he found with the username and password above, and null if he didn't found any
    const user = await User.findOne({ username: user_name, password: pass });
    // if the user doesn't exist, return null immediately
    if (!user) return null;
    // else, create the JWT for the user using his id and his privilege level and a secret key for security
    const token = jwt.sign(
        // the userId and his privilege level
        { userId: user._id , privilegeLevel: user.privilegeLevel},
        // secret key
        process.env.JWT_SECRET
    );
    return token;
};

// gets a token and return the id of the user beind it
const getIdFromToken = async (token) => {
    try {
        // verify the token
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        // check if the userId we got from the token is really exists
        const userId = decoded.userId;
        const user = await User.findById(userId);
        // if not return null
        if (!user) return null;
        // else return his id
        return userId;
    } catch (error) {
        return null;
    }
};

// helper functions

// this function is boolean and checks if a user is loggedIn
const isLoggedIn = async (token) => {
    try {
        // verify the token
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        // check if the userId we got from the token is really exists
        const userId = decoded.userId;
        const user = await User.findById(userId);
        // if not return false
        if (!user) return false;
        return true;
    } catch(error) {
        // if the verification failed
        return false;
    }
}

// this function will return false if the user isn't logged in and his privilegeLevel if he's loggedIn
const privilegeLevelByToken = async (token) => {
    try {
        // verify the token
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        // check if the userId we got from the token is really exists
        const userId = decoded.userId;
        const user = await User.findById(userId);
        // if not return false
        if (!user) return false;
        return decoded.privilegeLevel;
    } catch(error) {
        // if the verification failed
        return false;
    }
}

module.exports = {createToken, isLoggedIn ,privilegeLevelByToken, getIdFromToken}