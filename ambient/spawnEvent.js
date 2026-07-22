const EVENT_TYPE = require("./event_types.js");

const handlers = {
    zombies: require("./handlers/zombies.js"), 
    pirate: require("./handlers/zombies.js"), 
    rescue: require("./handlers/rescue.js")
};

function spawnEvent(eventId) {
    const handler = handler[eventId];

    if (!handler) {
        console.log(`Unkown event type: ${eventId}`);
        return false;
    }

    handler();
    return true;
}

module.exports = spawnEvent;