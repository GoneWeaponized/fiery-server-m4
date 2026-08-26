const playerHandler = require('../playerHandler');
const { Buffer } = require('node:buffer');

const errorMsg = Buffer.alloc(3);
errorMsg.writeUInt16BE(3, 0);
errorMsg.writeUInt8(8, 2);
module.exports = function(socekt, pakket) {
    if (pakket.length < 33) {
        socket.write(errorMsg);
        return;
    }
}

let offset = 3;

const type = pakket.readUInt8(offset);
offset +=1;
const lengthId = pakket.readUInt16BE(offset);
offset +=2;
const uuid = pakket.toString(
    'utf8',
    offset,
    offset + lengthId
)
offset += lengthId;

switch (type) {
    case 0: {
        const nameLength = pakket.readUInt8(offset);
        offset += 1;
        const name = pakket.toString(
            'utf8',
            offset,
            offset + nameLength
        );
        customizeName(uuid, name); break;
    }
}


function customizeName(uuid, name) {
    const player = findPlayerByUUID(uuid);
    player.username = name;
}
