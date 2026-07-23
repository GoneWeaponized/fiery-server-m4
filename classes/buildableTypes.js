const {
    Base,
    Overlord,
    SAMsite,
    CIWS,
    Artillery,
    Airbase
} = require('../entities/buildables');




const BUILDABLES = {
    BASE: {
        typeId: 0,
        name: "Base",
        cost: 30000,
        class: Base
    },

    OVERLORD: {
        typeId: 1,
        name: "Overlord",
        cost: 0,
        class: Overlord
    },

    SAMSITE: {
        typeId: 2,
        name: "SAM Site",
        cost: 50000,
        class: SAMsite
    },

    CIWS: {
        typeId: 3,
        name: "CIWS",
        cost: 5000,
        class: CIWS
    },

    ARTILLERY: {
        typeId: 4,
        name: "Artillery",
        cost: 5000,
        class: Artillery
    },

    AIRBASE: {
        typeId: 5,
        name: "Airbase",
        cost: 100000,
        class: Airbase
    },
};

const BUILDABLES_BY_ID = {};

for (const buildable of Object.values(module.exports)) {
    BUILDABLES_BY_ID[buildable.typeId] = buildable;
}

module.exports = {
    BUILDABLES,
    BUILDABLES_BY_ID
};