const resourceHandler = require('../resourceHandler');
const playerHandler = require('../playerHandler');
const buildHandler = require('../buildingHandler')
const { Buffer } = require('node:buffer');

let offset = 3;

const errormsg = Buffer.alloc(3);
errormsg.writeUInt16BE(2, 0);
errormsg.writeUInt8(6, 2);

function makeStructureEntry(socket, pakket) {
    const uuidLen = pakket.readUInt16BE(offset);
    offset += 2;

    const uuid = pakket.toString(
        'utf8',
        offset,
        offset + uuidLen
    );
    offset += uuidLen;

    const typeStructure = pakket.readUInt16BE(offset);

    const currentPlayer = playerHandler.findPlayerByUUID(uuid);
    if (!currentPlayer) {socket.write(errormsg); return;}
    let lat = parseFloat(currentPlayer.entity.lat);
    let long = parseFloat(currentPlayer.entity.lng);

    buildHandler.construct(socket, lat, long, typeStructure, uuid);
}

module.exports = {makeStructureEntry}
