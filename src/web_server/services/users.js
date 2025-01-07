const User = require("../models/users")
const net = require('net');

const PORT = process.env.CPP_SERVER_PORT
const serverIP = process.env.CPP_SERVER_IP


const createUser = async (username, password, email, phoneNumber, picture) => {
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
  
    const savedUser = await user.save()
    // return the user that was created
    return savedUser
}

const getUserById = async (id) => {
    // return the user inside the db by his ID. His password won't be returned because of 'select = false' 
    return await User.findById(id)
}

module.exports = {getUserById, createUser }