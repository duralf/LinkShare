package LinkShare.AndroidShare;
import java.net.*;
import kotlin.concurrent.thread
val PROTOCOL_PORT = 31235


fun FindOnLAN() {
	thread() {
		val socket = MulticastSocket(31235)
		socket.send(DatagramPacket())
		println("Hello server!")
		
	};
}

fun main( args: Array<String> ) {
	FindOnLAN()
}
