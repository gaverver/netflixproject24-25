const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const User = new Schema({
    // the username of the user
    username : {
        type: String,
        required: true
    },
    // the password of the user
    password : {
        type: String,
        required: true,
        select: false // excludes password from queries done on users
    },
    // the email of the user
    email: {
        type: String,
        required: true
    },
    // the phone number of the user
    phoneNumber: {
        type: String,
        required: true
    },
    // the picture of the user, selected by a list of pictures
    picture: {
        type: String,
        default: "default.png"
    },
    // list of movie id's that were watched by the user
    watched_movies: {
        type: [Schema.Types.ObjectId], 
        ref: 'movies', // reference to the 'movies' collection
        default: []
    }
});

module.exports = mongoose.model('users', User);