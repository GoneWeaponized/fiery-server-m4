// structureHandler code

const structures = require('../data/structures.json');
const { saveStructures } = require('./buildingHandler');

for (const structure of structures){
    if (structure.data.isOnline)
        continue;

    if(Date.now() - structure.data.bootStarted >= structure.data.bootTime){
        structure.data.isOnline = true;
        console.log(`${structure.name} is now online`);

        saveStructures();
    }
}
