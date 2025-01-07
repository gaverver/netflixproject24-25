const categoryService = require('../services/categories');
const mongoose = require('mongoose')

const createCategory = async (req, res) => {
    
    // avoid server crushes, by catching an error
    try {
        // for comfort
        const { name, promoted, movieIds } = req.body;

        // name is a required field
        if (name === undefined) {
            return res.status(400).json({ error:'Name is required' });
        }

        // validate the input
        if (typeof name !== 'string') {
            return res.status(400).json({ error: 'Invalid data: name must be a string' });
        }
    
        // promoted doesn't have to be passed, it has a default value
        if (promoted !== undefined && typeof promoted !== 'boolean') {
            return res.status(400).json({ error: 'Invalid data: promoted must be a boolean' });
        }

        // movieIds doesn't have to be passed, it has a default value
        if (movieIds !== undefined) {
            // check if the movieIds inserted is an array of valid ObjectIds
            if (!Array.isArray(movieIds)) {
                return res.status(400).json({ error: 'Invalid data: movieIds must be an array' });
            }
            if (!movieIds.every(id => mongoose.Types.ObjectId.isValid(id))) {
                return res.status(400).json({ error: 'Invalid data: movieIds must contain valid ObjectIds' });
            }
        }
    
        const newCategory = await categoryService.createCategory(name, promoted, movieIds)
        // add Location to the http header for the new category that has been created
        return res.status(201).set('Location', `/api/categories/${newCategory._id}`).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
};

const getCategories = async (req, res) => {
    // avoid server crushes, by catching an error
    try {
        // gets all exsiting categories
        return res.status(200).json(await categoryService.getCategories());   
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
};

const getCategory = async (req, res) => {

    // check if the category's id is invalid
    if (!mongoose.Types.ObjectId.isValid(req.params.id)) {
        return res.status(400).json({ error:'Invalid data: category id must be a valid ObjectId'});
    }

    // avoid server crushes, by catching an error
    try {
        const category = await categoryService.getCategoryById(req.params.id);
        
        // getCategoryById in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        
        return res.status(200).json(category);
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
};

const updateCategory = async (req, res) => {

    // check if the category's id is invalid
    if (!mongoose.Types.ObjectId.isValid(req.params.id)) {
        return res.status(400).json({ error:'Invalid data: category id must be a valid ObjectId'});
    }

    // for comfort
    const { name, promoted, movieIds } = req.body;

    // check if the body has some new data, if not, it's a bad request
    if (name === undefined && promoted === undefined && movieIds === undefined) {
        return res.status(400).json({ error:'No new data have been inserted' });    
    }

    // validate the input
    if (name !== undefined && typeof name !== 'string') {
        return res.status(400).json({ error: 'Invalid data: name must be a string' });
    }

    if (promoted !== undefined && typeof promoted !== 'boolean') {
        return res.status(400).json({ error: 'Invalid data: promoted must be a boolean' });
    }

    // movieIds doesn't have to be passed, it has a default value
    if (movieIds !== undefined) {
        // check if the movieIds inserted is an array of valid ObjectIds
        if (!Array.isArray(movieIds)) {
            return res.status(400).json({ error: 'Invalid data: movieIds must be an array' });
        }
        if (!movieIds.every(id => mongoose.Types.ObjectId.isValid(id))) {
            return res.status(400).json({ error: 'Invalid data: movieIds must contain valid ObjectIds' });
        }
    }

    // avoid server crushes, by catching an error
    try {
        const category = await categoryService.updateCategory(req.params.id, name, promoted, movieIds);
        // updateCategory in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        return res.status(204).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

const deleteCategory = async (req, res) => {

    // check if the category's id is invalid
    if (!mongoose.Types.ObjectId.isValid(req.params.id)) {
        return res.status(400).json({ error:'Invalid data: category id must be a valid ObjectId'});
    }

    // avoid server crushes, by catching an error
    try {
        const category = await categoryService.deleteCategory(req.params.id);
        // deleteCategory in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        return res.status(204).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

// exports all functions so the router for categories could use them
module.exports = {createCategory, getCategories, getCategory, updateCategory, deleteCategory };