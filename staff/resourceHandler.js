const fs = require('fs');
const { Buffer } = require('node:buffer');
const path = require("path");
const costs = require("../constants/costs");

const RESOURCES_FILE = path.join(__dirname,"../data/resources.json");

let resources = [];

if (fs.existsSync(RESOURCES_FILE)) {
    resources = JSON.parse(fs.readFileSync(RESOURCES_FILE, 'utf8'));
}

// ================== UTILITY ==========================================

function makeResEntry(uuid) {

    resourceOwner = {
        owner: uuid,
        resources: {
            money: 40000,
            manpower: 50000
        }
    };
    addResourceOwner(resourceOwner, uuid);
}

function saveResources(){
    fs.writeFileSync(RESOURCES_FILE,JSON.stringify(resources,null,2));
}
function findResourceByUUID(uuid){
    return resources.find(r => r.owner === uuid);
}

function addResourceOwner(resourceOwner, uuid) {
    resources.push(resourceOwner);
    saveResources();
    console.log("ADDED: Resource Owner for uuid: ", uuid);
}
function deductResource(uuid, amount) {

    const resourceOwner = findResourceByUUID(uuid);

    if (!resourceOwner) {
        console.log(`Resource owner not found for UUID ${uuid}`);
        return false;
    }

    if (resourceOwner.resources.money < amount) {
        console.log(`${uuid} does not have enough money.`);
        return false;
    }

    resourceOwner.resources.money -= amount;

    saveResources();

    console.log(`${uuid} spent $${amount}.`);

    return true;
}

// Client functions have been set in ./client-parse/resourceRequester.js

// ============== CLI STUFF =======================
console.log("\nRESOLVED RESOURCES FILE PATH:", fs.realpathSync(RESOURCES_FILE));
if (resources.length > 0) { console.log("Resources loaded!\n");}

module.exports = { 
    saveResources,
    findResourceByUUID, 
    makeResEntry,
    deductResource
 };
