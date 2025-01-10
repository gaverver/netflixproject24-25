const Category = require('../models/categories');
const utilities = require('../services/utilities')

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
        // add to each movie this category
        if (Array.isArray(movieIds) && movieIds.length > 0) {
            for (const movie of movieIds) {
                await utilities.addCategoryToMovie(movie, category._id)
            }
        }
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
    // if there was no fromer movies
    if (!formerMovies || formerMovies.length == 0) {
        return category;
    }
    // delete from each former movie the category from categories field
    for (const movie of formerMovies) {
        await utilities.deleteCategoryFromMovie(movie, id);
    }
    // add to each new movie the category to the categories field
    if (movieIds !== undefined) {
        for (const movie of movieIds) {
            await utilities.addCategoryToMovie(movie, id);
        }
    }
    return category;
};

const deleteCategory = async (id) => {
    // get the category by id and delete it
    const category = await Category.findById(id);
    // if the category does not exists, return null
    if (!category) return null;
    // check if movieIds is empty and nothing more to delete
    if (!category.movieIds || category.movieIds.length === 0) {
        return await Category.findByIdAndDelete(id);
    }
    // else, delete the category from every movie that has it
    for (const movie of category.movieIds) {
        await utilities.deleteCategoryFromMovie(movie, id);
    }
    return await Category.findByIdAndDelete(id);
};


module.exports = { createCategory, getCategoryById, getCategories, updateCategory, deleteCategory}