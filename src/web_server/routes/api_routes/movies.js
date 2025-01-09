const express = require('express');
var router = express.Router();
const movieController = require('../../controllers/movies');

router.route('/')
    .get(movieController.getMovies)
    .post(movieController.createMovie);

router.route('/:id')
    .get(movieController.getMovieById)
    .put(movieController.updateMovie)
    .delete(movieController.deleteMovie);

router.route('/:id/recommend')
    .get(movieController.getRecommendation)
    .post(movieController.addMovieToUser)

router.route('/search/:query')
    .get(movieController.queryGet)

module.exports = router;