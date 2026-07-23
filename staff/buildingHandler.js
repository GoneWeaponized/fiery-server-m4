const krakey = require('./krakey');
const path = require("path");
const resourceHandler = require('./resourceHandler');
const fs = require('fs');
const STRUCTURES_FILE = path.join(__dirname,"../data/structures.json");
const { Base, Overlord, SAMsite } = require('../classes/buildables');
const crypto = require("crypto");
const { COST_LOOKUP } = require('../constants/costs');

let structures = [];

if (fs.existsSync(STRUCTURES_FILE)){
    structures = JSON.parse(fs.readFileSync(STRUCTURES_FILE,"utf8"));
}

function saveStructures(){
    fs.writeFileSync(STRUCTURES_FILE,JSON.stringify(structures,null,2));
}

function construct(socket, lat, long, typeStructure, uuid) {
    // this thing only constructs. The request is validated by buildingRequest
    const owner = uuid;
    switch(typeStructure) {
        case Base.TYPE: const newBase = new Base(lat, long, owner); addBuilding(socket, uuid, newBase, Base.TYPE); break;
        case Overlord.TYPE: const newOv = new Overlord(lat, long, owner); addBuilding(uuid, newOv, Overlord.TYPE); break; // admin only lmao
    }

}

function addStructure(structure, uuid){
    structures.push(structure);
    saveStructures();
    console.log(structure + " -> Has been added.");
}

function validateRequest(socket, uuid, type) {
    const error = Buffer.alloc(3);
    error.writeUInt16BE(3,0);
    error.writeUInt8(18,2);
    const res = resourceHandler.findResourceByUUID(uuid);
    if(res.resources.money < COST_LOOKUP[type]) {
        console.log("Low Money, build request denied.");
        socket.write(error);
        return false;
    }
    res.resources.money -= COST_LOOKUP[type];
    resourceHandler.saveResources();
    return true;
}

function addBuilding(socket, uuid, building, type) {
    if (!validateRequest(socket, uuid, type)) {
        return;
    }
    const subId = crypto.randomBytes(8).toString('hex');
    structure = {
        owner: building.owner,
        type: type,
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
    addStructure(structure)
    saveStructures();
}
module.exports = {
    addBuilding, construct, saveStructures
};
// A structure inventory file will be created separately for structures that do have a inventory
// Running outta time bruh
