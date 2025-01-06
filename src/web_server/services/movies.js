const Movie = require('../models/movies')
const categoriesService = require('categories')
const App = require('../app')
const net = require('net')
const mongoose = require('mongoose');

const createMovie = async (name, description, actors, published, age_limit, creators, categories) => {
    //create movie with required fields
    const movie = new Movie({
        name: name,
        description: description,
        creators: creators,
    })
    //check if 'actors' passed
    if (actors !== undefined) {
        movie.actors = actors
    }
    //check if 'published' passed
    if (published !== undefined) {
        movie.published = published
    }
    //check if 'age_limit' passed
    if (age_limit !== undefined) {
        movie.age_limit = age_limit
    }
    //check if 'categories' passed
    if (categories !== undefined) {
        movie.categories = categories
    }
    return await movie.save()
}

const deleteMovie = async (id) => {
    //delete from mongoDB
    const movie = await Movie.findByIdAndDelete(id);
    if (!movie) return null;
    //delete movie from recommendation system
    const [ip, port] = App.recommendConString.split(':');
    for (const userId of movie.users) {
        const response = await sendMessageToServer(ip, parseInt(port), `DELETE ${userId} ${id}`);
    }

    return movie;
}

const getMovieById = async (id) => {
    return await Movie.findById(id);
}

const getMovies = async (userId) => {
    //get all categories
    const categories = await categoriesService.getCategories()
    //list of all the movies that will be returned
    const allMovies = [];
    for (const category of categories) {
        if (category.promoted) {
            const movies = await Movie.find({
                categories: { $in: [category._id] },
                users: { $ne: mongoose.Types.ObjectId(userId)}
            }).limit(20)
            //add the movies to the list
            allMovies.push({
                category: category.name,
                categoryId: category._id,
                movies
              });
        }
    }

    return allMovies;
}

const updateMovie = async (id, name, description, actors, published, age_limit, creators, categories) => {
    movie = getMovieById(id);
    if (!movie) return null;
    movie.name = name;
    movie.description = description;
    movie.actors = actors;
    movie.published = published;
    movie.age_limit = age_limit;
    movie.creators = creators;
    movie.categories = categories;
    
    return await movie.save();
}

const getRecommendation = async (userId, movieId) => {
    const [ip, port] = App.recommendConString.split(':');
    const response = await sendMessageToServer(ip, parseInt(port), `GET ${userId} ${movieId}`);
    return response;
}

const addMovieToUser = async (userId, movieId) => {
    //add to mongo
    const updatedMovie = await Movie.findByIdAndUpdate(
        movieId, 
        { $push: { users: userId } }, 
        { new: true }
    );
    if (!updateMovie) return null;
    //add to the recommendation system
    const [ip, port] = App.recommendConString.split(':');
    //try to post
    const response = await sendMessageToServer(ip, parseInt(port), `POST ${userId} ${movieId}`);
    //if user already exists in the system, use patch
    if (response == "404 Not Found") {
        await sendMessageToServer(ip, parseInt(port), `PATCH ${userId} ${movieId}`);
    }

    return updateMovie;
}


//helper function to send a message to the server and wait for a response
const sendMessageToServer = (ip, port, message) => {
    return new Promise((resolve, reject) => {
        const client = new net.Socket();

        client.connect(port, ip, () => {
            client.write(message);
        });

        client.on('data', (data) => {
            resolve(data.toString());
            client.destroy();
        });

        client.on('error', (error) => {
            reject(error);
        });

    });
};