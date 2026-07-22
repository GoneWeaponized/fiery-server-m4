const krakey = require('./krakey');
const path = require("path");
const { Base, Overlord, SAMsite } = require('../classes/buildables');
function construct(socket, lat, long, typeStructure, uuid) {

    const errormsg = Buffer.alloc(1);
    errormsg.writeUInt8(6, 0);
    if(typeStructure == null) {
    socket.write(errormsg); // in case if its null
    }

    const owner = uuid;
    switch(typeStructure) {
        case 0: const newBase = new Base(lat, long, owner); addBuilding(newBase);
    }

}

function addBuilding(data) {

}
