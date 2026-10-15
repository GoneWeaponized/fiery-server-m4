const { addStr, addMissile, startEngine, getMissile, getStructures } = require('./build/Release/FE_engine');
let distances = [55.751407590272514, 37.624116370723414, 53.900602350921815, 27.558945558109507];
const a = new Float64Array([55.751407590272514, 37.624116370723414]);
const b = new Float64Array([53.900602350921815, 27.55885]);
addMissile("5abd",a,b, 300000.0, 500, 0);
    addStr("LMAO", "MAO", 3000.0, 53.900602350921815, 53.900602350921815,27.558945558109507, 27.558945558109507);

startEngine();
console.log(getMissile());
console.log(getStructures());
console.log("Engine started in background. Watching simulation...");

setTimeout(() => {
    console.log("Test script ending.");
    process.exit(0);
}, 15000);
