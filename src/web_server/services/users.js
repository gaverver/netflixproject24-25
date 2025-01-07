const User = require("../models/users")
const net = require('net');

const PORT = process.env.CPP_SERVER_PORT
const serverIP = process.env.CPP_SERVER_IP


const createUser = async (username, password, email, phoneNumber, picture, watched_movies) => {
    // check if a user with the same username/email/phone number as entered exists. If yes, then a new user won't be created in the db.
    const existingUser = await User.find({
        $or: [
            { username: username },
            { email: email },
            { phoneNumber: phoneNumber }
        ]
    });
    if (existingUser && existingUser.length > 0) {
        return null; // User already exists, returns null
    }
    // Create a new user with the given fields.
    const user = new User({ username: username, password: password, email: email, phoneNumber: phoneNumber})
    // if a picture was entered, switch to it.
    if (picture) {
        user.picture = picture
    }
    // if a list of watched movies was entered, switch to it.
    if (watched_movies) {
        user.watched_movies = watched_movies
    }
  
    const createRecommendationConnection = (userId, movieString) => {
        return new Promise((resolve, reject) => {
            // connect to the recommendation server at the serverIp and port in the config files.
            const client = net.createConnection({ host: serverIP, port: PORT }, () => {
                // add the user to the recommendation system.
                if (movieString === "") {
                    const movieId = '1'.repeat(24);
                    client.write(`POST ${userId} ${movieId}`);
                    client.write(`DELETE ${userId} ${movieId}`);
                } else {
                    client.write(`POST ${userId} ${movieString}`);
                }
            });

            client.on('data', (data) => resolve(data.toString()));
            client.on('error', (err) => reject(err));
        });
    };
    var recommendationResponse = null
    try {
        // get the response data from the cpp server
        recommendationResponse = await createRecommendationConnection(user.id, watched_movies.join(' '));
    } catch (error) {
        return null;
    }
    const savedUser = await user.save()
    // return the user that was created and the response of the recommendation system.
    return await [savedUser, recommendationResponse]
}

const getUserById = async (id) => {
    // return the user inside the db by his ID. His password won't be returned because of 'select = false' 
    return await User.findById(id)
}

module.exports = {getUserById, createUser }