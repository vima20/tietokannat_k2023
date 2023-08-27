const db = require('../database');

const drive = {
    getAll: function (callback) {
        return db.query('select * from drives', callback);
    },
    getByusername: function (username, callback) {
        console.log("User: " + [username]);
        return db.query('select * from drives \
            inner join users on drives.userId=users.userId \
            where username = ?', [username], callback);
    },
    getAvgByusername: function (username, callback) {
        db.query('SELECT AVG(distance) AS average FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], function (err, result) {
            if (err) {
                console.log("Error retrieving average distance:", err);
            } else {
                console.log("User:", username);
                console.log("Response:", result);
                console.log("Avg:", result[0].average);
            }
        });
        return db.query('SELECT AVG(distance) AS average FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], callback);   
    },
    getMinByusername: function (username, callback) {
        db.query('SELECT MIN(distance) AS minimum FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], function (err, result) {
            if (err) {
                console.log("Error retrieving minimum distance:", err);
            } else {
                console.log("User:", username);
                console.log("Response:", result);
                console.log("Min:", result[0].minimum);
            }
        });
        return db.query('SELECT MIN(distance) AS minimum FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], callback);
    },    
    getMaxByusername: function (username, callback) {
        db.query('SELECT MAX(distance) AS maximum FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], function (err, result) {
            if (err) {
                console.log("Error retrieving maximum distance:", err);
            } else {
                console.log("User:", username);
                console.log("Response:", result);
                console.log("Max:", result[0].maximum);
            }
        });
        return db.query('SELECT MAX(distance) AS maximum FROM drives \
            INNER JOIN users ON drives.userId = users.userId \
            WHERE username = ?', [username], callback);
    },    
    add: function (drive, callback) {
        // Perform a query to get the userId based on the provided username
        db.query('SELECT userId FROM users WHERE username = ?', [drive.username], function(err, result) {
            if (err) {
                callback(err, null);
            } else {
                if (result.length === 0) {
                    callback(new Error('User not found'), null);
                } else {
                    // Use the retrieved userId to insert the drive record
                    const userId = result[0].userId;
                    db.query('INSERT INTO drives (userId, time, distance, position, gpsStartLAT, gpsStartLNG, gpsEndLAT, gpsEndLNG) VALUES (?, ?, ?, ?, ?, ?, ?, ?)',
                        [userId, drive.time, drive.distance, drive.position, drive.gpsStartLAT, drive.gpsStartLNG, drive.gpsEndLAT, drive.gpsEndLNG],
                        callback);
                }
            }
        });
    },
    delete: function (id, callback) {
        return db.query('delete from drives where driveId=?', [id], callback);
    },
            
    }
module.exports = drive;