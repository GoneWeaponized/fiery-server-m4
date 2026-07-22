const net = require('net');

const client = new net.Socket();
const PORT = 5010;
const HOST = '127.0.0.1';
const uuid = "f1a52f93-76c5-4f52-ac8e-7475dcbe127c";
const packet = Buffer.alloc(uuid.length + 5);
packet.writeUInt16BE(packet.length, 0);
packet.writeUInt8(4, 2);
packet.writeUInt16BE(uuid.length, 3);
packet.write(uuid, 5, 'utf8');

client.connect(PORT, HOST, () => {
    console.log('[TEST] Connected to server. Sending resource request packet...');
    client.write(packet);
});

// 4. Handle incoming response from the server
client.on('data', (data) => {
    console.log('[TEST] Received response buffer from server:', data);

    if (data.length === 3 && data[2] === 0x06) {
        console.log('[TEST] Result: Player/Resources NOT FOUND (Error code 0x06 received).');
        client.destroy();
        return;
    }

    // Parse the response based on your server's layout (11 bytes expected)
    try {
        const resLength = data.readUInt16BE(0);
        const flag = data.readUInt8(2);
        const money = data.readUInt32BE(3);
        const manpower = data.readUInt32BE(7);

        console.log('--- Parsed Server Response ---');
        console.log(`Packet Length : ${resLength}`);
        console.log(`Flag          : ${flag}`);
        console.log(`Money         : ${money}`);
        console.log(`Manpower      : ${manpower}`);
    } catch (err) {
        console.error('[TEST] Failed to parse response packet:', err.message);
    }

    client.destroy();
});

client.on('close', () => {
    console.log('[TEST] Connection closed.');
});

client.on('error', (err) => {
    console.error('[TEST] Connection error:', err.message);
});
