const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Category = new Schema({
    // the name of the category
    name : {
        type: String,
        required: true
    },
    // if the category promoted or not
    promoted: {
        type: Boolean,
        default: false
    },
    // a list of movies ids of the movies that belong to this category
    movieIds: {
        type: [Schema.Types.ObjectId],
        // reference to the 'movies' collection
        ref: 'movies',
        default: []
    }
});

// the name of the collection is categories
module.exports = mongoose.model('categories', Category);