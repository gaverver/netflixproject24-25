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
        await Image.deleteOne({ _id: id });
        return image;
    }
    return null;
}

// helper function that get an image id and return a url for the image
async function convertToURL(id) {
    const image = await getImageById(id);
    // if image not found return null
    if (!image) {
        return null;
    }
    const base64Image = image.data.toString("base64");
    const imageURL = `data:image/${image.contentType};base64,${base64Image}`;
    return imageURL;
}

module.exports = { createImage, getImageById, deleteImage, convertToURL }