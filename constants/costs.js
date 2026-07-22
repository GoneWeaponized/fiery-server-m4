const COSTS = Object.freeze({
    BASE: 30000,
    SAMSITE: 10000,
    CIWS: 5000,
    ARTILLERY: 5000,
    AIRBASE: 100000
});

const COST_LOOKUP = [
    COSTS.BASE,    // Index 0
    COSTS.SAMSITE,  // Index 1
    COSTS.CIWS,
    COSTS.ARTILLERY,
    COSTS.AIRBASE
];

module.exports = { COSTS, COST_LOOKUP };
