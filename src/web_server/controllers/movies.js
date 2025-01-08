const movieService = require('../services/movies');
const mongoose = require('mongoose');

//helper function for validation of input
const validationCheck = (name, description, actors, published, age_limit, creators, categories, photo, res) => {
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

    if (photo !== undefined && !mongoose.Types.ObjectId.isValid(photo)) {
        return res.status(400).json({ error: 'Invalid data: photo must contain valid ObjectId' })
    }
    return true;
}

//function for creating a movie
const createMovie = async (req, res) => {
    //get the required parameters
    const {name, description, actors, published, age_limit, creators, categories, photo} = req.body;
    //validation of arguments

    const x = validationCheck(name, description, actors, published, age_limit, creators, categories, photo, res);
    if (x !== true) {
        return x;
    }

    await movieService.createMovie(name, description, actors, published, age_limit, creators, photo, categories);
    return res.status(201).end();

}

const getMovies = async (req, res) => {
    //get the corresponding movies
    const userId = req.headers['userId'];

    if (!userId) {
        return res.status(400).json({ error: 'User ID is required in the headers' });
    }

    // Validate the user ID format
    if (!mongoose.Types.ObjectId.isValid(userId)) {
        return res.status(400).json({ error: 'Invalid User ID format' });
    }
    movies = await movieService.getMovies(userId);

    return res.status(200).json({ movies });
}

const getMovieById = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const movie = await movieService.getMovieById(req.params.id);
        
        // getMovieById in the services returns null if the category havn't found
        if (!movie) {
            return res.status(404).json({ error:'Movie not found' });
        }
        
        return res.status(200).json(movie);
    } catch (error) {
        return res.status(404).json({ error:'Movie not found' });
    }
}

const deleteMovie = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const movie = await movieService.deleteMovie(req.params.id);
        // deleteMovie in the services returns null if the category havn't found
        if (!movie) {
            return res.status(404).json({ error:'movie not found' });
        }
        return res.status(204).end();
    } catch (error) {
        return res.status(404).json({ error:'Movie not found' });
    }
}

const getRecommendation = async (req, res) => {
    //get response from the server
    const response = await movieService.getRecommendation(req.headers['userId'], req.params.id)
    const resStatus = response.substring(0, 3);
    if (stat === '200') {
        //return json of the ids
        return res.status(resStatus).json(JSON.stringify(response.replace(/^200 Ok\n\n/, '').split(' ')))
    } else {
        //return why it failed
        return res.status(resStatus).json({ error : response })
    }
}

const addMovieToUser = async (req, res) => {
    const movie = movieService.addMovieToUser(req.headers['userId'], req.params.id)
    if (!movie) {
        return res.status(404).json( {error : "404 not found"} )
    }
    return res.status(204).end();
}

const queryGet = async (req, res) => {
    const movies = await movieService.queryGet(req.params.query);
    res.status(200).json({ movies })
}

const updateMovie = async (req, res) => {
    const {name, description, actors, published, age_limit, creators, categories, photo} = req.body;
    const id = req.body.id;
    //validation check
    const x = validationCheck(name, description, actors, published, age_limit, creators, categories, photo, res);
    if (x !== true) {
        return x;
    }
    //check validation of id
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
    }
    //update

    const movie = await movieService.updateMovie(id,name, description, actors, published, age_limit, creators, categories, photo);
    if (!movie) {
        return res.status(404).json({ error:'Movie not found' });
    }
    return res.status(204).end();
}