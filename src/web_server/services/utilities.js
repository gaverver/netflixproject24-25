const Movie = require('../models/movies')
const Category = require('../models/categories');
const User = require("../models/users")
//rmove a category from a movie
const deleteCategoryFromMovie = async (movieId, categoryId) => {
    const movie = await Movie.findById(id);
    movie.categories = movie.categories.filter(id => id !== categoryId);
    await movie.save();
}
//add a category to a movie
const addCategoryToMovie = async (movieId, categoryId) => {
    const movie = await Movie.findById(id);
    if (!movie.categories.includes(categoryId)) {
        movie.categories.push(categoryId);
    }
    await movie.save();
}

// 2 helper functions to use
const addMovieToCategory = async (categoryId, movieId) => {
    // get the category
    const category = await Category.findById(id);
    // check if the movie is not already there
    if (!category.movieIds.includes(movieId)) {
        // add the movieId to the movieIds array
        category.movieIds.push(movieId);
    }
    await category.save();
};

const deleteMovieFromCategory = async (categoryId, movieId) => {
    // get the category
    const category = await Category.findById(id);
    // get the index of the movie in the array
    const index = category.movieIds.indexOf(movieId);
    // check if the movie is there
    if (index !== -1) {
        // remove the movieId from the movieIds array
        category.movieIds.splice(index, 1);
    }
    await category.save();
};

// helper function to add a watched movie - used by movies
async function addMovieToUser(userId, movieId) {
    const user = await User.find({_id: userId})
    user.watched_movies.push(movieId)
    await user.save();
}

// helper function to delete a watched movie - used by movies
async function deleteMovieFromUser(userId, movieId) {
    const user = await User.find({_id: userId})
    user.watched_movies = watched_movies.filter(movie => movie !== movieId);
    await user.save();
}

const getCategories = async () => { return await Category.find({}); };


module.exports = {deleteCategoryFromMovie, addCategoryToMovie, addMovieToCategory, deleteMovieFromCategory, addMovieToUser, deleteMovieFromUser, getCategories}