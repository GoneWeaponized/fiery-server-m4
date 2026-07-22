const COSTS = Object.freeze({
    BASE: 30000,
    SAMSITE: 10000
});

const COST_LOOKUP = [
    COSTS.BASE,    // Index 0
    COSTS.SAMSITE  // Index 1
];

module.exports = { COSTS, COST_LOOKUP };
