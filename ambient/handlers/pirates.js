const randomLocation = require("../util/randomLocation");

module.exports = function () {

    return {
        lat: randomLocation.randomLatitude(),
        lng: randomLocation.randomLongitude(),

        radius: randomLocation.randomRadius(1000, 6000),

        reward: 200000,

        // seconds
        duration: 6 * 60 * 60
    };
};