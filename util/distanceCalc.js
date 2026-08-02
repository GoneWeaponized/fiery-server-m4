const distCalc = require("../build/Release/addon-cpp");

function findDistance(coordArray) {
    console.log(distCalc.getDistance(coordArray));
    return distCalc.getDistance(coordArray);
}

module.exports = { findDistance };
