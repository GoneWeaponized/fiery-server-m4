const net = require('net');

const PORT = 5010;
const HOST = '127.0.0.1';


const uuid = "b1968097-e099-4195-8b9d-d586d82d816a";
const idLength = Buffer.byteLength(uuid, 'utf8');
const packetSize = 2 + 1 + 8 + 8 + 4 + idLength;

const buf = Buffer.alloc(packetSize);
buf.writeUInt16BE(packetSize, 0);
buf.writeUInt8(1, 2);
buf.writeDoubleBE(54.2322178, 3);
buf.writeDoubleBE(3.894163, 11);
buf.writeInt32BE(idLength, 19);
buf.write(uuid, 23, 'utf8');



const client = new net.Socket();

client.connect(PORT, HOST, () => {
    console.log('Azimuth set to target, cannoning the pakket');
    client.on("data", buffer => {
        console.log(buffer.length);
        let offset = 0;

        while (offset < buffer.length) {

            const packetLength = buffer.readUInt16BE(offset);

            const packet = buffer.subarray(offset, offset + packetLength);
            const com = packet.readUInt8(2)
            const lat = packet.readDoubleBE(3);
            const lng = packet.readDoubleBE(11);
            const subId = packet.toString("utf8", 19, 35);

            const nameLength = packet.readUInt16BE(35);

            const username = packet.toString(
                "utf8",
                37,
                37 + nameLength
            );

            console.log("Player");
            console.log("------");
            console.log("Latitude :", lat);
            console.log("Longitude:", lng);
            console.log("SubID    :", subId);
            console.log("Username :", username);
            console.log("Type of command: ", com);
            offset += packetLength;
        }
    });
    client.write(buf);

});

client.once('close', () => {
    console.log('Connection bombareded.');
});

client.on('error', (err) => {
    console.error('Connection got caught in the trencehs!!!!: ', err.message);
});
