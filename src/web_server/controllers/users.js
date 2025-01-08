const userService = require("../services/users")
const fs = require('fs')

async function countFilesInDirectory(directoryPath) {
    try {
      const files = await fs.promises.readdir(directoryPath); // Use promises to make it async
      // counts the files inside the avatars folder(user pictures)
      const fileCount = files.filter(file => {
        return fs.statSync(path.join(directoryPath, file)).isFile();
      }).length;
      return fileCount;
    } catch (err) {
      return 0; // Return 0 if there's an error
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
    const phoneNumberRegex = /^(?:\+?\d{1,3}[-.\s]?)?(?:\(?\d{1,4}\)?[-.\s]?)?\d{1,4}[-.\s]?\d{1,4}[-.\s]?\d{1,9}$/;
    if (phoneNumberRegex.test(phoneNumber) === false) {
        return res.status(400).json({ error:'phone number is in an invalid format' });
    }
    // check if a picture was entered
    if (picture === undefined) {
        return res.status(400).json({ error:'picture is required'})
    }
    // check if the type is a number - as it represents the number of the picture in the folder
    if (!Number.isInteger(picture)) {
        return res.status(400).json({ error:'Invalid data: picture must be an int' });
    }
    // check if the number is valid
    if (picture < 1 || picture > MAX_IMAGES) {
        return res.status(400).json({ error:`picture must be between 1 and ${MAX_IMAGES}`})
    }

    

    try {
        // create the user using the service.
        const response = await userService.createUser(username, email, password, phoneNumber, picture)
        // respond with the fitting response depending on the returned value
        if (response === null) {
            return res.status(404).json({ error:'username and/or email and/ or phoneNumber already exists'})
        }
        res.status(201).end()
    } catch (error) {
        // return error indicates that the server has crushed
        return res.status(500).json({ error: 'Internal Server Error' });
    }

}

const getUserById = async (req, res) => {
    const id = req.params.id
    try {
        const user = await userService.getUserById(id); 
        // getUserById in the services returns null if the category wasn't found
        if (!user) {
            return res.status(404).json({ error:'User not found' });
        }
        // return the user and his details
        return res.status(200).json(user);
    } catch (error) {
        return res.status(404).json({ error:'User not found' });
    }
}

module.exports = {getUserById, createUser }
