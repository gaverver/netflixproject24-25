const multer = require('multer');
const path = require('path');
const Video = require('../models/videos');
const fs = require("fs")

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, '../../data/videos'); // Save files in the "videos" folder
    },
    filename: (req, file, cb) => {
        // give the file names so there will be no collisions at first
        const uniqueSuffix = Date.now() + '-' + Math.round(Math.random() * 1e9);
        cb(null, file.fieldname + '-' + uniqueSuffix + path.extname(file.originalname));
    },
});

const upload = multer({ storage })

const createVideo = async (filePath) => {
    // create the video in the db
    const video = new Video({filePath: filePath});
    await video.save()
    // change the video name to his id
    const new_path = `../../data/videos/${video.id}.mp4`
    fs.rename(filePath, new_path, (err) => {
        if (err)
            return null
    })
    video.filePath = new_path
    return await video.save();
};

const getVideoById = async (id) => {
    // return the video with the given id
    const video = await Video.findById(id);
    return video;
};

module.exports = {
    upload,
    createVideo,
    getVideoById
};