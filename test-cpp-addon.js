const distCalc = require('./build/Release/addon-cpp');
let distances = [55.751407590272514, 37.624116370723414, 53.900602350921815, 27.558945558109507];
console.log(distCalc.getDistance(distances));
