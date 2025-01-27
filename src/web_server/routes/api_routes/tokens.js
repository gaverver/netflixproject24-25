const express = require('express');
var router = express.Router();
const tokensController = require('../../controllers/tokens');

router.route('/')
    .post(tokensController.createToken);

router.route('/:token')
    .get(tokensController.getIdFromToken);

module.exports = router;