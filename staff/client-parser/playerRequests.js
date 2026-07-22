const { Buffer } = require('node:buffer');
const registration = require('../registration');
const playerHandler = require('../playerHandler')

let offset = 3;

function makePlayerEntry(socket, pakket) {
    const usernameLength = pakket.readUInt16BE(offset);
    offset += 2;

    const username = pakket.toString(
        'utf8',
        offset,
        offset + usernameLength
    );
    offset += usernameLength;

    const lat = pakket.readDoubleBE(offset);
    offset += 8;

    const long = pakket.readDoubleBE(offset);
    console.log("Krakey LOG START:\n");
    console.log(username);
    console.log(lat);
    console.log(long);
    console.log("Krakey LOG END\n\n");
    registration.createPlayer(socket, lat, long, username);
}

function updateLoc(socket, pakket) {
    const lat = pakket.readDoubleBE(3);
    const long = pakket.readDoubleBE(11);

    const idLength = pakket.readInt32BE(19);
    const uuid = pakket.toString(
        'utf8',
        23,
        23 + idLength
    );

    playerHandler.updateOldPlayer(lat, long, uuid);
    playerHandler.getPlayersClient(socket); // Hey rui can you add some loop that sends this typa data when a guy updates location once. Sends until player disonnects
}
module.exports = {makePlayerEntry, updateLoc};
