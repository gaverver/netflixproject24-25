const movieService = require('../services/movies');
const mongoose = require('mongoose');

//helper function for validation of input
const validationCheck = (name, description, actors, published, age_limit, creators, categories, photo, res) => {
    
    //check the type of the arguments
    if (name !== undefined && typeof name !== 'string') {
        return res.status(400).json({ error: 'Invalid data: name must be a string' });
    }
    if (description !== undefined && typeof description !== 'string') {
        return res.status(400).json({ error: 'Invalid data: description must be a string' });
    }
    if (creators !== undefined && !Array.isArray(creators)) {
        return res.status(400).json({ error: 'Invalid data: creators must be an array' });
    }
    if (creators !== undefined && !creators.every(i => typeof i === 'string')) {
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
    try {
        //get the required parameters
        const {name, description, actors, published, age_limit, creators, categories, photo} = req.body;
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
        
        //validation of arguments
        const x = validationCheck(name, description, actors, published, age_limit, creators, categories, photo, res);
        if (x !== true) {
            return x;
        }

        const newMovie = await movieService.createMovie(name, description, actors, published, age_limit, creators, photo, categories);
        if (newMovie === null) {
            return res.status(404).json({ error: 'name and/or photo already exists' })
        }
        return res.status(201).set('Location', `/api/movies/${newMovie._id}`).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
    

}

const getMovies = async (req, res) => {
    //get the corresponding movies
    const userId = req.headers['userid'];

    if (!userId) {
        return res.status(400).json({ error: 'User ID is required in the headers' });
    }

    // Validate the user ID format
    if (!mongoose.Types.ObjectId.isValid(userId)) {
        return res.status(400).json({ error: 'Invalid User ID format' });
    }
    try {
        movies = await movieService.getMovies(userId);

        return res.status(200).json({ movies });
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

const getMovieById = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const id = req.params.id
        if (!mongoose.Types.ObjectId.isValid(id)) {
            return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
        }
        const movie = await movieService.getMovieById(id);
        
        // getMovieById in the services returns null if the category havn't found
        if (!movie) {
            return res.status(404).json({ error:'Movie not found' });
        }
        
        return res.status(200).json(movie);
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

const deleteMovie = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const id = req.params.id
        if (!mongoose.Types.ObjectId.isValid(id)) {
            return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
        }
        const movie = await movieService.deleteMovie(id);
        // deleteMovie in the services returns null if the category havn't found
        if (!movie) {
            return res.status(404).json({ error:'movie not found' });
        }
        return res.status(204).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

const getRecommendation = async (req, res) => {
    //validation check
    if (!mongoose.Types.ObjectId.isValid(req.headers['userid'])) {
        return res.status(400).json({ error: 'Invalid data: userId must contain valid ObjectId' });
    }
    if (!mongoose.Types.ObjectId.isValid(req.params.id)) {
        return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
    }
    //get response from the server
    try {
        const response = await movieService.getRecommendation(req.headers['userid'], req.params.id)
        const resStatus = response.substring(0, 3);
        if (resStatus === '200') {
            //return json of the ids
            return res.status(resStatus).json(JSON.stringify(response.replace(/^200 Ok\n\n/, '').split(' ')))
        } else {
            //return why it failed
            return res.status(resStatus).json({ error : response })
        }
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
    
}

const addMovieToUser = async (req, res) => {
    //validation check
    if (!mongoose.Types.ObjectId.isValid(req.headers['userid'])) {
        return res.status(400).json({ error: 'Invalid data: userId must contain valid ObjectId' });
    }
    if (!mongoose.Types.ObjectId.isValid(req.params.id)) {
        return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
    }
    try {
        const movie = await movieService.addMovieToUser(req.headers['userid'], req.params.id)
        if (!movie) {
            return res.status(404).json( {error : "user and/or movie doesn't exists"} )
        }
        if (movie === "user already added") {
            return res.status(404).json( {error : "404 user already added"} )
        }
        return res.status(204).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

const queryGet = async (req, res) => {
    try {
        const movies = await movieService.queryGet(req.params.query);
        res.status(200).json({ movies })
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

const updateMovie = async (req, res) => {
    const {name, description, actors, published, age_limit, creators, categories, photo} = req.body;
    const id = req.params.id;
    //check validation of id
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(400).json({ error: 'Invalid data: id must contain valid ObjectId' });
    }
    //validation check
    const x = validationCheck(name, description, actors, published, age_limit, creators, categories, photo, res);
    if (x !== true) {
        return x;
    }
    //update
    try {
        const movie = await movieService.updateMovie(id,name, description, actors, published, age_limit, creators, categories, photo);
        if (!movie) {
            return res.status(404).json({ error:'Movie not found' });
        }
        return res.status(204).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

module.exports = {createMovie, getMovies, getMovieById, deleteMovie, getRecommendation, addMovieToUser, queryGet, updateMovie}