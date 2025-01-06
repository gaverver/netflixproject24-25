const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const User = new Schema({
    username : {
        type: String,
        required: true
    },
    password : {
        type: String,
        required: true
    },
    email: {
        type: String,
        required: true
    },
    phoneNumber: {
        type: String,
        required: true
    },
    picture: {
        type: String,
        default: "default.png"
    },
    watched_movies: {
        type: [Schema.Types.ObjectId],
        ref: 'movies',
        default: []
    }
});

module.exports = mongoose.model('users', User);