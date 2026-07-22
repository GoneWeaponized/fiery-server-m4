const resourceHandler = require('../resourceHandler');
const playerHandler = require('../playerHandler');
const { Buffer } = require('node:buffer');

function getPlayerResources(socket, pakket) {
    let idLen = pakket.readUInt16BE(3);
    let uuid = pakket.toString('utf8', 5, 5+ idLen);

    const pResources = resourceHandler.findResourceByUUID(uuid);
    const err = Buffer.alloc(3);
    err.writeUInt16BE(3, 0);
    err.writeUInt8(6, 2)
    if (!pResources) {socket.write(err); return;}

    let money = pResources.resources.money;
    let manpower = pResources.resources.manpower;
    // flag = 4
    const buf = Buffer.alloc(11);
    let offset = 0;

    buf.writeUInt16BE(buf.length, offset);
    offset += 2;

    buf.writeUInt8(4, offset);
    offset += 1;

    buf.writeUInt32BE(money, offset);
    offset += 4;

    buf.writeUInt32BE(manpower, offset);
    // add more later
    socket.write(buf);
    return;
}
module.exports = {
    getPlayerResources
};
