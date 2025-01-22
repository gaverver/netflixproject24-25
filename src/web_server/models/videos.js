const mongoose = require('mongoose');

const Video = new mongoose.Schema({
    filePath: { 
        type: String, 
        required: true 
    }
});

module.exports = mongoose.model('videos', Video);
