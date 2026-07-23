const krakey = require('./krakey');
const path = require("path");
const resourceHandler = require('./resourceHandler');
const fs = require('fs');
const STRUCTURES_FILE = path.join(__dirname,"../data/structures.json");
const crypto = require("crypto");
const { BUILDABLES, BUILDABLES_BY_ID } = require("../classes/buildableTypes");

let structures = [];

if (fs.existsSync(STRUCTURES_FILE)){
    structures = JSON.parse(fs.readFileSync(STRUCTURES_FILE,"utf8"));
}

function saveStructures(){
    fs.writeFileSync(STRUCTURES_FILE,JSON.stringify(structures,null,2));
}

function construct(socket, lat, long, typeId, uuid) {

    const buildable = BUILDABLES_BY_ID[typeId];

    if (!buildable) {
        console.log(`Unknown buildable type: ${typeId}`);
        return false;
    }

    if (!validateRequest(socket, uuid, buildable.cost)) {
        return false;
    }

    const building = new buildable.class(lat, long, uuid);

    addBuilding(building, buildable);

    return true;
}

function addStructure(structure, uuid){
    structures.push(structure);
    console.log(structure + " -> Has been added.");
}

function validateRequest(socket, uuid, cost) {

    const error = Buffer.alloc(3);

    error.writeUInt16BE(3,0);
    error.writeUInt8(18,2);

    if (!resourceHandler.deductResource(uuid, cost)) {

        console.log("Low Money, build request denied.");

        socket.write(error);

        return false;
    }

    return true;
}

function addBuilding(socket, uuid, building, buildable) {
    if (!validateRequest(socket, uuid, buildable.cost)) {
        return;
    }
    const subId = crypto.randomBytes(8).toString('hex');
    const structure = {
        owner: building.owner,
        type: buildable.typeId,
        name: buildable.name,
        position: {
            lat: building.lat,
            long: building.long
        },
        data: {
            hp: building.Hp,
            subId: subId,
            hasInventory: building.HasInv,
            isOnline: building.online,
            bootTime: 1800000 // same for all
        }
    }
    addStructure(structure);
    saveStructures();
}
module.exports = {
    addBuilding, construct, saveStructures
};
// A structure inventory file will be created separately for structures that do have a inventory
// Running outta time bruh