const express = require('express');
var router = express.Router();
const tokensController = require('../../controllers/tokens');

router.route('/')
    .post(tokensController.createToken);

module.exports = router;