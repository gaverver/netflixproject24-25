const categoryService = require('../services/categories');

const createCategory = async (req, res) => {

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

    if (promoted !== undefined && typeof promoted !== 'boolean') {
        return res.status(400).json({ error: 'Invalid data: promoted must be a boolean' });
    }

    if (movieIds !== undefined) {
        if (!Array.isArray(movieIds)) {
            return res.status(400).json({ error: 'Invalid data: movieIds must be an array' });
        }
        if (!movieIds.every(id => mongoose.Types.ObjectId.isValid(id))) {
            return res.status(400).json({ error: 'Invalid data: movieIds must contain valid ObjectIds' });
        }
    }
    
    await categoryService.createCategory(name, promoted, movieIds)
    return res.status(201).end();
};

const getCategories = async (req, res) => {
    // gets all exsiting categories
    return res.status(200).json(await categoryService.getCategories());
};

const getCategory = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const category = await categoryService.getCategoryById(req.params.id);
        
        // getCategoryById in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        
        return res.status(200).json(category);
    } catch (error) {
        return res.status(404).json({ error:'Category not found' });
    }
};

const updateCategory = async (req, res) => {

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

    if (movieIds !== undefined) {
        if (!Array.isArray(movieIds)) {
            return res.status(400).json({ error: 'Invalid data: movieIds must be an array' });
        }
        if (!movieIds.every(id => mongoose.Types.ObjectId.isValid(id))) {
            return res.status(400).json({ error: 'Invalid data: movieIds must contain valid ObjectIds' });
        }
    }

    // avoid server crushes, by routing to an invalid place
    try {
        const category = await categoryService.updateCategory(req.params.id, name, movieIds);
        // updateCategory in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        return res.status(204).end();
    } catch (error) {
        return res.status(404).json({ error:'Category not found' });
    }
}

const deleteCategory = async (req, res) => {
    // avoid server crushes, by routing to an invalid place
    try {
        const category = await categoryService.deleteCategory(req.params.id);
        // deleteCategory in the services returns null if the category havn't found
        if (!category) {
            return res.status(404).json({ error:'Category not found' });
        }
        return res.status(204).end();
    } catch (error) {
        return res.status(404).json({ error:'Category not found' });
    }
}

// exports all functions so the router for categories could use them
module.exports = {createCategory, getCategories, getCategory, updateCategory, deleteCategory };