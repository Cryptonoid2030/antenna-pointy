use serialport::SerialPort;
use std::io::{self,Read};
use std::time::Duration;

fn main() -> Result<(), Box<dyn std::error::Error>> {

    let port_name = "/dev/ttyACM0";
    let baud_rate = 19200;

    let mut port = serialport::new(port_name, baud_rate)
    .timeout(Duration::from_millis((1000)))
    .open()?;

    let mut buffer = vec![0; 100];

    loop {
        match port.read(&mut buffer) {
            Ok(n) => println!("Read: {}", String::from_utf8_lossy(&buffer[..n])),
            Err(ref e) if e.kind() == io::ErrorKind::TimedOut => (),
            Err(e) => eprintln!("Error: {:?}", e),
        }
        std::thread::sleep(Duration::from_millis(100));

    }

}
