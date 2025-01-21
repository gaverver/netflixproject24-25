const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Movie = new Schema({
    name : {
        type: String,
        required: true
    },
    description : {
        type: String,
        required: true
    },
    actors: {
        type: [String],
        default: []
    },
    published: {
        type: Date,
        default: Date.now
    },
    age_limit: {
        type: Number,
        default: 0,
        min: 0,
        max: 18
    },
    creators: {
        type: [String],
        required: true
    },
    categories: {
        type: [Schema.Types.ObjectId],
        ref: 'categories',
        default: []
    },
    users: {
        type: [Schema.Types.ObjectId],
        ref: 'users',
        default: []
    },
    photo: {
        type: Schema.Types.ObjectId,
        ref: 'images',
        required: true
    },
    video: {
        type: String,
        required: true,
        select: false
    }
});

module.exports = mongoose.model('movies', Movie);