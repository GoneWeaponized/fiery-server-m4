const net = require('net');

const client = new net.Socket();
const PORT = 5010;
const HOST = '127.0.0.1';
const uuid = "0c7dc9aa-d6f1-4693-9e49-afbd6acdc368";
const packet = Buffer.alloc(uuid.length + 7);

packet.writeUInt16BE(packet.length, 0);
packet.writeUInt8(3, 2);
packet.writeUInt16BE(uuid.length, 3);
packet.write(uuid, 5, 'utf8');
packet.writeUInt16BE(0, 5 + uuid.length);

client.connect(PORT, HOST, () => {
    console.log('[TEST] Connected to server. Sending structure build packet...');
    client.write(packet);
});

// 4. Handle incoming response from the server
client.on('data', (data) => {
    console.log('[TEST] Received response buffer from server:', data);

    if (data.length === 3 && data[2] === 0x18) {
        console.log('[TEST] Result: Invalid distance (Error code 0x18 received).');
        client.destroy();
        return;
    }

    // Parse the response based on your server's layout (11 bytes expected)

    client.destroy();
});

client.on('close', () => {
    console.log('[TEST] Connection closed.');
});

client.on('error', (err) => {
    console.error('[TEST] Connection error:', err.message);
});
