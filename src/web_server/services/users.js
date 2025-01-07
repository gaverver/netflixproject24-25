const User = require("../models/users")
const net = require('net');

const PORT = process.env.CPP_SERVER_PORT
const serverIP = process.env.CPP_SERVER_IP


const createUser = async (username, password, email, phoneNumber, picture, watched_movies) => {
    // check if a user with the same username as entered is exists. If yes, then a new user won't be created in the db.
    const findUser = await db.users.find({username: username})
    if (findUser && findUser.length > 0) {
        return null
    }
    // check if a user with the same email as entered is exists. If yes, then a new user won't be created in the db.
    const findEmail = await db.users.find({email: email})
    if (findEmail && findEmail.length > 0) {
        return null
    }
    // check if a user with the same phoneNumber as entered is exists. If yes, then a new user won't be created in the db.
    const findPhoneNumber = await db.users.find({phoneNumber: phoneNumber})
    if (findPhoneNumber && findPhoneNumber.length > 0) {
        return null
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
    // connect to the recommendation server at the serverIp and port in the config files.
    const client = net.createConnection({ host: serverIP, port: PORT }, () => {
        // add the user to the recommendation system.
        const movieString = watched_movies.join(' ')
        if (movieString == "") {
            const movieId = '1'.repeat(24)
            client.write(`POST ${user.id} ${movieId}`)
            client.write(`DELETE ${user.id} ${movieId}`)
        }
        client.write(`POST ${user.id} ${movieString}`)
    })
    
    const recommendationPromise = new Promise((resolve, reject) => {
        // connect to the cpp recommendation system
        const client = net.createConnection({ host: serverIP, port: PORT }, () => {
            // add the user and the watched movies to the system.
            const movieString = watched_movies.join(' ');
            if (movieString === "") {
                const movieId = '1'.repeat(24);
                client.write(`POST ${user.id} ${movieId}`);
                client.write(`DELETE ${user.id} ${movieId}`);
            }
            client.write(`POST ${user.id} ${movieString}`);
        });

        client.on('data', (data) => {
            resolve(data.toString()); // Resolve the promise with the response data
        });

        client.on('error', (err) => {
            reject(err); // Reject the promise on error
        });
    });
    var recommendationResponse = null
    try {
        // get the response data from the cpp server
        recommendationResponse = await Promise.all(recommendationPromise);
    } catch (error) {
        return null;
    }

    // return the user that was created and the response of the recommendation system.
    return await [user.save(), recommendationResponse]
}

const getUserById = async (id) => {
    // return the user inside the db by his ID. His password won't be returned because of 'select = false' 
    return await User.findById(id)
}

module.exports = {getUserById, createUser }