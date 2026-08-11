const krakey = require('./krakey');
const path = require("path");
const resourceHandler = require('./resourceHandler');
const fs = require('fs');
const STRUCTURES_FILE = path.join(__dirname, "../data/structures.json");
const crypto = require("crypto");
const { BUILDABLES_BY_ID } = require("../classes/buildableTypes");
const playerHandler = require("./playerHandler");
const RBush = require('rbush').default;
let structures = [];
const tree = new RBush(16);
if (fs.existsSync(STRUCTURES_FILE)) {
    structures = JSON.parse(fs.readFileSync(STRUCTURES_FILE, "utf8"));
}
function saveStructures() {
    fs.writeFileSync(STRUCTURES_FILE, JSON.stringify(structures, null, 2));
}
function sendInvalidDistance(socket) {
    let buf = Buffer.alloc(3);
    buf.writeUInt16BE(2,0);
    buf.writeUInt8(18,2);
    socket.write(buf);
}
console.log("buildingHandler: loading structures in tree.");
const formattedItems = structures.map(item => (
    {
        minX: item.position.long,
        minY: item.position.lat,
        maxX: item.position.long,
        maxY: item.position.lat,
        id: item.data.subId
    }));
tree.load(formattedItems);
console.log(`buildingHandler: Loaded ${tree.all().length} structures in tree`);

function construct(socket, lat, long, typeId, uuid) {

    const buildable = BUILDABLES_BY_ID[typeId];

    if (!buildable) {
        console.log(`Unknown buildable type: ${typeId}`);
        return false;
    }
    // Validate and deduct the player's resources once.
    if (!validateRequest(socket, uuid, buildable.cost)) {
        return false;
    }
    let ownerId = playerHandler.findPlayerByUUID(uuid);
    uuid = ownerId.subId;
    const building = new buildable.class(lat, long, uuid);

    addBuilding(building, buildable);

    return true;
}

function addStructure(structure) {
    structures.push(structure);
    const formattedItem = {
        minX: structure.position.long,
        minY: structure.position.lat,
        maxX: structure.position.long,
        maxY: structure.position.lat,
    };
    tree.insert(formattedItem);
    console.log(`${structure.name} -> Has been added.`);
}

function removeStructure(id) {
    const targetId = structures.findIndex(structure => structure.data.subId === id);
    if (targetId !== -1) {
        structures.splice(targetId, 1);
        console.log(`Removed ${id} from the structures array. Will take effect on quitting.`);
        tree.remove({ id: id }, (a, b) => a.id === b.id);
    }
}

function validateRequest(socket, uuid, cost) {

    const error = Buffer.alloc(3);

    error.writeUInt16BE(3, 0);
    error.writeUInt8(18, 2);

    if (!resourceHandler.deductResource(uuid, cost)) {

        console.log("Low Money, build request denied.");

        socket.write(error);

        return false;
    }

    return true;
}

function addBuilding(building, buildable) {

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
            subId,
            hasInventory: building.HasInv,
            isOnline: building.online,
            bootTime: 1800000,
            bootStarted: Date.now()
        }
    };

    addStructure(structure);
    saveStructures();
}


function placementValidate(lat, long) {
    const rad = 0.001; //100 meters
    // Rui, we can use it for much more than just structure placement lol
    const searchBox = {
        minX: long - rad,
        minY: lat - rad,
        maxX: long + rad,
        maxY: lat + rad
    };
    const results = tree.search(searchBox);
    if (results.length > 0) {
        results.forEach(item => {
            console.log(`Found ID: ${item.id} at Latitude: ${item.maxY}, Longitude: ${item.maxX}`);
            if(findDistance([lat,long,item.maxY,item.maxX]) < 0.1) {
                return false;
            }
        });
        return true;
    } else {
        // debug stuff  console.log("No locations found nearby.");
        return true;
    }
}

function sendStructures(socket) {
    for (const structure of structures) {
        try {
            let name = structure.name;
            let subId = structure.owner;
            let structId = structure.data.subId;

            let structIdBuf = Buffer.from(structId, 'utf8');
            let nameBuf = Buffer.from(name, 'utf8');
            let subIdBuf = Buffer.from(subId, 'utf8');

            const sizeBuf = 2 + 1 + structIdBuf.length + 2 + nameBuf.length + subIdBuf.length + 8 + 8 + 1 + 2 + 1 + 1;

            let data = Buffer.alloc(sizeBuf);
            let offset = 0;

            data.writeUInt16BE(sizeBuf, offset);
            offset += 2;
            data.writeUInt8(2, offset);
            offset += 1;

            structIdBuf.copy(data, offset);
            offset += structIdBuf.length;

            data.writeUInt16BE(nameBuf.length, offset);
            offset += 2;

            nameBuf.copy(data, offset);
            offset += nameBuf.length;

            subIdBuf.copy(data, offset);
            offset += subIdBuf.length;

            data.writeDoubleBE(structure.position.lat, offset);
            offset += 8;

            data.writeDoubleBE(structure.position.long, offset);
            offset += 8;

            data.writeUInt8(structure.type, offset);
            offset += 1;

            data.writeUInt16BE(structure.data.hp, offset);
            offset += 2;

            data.writeUInt8(structure.data.hasInventory ? 1 : 0, offset);
            offset += 1;

            data.writeUInt8(structure.data.isOnline ? 1 : 0, offset);
            offset += 1;

            socket.write(data);
        } catch (error) {
            console.log(error);
        }

    }
}

module.exports = {
    addBuilding,
    construct,
    saveStructures,
    sendStructures,
    removeStructure
};

// A structure inventory file will be created separately for structures that do have an inventory.
