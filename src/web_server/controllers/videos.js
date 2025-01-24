const videoService = require('../services/videos');
const mongoose = require('mongoose')
const fs = require('fs');

const uploadVideo = async (req, res) => {
    try {
        // check if the file exists in the request
        if (!req.file) {
            return res.status(400).json({ message: 'No file uploaded' });
        }

        // create a video using the entered path
        const savedVideo = await videoService.createVideo(req.file.path);
        if (savedVideo == null) {
            res.status(500).json({ message: 'Internal Server error' });
        }
        
        return res.status(201).set('Location', `/videos/${savedVideo._id}`).end();
    } catch (error) {
        res.status(500).json({ message: 'Internal Server error' });
    }
};

const getVideoById = async (req, res) => {
    const id = req.params.id
    // check if the video's id is invalid
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(404).json({ error:'Invalid data: video id must be a valid ObjectId'});
    }
    try {
        const video = await videoService.getVideoById(id); 
        // getvideoById in the services returns null if the category wasn't found
        if (!video) {
            return res.status(404).json({ error:'video not found' });
        }
        // return the video and his details
        return res.status(200).json(video);
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}


const streamVideoById = async (req, res) => {
    const videoId = req.params.id;
    // Get the video’s actual location and size
    x = await videoService.getVideoById(videoId);
    const videoPath = (await videoService.getVideoById(videoId)).filePath;
    const leSize = fs.statSync(videoPath).size;
    // Extract the range requested by the browser
    const range = req.headers.range;
    const parts = range.substring(6).split('-');
    const start = parseInt(parts[0]);
    const chunk_size = 10 ** 6; // 1MB
    const end = Math.min(start + chunk_size, leSize - 1);
    const le = fs.createReadStream(videoPath, { start, end });
    // Stream requested chunk
    const head = {
        'Content-Range': `bytes ${start}-${end}/${leSize}`,
        'Accept-Ranges': 'bytes',
        'Content-Length': chunk_size,
        'Content-Type': 'video/mp4',
    };
    res.writeHead(206, head);
    le.pipe(res);
}

const DeleteVideoById = async (req, res) => {
    const id = req.params.id
    // check if the video's id is invalid
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(404).json({ error:'Invalid data: video id must be a valid ObjectId'});
    }
    try {
        const video = await videoService.DeleteVideoById(id); 
        // DeleteVideoById in the services returns null if the category wasn't found
        if (!video) {
            return res.status(404).json({ error:'video not found' });
        }
        // return the video and his details
        return res.status(204).json(video);
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
};

module.exports = {
    getVideoById,
    uploadVideo,
    streamVideoById
    DeleteVideoById
};