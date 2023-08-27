
const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/login_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');

router.post('/', 
    function (request, response) {
    console.log("Request: ", request.body);
    // Check if the request body contains both idKortti and pin properties
    if (!request.body.username || !request.body.pin) {
        console.log("username or pin missing");
        return response.send({ login: false });
    }

    const username = request.body.username;
    const pin = request.body.pin;
    
    login.checkUsername(username, async function (err, dbResult) {
        console.log("Response: ", dbResult);
        if (err) {
            console.log("Error:", err);
            return response.send({ login: false });
        } else {
            //console.log("pin: ", pin);
            //console.log("Hashed_pin: ", dbResult[0].pin);
            const compareResult = await bcrypt.compare(pin, dbResult[0].pin);
            //console.log("Compare result: ", compareResult);

            if (!compareResult) {
                console.log("Error: Wrong pin");
                return response.send({ login: false });
            } else {
                console.log("username and pin ok");
                //const token = generateAccessToken({ username: username });
                //console.log("Token: ", { token: token });
                //return response.send({ token: token });
                return response.send({ login: true });
            }
        }
    });
});

function generateAccessToken(idKortti) {
    dotenv.config();
    return jwt.sign(idKortti, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports = router;
