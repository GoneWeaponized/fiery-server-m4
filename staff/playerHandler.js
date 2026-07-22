const fs = require('fs');
const { Buffer } = require('node:buffer');
const path = require("path");

const PLAYERS_FILE = path.join(__dirname,"../data/players.json");

let players = [];

if (fs.existsSync(PLAYERS_FILE)){
    players = JSON.parse(fs.readFileSync(PLAYERS_FILE,"utf8"));
}

function savePlayers(){
	fs.writeFileSync(PLAYERS_FILE,JSON.stringify(players,null,2));
}

function findPlayerByName(username){
	const clean = username.trim().toLowerCase();
	return players.some(p => p.username.trim().toLowerCase() === clean);
}

function findPlayerByUUID(uuid){
	return players.find(p => p.uuid === uuid);
}

function getPlayers(){
	return players;
}

function addPlayer(player, uuid){
	players.push(player);
	savePlayers();
	console.log(player.username + " -> Has been added. with UUID: " + uuid);
}

// ========================================== PLAYER EVENTS ===================================================

function purchaseEvent(socket, lat, long, request, uuid) {
	const requester = findPlayerByUUID(uuid);
	if (!requester) {
		console.log("requester not found for " + uuid)
		return;
	}
	const player = findPlayerByUUID(uuid);

	if(!player) return;

	validateReq(uuid, request, lat, long, socket);
}

function updateOldPlayer(lat, long ,uuid) {
	const requester = findPlayerByUUID(uuid);
	if (!requester) {
		console.log("requester not found for " + uuid)
		return;
	}
	const player = findPlayerByUUID(uuid);

	if(!player) return;

	player.entity.lat = lat;
	player.entity.lng = long;

	savePlayers();
	//console.log("[CLIENT UPDATE PLAYER REQUEST] Player: \"" + player.username + "\" subID: \"" + player.subId + "\" - updated successfully.");
}
// =============== FUNCTIONS THAT SEND TO CLIENT =====================

function getPlayersClient(socket) {
	for (const player of players) {

		const lat = parseFloat(player.entity.lat);
		const lng = parseFloat(player.entity.lng);

		const nameBuf = Buffer.from(player.username, 'utf8');
		const subIdBuf = Buffer.from(player.subId, 'utf8'); //always 16 characters long.

		const packetSize = 37 + nameBuf.length;
		const temp = Buffer.alloc(packetSize);

		temp.writeUInt16BE(packetSize, 0); // packet size
		temp.writeUInt8(1, 2);             // Flag (type)
		temp.writeDoubleBE(lat, 3);
		temp.writeDoubleBE(lng, 11);

		temp.write(player.subId, 19, 16, 'utf8');

		temp.writeUInt16BE(nameBuf.length, 35); // Name Length
		nameBuf.copy(temp, 37);                 // Name Content

		socket.write(temp);
	}
}

// =================== CLI STUFF AND OUTPUTS ==================

console.log("RESOLVED PATH:", fs.realpathSync(PLAYERS_FILE));
console.log("PLAYERS LOADED AT START:", players.length);

// =================== UTILITY FUNCTIONS ======================


function listPlayers() {
	console.log("\n====== LISTING PLAYERS ======\n");
	for (const player of players) {
		console.log(`${player.username.padEnd(20)} | SubID: [${player.subId}]`);
	}
	console.log("\n====== END OF PLAYERS LIST ======\n");
}

function playerInfo(username) {

	console.log(`information of \'${username}\' : \n`);
	for (const player of players) {
		if (player.username === username) {
			console.log("UUID: " + player.uuid);
			console.log("lat: " + JSON.parse(player.entity.lat));
			console.log("long: " + JSON.parse(player.entity.lng + "\n"));
			break;
		}
	}
}


module.exports = { getPlayersClient, updateOldPlayer, listPlayers, findPlayerByName, playerInfo, addPlayer, savePlayers }
