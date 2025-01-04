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
        required: true,
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
    image: {
        type: Schema.Types.ObjectId,
        ref: 'images',
        required: true
    }
});

module.exports = mongoose.model('movies', Movie);