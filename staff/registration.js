const fs = require('fs');
const path = require("path");
const {findPlayerByName, addPlayer} = require("./playerHandler");
const crypto = require("crypto");
const { isValidUsername } = require("../util/validation");
const {makeResEntry} = require("./resourceHandler");
let errorBuf = Buffer.alloc(3);
errorBuf.writeUInt16BE(errorBuf.length, 0);

function createPlayer(socket, lat, long, username) {
    //if(username !=+ String){
    //    socket.write("ERROR");
    //    console.log("Error in name parsing, not string type\n");
    //    return;
    //}
    username = username.replace(/[\r\n]/g, "");
    if(!isValidUsername(username)){

        errorBuf.writeUInt8(6, 2);
        socket.write(errorBuf);
        console.log("Error in validating, not valid name.");
        return;
    }
    let player = findPlayerByName(username);

    if (player){

        errorBuf.writeUInt8(12, 2);
        socket.write(errorBuf);
        console.log("PLayer exists\n");
        return;
    }
    console.log("New creation request.\n");
    const uuid = crypto.randomUUID();
    const subId = crypto.randomBytes(8).toString('hex');

    const lng = long;

    player = {
        uuid,
        username,
        subId,
        entity:{
            type: "player",
            lat,
            lng
        }
    };

    addPlayer(player, uuid);
    makeResEntry(uuid);
    let tmp = Buffer.alloc(uuid.length + 3);
    tmp.writeUInt16BE(tmp.length, 0);
    tmp.writeUInt8(0, 2); // Flag 0 -> New Login Accepted, here is your UUID.
    tmp.write(uuid, 3,'utf8');
    socket.write(tmp);
}

module.exports = { createPlayer }
