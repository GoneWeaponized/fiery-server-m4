const net = require('net');

const PORT = 5010;
const HOST = '127.0.0.1';

const name = "Lolmantrollman";
const nLen = Buffer.byteLength(name, 'utf8');
const packetSize = 5 + name.length + 16;

const buf = Buffer.alloc(packetSize);
buf.writeUInt16BE(packetSize, 0);
buf.writeUInt8(0, 2);
buf.writeUInt16BE(nLen, 3);
buf.write(name, 5, 'utf8');
buf.writeDoubleBE(34.634234, 5 + nLen );
buf.writeDoubleBE(82.93432, 5 + nLen+8);

const client = new net.Socket();

client.connect(PORT, HOST, () => {
    console.log('Azimuth set to target, cannoning the pakket');
    client.on("data", buffer => {

        thing = Buffer.alloc(buffer.length);
        //console.log(buffer);
        const lent = buffer.readUInt16BE(0);
        const type = buffer.readUInt8(2);
        switch(type) {
            case 6: console.log("The shell was intercepted!? HOW!?? - Invalid Name"); break;
            case 12: console.log("Area arleady bombarded. - Exists"); break;
            default: console.log("I guess everything is fine? - Nothing wrong."); thing.write(buffer.toString(), 3, 'utf8'); console.log(thing.toString());
        }

        console.log(lent, " Length");
        console.log(type, " Type");
        }
    );
    client.write(buf);

});

client.once('close', () => {
    console.log('Connection bombareded.');
});

client.on('error', (err) => {
    console.error('Connection got caught in the trenches!!!!: ', err.message);
});


