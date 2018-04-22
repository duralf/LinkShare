package LinkShare.AndroidShare;
import java.net.*;
import java.io.*;
import kotlin.concurrent.thread
import java.nio.charset.StandardCharsets


val PROTOCOL_PORT = 31235
val BROADCAST = "255.255.255.255"
val UTF_8 = StandardCharsets.UTF_8
val socket = DatagramSocket(PROTOCOL_PORT)

fun MakeDatagramPacket(s: String): DatagramPacket {
	val b = s.toByteArray()
	return DatagramPacket(b, b.size, InetAddress.getByName(BROADCAST), PROTOCOL_PORT)
}

fun FindOnLAN(cb: (address: InetAddress) -> Unit) {
	thread() {
		socket.broadcast = true
		socket.send(MakeDatagramPacket("HELO"))
		try {
			socket.setSoTimeout(1600)
			while(true) {
				val pack = DatagramPacket(ByteArray(10), 6)
				socket.receive(pack)
				if( pack.getSocketAddress() == socket.getLocalSocketAddress() )
					continue
				if( String(pack.getData(),UTF_8) == "HELO!" ) {
					cb(pack.address)
				} else {
					println("Received unknown message!")
					println(String(pack.getData(),UTF_8))
				}

			}
		} catch( stoe: SocketTimeoutException ) {
		} catch( ioe: IOException ) {
			ioe.printStackTrace()
		}
		
	};
}

fun main( args: Array<String> ) {
	FindOnLAN( {
		address: InetAddress ->
		println("Hello server!")
	})
}
