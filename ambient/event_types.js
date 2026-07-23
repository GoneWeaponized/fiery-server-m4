// event_types.js

module.exports = {
    zombies: {
        typeId: 1,
        name: "zombies",
        weight: 15,
        cooldown: 5 * 60 * 1000,      // 5 minutes
        lastSpawn: 0,
        message: "A zombie horde has been sighted.",
        handler: require("./handlers/zombies")
    },

    pirate: {
        typeId: 2,
        name: "pirate",
        weight: 10,
        cooldown: 10 * 60 * 1000,     // 10 minutes
        lastSpawn: 0,
        message: "Pirates have appeared on the horizon.",
        handler: require("./handlers/pirates")
    },

    rescue: {
        typeId: 3,
        name: "rescue",
        weight: 75,
        cooldown: 2 * 60 * 1000,      // 2 minutes
        lastSpawn: 0,
        message: "A distress signal has been received.",
        handler: require("./handlers/rescue")
    }
};