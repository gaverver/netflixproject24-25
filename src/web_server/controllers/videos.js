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
    try {
        x = await videoService.getVideoById(videoId);
    } catch (error) {
        return res.status(404).json({ error: 'video not found' });
    }
    const videoPath = (await videoService.getVideoById(videoId)).filePath;
    const range = req.headers.range;
    if (!range) {
        return res.status(400).json( {error: 'no requested range'} );
    }
    try {
        const leSize = fs.statSync(videoPath).size;
        const parts = range.replace(/bytes=/, "").split('-');
        console.log(parts)
        const start = parseInt(parts[0], 10);
        const end = parts[1] ? Math.min(parseInt(parts[1], 10), leSize - 1) : Math.min(start + 10**6,leSize - 1);
        console.log(end)
        // const chunk_size = 10 ** 6; // 1MB
        const chunk_size = start - end + 1;
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
    } catch (error) {
        console.log(error)
        return res.status(500).json({ error: 'Internal Server Error' });
    }
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
    streamVideoById,
    DeleteVideoById
};
