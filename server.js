const net = require('net');
const readline = require('readline');
const krakey = require("./staff/krakey");  // Krakey is a parser caller
const playerHandler = require("./staff/playerHandler"); 
const spawnEvent = require('./ambient/spawnEvent'); //Ambient Event Spawning
const { spawn } = require('child_process');

const PORT = 5010;
const MAX_LENGTH = 65536;
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

const server = net.createServer(socket =>{

	let accumulator = Buffer.alloc(0);
	
    console.log("Client Connected.\n");
	
    socket.on("data", buffer => {
		
		accumulator = Buffer.concat([accumulator, buffer]);
		while (true) {
		   if (accumulator.length < 2) {break;}
		const hLength = accumulator.readUInt16BE(0);
		if (accumulator.length < hLength) {break;}
		const pakket = accumulator.subarray(0, hLength);
		accumulator = accumulator.subarray(hLength);
		krakey(socket, pakket);
		   }
    });

    socket.on("close", ()=>{
        console.log("\nClient Disconnected");
    });

    socket.on("error", err =>{
        console.log(err);
    });
});

rl.setPrompt("--- COMMANDS AVAIABLE: --- \nlist players \ninfo player <player_name>\n --- Type a command to use it (case sensitive)\n\n");
rl.prompt();
rl.on("line", line => {

    switch (line) {
        case ("" || " "): break;
        case "quit" : console.log("\n\nClosing the server."); playerHandler.savePlayers(); console.log("Quitting."); process.exit(); break;
        case "list players":
            playerHandler.listPlayers();
            break;

        case "help":
            console.log("Available commands...");
            break;
        default:
            if (line.startsWith("info player ")) {
                const playerName = line.substring("info player ".length);
                if (!playerHandler.findPlayerByName(playerName)) { console.log("Player Doesn't exists.\n"); break;}
                playerHandler.playerInfo(playerName);
                break;
            }
            if(line.startsWith("getmoney ")) {
                const uuid = line.substring("getmoney ".length);
                playerHandler.getMoney(uuid);
            }
            else {
                console.log("Unknown command.\n");
            break;
            }
        }
    });

    //interval for ambient event
setInterval(()=>{
    spawnEvent();
}, 200000);


server.listen(PORT, () =>{
    console.log(`TCP Server running on ${PORT} \n`);
});
