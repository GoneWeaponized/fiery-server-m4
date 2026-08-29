const { Buffer } = require('node:buffer');
const { getPlayersClient, updateOldPlayer, purchaseEvent } = require('./playerHandler');
const resourceRequests = require('./client-parser/resourceRequests');
const playerRequests = require('./client-parser/playerRequests');
const buildRequest =  require('./client-parser/buildRequest')
const { createPlayer } = require('./registration');
const construct = require('./buildingHandler');
//const customizeReq = require('./client-parser/customizeReq');
module.exports = function (socket, pakket) {
	if (!pakket || pakket.length < 3) {
		console.log("Received packet too small to process headers.");
		return;
	}

	const lengthHeader = pakket.readUInt16BE(0);
	const type = pakket.readUInt8(2);

	switch (type) {

		case 0: {
			try {
				playerRequests.makePlayerEntry(socket, pakket);
			} catch(error) {
				console.log("Error in making player Entry.\n", error);
			}

			break;
		}

		case 1: {
			playerRequests.updateLoc(socket, pakket);
			break;
		}
		//case 2 is reserved for something else.
		case 3: buildRequest.makeStructureEntry(socket, pakket); break;

		case 4: resourceRequests.getPlayerResources(socket, pakket); break;

		case 5: buildRequest.getStructures(socket); break;

		//case 10: customizeReq(socket, pakket); break;

		default:
			console.log("Unknown packet type:", type);
			break;
	}
};
