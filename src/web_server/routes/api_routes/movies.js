const express = require('express');
var router = express.Router();
const movieController = require('../../controllers/movies');
const tokenController = require('../../controllers/tokens');
router.route('/')
    .get(tokenController.isLoggedIn, movieController.getMovies)
    .post(tokenController.isLoggedIn, tokenController.isAdmin, movieController.createMovie);

router.route('/:id')
    .get(movieController.getMovieById)
    .put(tokenController.isLoggedIn, tokenController.isAdmin, movieController.updateMovie)
    .delete(tokenController.isLoggedIn, tokenController.isAdmin, movieController.deleteMovie);

router.route('/:id/recommend')
    .get(tokenController.isLoggedIn, movieController.getRecommendation)
    .post(tokenController.isLoggedIn, movieController.addMovieToUser)

router.route('/search/:query')
    .get(movieController.queryGet)

module.exports = router;