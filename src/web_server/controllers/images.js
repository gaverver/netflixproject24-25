const imageServices = require('../services/images');
const mongoose = require('mongoose');

// function to handle image upload
async function uploadImage(req, res) {
    // get the content type from the header
    const contentType = req.headers['content-type'];

    // if no Content-Type is provided, return an error
    if (!contentType) {
        return res.status(400).json({ message: 'content-type header is missing' });
    }

    // check for application/octet-stream (for raw binary file upload)
    if (contentType.includes('application/octet-stream')) {
        const data = [];
        
        // collect the binary data in chunks
        req.on('data', chunk => {
            data.push(chunk);
        });

        // when data collection is finished, save the image
        req.on('end', async () => {
            const imageData = Buffer.concat(data);
            
            try {
                // call the service to create the image in the database
                const image = await imageServices.createImage(imageData, contentType);
                // set the location header so the user could see the location to the new image
                return res.status(201).set('Location', `/api/images/${image._id}`).end();
            } catch (error) {
                return res.status(500).json({ error: 'Internal Server Error' });
            }
        });
    } else {
        // if the content-type is not supported
        return res.status(400).json({ message: 'Invalid content type, please use application/octet-stream for binary data' });
    }
}


async function getImage(req, res) {
    // for comfort
    const id = req.params.id;
    // if the id isn't valid
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(404).json({ error: 'Image not found' });
    }
    try {
        const image = await imageServices.getImageById(id);
        // if image not found
        if (!image) {
            return res.status(404).json({ error: 'Image not found' });
        }
        
        return res.status(200).json(image);
    } catch (error) {
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

async function deleteImage(req, res) {
    // for comfort
    const id = req.params.id;
    // if the id isn't valid
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(404).json({ error: 'Image not found' });
    }
    try {
        const image = await imageServices.deleteImage(id);
        // if image not found
        if (!image) {
            return res.status(404).json({ error: 'Image not found' });
        }

        return res.status(204).end();
    } catch (error) {
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

module.exports = { uploadImage, getImage, deleteImage }