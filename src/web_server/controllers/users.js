const userService = require("../services/users")
const fs = require('fs')
const mongoose = require('mongoose')

async function countFilesInDirectory(directoryPath) {
    try {
      const files = await fs.promises.readdir(directoryPath); // Read the directory contents
      const fileCount = await Promise.all(files.map(async (file) => {
        try {
          const stats = await fs.promises.stat(`${directoryPath}/${file}`); // Concatenate strings for the file path
          return stats.isFile() ? 1 : 0; // Return 1 if it's a file, otherwise 0
        } catch (err) {
          return 0; // Return 0 in case of error with file stats
        }
      }));
      return fileCount.reduce((total, count) => total + count, 0); // Sum up the counts
    } catch (err) {
      return 0; // Return 0 if there's an error reading the directory
    }
  }

const createUser = async (req, res) => {
    // count the files inside te avatars directory - this will be the collection of the avatars that the user can use
    const MAX_IMAGES = await countFilesInDirectory("../../data/avatars")
    if (MAX_IMAGES === 0) {
        return res.status(500).json({ error: 'Internal Server Error' });
    }
    // get all the necessary details.
    const username = req.body.username
    const email = req.body.email
    const password = req.body.password
    const phoneNumber = req.body.phoneNumber
    const picture = req.body.picture
    // check if a username was entered
    if (username === undefined) {
        return res.status(400).json({ error:'username is required' });
    }
    // check if the type of username is a string
    if (typeof username !== 'string') {
        return res.status(400).json({ error:'Invalid data: username must be a string' });
    }

    // check if a password was entered
    if (password === undefined) {
        return res.status(400).json({ error:'password is required' });
    }
    // check if the type of password is a string
    if (typeof password !== 'string') {
        return res.status(400).json({ error:'Invalid data: password must be a string' });
    }

    // check if an email was entered
    if (email === undefined) {
        return res.status(400).json({ error:'email is required' });
    }
    // check if the email is with the correct format.
    const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/
    if (emailRegex.test(email) === false) {
        return res.status(400).json({ error:'email is in an invalid format' });
    }

    // check if a phoneNumber was entered
    if (phoneNumber === undefined) {
        return res.status(400).json({ error:'email is required' });
    }
    // check if the phoneNumber is with the correct format.
    const phoneNumberRegex = /^\d{10}$/;
    if (phoneNumberRegex.test(phoneNumber) === false) {
        return res.status(400).json({ error:'phone number is in an invalid format' });
    }
    // check if a picture was entered
    if (picture !== undefined) {
        // check if the type is a number - as it represents the number of the picture in the folder
        if (!Number.isInteger(picture)) {
            return res.status(400).json({ error:'Invalid data: picture must be an int' });
        }
        // check if the number is valid
        if (picture < 1 || picture > MAX_IMAGES) {
            return res.status(400).json({ error:`picture must be between 1 and ${MAX_IMAGES}`})
        }
    }
    

    

    try {
        // create the user using the service.
        const response = await userService.createUser(username, password, email, phoneNumber, picture)
        // respond with the fitting response depending on the returned value
        if (response === null) {
            return res.status(404).json({ error:'username and/or email and/ or phoneNumber already exists'})
        }
        res.status(201).set('Location', `/api/users/${response._id}`).end();
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

const getUserById = async (req, res) => {
    const id = req.params.id
    // check if the user's id is invalid
    if (!mongoose.Types.ObjectId.isValid(id)) {
        return res.status(404).json({ error:'Invalid data: user id must be a valid ObjectId'});
    }
    try {
        const user = await userService.getUserById(id); 
        // getUserById in the services returns null if the category wasn't found
        if (!user) {
            return res.status(404).json({ error:'User not found' });
        }
        // return the user and his details
        return res.status(200).json(user);
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }
}

module.exports = {getUserById, createUser }
