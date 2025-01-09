const express = require('express');
var router = express.Router();
const imageController = require('../controllers/images');

router.route('/')
    .post(imageController.uploadImage);

router.route('/:id')
    .get(imageController.getImage)
    .delete(imageController.deleteImage);

module.exports = router;