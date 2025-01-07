const movieService = require('../services/movies');
const mongoose = require('mongoose');

//function for creating a movie
const createMovie = async (req, res) => {
    //get the required parameters
    const {name, description, actors, published, age_limit, creators, categories} = req.body;
    //validation of arguments

    //check that the required arguments passed
    if (name === undefined) {
        return res.status(400).json({ error:'Name is required' });
    }
    if (description === undefined) {
        return res.status(400).json({ error:'Description is required' });
    }
    if (creators === undefined) {
        return res.status(400).json({ error:'Creators is required' });
    }
    
    //check the type of the arguments
    if (typeof name !== 'string') {
        return res.status(400).json({ error: 'Invalid data: name must be a string' });
    }
    if (typeof description !== 'string') {
        return res.status(400).json({ error: 'Invalid data: description must be a string' });
    }
    if (!Array.isArray(creators)) {
        return res.status(400).json({ error: 'Invalid data: creators must be an array' });
    }
    if (!creators.every(i => typeof i === 'string')) {
        return res.status(400).json({ error: 'Invalid data: creators must contain valid ObjectIds' });
    }
    if (published !== undefined && isNaN(new Date(published).getTime())) {
        return res.status(400).json({ error: 'Invalid data: published must be a valid date' });
    }
    if (age_limit !== undefined && (typeof age_limit !== 'number' || age_limit < 0 || age_limit > 18)) {
        return res.status(400).json({ error: 'Invalid data: age_limit must be a number between 0 and 18' });
    }

    if (actors !== undefined) {
        if (!Array.isArray(actors)) {
            return res.status(400).json({ error: 'Invalid data: actors must be an array' });
        }
        if (!actors.every(actor => typeof actor === 'string')) {
            return res.status(400).json({ error: 'Invalid data: actors must contain valid ObjectIds' });
        }
    }

    if (categories !== undefined) {
        if (!Array.isArray(categories)) {
            return res.status(400).json({ error: 'Invalid data: categories must be an array' });
        }
        if (!categories.every(id => mongoose.Types.ObjectId.isValid(id))) {
            return res.status(400).json({ error: 'Invalid data: categories must contain valid ObjectIds' });
        }
    }

    await movieService.createMovie(name, description, actors, published, age_limit, creators, categories);
    return res.status(201).end();

}

const getMovies = async (req, res) => {
    //get the corresponding movies
    const userId = req.headers['userId']; // Replace 'user-id' with the actual header name used

    if (!userId) {
        return res.status(400).json({ error: 'User ID is required in the headers' });
    }

    // Validate the user ID format (assuming it's a MongoDB ObjectId)
    if (!mongoose.Types.ObjectId.isValid(userId)) {
        return res.status(400).json({ error: 'Invalid User ID format' });
    }
    movies = movieService.getMovies(userId);

    res.st
}