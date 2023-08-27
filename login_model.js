const db = require('../database');
const bcrypt = require('bcryptjs');

const login = {
  checkUsername: function(user, callback) {
    console.log("USER: " + user);
    db.query('SELECT username FROM users WHERE username IS NOT NULL', function(error, results) {
      if (error) {
        console.log(error);
        callback(error, null);
      } else {
        let matchFound = false;
        // loop through the results and compare each hashed username to the one you want to check
        for (let i = 0; i < results.length; i++) {
            console.log("User: " + results[i].username);
            if (results[i].username === user) {
            matchFound = true;
            db.query('SELECT pin FROM users WHERE username=?', [results[i].username], callback);
            break;
          }
        }
        if (!matchFound) {
          console.log("No match found");
          callback("Wrong username", null);
        }
      }
    });
  }
};

module.exports = login;
