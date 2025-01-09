const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const Image = new Schema({
    // the data of the image in binary bytes
    data: {
        type: Buffer,
        required: true
    },
    // content type like 'png' or 'jpeg' etc...
    contentType: {
        type: String,
        required: true
    }
});

// the name of the collection is images
module.exports = mongoose.model('images', Image);