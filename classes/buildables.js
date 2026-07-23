class Base {
    static TYPE = 0;
    constructor(lat, long, owner) {
        this.lat = lat;
        this.long = long;
        this.owner = owner;
    }
}
class Overlord {
    static TYPE = 1;
    constructor(lat, long, owner) {
        this.lat = lat;
        this.long = long;
        this.owner = owner;
    }
}
class SAMsite {
    static TYPE = 2;
    constructor(lat, long, owner) {
        this.lat = lat;
        this.long = long;
        this.owner = owner;}
}
class CIWS {
    static TYPE = 3;
    constructor(lat, long, owner){
        this.lat = lat;
        this.long = long;
        this.owner = owner;
    }
}
class Artillery {
    static TYPE = 4;
    constructor(lat, long, owner){
        this.lat = lat;
        this.long = long;
        this.owner = owner;
    }
}
class Airbase {
    static TYPE = 5;
    constructor(lat, long, owner) {
        this.lat = lat;
        this.long = long;
        this.owner = owner;
    }
}

function writeBuild(socket, data) {
    const msg = Buffer.alloc(data.length)
    msg.writeUInt8()
}

module.exports = {Base, Overlord, SAMsite, CIWS, Artillery, Airbase};
