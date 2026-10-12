const { addStr, addMissile, startEngine, getMissile } = require('./build/Release/FE_engine');
let distances = [55.751407590272514, 37.624116370723414, 53.900602350921815, 27.558945558109507];
const a = new Float64Array([55.751407590272514, 37.624116370723414]);
const b = new Float64Array([53.900602350921815, 27.558945558109507]);
if (addMissile("5abd",a,b, 300000.0)) {
    addStr("LMAO", "MAO", 3000.0, 55.751407590272514, 37.624116370723414, 53.900602350921815, 27.558945558109507);
    addMissile("jiod4",a,b, 300.0)
    addMissile("wdwf3",a,b, 400.0)
    addMissile("53fe3",a,b, 600.0)
    console.log("Added a missile!");
}
startEngine();
console.log(getMissile());
console.log("Engine started in background. Watching simulation...");

// 3. Keep the Node.js process alive for 15 seconds so you can watch it tick
setTimeout(() => {
    console.log("Test script ending.");
    process.exit(0);
}, 15000);
