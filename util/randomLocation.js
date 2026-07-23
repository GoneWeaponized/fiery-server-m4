function randomLatitude() {
    return (Math.random() * 180) - 90;
}

function randomLongitude() {
    return (Math.random() * 360) - 180;
}
function randomRadius(min, max){
    return Math.floor(Math.random() * (max - min + 1 )) + min;
}

module.exports = {
    randomLatitude,
    randomLongitude
};