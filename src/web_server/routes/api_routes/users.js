const express = require('express');
var router = express.Router();
const userController = require('../../controllers/users');

router.route('/')
    .post(userController.createUser);

router.route('/:id')
    .get(userController.getUserById);

module.exports = router;
