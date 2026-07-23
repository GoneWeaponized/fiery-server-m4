const EVENT_TYPES = require("./event_types");
const playerHandler = require("../staff/playerHandler");

let nextEventId = 1;

const activeEvents =[];

function chooseRandomEvent() {

    const now = Date.now();

    const available = Object.values(EVENT_TYPES).filter(event =>
        now - event.lastSpawn >= event.cooldown
    );

    if (available.length === 0)
        return null;

    const totalWeight = available.reduce(
        (sum, event) => sum + event.weight,
        0
    );

    let roll = Math.random() * totalWeight;

    for (const event of available) {

        roll -= event.weight;

        if (roll <= 0) {
            event.lastSpawn = now;
            return event;
        }
    }

    return available[0];
}

function spawnEvent() {

    const eventType = chooseRandomEvent();

    if (!eventType)
        return false;

    const event = eventType.handler();

    event.instanceId = nextEventId++;
    event.typeId = eventType.typeId;
    event.name = eventType.name;
    event.message = eventType.message;

    // Calculate when the event expires
    event.expires = Date.now() + (event.duration * 1000);

    // Store it in memory
    activeEvents.push(event);

    playerHandler.sendAmbientEvent(event);

    console.log(
        `[AMBIENT] Spawned ${event.name} (#${event.instanceId})`
    );

setInterval(() => {

    const now = Date.now();

    for (let i = activeEvents.length - 1; i >= 0; i--) {

        if (activeEvents[i].expires <= now) {

            console.log(
                `[AMBIENT] Event #${activeEvents[i].instanceId} expired.`
            );

            // Later we'll notify clients.

            activeEvents.splice(i, 1);
        }
    }

}, 5000);

    return event;
}

function getActiveEvents() {
    return activeEvents;
}

function findEvent(instanceId) {
    return activeEvents.find(
        event => event.instanceId === instanceId
    );
}

function removeEvent(instanceId) {

    const index = activeEvents.findIndex(
        event => event.instanceId === instanceId
    );

    if (index !== -1)
        activeEvents.splice(index, 1);
}

module.exports = {
    spawnEvent,
    getActiveEvents,            
    findEvent,
    removeEvent    
};