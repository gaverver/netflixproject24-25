const Category = require('../models/categories');

const createCategory = async (name, promoted, movieIds) => {
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
        category.name = name;
    }
    // check if 'promoted' is explicitly passed
    if (promoted !== undefined) {
        category.promoted = promoted;
    }
    // check if 'movieIds' is explicitly passed
    if (movieIds !== undefined) {
        category.movieIds = movieIds;
    }
    await category.save();
    return category;
};

const deleteCategory = async (id) => {
    return await Category.findByIdAndDelete(id);
};

module.exports = {createCategory, getCategoryById, getCategories, updateCategory, deleteCategory }