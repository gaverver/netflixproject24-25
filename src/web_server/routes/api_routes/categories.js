const express = require('express');
var router = express.Router();
const categoryController = require('../../controllers/categories');
const tokenController = require('../../controllers/tokens');

router.route('/')
    .get(categoryController.getCategories)
    .post(tokenController.isLoggedIn, tokenController.isAdmin, categoryController.createCategory);

router.route('/:id')
    .get(categoryController.getCategory)
    .patch(tokenController.isLoggedIn, tokenController.isAdmin, categoryController.updateCategory)
    .delete(tokenController.isLoggedIn, tokenController.isAdmin, categoryController.deleteCategory);

module.exports = router;