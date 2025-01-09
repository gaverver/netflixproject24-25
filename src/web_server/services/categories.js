const Category = require('../models/categories');
const MovieService = require('../services/movies')

const createCategory = async (name, promoted, movieIds) => {
    // check if there is a category with this name
    if (await Category.findOne({ name: name })) return null;
    // create a new category with the name inserted
    const category = new Category({ name: name });
    // check if 'promoted' is explicitly passed
    if (promoted !== undefined) {
        category.promoted = promoted;
    }
    // check if 'movieIds' is explicitly passed
    if (movieIds !== undefined) {
        category.movieIds = movieIds;
    }
    return await category.save();
};

const getCategoryById = async (id) => { return await Category.findById(id); };

const getCategories = async () => { return await Category.find({}); };

const updateCategory = async (id, name, promoted, movieIds) => {
    const category = await getCategoryById(id);
    if (!category) return null;
    // check if 'name' is explicitly passed
    if (name !== undefined) {
        // check if there is a different category with the new name
        if (await Category.findOne({ name: name, _id: { $ne: id } })) return "name already exists";
        // else, update the name
        category.name = name;
    }
    // check if 'promoted' is explicitly passed
    if (promoted !== undefined) {
        category.promoted = promoted;
    }
    // remember the former movies of this category
    const formerMovies = category.movieIds
    // check if 'movieIds' is explicitly passed
    if (movieIds !== undefined) {
        category.movieIds = movieIds;
    }
    await category.save();
    // update in each new/removed movie the categories field
    // delete from each former movie the category from categories field
    for (const movie of formerMovies) {
        await MovieService.deleteCategoryFromMovie(movie, id);
    }
    // add to each new movie the category to the categories field
    for (const movie of movieIds) {
        await MovieService.addCategoryToMovie(movie, id);
    }
    return category;
};

const deleteCategory = async (id) => {
    // get the category by id and delete it
    const deletedCategory = await Category.findByIdAndDelete(id);
    // if the category does not exists, return null
    if (!deletedCategory) return null;
    // else, delete the category from every movie that has it
    for (const movie of deletedCategory.movieIds) {
        await MovieService.deleteCategoryFromMovie(movie, id);
    }
    return deletedCategory;
};

// 2 helper functions to use
const addMovieToCategory = async (categoryId, movieId) => {
    // get the category
    const category = await getCategoryById(categoryId);
    // check if the movie is not already there
    if (!category.movieIds.includes(movieId)) {
        // add the movieId to the movieIds array
        category.movieIds.push(movieId);
    }
    await category.save();
};

const deleteMovieFromCategory = async (categoryId, movieId) => {
    // get the category
    const category = await getCategoryById(categoryId);
    // get the index of the movie in the array
    const index = category.movieIds.indexOf(movieId);
    // check if the movie is there
    if (index !== -1) {
        // remove the movieId from the movieIds array
        category.movieIds.splice(index, 1);
    }
    await category.save();
};

module.exports = { createCategory, getCategoryById, getCategories, updateCategory, deleteCategory, addMovieToCategory, deleteMovieFromCategory }