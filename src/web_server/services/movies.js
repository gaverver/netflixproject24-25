const Movie = require('../models/movies')
const categoriesService = require('./categories')
const usersService = require('./users')
const App = require('../app')
const net = require('net')
const mongoose = require('mongoose');
//functio to create a movie
const createMovie = async (name, description, actors, published, age_limit, creators, categories, photo) => {
    //check if there's a movie with the same name or photo
    const existingMovie = await Movie.find({
        $or: [
            { name: name },
            { photo: photo }
        ]
    })
    if (existingMovie && existingMovie.length > 0) {
        return null
    }
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
        for (const categoryId of categories) {
            await categoriesService.addMovieToCategory(categoryId, movie._id);
        }
    }
    if (photo !== undefined) {
        movie.photo = photo
    }
    return await movie.save()
}
//function to delete a movie
const deleteMovie = async (id) => {
    //delete from mongoDB
    const movie = await Movie.findByIdAndDelete(id);
    if (!movie) return null;
    //delete movie from users
    const [ip, port] = App.recommendConString.split(':');
    for (const userId of movie.users) {
        const response = await sendMessageToServer(ip, parseInt(port), `DELETE ${userId} ${id}`);
        await usersService.deleteMovieFromUser(userId, id);
    }
    //delete from category
    for (const categoryId of movie.categories) {
        await categoriesService.deleteMovieFromCategory(categoryId, id);
    }

    return movie;
}
//function to get a movie by its id
const getMovieById = async (id) => {
    return await Movie.findById(id);
}
//function to get promoted catrgorie's movies
const getMovies = async (userId) => {
    //get the user
    user = usersService.getUserById(userId);
    watchedMovies = user.watched_movies;
    //get all categories
    const categories = await categoriesService.getCategories()
    //list of all the movies that will be returned
    const allMovies = {};
    //iterating over the categories
    for (const category of categories) {
        if (category.promoted) {
            //shuffle the movies in this category
            const randomMovies = category.movieIds.sort((a,b) => Math.random() - 0.5)
            //get the 20 movies
            const movies = randomMovies.filter(movieId => !watchedMovies.includes(movieId)).slice(0,20);
            //add the movies to the list
            allMovies[category.name] = movies;
        }
    }
    //add the special category
    const lastMovies = watchedMovies.slice(-20);
    const randomLastMovies = lastMovies.sort((a,b) => Math.random() - 0.5);
    allMovies["Recent watch movies"] = randomLastMovies;
    return allMovies;
}
//function to update a movie
const updateMovie = async (id, name, description, actors, published, age_limit, creators, categories, photo) => {
    movie = getMovieById(id);
    if (!movie) return null;
    movie.name = name;
    movie.description = description;
    movie.actors = actors;
    if (published !== undefined) {
        movie.published = published;
    }
    if (age_limit !== undefined) {
        movie.age_limit = age_limit;
    }
    if (creators === undefined) {
        movie.creators = [];
    } else {
        movie.creators = creators;
    }
    if (movie.categories && movie.categories.length > 0) {
        for (categoryId of movie.categories) {
            await categoriesService.deleteMovieFromCategory(categoryId, id);
        }
    }
    if (categories === undefined) {
        movie.categories = [];
    } else {
        movie.categories = categories;
        for (categoryId of categories) {
            categoriesService.addMovieToCategory(categoryId, id);
        }
    }
    if (photo !== undefined) {
        movie.photo = photo;
    }
    
    return await movie.save();
}
//function to get recommendation for movies
const getRecommendation = async (userId, movieId) => {
    const [ip, port] = App.recommendConString.split(':');
    const response = await sendMessageToServer(ip, parseInt(port), `GET ${userId} ${movieId}`);
    return response;
}
//add a movie to user
const addMovieToUser = async (userId, movieId) => {

    //check that the user and the movie exists
    const movie = getMovieById(movieId)
    if (!movie) {
        return null
    }
    const user = usersService.getUserById(userId)
    if (!user) {
        return null
    }
    //add to mongo
    const updatedMovie = await Movie.findByIdAndUpdate(
        movieId, 
        { $push: { users: userId } }, 
        { new: true }
    );
    if (!updatedMovie) return null;
    const updatedUser = await usersService.addMovieToUser(userId, movieId)
    if (!updatedUser) return null;
    //add to the recommendation system
    const [ip, port] = App.recommendConString.split(':');
    //try to post
    const response = await sendMessageToServer(ip, parseInt(port), `POST ${userId} ${movieId}`);
    //if user already exists in the system, use patch
    if (response == "404 Not Found") {
        await sendMessageToServer(ip, parseInt(port), `PATCH ${userId} ${movieId}`);
    }

    return updatedMovie;
}
//function to get movies that applies a query
const queryGet = async (query) => {
    const movies = await Movie.find({
        $or: [
          { name: { $regex: `.*${query}.*`, $options: 'i' } },
          { description: { $regex: `.*${query}.*`, $options: 'i' } },
          { actors: { $elemMatch: { $regex: `.*${query}.*`, $options: 'i' } } },
          { creators: { $elemMatch: { $regex: `.*${query}.*`, $options: 'i' } } }
        ]
      });
      return movies;
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

const deleteCategoryFromMovie = async (movieId, categoryId) => {
    const movie = await getMovieById(movieId);
    movie.categories = movie.categories.filter(id => id !== categoryId);
    await movie.save();
}

const addCategoryToMovie = async (movieId, categoryId) => {
    const movie = await getMovieById(movieId);
    if (!movie.categories.includes(categoryId)) {
        movie.categories.push(categoryId);
    }
    await movie.save();
}

module.exports = {createMovie, deleteMovie, getMovieById, getMovies, updateMovie, getRecommendation, addMovieToUser, queryGet, deleteCategoryFromMovie, addCategoryToMovie}