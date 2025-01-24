const express = require('express');
const videoController = require('../controllers/videos');
const videoService = require('../services/videos');

const router = express.Router();

// Route for uploading videos
router.post('/', videoService.upload.single('video'), videoController.uploadVideo);

router.route('/:id')
    .get(videoController.getVideoById)
    .delete(videoController.DeleteVideoById)

router.route('/watch/:id')
    .get(videoController.streamVideoById)

module.exports = router;