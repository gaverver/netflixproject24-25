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
  
  try {
    const video = await videoService.getVideoById(videoId);
    const videoPath = video.filePath;
    const stat = fs.statSync(videoPath);
    const fileSize = stat.size;
    const range = req.headers.range;

    if (range) {
      const parts = range.replace(/bytes=/, "").split("-");
      const start = parseInt(parts[0], 10);
      const end = parts[1] ? parseInt(parts[1], 10) : fileSize - 1;
      const chunksize = (end - start) + 1;
      const file = fs.createReadStream(videoPath, {start, end});
      const head = {
        'Content-Range': `bytes ${start}-${end}/${fileSize}`,
        'Accept-Ranges': 'bytes',
        'Content-Length': chunksize,
        'Content-Type': 'video/mp4',
      };
      res.writeHead(206, head);
      file.pipe(res);
    } else {
      const head = {
        'Content-Length': fileSize,
        'Content-Type': 'video/mp4',
      };
      res.writeHead(200, head);
      fs.createReadStream(videoPath).pipe(res);
    }
  } catch (error) {
    console.error('Error streaming video:', error);
    res.status(500).json({ error: 'Internal Server Error' });
  }
};

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
