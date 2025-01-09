const express = require('express');
var router = express.Router();
//import routers
const categoriesRouter = require('./api_routes/categories')
const tokensRouter = require('./api_routes/tokens')
const moviesRouter = require('./api_routes/movies')
const usersRouter = require('./api_routes/users')

router.use('/categories', categoriesRouter)
router.use('/tokens', tokensRouter)
router.use('/movies', moviesRouter)
router.use('/users', usersRouter)

module.exports = router