const Image = require('../models/images');

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
        return await Image.deleteOne(id);
    }
    return null;
}

module.exports = { createImage, getImageById, deleteImage }