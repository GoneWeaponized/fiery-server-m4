const playerHandler = require('./playerHandler');
const path = require("path");
const PLAYERS_FILE = path.join(__dirname,"../data/players.json");
const { COST_LOOKUP } = require('../constants/costs');

function requests(uuid, request, lat, long, socket) {

    const reqId = parseInt(request, 10);
    if (isNaN(reqId) || reqId < 0 || reqId >= COST_LOOKUP.length) {
        socket.write(Buffer.from([0x06]));
        console.log(`[ERROR] Invalid request ID received: ${requestId}`);
        return;
    }
    const cost = COST_LOOKUP[reqId];
    switch(reqId) {
        case 0: building.construct(socket, lat, long, 0, uuid)//build request
    }
}
