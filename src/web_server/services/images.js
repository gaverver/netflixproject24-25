const Image = require('../models/image');

async function createImage(data, contentType) {
    // creating a new inage and saving the changes
    const image = new Image({ data: data, contentType: contentType });
    await image.save();
    return image;
}

async function getImageById(id) {
    return await Image.findById(id);
}

async function deleteImage(id) {
    const image = await Image.findById(id);
    // if the image didn't found return null
    if (image) {
        return await Image.findByIdAndDelete(id);
    }
    return null;
}

module.exports = { createImage, getImageById, deleteImage }