const express = require('express');
const videoController = require('../controllers/videos');
const videoService = require('../services/videos');
const tokenController = require('../controllers/tokens');

const router = express.Router();

// Route for uploading videos
router.post('/', tokenController.isLoggedIn, tokenController.isAdmin, videoService.upload.single('video'), videoController.uploadVideo);

router.route('/:id')
    .get(tokenController.isLoggedIn, tokenController.isAdmin, videoController.getVideoById)
    .delete(tokenController.isLoggedIn, tokenController.isAdmin, videoController.DeleteVideoById)

router.route('/watch/:id')
    // .get(tokenController.isLoggedIn, videoController.streamVideoById)\
    .get(videoController.streamVideoById)

module.exports = router;
